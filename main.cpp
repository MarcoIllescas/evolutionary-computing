/*
    MAIN PROGRAM
    Usage of the Simple Genetic Algorithm
*/
#include <iostream>
#include <ctime>
#include "SGA.hpp"
#include "Problems.hpp"

using namespace std;

// ===================== GA CONFIGURATION ===================== //
const unsigned int NUMBER_OF_GENES = 1;
const unsigned int BITS_PER_GENE[NUMBER_OF_GENES] = {8};

const unsigned int POPULATION_SIZE = 10;
const unsigned int MAX_GENERATIONS = 100;

const float UPPER_LIMITS[NUMBER_OF_GENES] = {255};
const float LOWER_LIMITS[NUMBER_OF_GENES] = {0};

// ===================== MAIN ===================== //
int main() {
    srand((unsigned)time(nullptr));

    cout << "Starting Genetic Algorithm..." << endl;

    MaximizeParabola currentProblem;

    GeneticAlgorithm ga(
        POPULATION_SIZE,
        NUMBER_OF_GENES,
        BITS_PER_GENE,
        UPPER_LIMITS,
        LOWER_LIMITS,
        &currentProblem
    );

    // ===== INITIAL EVALUATION ===== //
    ga.decodeToInteger();
    ga.decodeToReal();
    ga.evaluatePopulation();
    ga.computeFitness(MAXIMIZE);

    cout << "Initial Population:" << endl;
    ga.printPopulation();

    // ===== EVOLUTION LOOP ===== //
    for (unsigned int generation = 1; generation <= MAX_GENERATIONS; generation++) {

        ga.selectionRoulette();
        ga.crossoverOnePoint(0.88);
        ga.mutation(0.03);

        ga.applyElitism();

        ga.nextGeneration();

        // Re-evaluate
        ga.decodeToInteger();
        ga.decodeToReal();
        ga.evaluatePopulation();
        ga.computeFitness(MAXIMIZE);

        cout << "\nGeneration " << generation << ":" << endl;
        ga.printPopulation();
    }

    cout << "\nEnd of program." << endl;
    return 0;
}