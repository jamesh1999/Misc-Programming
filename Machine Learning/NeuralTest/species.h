#ifndef SPECIES_H
#define SPECIES_H

#include "genome.h"
#include "geneticalgorithm.h"

namespace NN
{

class Species
{
friend class GeneticAlgorithm;

    std::vector<Genome> m_individuals;
    std::vector<double> m_scores;
public:
    Species();

    int GetSize();
    Genome GetIndividual();
    double GetScore();
    void SetScore(double);
};

}

#endif // SPECIES_H
