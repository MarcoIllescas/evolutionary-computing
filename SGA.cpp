/*
    SIMPLE GENETIC ALGORITHM
    SGA.cpp
*/
#include "SGA.hpp"

// ===================== CONSTRUCTOR ===================== //
GeneticAlgorithm::GeneticAlgorithm(
    unsigned int populationSize,
    unsigned int numberOfGenes,
    const unsigned int* bitsPerGene,
    const float* upperLimits,
    const float* lowerLimits
)
    : populationSize(populationSize),
      numberOfGenes(numberOfGenes),
      bitsPerGene(bitsPerGene),
      upperLimits(upperLimits),
      lowerLimits(lowerLimits),
      chromosomeSize(0),
      bestIndex(0),
      worstIndex(0),
      sumObjective(0.0f),
      avgObjective(0.0f),
      sumFitness(0.0f),
      avgFitness(0.0f)
{
    this->populationSize = populationSize;
    this->numberOfGenes = numberOfGenes;
    this->bitsPerGene = bitsPerGene;
    this->upperLimits = upperLimits;
    this->lowerLimits = lowerLimits;

    chromosomeSize = 0;

    // Compute chromosome size
    for (unsigned int i = 0; i < numberOfGenes; i++)
    {
        chromosomeSize += bitsPerGene[i];
    }

    // Allocate memory
    population = new Individual[populationSize];
    newPopulation = new Individual[populationSize];
    selectionIndices = new unsigned int[populationSize];

    // Initialize individuals
    for (unsigned int i = 0; i < populationSize; i++)
    {
        population[i].chromosome = new BYTE[chromosomeSize];
        population[i].intValues = new unsigned int[numberOfGenes];
        population[i].realValues = new float[numberOfGenes];

        newPopulation[i].chromosome = new BYTE[chromosomeSize];
        newPopulation[i].intValues = new unsigned int[numberOfGenes];
        newPopulation[i].realValues = new float[numberOfGenes];

        // Random initialization
        for (unsigned int j = 0; j < chromosomeSize; j++)
        {
            population[i].chromosome[j] = rand() % 2;
        }

        // Initialize values
        for (unsigned int g = 0; g < numberOfGenes; g++)
        {
            population[i].intValues[g] = 0;
            population[i].realValues[g] = 0.0f;
        }

        population[i].fitnessValue = 0.0f;
        population[i].objectiveValue = 0.0f;
    }
}

// ===================== DECODE REAL ===================== //
void GeneticAlgorithm::decodeToReal() {
    for (unsigned int i = 0; i < populationSize; i++) {
        for (unsigned int g = 0; g < numberOfGenes; g++) {
            float range = upperLimits[g] - lowerLimits[g];
            float denominator = pow(2, bitsPerGene[g]) - 1;

            population[i].realValues[g] = ((population[i].intValues[g] / denominator) * range) + lowerLimits[g];
        }
    }
}

// ===================== DECODE INTEGER ===================== //
void GeneticAlgorithm::decodeToInteger() {
    for (unsigned int i = 0; i < populationSize; i++) {

        unsigned int indexBit = 0;

        for (unsigned int g = 0; g < numberOfGenes; g++) {

            unsigned int value = 0;

            for (unsigned int b = 0; b < bitsPerGene[g]; b++) {
                value += population[i].chromosome[indexBit] << b;
                indexBit++;
            }

            population[i].intValues[g] = value;
        }
    }
}

// ===================== PRINT INDIVIDUAL ===================== //
void GeneticAlgorithm::printIndividual(unsigned int index) {
    unsigned int accumulated = chromosomeSize - 1;
    int g = numberOfGenes - 1;

    cout << "[" << index << "]";

    for (int i = chromosomeSize - 1; i >= 0; i--) {
        if (i == (int)accumulated) {
            cout << ": ";
            accumulated -= bitsPerGene[g];
            g--;
        }
        cout << (int)population[index].chromosome[i];
    }

    cout << " | INT: ";
    for (int j = numberOfGenes - 1; j >= 0; j--) {
        cout << population[index].intValues[j] << " ";
    }

    cout << "| REAL: ";
    for (int j = numberOfGenes - 1; j >= 0; j--) {
        cout << population[index].realValues[j] << " ";
    }

    cout << "| Obj: " << population[index].objectiveValue;
    cout << " Fit: " << population[index].fitnessValue;
    cout << endl;
}

// ===================== PRINT POPULATION ===================== //
void GeneticAlgorithm::printPopulation() {
    for (unsigned int i = 0; i < populationSize; i++) {
        printIndividual(i);
    }

    cout << "Best ID: " << bestIndex << endl;
    cout << "Worst ID: " << worstIndex << endl;
    cout << "Avg Obj: " << avgObjective << endl;
    cout << "Avg Fit: " << avgFitness << endl;
}

// ===================== EVALUATION ===================== //
void GeneticAlgorithm::evaluatePopulation() {
    bestIndex = 0;
    worstIndex = 0;
    sumObjective = 0.0f;

    for (unsigned int i = 0; i < populationSize; i++) {
        population[i].objectiveValue = objectiveFunction(i);

        if (population[i].objectiveValue > population[bestIndex].objectiveValue)
            bestIndex = i;

        if (population[i].objectiveValue < population[worstIndex].objectiveValue)
            worstIndex = i;

        sumObjective += population[i].objectiveValue;
    }

    avgObjective = sumObjective / populationSize;
}

// ===================== FITNESS ===================== //
void GeneticAlgorithm::computeFitness(OptimizationType type) {
    sumFitness = 0.0f;

    if (type == MAXIMIZE) {
        float range = population[bestIndex].objectiveValue - population[worstIndex].objectiveValue;

        for (unsigned int i = 0; i < populationSize; i++) {
            population[i].fitnessValue =
                100 * ((population[i].objectiveValue - population[worstIndex].objectiveValue) / range);

            sumFitness += population[i].fitnessValue;
        }
    } else {
        for (unsigned int i = 0; i < populationSize; i++) {
            population[i].fitnessValue =
                population[bestIndex].objectiveValue - population[i].objectiveValue;
        }

        float minFit = population[worstIndex].fitnessValue;
        float range = population[bestIndex].fitnessValue - minFit;

        for (unsigned int i = 0; i < populationSize; i++) {
            population[i].fitnessValue =
                100 * ((population[i].fitnessValue - minFit) / range);

            sumFitness += population[i].fitnessValue;
        }
    }

    avgFitness = sumFitness / populationSize;
}

// ===================== ROULETTE ===================== //
void GeneticAlgorithm::selectionRoulette() {
    float cumulativeProb[populationSize];
    float sum = 0.0f;

    for (unsigned int i = 0; i < populationSize; i++) {
        sum += population[i].fitnessValue / sumFitness;
        cumulativeProb[i] = sum;
    }

    for (unsigned int i = 0; i < populationSize; i++) {
        float r = (float)rand() / RAND_MAX;
        unsigned int sel = 0;

        while (cumulativeProb[sel] < r) {
            sel++;
        }

        selectionIndices[i] = sel;
    }
}

// ===================== CROSSOVER ===================== //
void GeneticAlgorithm::crossoverOnePoint(double probCrossover) {
    unsigned int limit = chromosomeSize - 1;

    for (unsigned int i = 0; i < populationSize; i += 2) {
        double r = (double)rand() / RAND_MAX;

        unsigned int parent1 = selectionIndices[i];
        unsigned int parent2 = selectionIndices[i + 1];

        unsigned int crossoverPoint = rand() % limit;

        for (unsigned int j = 0; j < chromosomeSize; j++) {
            if (r < probCrossover && j > crossoverPoint) {
                newPopulation[i].chromosome[j] = population[parent2].chromosome[j];
                newPopulation[i + 1].chromosome[j] = population[parent1].chromosome[j];
            } else {
                newPopulation[i].chromosome[j] = population[parent1].chromosome[j];
                newPopulation[i + 1].chromosome[j] = population[parent2].chromosome[j];
            }
        }
    }
}

// ===================== MUTATION ===================== //
void GeneticAlgorithm::mutation(double probMutation) {
    for (unsigned int i = 0; i < populationSize; i++) {
        for (unsigned int j = 0; j < chromosomeSize; j++) {
            double r = (double)rand() / RAND_MAX;

            if (r < probMutation) {
                newPopulation[i].chromosome[j] =
                    1 - newPopulation[i].chromosome[j];
            }
        }
    }
}

// ===================== ELITISM ===================== //
void GeneticAlgorithm::applyElitism() {
    // Copy best individual into new population replacing worst
    for (unsigned int j = 0; j < chromosomeSize; j++) {
        newPopulation[worstIndex].chromosome[j] =
            population[bestIndex].chromosome[j];
    }
}

// ===================== NEXT GENERATION ===================== //
void GeneticAlgorithm::nextGeneration() {
    Individual* temp = population;
    population = newPopulation;
    newPopulation = temp;
}

// ===================== DESTRUCTOR ===================== //
GeneticAlgorithm::~GeneticAlgorithm() {
    // Free individuals in population
    for (unsigned int i = 0; i < populationSize; i++) {
        delete[] population[i].chromosome;
        delete[] population[i].intValues;
        delete[] population[i].realValues;

        delete[] newPopulation[i].chromosome;
        delete[] newPopulation[i].intValues;
        delete[] newPopulation[i].realValues;
    }

    // Free arrays
    delete[] population;
    delete[] newPopulation;
    delete[] selectionIndices;
}

float GeneticAlgorithm::objectiveFunction(unsigned int index) {
    float x = population[index].intValues[0];

    // Example 1: Maximization parabola
    return 250 - pow(x - 155, 2);

    // ===== Other test cases ===== //

    // Root finding:
    // return (x * x) - (23 * x) - 50;

    // Minimization:
    // return pow(x - 115, 2);

    // Multi-variable:
    // float x0 = population[index].realValues[0];
    // float x1 = population[index].realValues[1];
    // return 250 - pow(x0 - 64, 2) - pow(x1 - 128, 2);
}