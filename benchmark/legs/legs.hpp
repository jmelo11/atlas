#ifndef C0C08D61_4C04_4F93_8145_C380285A0973
#define C0C08D61_4C04_4F93_8145_C380285A0973

#include <atlas/cashflows/legs/fixedrateleg.hpp>
#include <atlas/cashflows/legs/floatingrateleg.hpp>
#include <atlas/cashflows/legs/makeleg.hpp>
#include <atlas/multithreading/threadpool.hpp>

namespace LegsBenchmark {
    using namespace Atlas;

    struct Common {
        Date startDate      = Date(1, Month::Jan, 2020);
        Date endDate        = Date(1, Month::Jan, 2030);
        Frequency frequency = Frequency::Semiannual;
        Side side           = Side::Long;
        double notional     = 100;
    };

    inline void fixedRateLeg(size_t n) {
        Common vars;

        InterestRate rate(0.05, Actual360());

        for (size_t i = 0; i < n; ++i) {
            auto leg = MakeLeg<FixedRateLeg, double>()
                           .startDate(vars.startDate)
                           .endDate(vars.endDate)
                           .notional(vars.notional)
                           .side(vars.side)
                           .paymentFrequency(vars.frequency)
                           .rate(rate)
                           .build();
        }
    };

    inline void floatingRateLeg(size_t n) {
        Common vars;

        InterestRateIndex index(Frequency::Semiannual);

        for (size_t i = 0; i < n; ++i) {
            auto leg = MakeLeg<FloatingRateLeg, double>()
                           .startDate(vars.startDate)
                           .endDate(vars.endDate)
                           .notional(vars.notional)
                           .side(vars.side)
                           .paymentFrequency(vars.frequency)
                           .interestRateIndex(index)
                           .build();
        }
    };

    inline void fixedRateLeg_MT(size_t n) {
        size_t numThreads = std::thread::hardware_concurrency();
        ThreadPool* pool  = ThreadPool::getInstance();
        pool->start();
        std::vector<TaskHandle> futures;
        for (size_t i = 0; i < numThreads; ++i) {
            auto task = [&]() {
                fixedRateLeg(n / numThreads);
                return true;
            };
            futures.push_back(pool->spawnTask(task));
        }
        for (auto& future : futures) { pool->activeWait(future); }
        pool->stop();
    };

    inline void floatingRateLeg_MT(size_t n) {
        size_t numThreads = std::thread::hardware_concurrency();
        ThreadPool* pool  = ThreadPool::getInstance();
        pool->start();
        std::vector<TaskHandle> futures;
        for (size_t i = 0; i < numThreads; ++i) {
            auto task = [&]() {
                floatingRateLeg(n / numThreads);
                return true;
            };
            futures.push_back(pool->spawnTask(task));
        }
        for (auto& future : futures) { pool->activeWait(future); }
        pool->stop();
    };
}  // namespace LegsBenchmark

#endif /* C0C08D61_4C04_4F93_8145_C380285A0973 */
