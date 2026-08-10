import pandas as pd
from metrics import PerformanceMetrics

class BacktestEngine:
    ''' Runs backtests (portfolio simulation, order execution, bookkeeping)
    -parameter csv_file: path to historical data CSV file
    -parameter capital: initial cash balance
    -parameter fee_rate: transaction fee rate as percentage
    -parameter slippage_percentage: slippage percentage
    '''
    def __init__(self, csv_file, initial_capital=10000.0, fee_rate=0.001, slippage_percentage=0.0005):
        self.csv_file = csv_file
        self.capital = initial_capital
        self.fee_rate = fee_rate
        self.slippage_percentage = slippage_percentage
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
                # Slippage: buy shares at slightly higher price than closing price
                execution_price = current_price * (1 + self.slippage_percentage)

                if cash >= execution_price:
                    # Determine how many shares we can afford
                    shares_to_buy = int(cash // execution_price)

                    if shares_to_buy > 0:
                        raw_stock_cost = shares_to_buy * execution_price
                        transaction_fee = raw_stock_cost * self.fee_rate

                        # Overall cost includes transaction fee
                        total_cost = raw_stock_cost + transaction_fee

                        # Check we can still afford shares with fee included
                        if cash >= total_cost:
                            shares_held += shares_to_buy
                            cash -= total_cost
                        else:
                            # Buy one less share if fee brings total cost too high
                            shares_to_buy -= 1
                            if shares_to_buy > 0:
                                raw_stock_cost = shares_to_buy * execution_price
                                transaction_fee = raw_stock_cost * self.fee_rate
                                shares_held += shares_to_buy
                                cash -= (raw_stock_cost + transaction_fee)
                        stock_value = shares_held * current_price
                        total_equity = cash + stock_value
                        print(f"{row['Date']} | BUY {shares_to_buy} shares at ${current_price:.2f} | Execution price: ${execution_price:.2f} | Fee: ${transaction_fee:.2f}| Cash: ${cash:.2f} | Stock Value: ${stock_value:.2f} | Total Equity: ${total_equity:.2f}\n")

            # Sell execution logic
            elif position_change == -2 and shares_held > 0:
                # Slippage: sell at slightly lower price than closing price
                execution_price = current_price * (1 - self.slippage_percentage)

                raw_sale_revenue = shares_held * execution_price
                transaction_fee = raw_sale_revenue * self.fee_rate

                # Cash gained is revenue minus the transaction fee
                net_cash_gained = raw_sale_revenue - transaction_fee
                cash += net_cash_gained
                total_equity = cash # shares_held is about to be 0
                print(f"{row['Date']} | SELL {shares_held} shares at ${current_price:.2f} | Execution price: ${execution_price:.2f} | Fee: ${transaction_fee:.2f} | Cash: ${cash:.2f} | Total Equity: ${total_equity:.2f}\n")
                shares_held = 0

            portfolio_values.append(cash + (shares_held * current_price))

        self.df['Total_Value'] = portfolio_values
        final_return = ((portfolio_values[-1] - self.capital) / self.capital) * 100

        # Risk metrics
        sharpe_ratio = PerformanceMetrics.calculate_sharpe_ratio(portfolio_values)
        max_drawdown = PerformanceMetrics.calculate_max_drawdown(portfolio_values)

        print("\n* BACKTEST RESULTS *")
        print(f"Initial Capital: ${self.capital:.2f}")
        print(f"Final Portfolio Value: ${portfolio_values[-1]:.2f}")
        print(f"Total Return: {final_return:.2f}%")
        print(f"Annualised Sharpe Ratio: {sharpe_ratio:.2f}")
        print(f"Maximum Drawdown: {max_drawdown:.2f}%")
