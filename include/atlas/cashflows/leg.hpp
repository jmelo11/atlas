#ifndef FCBEE32E_AB43_4796_A9DB_9E6108A27EF5
#define FCBEE32E_AB43_4796_A9DB_9E6108A27EF5

#include <atlas/cashflows/cashflow.hpp>
#include <string>
#include <vector>

namespace Atlas {

    class Leg {
       public:
        Leg(){};

        Leg(std::vector<Cashflow> cashflows) : cashflows_(cashflows){};

        Leg(std::vector<Cashflow> cashflows, const std::string& discountCurve,
            const std::string& forecastCurve)
        : cashflows_(cashflows), discountCurve_(discountCurve), forecastCurve_(forecastCurve) {}

        void discountCurve(const std::string& curve) { discountCurve_ = curve; }

        const std::string& discountCurve() const { return discountCurve_; }

        void forecastCurve(const std::string& curve) { forecastCurve_ = curve; }

        const std::string& forecastCurve() const { return forecastCurve_; }
        
        std::vector<Cashflow> cashflows() const { return cashflows_; }
        
       private:
        std::vector<Cashflow> cashflows_;
        std::string discountCurve_ = "undefined";
        std::string forecastCurve_ = "undefined";
    };

}  // namespace Atlas

#endif /* FCBEE32E_AB43_4796_A9DB_9E6108A27EF5 */
