# Quantitative Backtesting Engine (C++17 / Python)

A modular, high-performance execution engine built to backtest quantitative trading strategies on historical market data. Originally prototyped in Python, the execution core was ported to modern C++ to achieve significant performance improvements.

## Performance Benchmark

Both timers now include CSV loading, strategy setup and signal generation, portfolio simulation, and performance metric calculations. Console output is outside the timed region (Python runs with `verbose=False`). C++ also records its trade log during simulation. Use the same chronological CSV and parameters for both programs, build C++ with `g++ -std=c++17 -O3 main.cpp Engine.cpp MarketData.cpp Metrics.cpp Strategy.cpp -o engine`, and repeat runs before reporting an updated comparison. These are full pipeline timings, not isolated execution-loop timings.

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