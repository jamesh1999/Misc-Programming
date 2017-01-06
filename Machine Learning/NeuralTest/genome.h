#ifndef GENOME_H
#define GENOME_H
#include <vector>
#include <string>
#include "neuron.h"
#include "global.h"
#include "network.h"

namespace NN
{

typedef struct
{
    double bias = 0.0;
    unsigned id = 0;
    NeuronType type = HIDDEN;
} NeuronGene;

typedef struct
{
    double weight = 1.0;
    unsigned innovation = 0;
    unsigned source = 0;
    unsigned dest = 0;
    bool active = true;
} ConnectionGene;

typedef struct
{
    bool average_parents = false;
    bool all_genes = false;
} MatingSettings;

typedef struct
{
    double add_neuron_chance = 0.05;
    double add_connection_chance = 0.05;
    double modify_bias_chance = 0.2;
    double modify_weight_chance = 0.2;
    double function_a = 2.0;
    double function_b = -1.0;
    double max_delta = 0.1;
    bool use_function_delta = false;
    bool enable_modify_bias = true;
    bool enable_structural_changes = true;
    bool auto_trim = false;
} MutationSettings;

typedef struct
{
    double disjointWeight = 0.5;
    double excessWeight = 0.5;
    double connectionWeightWeight = 0.04;
    double neuronBiasWeight = 0.01;
    bool normalize = false;

} CompatibilitySettings;

class Genome
{
public:
    std::vector<ConnectionGene> m_connection_genes;
    std::vector<NeuronGene> m_neuron_genes;
    double m_score = 0.0;

public:
    Genome() = default;
    Genome(const Genome&);

    void CreateNetwork(Network*);
    void SetGenomeHex(std::string);
    std::string GetGenomeHex();
    void SetGenomeGenes(std::vector<NeuronGene>, std::vector<ConnectionGene>);
    void SortGenes();

    Genome Mate(const Genome&, const MatingSettings&);
    Genome Mate(const Genome&);
    Genome operator+(const Genome&);
    void Mutate(const MutationSettings&);
    void Trim();

    void SetScore(double);
    double GetScore();

    double GetIncompatibility(const Genome&, const CompatibilitySettings&);
    double GetIncompatibility(const Genome&);
    double operator^(const Genome&);

    static Genome GetBase(int,int);
};

}

#endif // GENOME_H
