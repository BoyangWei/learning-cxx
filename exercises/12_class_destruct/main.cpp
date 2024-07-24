#include "../exercise.h"

// READ: 析构函数 <https://zh.cppreference.com/w/cpp/language/destructor>

/// @brief 任意缓存容量的斐波那契类型。
/// @details 可以在构造时传入缓存容量，因此需要动态分配缓存空间。
class DynFibonacci {
    size_t *cache;
    int cached;
    int cap;

public:
    // TODO: 实现动态设置容量的构造器
    DynFibonacci(int capacity): cache(new size_t[capacity]), cached(0), cap(capacity) {
        switch (capacity)
        {
        case 1:
            cache[0]=0;
            break;
        case 2:
            cache[0]=0;
            cache[1]=1;
        default:
            cache[0]=0;
            cache[1]=1;
            for (size_t i = 2; i < capacity; i++)
            {
                cache[i]=0;
            }
            
            break;
        }
    }

    // TODO: 实现析构器，释放缓存空间
    ~DynFibonacci(){
        delete[] cache;
    };

    // TODO: 实现正确的缓存优化斐波那契计算
    size_t get(int i) {
        if (i<2){
            return cache[i];
        }else if (cache[cap-1]!=0)
        {
            return cache[i];
        }
        for (cached = 2; cached < cap; ++cached) {
            cache[cached] = cache[cached - 1] + cache[cached - 2];
        }
        return cache[i];
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    return 0;
}
