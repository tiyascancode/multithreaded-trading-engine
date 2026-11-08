#include <iomanip>
#include <iostream>
#include "Engine.h"

// Print full precision results for comparison with the Python engine
int main(int argc, char* argv[]) {
    if (argc != 7) return 1;
    auto data = CSVReader::loadCSV(argv[1]);
    MovingAverageCrossover strategy(stoi(argv[2]), stoi(argv[3]));
    auto signals = strategy.generateSignals(data);
    BacktestEngine engine(stod(argv[4]), stod(argv[5]), stod(argv[6]));
    vector<Trade> trades;
    auto equity = engine.runBacktest(data, signals, trades);
    cout << setprecision(17);
    for (size_t i = 0; i < data.size(); ++i) {
        cout << "ROW," << static_cast<int>(signals[i]) << "," << equity[i] << "\n";
    }
    for (const auto& trade : trades) {
        cout << "TRADE," << trade.date << "," << trade.type << "," << trade.price << ","
             << trade.shares << "," << trade.cost << "," << trade.fee << "\n";
    }
    double initial_capital = stod(argv[4]);
    double final_value = equity.empty() ? initial_capital : equity.back();
    cout << "METRICS," << (final_value - initial_capital) / initial_capital * 100.0
         << "," << PerformanceMetrics::calculateSharpeRatio(equity)
         << "," << PerformanceMetrics::calculateMaxDrawdown(equity) << "\n";
}
