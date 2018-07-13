#ifndef SPECIES_H
#define SPECIES_H

#include "genome.h"

namespace NN
{

class Species
{
friend class GeneticAlgorithm;
friend bool compareSpecies(const Species&, const Species&);
private:
    std::vector<Genome> m_individuals;
    Genome m_representative;

    //For use by GeneticAlgorithm
    double m_species_score;
    unsigned starting_generation;
    void SetBaseScore();

public:
    int GetSize();
    Genome GetIndividual(int);
    double GetScore(int);
    void SetScore(int, double);

    void SortBest();

    Genome GetRepresentative();
    void SetRepresentative(Genome);
};

}

#endif // SPECIES_H
