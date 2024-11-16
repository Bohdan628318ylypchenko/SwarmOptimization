#include "pch.h"

#include "automatic_optimization.h"
#include "demo_run.h"
#include "swarm_rndgen.h"

#include "swarm_solution.h"
#include "target_functions.h"

#include <iostream>

using swarm::core::FileRandomNumbersGenerator;
using swarm::core::Solution;

using swarm::demo::OptimizationParameters;
using swarm::demo::demo_run_full;
using swarm::demo::demo_run_value;
using swarm::demo::griewank;
using swarm::demo::rastrigin;
using swarm::demo::rosenbrock;
using swarm::demo::simple;
using swarm::demo::ackley;

using std::cout, std::endl;

int main(void)
{
    if (FileRandomNumbersGenerator::initialize_random_numbers_source(
        L"normalized-quantum-big.rnd")
    ) {
        FileRandomNumbersGenerator::free_random_numbers_source();
        return 1;
    }

    // uncomment to execute automatic optimization parameters selection (takes A LOT OF TIME!)
    //cout << "=== griewank: ===" << endl;
    //swarm::demo::griewank_research();
    //cout << "=== rastringin: ===" << endl;
    //swarm::demo::rastrigin_research();
    //cout << "=== rosenbrock: ===" << endl;
    //swarm::demo::rosenbrock_research();
    //cout << "=== simple: ===" << endl;
    //swarm::demo::simple_research();
    //cout << "=== ackley: ===" << endl;
    //swarm::demo::ackley_research();

    // Optimal parameters. To ensure, execute automatic optimization.
    //OptimizationParameters op
    //{
    //    2000, 4, 2, 20000, 1.2, 100, 0.01
    //};

    //cout << "=== griewank final run: ===" << endl;
    //demo_run_full(op, griewank);

    //cout << "=== rastringin final run: ===" << endl;
    //demo_run_full(op, rastrigin);

    //cout << "=== rosenbrock final run: ===" << endl;
    //demo_run_full(op, rosenbrock);

    //cout << "=== simple final run: ===" << endl;
    //demo_run_full(op, simple);

    //cout << "=== ackley final run: ===" << endl;
    //demo_run_full(op, ackley);

    OptimizationParameters ops[]
    {
        { 100, 4, 2, 20000, 1.2, 100, 0.01 },
        { 200, 4, 2, 20000, 1.2, 100, 0.01 },
        { 500, 4, 2, 20000, 1.2, 100, 0.01 },
        { 1000, 4, 2, 20000, 1.2, 100, 0.01 },
        { 1500, 4, 2, 20000, 1.2, 100, 0.01 },
        { 2000, 4, 2, 20000, 1.2, 100, 0.01 },
        { 4000, 4, 2, 20000, 1.2, 100, 0.01 },
        { 8000, 4, 2, 20000, 1.2, 100, 0.01 },
    };

    cout << "=== griewank particle count research: ===" << endl;
    for (auto& t : ops)
        demo_run_value(t, griewank);

    cout << "=== rastrigin particle count research: ===" << endl;
    for (auto& t : ops)
        demo_run_value(t, rastrigin);

    cout << "=== rosenbrock particle count research: ===" << endl;
    for (auto& t : ops)
        demo_run_value(t, rosenbrock);

    cout << "=== simple particle count research: ===" << endl;
    for (auto& t : ops)
        demo_run_value(t, simple);

    cout << "=== ackley particle count research: ===" << endl;
    for (auto& t : ops)
        demo_run_value(t, ackley);

    FileRandomNumbersGenerator::free_random_numbers_source();
    return 0;
}
