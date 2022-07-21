#pragma once

#include <map>
#include <unordered_map>
#include <any>

#include <memory>
#include <string>


#include "qlconfig.hpp"
#include "configurations.hpp"
#include <iostream>


namespace CurveManager {

	class Index;

	class CurveStore {

	public:
		CurveStore() {};
		std::shared_ptr<YieldTermStructure> getCurve(const Date& refDate, const std::string& name) {
			return curveMap_[refDate][name];
		};
		bool hasCurve(Date refDate, const std::string& name) {
			if (curveMap_.find(refDate) != curveMap_.end())
				return curveMap_[refDate].find(name) != curveMap_[refDate].end();
			else
				return false;
			return false;
		}
		void addCurve(Date date, const std::string& name, std::shared_ptr<YieldTermStructure>& curve) {
			curveMap_[date][name] = std::move(curve);
		}

	private:
		std::map<Date, std::unordered_map<std::string, std::shared_ptr<YieldTermStructure>>> curveMap_;
		std::map<Date, std::unordered_map<std::string, std::shared_ptr<YieldTermStructure>>> indexMap_;
	};

	class CurveBuilder
	{

	public:
		CurveBuilder(CurveStore& curveStore) : curveStore_(curveStore) {}

		/* queries the parameters from db, parse them, build the ratehelpers with the prices and parameters, then build curve */
		std::shared_ptr<YieldTermStructure> get(const Date& refDate, const std::string& name) {
			if (!curveStore_.hasCurve(refDate, name)) {
				std::shared_ptr<BaseConfiguration> configurations = getCurveConfig(refDate, name);
				build(configurations);
			}
			return curveStore_.getCurve(refDate, name);
		};

	private:

		/* builds the curve with the given rate helpers /fixed interpoaltion and data type */
		void build(std::shared_ptr<BaseConfiguration> config) {
			auto& [refDate, type, subType, subConfig] = *config;
			if (type == BaseConfiguration::ConfigType::CURVE)
			{
				/* set system date */
				//Settings::instance().evaluationDate() = refDate;

				std::shared_ptr<CurveConfig> castedConfig = std::static_pointer_cast<CurveConfig>(subConfig);
				auto& [name, dayCounter] = *castedConfig;

				/*build helpers*/
				auto rateHelperConfigs = getRateHelperConfigs(refDate, name);
				std::vector<boost::shared_ptr<RateHelper>> helpers;
				for (auto& rhc : rateHelperConfigs) {
					boost::shared_ptr<RateHelper> tmp = buildHelper(rhc);
					helpers.push_back(tmp);
				}

				/* define curve and add to store*/
				PiecewiseYieldCurve<ForwardRate, Linear>curve();
				//std::cout << curve.discount(1) << "\n";
				std::shared_ptr<YieldTermStructure> curvePtr = std::make_shared<PiecewiseYieldCurve<ForwardRate, Linear>>(refDate, helpers, dayCounter);

				/* prevent curve from changing with settings evaldate */
				//curve.unregisterWith(Settings::instance().evaluationDate());
				curveStore_.addCurve(refDate, name, curvePtr);
			}
			else {
				throw std::exception("type is not curve");
			}
		};

		/* queries the parameters and px */
		boost::shared_ptr<RateHelper> buildHelper(std::shared_ptr<BaseConfiguration> config) {
			auto& [refDate, type, subType, subConfig] = *config;
			if (type == BaseConfiguration::ConfigType::RATEHELPER)
			{
				switch (subType)
				{
				case CurveManager::BaseConfiguration::DEPOSIT:
				{
					std::shared_ptr<DepositConfig> castedConfig = std::static_pointer_cast<DepositConfig>(subConfig);
					auto& [rate, tenor, fixingDays, calendar, convention, endOfMonth, dayCounter] = *castedConfig;
					return boost::make_shared<DepositRateHelper>(rate, tenor, fixingDays, calendar, convention, endOfMonth, dayCounter);
				}
				case CurveManager::BaseConfiguration::BOND:{
					std::shared_ptr<FixedBondConfig> castedConfig = std::static_pointer_cast<FixedBondConfig>(subConfig);
					auto& [fwdPoint, spotFx, tenor, fixingDays, calendar, convention, endOfMonth, isFxBaseCurrencyCollateralCurrency, collateralCurve, tradingCalendar] = *castedConfig;
					return boost::make_shared<SwapRateHelper>(fwdPoint, spotFx, tenor, fixingDays, calendar, convention, endOfMonth, isFxBaseCurrencyCollateralCurrency, collateralCurve, tradingCalendar);
				}
				case CurveManager::BaseConfiguration::SWAP:
				{
					std::shared_ptr<SwapConfig> castedConfig = std::static_pointer_cast<SwapConfig>(subConfig);
					auto& [rate, swapIndex, spread, fwdStart, discountingCurve] = *castedConfig;
					return boost::make_shared<SwapRateHelper>(rate, swapIndex, spread, fwdStart, discountingCurve);
				}
				case CurveManager::BaseConfiguration::FXSWAP:
				{
					std::shared_ptr<FxSwapConfig> castedConfig = std::static_pointer_cast<FxSwapConfig>(subConfig);
					auto& [fwdPoint, spotFx, tenor, fixingDays, calendar, convention, endOfMonth, isFxBaseCurrencyCollateralCurrency, collateralCurve, tradingCalendar] = *castedConfig;
					return boost::make_shared<SwapRateHelper>(fwdPoint, spotFx, tenor, fixingDays, calendar, convention, endOfMonth, isFxBaseCurrencyCollateralCurrency, collateralCurve, tradingCalendar);
				}
				case CurveManager::BaseConfiguration::OIS: {
					std::shared_ptr<OISRateConfig> castedConfig = std::static_pointer_cast<OISRateConfig>(subConfig);
					auto& [settlementDays,
						tenor,
						fixedRate,
						overnightIndex,
						discountingCurve,
						telescopicValueDates,
						paymentLag,
						paymentConvention,
						paymentFrequency,
						paymentCalendar,
						forwardStart,
						overnightSpread] = *castedConfig;
					return boost::make_shared<SwapRateHelper>(settlementDays,
						tenor,
						fixedRate,
						overnightIndex,
						discountingCurve,
						telescopicValueDates,
						paymentLag,
						paymentConvention,
						paymentFrequency,
						paymentCalendar,
						forwardStart,
						overnightSpread);
				}
				case CurveManager::BaseConfiguration::XCCY:{
					std::shared_ptr<XCCYRateConfig> castedConfig = std::static_pointer_cast<XCCYRateConfig>(subConfig);
					auto& [basis,
					tenor,
					fixingDays,
					calendar,
					convention,
					endOfMonth,
					baseCurrencyIndex,
					quoteCurrencyIndex,
					collateralCurve,
					isFxBaseCurrencyCollateralCurrency,
					isBasisOnFxBaseCurrencyLeg ] = *castedConfig;
					return boost::make_shared<ConstNotionalCrossCurrencyBasisSwapRateHelper>(basis,
						tenor,
						fixingDays,
						calendar,
						convention,
						endOfMonth,
						baseCurrencyIndex,
						quoteCurrencyIndex,
						collateralCurve,
						isFxBaseCurrencyCollateralCurrency,
						isBasisOnFxBaseCurrencyLeg);
				}
				case CurveManager::BaseConfiguration::NONE:
					break;

				default:
					break;
				}
			}
			else {
				throw std::exception("type is not ratehelper");
			}
		}

		/* this method gets the config and prices from the db for each rate helper in the curve*/
		std::vector<std::shared_ptr<BaseConfiguration>> getRateHelperConfigs(const Date& refDate, const std::string& name) {
			std::vector<std::shared_ptr<BaseConfiguration>> configurations;
			DepositConfig depoConfig1{ .rate = 0.01, .tenor = Period(7, TimeUnit::Days) };
			std::cout << depoConfig1.tenor << "\n";

			BaseConfiguration config1{ .refDate = Date(1,Month::Apr, 2021),
										.type = BaseConfiguration::ConfigType::RATEHELPER,
										.subType = BaseConfiguration::ConfigSubType::DEPOSIT,
										.subConfig = std::make_shared<DepositConfig>(depoConfig1) };
			configurations.push_back(std::make_shared<BaseConfiguration>(config1));

			for (size_t i = 1; i < 5; i++)
			{
				DepositConfig depoConfig{ .rate = 0.01 + i * 2 / 100, .tenor = Period(i, TimeUnit::Years) };
				std::cout << depoConfig.tenor << "\n";

				BaseConfiguration config{ .refDate = Date(1,Month::Apr, 2021),
											.type = BaseConfiguration::ConfigType::RATEHELPER,
											.subType = BaseConfiguration::ConfigSubType::DEPOSIT,
											.subConfig = std::make_shared<DepositConfig>(depoConfig) };

				configurations.push_back(std::make_shared<BaseConfiguration>(config));
			}
			return configurations;
		};

		/* this method gets the config for each curve*/
		std::shared_ptr<BaseConfiguration> getCurveConfig(const Date& refDate, const std::string& name) {
			CurveConfig curveConfig{ .name = name, .dayCounter = Act360() };
			BaseConfiguration config{ .refDate = refDate,
				.type = BaseConfiguration::ConfigType::CURVE,
				.subType = BaseConfiguration::ConfigSubType::NONE,
				.subConfig = std::make_shared<CurveConfig>(curveConfig) };
			return std::make_shared<BaseConfiguration>(config);
		};

		CurveStore& curveStore_;
	};
}


