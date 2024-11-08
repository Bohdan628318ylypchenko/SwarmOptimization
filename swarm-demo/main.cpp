#include "pch.h"

#include "automatic_optimization.h"
#include "swarm_rndgen.h"

#include <iostream>

using swarm::core::FileRandomNumbersGenerator;
using std::cout, std::endl;

int main(void)
{
    if (FileRandomNumbersGenerator::initialize_random_numbers_source(
        L"normalized-quantum-big.rnd")
    ) {
        FileRandomNumbersGenerator::free_random_numbers_source();
        return 1;
    }

    cout << "=== simple: ===" << endl;
    swarm::demo::simple_research();
    cout << "=== ackley: ===" << endl;
    swarm::demo::ackley_research();
    cout << "=== griewank: ===" << endl;
    swarm::demo::griewank_research();
    cout << "=== rastringin: ===" << endl;
    swarm::demo::rastrigin_research();
    cout << "=== rosenbrock: ===" << endl;
    swarm::demo::rosenbrock_research();

    FileRandomNumbersGenerator::free_random_numbers_source();
    return 0;
}
