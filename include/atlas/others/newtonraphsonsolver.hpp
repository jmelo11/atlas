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
        // dual solve(std::function<dual(dual)> f, double x0 = 0.0, double tol = 1e-6, int max_iter = 100) {
        //     dual x = x0;
        //     dual y = f(x);
        //     for (int i = 0; i < max_iter; i++) {
        //         if (std::isnan(val(x)) || std::isnan(val(y))) { throw std::runtime_error("NaN encountered"); }
        //         if (std::abs(val(y)) < tol) { return x; }
        //         double dfdx = derivative(f, wrt(x), at(x));
        //         if (abs(dfdx) < tol) {
        //             // df/dx is close to zero, so we can't divide by it
        //             throw std::runtime_error("Division by zero");
        //         }
        //         x = x - y / dfdx;
        //         y = f(x);
        //     }
        //     return x;
        // };

        // dual solve(std::function<dual(dual)> f, double x0 = 0.0, double tol = 1e-6, int max_iter = 100) {
        //     tape_type* tape  = tape_type::getActive();
        //     bool destroyTape = false;
        //     if (tape == nullptr) {
        //         tape        = new tape_type();
        //         destroyTape = true;
        //     }
        //     dual x = x0;
        //     tape->registerInput(x);
        //     if (!tape->isActive()) {}
        //     tape->newRecording();
        //     tape->printStatus();
        //     dual y = f(x);
        //     tape->registerOutput(y);
        //     size_t pos = tape->getPosition();

        //     // y = f(x);
        //     for (int i = 0; i < max_iter; i++) {
        //         if (std::isnan(val(x)) || std::isnan(val(y))) { throw std::runtime_error("NaN encountered"); }
        //         if (std::abs(val(y)) < tol) { break; }

        //         derivative(y) = 1.0;
        //         tape->computeAdjoints();
        //         double dfdx = derivative(x);
        //         if (abs(dfdx) < tol) {
        //             // df/dx is close to zero, so we can't divide by it
        //             throw std::runtime_error("Division by zero");
        //         }
        //         tape->clearDerivativesAfter(pos);
        //         x = x - y / dfdx;
        //         y = f(x);
        //     }
        //     if (destroyTape) { delete tape; }
        //     return val(x);
        // };

        template <class Tape>
        class NewtonCallback : public xad::CheckpointCallback<Tape> {
           public:
            typedef typename Tape::slot_type slot_type;      // type for slot in the tape
            typedef typename Tape::value_type value_type;    // double
            typedef typename Tape::active_type active_type;  // AReal<double>

            NewtonCallback(std::function<active_type(active_type)> f, value_type x0, value_type tol, int max_iter)
            : f(f), x0(x0), tol(tol), max_iter(max_iter) {}

            void computeAdjoint(Tape* tape) override {
                dual x = x0;
                tape->registerInput(x);
                xad::ScopedNestedRecording<Tape> nested(tape);
                dual y = f(x);
                tape->registerOutput(y);
                // y = f(x);
                for (int i = 0; i < max_iter; i++) {
                    if (std::isnan(val(x)) || std::isnan(val(y))) { throw std::runtime_error("NaN encountered"); }
                    if (std::abs(val(y)) < tol) { break; }

                    derivative(y) = 1.0;
                    nested.computeAdjoints();
                    double dfdx = derivative(x);
                    if (abs(dfdx) < tol) {
                        // df/dx is close to zero, so we can't divide by it
                        throw std::runtime_error("Division by zero");
                    }
                    x = x - y / dfdx;
                    y = f(x);
                }
                result = x;
            }
            active_type getOutput() { return result; }

           private:
            active_type result;
            std::function<active_type(active_type)> f;
            value_type x0;
            value_type tol;
            int max_iter;
        };

        dual solve(std::function<dual(dual)> f, double x0 = 0.0, double tol = 1e-6, int max_iter = 100) {
            tape_type* tape  = tape_type::getActive();
            bool destroyTape = false;
            if (tape == nullptr) {
                tape        = new tape_type();
                destroyTape = true;
                tape->newRecording();
            }
            NewtonCallback<tape_type> callback(f, x0, tol, max_iter);
            tape->insertCallback(&callback);
            tape->computeAdjoints();
            return callback.getOutput();
        };
    };
}  // namespace Atlas
#endif /* E8BC3CB8_B465_4ED9_87C4_4EC20105B063 */
