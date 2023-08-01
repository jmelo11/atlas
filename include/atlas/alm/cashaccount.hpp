#ifndef DAFFCAC9_2E4F_4096_9F4F_22926995D6FB
#define DAFFCAC9_2E4F_4096_9F4F_22926995D6FB

#include <atlas/atlasdefine.hpp>
#include <atlas/instruments/mixins/onelegmixin.hpp>

namespace Atlas {

    template <template <typename> class LegType, typename adouble = double>
    class CashAccount : public OneLegMixin<LegType, adouble> {
       public:
        CashAccount(const Date& refDate, adouble initBalance, const Currency& ccy) : refDate_(refDate), initBalance_(initBalance), currency_(ccy){};
        ~CashAccount() = default;

        /**
         * @brief Returns the balance of the cash account at a given date.
         *
         * @param date
         * @return adouble
         */
        inline adouble balance(const Date& date) const { return balance_.at(date); };

        /**
         * @brief Returns the currency of the cash account.
         *
         * @return Currency
         */
        inline Currency currency() const { return currency_; }

        /**
         * @brief Updates the balance of the cash account at a given date.
         *
         * @param date
         * @return adouble
         */
        inline void updateBalance(const Date& date, const adouble& amount) {
            if (date < refDate_) throw std::runtime_error("Cannot update balance before account's reference date.");
            if (balance_.find(date) == balance_.end()) {
                balance_[date] = amount;
            } else {
                balance_[date] += amount;
            }
        };

        /**
         * @brief Creates the relevant coupons.
         *
         */
        void createCoupons() = 0;

       protected:
        /**
         * @brief Transforms the balance into a vector of notionals.
         * @details The starting balance is the first element of the vector, then the balance is calculated as the
         * last day balance plus the cashflows of the next day available.
         * @return std::map<Date, adouble>
         */
        inline std::map<Date, adouble> notionalsFromBalance() const {
            std::map<Date, adouble> notionals;
            notionals[refDate_] = initBalance_;
            for (auto& [date, amount] : balance_) { notionals[date] = notionals.at(date - 1) + amount; }
            return notionals;
        }

       private:
        Date refDate_;
        adouble initBalance_;
        std::map<Date, adouble> balance_;
        Currency currency_;
    };
}  // namespace Atlas

#endif /* DAFFCAC9_2E4F_4096_9F4F_22926995D6FB */
