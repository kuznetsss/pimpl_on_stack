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

TEST(Pimpl, Move_constructor) {
  class C {
   public:
    C() = default;
    C(const C&) = delete;
    C(C&& other) noexcept {
        *this = std::move(other);
    }
    C& operator=(C&&) noexcept {
      constructed_by_move_ = true;
      return *this;
    }

    bool isConstructedByMove() const { return constructed_by_move_; }

   private:
    bool constructed_by_move_ = false;
  };

  pimpl_on_stack::Pimpl<C, 1, 1> pimpl;
  EXPECT_FALSE(pimpl->isConstructedByMove());

  pimpl_on_stack::Pimpl<C, 1, 1> moved_pimpl(std::move(pimpl));
  EXPECT_TRUE(moved_pimpl->isConstructedByMove());
  EXPECT_TRUE(pimpl.empty());
}

TEST(Pimpl, Operator_equals_and_reset) {
  bool destructor_used = false;
  class C {
   public:
    C(bool* d_used) : destructor_used_(d_used) {}
    ~C() { *destructor_used_ = true; }

   private:
    bool* destructor_used_;
  };
  pimpl_on_stack::Pimpl<C, 8, 8> p(&destructor_used);
  EXPECT_FALSE(p == nullptr);
  p.reset();
  EXPECT_TRUE(destructor_used);
  EXPECT_TRUE(p == nullptr);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
