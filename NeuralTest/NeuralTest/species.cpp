#include "species.h"
#include <algorithm>
#include <utility>
#include <cassert>

using namespace NN;

int Species::GetSize()
{
    return m_individuals.size();
}

Genome Species::GetIndividual(int index)
{
    assert(index < m_individuals.size());
    return m_individuals[index];
}

double Species::GetScore(int index)
{
    assert(index < m_individuals.size());
    return m_individuals[index].GetScore();
}

void Species::SetScore(int index, double val)
{
    assert(index < m_individuals.size());
    m_individuals[index].SetScore(val);
}

bool compareIndividual(Genome& a, Genome& b)
{
    return a.GetScore() > b.GetScore();
}

void Species::SortBest()
{
    std::sort(m_individuals.begin(), m_individuals.end(), compareIndividual);
}

Genome Species::GetRepresentative()
{
    return m_representative;
}

void Species::SetRepresentative(Genome new_representative)
{
    m_representative = new_representative;
}

void Species::SetBaseScore()
{
    for(Genome& g : m_individuals)
        g.SetScore(m_representative.GetScore());
}
