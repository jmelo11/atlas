#ifndef B34A82A6_5FFC_44CA_941F_7A0F27EE4570
#define B34A82A6_5FFC_44CA_941F_7A0F27EE4570

#include <atlas/data/bucket.hpp>
#include <vector>


namespace Atlas {
  

    /**
     * @brief A profile is a collection of buckets
     *
     * @tparam T type of the values to be stored (double, dual, etc.)
     * @param refDate reference date for the profile
     */
    template <class adouble>
    class Profile {
       public:
        Profile(Date refDate) : refDate_(refDate){};

        Profile operator+(const Profile& lhs) {
            if (refDate_ == lhs.refDate() && bucketCount() == lhs.bucketCount()) {
                Profile result;
                for (size_t i = 0; i < bucketCount(); i++) result.push_back(bucket(i) + lhs.bucket(i));
                return result;
            }
            throw std::runtime_error("Profiles do not match");
        }

        /**
         * @brief Gets the reference date of the profile
         *
         * @return Date
         */
        inline Date refDate() const { return refDate_; }

        /**
         * @brief Adds a new bucket to the profile
         *
         * @param start start date of the bucket
         * @param end end date of the bucket
         */
        inline void addNewBucket(const Date& start, const Date& end) { buckets_.push_back(Bucket<T>(start, end, 0)); }

        /**
         * @brief Gets the buckets of the profile
         *
         * @return std::vector<Bucket<adouble>>&
         */
        inline std::vector<Bucket<adouble>>& buckets() { return buckets_; }

        /**
         * @brief Gets the bucket at position i
         *
         * @return const std::vector<Bucket<adouble>>&
         */
        inline const Bucket<adouble>& bucket(size_t i) const { return buckets_[i]; }

        /**
         * @brief Gets the number of buckets in the profile
         *
         * @return size_t
         */
        inline size_t bucketCount() const { return buckets_.size(); }

       private:
        Date refDate_;
        std::vector<Bucket<adouble>> buckets_;
    };

    template <class adouble>
    std::ostream& operator<<(std::ostream& s, const Profile<adouble>& profile) {
        s << "refDate: " << profile.refDate() << "\n";
        for (size_t i = 0; i < profile.bucketCount(); i++) s << profile.bucket(i) << "\n";
        return s;
    }
}  // namespace Atlas

#endif /* B34A82A6_5FFC_44CA_941F_7A0F27EE4570 */
