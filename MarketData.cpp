#include "MarketData.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

vector<PriceBar> CSVReader::loadCSV(const string& filepath) {
    vector<PriceBar> bars;
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Error: could not open file " << filepath << "\n";
        return bars;
    }

    string line;
    string cell;
    getline(file, line); // Skip header line
    while (getline(file, line)) {
        stringstream lineStream(line);
        PriceBar bar;
        getline(lineStream, bar.date, ',');
        getline(lineStream, cell, ',');
        bar.open = stod(cell);
        getline(lineStream, cell, ',');
        bar.high = stod(cell);
        getline(lineStream, cell, ',');
        bar.low = stod(cell);
        getline(lineStream, cell, ',');
        bar.close = stod(cell);
        getline(lineStream, cell, ','); // Skip adjusted close value
        getline(lineStream, cell, ',');
        bar.volume = stol(cell);

        bars.push_back(bar); // Add bar to the vector
    }
    file.close();
    return bars;
}