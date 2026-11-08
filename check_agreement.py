import argparse
import csv
import math
from pathlib import Path
import random
import subprocess
import tempfile

import numpy as np

from engine import BacktestEngine
from strategies import MovingAverageCrossStrategy


def check_results(csv_file, executable, short_window=10, long_window=30,
                  capital=10000.0, fee=0.001, slippage=0.0005):
    engine = BacktestEngine(str(csv_file), capital, fee, slippage, verbose=False)
    result = engine.run_simulation(MovingAverageCrossStrategy(short_window, long_window))
    output = subprocess.run([str(executable), str(csv_file), str(short_window), str(long_window),
                             str(capital), str(fee), str(slippage)],
                            capture_output=True, text=True, check=True).stdout
    rows, trades, metrics = [], [], []
    for line in csv.reader(output.splitlines()):
        if line[0] == 'ROW':
            rows.append((int(line[1]), float(line[2])))
        elif line[0] == 'TRADE':
            trades.append((line[1], line[2], *map(float, line[3:])))
        elif line[0] == 'METRICS':
            metrics = list(map(float, line[1:]))

    positions = engine.df['Position'].fillna(0).to_numpy()
    signals = np.where(positions > 0, 1, np.where(positions < 0, -1, 0))
    assert [row[0] for row in rows] == list(signals), 'Signal mismatch'
    np.testing.assert_allclose([row[1] for row in rows], result['equity'], rtol=1e-10, atol=1e-8)
    assert len(trades) == len(result['trades']), 'Trade count mismatch'
    for cpp_trade, python_trade in zip(trades, result['trades']):
        assert cpp_trade[:2] == python_trade[:2], 'Trade date or type mismatch'
        assert cpp_trade[3] == python_trade[3], 'Share quantity mismatch'
        np.testing.assert_allclose(cpp_trade[2:], python_trade[2:], rtol=1e-10, atol=1e-8)
    np.testing.assert_allclose(metrics, [result['return'], result['sharpe'], result['drawdown']], rtol=1e-9, atol=1e-8)
    assert all(math.isfinite(value) for value in metrics)

    # Independently check that every purchase is affordable, including fees.
    cash, shares = capital, 0
    for date, kind, price, quantity, cost, charge in result['trades']:
        assert quantity > 0 and quantity == int(quantity)
        assert math.isclose(cost, quantity * price, rel_tol=1e-12)
        assert math.isclose(charge, cost * fee, rel_tol=1e-12, abs_tol=1e-12)
        if kind == 'BUY':
            assert cost + charge <= cash, 'Purchase exceeds available cash'
            extra_share_cost = (quantity + 1) * price * (1 + fee)
            assert extra_share_cost > cash or math.isclose(extra_share_cost, cash, rel_tol=1e-14), 'Purchase left an affordable whole share unbought'
            cash -= cost + charge
            shares += quantity
        else:
            assert quantity == shares
            cash += cost - charge
            shares = 0
        assert cash >= 0, 'Cash became negative'
    return len(rows), len(trades)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('executable', type=Path)
    parser.add_argument('--csv', type=Path, action='append', default=[])
    args = parser.parse_args()
    executable = args.executable.resolve()
    passed = 0
    with tempfile.TemporaryDirectory(dir=Path(__file__).parent) as directory:
        path = Path(directory) / 'prices.csv'
        rng = random.Random(42)
        datasets = [[], [100], [100, 110], [100] * 100,
                    list(range(100, 200)), list(range(200, 100, -1)),
                    [100, 100, 101, 99] * 40,
                    [100 + 20 * math.sin(i / 7) for i in range(200)]]
        datasets += [[round(100 + rng.uniform(-30, 30), 6) for _ in range(200)] for _ in range(20)]
        for prices in datasets:
            with path.open('w', newline='') as stream:
                writer = csv.writer(stream)
                writer.writerow(['Date', 'Close'])
                writer.writerows((f'2023-{i // 28 + 1:02d}-{i % 28 + 1:02d}', price) for i, price in enumerate(prices))
            for short, long in [(2, 5), (10, 30)]:
                for capital, fee, slip in [(10000, 0.001, 0.0005), (1000000, 0.05, 0), (10, 0.1, 0.01), (10000, 0, 0)]:
                    try:
                        check_results(path, executable, short, long, capital, fee, slip)
                    except Exception:
                        print('FAILED', datasets.index(prices), short, long, capital, fee, slip)
                        raise
                    passed += 1
        for source in args.csv:
            count, trades = check_results(source.resolve(), executable)
            print(f'PASS {source.name}: {count} rows, {trades} trades')
            passed += 1
            check_results(source.resolve(), executable, capital=1000000, fee=0.05)
            passed += 1
            # Check that both loaders handle unsorted source data consistently.
            with source.open() as stream:
                lines = list(csv.reader(stream))
            with path.open('w', newline='') as stream:
                csv.writer(stream).writerows([lines[0], *reversed(lines[1:])])
            check_results(path, executable)
            passed += 1
    print(f'PASS {passed} agreement cases: signals, every trade, equity curve, return, Sharpe and drawdown')
