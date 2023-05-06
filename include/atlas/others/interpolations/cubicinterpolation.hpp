#ifndef CD3CEF5A_A741_4348_87DD_2FAFE4AAF40E
#define CD3CEF5A_A741_4348_87DD_2FAFE4AAF40E

#include <vector>
#include <stdexcept>

namespace Atlas {
    template <typename T>
    T cubicInterpolation(T x, const std::vector<T>& x_, const std::vector<T>& y_, bool extrapolate) {
        if (x_.size() != y_.size()) { throw std::invalid_argument("x_ and y_ should have the same size."); }

        auto it      = std::lower_bound(x_.begin(), x_.end(), x);
        size_t index = std::distance(x_.begin(), it);

        if (!extrapolate) {
            if (index == 0 || index == x_.size()) {
                throw std::out_of_range("Extrapolation is not enabled, and the provided value is outside the range.");
            }
        }

        if (index == 0) {
            index = 1;
        } else if (index == x_.size()) {
            index = x_.size() - 1;
        }

        T t = (x - x_[index - 1]) / (x_[index] - x_[index - 1]);
        T a = y_[index - 1];
        T b = (y_[index] - y_[index - 1]) / (x_[index] - x_[index - 1]);
        T c = (3 * (y_[index] - y_[index - 1]) / (x_[index] - x_[index - 1]) - (2 * b + b)) / (x_[index] - x_[index - 1]);
        T d = (2 * (y_[index - 1] - y_[index]) / (x_[index] - x_[index - 1]) + (b + b)) / ((x_[index] - x_[index - 1]) * (x_[index] - x_[index - 1]));

        return a + b * t + c * t * t + d * t * t * t;
    }
}  // namespace Atlas

#endif /* CD3CEF5A_A741_4348_87DD_2FAFE4AAF40E */
