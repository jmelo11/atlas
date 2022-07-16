#pragma once
#include "instruments.hpp"

namespace atlas{

    /*
    * asume que la frecuencia de la tasa siempre es anual
    */
    struct RateIndex {
        const char* name_;
        DayCounter dayCounter_;
        Frequency freq_;
        Compounding comp_;
    };

    class FloatingRateInstrument :
        public CashFlowInstrument
    {
    public:
        FloatingRateInstrument(const Date& start, const Date& end, double faceAmount, double spread, RateIndex& index) : CashFlowInstrument(start, end, index.freq_, faceAmount), index_(index), spread_(spread) {}
        FloatingRateInstrument(std::vector<Date>& dates, RateIndex& index) : CashFlowInstrument(dates), index_(index) {}
        FloatingRateInstrument(RateIndex& index) : CashFlowInstrument(), index_(index) {};
        std::vector<size_t>* fwdRateIdx() {
            return &fwdRateIdx_;
        }
        RateIndex& index() {
            return index_;
        }
        double spread() const {
            return spread_;
        }
        void spread(double s) {
            spread_ = s;
        }
        void accept(Visitor& visitor) override;
        void accept(ConstVisitor& visitor) const override;

    protected:
        double spread_ = 0;
        RateIndex& index_;
        std::vector<size_t> fwdRateIdx_;
    };

    class FloatingBullet: public FloatingRateInstrument
    {
    public:
        FloatingBullet(const Date& start, const Date& end, double faceAmount, double spread, RateIndex& index) : FloatingRateInstrument(start, end, faceAmount, spread, index){
            std::vector<double>redemp(dates_.size()-2, 0);
            redemp.push_back(faceAmount);
            redemptions_ = redemp;
        };
       
        void accept(Visitor& visitor) override;
        void accept(ConstVisitor& visitor) const override;    
    };  
}
