#include <iostream>
#include <chrono> // For timing
#include "MarketData.h"
#include "Metrics.h"
#include "Strategy.h"
#include "Engine.h"

using namespace std;

int main() {
    // Check CSV file path and load data
    string filepath = "aapl_data_2023-01-01_to_2026-06-01.csv";
    cout << "Loading market data from " << filepath << "...\n";

    auto start = chrono::high_resolution_clock::now(); // Start clock for timing how long C++ takes

    vector<PriceBar> data = CSVReader::loadCSV(filepath); // Calls CSVReader class to loop through file and create PriceBar for each row.
    cout << "Loaded " << data.size() << " price bars.\n";

    // Generate signals using Moving Average Crossover strategy
    MovingAverageCrossover strategy(10, 30);
    vector<Signal> signals = strategy.generateSignals(data);

    int buy_count = 0;
    int sell_count = 0;
    for (size_t i = 0; i < signals.size(); ++i) {
        if (signals[i] == Signal::BUY) {
            buy_count++;
            // cout << "Buy signal on " << data[i].date << " at price $" << data[i].close << "\n";
        }
        else if (signals[i] == Signal::SELL) {
            sell_count++;
            // cout << "Sell signal on " << data[i].date << " at price $" << data[i].close << "\n";
        }
    }

    // Run backtest using BacktestEngine
    BacktestEngine engine(10000.0, 0.001, 0.0005); // Initial capital $10,000, fee rate 0.1%, slippage 0.05%
    vector<Trade> trades;
    vector<double> strategy_equity = engine.runBacktest(data, signals, trades);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, std::micro> elapsed = end - start;
    cout << "\nC++ execution time: " << elapsed.count() << " microseconds";

    if (!data.empty()) {
        cout << "\nLoaded " << data.size() << " price bars. \n";
    }

    cout << "\nTotal buy signals: " << buy_count << "\n";
    cout << "Total sell signals: " << sell_count << "\n";

    // Calculate STrategy performance metrics
    double strategy_sharpe_ratio = PerformanceMetrics::calculateSharpeRatio(strategy_equity);
    double strategy_max_drawdown = PerformanceMetrics::calculateMaxDrawdown(strategy_equity);
    double final_strategy_value = strategy_equity.back();

    // Calculate metrics on Buy-and-Hold strategy - buy all shares at the start and just hold onto them
    // vector<double> close_prices;
    // close_prices.reserve(data.size());
    // for (const auto&bar : data) {
        // close_prices.push_back(bar.close);
    // }    
    // double bh_sharpe_ratio = PerformanceMetrics::calculateSharpeRatio(close_prices);
    // double bh_max_drawdown = PerformanceMetrics::calculateMaxDrawdown(close_prices);
    // double bh_return = (data.back().close - data.front().close) / data.front().close * 100.0;

    // Performance Summary
    cout << "\n===================================\n";
    cout << "       BACKTEST ENGINE RESULTS       \n";
    cout << "=====================================\n";
    cout << "Initial capital:        $10,000.00\n";
    cout << "Final strategy equity: " << final_strategy_value << "\n"; // Monetary value of portfolio at end of backtest
    cout << "Strategy total return: " << (final_strategy_value - 10000.0) / 10000.0 * 100.0 << "\n";
    cout << "Total trades executed: " << trades.size() << "\n";
    cout << "-------------------------------------\n";
    cout << "Strategy Sharpe Ratio: " << strategy_sharpe_ratio << "\n";
    cout << "Strategy Max Drawdown: " << strategy_max_drawdown << "\n";
    cout << "\nTrade log:\n";
    for (const auto& t : trades) {
        cout << " " << t.date << " " << t.type << " " << t.shares << " @ $" << t.price << "fee = $" << t.fee << "\n";
    }
    // cout << "Buy & Hold return:     " << bh_return << "\n";
    // cout << "Buy & Hold Sharpe Ratio: "<< bh_sharpe_ratio << "\n";
    // cout << "Buy & Hold Max Drawdown: " << bh_max_drawdown << "\n";
    return 0;
}