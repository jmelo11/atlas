#ifndef D41E427D_25EB_4AB1_89A1_415A80BBF4BF
#define D41E427D_25EB_4AB1_89A1_415A80BBF4BF

namespace Atlas {

    /***
     * @brief Base class for all classes that can be indexed. It's used to store the indexes of variables needed for pricing (market values)
     */
    class Indexable {
       public:
        virtual ~Indexable(){};

        size_t dfIdx() const { return idx_[0]; }
        void dfIdx(size_t idx) { idx_[0] = idx; }

        size_t fwdIdx() const { return idx_[1]; }
        void fwdIdx(size_t idx) { idx_[1] = idx; }

        size_t ccyIdx() const { return idx_[2]; }
        void ccyIdx(size_t idx) { idx_[2] = idx; }
                
       private:
        size_t idx_[3]; // 0: discount factor index, 1: forward index, 2: currency index, 3: index index, 4: discount curve index
    };

}  // namespace Atlas

#endif /* D41E427D_25EB_4AB1_89A1_415A80BBF4BF */
