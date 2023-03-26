
#include <atlas/rates/rateindex.hpp>

namespace Atlas {

    RateIndex::RateIndex() = default;

    RateIndex::RateIndex(const std::string& name, Frequency fixingFreq, DayCounter dayCounter, Frequency rateFreq, Compounding rateComp)
    : name_(name), dayCounter_(dayCounter), fixingFreq_(fixingFreq), rateFreq_(rateFreq), rateComp_(rateComp){};

    const std::string& RateIndex::name() const {
        return name_;
    };

    Frequency RateIndex::rateFrequency() const {
        return rateFreq_;
    };

    const DayCounter& RateIndex::dayCounter() const {
        return dayCounter_;
    };

    Compounding RateIndex::rateCompounding() const {
        return rateComp_;
    };

    void RateIndex::addFixing(const Date& date, double value) {
        fixingHistory_[date] = value;
    };

    Frequency RateIndex::fixingFrequency() const {
        return fixingFreq_;
    };

    double RateIndex::getFixing(const Date& date) const {
        auto it = fixingHistory_.find(date);
        if (it != fixingHistory_.end()) { return it->second; }
        {
            std::string msg = "No fixing found for date";
            throw std::runtime_error(msg);
        }
    };

    std::unique_ptr<RateIndex> RateIndex::copy() const {
        auto copy = std::make_unique<RateIndex>(name_, fixingFreq_, dayCounter_, rateFreq_, rateComp_);
        copy->fixingHistory_ = fixingHistory_;
        return copy;
    };

}  // namespace Atlas
