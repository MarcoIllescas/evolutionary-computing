/*
    MAIN PROGRAM
    Usage of the Simple Genetic Algorithm
*/
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "SGA.hpp"
#include "Problems.hpp"

using namespace std;

// ===================== GA CONFIGURATION ===================== //
const unsigned int NUMBER_OF_GENES = 1;
const unsigned int BITS_PER_GENE[NUMBER_OF_GENES] = {8};

const unsigned int POPULATION_SIZE = 10;
const unsigned int MAX_GENERATIONS = 100;

const unsigned int PRINT_FREQUENCY = 10;

const float UPPER_LIMITS[NUMBER_OF_GENES] = {255};
const float LOWER_LIMITS[NUMBER_OF_GENES] = {0};

// ===================== MAIN ===================== //
int main() {
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

    // ===== OPEN CSV FILE AND WRITE HEADERS ===== //
    std::ofstream csvFile("ga_results.csv");
    csvFile << "Generation, Best, Average, Worst\n";

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

        // Write data
        csvFile << generation << "," << ga.getBestObjective() << "," << ga.getAvgObjective() << "," << ga.getWorstObjective() << "\n";

        if (generation == 1 || generation % PRINT_FREQUENCY == 0 || generation == MAX_GENERATIONS) {
            cout << "\nGeneration " << generation << ":" << endl;
            ga.printPopulation();
        }
    }

    // ===== CLOSE CSV FILE ===== //
    csvFile.close();
    cout << "\nData saved in ga_results.csv" << endl;

    cout << "\nEnd of program." << endl;
    return 0;
}