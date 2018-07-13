#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H
#include <vector>
#include "genome.h"
#include "species.h"

namespace NN
{

typedef struct
{
    double complexity_bias_amount = 1.0;
    double incompatability_threshold = 100;
    double incompatability_max = 0.6;
    double incompatability_min = 0.05;
    double adult_survival_percentage = 0.05;
    double interbreed_chance = 0.01;
    double young_bonus = 20.0;
    double old_penalty = -5.0;
    unsigned max_species = 3;
    unsigned max_population = 100;
    unsigned max_species_population = 5;
    unsigned old_threshold = 8;
    unsigned young_threshold = 2;
    bool complexity_bias = false;
    bool adaptive_compatability = false;
    bool interbreed_excess = false;
    bool random_interbreed = false;
    bool reuse_best = false;
    bool best_adults_survive = false;
    bool boost_young = true;
    bool penalise_old = true;
    bool boost_new = true;
} GASettings;

class GeneticAlgorithm
{
public:
    MutationSettings m_mutation_settings;
    GASettings m_settings;
    std::vector<Species> m_species;
    Genome m_best_ever;
    unsigned m_current_generation;

    void RepopulateSpecies(Species&);
public:
    GeneticAlgorithm();

    void Start(Genome);
    void Epoch();
    void Speciate();
    Genome BestIndividual();
    Species BestSpecies();
    int IndividualCount();
};

}

#endif // GENETICALGORITHM_H
