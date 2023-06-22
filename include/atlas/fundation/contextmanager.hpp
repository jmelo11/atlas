#ifndef E94A3F52_0C52_48D0_9A52_57300975A378
#define E94A3F52_0C52_48D0_9A52_57300975A378

#include <atlas/atlasconfig.hpp>
#include <atlas/fundation/context.hpp>
#include <unordered_map>
#include <vector>

namespace Atlas {
    /**
     * @class ContextManager
     * @brief ContextManager is a class that manages a collection of objects of type T.
     *
     * @tparam T
     */
    template <class T>
    class ContextManager {
       public:
        /**
         * @brief Construct a new Context Manager object
         *
         * @param refDate
         */
        ContextManager(const Date& refDate) : refDate_(refDate){};

        /**
         * @brief Create a Context object
         *
         * @param name
         * @param obj
         */
        void createContext(const std::string& name, const T& obj) {
            if (obj.refDate() != refDate()) { throw std::invalid_argument("Error: object ref date does not match manager's ref date."); }
            map_[name] = createContext(obj);
        }

        /**
         * @brief Returns the number of contexts.
         *
         * @return size_t
         */
        inline size_t size() const { return contexts_.size(); }

        /**
         * @brief Get the Context object
         *
         * @param idx
         * @return const Context<T>&
         */
        inline const Context<T>& getContext(size_t idx) const { return contexts_.at(idx); }

        /**
         * @brief Get the Context object
         *
         * @param name
         * @return const Context<T>&
         */
        inline const Context<T>& getContext(const std::string& name) const {
            auto it = map_.find(name);
            if (it == map_.end()) {
                std::string msg = "Error: name \"" + name + "\" not found in map!";
                throw std::runtime_error(msg);
            }
            return getContext(it->second);
        }

        /**
         * @brief Returns the reference date of the manager.
         *
         * @return const Date&
         */
        inline const Date& refDate() const { return refDate_; };

        /**
         * @brief Returns a clone of the manager.
         *
         * @return std::unique_ptr<ContextManager<T>>
         */
        std::unique_ptr<ContextManager<T>> clone() const {
            auto managerClone  = std::make_unique<ContextManager<T>>(refDate());
            managerClone->map_ = map_;

            for (const auto& context : contexts_) {
                auto objClone = context.object().clone();  // Use object() instead of accessing obj directly
                managerClone->createContext(*objClone);
            }
            return managerClone;
        }

        /**
         * @brief Returns true if the manager has a context with the given name.
         *
         * @param name
         * @return true
         * @return false
         */
        bool hasContext(const std::string& name) const { return map_.find(name) != map_.end(); }

       private:
        /**
         * @brief Create a Context object
         *
         * @param obj
         * @return size_t
         */
        inline size_t createContext(const T& obj) {
            size_t idx    = contexts_.size();
            auto objClone = obj.clone();
            Context<T> context(objClone, idx);
            contexts_.push_back(std::move(context));
            return idx;
        }

        Date refDate_;
        std::vector<Context<T>> contexts_;
        std::unordered_map<std::string, size_t> map_;
    };
}  // namespace Atlas

#endif /* E94A3F52_0C52_48D0_9A52_57300975A378 */
