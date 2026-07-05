import yfinance as yf
import pandas as pd
import os

def download_data(ticker, start_date, end_date):
    print(f"Downloading data for {ticker} from {start_date} to {end_date}")
    data = yf.download(ticker, start=start_date, end=end_date, multi_level_index=False) # Fetch historical data from Yahoo Finance

    if data.empty:
        print(f"No data found for {ticker} between {start_date} and {end_date}.")
        return None
    
    # Clean dataset to keep only Closing Price
    cleaned_data = data[['Close']].copy()

    # Save the cleaned data to a CSV file
    filename = f"{ticker.lower()}_data_{start_date}_to_{end_date}.csv"
    cleaned_data.to_csv(filename)
    print(f"Successfully saved to {filename}\n")

if __name__ == "__main__":
    # Get last three years of data
    download_data(ticker="AAPL", start_date="2023-01-01", end_date="2026-06-01")
    download_data(ticker="BTC-USD", start_date="2023-01-01", end_date="2026-06-01")
    print(os.getcwd())