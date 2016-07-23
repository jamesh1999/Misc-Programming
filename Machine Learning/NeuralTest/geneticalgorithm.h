#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H
#include <vector>
#include "genome.h"

namespace NN
{

typedef struct
{
    double complexity_bias_amount = 1.0;
    double incompatability_threshold = 0.2;
    double incompatability_max = 0.6;
    double incompatability_min = 0.05;
    double adult_survival_percentage = 0.05;
    double interbreed_chance = 0.01;
    unsigned max_species = 10;
    unsigned max_population = 100;
    unsigned max_species_population = 10;
    bool complexity_bias = false;
    bool adaptive_compatability = false;
    bool interbreed_excess = false;
    bool random_interbreed = false;
    bool reuse_best = false;
    bool best_adults_survive = false;

} GASettings;

typedef struct
{
    std::vector<Genome> m_individuals;
    std::vector<double> m_scores;
} Species;

class GeneticAlgorithm
{
public:
    MutationSettings m_mutation_settings;
    std::vector<Species> m_species;
    Genome m_best_ever;

    void RepopulateSpecies(Species&);
public:
    GeneticAlgorithm();

    void Start(Genome);
    void Epoch();
    void Speciate();
    Genome BestIndividual();
    Species BestSpecies();
};

}

#endif // GENETICALGORITHM_H
