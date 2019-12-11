#include <gtest/gtest.h>

#include "pimpl_on_stack/pimpl_on_stack.h"

TEST(Pimpl, First) {
  class C {
   public:
    C() { std::cout << "C constructor" << std::endl;}
    ~C() { std::cout << "C destructor" << std::endl;}
    int i = 0;
  };
  pimpl_on_stack::Pimpl<C, 4, 4> c;
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}