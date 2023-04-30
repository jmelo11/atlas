#ifndef D5FE712D_E293_4049_9A71_A2C0C44665A8
#define D5FE712D_E293_4049_9A71_A2C0C44665A8

namespace Atlas {

    /**
     * @brief A mixin class for two legs of an instrument
     *
     * @tparam FirstLeg
     * @tparam SecondLeg
     */
    template <class FirstLeg, class SecondLeg>
    class TwoLegMixin {
       public:
        TwoLegMixin() = default;

        TwoLegMixin(const FirstLeg& firstLeg, const SecondLeg& secondLeg) : firstLeg_(firstLeg), secondLeg_(secondLeg) {}

        const FirstLeg& firstLeg() const { return firstLeg_; }

        FirstLeg& firstLeg() { return firstLeg_; }

        const SecondLeg& secondLeg() const { return secondLeg_; }

        SecondLeg& secondLeg() { return secondLeg_; }

       private:
        FirstLeg firstLeg_;
        SecondLeg secondLeg_;
    };
}  // namespace Atlas

#endif /* D5FE712D_E293_4049_9A71_A2C0C44665A8 */
