#ifndef A53B011D_D929_446B_92A0_CDFF70167D4D
#define A53B011D_D929_446B_92A0_CDFF70167D4D

#include <atlas/atlasconfig.hpp>
#include <map>

namespace Atlas {

    /***
     * A rate index.
     */
    class RateIndex {
       public:
        RateIndex() = default;

        /***
         * Create a new rate index.
         * @param name The name of the index.
         * @param dayCounter The day counter used to calculate the accrued amount.
         * @param fixingFreq The frequency of fixing.
         * @param rateFreq The frequency of the rate.
         * @param rateComp The compounding of the rate.
         * @param fixingHistory The history of fixing values.
         */
        RateIndex(const std::string& name, DayCounter dayCounter, Frequency fixingFreq, Frequency rateFreq = Frequency::Annual,
                  Compounding rateComp = Compounding::Simple, std::map<Date, double> fixingHistory = std::map<Date, double>())
        : name_(name), dayCounter_(dayCounter), fixingFreq_(fixingFreq), rateComp_(rateComp), fixingHistory_(fixingHistory){};

        const std::string& name() const { return name_; }

        DayCounter dayCounter() const { return dayCounter_; }

        Frequency rateFrequency() const { return rateFreq_; }

        Compounding rateCompounding() const { return rateComp_; }

        Frequency fixingFrequency() const { return fixingFreq_; }

        void addFixing(const Date& date, double value) { fixingHistory_[date] = value; }  // how does it work with simulations?

        double getFixing(const Date& date) const {
            auto it = fixingHistory_.find(date);
            if (it != fixingHistory_.end()) { return it->second; }
            {
                std::string msg = "No fixing found for date";
                throw std::runtime_error(msg);
            }
        }

       private:
        std::string name_      = "undefined";
        DayCounter dayCounter_ = DayCounter();
        Frequency fixingFreq_  = Frequency::NoFrequency;
        Frequency rateFreq_    = Frequency::NoFrequency;
        Compounding rateComp_  = Compounding::Simple;
        std::map<Date, double> fixingHistory_;
    };

    /***
     * A store for rate indexes. Based on the flyweight pattern.
     */
    class RateIndexStore {
       public:
        /***
         * Create a new rate index store.
         */
        RateIndexStore() {}

        /***
         * Add a new index to the store and return its id.
         * @param index The index to add.
         * @return The id of the index.
         */
        size_t addIndex(const RateIndex& index) {
            indexes_.push_back(index);
            return indexes_.size() - 1;
        }

        /***
         * Get the index with the given id.
         * @param indexId The id of the index.
         * @return The index.
         */
        RateIndex& getIndex(size_t indexId) { return indexes_[indexId]; }

       private:
        std::vector<RateIndex> indexes_;
    };

    /***
     * Common rate indexes.
     */
    class LIBOR3M : public RateIndex {
       public:
        LIBOR3M() : RateIndex("LIBOR3M", Actual360(), Frequency::Quarterly) {}
    };
    class LIBOR1M : public RateIndex {
       public:
        LIBOR1M() : RateIndex("LIBOR1M", Actual360(), Frequency::Monthly) {}
    };
    class LIBOR6M : public RateIndex {
       public:
        LIBOR6M() : RateIndex("LIBOR6M", Actual360(), Frequency::Semiannual) {}
    };
    class LIBOR12M : public RateIndex {
       public:
        LIBOR12M() : RateIndex("LIBOR12M", Actual360(), Frequency::Annual) {}
    };
    class ICP : public RateIndex {
       public:
        ICP() : RateIndex("ICP", Actual360(), Frequency::Daily) {}
    };
    class SOFR : public RateIndex {
       public:
        SOFR() : RateIndex("SOFR", Actual360(), Frequency::Daily) {}
    };
    class TERMSOFR1M : public RateIndex {
       public:
        TERMSOFR1M() : RateIndex("TERMSOFR1M", Actual360(), Frequency::Monthly) {}
    };
    class TERMSOFR3M : public RateIndex {
       public:
        TERMSOFR3M() : RateIndex("TERMSOFR3M", Actual360(), Frequency::Quarterly) {}
    };
    class TERMSOFR6M : public RateIndex {
       public:
        TERMSOFR6M() : RateIndex("TERMSOFR6M", Actual360(), Frequency::Semiannual) {}
    };
    class TERMSOFR12M : public RateIndex {
       public:
        TERMSOFR12M() : RateIndex("TERMSOFR12M", Actual360(), Frequency::Annual) {}
    };

}  // namespace Atlas

#endif /* A53B011D_D929_446B_92A0_CDFF70167D4D */
