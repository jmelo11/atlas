#ifndef F2F7D698_D5F3_4B89_8FE9_DFD5F75622BE
#define F2F7D698_D5F3_4B89_8FE9_DFD5F75622BE

#include <atlas/others/interpolations/interpolator.hpp>
#include <stdexcept>
#include <vector>

namespace Atlas {

    /**
     * @class LinearInterpolator
     * @brief Linear interpolation
     * 
     * @tparam adouble 
     */
    template <typename adouble>
    class LinearInterpolator : public Interpolator<adouble> {
       public:
        LinearInterpolator() = default;

        LinearInterpolator(const std::vector<double>& x, const std::vector<adouble>& y, bool extrapolate = false)
        : x_(x), y_(y), extrapolate_(extrapolate) {
            if (x_.size() != y_.size()) { throw std::invalid_argument("x_ and y_ should have the same size."); }
        }

        adouble operator()(double x) const override { return linearInterpolation(x); }

        void enableExtrapolation(bool e) { extrapolate_ = e; }

       private:
        adouble linearInterpolation(double x) const {
            auto it      = std::lower_bound(x_.begin(), x_.end(), x);
            size_t index = std::distance(x_.begin(), it);

            if (!extrapolate_) {
                if (x < x_.front() || x > x_.back()) {
                    throw std::out_of_range("Extrapolation (linear) is not enabled, and the provided value is outside the range.");
                }
            }

            if (index == 0) { return y_[0] + (x - x_[0]) * (y_[1] - y_[0]) / (x_[1] - x_[0]); }

            if (index == x_.size()) {
                return y_[index - 1] + (x - x_[index - 1]) * (y_[index - 1] - y_[index - 2]) / (x_[index - 1] - x_[index - 2]);
            }

            return y_[index - 1] + (x - x_[index - 1]) * (y_[index] - y_[index - 1]) / (x_[index] - x_[index - 1]);
        }
        std::vector<double> x_;
        std::vector<adouble> y_;
        bool extrapolate_;
    };

}  // namespace Atlas

#endif /* F2F7D698_D5F3_4B89_8FE9_DFD5F75622BE */
