/*
    Simple Genetic Algorithm
    Implementation of the SGA
*/
#include <iostream>
#include <ctime>	// for rand()
#include <cmath>

using namespace std;

//********** Genetic Algorithm Configuration Parameters **********//
const int NumGen = 3;
const int NumBitsXGen[NumGen] = {2, 4, 8};
const int PopulationSize = 5;
const float LowerLimit[NumGen] = {0, 0, 0};
const float UpperLimit[NumGen] = {1, 1, 1};

typedef unsigned char BYTE;

typedef struct
{
    BYTE *Chromosome;
    unsigned int *IntValue;
    float Fitness;
    float *RealValue;
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
        Population[k].IntValue = new unsigned int[NumGen];
        Population[k].RealValue = new float[NumGen];

        // Initialize chromosome
        for(int i = 0; i < ChromosomeSize; i++) {
            Population[k].Chromosome[i] = rand() % 2;
        }

        for (int j = 0; j < NumGen; j++) {
            Population[k].IntValue[j] = 0;
            Population[k].RealValue[j] = 0.0;
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

            Population[k].IntValue[g] = Value;
            Position += NumBitsXGen[g];
        }
    }

    // Decodification to real values
    for(int k = 0; k < PopulationSize; k++) {
        float range;
        float Denominator;

        for (int g = 0; g < NumGen; g++) {
            range = UpperLimit[g] - LowerLimit[g];
            Denominator = pow(2, NumBitsXGen[g]) - 1;
            Population[k].RealValue[g] = ((Population[k].IntValue[g]/Denominator) * range) + LowerLimit[g];
        }
    }

    // Print population of individuals
    for(int k = 0; k < PopulationSize; k++) {
        cout << "INDIVIUAL [" << k << "]:" << endl;

        // 1. Print Gen (Binary)
        cout << "   CHROMOSOME: { ";
        int GlobalPosition = 0;
        for (int g = 0; g < NumGen; g++) {
            for (int b = NumBitsXGen[g] - 1; b >= 0; b--) {
                cout << (int)Population[k].Chromosome[GlobalPosition + b];
            }
            if (g < NumGen - 1) { cout << " : "; }
            GlobalPosition += NumBitsXGen[g];
        }
        cout << " }" << endl;

        // 2. Print Fenotype (Integer Value)
        cout << "   DECODIFICATION TO INT: { ";
        for (int g = 0; g < NumGen; g++) {
            cout << Population[k].IntValue[g];
            if (g < NumGen - 1) { cout << " | "; }
        }
        cout << " }" << endl;

        // 3. Print Fenotype (Float Value)
        cout << "   REAL VALUE: { ";
        for (int g = 0; g < NumGen; g++) {
            cout << Population[k].RealValue[g];
            if (g < NumGen - 1) { cout << " | "; }
        }
        cout << " }" << endl;

        cout << "--------------------------------------------------------" << endl;
    }

    // Free allocated memory
    for (int k = 0; k < PopulationSize; k++) {
        delete[] Population[k].Chromosome;
        delete[] Population[k].IntValue;
        delete[] Population[k].RealValue;
    }

    return 0;
}