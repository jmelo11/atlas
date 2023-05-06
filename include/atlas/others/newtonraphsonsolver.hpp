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
        dual solve(std::function<dual(dual)> f, double x0 = 0.0, double tol = 1e-6, int max_iter = 100) {
            tape_type* tape = tape_type::getActive();
            if (tape == nullptr) { tape = new tape_type(); }
            NewtonCallback<tape_type> callback(f, x0, tol, max_iter);
            dual x = callback.solve();
            return x;
        };

       private:
        template <class Tape>
        class NewtonCallback : public xad::CheckpointCallback<Tape> {
           public:
            typedef typename Tape::slot_type slot_type;      // type for slot in the tape
            typedef typename Tape::value_type value_type;    // double
            typedef typename Tape::active_type active_type;  // AReal<double>

            NewtonCallback(std::function<active_type(active_type)> f, value_type x0, value_type tol, size_t max_iter)
            : f_(f), x0_(x0), tol_(tol), max_iter_(max_iter) {}

            active_type solve() {
                tape_type* tape = tape_type::getActive();
                tape->insertCallback(this);
                derivative(x0_) = 0.0;
                tape->computeAdjoints();
                return x0_;
            }

           private:
            void computeAdjoint(Tape* tape) override {
                for (size_t i = 0; i < max_iter_; i++) {
                    active_type x = x0_;
                    tape->registerInput(x);
                    xad::ScopedNestedRecording<Tape> nested(tape);
                    active_type y = f_(x);
                    tape->registerOutput(y);
                    derivative(y) = 1.0;
                    nested.computeAdjoints();

                    if (std::isnan(val(x)) || std::isnan(val(y))) { throw std::runtime_error("NaN encountered"); }
                    if (std::abs(val(y)) < tol_) { break; }

                    double dfdx = derivative(x);
                    if (std::abs(dfdx) < tol_) { throw std::runtime_error("Division by zero"); }
                    x0_ = val(x) - val(y) / dfdx;
                    if (i == max_iter_ - 1) { throw std::runtime_error("Maximum number of iterations reached"); }
                }
            }

           private:
            std::function<active_type(active_type)> f_;
            active_type x0_;
            value_type tol_;
            size_t max_iter_;
        };
    };
}  // namespace Atlas
#endif /* E8BC3CB8_B465_4ED9_87C4_4EC20105B063 */
