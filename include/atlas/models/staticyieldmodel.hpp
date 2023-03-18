#ifndef AA32C5FD_25FD_4E54_B19F_3D8E3EF02A47
#define AA32C5FD_25FD_4E54_B19F_3D8E3EF02A47

#include <ql/interestrate.hpp>
#include <atlas/models/model.hpp>

namespace Atlas {
    class StaticYieldModel : public Model {
       public:
        StaticYieldModel(const MarketRequest& marketRequest, const InterestRate& yield, const Date& refDate = Date());

        StaticYieldModel(const MarketRequest& marketRequest, const Date& refDate = Date());

        void simulate(const std::vector<Date>& evalDates, Scenario& scenario) const override;

        void yield(const InterestRate& y) { yield_ = y; };

        InterestRate yield() const { return yield_; };

        MarketData simulate() const;

       private:
        void simulateDiscounts(const Date& refDate, MarketData& md) const;

        void simulateForwards(const Date& refDate, MarketData& md) const;

        InterestRate yield_;
        Date refDate_;
    };
}  // namespace Atlas

#endif /* AA32C5FD_25FD_4E54_B19F_3D8E3EF02A47 */
