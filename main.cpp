/*
    MAIN PROGRAM
    Simple Genetic Algorithm - 100 Independent Runs Experiment
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>

#include "SGA.hpp"
#include "Problems.hpp"

using namespace std;

// ===================== GA PARAMETERS ===================== //
const unsigned int POPULATION_SIZE = 100;
const unsigned int MAX_GENERATIONS = 1000;
const unsigned int PRINT_FREQUENCY = 100;

const double CROSSOVER_PROB = 0.88;
const double MUTATION_PROB  = 0.002;

const unsigned int NUM_RUNS = 100;

// ===================== MAIN ===================== //
int main() {

    cout << "Starting 100-run Genetic Algorithm Experiment..." << endl;

    // Select optimization problem
    GriewankFunction currentProblem;

    ProblemConfiguration conf = currentProblem.getConfiguration();
    unsigned int dims = conf.dimensions;

    // Store best result from each independent run
    vector<float> bestResults;

    // File for experiment statistics
    ofstream experimentFile("experiment_results.csv");
    experimentFile << "Run,BestObjective\n";

    // File for convergence tracking (only first run)
    ofstream convergenceFile("ga_results.csv");
    convergenceFile << "Generation,Best,Average,Worst\n";


    // =====================================================
    // OUTER LOOP: 100 Independent Executions
    // =====================================================
    for(unsigned int run = 1; run <= NUM_RUNS; run++) {

        cout << "\nStarting Run " << run << endl;

        // Initialize GA from scratch for this run
        GeneticAlgorithm ga(POPULATION_SIZE, &currentProblem);

        // Initial evaluation
        ga.decodeToInteger();
        ga.decodeToReal();
        ga.evaluatePopulation();
        ga.computeFitness();


        // =================================================
        // INNER LOOP: Evolution over generations
        // =================================================
        for(unsigned int generation = 1;
            generation <= MAX_GENERATIONS;
            generation++) {

            // Genetic operators
            ga.selectionRoulette();
            ga.crossoverOnePoint(CROSSOVER_PROB);
            ga.mutation(MUTATION_PROB);

            ga.applyElitism();
            ga.nextGeneration();

            // Re-evaluate new generation
            ga.decodeToInteger();
            ga.decodeToReal();
            ga.evaluatePopulation();
            ga.computeFitness();

            // Save convergence only for first run
            if(run == 1) {
                convergenceFile << generation << "," << ga.getBestObjective() << "," << ga.getAvgObjective() << "," << ga.getWorstObjective() << "\n";

                if(generation == 1 || generation % PRINT_FREQUENCY == 0 || generation == MAX_GENERATIONS) {
                    cout << "Generation " << generation << " | Best: " << ga.getBestObjective() << endl;
                }
            }

        } // End generations loop


        // Save best solution from this run
        float runBest = ga.getBestObjective();

        bestResults.push_back(runBest);

        experimentFile
            << run
            << ","
            << runBest
            << "\n";


        if(run % 10 == 0 || run == 1) {

            cout << "Run "
                 << run
                 << " completed. Best value = "
                 << runBest
                 << endl;
        }

    } // End 100-run loop


    convergenceFile.close();
    experimentFile.close();


    // =====================================================
    // STATISTICAL ANALYSIS
    // =====================================================

    float sum = accumulate(
        bestResults.begin(),
        bestResults.end(),
        0.0f
    );

    float mean = sum / NUM_RUNS;


    float variance = 0.0f;

    for(float val : bestResults) {

        variance += pow(val - mean, 2);
    }

    variance /= NUM_RUNS;

    float std_dev = sqrt(variance);


    float overallBest =
        *min_element(
            bestResults.begin(),
            bestResults.end()
        );


    // =====================================================
    // FINAL RESULTS
    // =====================================================

    cout << "\n===================================" << endl;
    cout << "EXPERIMENT RESULTS (100 RUNS)" << endl;

    cout << "Problem Dimensions: "
         << dims
         << endl;

    cout << "Mean Best Objective: "
         << mean
         << endl;

    cout << "Standard Deviation: "
         << std_dev
         << endl;

    cout << "Overall Best Found: "
         << overallBest
         << endl;

    cout << "===================================\n"
         << endl;


    // =====================================================
    // PYTHON PLOTTERS
    // =====================================================

    cout << "Generating convergence plot..." << endl;
    system("python3 plotter.py");


    if(dims == 2) {

        cout << "Generating surface plot..." << endl;

        // Use dummy values or adapt if you want best variables tracked
        system("python3 surface_plotter.py 0 0 0");
    }
    else {

        cout
        << "Surface plot skipped "
        << "(requires 2 dimensions)."
        << endl;
    }


    cout << "\nEnd of program." << endl;

    return 0;
}