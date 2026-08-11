/* Include guards, used to prevent the compiler from 
   including the header file more than once. Can also
   use #pragma once */
#ifndef MARKETDATA_H // If not defined
#define MARKETDATA_H // Define it
#endif // End of ifndef

#include <string>
#include <vector>

using namespace std; // Use the standard namespace 

// Data structure for single daily bar/candle
struct PriceBar {
    string date; 
    double open;
    double high;
    double low;
    double close;
    long volume;
};

class CSVReader {
    public:
        static vector<PriceBar> loadCSV(const string&filepath);
};


