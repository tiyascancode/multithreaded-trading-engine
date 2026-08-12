#include "Engine.h"
#include <cmath>
#include <iostream>

using namespace std;

BacktestEngine::BacktestEngine(double initial_cap, double comm, double slip)
    : initial_capital(initial_cap), commission(comm), slippage(slip) {}

vector<double> BacktestEngine::runBacktest(const vector<PriceBar>& bars, const vector<Signal>& signals, vector<Trade>& trades) {
    size_t n = bars.size();
    vector<double> portfolio_values(n, initial_capital);
    if (n==0) return portfolio_values; // Guard clause for empty bars
    double cash = initial_capital;
    double shares = 0.0;

    for (size_t i = 0; i < n; ++i) {
        double current_price = bars[i].close;

        // BUY signal
        if (signals[i] == Signal::BUY && cash > 0.0) {
            double execution_price = current_price * (1.0 + slippage); // Adjust for slippage
            double usable_cash = cash - commission;
            if (usable_cash > 0.0) {
                shares = usable_cash / execution_price;
                cash = 0.0; // All cash used to buy shares
                trades.push_back({bars[i].date, "BUY", execution_price, shares, usable_cash, commission});
            }
        }
        // SELL signal
        else if (signals[i] == Signal::SELL && shares > 0.0) {
            double execution_price = current_price * (1.0 - slippage);
            double revenue = shares * execution_price - commission;
            if (revenue > 0.0) {
                cash = revenue;
            } else cash = 0.0; // If revenue is negative after commission, set cash to zero
            trades.push_back({bars[i].date, "SELL", execution_price, shares, revenue, commission});
            shares = 0.0; // All shares sold
        }
        // Calculate total end-of-day equity
        portfolio_values[i] = cash + shares * current_price;
    }
    return portfolio_values;
}