#ifndef STRATEGY_H
#define STRATEGY_H
#endif

#include <vector>
#include "MarketData.h"

using namespace std;

enum class Signal {
    BUY = 1,
    SELL = -1,
    HOLD = 0
};

class MovingAverageCrossover {
    private:
        int short_window;
        int long_window;

    public:
        MovingAverageCrossover(int short_window = 10, int long_window = 30);
        vector<double> calculateSMA(const vector<double>& prices, int window) const; // Cannot modify variables 
        vector<Signal> generateSignals(const vector<PriceBar>& bars) const; // Generates position signals based on SMA crossovers
};