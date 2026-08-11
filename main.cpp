#include <iostream>
#include "MarketData.h"
#include "Metrics.h"

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