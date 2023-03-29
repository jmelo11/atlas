#ifndef C49C3B38_A428_4D92_AB3E_8E88957A544A
#define C49C3B38_A428_4D92_AB3E_8E88957A544A

#include <atlas/atlasconfig.hpp>
#include <atlas/rates/curvecontext.hpp>

namespace Atlas {

    /**
     * @class CurveContextStore
     * @brief Singleton class with thread local storage that stores all the curve contexts.
     */
    class CurveContextStore {
       public:
        /**
         * @brief Returns the instance of the store.
         * @return The instance of the store.
         */
        static CurveContextStore& instance();

        /**
         * @brief Creates a copy of curve context and adds it to the store.
         * @param contextName The name of the context.
         * @param curve The yield term structure.
         * @param index The rate index used as the underlying interest rate.
         */
        void createCurveContext(const std::string& contextName, const CurveStrategy& curve, const RateIndex& index);

        /**
         * @brief Returns the thread local version of the curve context with the given name.
         * @param contextName The name of the context.
         * @return The curve context.
         */
        const CurveContext& at(const std::string& contextName) const;

        /**
         * @brief Returns a thread local versoin of curve context at the given index.
         * @param idx The index of the context.
         * @return The curve context.
         */
        const CurveContext& at(size_t idx) const;

        /**
         * @brief Checks if the store has a context with the given name.
         * 
         * @param contextName string name of the context.
         * @return true if the store has a context with the given name.
         * @return false if the store does not have a context with the given name.
         */
        inline bool hasContext(const std::string& contextName) const { return nameToIdx_.find(contextName) != nameToIdx_.end(); }

        void copyContextsFromStore(const CurveContextStore& store);

       private:
        CurveContextStore() = default;

        static thread_local std::unique_ptr<CurveContextStore> instance_;
        static thread_local std::map<std::string, size_t> nameToIdx_;
        static thread_local std::vector<std::unique_ptr<CurveContext>> contexts_;
    };

}  // namespace Atlas

#endif /* C49C3B38_A428_4D92_AB3E_8E88957A544A */
