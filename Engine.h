#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <string>
#include "MarketData.h"
#include "Metrics.h"
#include "Strategy.h"

using namespace std;

struct Trade {
    string date;
    string type; // "BUY" or "SELL"
    double price;
    double shares;
    double cost;
    double fee;
};

class BacktestEngine {
    private:
        double initial_capital;
        double fee_rate; // Transaction fee as percentage of trade value
        double slippage; // Slippage as a percentage of trade price

    public:
        BacktestEngine(double initial_cap = 10000.0, double fee_rate = 0.001, double slip = 0.0005);
        vector<double> runBacktest(const vector<PriceBar>& bars, const vector<Signal>& signals, vector<Trade>& trades);
};
#endif