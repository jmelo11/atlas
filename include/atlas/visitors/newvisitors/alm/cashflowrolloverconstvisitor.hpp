#ifndef B728D291_2112_4242_838F_B908CB005E79
#define B728D291_2112_4242_838F_B908CB005E79

#include <atlas/visitors/newvisitors/basevisitor.hpp>

namespace Atlas {

    template<typename adouble>
    class CashflowRolloverConstVisitor : public BaseConstVisitor<adouble> {
        CashflowRolloverConstVisitor(const Date& evalDate) : evalDate_(evalDate){};

        void evalDate(const Date& evalDate) { evalDate_ = evalDate; }

        Date evalDate() const { return evalDate_; }

        void operator()(const CustomFixedRateInstrument<adouble>& instrument) {
            
        }

        template <class T>
        T getResults() const { return rollOverResult_; }

       private:
        
        Date evalDate_;
    }
}  // namespace Atlas

#endif /* B728D291_2112_4242_838F_B908CB005E79 */
