/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef DE0DE97B_4D3F_4EFB_8C7F_9D85C4CDE288
#define DE0DE97B_4D3F_4EFB_8C7F_9D85C4CDE288

#include <ql/termstructures/yieldtermstructure.hpp>
#include <atlas/models/model.hpp>
#include <unordered_map>

namespace Atlas {

    using CurveMap = std::unordered_map<std::string, std::shared_ptr<QuantLib::YieldTermStructure>>;

    class StaticCurveModel : public Model {
       public:
        StaticCurveModel(const MarketRequest& marketRequest, CurveMap discountCurves = CurveMap(), CurveMap forecastCurves = CurveMap(),
                         DoubleMap<std::string, QuantLib::Date, double> historicalData = DoubleMap<std::string, QuantLib::Date, double>());

        void addDiscountCurve(const std::string& curveName, std::shared_ptr<QuantLib::YieldTermStructure> curve) {
            discountCurves_[curveName] = curve;
        }

        void addForecastCurve(const std::string& curveName, std::shared_ptr<QuantLib::YieldTermStructure> curve) {
            forecastCurves_[curveName] = curve;
        }

        void setRequest(const MarketRequest& request) { marketRequest_ = request; }

        void simulate(const std::vector<QuantLib::Date>& evalDates, Scenario& scenario) const override;

        MarketData simulate() const;
        /*
         * falta check si la fecha requerida es anterior a la de la curva
         */
       private:
        void simulateDiscounts(MarketData& md) const;

        void simulateForwards(MarketData& md) const;

        bool checkRefDates();

        QuantLib::Date refDate_;
        CurveMap discountCurves_;
        CurveMap forecastCurves_;
        DoubleMap<std::string, QuantLib::Date, double> historicalData_;
    };
}  // namespace Atlas

#endif /* DE0DE97B_4D3F_4EFB_8C7F_9D85C4CDE288 */
