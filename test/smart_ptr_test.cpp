#include <gtest/gtest.h>
#include "shared_ptr.h"
#include "unique_ptr.h"

constexpr std::size_t TEST_BUFFER_SIZE = 7;

class SmartPointerTest : public testing::Test
{
protected:
    SharedPointer<int>     sharedInt;
    SharedPointer<char>    sharedChar;
    SharedPointer<double>  sharedDouble;

    UniquePointer<int>     uniqueInt;
    UniquePointer<char>    uniqueChar;
    UniquePointer<double>  uniqueDouble;

    void SetUp() override
    {
        sharedInt    = SharedPointer<int>(new int(25));
        sharedChar   = SharedPointer<char>(new char('O'));
        sharedDouble = SharedPointer<double>(new double(25.082004));

        uniqueInt    = UniquePointer<int>(new int(2004));
        uniqueChar   = UniquePointer<char>(new char('C'));
        uniqueDouble = UniquePointer<double>(new double(8.252004));
    };
};

TEST_F(SmartPointerTest, ClassSize)
{
    EXPECT_EQ(sizeof(sharedInt), 2 * sizeof(void*));
    EXPECT_EQ(sizeof(uniqueInt), sizeof(void*));
};

TEST_F(SmartPointerTest, DefaultCtor)
{
    SharedPointer<int> sp{};
    UniquePointer<int> up{};

    EXPECT_EQ(sp.get(), nullptr);
    EXPECT_EQ(up.get(), nullptr);
};

TEST_F(SmartPointerTest, SharedCopySemantics)
{
    SharedPointer<char> sharedChar2(sharedChar);
    EXPECT_EQ(sharedChar.get_count(), sharedChar2.get_count());
    EXPECT_EQ(*sharedChar, *sharedChar2);

    SharedPointer<char> sharedChar3 = sharedChar2;
    EXPECT_EQ(sharedChar2.get_count(), sharedChar3.get_count());
    EXPECT_EQ(*sharedChar2, *sharedChar3);
};

TEST_F(SmartPointerTest, MoveSemantics)
{
    SharedPointer<int> sharedInt2(std::move(sharedInt));
    EXPECT_EQ(sharedInt2.get_count(), 1);

    SharedPointer<int> sharedInt3 = std::move(sharedInt2);
    EXPECT_EQ(sharedInt3.get_count(), 1);

    EXPECT_EQ(sharedInt.get(), nullptr);
    EXPECT_EQ(sharedInt2.get(), nullptr);

    UniquePointer<int> uniqueInt2(std::move(uniqueInt));
    EXPECT_EQ(*uniqueInt2, 2004);

    UniquePointer<int> uniqueInt3 = std::move(uniqueInt2);
    EXPECT_EQ(*uniqueInt3, 2004);
};

TEST_F(SmartPointerTest, SharedReset)
{
    SharedPointer<double> sharedDouble2 = sharedDouble;
    EXPECT_EQ(sharedDouble2.get_count(), 2);

    double* raw = new double(99.95);
    sharedDouble.reset(raw);

    EXPECT_EQ(*sharedDouble, 99.95);
    EXPECT_EQ(sharedDouble.get_count(), 1);

    EXPECT_EQ(*sharedDouble2, 25.082004);
    EXPECT_EQ(sharedDouble2.get_count(), 1);
};

TEST_F(SmartPointerTest, UniqueRelease)
{
    char* rawChar = uniqueChar.release();
    EXPECT_TRUE(uniqueChar.get() == nullptr);
    EXPECT_EQ(*rawChar, 'C');
};