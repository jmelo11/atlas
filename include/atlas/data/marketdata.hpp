/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef AFBFF92A_3FC8_47D1_8AA5_E29304D51829
#define AFBFF92A_3FC8_47D1_8AA5_E29304D51829

#include <ql/compounding.hpp>
#include <ql/time/date.hpp>
#include <ql/time/daycounters/actual360.hpp>
#include <atlas/atlasconfig.hpp>
#include <map>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace Atlas {
    template <class A, class B, class C>
    using DoubleMap = std::unordered_map<A, std::unordered_map<B, C>>;
    /*
     * dado que el indice indica la tasa ya calculada, solo se necesita guardar cada una en el
     * vector
     */

    struct MarketRequest {
        struct Rate {
            Date startDate_;
            Date endDate_;
            DayCounter dayCounter_;
            Compounding compounding_;
            Frequency frequency_;

            std::string curve_;
            Rate(const std::string& referenceCurve, const Date& startDate, const Date& endDate, const DayCounter& dayCounter = Actual360(),
                 const Compounding& compounding = Compounding::Simple, const Frequency& frequency = Frequency::Annual)
            : startDate_(startDate),
              endDate_(endDate),
              dayCounter_(dayCounter),
              compounding_(compounding),
              frequency_(frequency),
              curve_(referenceCurve){};
        };

        struct DiscountFactor {
            Date date_;
            std::string discountCurve_;
            DiscountFactor(const std::string& discountCurve, const Date& date) : date_(date), discountCurve_(discountCurve) {}
        };

        size_t discountFactorIdx() const { return dfs.size() - 1; }
        size_t forwardRateIdx() const { return fwds.size() - 1; }

        std::vector<DiscountFactor> dfs;

        std::vector<Rate> fwds;
    };

    struct MarketRequest2 {
        struct Rate {
            Date startDate_;
            Date endDate_;
            DayCounter dayCounter_;
            Compounding compounding_;
            Frequency frequency_;
            size_t curve_;
            Rate(const size_t& referenceCurve, const Date& startDate, const Date& endDate, const DayCounter& dayCounter = Actual360(),
                 const Compounding& compounding = Compounding::Simple, const Frequency& frequency = Frequency::Annual)
            : startDate_(startDate),
              endDate_(endDate),
              dayCounter_(dayCounter),
              compounding_(compounding),
              frequency_(frequency),
              curve_(referenceCurve){};
        };

        struct DiscountFactor {
            Date date_;
            size_t discountCurve_;
            DiscountFactor(size_t discountCurve, const Date& date) : date_(date), discountCurve_(discountCurve) {}
        };

        size_t discountFactorIdx() const { return dfs.size() - 1; }
        size_t forwardRateIdx() const { return fwds.size() - 1; }

        std::vector<DiscountFactor> dfs;

        std::vector<Rate> fwds;
    };

    struct MarketData {
        double numerarie = 1;  // under no arbitrage context its ok, but in general it should be a vector?
        Date refDate;
        std::vector<double> dfs;
        std::vector<double> fwds;

        void allocate(const MarketRequest& marketRequest);

        void initialize();
    };

    struct HistoricalData {
        DoubleMap<std::string, Date, double> fwds;
    };

}  // namespace Atlas

#endif /* AFBFF92A_3FC8_47D1_8AA5_E29304D51829 */
