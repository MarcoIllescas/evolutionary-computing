/*
Simple Genetic Algorithm

    Implementation of the SGA
*/
#include <iostream>
#include <cstdlib>	// for rand()
#include <ctime>	// for rand()

using namespace std;

//********** Genetic Algorithm Configuration Parameters **********//
const int NumGen = 2;
const int NumBitsXGen[NumGen] = {8, 8};
const int PopulationSize = 5;

typedef unsigned char BYTE;

typedef struct
{
    BYTE *Chromosome;
    float Fitness;
} Individual;

int main() {
    Individual Population[PopulationSize];
    int ChromosomeSize = 0;

    cout << "Hello Genetic Algorithm!\n" << endl;
    cout << "Generating Population..." << endl;

    // Seed for random numbers
    srand(time(NULL));

    // Calculate Chromosome size
    for(int k = 0; k < NumGen; k++) {
        ChromosomeSize += NumBitsXGen[k];
    }

    // Initialize population for individuals
    for(int k = 0; k < PopulationSize; k++) {
        // Allocate memory for each Chromosome
        Population[k].Chromosome = new BYTE[ChromosomeSize];

        // Initialize chromosome
        for(int i = 0; i < ChromosomeSize; i++) {
            Population[k].Chromosome[i] = rand() % 2;
        }
        Population[k].Fitness = 0.0;
    }

    // Print population of individuals
    for(int k = 0; k < PopulationSize; k++) {
        cout<<"INDIVIDUAL["<< k <<"]: ";
        for(int i = 0; i < ChromosomeSize; i++) {
            cout << (int)Population[k].Chromosome[i];
        }
        cout << endl;
    }

    // Free allocated memory
    for (int k = 0; k < PopulationSize; k++) {
        delete[] Population[k].Chromosome;
    }

    return 0;
}