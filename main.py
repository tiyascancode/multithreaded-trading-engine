from engine import BacktestEngine
from strategies import MovingAverageCrossStrategy

if __name__ == "__main__":
    print("Initialising Encapsulated Backtest Run...")

    # 1. Instantiate reusable execution engine
    engine = BacktestEngine(csv_file="aapl_data_2023-01-01_to_2026-06-01.csv", initial_capital=10000.0, fee_rate=0.001, slippage_percentage=0.0005)

    # 2. Instantiate isolated strategy
    strategy = MovingAverageCrossStrategy(short_window=10, long_window=30)

    # 3. Run decoupled backtest simulation with injected strategy
    engine.run_simulation(strategy)