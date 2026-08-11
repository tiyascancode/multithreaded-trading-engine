#include <iostream>
#include "MarketData.h"

using namespace std;

int main() {
    string filepath = "aapl_data_2023-01-01_to_2026-06-01.csv";
    cout << "Loading market data from " << filepath << "...\n";
    vector<PriceBar> data = CSVReader::loadCSV(filepath);
    cout << "Loaded " << data.size() << " price bars.\n";
    if (!data.empty()) {
        cout << "Successfully parsed " << data.size() << " price bars from the CSV file.\n";
        cout << "First bar: Date: " << data.front().date << " | Close: $" << data.front().close << "\n";
        cout << "Last bar: Date: " << data.back().date << " | Close: $" << data.back().close << "\n";
    }
    return 0;
}