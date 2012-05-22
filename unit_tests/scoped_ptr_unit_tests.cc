#include <algorithm>
#include <gtest/gtest.h>
#include "v8/base/scoped_pointer.h"

namespace {

class Tester {
private :
    int val_;

public :
    Tester() {}

    explicit Tester(int val) : val_(val) {}

    const int GetValue() const {
        return val_;
    }

    void SetValue(int val) {
        val_ = val;
    }
};

template<typename T>
v8::base::scoped_ptr<T> make_temporary_ptr(const T& value) {
    v8::base::scoped_ptr<T> tmpPtr(new T(value));
    return tmpPtr;
}

} // basic tests

TEST(scoped_pointer_tests, basic) {
    v8::base::scoped_ptr<Tester> sptr(new Tester(1024));

    //
    // Test boolean operators and pointer semantic operators
    EXPECT_TRUE(sptr);
    EXPECT_FALSE(!sptr);
    EXPECT_EQ(1024, sptr->GetValue());
    EXPECT_EQ(1024, (*sptr).GetValue());
}

TEST(scoped_pointer_tests, get_release_reset_swap_impl) {
    v8::base::scoped_ptr<Tester> objA(new Tester(1024));

    //
    // test release
    Tester* tmpPtr = v8::base::scoped_pointer_release(objA);

    EXPECT_TRUE(!objA);

    //
    // test reset
    v8::base::scoped_pointer_reset(objA, tmpPtr);
    EXPECT_EQ(tmpPtr, v8::base::scoped_pointer_get(objA));

    //
    // test swapping
    v8::base::scoped_ptr<Tester> objB(new Tester(2048));

    EXPECT_EQ(1024, objA->GetValue());
    EXPECT_EQ(2048, objB->GetValue());

    swap(objA, objB);

    EXPECT_EQ(2048, objA->GetValue());
    EXPECT_EQ(1024, objB->GetValue());

    v8::base::scoped_pointer_reset(objA);
    EXPECT_TRUE(!objA);

    //
    // test get_impl()
    *(v8::base::scoped_pointer_get_impl(objA)) = new Tester(4096);
    EXPECT_EQ(4096, objA->GetValue());
}

TEST(scoped_pointer_tests, equality_operators) {
    v8::base::scoped_ptr<Tester> ptrObj(new Tester(1024));
    Tester* rawPtr = v8::base::scoped_pointer_get(ptrObj);

    EXPECT_FALSE(rawPtr != ptrObj);
    EXPECT_FALSE(ptrObj != rawPtr);
    EXPECT_TRUE(rawPtr == ptrObj);
    EXPECT_TRUE(ptrObj == rawPtr);
}

TEST(scoped_pointer_tests, move_constructors) {
    //
    // Move constructors
    v8::base::scoped_ptr<Tester> mainPtr(make_temporary_ptr(Tester(1024)));

    EXPECT_EQ(1024, mainPtr->GetValue());

    mainPtr = make_temporary_ptr(Tester(2048));
    EXPECT_EQ(2048, mainPtr->GetValue());
}

TEST(scoped_pointer_tests, array_syntax) {
    //
    // Array access
    v8::base::scoped_ptr<Tester, v8::base::default_array_storage> ptrObjects(
        new Tester[16]);

    for (size_t i = 0; i < 16; ++i) {
        ptrObjects[i].SetValue(i * 10);
    }

    for (size_t i = 0; i < 16; ++i) {
        EXPECT_EQ(i * 10, ptrObjects[i].GetValue());
    }
}