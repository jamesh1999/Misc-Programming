#include "geneticalgorithm.h"
#include <iostream>

using namespace NN;

GeneticAlgorithm::GeneticAlgorithm()
{

}

Species GeneticAlgorithm::BestSpecies()
{
    std::vector<double> scores;
    for(auto s : m_species)
    {
        double total = 0.0;
        for(auto score : s.m_scores)
            total += score;
        scores.push_back(total / s.m_scores.size());
    }

    double max = 0.0;
    int index = 0;
    for(int i = 0; i < scores.size(); ++i)
        if(scores[i] > max)
        {
            max = scores[i];
            index = i;
        }

    return m_species[index];
}

Genome GeneticAlgorithm::BestIndividual()
{
    Genome* best = nullptr;
    double max_score = 0.0;

    for(auto s : m_species)
        for(int i = 0; i < s.m_scores.size(); ++i)
            if(s.m_scores[i] > max_score)
            {
                max_score = s.m_scores[i];
                best = &s.m_individuals[i];
            }

    return *best;
}

void GeneticAlgorithm::Epoch()
{
    for(auto& s : m_species)
        RepopulateSpecies(s);
    std::cout <<m_species[0].m_individuals.size()<<std::endl;
}

void GeneticAlgorithm::RepopulateSpecies(Species& s)
{
    double best = s.m_scores[0];
    int best_index = 0;
    double second = s.m_scores[1];
    int second_index = 1;

    //Get two best
    for(int i = 2; i < s.m_scores.size(); ++i)
    {
        if(s.m_scores[i] > best)
        {
            second = best;
            second_index = best_index;
            best = s.m_scores[i];
            best_index = i;
        }
        else if(s.m_scores[i] > second)
        {
            second = s.m_scores[i];
            second_index = i;
        }
    }

    Genome child = s.m_individuals[best_index] + s.m_individuals[second_index];

    s.m_individuals.clear();
    for(int i = 0; i < 10; ++i)
        s.m_individuals.push_back(child);

    for(auto& individual : s.m_individuals)
        individual.Mutate(m_mutation_settings);
}

void GeneticAlgorithm::Start(Genome base)
{
    m_species.push_back(Species());

    for(int i = 0; i < 10; ++i)
    {
        m_species[0].m_scores.push_back(0.0);
        m_species[0].m_individuals.push_back(base);
    }
}

void GeneticAlgorithm::Speciate()
{

}
