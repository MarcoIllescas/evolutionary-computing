/*
    Simple Genetic Algorithm
    Implementation of the SGA
*/
#include <iostream>
#include <ctime>	// for rand()
#include <cmath>

using namespace std;

//********** Genetic Algorithm Configuration Parameters **********//
const int NumGen = 5;
const int NumBitsXGen[NumGen] = {4, 2, 4, 8, 4};
const int PopulationSize = 3;

typedef unsigned char BYTE;

typedef struct
{
    BYTE *Chromosome;
    unsigned int *IntValues;
    float Fitness;
} Individual;

int main() {
    Individual Population[PopulationSize];
    int ChromosomeSize = 0;

    cout << "Hello Genetic Algorithm!" << endl;
    cout << "Generating Population...\n" << endl;

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
        Population[k].IntValues = new unsigned int[NumGen];

        // Initialize chromosome
        for(int i = 0; i < ChromosomeSize; i++) {
            Population[k].Chromosome[i] = rand() % 2;
        }
        Population[k].Fitness = 0.0;
    }

    // Decodification to Int
    for(int k = 0; k < PopulationSize; k++) {
        int Position = 0;

        for(int g = 0; g < NumGen; g++) {
            unsigned int Value = 0;

            // Bit of the gen
            for (int b = 0; b < NumBitsXGen[g]; b++) {
                // Bit in the relative position b
                BYTE bit = Population[k].Chromosome[Position + b];
                Value |= (bit << b);
            }

            Population[k].IntValues[g] = Value;
            Position += NumBitsXGen[g];
        }
    }

    // Print population of individuals
    for(int k = 0; k < PopulationSize; k++) {
        cout << "INDIVIUAL [" << k << "]: { ";

        int GlobalPosition = 0;

        for(int g = 0; g < NumGen; g++) {
            for(int b = NumBitsXGen[g] - 1; b >= 0; b--) {
                cout << (int)Population[k].Chromosome[GlobalPosition + b];
            }

            if (g < NumGen - 1) { cout << " : "; }

            GlobalPosition += NumBitsXGen[g];
        }
        cout << " }" << endl;

        cout << "DECODIFICATION [" << k << "]: { ";
        for (int g = 0; g < NumGen; g++) {
            cout << "GEN[" << g << "]: " << Population[k].IntValues[g];
            if (g < NumGen - 1) { cout << " --- "; }
        }
        cout << " }" << endl;
        cout << "--------------------------------------------------------" << endl;
    }

    // Free allocated memory
    for (int k = 0; k < PopulationSize; k++) {
        delete[] Population[k].Chromosome;
        delete[] Population[k].IntValues;
    }

    return 0;
}