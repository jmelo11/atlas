#ifndef AF67A639_F380_427B_88DD_9765B0579217
#define AF67A639_F380_427B_88DD_9765B0579217

#if __INTELLISENSE__
#undef __ARM_NEON
#undef __ARM_NEON__
#endif

#include <XAD/XAD.hpp>
// #include <autodiff/forward/dual.hpp>
// #include <autodiff/forward/dual/eigen.hpp>

namespace Atlas {
    // using dual = autodiff::dual;
    using dual = xad::AD;
    using fwdDual = xad::FReal<double>;

    typedef xad::adj<double> mode;
    typedef mode::tape_type tape_type;

    inline double val(const dual& x) {
        return x.value();
    };

}  // namespace Atlas

#endif /* AF67A639_F380_427B_88DD_9765B0579217 */
