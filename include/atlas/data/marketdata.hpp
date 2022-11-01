/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef AFBFF92A_3FC8_47D1_8AA5_E29304D51829
#define AFBFF92A_3FC8_47D1_8AA5_E29304D51829

#include <ql/time/date.hpp>
#include <map>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace Atlas {

    using namespace QuantLib;

    template <class A, class B, class C>
    using DoubleMap = std::unordered_map<A, std::unordered_map<B, C>>;
    /*
     * dado que el indice indica la tasa ya calculada, solo se necesita guardar cada una en el vector
     */
    struct MarketRequest {
        struct Rate {
            Date startDate_;
            Date endDate_;
            std::string curve_;
            Rate(const Date& startDate, const Date& endDate, const std::string& referenceCurve)
            : startDate_(startDate), endDate_(endDate), curve_(referenceCurve) {}
        };

        struct DiscountFactor {
            Date date_;
            std::string discountCurve_;
            DiscountFactor(const Date& date, const std::string& discountCurve) : date_(date), discountCurve_(discountCurve) {}
        };

        /*
         * los factores de descuento solo necesitan la curva y la fecha a descontar
         */
        std::vector<std::tuple<std::string, Date>> dfs;

        /*
         * las tasas forward requieren fecha de inicio y termino
         */
        std::vector<std::tuple<std::string, Date, Date>> fwds;
    };

    struct MarketData {
        Date refDate;
        double numerarie = 1;
        std::vector<double> dfs;
        std::vector<double> fwds;

        void allocate(const MarketRequest& marketRequest);

        void initialize();
    };

    struct HistoricalData {
        DoubleMap<std::string, Date, double> fwds;
    };

}  // namespace atlas

#endif /* AFBFF92A_3FC8_47D1_8AA5_E29304D51829 */
