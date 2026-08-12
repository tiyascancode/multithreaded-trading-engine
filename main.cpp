#include <iostream>
#include "MarketData.h"
#include "Metrics.h"
#include "Strategy.h"

using namespace std;

int main() {
    string filepath = "aapl_data_2023-01-01_to_2026-06-01.csv";
    cout << "Loading market data from " << filepath << "...\n";
    vector<PriceBar> data = CSVReader::loadCSV(filepath); // Calls CSVReader class to loop through file and create PriceBar for each row.
    cout << "Loaded " << data.size() << " price bars.\n";
    if (!data.empty()) {
        cout << "Successfully parsed " << data.size() << " price bars from the CSV file.\n";
        cout << "First bar: Date: " << data.front().date << " | Close: $" << data.front().close << "\n";
        cout << "Last bar: Date: " << data.back().date << " | Close: $" << data.back().close << "\n";
    }
    MovingAverageCrossover strategy(10, 30);
    vector<Signal> signals = strategy.generateSignals(data);
    int buy_count = 0;
    int sell_count = 0;
    for (size_t i = 0; i < signals.size(); ++i) {
        if (signals[i] == Signal::BUY) {
            buy_count++;
            cout << "Buy signal on " << data[i].date << " at price $" << data[i].close << "\n";
        }
        else if (signals[i] == Signal::SELL) {
            sell_count++;
            cout << "Sell signal on " << data[i].date << " at price $" << data[i].close << "\n";
        }
    }
    cout << "\nTotal buy signals: " << buy_count << "\n";
    cout << "Total sell signals: " << sell_count << "\n";

    vector<double> close_prices;
    close_prices.reserve(data.size());
    for (const auto&bar : data) {
        close_prices.push_back(bar.close);
    }    
    double sharpe_ratio = PerformanceMetrics::calculateSharpeRatio(close_prices);
    double max_drawdown = PerformanceMetrics::calculateMaxDrawdown(close_prices);
    cout << "Sharpe Ratio: " << sharpe_ratio << "\n";
    cout << "Maximum Drawdown: " << max_drawdown << "%\n";
    return 0;
}