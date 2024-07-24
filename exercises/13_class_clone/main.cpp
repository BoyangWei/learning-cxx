#include "../exercise.h"

// READ: 复制构造函数 <https://zh.cppreference.com/w/cpp/language/copy_constructor>

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

    // TODO: 实现复制构造器
    DynFibonacci(DynFibonacci const &other): cache(new size_t[other.cap]),cached(other.cached), cap(other.cap){
        std::cout << "Run clone constructor" << std::endl;
        // copy value from other cache
        std::copy(other.cache, other.cache + cached + 1, cache);
    }

    // TODO: 实现析构器，释放缓存空间
    ~DynFibonacci(){
        delete[] cache;
    }

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

    // NOTICE: 不要修改这个方法
    // NOTICE: 名字相同参数也相同，但 const 修饰不同的方法是一对重载方法，可以同时存在
    //         本质上，方法是隐藏了 this 参数的函数
    //         const 修饰作用在 this 上，因此它们实际上参数不同
    size_t get(int i) const {
        if (i <= cached) {
            return cache[i];
        }
        ASSERT(false, "i out of range");
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    DynFibonacci const fib_ = DynFibonacci(fib);
    ASSERT(fib_.get(10) == fib.get(10), "Object cloned");
    return 0;
}
