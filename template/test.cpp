#include <bits/stdc++.h>
using namespace std;

template <class T> class caller{
    public:
        T obj;
        caller(const T & obj): obj(obj){};
        void callShout(){
            obj.shout();
        }
};

class shouter {
    public:
        void shout(){
            cout << "hi!\n";
        }
};


int main(){
    shouter member;
    caller<shouter> test(member);
    test.callShout();
}