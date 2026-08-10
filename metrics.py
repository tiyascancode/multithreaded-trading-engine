import numpy as np
import pandas as pd

class PerformanceMetrics:
    '''Utility class for evaluating risk-adjusted performance metrics.'''
    @staticmethod
    def calculate_sharpe_ratio(portfolio_values, risk_free_rate=0.0, periods_per_year=252):
        '''Calculates annualised Sharpe Ratio based on daily portfolio returns.'''
        equity_series = pd.Series(portfolio_values)
        daily_returns = equity_series.pct_change().dropna() # Vectorised operation applied to whole data list
        if daily_returns.std() == 0:
            return 0.0
        
        # Excess returns over risk-free rate per period
        mean_daily_return = daily_returns.mean() - (risk_free_rate / periods_per_year)
        std_daily_return = daily_returns.std()

        # Annualise the daily ratio by multiplying by sqrt(252 trading days)
        sharpe_ratio = (mean_daily_return / std_daily_return) * np.sqrt(periods_per_year)
        return sharpe_ratio
    
    @staticmethod
    def calculate_max_drawdown(portfolio_values):
        '''Calculates Max Drawdown (MDD) percentage from largest peak-to-trough drop in value of investment portfolio.'''
        equity_series = pd.Series(portfolio_values)
        running_max = equity_series.cummax() # Rolling peak across timeline, another vectorised operation
        drawdowns = (equity_series - running_max) / running_max # Percentage drawdown from rolling peak
        max_drawdown = drawdowns.min() * 100
        return max_drawdown