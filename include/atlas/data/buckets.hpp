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
     * @tparam T type of the values to be stored (double, dual, etc.)
     *
     */
    template <class T>
    struct Bucket {
        Date start_;
        Date end_;
        T value_;

        Bucket<T> operator+(const Bucket<T>& lhs) {
            if (start_ == lhs.start_ && end_ == lhs.end_) return {start_, end_, value_ + lhs.value_};
            throw std::runtime_error("Buckets do not match");
        };
    };

    /**
     * @brief Overload of the << operator for Bucket
     *
     * @tparam T
     * @param s
     * @param bucket
     * @return std::ostream&
     */
    template <class T>
    std::ostream& operator<<(std::ostream& s, const Bucket<T>& bucket) {
        return s << "(" << bucket.start_ << ", " << bucket.end_ << "," << bucket.value_ << ")";
    }

    /**
     * @brief A profile is a collection of buckets
     *
     * @tparam T type of the values to be stored (double, dual, etc.)
     * @param referenceDate reference date for the profile
     */
    template <class T>
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
        Date referenceDate() const { return referenceDate_; }
        void addNewBucket(Date start, Date end) { buckets_.push_back(Bucket<T>(start, end, 0)); }
        std::vector<Bucket<T>>& buckets() { return buckets_; }
        Bucket<T> bucket(size_t i) const { return buckets_[i]; }
        size_t bucketCount() const { return buckets_.size(); }

       private:
        Date referenceDate_;
        std::vector<Bucket<T>> buckets_;
    };

    template <class T>
    std::ostream& operator<<(std::ostream& s, const Profile<T>& profile) {
        s << "refDate: " << profile.referenceDate() << "\n";
        for (size_t i = 0; i < profile.bucketCount(); i++) s << profile.bucket(i) << "\n";
        return s;
    }
}  // namespace Atlas

#endif /* E3B070C1_8DDC_4DD1_AD78_061520147053 */
