#include <gtest/gtest.h>

#include "pimpl_on_stack/pimpl_on_stack.h"

TEST(Pimpl, Constructor_destructor) {
  bool constructor_used = false;
  bool destructor_used = false;

  class C {
   public:
    C(bool* constructor_used, bool* destructor_used)
        : destructor_used_(destructor_used) {
      *constructor_used = true;
    }
    ~C() { *destructor_used_ = true; }

   private:
    bool* destructor_used_;
  };

  { pimpl_on_stack::Pimpl<C, 8, 8> c(&constructor_used, &destructor_used); }
  EXPECT_TRUE(constructor_used);
  EXPECT_TRUE(destructor_used);
}

TEST(Pimpl, Default_constructor) {
  constexpr int kValue = 123;

  struct S {
    int i = kValue;
  };

  pimpl_on_stack::Pimpl<S, 4, 4> pimpl;
  EXPECT_EQ(pimpl->i, kValue);
}

TEST(Pimpl, Move_constructor) {}

TEST(Pimpl, Operator_eq) {}

TEST(Pimpl, Operator_derefference) {}

TEST(Pimpl, Reset) {}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}