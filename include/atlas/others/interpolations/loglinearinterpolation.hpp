#ifndef FFA52AA6_0136_4DCC_979E_53F2DFDCCDE1
#define FFA52AA6_0136_4DCC_979E_53F2DFDCCDE1

#include <atlas/others/interpolations/linearinterpolation.hpp>
#include <cmath>

namespace Atlas {

    template <typename adouble>
    class LogLinearInterpolator : public Interpolator<adouble> {
       public:

        LogLinearInterpolator() = default;
        
        LogLinearInterpolator(const std::vector<adouble>& x, const std::vector<adouble>& y, bool extrapolate = false)
        : x_(x), y_(y), extrapolate_(extrapolate) {
            if (x_.size() != y_.size()) { throw std::invalid_argument("x_ and y_ should have the same size."); }
        }

        adouble operator()(adouble x) const override { return logLinearInterpolation(x); }

        void enableExtrapolation() { extrapolate_ = true; }
       private:
        adouble logLinearInterpolation(adouble x) const {
            auto it      = std::lower_bound(x_.begin(), x_.end(), x);
            size_t index = std::distance(x_.begin(), it);

            if (!extrapolate_) {
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

        std::vector<adouble> x_;
        std::vector<adouble> y_;
        bool extrapolate_;
    };

}  // namespace Atlas

#endif /* FFA52AA6_0136_4DCC_979E_53F2DFDCCDE1 */
