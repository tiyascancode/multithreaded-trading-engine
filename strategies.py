# Follows Strategy design pattern - all strategies encapsulated and interchangeable

import pandas as pd

class MovingAverageCrossStrategy:
    ''' Simple Moving Average (SMA) Crossover Strategy - Buy when fast SMA crosses above slow SMA,
        sell when fast SMA crosses below slow SMA.'''
    def __init__(self, short_window=10, long_window=30):
        self.short_window = short_window
        self.long_window = long_window

    def generate_signals(self, df):
        #Calculate both moving averages (fast SMA and slow SMA)
        df['Short_SMA'] = df['Close'].rolling(window=self.short_window, min_periods=1).mean()
        df['Long_SMA'] = df['Close'].rolling(window=self.long_window, min_periods=1).mean()

        # Create a 'Signal' column: 1 for buy (bullish), -1 for sell (bearish), 0 for hold (neutral)
        df['Signal'] = 0

        # Generate signals when the short SMA crosses the long SMA
        df.loc[df['Short_SMA'] > df['Long_SMA'], 'Signal'] = 1 # Buy signal
        df.loc[df['Short_SMA'] <= df['Long_SMA'], 'Signal'] = -1 # Sell signal

        # Track exact moment the signal changes and of crossovers
        df['Position'] = df['Signal'].diff()
        return df