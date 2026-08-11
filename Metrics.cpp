#include "Metrics.h"
#include <algorithm>
#include <cmath> // Standard maths operations
#include <numeric> // For std::accumulate - fast array algorithms

using namespace std;

double PerformanceMetrics::calculateSharpeRatio(const vector<double>& portfolio_values,
                                                double risk_free_rate,
                                                int periods_per_year) {
    if (portfolio_values.size() < 2) return 0.0; // Need at least 2 daily equity values to calculate percentage return

    // Calculate daily percentage returns
    vector<double> daily_returns;
    daily_returns.reserve(portfolio_values.size() - 1); // Pre-allocates space in memory!
    for (size_t i = 1; i < portfolio_values.size(); ++i) {
        double ret = (portfolio_values[i] - portfolio_values[i - 1]) / portfolio_values[i - 1]; // Standard percentage change
        daily_returns.push_back(ret);
    }

    // Calculate mean daily return
    double sum = accumulate(daily_returns.begin(), daily_returns.end(), 0.0); // Iterates from start to end, summing every number, starting from 0.0
    double mean_return = sum / daily_returns.size();

    // Calculate daily return standard deviation - we want to know how much the returns deviate from the mean return, which is a measure of risk.
    double variance_sum = 0.0;
    for (double r : daily_returns) {
        variance_sum += (r - mean_return) * (r - mean_return); // Sum of squared deviations from the mean
    }

    // For sample standard deviation, divide by (n-1)
    double std_dev = sqrt(variance_sum / (daily_returns.size() - 1));
    if (std_dev == 0.0) return 0.0; // Avoid division by zero

    // Annualise Sharpe Ratio - (Excess Return / Standard Deviation) * sqrt(periods_per_year)
    double excess_return = risk_free_rate / periods_per_year;
    double sharpe_ratio = (mean_return - excess_return) / std_dev * sqrt(periods_per_year);

    return sharpe_ratio;
                                                }

double PerformanceMetrics::calculateMaxDrawdown(const vector<double>& portfolio_values) {
    if (portfolio_values.empty()) return 0.0; // Guard clause for empty portfolio values
    double max_drawdown = 0.0;
    double running_peak = portfolio_values[0]; // Initialise running peak to Day 1 value
    
    // Track running peak and maximum negative deviation
    for (double current_value : portfolio_values) {
        if (current_value > running_peak) {
            running_peak = current_value;
        }
        double drawdown = (current_value - running_peak) / running_peak; // Calculate drawdown as a percentage
        if (drawdown < max_drawdown) {
            max_drawdown = drawdown; // Update max drawdown if current drawdown is worse
        }
    }
    return max_drawdown * 100.0;  
}