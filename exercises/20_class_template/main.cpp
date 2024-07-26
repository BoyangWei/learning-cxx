#include "../exercise.h"

// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;
    size_t size;

    Tensor4D(unsigned int const shape_[4], T const *data_) {
        // TODO: 填入正确的 shape 并计算 size
        std::copy(shape_, shape_ + 4, shape);
        size = shape[0] * shape[1] * shape[2] * shape[3];
        data = new T[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = data_[i];
        }
    }
    ~Tensor4D() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 这个加法需要支持“单向广播”。
    // 具体来说，`others` 可以具有与 `this` 不同的形状，形状不同的维度长度必须为 1。
    // `others` 长度为 1 但 `this` 长度不为 1 的维度将发生广播计算。
    // 例如，`this` 形状为 `[1, 2, 3, 4]`，`others` 形状为 `[1, 2, 1, 4]`，
    // 则 `this` 与 `others` 相加时，3 个形状为 `[1, 2, 1, 4]` 的子张量各自与 `others` 对应项相加。
    Tensor4D &operator+=(Tensor4D const &others) {
        // TODO: 实现单向广播的加法
        unsigned int broadcast_shape[4];
        size_t broadcast_size = 1;
        bool needs_broadcast = false;

        // 计算广播后的形状和总大小
        for (int i = 0; i < 4; ++i) {
            if (shape[i] == others.shape[i]) {
                broadcast_shape[i] = shape[i];
            } else if (shape[i] == 1 ) {
                broadcast_shape[i] = others.shape[i];
                needs_broadcast = true;
            } else if (others.shape[i] == 1 ) {
                broadcast_shape[i] = shape[i];
                needs_broadcast = true;
            } else {
                throw std::invalid_argument("Shapes are not compatible for broadcasting.");
            }
            broadcast_size *= broadcast_shape[i];
        }

        if (!needs_broadcast) {
            // 无需广播，直接相加
            for (size_t i = 0; i < size; ++i) {
                data[i] += others.data[i];
            }
        } else {
            // 广播加法
            T *result_data = new T[broadcast_size];
            size_t this_stride[4], others_stride[4], broadcast_stride[4];

            // 计算 stride
            this_stride[3] = others_stride[3] = broadcast_stride[3] = 1;
            for (int i = 2; i >= 0; --i) {
                this_stride[i] = this_stride[i + 1] * shape[i + 1] ;
                others_stride[i] = others_stride[i + 1] * others.shape[i + 1];
                broadcast_stride[i] = broadcast_stride[i + 1] * broadcast_shape[i + 1];
            }

            for (size_t i = 0; i < broadcast_size; ++i) {
                size_t this_index = 0, others_index = 0, temp = i;
                //通过stride计算索引，也就是映射关系。广播后的tensor中的元素对应到广播的tensor中的元素
                for (int j = 0; j < 4; ++j) {
                    size_t idx = temp / broadcast_stride[j];
                    temp %= broadcast_stride[j];
                    this_index += (shape[j] > 1 ? idx : 0) * this_stride[j];
                    others_index += (others.shape[j] > 1 ? idx : 0) * others_stride[j];
                }
                result_data[i] = data[this_index] + others.data[others_index];
            }

            delete[] data;
            data = result_data;
            size = broadcast_size;
            std::copy(broadcast_shape, broadcast_shape + 4, shape);
        }
        return *this;
    }
};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        auto t0 = Tensor4D(shape, data);
        auto t1 = Tensor4D(shape, data);
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        double d0[]{
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};

        unsigned int s1[]{1, 2, 1, 1};
        double d1[]{
            1,// 将对 t0 的前半部分影响
            2 // 将对 t0 的后半部分影响
        };

        // 创建张量
        Tensor4D<double> t0(s0, d0);
        Tensor4D<double> t1(s1, d1);

        // 执行广播加法
        t0 += t1;

        // 断言结果
        for (size_t i = 0; i < 12; ++i) {// 前半部分 (1,2,3,4) 的前 2 维数据
            ASSERT(t0.data[i] == d0[i] + 1, "t0.data[i] should be incremented by 1");
        }

        for (size_t i = 12; i < t0.size; ++i) {// 后半部分 (1,2,3,4) 的前 2 维数据
            ASSERT(t0.data[i] == d0[i] + 2, "t0.data[i] should be incremented by 2");
        }
    }
}
