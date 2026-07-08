import pandas as pd

class BacktestEngine:
    ''' Runs backtests (portfolio simulation, order execution, bookkeeping)'''
    def __init__(self, csv_file, initial_capital=10000.0):
        self.csv_file = csv_file
        self.capital = initial_capital
        self.df = None

    def load_data(self):
        print(f"Loading market data from {self.csv_file}...")
        self.df = pd.read_csv(self.csv_file)
        self.df = self.df.sort_values('Date').reset_index(drop=True)

    def run_simulation(self, strategy):
        self.load_data()
        print("Running strategy simulation...\n")

        # Inject strategy dependency dynamically (Dependency Injection design patter! - implementation
        # of inversion of control principle)
        self.df = strategy.generate_signals(self.df)

        cash = self.capital
        shares_held = 0
        portfolio_values = []

        for index, row in self.df.iterrows():
            current_price = row['Close']
            position_change = row['Position']

            # Buy execution logic
            if position_change == 2 or (position_change == 1 and shares_held == 0):
                if cash >= current_price:
                    shares_to_buy = int(cash // current_price)
                    shares_held += shares_to_buy
                    cash -= shares_to_buy * current_price
                    stock_value = shares_held * current_price
                    total_equity = cash + stock_value
                    print(f"{row['Date']} | BUY {shares_to_buy} shares at ${current_price:.2f} | Cash: ${cash:.2f} | Stock Value: ${stock_value:.2f} | Total Equity: ${total_equity:.2f}\n")

            # Sell execution logic
            elif position_change == -2 and shares_held > 0:
                stock_value_before_sale = shares_held * current_price
                cash += stock_value_before_sale
                total_equity = cash 
                print(f"{row['Date']} | SELL {shares_held} shares at ${current_price:.2f} | Cash: ${cash:.2f} | Liquidated Value: ${stock_value_before_sale:.2f} | Total Equity: ${total_equity:.2f}\n")
                shares_held = 0

            portfolio_values.append(cash + (shares_held * current_price))

        self.df['Total_Value'] = portfolio_values
        final_return = ((portfolio_values[-1] - self.capital) / self.capital) * 100

        print("\n* BACKTEST RESULTS *")
        print(f"Initial Capital: ${self.capital:.2f}")
        print(f"Final Portfolio Value: ${portfolio_values[-1]:.2f}")
        print(f"Total Return: {final_return:.2f}%")
