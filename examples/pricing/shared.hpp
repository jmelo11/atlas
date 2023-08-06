#ifndef CE447B1A_BDE9_4B75_BC43_82F3BDFE4314
#define CE447B1A_BDE9_4B75_BC43_82F3BDFE4314

#include <atlas/fundation/marketstore.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <atlas/visitors/cashflowaggregation/instrumentcashflowsconstvisitor.hpp>

namespace PricingExample {

    using namespace Atlas;

    inline void printCenteredBlock(const std::string& text, int blockWidth = 40) {
        int padding = blockWidth - text.size();
        if (padding < 0) { padding = 0; }
        int paddingLeft  = padding / 2;
        int paddingRight = padding - paddingLeft;

        std::cout << std::string(blockWidth, '=') << '\n';
        std::cout << std::string(paddingLeft, ' ') << text << std::string(paddingRight, ' ') << '\n';
        std::cout << std::string(blockWidth, '=') << '\n';
    }

    inline void printLine(const std::string& key, double value, int keyWidth) {
        const std::string sep = ":";
        std::cout << std::left << std::setw(keyWidth) << key << sep << value << '\n';
    }

    void printTable(const InstrumentCashflowsConstVisitor<double>::Results& results) {
        std::map<std::string, std::map<Date, double>> data = {{"redemptions", results.redemptions},
                                                              {"floatingRateCoupons", results.floatingRateCoupons},
                                                              {"fixedRateCoupons", results.fixedRateCoupons},
                                                              {"disbursements", results.disbursements}};

        int colWidth = 20;  // adjust based on your needs

        // Get all dates
        std::set<Date> allDates;
        for (const auto& col : data) {
            for (const auto& dateValuePair : col.second) { allDates.insert(dateValuePair.first); }
        }

        // Print headers
        std::cout << std::left << std::setw(colWidth) << "Date";
        for (const auto& col : data) { std::cout << std::setw(colWidth) << col.first; }
        std::cout << "\n";

        // Iterate over all dates
        for (const auto& date : allDates) {
            std::cout << std::left << std::setw(colWidth) << parseDate(date);
            for (const auto& col : data) {
                const auto& dateValueMap = col.second;
                auto it                  = dateValueMap.find(date);
                if (it != dateValueMap.end()) {
                    std::cout << std::setw(colWidth) << it->second;
                } else {
                    std::cout << std::setw(colWidth) << "-";  // print "N/A" if date doesn't exist in this column
                }
            }
            std::cout << "\n";
        }
    }

    inline MarketStore<double> createStore(const Date& refDate) {
        double rate = 0.03;
        double fx   = 800;

        DayCounter dc           = Actual360();
        Compounding compounding = Compounding::Simple;
        Frequency frequency     = Frequency::Annual;

        YieldTermStructure curve = FlatForwardTermStructure(refDate, rate, dc, compounding, frequency);
        InterestRateIndex index(Frequency::Annual);
        MarketStore store(refDate, CLP());
        store.curveManager().addCurveContext("ExampleCurve", curve, index, CLP());
        store.fxManager().addExchangeRate(CLP(), USD(), fx);
        return store;
    }
}  // namespace PricingExample

#endif /* CE447B1A_BDE9_4B75_BC43_82F3BDFE4314 */
