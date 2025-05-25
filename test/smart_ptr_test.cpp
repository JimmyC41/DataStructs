#include <gtest/gtest.h>
#include "shared_ptr.h"
#include "unique_ptr.h"
#include "common.h"

constexpr std::size_t TEST_BUFFER_SIZE = 7;

using ::testing::Test;
using ::testing::WithParamInterface;
using ::testing::Values;

// From gtest sample #5: Shared testing logic

class QuickTest : public Test
{
protected:
    time_t start_time;

    void SetUp() override
    {
        start_time = time(nullptr);
    }

    void TearDown() override
    {
        const time_t end_time = time(nullptr);
        EXPECT_TRUE(end_time - start_time <= 1) << "This test took too long...\n";
    }
};

// From gtest sample #6: Typed tests

template <typename T>
class SmartPointerTypeTest : public QuickTest
{
protected:
    SharedPointer<T> shared1;
    UniquePointer<T> unique1;
    
    void SetUp() override
    {

        QuickTest::SetUp();
        shared1 = SharedPointer<T>(new T{});
        unique1 = UniquePointer<T>(new T{});
    };
};

using TypeParams_ = ::testing::Types<int, double, char, float>;

TYPED_TEST_SUITE(SmartPointerTypeTest, TypeParams_);

TYPED_TEST(SmartPointerTypeTest, ClassSize)
{
    EXPECT_EQ(sizeof(this->shared1), 2 * sizeof(void*));
    EXPECT_EQ(sizeof(this->unique1), sizeof(void*));
};

TYPED_TEST(SmartPointerTypeTest, DefaultCtor)
{
    SharedPointer<TypeParam> sp{};
    UniquePointer<TypeParam> up{};
    EXPECT_EQ(sp.get(), nullptr);
    EXPECT_EQ(up.get(), nullptr);
};

TYPED_TEST(SmartPointerTypeTest, SharedCopySemantics)
{
    SharedPointer<TypeParam> shared2(this->shared1);
    EXPECT_EQ(this->shared1.get_count(), shared2.get_count());
    EXPECT_EQ(*this->shared1, *shared2);

    SharedPointer<TypeParam> shared3 = shared2;
    EXPECT_EQ(shared2.get_count(), shared3.get_count());
    EXPECT_EQ(*shared2, *shared3);
};

TYPED_TEST(SmartPointerTypeTest, MoveSemantics)
{
    SharedPointer<TypeParam> shared2(std::move(this->shared1));
    EXPECT_EQ(shared2.get_count(), 1);

    SharedPointer<TypeParam> shared3 = std::move(shared2);
    EXPECT_EQ(shared3.get_count(), 1);

    EXPECT_EQ(this->shared1.get(), nullptr);
    EXPECT_EQ(shared2.get(), nullptr);

    TypeParam uniqueValue = *this->unique1;

    UniquePointer<TypeParam> unique2(std::move(this->unique1));
    EXPECT_EQ(*unique2, uniqueValue);

    UniquePointer<TypeParam> unique3 = std::move(unique2);
    EXPECT_EQ(*unique3, uniqueValue);
};

TYPED_TEST(SmartPointerTypeTest, SharedReset)
{
    SharedPointer<TypeParam> shared2 = this->shared1;
    EXPECT_EQ(shared2.get_count(), 2);

    this->shared1.reset(new TypeParam{});
    EXPECT_NE(this->shared1.get(), nullptr);

    EXPECT_EQ(this->shared1.get_count(), 1);
    EXPECT_EQ(shared2.get_count(), 1);
};

TYPED_TEST(SmartPointerTypeTest, UniqueRelease)
{
    TypeParam* rawValue = this->unique1.release();
    EXPECT_TRUE(this->unique1.get() == nullptr);
    EXPECT_NE(rawValue, nullptr);
};

// Value Parmaterized Tests

class SharedPointerValueTest : public QuickTest, public WithParamInterface<int>
{
protected:
    SharedPointer<int> sp;
    int param;

    void SetUp() override
    {
        QuickTest::SetUp();
        param = GetParam();
        sp = SharedPointer<int>(new int(param));
    }
};

class UniquePointerValueTest : public QuickTest, public WithParamInterface<int>
{
protected:
    UniquePointer<int> up;
    int param;

    void SetUp() override
    {
        QuickTest::SetUp();
        param = GetParam();
        up = UniquePointer<int>(new int(param));
    }
};

TEST_P(SharedPointerValueTest, DataAndRefCount)
{
    EXPECT_EQ(*sp, param);
    EXPECT_EQ(sp.get_count(), 1);

    auto copy1 = sp;
    EXPECT_EQ(*sp, *copy1);
    EXPECT_EQ(sp.get_count(), 2);

    auto copy2 = sp;
    EXPECT_EQ(*sp, *copy2);
    EXPECT_EQ(sp.get_count(), 3);

    auto newOwner = std::move(sp);
    EXPECT_EQ(newOwner.get_count(), 3);
};

TEST_P(UniquePointerValueTest, Release)
{
    EXPECT_EQ(*up, param);

    auto up2(std::move(up));
    EXPECT_EQ(up.get(), nullptr);
    EXPECT_EQ(*up2, param);

    auto up3 = std::move(up2);
    EXPECT_EQ(up2.get(), nullptr);
    EXPECT_EQ(*up3, param);

    EXPECT_EQ(*up3.release(), param);
    EXPECT_EQ(up3.get(), nullptr);
};

INSTANTIATE_TEST_SUITE_P(SharedInt, SharedPointerValueTest, Values(-1, 0, 1, 100));
INSTANTIATE_TEST_SUITE_P(UniqueInt, UniquePointerValueTest, Values(-1, 0, 1, 100));
