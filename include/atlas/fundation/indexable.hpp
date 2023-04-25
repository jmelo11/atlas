#ifndef D41E427D_25EB_4AB1_89A1_415A80BBF4BF
#define D41E427D_25EB_4AB1_89A1_415A80BBF4BF

namespace Atlas {

    /**
     * @class Indexable
     * The Indexable class represents an object that can be indexed in multiple dimensions.
     * It has three index attributes: discount factor index, forward index, and currency index.
     *
     * Inspired in A. Savine book "Modern computational finance: AAD and parallel simulation"
     */
    class Indexable {
       public:
        virtual ~Indexable(){};

        /**
         * Returns the index of the discount factor.
         * @return The index of the discount factor.
         */
        size_t dfIdx() const { return idx_[0]; }

        /**
         * Sets the index of the discount factor.
         * @param idx The index of the discount factor to set.
         */
        void dfIdx(size_t idx) { idx_[0] = idx; }

        /**
         * Returns the index of the forward rate.
         * @return The index of the forward rate.
         */
        size_t fwdIdx() const { return idx_[1]; }

        /**
         * Sets the index of the forward rate.
         * @param idx The index of the forward rate to set.
         */
        void fwdIdx(size_t idx) { idx_[1] = idx; }

        /**
         * Returns the index of the related price.
         * @return The index of the price.
         */
        size_t priceIdx() const { return idx_[2]; }

        /**
         * Sets the index of the price.
         * @param idx The index of the price to set.
         */
        void priceIdx(size_t idx) { idx_[2] = idx; }

        /**
         * @brief Returns the index of the fx price.
         * @return The index of the fx price.
         */
        size_t fxIdx() const { return idx_[3]; }

        /**
         * @brief Sets the index of the fx price.
         * @param idx The index of the fx price to set.
         */
        void fxIdx(size_t idx) { idx_[3] = idx; }

       private:
        size_t idx_[4]; /**< An array of indices representing the discount factor index (0), forward index (1), price index (2) and fx price (3). */
    };

}  // namespace Atlas

#endif /* D41E427D_25EB_4AB1_89A1_415A80BBF4BF */
