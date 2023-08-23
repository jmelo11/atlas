#ifndef C59BCDC6_ECD5_4170_8360_E5A17A0FD65C
#define C59BCDC6_ECD5_4170_8360_E5A17A0FD65C

#include <atlas/rates/index/index.hpp>
#include <atlas/rates/index/ratedefinition.hpp>

namespace Atlas {

    /**
     * @class InterestRateIndex
     * @brief This class has the fixings and rate definition of a given index.
     *
     * @tparam adouble
     */
    template <typename adouble = double>
    class InterestRateIndex : public Index<adouble> {
       public:
        /**
         * @brief Construct a new Interest Rate Index object
         *
         * @param fixingFreq
         * @param rateDef
         */
        InterestRateIndex(Frequency fixingFreq = Frequency::NoFrequency, const RateDefinition& rateDef = RateDefinition())
        : Index<adouble>(fixingFreq), rateDef_(rateDef){};

        /**
         * @brief Construct a new Interest Rate Index object
         *
         * @param tenor
         * @param rateDef
         */
        InterestRateIndex(const Period& tenor, const RateDefinition& rateDef = RateDefinition())
        : Index<adouble>(tenor), rateDef_(rateDef){};

        /**
         * @brief Destructor
         *
         */
        virtual ~InterestRateIndex() = default;

        /**
         * @brief Return the rate definition of the index
         *
         * @return RateDefinition
         */
        inline const RateDefinition& rateDefinition() const { return rateDef_; };

        /**
         * @brief Checks if the index is valid.
         * @details A valid index is one that has a valid rate definition and a fixing frequency different than NoFrequency
         *
         * @return true
         * @return false
         */
        inline bool isValid() const { return this->tenor() != Period(); };

       private:
        RateDefinition rateDef_;
    };
}  // namespace Atlas

#endif /* C59BCDC6_ECD5_4170_8360_E5A17A0FD65C */
