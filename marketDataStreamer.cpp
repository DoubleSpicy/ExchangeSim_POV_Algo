// zmq
#include <string>
#include <zmq.hpp>
#include <signal.h>
#include <unistd.h>
#include <chrono>
#include <numeric>

// for reading .csv
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

template < typename Type > std::string to_str (const Type & t)
{
std::ostringstream os;
os << t;
return os.str ();
}

int main(int argc, char* argv[]){
    // ./marketDataStreamer ./data/market_data.csv
    // a.k.a. "Fake Bloomberg"
    // read csv
    // https://java2blog.com/read-csv-file-in-cpp/#:~:text=To%20read%20a%20CSV%20file%2C,variable%20as%20its%20second%20argument.
    std::string line, word;
    std::vector<std::vector<std::string>> content;
    std::vector<std::string> row;
    std::fstream file(argv[1], std::ios::in);
    std::cout << "Opening file: " << argv[1] << std::endl;
    while(std::getline(file, line)){
        row.clear();
        std::stringstream str(line);
        while(std::getline(str, word, ',')){
            row.push_back(word);
        }
        content.push_back(row);
    }

    // use in the following format:
    // .marketDataStreamer data.csv 
    // prepare the pub streaming socket
    zmq::context_t context (1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://127.0.0.1:5556"); // can be remapped using docker.
    usleep(200000); 
    zmq::message_t msg(1);
    std::stringstream ss;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    int rowIdx = 0;
    long long cnt = 0;
    while (rowIdx < content.size()) {
        // Send current clock (msecs) to subscribers
        end = std::chrono::steady_clock::now();
        if(rowIdx + 1 < content.size() && content[rowIdx][1] == content[rowIdx+1][1]) rowIdx++;
        if(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() + 5399999 - stoi(content[rowIdx][1]) >= 0){
            // std::cout << "TimeNow: " << std::chrono::time_point_cast<std::chrono::milliseconds>(end).time_since_epoch().count() << std::endl;
            std::string data = "";
            int cnt = 0;
            
            for(auto &str: content[rowIdx]){
                std::cout << str << " ";
                if(cnt == 1){
                    data += to_str(std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count()) + ";";
                }
                else{
                    data += str + ";";
                
                }
                cnt++;
            }
            std::cout << std::endl;
            msg.rebuild(data.size());
            memcpy(msg.data(), data.data(), data.size());
            std::cout << "sent: " << data << std::endl;
            publisher.send(msg);
            rowIdx++;
        }
    }
    return 0;
}