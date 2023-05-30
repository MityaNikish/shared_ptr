#pragma once

#include <concepts>
#include "Exception.hpp"

typedef unsigned int uint;


template <class Type, class TDeleter>
concept Correct_deleter_traits = std::is_default_constructible_v<TDeleter> && Correct_single_deleter_traits<Type, TDeleter>;

template <class Type, class TDeleter>
concept Correct_single_deleter_traits = requires(TDeleter del, Type *ptr) {
    { del(ptr) } -> std::same_as<void>;
};

template<class Type, class TDeleter = std::default_delete<Type>>
    requires Correct_deleter_traits<std::remove_extent_t<Type>, TDeleter>
class SharedPTR {
    typedef SharedPTR<Type, TDeleter> t_SharedPTR;
    using elementType = std::remove_extent_t<Type>;

public: // Constructors and destructor.

    SharedPTR(): ptr(), refCount(new uint(0)) {}

    explicit SharedPTR(elementType* pObject) : ptr(pObject), refCount(new uint(0))
    {
        if (nullptr != pObject)
        {
            (*refCount)++;
        }
    }

    SharedPTR(t_SharedPTR&& sharedPTR) noexcept: ptr(sharedPTR.ptr), refCount(sharedPTR.refCount) // Move constructor.
    {
        if (nullptr != sharedPTR.ptr)
        {
            (*refCount)++;
        }
    }

    SharedPTR(const t_SharedPTR& sharedPTR) noexcept : ptr(sharedPTR.ptr), refCount(sharedPTR.refCount)
    {
        if (nullptr != sharedPTR.ptr)
        {
            (*refCount)++;
        }
    }

    ~SharedPTR()
    {
        cleanup();
    }

public: // Assignment.

    t_SharedPTR& operator=(t_SharedPTR&& sharedPTR)
    {
        if ( ptr == sharedPTR.ptr)
        {
            return *this;
        }

        cleanup();

        ptr = sharedPTR.ptr;
        refCount = sharedPTR.refCount;

        sharedPTR.ptr = nullptr;
        sharedPTR.refCount = new uint(0);

        return *this;
    }

    t_SharedPTR& operator=(elementType* pObject)
    {
        this->reset(pObject);

        return *this;
    }

    t_SharedPTR& operator=(const t_SharedPTR& sharedPTR)
    {
        if (ptr == sharedPTR.ptr)
        {
            return *this;
        }

        cleanup();

        ptr = sharedPTR.ptr;
        refCount = sharedPTR.refCount;

        if (nullptr != sharedPTR.ptr)
        {
            (*refCount)++;
        }

        return *this;
    }


public: // Observers.

    elementType& operator[](size_t indx) const requires (std::is_array_v<Type>)
    {
        if (ptr == nullptr)
        {
            throw Invalid_value_exception();
        }
        return ptr[indx];
    }

    elementType& operator*() const requires (!std::is_array_v<Type>)  // Dereference the stored pointer.
    {
        if (ptr == nullptr)
        {
            throw Invalid_value_exception();
        }
        return *ptr;
    }

    elementType* operator->() const requires (!std::is_array_v<Type>) // Return the stored pointer.
    {
        if (ptr == nullptr)
        {
            throw Invalid_value_exception();
        }
        return ptr;
    }

    elementType* get() const noexcept// Return the stored pointer.
    {
        return ptr;
    }

    TDeleter& get_deleter() noexcept // Return a reference to the stored deleter.
    {
        return deleter;
    }

    operator bool() const noexcept// Return false if the stored pointer is null.
    {
        return ptr != nullptr;
    }

public: // Modifiers.

    void release()// Release ownership of any stored pointer.
    {
        this->reset();
    }

    void reset(elementType* pObject = nullptr) // Replace the stored pointer.
    {
        if (pObject == ptr)
        {
            return;
        }

        cleanup();

        ptr = pObject;

        refCount = new uint(pObject ? 0 : 1);
    }

    void swap(t_SharedPTR& sharedPTR) noexcept// Exchange the pointer with another object.
    {
        std::swap(ptr, sharedPTR.ptr);
        std::swap(refCount, sharedPTR.refCount);
    }

private:

    elementType* ptr;
    uint* refCount;
    TDeleter deleter = TDeleter();

private:

    void cleanup()
    {
        if (*refCount != 0)
        {
            (*refCount)--;
        }

        if (*refCount == 0)
        {     
            get_deleter() (ptr);
            delete refCount;
        }
    }
};