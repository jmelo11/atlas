#ifndef F2F7D698_D5F3_4B89_8FE9_DFD5F75622BE
#define F2F7D698_D5F3_4B89_8FE9_DFD5F75622BE

#include <stdexcept>
#include <vector>

namespace Atlas {
    template <typename T>
    T linearInterpolation(T x, const std::vector<T>& x_, const std::vector<T>& y_, bool extrapolate = false) {
        if (x_.size() != y_.size()) { throw std::invalid_argument("x_ and y_ should have the same size."); }

        auto it      = std::lower_bound(x_.begin(), x_.end(), x);
        size_t index = std::distance(x_.begin(), it);

        if (!extrapolate) {
            if (index == 0 || index == x_.size()) {
                throw std::out_of_range("Extrapolation is not enabled, and the provided value is outside the range.");
            }
        }

        if (index == 0) { return y_[0] + (x - x_[0]) * (y_[1] - y_[0]) / (x_[1] - x_[0]); }

        if (index == x_.size()) { return y_[index - 1] + (x - x_[index - 1]) * (y_[index - 1] - y_[index - 2]) / (x_[index - 1] - x_[index - 2]); }

        return y_[index - 1] + (x - x_[index - 1]) * (y_[index] - y_[index - 1]) / (x_[index] - x_[index - 1]);
    }
}  // namespace Atlas

#endif /* F2F7D698_D5F3_4B89_8FE9_DFD5F75622BE */
