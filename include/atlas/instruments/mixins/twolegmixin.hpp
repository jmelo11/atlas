#ifndef D5FE712D_E293_4049_9A71_A2C0C44665A8
#define D5FE712D_E293_4049_9A71_A2C0C44665A8

namespace Atlas {

    /**
     * @class TwoLegMixin
     * @brief A mixin class for two legs of an instrument
     *
     * @tparam FirstLeg<adouble>
     * @tparam SecondLeg<adouble>
     */
    template <template <typename> class FirstLeg, template <typename> class SecondLeg, typename adouble>
    class TwoLegMixin {
       public:
        /**
         * @brief Construct a new Two Leg Mixin object
         *
         */
        TwoLegMixin() = default;

        /**
         * @brief Construct a new Two Leg Mixin object
         *
         * @param firstLeg
         * @param secondLeg
         */
        TwoLegMixin(const FirstLeg<adouble>& firstLeg, const SecondLeg<adouble>& secondLeg) : firstLeg_(firstLeg), secondLeg_(secondLeg) {}

        /**
         * @brief Returns the first leg of the instrument.
         *
         * @return const FirstLeg<adouble>&
         */
        const FirstLeg<adouble>& firstLeg() const { return firstLeg_; }

        /**
         * @brief Returns the first leg of the instrument.
         *
         * @return FirstLeg<adouble>&
         */
        FirstLeg<adouble>& firstLeg() { return firstLeg_; }

        /**
         * @brief Returns the second leg of the instrument.
         *
         * @return const SecondLeg<adouble>&
         */
        const SecondLeg<adouble>& secondLeg() const { return secondLeg_; }

        /**
         * @brief Returns the second leg of the instrument.
         *
         * @return SecondLeg<adouble>&
         */
        SecondLeg<adouble>& secondLeg() { return secondLeg_; }

       protected:
        FirstLeg<adouble> firstLeg_;
        SecondLeg<adouble> secondLeg_;
    };
}  // namespace Atlas

#endif /* D5FE712D_E293_4049_9A71_A2C0C44665A8 */
