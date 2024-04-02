#include <iostream>
#include <stdexcept>

class NullPointerException : public std::runtime_error
{
public:
    NullPointerException(const std::string &message)
        : std::runtime_error(message) {}
};

template <typename T>
class mySharedPtr
{
private:
    T *ptr;
    size_t *ref_count;

public:
    mySharedPtr() : ptr(nullptr), ref_count(new size_t(0)) {}

    explicit mySharedPtr(T *p) : ptr(p), ref_count(new size_t(1)) {}

    mySharedPtr(const mySharedPtr<T> &otherPtr) : ptr(otherPtr.ptr), ref_count(otherPtr.ref_count)
    {
        (*ref_count)++;
    }

    ~mySharedPtr()
    {
        if (ptr != nullptr && --(*ref_count) == 0)
        {
            delete ptr;
            delete ref_count;
        }
    }

    mySharedPtr<T> &operator=(const mySharedPtr<T> &otherPtr)
    {
        if (this != &otherPtr)
        {
            if (ptr != nullptr && --(*ref_count) == 0)
            {
                delete ptr;
                delete ref_count;
                ptr = nullptr;
            }

            ptr = otherPtr.ptr;
            ref_count = otherPtr.ref_count;
            (*ref_count)++;
        }
        return *this;
    }

    T &operator*() const
    {
        if (ptr == nullptr)
            throw std::runtime_error("Dereferencing null pointer");
        return *ptr;
    }

    T *operator->() const
    {
        if (ptr == nullptr)
            throw NullPointerException("Accessing null pointer");
        return ptr;
    }

    size_t base_count() const
    {
        return *ref_count;
    }
};

int main()
{
    mySharedPtr<int> p1(new int(999));
    std::cout << "p1: " << *p1 << ", base_count: " << p1.base_count() << std::endl;

    mySharedPtr<int> p2 = p1;
    std::cout << "p2: " << *p2 << ", base_count: " << p2.base_count() << std::endl;

    mySharedPtr<int> p3(new int(500));
    std::cout << "p3: " << *p3 << ", base_count: " << p3.base_count() << std::endl;

    p2 = p3;
    std::cout << "p2 after assignment: " << *p2 << ", base_count: " << p2.base_count() << std::endl;

    mySharedPtr<std::string> p4(new std::string("Hello"));
    std::cout << "p4: " << *p4 << ", base_count: " << p4.base_count() << std::endl;

    mySharedPtr<std::string> p5 = p4;
    std::cout << "p5: after assignment: " << *p5 << ", base_count: " << p5.base_count() << std::endl;

    struct MyClass
    {
        int value;
        MyClass(int v) : value(v) {}
    };
    mySharedPtr<MyClass> p6(new MyClass(100));
    std::cout << "p6: " << p6->value << ", base_count: " << p6.base_count() << std::endl;

    mySharedPtr<double> p7(new double(3.14));
    std::cout << "p7: " << *p7 << ", base_count: " << p7.base_count() << std::endl;

    mySharedPtr<double> p8 = p7;
    std::cout << "p8: after assignment: " << *p8 << ", base_count: " << p8.base_count() << std::endl;

    try
    {
        mySharedPtr<int> p9;
        std::cout << "p9: " << *p9 << ", base_count: " << p9.base_count() << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    try
    {
        mySharedPtr<std::string> pNull;
        pNull->size();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "NullPointerException caught: " << e.what() << std::endl;
    }

    return 0;
}