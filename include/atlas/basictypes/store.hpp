#ifndef C4437F3A_43D6_4F5D_A126_49E931EA21A9
#define C4437F3A_43D6_4F5D_A126_49E931EA21A9

#include <vector>

namespace Atlas {

    template <typename T>
    class Store {
       public:
        /***
         * Create a new object store.
         */
        Store() {}

        /***
         * Add a new index to the store and return its id.
         * @param index The index to add.
         * @return The id of the index.
         */
        size_t addItem(const T& item) {
            items_.push_back(item);
            return items_.size() - 1;
        }

        /***
         * Get the index with the given id.
         * @param indexId The id of the index.
         * @return The index.
         */
        T& getItem(size_t id) { return items_[id]; }

       private:
        std::vector<T> items_;
    };
}  // namespace Atlas

#endif /* C4437F3A_43D6_4F5D_A126_49E931EA21A9 */
