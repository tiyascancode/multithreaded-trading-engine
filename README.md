# Quantitative Backtesting Engine (C++17 / Python)

A modular, high-performance execution engine built to backtest quantitative trading strategies on historical market data. Originally prototyped in Python, the execution core was ported to modern C++ to achieve significant performance improvements.

## Performance Benchmark

Execution speed comparison for running a full strategy backtest across historical daily price data (averaged over multiple runs):

| Implementation | Average Execution Time | Speedup Factor |
| :--- | :--- | :--- |
| **Python** | ~102.5 ms (102,498 µs) | Baseline (1x) |
| **C++ (GCC `-O3`)** | **~2.95 ms** (2,958 µs) | **~35x Faster** |

*Timings measured using `std::chrono` (C++) and `time.perf_counter()` (Python).*

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