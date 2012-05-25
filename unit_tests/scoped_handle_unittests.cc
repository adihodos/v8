#include <gtest/gtest.h>
#include "v8/base/scoped_handle.h"

namespace {

using namespace v8::base;

struct dummy_handle_t : public v8::base::handle_traits_base<int> {
    static int null_handle() {
        return -1;
    }

    static void dispose(int) {}
};

typedef v8::base::scoped_handle<dummy_handle_t> handle_type;

handle_type makeTempHandle(int val) {
    handle_type tmpHandle(val);
    return tmpHandle;
}

}

TEST(scopedhandle_tests, basic) {
    handle_type testHandle;

    EXPECT_TRUE(!testHandle);
    EXPECT_FALSE(testHandle);
    EXPECT_EQ(dummy_handle_t::null_handle(), scoped_handle_get(testHandle));
}

TEST(scopedhandle_tests, move_constructors) {
    handle_type testHandle(std::move(makeTempHandle(2)));
    EXPECT_EQ(2, scoped_handle_get(testHandle));

    testHandle = makeTempHandle(3);
    EXPECT_EQ(3, scoped_handle_get(testHandle));
}

TEST(scopedhandle_tests, release_reset_get_swap) {
    handle_type firstHandle(1);

    int tmpHandle = scoped_handle_release(firstHandle);
    EXPECT_TRUE(!firstHandle);
    EXPECT_EQ(dummy_handle_t::null_handle(), scoped_handle_get(firstHandle));


    scoped_handle_reset(firstHandle);
    EXPECT_TRUE(!firstHandle);

    scoped_handle_reset(firstHandle, tmpHandle);
    EXPECT_FALSE(!firstHandle);
    EXPECT_EQ(tmpHandle, scoped_handle_get(firstHandle));
    //
    // reset with self should be a noop
    scoped_handle_reset(firstHandle, scoped_handle_get(firstHandle));
    EXPECT_EQ(tmpHandle, scoped_handle_get(firstHandle));

    handle_type secondHandle(2);
    EXPECT_EQ(1, scoped_handle_get(firstHandle));
    EXPECT_EQ(2, scoped_handle_get(secondHandle));

    swap(firstHandle, secondHandle);

    EXPECT_EQ(2, scoped_handle_get(firstHandle));
    EXPECT_EQ(1, scoped_handle_get(secondHandle));

    int* handlePtr = scoped_handle_get_impl(firstHandle);
    *handlePtr = 5;
    EXPECT_EQ(5, scoped_handle_get(firstHandle));
}

TEST(scopedhandle_tests, eq_neq_operators) {
    handle_type smartHandle(1);

    EXPECT_TRUE(1 == smartHandle);
    EXPECT_FALSE(1 != smartHandle);
    EXPECT_TRUE(smartHandle == 1);
    EXPECT_FALSE(smartHandle != 1);
}