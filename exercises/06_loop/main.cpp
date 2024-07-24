#include "../exercise.h"

// TODO: 改正函数实现，实现正确的缓存优化斐波那契计算
static unsigned long long fibonacci(int i) {
    // TODO: 为缓存设置正确的初始值
    static unsigned long long cache[96], cached;
    // TODO: 设置正确的循环条件
    auto len = sizeof(cache) / sizeof(cache[0]);
    for (; cached < len; ++cached) {
        if (cached == 0) {
            cache[cached] = 0;
        } else if (cached == 1) {
            cache[cached] = 1;
        } else {
            cache[cached] = cache[cached - 1] + cache[cached - 2];
        }
    }
    return cache[i];
}

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    ASSERT(fibonacci(0) == 0, "fibonacci(0) should be 0");
    ASSERT(fibonacci(1) == 1, "fibonacci(1) should be 1");
    ASSERT(fibonacci(2) == 1, "fibonacci(2) should be 1");
    ASSERT(fibonacci(3) == 2, "fibonacci(3) should be 2");
    ASSERT(fibonacci(10) == 55, "fibonacci(10) should be 55");

    auto fib90 = fibonacci(90);
    std::cout << "fibonacci(90) = " << fib90 << std::endl;
    ASSERT(fib90 == 2880067194370816120, "fibonacci(90) should be 2880067194370816120");
    return 0;
}
