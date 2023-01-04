
#include <atlas/aad/aad.hpp>

namespace Atlas {
    std::vector<AADNode> tape;

    std::ostream& operator<<(std::ostream& os, const AADNumber& rhs) const {
        os << rhs.value;
        return os;
    };

    std::string to_string(const AADNumber& arg) const {
        return std::to_string(arg.value);
    };
}  // namespace Atlas