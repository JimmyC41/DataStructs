#include "common.h"

struct ControlBlock
{
    size_t refCount_{};
    std::mutex mutex_;
};

template <typename T>
class SharedPointer
{
private:
    T* ptr_;
    ControlBlock* controlBlock_;

public:
    SharedPointer() : SharedPointer(nullptr) {}
    SharedPointer(T* rawPointer)
    { 
        Initialize(rawPointer);
    }

    SharedPointer(const SharedPointer& other) noexcept
    {
        CopyFrom(other);
    }

    SharedPointer& operator=(const SharedPointer& other) noexcept
    {
        if (this != &other)
        {
            TryRelease();
            CopyFrom(other);
        }
        return *this;
    }

    SharedPointer(SharedPointer&& other) noexcept
    {
        MoveFrom(std::move(other));
    }

    SharedPointer& operator=(SharedPointer&& other) noexcept
    {
        if (this != &other)
        {
            TryRelease();
            MoveFrom(other);
        }
        return *this;
    }

    ~SharedPointer()
    {
        TryRelease();
    }

    void reset(T* other)
    {
        TryRelease();
        
        if (!other)
            return;
        
        Initialize(other);
    }

    size_t get_count() const
    {
        std::scoped_lock lock{controlBlock_->mutex_};
        return controlBlock_->refCount_;
    }

    T* get() const { return ptr_; }
    T* operator->() const { return ptr_; }
    T& operator*() const { return *ptr_; }
    operator bool() const noexcept { return ptr_ != nullptr; }

private:
    // Sets the raw pointer and allocates control block
    void Initialize(T* rawPointer)
    {
        std::cout << "New shared pointer initialized!\n";
        ptr_ = rawPointer;
        controlBlock_ = new ControlBlock{1};
    }

    // Points to the same resource and control block
    void CopyFrom(const SharedPointer& other) noexcept
    {
        std::cout << "Existing shared pointer copied!\n";
        ptr_ = other.ptr_;
        controlBlock_ = other.controlBlock_;

        if (!controlBlock_)
            return;
        
        {
            std::scoped_lock lock{controlBlock_->mutex_};
            controlBlock_->refCount_++;
            std::cout << std::format("refCount: {}\n", controlBlock_->refCount_);
        }
    }

    // Transfer ownership resource and control block
    void MoveFrom(SharedPointer&& other) noexcept
    {
        std::cout << "Ownership of shared pointer moved!\n";
        ptr_ = std::exchange(other.ptr_, nullptr);
        controlBlock_ = std::exchange(other.controlBlock_, nullptr);
    }

    // Decrement ref count, release resources if ref count is 0
    void TryRelease() noexcept
    {
        // Early return if the shared_ptr is not managing any resource
        if (!controlBlock_)
            return;

        {
            std::scoped_lock lock{controlBlock_->mutex_};
            if (--controlBlock_->refCount_ != 0) // refCount is decremented
                return;
        }

        std::cout << "refCount reached 0. Destroying now...";

        // controlBlock_ is not null and refCount_ = 0:
        delete ptr_;
        ptr_ = nullptr;
        delete controlBlock_;
        controlBlock_ = nullptr;
    }
};