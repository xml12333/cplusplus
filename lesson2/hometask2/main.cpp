#include <iostream>
#include <memory>

class MyClass
{
public:
    MyClass()
    {
        std::cout << "MyClass constructed" << std::endl;
    }

    ~MyClass()
    {
        std::cout << "MyClass destructed" << std::endl;
    }

    void doSomething()
    {
        std::cout << "Doing something" << std::endl;
    }
};

void checkWeakPtr(const std::weak_ptr<MyClass>& weakPtr)
{
    if (auto sharedPtr = weakPtr.lock())
    {
        std::cout << "WeakPtr is valid" << std::endl;
        std::cout << "Count of sharedPtr: "
                  << sharedPtr.use_count()
                  << std::endl;
    }
    else
    {
        std::cout << "WeakPtr is expired" << std::endl;
    }
}
int main()
{
    std::shared_ptr<MyClass> sharedPtr1 = std::make_shared<MyClass>();

    std::shared_ptr<MyClass> sharedPtr2 = sharedPtr1;

    std::weak_ptr<MyClass> weakPtr = sharedPtr1;

    sharedPtr1->doSomething();
    sharedPtr2->doSomething();

    checkWeakPtr(weakPtr);
    
    std::shared_ptr<MyClass> lockedSharedPtr = weakPtr.lock();

    sharedPtr1.reset();
    sharedPtr2.reset();
    
    checkWeakPtr(weakPtr);

    lockedSharedPtr.reset();
     
    checkWeakPtr(weakPtr);
    
    return 0;
}