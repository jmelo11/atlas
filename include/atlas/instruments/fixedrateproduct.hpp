#ifndef A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D
#define A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D

#include <ql/interestrate.hpp>
#include <atlas/instruments/instrument.hpp>

namespace Atlas {

    // general porpuse class
    class FixedRateProduct : public Instrument {
        FixedRateProduct(std::vector<QuantLib::Date> dates, std::vector<double> redemptions,
                         QuantLib::InterestRate rate);

        void accept(Visitor& visitor) override;

        void accept(ConstVisitor& visitor) const override;

        static std::vector<FixedRateCoupon> calculateNotionals(
            const std::vector<QuantLib::Date>& dates, const std::vector<Redemption>& redemptions,
            const QuantLib::InterestRate& rate, double notional);

        static void sortRedemptions(std::vector<Redemption>& redemptions);
    };
}  // namespace Atlas

#endif /* A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D */
