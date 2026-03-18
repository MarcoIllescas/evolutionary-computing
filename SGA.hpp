/*
    SIMPLE GENETIC ALGORITHM
    SGA.hpp
*/

#include <iostream>
#include <cmath>

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

public:
    // Destructor
    ~GeneticAlgorithm();

    // Constructor
    GeneticAlgorithm(
        unsigned int populationSize,
        unsigned int numberOfGenes,
        const unsigned int* bitsPerGene,
        const float* upperLimits,
        const float* lowerLimits
    );

    // Decoding
    void decodeToInteger();
    void decodeToReal();

    // Evaluation
    virtual float objectiveFunction(unsigned int index);
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