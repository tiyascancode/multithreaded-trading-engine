/* Include guards, used to prevent the compiler from 
   including the header file more than once. Can also
   use #pragma once */
#ifndef MARKETDATA_H // If not defined
#define MARKETDATA_H // Define it

#include <string>
#include <vector>

using namespace std; // Use the standard namespace 

// Data structure for single daily bar/candle
// Only date and close are stored to match Python code
struct PriceBar {
    string date;
    double close;
};

class CSVReader {
    public:
        static vector<PriceBar> loadCSV(const string&filepath);
};
#endif // End of ifndef

