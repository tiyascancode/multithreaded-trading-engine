#ifndef METRICS_H
#define METRICS_H
#endif

#include <vector>

using namespace std;

class PerformanceMetrics {
    public:
        // Calculates annualised Sharpe Ratio from daily portfolio total equity values.
        static double calculateSharpeRatio(const vector<double>& portfolio_values,
                                           double risk_free_rate = 0.0,
                                           int periods_per_year = 252);
        
        // Calculates Maximum Drawdown from largest peak between lowest value and highest value in portfolio total equity values.
        static double calculateMaxDrawdown(const vector<double>& portfolio_values);
};