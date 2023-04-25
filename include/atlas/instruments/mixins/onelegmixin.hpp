#ifndef C3DACA02_A13E_4C45_95EE_D1B496237D23
#define C3DACA02_A13E_4C45_95EE_D1B496237D23

namespace Atlas {

    /**
     * @brief A class for single leg instruments.
     *
     * @tparam adouble number type
     * @tparam FirstLeg type of the first leg
     */
    template <class FirstLeg>
    class OneLegMixin {
       public:
        OneLegMixin() = default;

        OneLegMixin(const FirstLeg& leg) : leg_(leg){};

        inline const FirstLeg& leg() const { return leg_; }

        inline FirstLeg& leg() { return leg_; }

       private:
        FirstLeg leg_;
    };
}  // namespace Atlas

#endif /* C3DACA02_A13E_4C45_95EE_D1B496237D23 */
