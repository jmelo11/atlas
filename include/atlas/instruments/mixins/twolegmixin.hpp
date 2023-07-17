#ifndef D5FE712D_E293_4049_9A71_A2C0C44665A8
#define D5FE712D_E293_4049_9A71_A2C0C44665A8

namespace Atlas {

    /**
     * @brief A mixin class for two legs of an instrument
     *
     * @tparam FirstLeg<adouble>
     * @tparam SecondLeg<adouble>
     */
    template <template <typename> class FirstLeg, template <typename> class SecondLeg, typename adouble>
    class TwoLegMixin {
       public:
        TwoLegMixin() = default;

        TwoLegMixin(const FirstLeg<adouble>& firstLeg, const SecondLeg<adouble>& secondLeg) : firstLeg_(firstLeg), secondLeg_(secondLeg) {}

        const FirstLeg<adouble>& firstLeg() const { return firstLeg_; }

        FirstLeg<adouble>& firstLeg() { return firstLeg_; }

        const SecondLeg<adouble>& secondLeg() const { return secondLeg_; }

        SecondLeg<adouble>& secondLeg() { return secondLeg_; }

       protected:
        FirstLeg<adouble> firstLeg_;
        SecondLeg<adouble> secondLeg_;
    };
}  // namespace Atlas

#endif /* D5FE712D_E293_4049_9A71_A2C0C44665A8 */
