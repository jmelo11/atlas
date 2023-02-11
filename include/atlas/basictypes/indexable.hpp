#ifndef D41E427D_25EB_4AB1_89A1_415A80BBF4BF
#define D41E427D_25EB_4AB1_89A1_415A80BBF4BF

namespace Atlas {

    /***
     * @brief Base class for all classes that can be indexed. It's used to store the index of the object in the indexable container.     
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
        size_t idx_[3]; // 24 bytes
    };

}  // namespace Atlas

#endif /* D41E427D_25EB_4AB1_89A1_415A80BBF4BF */
