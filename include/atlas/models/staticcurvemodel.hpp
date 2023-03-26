/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef DE0DE97B_4D3F_4EFB_8C7F_9D85C4CDE288
#define DE0DE97B_4D3F_4EFB_8C7F_9D85C4CDE288

#include <atlas/models/model.hpp>
#include <atlas/rates/curvecontextstore.hpp>
#include <map>

namespace Atlas {

    class StaticCurveModel : public Model {
       public:
        StaticCurveModel(const MarketRequest& marketRequest);

        void setRequest(const MarketRequest& request) { marketRequest_ = request; };

        void simulate(const std::vector<Date>& evalDates, Scenario& scenario) const override;

        MarketData simulate(const Date& refDate) const;

       private:
        void simulateDiscounts(MarketData& md) const;

        void simulateForwards(MarketData& md) const;

        bool checkRefDates();

        const CurveContextStore& curveStore_;
    };
}  // namespace Atlas

#endif /* DE0DE97B_4D3F_4EFB_8C7F_9D85C4CDE288 */
