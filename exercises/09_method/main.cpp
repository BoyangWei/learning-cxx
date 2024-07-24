#include "../exercise.h"

struct Fibonacci {
    unsigned long long cache[128];
    int cached;
    int len = sizeof(cache)/sizeof(cache[0]);

    // TODO: 实现正确的缓存优化斐波那契计算
    unsigned long long get(int i) {
        cache[0]=0;
        cache[1]=1;
        if (i > cached)
        {
            for (cached = 2 < cached ? cached : 2; cached <= i && cached <len; ++cached)
            {
                cache[cached] = cache[cached - 1] + cache[cached - 2];
            }
            cached = i;
        }
        return cache[i];
    }
};

int main(int argc, char **argv) {
    // TODO: 初始化缓存结构体，使计算正确
    Fibonacci fib = {{},0};
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    return 0;
}
