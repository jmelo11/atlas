#ifndef AF67A639_F380_427B_88DD_9765B0579217
#define AF67A639_F380_427B_88DD_9765B0579217

// If USE_AUTODIFF is defined, then we use autodiff to perform automatic differentiation
#include <autodiff/forward/dual.hpp>

namespace Atlas {
    // using adouble = autodiff::dual;
    using dual = autodiff::dual;
}  // namespace Atlas

#endif /* AF67A639_F380_427B_88DD_9765B0579217 */
