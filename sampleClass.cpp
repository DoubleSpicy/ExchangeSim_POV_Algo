// A2DD.cpp
#include "sampleClass.h"


namespace TEST{
  A2Parent::A2Parent(int x, int y){
  gx = x;
  gy = y;
}
  void A2Parent::print(){
    std::cout << "gx: " << gx << " ,gy: " << gy << std::endl;
  }
};

// class A2Child: A2Parent{
//     int z;
//     public:
//       A2Child(int x, int y, int z);
//       void print();
// };