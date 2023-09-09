echo "starting the simulator..."

./exchange > exchange.log & ./algoEngine > algoEngine.log & ./marketDataStreamer ./data/market_data.csv > marketDataStreamer.log
