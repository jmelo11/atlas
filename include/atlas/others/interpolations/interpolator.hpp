#ifndef DA41E45F_4B57_4DCA_A205_48A7439B2623
#define DA41E45F_4B57_4DCA_A205_48A7439B2623

namespace Atlas {

    template <typename adouble>
    class Interpolator {
       public:
        virtual adouble operator()(double x) const = 0;
    };

}  // namespace Atlas

#endif /* DA41E45F_4B57_4DCA_A205_48A7439B2623 */
