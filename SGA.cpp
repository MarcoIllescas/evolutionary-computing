/*
    SIMPLE GENETIC ALGORITHM
    SGA.cpp
*/
#include "SGA.hpp"

// ===================== CONSTRUCTOR ===================== //
GeneticAlgorithm::GeneticAlgorithm(
    unsigned int populationSize,
    const IOptimizationProblem* problem
)
    : populationSize(populationSize),
      problem(problem),
      randomGenerator(std::random_device{}())
{
    ProblemConfiguration configuration = problem->getConfiguration();
    this->numberOfGenes = configuration.dimensions;
    this->bitsPerDimension = configuration.bitsPerDimension;
    this->upperLimit = configuration.upperLimit;
    this->lowerLimit = configuration.lowerLimit;

    chromosomeSize = numberOfGenes * bitsPerDimension;

    // Safe initialization
    bestIndex = 0;
    worstIndex = 0;
    sumObjective = 0.0f;
    avgObjective = 0.0f;
    sumFitness = 0.0f;
    avgFitness = 0.0f;

    // Allocate memory
    population = new Individual[populationSize];
    newPopulation = new Individual[populationSize];
    selectionIndices = new unsigned int[populationSize];

    // Randomness bit distribution
    std::uniform_int_distribution<int> distBit(0, 1);

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
            population[i].chromosome[j] = distBit(randomGenerator);
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
            float range = upperLimit - lowerLimit;
            float denominator = std::pow(2, bitsPerDimension) - 1;

            population[i].realValues[g] = ((population[i].intValues[g] / denominator) * range) + lowerLimit;
        }
    }
}

// ===================== DECODE INTEGER ===================== //
void GeneticAlgorithm::decodeToInteger() {
    for (unsigned int i = 0; i < populationSize; i++) {
        unsigned int indexBit = 0;

        for (unsigned int g = 0; g < numberOfGenes; g++) {
            unsigned int value = 0;

            for (unsigned int b = 0; b < bitsPerDimension; b++) {
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
            accumulated -= bitsPerDimension;
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
    cout << "\n";
}

// ===================== PRINT POPULATION ===================== //
void GeneticAlgorithm::printPopulation() {
    for (unsigned int i = 0; i < populationSize; i++) {
        printIndividual(i);
    }

    cout << "Best ID: " << bestIndex << "\n";
    cout << "Worst ID: " << worstIndex << "\n";
    cout << "Avg Obj: " << avgObjective << "\n";
    cout << "Avg Fit: " << avgFitness << std::endl;
}

// ===================== EVALUATION ===================== //
void GeneticAlgorithm::evaluatePopulation() {
    OptimizationType type = problem->getConfiguration().type;

    sumObjective = 0.0f;
    for (unsigned int i = 0; i < populationSize; i++) {
        population[i].objectiveValue = problem->evaluate(
            population[i].intValues,
            population[i].realValues,
            numberOfGenes
        );
        sumObjective += population[i].objectiveValue;
    }

    avgObjective = sumObjective / populationSize;

    bestIndex = 0;
    worstIndex = 0;

    for (unsigned int i = 0; i < populationSize; i++) {
        if (type == MAXIMIZE) {
            if (population[i].objectiveValue > population[bestIndex].objectiveValue) bestIndex = i;
            if (population[i].objectiveValue < population[worstIndex].objectiveValue) worstIndex = i;
        } else {
            if (population[i].objectiveValue < population[bestIndex].objectiveValue) bestIndex = i;
            if (population[i].objectiveValue > population[worstIndex].objectiveValue) worstIndex = i;
        }
    }
}

// ===================== FITNESS ===================== //
void GeneticAlgorithm::computeFitness() {
    OptimizationType type = problem->getConfiguration().type;
    sumFitness = 0.0f;

    if (type == MAXIMIZE) {
        float maxObj = population[bestIndex].objectiveValue;
        float minObj = population[worstIndex].objectiveValue;
        float range = maxObj - minObj;

        if (range == 0.0f) {
            for (unsigned int i = 0; i < populationSize; i++) {
                population[i].fitnessValue = 1.0f;
                sumFitness += population[i].fitnessValue;
            }
        } else {
            for (unsigned int i = 0; i < populationSize; i++) {
                population[i].fitnessValue = population[bestIndex].objectiveValue - population[i].objectiveValue;
            }

            float minFit = population[worstIndex].fitnessValue;
            float maxFit = population[bestIndex].fitnessValue;
            float fitRange = maxFit - minFit;

            if (fitRange != 0.0f) {
                sumFitness = 0.0f;
                for (unsigned int i = 0; i < populationSize; i++) {
                    population[i].fitnessValue = 100.0f * ((population[i].fitnessValue - minFit) / fitRange);
                    sumFitness += population[i].fitnessValue;
                }
            }
        }
    }
    else if (type == MINIMIZE) {
        float minObj = population[bestIndex].objectiveValue;
        float maxObj = population[worstIndex].objectiveValue;
        float range = maxObj - minObj;

        if (range == 0.0f) {
            for (unsigned int i = 0; i < populationSize; i++) {
                population[i].fitnessValue = 1.0f;
                sumFitness += population[i].fitnessValue;
            }
        } else {
            for (unsigned int i = 0; i < populationSize; i++) {
                population[i].fitnessValue = population[worstIndex].objectiveValue - population[i].objectiveValue;
            }

            float minFit = population[worstIndex].fitnessValue;
            float maxFit = population[bestIndex].fitnessValue;
            float fitRange = maxFit - minFit;

            if (fitRange != 0.0f) {
                sumFitness = 0.0f;
                for (unsigned int i = 0; i < populationSize; i++) {
                    population[i].fitnessValue = 100.0f * ((population[i].fitnessValue - minFit) / fitRange);
                    sumFitness += population[i].fitnessValue;
                }
            }
        }
    }

    if (sumFitness == 0.0f) {
        for (unsigned int i = 0; i < populationSize; i++) {
            population[i].fitnessValue = 1.0f;
        }
        sumFitness = populationSize;
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

    cumulativeProb[populationSize - 1] = 1.0f;

    // Random initialization
    std::uniform_real_distribution<float> distProb(0.0f, 1.0f);

    for (unsigned int i = 0; i < populationSize; i++) {
        float r = distProb(randomGenerator);
        unsigned int sel = 0;

        while (sel < populationSize - 1 && cumulativeProb[sel] < r) {
            sel++;
        }

        selectionIndices[i] = sel;
    }
}

// ===================== CROSSOVER ===================== //
void GeneticAlgorithm::crossoverOnePoint(double probCrossover) {
    unsigned int limit = chromosomeSize - 1;

    // Distribution for the crossover probability
    std::uniform_real_distribution<double> distProb(0.0, 1.0);
    // Distribution to choose the uniform cutoff point [0, limit - 1]
    std::uniform_int_distribution<unsigned int> distPoint(0, limit > 0 ? limit - 1 : 0);

    for (unsigned int i = 0; i < populationSize; i += 2) {
        double r = distProb(randomGenerator);

        unsigned int parent1 = selectionIndices[i];
        unsigned int parent2 = selectionIndices[i + 1];

        unsigned int crossoverPoint = distPoint(randomGenerator);

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
    // Random initialization
    std::uniform_real_distribution<double> distProb(0.0, 1.0);

    for (unsigned int i = 0; i < populationSize; i++) {
        for (unsigned int j = 0; j < chromosomeSize; j++) {
            double r = distProb(randomGenerator);

            if (r < probMutation) {
                newPopulation[i].chromosome[j] = 1 - newPopulation[i].chromosome[j];
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

