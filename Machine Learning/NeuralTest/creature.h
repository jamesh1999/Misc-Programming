#ifndef CREATURE_H
#define CREATURE_H

#include "global.h"
#include <vector>
#include <string>
#include "food.h"
#include "network.h"
#include "genome.h"

#define TURN_SPEED PI/16
#define FORWARD_SPEED 0.015
#define HEALTH_LOSS 0.05

class Creature
{
public:
    Creature();
    vector2 m_position;
    double m_rotation = PI;
    double m_size = 10.0;

    void Update();
    void Senses(const std::vector<Food>&);
    void SetDNA(std::string);
    void SetDNA(NN::Genome);
    bool IsDead();
    double GetScore();

private:
    double m_sensors[4];
    NN::Network m_nn;
    NN::Genome m_genome;
    double m_score = 0.0;
};

#endif // CREATURE_H
