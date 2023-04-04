#include "../pch.hpp"
#include <atlas/others/newtonraphsonsolver.hpp>
using namespace Atlas;

TEST(Others, NewtonRaphson) {
    auto f = [](dual x) { return x * x; };

    NewtonRaphsonSolver solver;
    double x0 = 0.0;
    dual y    = solver.solve(f, x0);
    EXPECT_NEAR(y.val, 0.0, 1e-6);
}