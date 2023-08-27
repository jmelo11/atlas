#ifndef AF67A639_F380_427B_88DD_9765B0579217
#define AF67A639_F380_427B_88DD_9765B0579217

#if __INTELLISENSE__
#undef __ARM_NEON
#undef __ARM_NEON__
#endif

// #include <autodiff/forward/dual.hpp>
// #include <autodiff/forward/dual/eigen.hpp>

#define USE_NONE true
#define USE_DYNAMIC_POLYMORPHISM false
#define USE_STATIC_POLYMORPHISM false

namespace Atlas {

    using dual = double;

    template <typename T>
    inline double val(const T& x) {
        if constexpr (std::is_same_v<T, double>) {
            return x;
        } else {
            return x.value();
        }
    };

}  // namespace Atlas

#endif /* AF67A639_F380_427B_88DD_9765B0579217 */
