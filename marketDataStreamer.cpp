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

int main(int argc, char* argv[]){
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
        // std::cout << " sent: " << cnt << ": ";
        std::stringstream str(line);
        // if(line[0] == 'Q'){
        //     int idx = 0;
        //     while(std::getline(str, word, ',')){
        //         std::stringstream curLine(word);
        //         std::string temp;
        //         while(std::getline(curLine, temp, ' ')){
        //             row.push_back(temp);
        //         }
        //         idx++;
        //     }
        // }
        // if(line[0] == 'T'){
        //     while(std::getline(str, word, ',')){
        //         row.push_back(word);
        //     }
        // }

        content.push_back(row);
    }

    // for(int i = 0; i < content.size(); i++){
    //     std::cout << "row size: " << content[i].size();
    //     for(int j = 0; j < content[i].size(); j++){
    //         std::cout << content[i][j] << "|";
    //     }
    //     std::cout << std::endl;
    // }



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
        if(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() + 5399999 - stoi(content[rowIdx][1]) >= 0){
            std::cout << "TimeNow: " << std::chrono::time_point_cast<std::chrono::milliseconds>(end).time_since_epoch().count() << std::endl;
            std::string data = "";
            for(auto &str)
            // if(content[rowIdx][0] == "T"){
            //     std::string data = "T;" + content[rowIdx][2] + ";" + content[rowIdx][3] + ";";
            //     msg.rebuild(data.size());
            //     memcpy(msg.data(), data.data(), data.size());
            //     publisher.send(msg);
            //     std::cout << "cnt: " << cnt << " sent: " << data << std::endl;
            // }
            // else{
            //     std::string data = "";
            //     for(auto &s: content[rowIdx]){
            //         data += s + ";";
            //     }
            //     msg.rebuild(data.size());
            //     memcpy(msg.data(), data.data(), data.size());
            //     publisher.send(msg);
            //     std::cout << "cnt: " << cnt << " sent: " << data << std::endl;
            // }
            
            rowIdx++;
        }
    }
    return 0;
}