#ifndef FF4C5D8A_ED4E_464E_8E4F_698A9793E39B
#define FF4C5D8A_ED4E_464E_8E4F_698A9793E39B

#include <atlas/atlasconfig.hpp>

namespace Atlas {
    /**
     * @class YieldTermStructureStrategy
     * @brief A class representing a yield term structure. The strategy pattern is used to allow for different
     * implementations of the yield term structure.
     *
     * @tparam adouble
     */
    template <typename adouble>
    class YieldTermStructureStrategy {
       public:
        virtual ~YieldTermStructureStrategy() = default;

        virtual adouble discount(const Date& date) const = 0;

        virtual adouble discount(double t) const = 0;

        virtual adouble forwardRate(const Date& startDate, const Date& endDate, const DayCounter& dayCounter, Compounding comp,
                                    Frequency freq) const = 0;

        virtual std::unique_ptr<YieldTermStructureStrategy> clone() const = 0;

        Date refDate() const {
            if (refDate_ == Date()) { throw std::invalid_argument("Reference date not set"); }
            return refDate_;
        };

       protected:
        Date refDate_;
    };
}  // namespace Atlas

#endif /* FF4C5D8A_ED4E_464E_8E4F_698A9793E39B */
