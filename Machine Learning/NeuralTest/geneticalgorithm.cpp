#include "geneticalgorithm.h"
#include <iostream>
#include <algorithm>

using namespace NN;

GeneticAlgorithm::GeneticAlgorithm()
{

}

Species GeneticAlgorithm::BestSpecies()
{
    /*std::vector<double> scores;
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
        }*/

    return m_species[0];
}

Genome GeneticAlgorithm::BestIndividual()
{
    Genome* best = nullptr;
    /*double max_score = 0.0;

    for(auto s : m_species)
        for(int i = 0; i < s.m_scores.size(); ++i)
            if(s.m_scores[i] > max_score)
            {
                max_score = s.m_scores[i];
                best = &s.m_individuals[i];
            }*/

    return *best;
}

bool NN::compareSpecies(const Species& a, const Species& b)
{
    return a.m_species_score > b.m_species_score;
}

void GeneticAlgorithm::Epoch()
{
    //Repopulate & pool individuals
    std::vector<Genome> all_individuals;
    all_individuals.reserve(IndividualCount());
    for(Species& s : m_species)
    {
        RepopulateSpecies(s);
        all_individuals.insert(all_individuals.end(), s.m_individuals.begin(), s.m_individuals.end());

        //Clear species
        s.m_individuals.clear();
    }

    //Decide on species
    for(Genome& individual : all_individuals)
    {
        bool inserted = false;
        for(Species& s : m_species)
        {
            if((individual ^ s.GetRepresentative()) < m_settings.incompatability_threshold)
            {
                inserted = true;
                s.m_individuals.push_back(individual);
                break;
            }
        }

        //Create new species
        if(!inserted)
        {
            Species new_species;
            new_species.SetRepresentative(individual);
            new_species.m_individuals.push_back(individual);
            new_species.starting_generation = m_current_generation;
            m_species.push_back(new_species);
        }
    }

    for(Species& s : m_species)
        s.SetBaseScore();

    //Remove empty species
    std::vector<Species> buffer = m_species;
    m_species.clear();
    for(Species& s : buffer)
        if(s.GetSize())
            m_species.push_back(s);

    //Cull excess species
    for(Species& s : m_species)
    {
        s.SortBest();
        s.m_species_score = s.GetScore(0);

        if(m_settings.penalise_old
           && m_current_generation - s.starting_generation > m_settings.old_threshold)
            s.m_species_score -= m_settings.old_penalty;

        if(m_settings.boost_young
           && m_current_generation - s.starting_generation < m_settings.young_threshold)
            s.m_species_score += m_settings.young_bonus;

        if(m_settings.boost_new
           && m_current_generation == s.starting_generation)
            s.m_species_score += 9999.9;
    }

    std::sort(m_species.begin(), m_species.end(), compareSpecies);

    while(m_species.size() > m_settings.max_species)
        m_species.pop_back();

    ++m_current_generation;
}

void GeneticAlgorithm::RepopulateSpecies(Species& s)
{
    if(s.GetSize() == 0) return;

    s.SortBest();


    Genome child;
    if(s.GetSize() > 1)
        child = s.m_individuals[0] + s.m_individuals[1];
    else
        child = s.m_individuals[0];

    s.m_individuals.clear();
    for(int i = 0; i < m_settings.max_species_population; ++i)
        s.m_individuals.push_back(child);

    for(auto& individual : s.m_individuals)
        individual.Mutate(m_mutation_settings);
}

void GeneticAlgorithm::Start(Genome base)
{
    Species new_species;
    new_species.SetRepresentative(base);
    new_species.m_individuals.push_back(base);

    RepopulateSpecies(new_species);

    m_species.push_back(new_species);
    m_current_generation = 0;
}

void GeneticAlgorithm::Speciate()
{

}

int GeneticAlgorithm::IndividualCount()
{
    int ret = 0;
    for(Species& s : m_species)
        ret += s.GetSize();

    return ret;
}
