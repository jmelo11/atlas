#include "../pch.hpp"
#include <atlas/atlasconfig.hpp>
#include <atlas/fundation/exchangeratemanager.hpp>

using namespace Atlas;

struct ExchangeManagerSetup {
    ExchangeManagerSetup() {
        Date refDate(1, Month::Apr, 2020);
        manager = new ExchangeRateManager<double>(refDate);
        manager->addExchangeRate(USD(), EUR(), 0.9);
        manager->addExchangeRate(EUR(), GBP(), 0.8);
        manager->addExchangeRate(GBP(), USD(), 1.2);
    };

    ExchangeRateManager<double>* manager;
};

TEST(ExchangeRateManager, DirectRate) {
    ExchangeManagerSetup setup;
    auto manager = setup.manager;
    auto rate    = manager->exchange(USD(), EUR());
    EXPECT_EQ(rate, 0.9);

    rate = manager->exchange(EUR(), GBP());
    EXPECT_EQ(rate, 0.8);

    rate = manager->exchange(GBP(), USD());
    EXPECT_EQ(rate, 1.2);

    rate = manager->exchange(GBP(), GBP());
    EXPECT_EQ(rate, 1.0);
}

TEST(ExchangeRateManager, Inverse) {
    ExchangeManagerSetup setup;
    auto manager = setup.manager;
    auto rate    = manager->exchange(EUR(), USD());
    EXPECT_NEAR(rate, 1.0 / 0.9, 1e-6);

    rate = manager->exchange(GBP(), EUR());
    EXPECT_NEAR(rate, 1.0 / 0.8, 1e-6);

    rate = manager->exchange(USD(), GBP());
    EXPECT_NEAR(rate, 1.0 / 1.2, 1e-6);
}

TEST(ExchangeRateManager, ChainedExchangeRates) {
    ExchangeManagerSetup setup;
    auto manager = setup.manager;

    manager->addExchangeRate(USD(), JPY(), 110.0);

    // Test chained exchange rate: EUR -> USD -> JPY
    auto rate = manager->exchange(EUR(), JPY());
    EXPECT_NEAR(rate, 110.0 / 0.9, 1e-6);

    // Test chained exchange rate: GBP -> EUR -> USD -> JPY
    rate = manager->exchange(GBP(), JPY());
    EXPECT_NEAR(rate, 1.2 * 110.0, 1e-6);
}

TEST(ExchangeRateManager, ArbitrageDetection) {
    // No arbitrage case
    ExchangeManagerSetup setup;
    auto manager = setup.manager;

    manager->addExchangeRate(USD(), EUR(), 0.9);
    manager->addExchangeRate(EUR(), GBP(), 0.8);
    manager->addExchangeRate(GBP(), USD(), 1.2);
    std::vector<size_t> arbitrageCycle = manager->detectArbitrage();
    EXPECT_TRUE(arbitrageCycle.empty());

    // Arbitrage case
    manager->addExchangeRate(USD(), EUR(), 0.9);
    manager->addExchangeRate(EUR(), GBP(), 1.5);
    manager->addExchangeRate(GBP(), USD(), 1.2);
    arbitrageCycle = manager->detectArbitrage();
    EXPECT_FALSE(arbitrageCycle.empty());

    // Check if the arbitrage cycle has the correct currencies
    size_t usdCode = USD().numericCode();
    size_t eurCode = EUR().numericCode();
    size_t gbpCode = GBP().numericCode();
    std::unordered_set<size_t> expectedCurrencies = {usdCode, eurCode, gbpCode};
    std::unordered_set<size_t> detectedCurrencies(arbitrageCycle.begin(), arbitrageCycle.end());
    EXPECT_EQ(expectedCurrencies, detectedCurrencies);
}
