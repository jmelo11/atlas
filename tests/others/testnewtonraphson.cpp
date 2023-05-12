#include <gtest/gtest.h>
#include <atlas/others/newtonraphsonsolver.hpp>
using namespace Atlas;

TEST(NewtonRaphsonTest, TestGeneralFunction) {
    auto f = [](dual x) {
        dual eq = 3 * x - cos(x) - 1;
        return eq;
    };

    NewtonRaphsonSolver solver;
    double x0 = 10.0;
    dual y    = solver.solve(f, x0);
    EXPECT_NEAR(val(y), 0.607102, 1e-6);
}

TEST(NewtonRaphsonTest, TestCubicFunction) {
    auto f = [](dual x) {
        dual eq = x * x * x - 8;
        return eq;
    };

    NewtonRaphsonSolver solver;
    double x0   = 0.01;
    dual result = solver.solve(f, x0);
    EXPECT_NEAR(val(result), 2.0, 0.00001);
}

TEST(NewtonRaphsonTest, TestNoRootFunction) {
    auto f = [](dual x) {
        dual eq = x * x + 4;
        return eq;
    };

    NewtonRaphsonSolver solver;
    double x0      = 1.0;
    double tol     = 1e-6;
    size_t maxIter = 10000;
    // In this case, the function has no real root, so the result may not meet the tolerance criteria
    EXPECT_THROW(solver.solve(f, x0, tol, maxIter), std::runtime_error);
}