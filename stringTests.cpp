#include<string>
#include<iostream>
#include "FixParser.hpp"
using namespace FIX;
using namespace std;
// using namespace TEST;

int main(){
    // FIX::order test1();
    // "a123", "b456", 10.1, "MARKET"
    // , 987.65, "BUY", 0.05
    FIX::order test("38=10.1;35=a123;37=b456;");
    
    cout << test.to_string() << endl;
    FIX::order marketQuote("0", "0", 1234, "0", 7654, "0", 0, "BID", 0);
    cout << marketQuote.to_string() << endl;
}