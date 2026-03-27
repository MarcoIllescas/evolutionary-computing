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
const unsigned int POPULATION_SIZE = 50;
const unsigned int MAX_GENERATIONS = 1000;
const unsigned int PRINT_FREQUENCY = 100;

// ===================== MAIN ===================== //
int main() {
    cout << "Starting Genetic Algorithm..." << endl;

    // 1. Select problem
    SphereFunction currentProblem;

    // 2. Motor initialization
    GeneticAlgorithm ga(POPULATION_SIZE, &currentProblem);

    // ===== OPEN CSV FILE AND WRITE HEADERS ===== //
    std::ofstream csvFile("ga_results.csv");
    csvFile << "Generation,Best,Average,Worst\n";

    // ===== EVOLUTION LOOP ===== //
    for (unsigned int generation = 1; generation <= MAX_GENERATIONS; generation++) {

        ga.selectionRoulette();
        ga.crossoverOnePoint(0.88);
        ga.mutation(0.01);

        ga.applyElitism();
        ga.nextGeneration();

        // Re-evaluate
        ga.decodeToInteger();
        ga.decodeToReal();
        ga.evaluatePopulation();
        ga.computeFitness();

        // Write data
        csvFile << generation << "," << ga.getBestObjective() << "," << ga.getAvgObjective() << "," << ga.getWorstObjective() << "\n";

        if (generation == 1 || generation % PRINT_FREQUENCY == 0 || generation == MAX_GENERATIONS) {
            cout << "\nGeneration " << generation << ":" << endl;
            // ga.printPopulation();
            cout << " Best Objective Value: " << ga.getBestObjective() << endl;
        }
    }

    // ===== CLOSE CSV FILE ===== //
    csvFile.close();
    cout << "\nData saved in ga_results.csv" << endl;

    // ===== CONNECT PLOTTER ===== //
    cout << "Generating graph with Python" << endl;
    std::system("python3 plotter.py");

    cout << "\nEnd of program." << endl;
    return 0;
}