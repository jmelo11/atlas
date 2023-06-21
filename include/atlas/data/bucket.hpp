/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef E3B070C1_8DDC_4DD1_AD78_061520147053
#define E3B070C1_8DDC_4DD1_AD78_061520147053

#include <atlas/atlasconfig.hpp>
#include <iostream>

namespace Atlas {

    /**
     * @brief A bucket is a time interval with a value
     *
     * @tparam adouble type of the values to be stored (double, dual, etc.)
     *
     */
    template <typename adouble>
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
}  // namespace Atlas

#endif /* E3B070C1_8DDC_4DD1_AD78_061520147053 */
