#ifndef FAA98004_22C5_434A_905B_A8FC979A8719
#define FAA98004_22C5_434A_905B_A8FC979A8719

#include <atlas/atlasconfig.hpp>

namespace Atlas {

    /**
     * @class RateDefinition
     * @brief This class holds the information that defines a interest rate index.
     * @details A valid rate definition is one that is not default-constructed.
     */
    class RateDefinition {
       public:
        

        /**
         * @brief Construct a new Rate Definition object
         *
         * @param dayCounter
         * @param frequency
         * @param compounding
         */
        RateDefinition(const DayCounter& dayCounter = Actual360(), Compounding compounding = Compounding::Simple, Frequency frequency = Frequency::Annual)
        : dayCounter_(dayCounter), compounding_(compounding), frequency_(frequency){};

        /**
         * @brief Get the Day Counter object
         *
         * @return const DayCounter&
         */
        inline const DayCounter& dayCounter() const { return dayCounter_; };

        /**
         * @brief Get the rate compounding frequency
         *
         * @return Frequency
         */
        inline Frequency frequency() const { return frequency_; };

        /**
         * @brief Get the rate compounding
         *
         * @return Compounding
         */
        inline Compounding compounding() const { return compounding_; };

       

       private:
        DayCounter dayCounter_   = DayCounter();
        Compounding compounding_ = Compounding::Simple;
        Frequency frequency_     = Frequency::NoFrequency;
    };
}  // namespace Atlas

#endif /* FAA98004_22C5_434A_905B_A8FC979A8719 */
