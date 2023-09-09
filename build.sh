g++ marketDataStreamer.cpp FixParser.cpp -o marketDataStreamer -lzmq -O3
g++ algoEngine.cpp FixParser.cpp BidAskQueue.cpp POV.cpp -o algoEngine -lzmq -O3
g++ exchange.cpp FixParser.cpp BidAskQueue.cpp -o exchange -lzmq -O3
