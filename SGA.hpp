/*
    SIMPLE GENETIC ALGORITHM
    SGA.hpp
*/

#include <iostream>
#include <cmath>
#include <random>

#include "IOptimizationProblem.hpp"

using namespace std;

// ===================== TYPE DEFINITIONS ===================== //
typedef unsigned char BYTE;

enum OptimizationType {
    MAXIMIZE,
    MINIMIZE
};

// ===================== INDIVIDUAL STRUCT ===================== //
struct Individual {
    BYTE* chromosome;

    unsigned int* intValues;
    float* realValues;

    float objectiveValue;
    float fitnessValue;

    unsigned int parent1;
    unsigned int parent2;
};

// ===================== GENETIC ALGORITHM CLASS ===================== //
class GeneticAlgorithm {
private:
    std::mt19937 randomGenerator;

    // Population
    Individual* population;
    Individual* newPopulation;

    unsigned int* selectionIndices;

    // Configuration
    unsigned int chromosomeSize;
    unsigned int numberOfGenes;
    const unsigned int* bitsPerGene;
    unsigned int populationSize;

    const float* upperLimits;
    const float* lowerLimits;

    // Statistics
    unsigned int bestIndex;
    unsigned int worstIndex;

    float sumObjective;
    float avgObjective;

    float sumFitness;
    float avgFitness;

    const IOptimizationProblem* problem;

public:
    // Destructor
    ~GeneticAlgorithm();

    // Constructor
    GeneticAlgorithm(
        unsigned int populationSize,
        unsigned int numberOfGenes,
        const unsigned int* bitsPerGene,
        const float* upperLimits,
        const float* lowerLimits,
        const IOptimizationProblem* problem
    );

    // Decoding
    void decodeToInteger();
    void decodeToReal();

    // Evaluation
    void evaluatePopulation();
    void computeFitness(OptimizationType type);

    // Selection
    void selectionRoulette();

    // Genetic operators
    void crossoverOnePoint(double probabilityCrossover);
    void mutation(double probabilityMutation);

    // Elitism
    void applyElitism();

    // Population management
    void nextGeneration();

    // Debug / Output
    void printIndividual(unsigned int index);
    void printPopulation();
};