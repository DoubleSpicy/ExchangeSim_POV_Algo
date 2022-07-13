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
    cout << test.FIX::order::convertToString() << endl;
}