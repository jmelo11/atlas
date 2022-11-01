/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef E8ADA272_8AD5_4171_8E4A_A5E4B0113874
#define E8ADA272_8AD5_4171_8E4A_A5E4B0113874

#include <ql/interestrate.hpp>
#include <ql/time/schedule.hpp>
#include <atlas/instruments/instruments.hpp>

namespace Atlas {

    using namespace QuantLib;

    class FixFloatSwap : public MultiLegInstrument {
       public:
        enum Side { Payer, Receiver };

        FixFloatSwap(Side side, const Date& startDate, const Date& endDate, const InterestRate& rate, Frequency fixLegFrequency,
                     Frequency floatLegFrequency, double faceAmount, double spread, MetaData& metaData)
        : MultiLegInstrument(), spread_(spread), side_(side), rate_(rate) {
            faceAmount_ = faceAmount;

            Schedule fixSchedule = MakeSchedule().from(startDate).to(endDate).withFrequency(fixLegFrequency);
            std::vector<double> fixRedemptions(fixSchedule.size() - 2, 0);
            fixRedemptions.push_back(faceAmount);

            Leg fixLeg(fixSchedule.dates(), fixRedemptions);
            fixLeg.metaData(metaData);

            Schedule floatSchedule = MakeSchedule().from(startDate).to(endDate).withFrequency(floatLegFrequency);
            std::vector<double> floatRedemptions(floatSchedule.size() - 2, 0);
            floatRedemptions.push_back(faceAmount);

            Leg floatLeg(floatSchedule.dates(), floatRedemptions);
            floatLeg.metaData(metaData);

            legs_.push_back(fixLeg);
            legs_.push_back(floatLeg);
        }

        Side side() const { return side_; }

        double spread() const { return spread_; }

        void rate(InterestRate& rate) { rate_ = rate; }

        const InterestRate& rate() const { return rate_; };

        void accept(Visitor& visitor) override;

        void accept(ConstVisitor& visitor) const override;

       private:
        InterestRate rate_;
        double spread_;
        Side side_;
    };
}  // namespace atlas

#endif /* E8ADA272_8AD5_4171_8E4A_A5E4B0113874 */
