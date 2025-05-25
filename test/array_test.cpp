#include <gtest/gtest.h>
#include "array.h"
#include "common.h"

constexpr int ARR_SIZE = 10;
constexpr int FILL_VAL = 2004;

using ::testing::Test;
using ::testing::WithParamInterface;
using ::testing::Values;
using IntVec = std::vector<int>;

class ArrayTest : public Test, public WithParamInterface<IntVec>
{
protected:
    Array<int, ARR_SIZE> arr;

    void SetUp() override
    {
        auto const& vec = GetParam();
        for (size_t i = 0; i < vec.size() && i < arr.size(); ++i)
            arr[i] = vec[i];
    }
};

TEST_P(ArrayTest, CheckContents)
{
    EXPECT_EQ(arr.size(), ARR_SIZE);
    const auto& vec = GetParam();

    int idx{0};

    for (int val : vec)
        EXPECT_EQ(arr[idx++], val);
    
    for (; idx < ARR_SIZE; ++idx)
        EXPECT_EQ(arr[idx], int{});
};

TEST_P(ArrayTest, FrontAndBack)
{
    const auto& vec = GetParam();
    int vecFront = (vec.size() != 0 ? vec[0] : 0);
    int vecBack = (vec.size() == ARR_SIZE ? vec[ARR_SIZE - 1] : 0);

    EXPECT_EQ(arr.front(), vecFront);
    EXPECT_EQ(arr.back(), vecBack);
};

TEST_P(ArrayTest, FillContents)
{
    arr.fill(FILL_VAL);
    EXPECT_EQ(arr.front(), arr.back());

    for (size_t i = 0; i < ARR_SIZE; ++i)
        EXPECT_EQ(arr[i], FILL_VAL);
};

INSTANTIATE_TEST_SUITE_P(ArrayInt, ArrayTest,
    Values(
        IntVec{},
        IntVec{1, 3},
        IntVec{1, 3, 5},
        IntVec{1, 3, 5, 7},
        IntVec{1, 3, 5, 7, 9},
        IntVec{1, 3, 5, 7, 9, 11},
        IntVec{1, 3, 5, 7, 9, 11, 13},
        IntVec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        IntVec{1000, 900, 800, 700, 600, 500, 400, 300, 200, 100}
    )
);