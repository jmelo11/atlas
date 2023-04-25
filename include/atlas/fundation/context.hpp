#ifndef AFC57A36_0A96_4135_BAEB_CA354309F7A8
#define AFC57A36_0A96_4135_BAEB_CA354309F7A8

#include <algorithm>

namespace Atlas {

    template <class T>
    class ContextManager;

    /**
     * @brief A context is a wrapper around an object of type T. It is used to
     * store the object in a context manager. The context manager can then
     * retrieve the object by index or by name. It has an unique index and a
     * pointer to the object.
     * @tparam T
     */
    template <class T>
    class Context {
       public:
        /**
         * @brief Returns the index of the context.
         *
         * @return size_t
         */
        inline size_t idx() const { return idx_; }

        /**
         * @brief Returns a reference to the object.
         *
         * @return const T&
         */
        const T& object() const { return *obj_; }

        Context(const Context& other)            = delete;
        Context& operator=(const Context& other) = delete;

       private:
        /**
         * @brief Construct a new Context object
         *
         * @param obj
         * @param idx
         */
        Context(std::unique_ptr<T>& obj, size_t idx) : obj_(std::move(obj)), idx_(idx) {}

        /**
         * @brief Construct a new Context object
         *
         * @param other
         */
        Context(Context&& other) : obj_(std::move(other.obj_)), idx_(other.idx_) {
            other.obj_ = nullptr;
            other.idx_ = 0;
        }

        /**
         * @brief Copy assignment operator
         *
         * @param other
         * @return Context&
         */
        Context& operator=(Context&& other) {
            if (this != &other) {
                obj_       = move(other.obj_);
                idx_       = other.idx_;
                other.obj_ = nullptr;
                other.idx_ = 0;
            }
            return *this;
        }

        /**
         * @brief Copy assignment operator
         *
         * @param other
         * @return Context&
         */
        Context<T> clone() const { return Context<T>(std::make_unique<T>(*obj_), idx_); }

        std::unique_ptr<T> obj_;
        size_t idx_;
        friend class ContextManager<T>;
    };
}  // namespace Atlas

#endif /* AFC57A36_0A96_4135_BAEB_CA354309F7A8 */
