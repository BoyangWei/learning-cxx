#include "../exercise.h"

// READ: 移动构造函数 <https://zh.cppreference.com/w/cpp/language/move_constructor>
// READ: 移动赋值 <https://zh.cppreference.com/w/cpp/language/move_assignment>
// READ: 运算符重载 <https://zh.cppreference.com/w/cpp/language/operators>

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

    // TODO: 实现移动构造器
    DynFibonacci(DynFibonacci &&other) noexcept : cache(other.cache),cached(other.cached),cap(other.cap){
        other.cache=nullptr;
    };

    // TODO: 实现移动赋值
    // NOTICE: ⚠ 注意移动到自身问题 ⚠
    DynFibonacci &operator=(DynFibonacci &&other) noexcept {
        if ( &other != this ){
            cache = other.cache;
            cached = other.cached;
            cap = other.cap;
            other.cache=nullptr;
        }
        return *this;
    }

    // TODO: 实现析构器，释放缓存空间
    ~DynFibonacci(){
        delete[] cache;
    }

    // TODO: 实现正确的缓存优化斐波那契计算
    size_t operator[](int i) {
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
    
    size_t operator[](int i) const{
        if (i <= cached) {
            return cache[i];
        }
        ASSERT(false, "i out of range");
    }

    // NOTICE: 不要修改这个方法
    bool is_alive() const {
        return cache;
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci const fib_ = std::move(fib);
    ASSERT(!fib.is_alive(), "Object moved");
    ASSERT(fib_[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci fib0(6);
    DynFibonacci fib1(12);

    fib0 = std::move(fib1);
    fib0 = std::move(fib0);
    ASSERT(fib0[10] == 55, "fibonacci(10) should be 55");

    return 0;
}
