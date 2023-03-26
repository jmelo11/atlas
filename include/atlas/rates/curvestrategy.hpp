#ifndef B5CEBFBE_528A_462C_A14A_5663327AD130
#define B5CEBFBE_528A_462C_A14A_5663327AD130

#include <ql/termstructures/yield/discountcurve.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/atlasconfig.hpp>

namespace Atlas {

    /**
     * @brief A base class for curve strategies.
     */
    class CurveStrategy {
       public:
        /**
         * @brief Virtual destructor for `CurveStrategy`.
         */
        virtual ~CurveStrategy() = default;

        /**
         * @brief Returns a reference to the yield term structure of the curve.
         *
         * @return A reference to the yield term structure of the curve.
         */
        virtual const QuantLib::YieldTermStructure& curve() const = 0;

        /**
         * @brief Creates a copy of the `CurveStrategy` object.
         *
         * @return A unique pointer to the copied `CurveStrategy` object.
         */
        virtual std::unique_ptr<CurveStrategy> copy() const = 0;
    };

    /**
     * @brief A curve strategy based on a discount curve.
     */
    class DiscountCurveStrategy : public CurveStrategy {
       public:
        /**
         * @brief Constructs a new `DiscountCurveStrategy` object.
         *
         * @param dates The vector of curve dates.
         * @param discounts The vector of corresponding discount factors.
         * @param dayCounter The day counter used to calculate time between dates.
         */
        DiscountCurveStrategy(const std::vector<Date>& dates, const std::vector<double>& discounts, DayCounter dayCounter)
        : curve_(std::make_unique<QuantLib::DiscountCurve>(dates, discounts, dayCounter)) {}

        /**
         * @brief Copy constructor for `DiscountCurveStrategy`.
         *
         * @param other The `DiscountCurveStrategy` object to be copied.
         */
        DiscountCurveStrategy(const DiscountCurveStrategy& other) : curve_(std::make_unique<QuantLib::DiscountCurve>(*other.curve_)) {}

        /**
         * @brief Assignment operator overload for `DiscountCurveStrategy`.
         *
         * @param other The `DiscountCurveStrategy` object to be assigned.
         * @return A reference to the assigned `DiscountCurveStrategy` object.
         */
        DiscountCurveStrategy& operator=(const DiscountCurveStrategy& other) {
            if (this != &other) { curve_ = std::make_unique<QuantLib::DiscountCurve>(*other.curve_); }
            return *this;
        }

        /**
         * @brief Creates a copy of the `DiscountCurveStrategy` object.
         *
         * @return A unique pointer to the copied `CurveStrategy` object.
         */
        std::unique_ptr<CurveStrategy> copy() const override {
            std::vector<Date> dates(curve_->dates().begin(), curve_->dates().end());
            std::vector<double> discounts(curve_->data().begin(), curve_->data().end());
            DayCounter dayCounter = curve_->dayCounter();
            std::unique_ptr<CurveStrategy> copy_(new DiscountCurveStrategy(dates, discounts, dayCounter));
            return copy_;
        }

        /**
         * @brief Returns a reference to the yield term structure of the discount curve.
         *
         * @return A reference to the yield term structure of the discount curve.
         */
        const QuantLib::YieldTermStructure& curve() const override { return *curve_; };

       private:
        std::unique_ptr<QuantLib::DiscountCurve> curve_; /**< The discount curve managed by this object. */
    };

    /**
     * @brief A curve strategy based on a flat forward yield curve.
     */
    class FlatForwardStrategy : public CurveStrategy {
       public:
        /**
         * @brief Constructs a new `FlatForwardStrategy` object.
         *
         * @param referenceDate The reference date of the curve.
         * @param forward The flat forward rate of the curve.
         * @param dayCounter The day counter used to calculate time between dates.
         * @param compounding The compounding frequency of the curve.
         * @param frequency The frequency of the curve.
         */
        FlatForwardStrategy(const Date& referenceDate, double forward = 0.03, const DayCounter& dayCounter = Actual360(),
                            Compounding compounding = Compounding::Continuous, Frequency frequency = Frequency::Annual)
        : referenceDate_(referenceDate), forward_(forward), dayCounter_(dayCounter), compounding_(compounding), frequency_(frequency) {
            curve_ = std::make_unique<QuantLib::FlatForward>(referenceDate_, forward_, dayCounter_, compounding_, frequency_);
        };

        /**
         * @brief Copy constructor for `FlatForwardStrategy`.
         *
         * @param other The `FlatForwardStrategy` object to be copied.
         */
        FlatForwardStrategy(const FlatForwardStrategy& other)
        : referenceDate_(other.referenceDate_),
          forward_(other.forward_),
          dayCounter_(other.dayCounter_),
          compounding_(other.compounding_),
          frequency_(other.frequency_) {
            curve_ = std::make_unique<QuantLib::FlatForward>(*other.curve_);
        };

        /**
         * @brief Assignment operator overload for `FlatForwardStrategy`.
         *
         * @param other The `FlatForwardStrategy` object to be assigned.
         * @return A reference to the assigned `FlatForwardStrategy` object.
         */
        FlatForwardStrategy& operator=(const FlatForwardStrategy& other) {
            if (this != &other) {
                referenceDate_ = other.referenceDate_;
                forward_       = other.forward_;
                dayCounter_    = other.dayCounter_;
                compounding_   = other.compounding_;
                frequency_     = other.frequency_;
                curve_         = std::make_unique<QuantLib::FlatForward>(*other.curve_);
            }
            return *this;
        }

        /**
         * @brief Creates a copy of the `FlatForwardStrategy` object.
         *
         * @return A unique pointer to the copied `CurveStrategy` object.
         */
        std::unique_ptr<CurveStrategy> copy() const override {
            std::unique_ptr<CurveStrategy> copy_(new FlatForwardStrategy(referenceDate_, forward_, dayCounter_, compounding_, frequency_));
            return copy_;
        }

        /**
         * @brief Returns a reference to the yield term structure of the flat forward curve.
         *
         * @return A reference to the yield term structure of the flat forward curve.
         */
        const QuantLib::YieldTermStructure& curve() const override { return *curve_; };

       private:
        Date referenceDate_;
        double forward_;
        DayCounter dayCounter_;
        Compounding compounding_;
        Frequency frequency_;

        std::unique_ptr<QuantLib::FlatForward> curve_;
    };

}  // namespace Atlas

#endif /* B5CEBFBE_528A_462C_A14A_5663327AD130 */
