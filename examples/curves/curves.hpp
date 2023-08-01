#ifndef B8FBB453_7DF9_4E75_B42B_17EADE712A0A
#define B8FBB453_7DF9_4E75_B42B_17EADE712A0A

#include <atlas/atlasconfig.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>

namespace CurveExample {
    using namespace Atlas;

    template <class Curve>
    inline void discountPrinter(Curve curve) {
        std::vector<double> times = {0.0, 1.0, 2.0, 3.0, 4.0};
        for (auto t : times) { std::cout << "t = " << t << ", discount(t) = " << curve.discount(t) << std::endl; }
    }

    template <class Curve>
    inline void forwardRatePrinter(Curve curve) {
        std::vector<double> times = {0.0, 1.0, 2.0, 3.0, 4.0};
        DayCounter dc             = Actual360();
        Compounding comp          = Compounding::Compounded;
        Frequency freq            = Frequency::Annual;
        for (size_t i = 0; i < times.size() - 1; ++i) {
            Date start = curve.refDate() + Period(times[i], TimeUnit::Years);
            Date end   = curve.refDate() + Period(times[i + 1], TimeUnit::Years);
            std::cout << "t = " << times[i] << ", forwardRate(t0, t1) = " << curve.forwardRate(start, end, dc, comp, freq) << std::endl;
        }
    }

    inline void usageExample() {
        double curveRate  = 0.05;
        Date curveRefDate = Date(1, Month::Jan, 2020);

        YieldTermStructure flatForward = FlatForwardTermStructure(curveRefDate, curveRate);
        discountPrinter(flatForward);
        forwardRatePrinter(flatForward);
    }
}  // namespace CurveExample

#endif /* B8FBB453_7DF9_4E75_B42B_17EADE712A0A */
