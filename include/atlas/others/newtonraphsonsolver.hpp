#ifndef E8BC3CB8_B465_4ED9_87C4_4EC20105B063
#define E8BC3CB8_B465_4ED9_87C4_4EC20105B063

#include <atlas/atlasconfig.hpp>
#include <numeric>

namespace Atlas {
    /**
     * @brief Newton-Raphson solver
     *
     */
    class NewtonRaphsonSolver {
       public:
        NewtonRaphsonSolver() = default;

        /**
         * @brief Solve a function f(x) = 0 using Newton-Raphson
         *
         * @param f function to be solved
         * @param x0 initial guess
         * @param tol tolerance
         * @param max_iter maximum number of iterations
         * @return dual
         */
        dual solve(std::function<dual(dual)> f, double x0, double tol = 1e-6, int max_iter = 100) {
            dual x = x0;
            dual y = f(x);
            for (int i = 0; i < max_iter; ++i) {
                if (abs(y) < tol) { break; }
                double dfdx = derivative(f, wrt(x), at(x));
                x           = x - y / dfdx;
                y           = f(x);
            }
            return x;
        }
    };

}  // namespace Atlas

#endif /* E8BC3CB8_B465_4ED9_87C4_4EC20105B063 */
