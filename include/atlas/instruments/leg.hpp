/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef E4F7ED0C_A61A_41D1_8DF7_A0F52A7228B6
#define E4F7ED0C_A61A_41D1_8DF7_A0F52A7228B6

#include <atlas/curves/rateindex.hpp>
#include <atlas/data/metadata.hpp>
#include <stdexcept>
#include <vector>

namespace Atlas {

    class Leg {
       public:
        Leg(){};

        Leg(const std::vector<Date>& dates) : dates_(dates){};

        Leg(const std::vector<Date>& dates, const std::vector<double>& redemptions) : dates_(dates), redemptions_(redemptions) {
            if (dates.size() - 1 != redemptions.size()) { throw std::runtime_error("Leg: dates must have remdemptions size+1"); };

            size_ = dates.size() - 1;
            interest_.reserve(size_);
            outstanding_.reserve(size_);
            FWDIdx_.reserve(size_);
            DFIdx_.reserve(size_ + 1);
        }

        /* const */
        const Date& startDate() const { return dates_.front(); }

        const Date& endDate() const { return dates_.back(); }

        const std::vector<Date>& dates() const { return dates_; }

        const auto& constDFIdx() const { return DFIdx_; }

        const auto& constFWDIdx() const { return FWDIdx_; }

        const auto& constRedemptions() const { return redemptions_; }

        const auto& constOutstanding() const { return outstanding_; }

        const auto& constInterest() const { return interest_; }

        /* non-const */

        std::vector<double>& outstanding() { return outstanding_; }

        std::vector<double>& redemptions() { return redemptions_; }

        std::vector<double>& interest() { return interest_; }

        std::vector<size_t>& DFIdx() { return DFIdx_; }

        std::vector<size_t>& FWDIdx() { return FWDIdx_; }

        size_t size() const { return size_; }

        void metaData(MetaData& metaData) { metaData_ = metaData; }

        const MetaData& metaData() const { return metaData_; }

        void calculateOutstanding() const {
            outstanding_.clear();
            double amount = 0;
            for (auto m : redemptions_) amount += m;

            outstanding_.push_back(amount);
            for (auto m : redemptions_) {
                amount -= m;
                outstanding_.push_back(amount);
            }
            outstanding_.pop_back();
        }

       private:
        size_t size_;
        MetaData metaData_;

        std::vector<Date> dates_;
        std::vector<double> interest_;
        std::vector<double> redemptions_;
        mutable std::vector<double> outstanding_;

        std::vector<size_t> DFIdx_;
        std::vector<size_t> FWDIdx_;
    };
}  // namespace Atlas

#endif /* E4F7ED0C_A61A_41D1_8DF7_A0F52A7228B6 */
