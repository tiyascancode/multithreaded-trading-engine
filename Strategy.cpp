#include "Strategy.h"
#include <numeric> // Numerical algorithms for collections of numbers
#include <stdexcept> // Standard error exceptions like runtime error

using namespace std;

MovingAverageCrossover::MovingAverageCrossover(int short_window, int long_window)
    : short_window(short_window), long_window(long_window) {
        if (short_window >= long_window) {
            throw invalid_argument("Short window must be strictly less than long window.");
        }
    }
vector<double> MovingAverageCrossover::calculateSMA(const vector<double>& prices, int window) const {
    vector<double> sma(prices.size(), 0.0);
    if (prices.empty()) {
        return sma; 
    }
    // Calculate initial rolling window sum
    double running_sum = 0.0;
    for (size_t i = 0; i < prices.size(); ++i) {
        running_sum += prices[i];
        if (i < static_cast<size_t>(window)) {
            sma[i] = running_sum / static_cast<double>(i + 1);
        } else {
            running_sum -= prices[i - window]; // Drop the oldest price leaving window
            sma[i] = running_sum / window;
        }
    }
    return sma;
}

vector<Signal> MovingAverageCrossover::generateSignals(const vector<PriceBar>& bars) const {
    size_t n = bars.size();
    vector<Signal> signals(n, Signal::HOLD);

    if (n < 2) {
        return signals; // Not enough data to generate signals
    }

    // Extract closing prices from PriceBar objects
    vector<double> close_prices;
    close_prices.reserve(n);
    for (const auto& bar : bars) {
        close_prices.push_back(bar.close);
    }

    // Calculate short-term and long-term SMAs
    vector<double> short_sma = calculateSMA(close_prices, short_window);
    vector<double> long_sma = calculateSMA(close_prices, long_window);

    int current_position = 0; // 0 = no position, 1 = long, -1 = short

    for (size_t i = 1; i < n; ++i) { // size_t is an unsigned integer type used for sizes and arrays/vectors - starts at 0
        // Golden cross - short SMA crosses above long SMA
        if (short_sma[i] > long_sma[i] && short_sma[i - 1] <= long_sma[i - 1]) {
            if (current_position != 1) { // Only buy if current position not already long
                signals[i] = Signal::BUY;
                current_position = 1;
            }
        }
        // Death cross - short SMA crosses below long SMA
        else if (short_sma[i] <= long_sma[i] && short_sma[i - 1] > long_sma[i - 1]) {
            if (current_position != -1) { // Only sell if current position not already short
                signals[i] = Signal::SELL;
                current_position = -1;
            }
        }
    }
    return signals;
}