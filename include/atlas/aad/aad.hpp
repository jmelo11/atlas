#ifndef DE504ED0_CA8A_4458_8288_8D473F8CFD52
#define DE504ED0_CA8A_4458_8288_8D473F8CFD52

#include <atlas/aad/gaussians.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

namespace Atlas {

    struct AADNode {
        int nargs;
        int idx1;
        int idx2;
        double der1;
        double der2;

        AADNode(){};
    };

    extern std::vector<AADNode> tape;

    struct AADNumber {
        double value;
        int idx;

        AADNumber(){};

        AADNumber(const double& v) : value(v) {
            tape.push_back(AADNode());
            AADNode& node = tape.back();
            idx           = tape.size() - 1;
            node.nargs    = 0;
        };

        // basic operators
        friend AADNumber operator+(const AADNumber& lhs, const AADNumber& rhs) {
            tape.push_back(AADNode());
            AADNode& node = tape.back();
            node.idx1     = lhs.idx;
            node.idx2     = rhs.idx;
            node.der1     = 1;
            node.der2     = 1;
            node.nargs    = 2;
            AADNumber result;
            result.value = lhs.value + rhs.value;
            result.idx   = tape.size() - 1;
            return result;
        }

        friend AADNumber operator-(const AADNumber& lhs, const AADNumber& rhs) {
            tape.push_back(AADNode());
            AADNode& node = tape.back();
            node.idx1     = lhs.idx;
            node.idx2     = rhs.idx;
            node.der1     = 1;
            node.der2     = -1;
            node.nargs    = 2;
            AADNumber result;
            result.value = lhs.value - rhs.value;
            result.idx   = tape.size() - 1;
            return result;
        }

        friend AADNumber operator*(const AADNumber& lhs, const AADNumber& rhs) {
            tape.push_back(AADNode());
            AADNode& node = tape.back();
            node.idx1     = lhs.idx;
            node.idx2     = rhs.idx;
            node.der1     = rhs.value;
            node.der2     = lhs.value;
            node.nargs    = 2;
            AADNumber result;
            result.value = lhs.value * rhs.value;
            result.idx   = tape.size() - 1;
            return result;
        }

        friend AADNumber operator/(const AADNumber& lhs, const AADNumber& rhs) {
            tape.push_back(AADNode());
            AADNode& node = tape.back();
            node.idx1     = lhs.idx;
            node.idx2     = rhs.idx;
            node.der1     = 1 / rhs.value;
            node.der2     = -lhs.value / (rhs.value * rhs.value);
            node.nargs    = 2;
            AADNumber result;
            result.value = lhs.value / rhs.value;
            result.idx   = tape.size() - 1;
            return result;
        }

        AADNumber operator+() const { return AADNumber(0.0) + *this; }

        AADNumber operator-() const { return AADNumber(0.0) - *this; }

        AADNumber& operator-=(const AADNumber& rhs) {
            *this = *this - rhs;
            return *this;
        }

        AADNumber& operator+=(const AADNumber& rhs) {
            *this = *this + rhs;
            return *this;
        }

        AADNumber& operator/=(const AADNumber& rhs) {
            *this = *this / rhs;
            return *this;
        }

        AADNumber& operator*=(const AADNumber& rhs) {
            *this = *this * rhs;
            return *this;
        }

        AADNumber& operator++() {
            *this = *this + AADNumber(1.0);
            return *this;
        }

        AADNumber& operator--() {
            *this = *this - AADNumber(1.0);
            return *this;
        }

        // logical
        friend bool operator<(const AADNumber& lhs, const AADNumber& rhs) { return lhs.value < rhs.value; }

        friend bool operator>(const AADNumber& lhs, const AADNumber& rhs) { return lhs.value > rhs.value; }

        friend bool operator<=(const AADNumber& lhs, const AADNumber& rhs) { return lhs.value <= rhs.value; }

        friend bool operator>=(const AADNumber& lhs, const AADNumber& rhs) { return lhs.value >= rhs.value; }

        friend bool operator==(const AADNumber& lhs, const AADNumber& rhs) { return lhs.value == rhs.value; }

        friend bool operator!=(const AADNumber& lhs, const AADNumber& rhs) { return lhs.value != rhs.value; }

        friend bool operator&&(const AADNumber& lhs, const AADNumber& rhs) { return lhs.value && rhs.value; }

        friend bool operator||(const AADNumber& lhs, const AADNumber& rhs) { return lhs.value || rhs.value; }

        // logical
        friend bool operator<(const AADNumber& lhs, const double& rhs) { return lhs.value < rhs; }

        friend bool operator>(const AADNumber& lhs, const double& rhs) { return lhs.value > rhs; }

        friend bool operator<=(const AADNumber& lhs, const double& rhs) { return lhs.value <= rhs; }

        friend bool operator>=(const AADNumber& lhs, const double& rhs) { return lhs.value >= rhs; }

        friend bool operator==(const AADNumber& lhs, const double& rhs) { return lhs.value == rhs; }

        friend bool operator!=(const AADNumber& lhs, const double& rhs) { return lhs.value != rhs; }

        friend bool operator&&(const AADNumber& lhs, const double& rhs) { return lhs.value && rhs; }

        friend bool operator||(const AADNumber& lhs, const double& rhs) { return lhs.value || rhs; }

        // math expressions
        friend AADNumber exp(const AADNumber& arg) {
            tape.push_back(AADNode());
            AADNode& node = tape.back();
            node.idx1     = arg.idx;
            node.nargs    = 1;
            AADNumber result;
            result.value = std::exp(arg.value);
            node.der1    = result.value;
            result.idx   = tape.size() - 1;
            return result;
        }

        friend AADNumber fabs(const AADNumber& arg) {
            tape.push_back(AADNode());
            AADNumber result;
            result.value = std::fabs(arg.value);
            result.idx   = tape.size() - 1;

            AADNode& node = tape.back();
            node.idx1     = arg.idx;
            node.der1     = arg.value < 0 ? 0 : 1;
            node.nargs    = 1;
            return result;
        }

        friend AADNumber log(const AADNumber& arg) {
            tape.push_back(AADNode());
            AADNode& node = tape.back();
            node.idx1     = arg.idx;
            node.der1     = 1 / arg.value;
            node.nargs    = 1;
            AADNumber result;
            result.value = std::log(arg.value);
            result.idx   = tape.size() - 1;
            return result;
        }

        friend AADNumber sqrt(const AADNumber& arg) {
            tape.push_back(AADNode());

            AADNumber result;
            result.value = std::sqrt(arg.value);
            result.idx   = tape.size() - 1;

            AADNode& node = tape.back();
            node.idx1     = arg.idx;
            node.der1     = 0.5 / result.value;
            node.nargs    = 1;
            return result;
        }

        friend std::string to_string(const AADNumber& arg);

        friend AADNumber pow(const AADNumber& lhs, const AADNumber& rhs) {
            tape.push_back(AADNode());
            AADNode& node = tape.back();
            node.idx1     = lhs.idx;
            node.idx2     = rhs.idx;
            node.der1     = rhs.value * std::pow(lhs.value, rhs.value - 1);
            node.der2     = std::log(rhs.value) * std::exp(lhs.value * std::log(rhs.value));
            node.nargs    = 2;
            AADNumber result;
            result.value = std::pow(lhs.value, rhs.value);
            result.idx   = tape.size() - 1;
            return result;
        }

        // operators with doubles
        friend AADNumber operator+(const AADNumber& lhs, const double& rhs) {
            tape.push_back(AADNode());
            AADNode& node = tape.back();
            node.idx1     = lhs.idx;
            node.der1     = 1;
            node.nargs    = 1;
            AADNumber result;
            result.value = lhs.value + rhs;
            result.idx   = tape.size() - 1;
            return result;
        }

        friend AADNumber operator-(const AADNumber& lhs, const double& rhs) {
            tape.push_back(AADNode());
            AADNode& node = tape.back();
            node.idx1     = lhs.idx;
            node.der1     = 1;
            node.nargs    = 2;
            AADNumber result;
            result.value = lhs.value - rhs;
            result.idx   = tape.size() - 1;
            return result;
        }

        friend AADNumber operator*(const AADNumber& lhs, const double& rhs) {
            tape.push_back(AADNode());
            AADNode& node = tape.back();
            node.idx1     = lhs.idx;
            node.der1     = rhs;
            node.nargs    = 1;
            AADNumber result;
            result.value = lhs.value * rhs;
            result.idx   = tape.size() - 1;
            return result;
        }

        friend AADNumber operator/(const AADNumber& lhs, const double& rhs) {
            tape.push_back(AADNode());
            AADNode& node = tape.back();
            node.idx1     = lhs.idx;
            node.der1     = 1 / rhs;
            node.nargs    = 1;
            AADNumber result;
            result.value = lhs.value / rhs;
            result.idx   = tape.size() - 1;
            return result;
        }

        AADNumber& operator-=(const double& rhs) {
            *this = *this - rhs;
            return *this;
        }

        AADNumber& operator+=(const double& rhs) {
            *this = *this + rhs;
            return *this;
        }

        AADNumber& operator/=(const double& rhs) {
            *this = *this / rhs;
            return *this;
        }

        AADNumber& operator*=(const double& rhs) {
            *this = *this * rhs;
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const AADNumber& rhs);

        friend AADNumber normalDens(const AADNumber& arg) {
            //  create a new record on tape
            tape.push_back(AADNode());
            AADNode& node = tape.back();

            //  compute result
            AADNumber result;
            result.value = normalDens(arg.value);

            //  reference record on tape
            result.idx = tape.size() - 1;

            //  populate record on tape
            node.nargs = 1;
            node.idx1  = arg.idx;

            //  compute derivative
            node.der1 = -result.value * arg.value;

            return result;
        }

        friend AADNumber normalCdf(const AADNumber& arg) {
            //  create a new record on tape
            tape.push_back(AADNode());
            AADNode& node = tape.back();

            //  compute result
            AADNumber result;
            result.value = normalCdf(arg.value);

            //  reference record on tape
            result.idx = tape.size() - 1;

            //  populate record on tape
            node.nargs = 1;
            node.idx1  = arg.idx;

            //  compute derivative
            node.der1 = normalDens(arg.value);

            return result;
        }
    };

    inline std::vector<double> calculateAdjoints(const AADNumber& result) {
        //  initialization
        std::vector<double> adjoints(tape.size(), 0.0);  //  initialize all to 0
        int N       = result.idx;                        //  find N
        adjoints[N] = 1.0;                               //  seed aN = 1

        //  backward propagation
        for (int j = N; j > 0; --j)  //  iterate backwards over tape
        {
            if (tape[j].nargs > 0) {
                //  propagate first argument
                adjoints[tape[j].idx1] += adjoints[j] * tape[j].der1;
                if (tape[j].nargs > 1) {
                    //  propagate second argument
                    adjoints[tape[j].idx2] += adjoints[j] * tape[j].der2;
                }
            }
        }
        return adjoints;
    }

}  // namespace Atlas

#endif /* DE504ED0_CA8A_4458_8288_8D473F8CFD52 */
