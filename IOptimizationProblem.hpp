#ifndef IOPTIMIZATIONPROBLEM_HPP
#define IOPTIMIZATIONPROBLEM_HPP

class IOptimizationProblem {
public:
    virtual ~IOptimizationProblem() = default;

    virtual float evaluate(const unsigned int* intValues, const float* realValues, unsigned int numGenes) const = 0;
};

#endif