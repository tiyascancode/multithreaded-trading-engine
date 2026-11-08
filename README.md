# Quantitative Backtesting Engine (C++17 / Python)

A modular, high-performance execution engine built to backtest quantitative trading strategies on historical market data. Originally prototyped in Python, the execution core was ported to modern C++ to achieve significant performance improvements.

## Performance Benchmark

Both timers include CSV loading, strategy setup and signal generation, portfolio simulation, trade recording, and performance metric calculations. Console output and process startup/imports are outside the timed region (Python runs with `verbose=False`). These are full pipeline timings, not isolated execution-loop timings.

Results on the original AAPL dataset (854 daily prices), using 10/30 SMA windows, $10,000 initial capital, 0.1% fees and 0.05% slippage:

| Implementation | Median execution time | Speedup |
| :--- | :--- | :--- |
| Python | 65.180 ms | 1x |
| C++ (GCC `-O3`) | 1.987 ms | 32.8x |

Measured on 8 November 2026 on Windows 11 with an AMD Ryzen 5 7530U, Python 3.12.14, pandas 3.0.1, NumPy 2.3.5 and GCC 16.1.0 (`-std=c++17 -O3 -static`). Each implementation had 3 warm-up runs and 21 measured runs, alternating execution order. Speedup is the ratio of median times on this machine and dataset. Full measurements and the input checksum are saved in `benchmark_results.json`.

*Timings measured using `std::chrono` (C++) and `time.perf_counter()` (Python).*

### Checking Python and C++ agreement

Build the comparison program and run the checks with the same Python environment used for the project:

```bash
g++ -std=c++17 -O3 check_results.cpp Engine.cpp MarketData.cpp Metrics.cpp Strategy.cpp -o check_results
python check_agreement.py ./check_results --csv aapl_data_2023-01-01_to_2026-06-01.csv
```

The checks compare signals, trade dates and types, whole-share quantities, execution prices, fees, the full equity curve, total return, Sharpe ratio and maximum drawdown. They also independently check that purchases remain affordable. Generated cases cover empty/short inputs, flat and changing prices, random prices, high fees and insufficient cash. Monetary comparisons use a small floating-point tolerance; dates, signal values and share counts must match exactly.

### Running the benchmark

```bash
g++ -std=c++17 -O3 main.cpp Engine.cpp MarketData.cpp Metrics.cpp Strategy.cpp -o engine
python benchmark.py aapl_data_2023-01-01_to_2026-06-01.csv ./engine --runs 21
```

On Windows, use the `.exe` filenames and add `-static` to match the recorded build. The benchmark copies the supplied CSV to a temporary directory and leaves the source file unchanged.

---

## Key Features

* **Execution Friction:** Models percentage transaction fee rates (`fee_rate`) and execution slippage to simulate realistic fills and accurate equity tracking.
* **Risk & Performance Metrics:** Calculates performance analytics including active strategy returns, annualized Sharpe ratio, and maximum drawdown (MDD).
* **Decoupled Architecture:** Clean separation between data parsing, strategy logic, execution loop, and performance reporting.

---

## 🛠 Tech Stack

* **Languages:** C++17, Python 3
* **Build/Compiler:** GCC (`g++ -O3`)
* **Tooling & Version Control:** Git, GitHub (Feature Branching, PR Workflow)
