#ifndef AA32C5FD_25FD_4E54_B19F_3D8E3EF02A47
#define AA32C5FD_25FD_4E54_B19F_3D8E3EF02A47

#include <ql/interestrate.hpp>
#include <atlas/models/model.hpp>

namespace Atlas {
    class StaticYieldModel : public Model {
       public:
        StaticYieldModel(const MarketRequest& marketRequest, const QuantLib::InterestRate& yield, const QuantLib::Date& refDate = QuantLib::Date());

        void simulate(const std::vector<QuantLib::Date>& evalDates, Scenario& scenario) const override;

        MarketData simulate() const;

       private:
        void simulateDiscounts(const QuantLib::Date& refDate, MarketData& md) const;

        void simulateForwards(const QuantLib::Date& refDate, MarketData& md) const;

        QuantLib::InterestRate yield_;
        QuantLib::Date refDate_;
    };
}  // namespace Atlas

#endif /* AA32C5FD_25FD_4E54_B19F_3D8E3EF02A47 */
