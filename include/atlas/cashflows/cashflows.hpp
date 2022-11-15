#ifndef AFA7975E_AEE5_4206_9E31_0E59CE9EDB1E
#define AFA7975E_AEE5_4206_9E31_0E59CE9EDB1E

#include <vector>

namespace Atlas {
    
    class Redemption;

    class Cashflows {
       public:
        static void sortRedemptions(std::vector<Redemption>& redemptions);
    };
}  // namespace Atlas

#endif /* AFA7975E_AEE5_4206_9E31_0E59CE9EDB1E */
