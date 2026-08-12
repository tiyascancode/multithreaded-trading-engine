#include "Engine.h"
#include <cmath>
#include <iostream>

using namespace std;

BacktestEngine::BacktestEngine(double initial_cap, double fee_rate, double slip)
    : initial_capital(initial_cap), fee_rate(fee_rate), slippage(slip) {}

vector<double> BacktestEngine::runBacktest(const vector<PriceBar>& bars, const vector<Signal>& signals, vector<Trade>& trades) {
    size_t n = bars.size();
    vector<double> portfolio_values(n, initial_capital);
    if (n==0) return portfolio_values; // Guard clause for empty bars
    double cash = initial_capital;
    long long shares_held = 0.0; // Whole shares only

    for (size_t i = 0; i < n; ++i) {
        double current_price = bars[i].close;

        // BUY signal
        if (signals[i] == Signal::BUY) {
            double execution_price = current_price * (1.0 + slippage); // Adjust for slippage
            if (cash >= execution_price) {
                long long shares_to_buy = static_cast<long long>(cash / execution_price);
                if (shares_to_buy > 0) {
                    double raw_stock_cost = shares_to_buy * execution_price;
                    double transaction_fee = raw_stock_cost * fee_rate;
                    double total_cost = raw_stock_cost + transaction_fee;
                    if (cash >= total_cost) {
                        shares_held += shares_to_buy;
                        cash -= total_cost;
                    } else {
                        shares_to_buy -= 1; // Buy one less share if fee brings total cost too high
                        if (shares_to_buy > 0) {
                            raw_stock_cost = shares_to_buy * execution_price;
                            transaction_fee = raw_stock_cost * fee_rate;
                            shares_held += shares_to_buy;
                            cash -= (raw_stock_cost * transaction_fee);
                        }
                    }
                    if (shares_to_buy > 0) {
                        trades.push_back({bars[i].date, "BUY", execution_price, static_cast<double>(shares_to_buy), raw_stock_cost, transaction_fee});
                    }
                }
            }
        }
        // SELL signal
        else if (signals[i] == Signal::SELL && shares_held > 0.0) {
            double execution_price = current_price * (1.0 - slippage);
            double raw_sale_revenue = shares_held * execution_price;
            double transaction_fee = raw_sale_revenue * fee_rate;
            double net_cash_gained = raw_sale_revenue - transaction_fee;
            cash += net_cash_gained;
            trades.push_back({bars[i].date, "SELL", execution_price, static_cast<double>(shares_held), raw_sale_revenue, transaction_fee});
            shares_held = 0; // All shares sold
        }
        // Calculate total end-of-day equity
        portfolio_values[i] = cash + shares_held * current_price;
    }
    return portfolio_values;
}