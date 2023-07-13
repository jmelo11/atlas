#ifndef B22F9696_C417_4ADC_8D6B_634A83E051BF
#define B22F9696_C417_4ADC_8D6B_634A83E051BF

#include <atlas/visitors/newvisitors/basevisitor.hpp>

namespace Atlas {
    class InstrumentRolloverConstVisitor : public BaseConstVisitor {
        InstrumentRolloverConstVisitor() = default;

        InstrumentVariant getResults() const { return rollOverResult_; }

        private:
        InstrumentVariant rollOverResult_;
    }
}  // namespace Atlas

#endif /* B22F9696_C417_4ADC_8D6B_634A83E051BF */
