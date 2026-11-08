#include "MarketData.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

vector<PriceBar> CSVReader::loadCSV(const string& filepath) {
    vector<PriceBar> bars;
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Error: could not open file " << filepath << "\n";
        return bars;
    }

    string line;
    getline(file, line); // Skip header line
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream lineStream(line);
        PriceBar bar;
        string cell;
        getline(lineStream, bar.date, ',');
        getline(lineStream, cell, ',');
        if (cell.empty()) continue;
        bar.close = stod(cell);
        bars.push_back(bar); // Add bar to the vector
    }
    file.close();
    // Match Python by processing prices in date order
    stable_sort(bars.begin(), bars.end(), [](const PriceBar& a, const PriceBar& b) {
        return a.date < b.date;
    });
    return bars;
}
