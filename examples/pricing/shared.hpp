#ifndef CE447B1A_BDE9_4B75_BC43_82F3BDFE4314
#define CE447B1A_BDE9_4B75_BC43_82F3BDFE4314

#include <atlas/fundation/marketstore.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>

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
