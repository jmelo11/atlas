/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef E3B070C1_8DDC_4DD1_AD78_061520147053
#define E3B070C1_8DDC_4DD1_AD78_061520147053

#include <atlas/atlasconfig.hpp>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

namespace Atlas {

    /**
     * @brief A bucket is a time interval with a value
     *
     * @tparam adouble type of the values to be stored (double, dual, etc.)
     *
     */
    template <class adouble>
    struct Bucket {
        Date start_    = Date();
        Date end_      = Date();
        adouble value_ = 0.0;

        Bucket<adouble> operator+(const Bucket<adouble>& lhs) {
            if (start_ == lhs.start_ && end_ == lhs.end_) return {start_, end_, value_ + lhs.value_};
            throw std::runtime_error("Buckets do not match");
        };
    };

    /**
     * @brief Overload of the << operator for Bucket
     *
     * @tparam adouble
     * @param s
     * @param bucket
     * @return std::ostream&
     */
    template <class adouble>
    std::ostream& operator<<(std::ostream& s, const Bucket<adouble>& bucket) {
        return s << "(" << bucket.start_ << ", " << bucket.end_ << "," << bucket.value_ << ")";
    }

    /**
     * @brief A profile is a collection of buckets
     *
     * @tparam T type of the values to be stored (double, dual, etc.)
     * @param referenceDate reference date for the profile
     */
    template <class adouble>
    class Profile {
       public:
        Profile(Date referenceDate) : referenceDate_(referenceDate){};
        Profile operator+(const Profile& lhs) {
            if (referenceDate_ == lhs.referenceDate() && bucketCount() == lhs.bucketCount()) {
                Profile result;
                for (size_t i = 0; i < bucketCount(); i++) result.push_back(buckets(i) + lhs.buckets(i));
                return result;
            }
            throw std::runtime_error("Profiles do not match");
        }

        /**
         * @brief Gets the reference date of the profile
         *
         * @return Date
         */
        Date referenceDate() const { return referenceDate_; }

        /**
         * @brief Adds a new bucket to the profile
         *
         * @param start start date of the bucket
         * @param end end date of the bucket
         */
        void addNewBucket(const Date& start, const Date& end) { buckets_.push_back(Bucket<T>(start, end, 0)); }

        /**
         * @brief Gets the buckets of the profile
         *
         * @return std::vector<Bucket<adouble>>&
         */
        std::vector<Bucket<adouble>>& buckets() { return buckets_; }

        /**
         * @brief Gets the bucket at position i
         *
         * @return const std::vector<Bucket<adouble>>&
         */
        const Bucket<adouble>& bucket(size_t i) const { return buckets_[i]; }

        /**
         * @brief Gets the number of buckets in the profile
         *
         * @return size_t
         */
        size_t bucketCount() const { return buckets_.size(); }

       private:
        Date referenceDate_;
        std::vector<Bucket<adouble>> buckets_;
    };

    template <class adouble>
    std::ostream& operator<<(std::ostream& s, const Profile<adouble>& profile) {
        s << "refDate: " << profile.referenceDate() << "\n";
        for (size_t i = 0; i < profile.bucketCount(); i++) s << profile.bucket(i) << "\n";
        return s;
    }
}  // namespace Atlas

#endif /* E3B070C1_8DDC_4DD1_AD78_061520147053 */
