#ifndef FFA52AA6_0136_4DCC_979E_53F2DFDCCDE1
#define FFA52AA6_0136_4DCC_979E_53F2DFDCCDE1

#include <atlas/others/interpolations/linearinterpolation.hpp>
#include <cmath>

namespace Atlas {

    template <typename T>
    T logLinearInterpolation(T x, const std::vector<T>& x_, const std::vector<T>& y_, bool extrapolate = false) {
        if (x_.size() != y_.size()) { throw std::invalid_argument("x_ and y_ should have the same size."); }

        auto it      = std::lower_bound(x_.begin(), x_.end(), x);
        size_t index = std::distance(x_.begin(), it);

        if (!extrapolate) {
            if (index == 0 || index == x_.size()) {
                throw std::out_of_range("Extrapolation is not enabled, and the provided value is outside the range.");
            }
        }

        if (index == 0) { return y_[0] * std::exp((x - x_[0]) * (std::log(y_[1]) - std::log(y_[0])) / (x_[1] - x_[0])); }

        if (index == x_.size()) {
            return y_[index - 1] *
                   std::exp((x - x_[index - 1]) * (std::log(y_[index - 1]) - std::log(y_[index - 2])) / (x_[index - 1] - x_[index - 2]));
        }

        return y_[index - 1] * std::exp((x - x_[index - 1]) * (std::log(y_[index]) - std::log(y_[index - 1])) / (x_[index] - x_[index - 1]));
    }
}  // namespace Atlas

#endif /* FFA52AA6_0136_4DCC_979E_53F2DFDCCDE1 */
