#include "creature.h"
#include <cmath>
//#include <iostream>

Creature::Creature()
{
    //Give the creature a random position
    m_position[0] = static_cast<double>(rand()) / RAND_MAX;
    m_position[1] = static_cast<double>(rand()) / RAND_MAX;
}

void Creature::Update()
{
    //Update network
    std::vector<double> sensors = {m_sensors[0], m_sensors[1],
                                   m_sensors[2], m_sensors[3]};
    m_nn.Reset();
    m_nn.SetSensors(sensors);
    m_nn.Evaluate();
    auto outputs = m_nn.GetOutputs();

    //Left turn
    if(outputs[0] > outputs[1])
        m_rotation -= TURN_SPEED;

    //Right turn
    else if(outputs[1] > outputs[0])
        m_rotation += TURN_SPEED;

    //Clamp m_rotation
    if(m_rotation < 0.0)
        m_rotation += 2 * PI;
    else if(m_rotation > 2 * PI)
        m_rotation -= 2 * PI;

    //Forward movement
    if(outputs[2] > 0.0)
    {
        m_position[0] += std::cos(m_rotation) * FORWARD_SPEED;
        m_position[1] += std::sin(m_rotation) * FORWARD_SPEED;
    }

    //std::cout << outputs[0] << "," << outputs[1] << "," <<outputs[2] << std::endl;

    m_score += 1;

    //Lose some health
    m_size -= HEALTH_LOSS;
}

//Calculate sensor values based on food positions/sizes
void Creature::Senses(const std::vector<Food> &food_sources)
{
    //Calculate sensor positions based on creature pos/rot
    //Distance from creature to sensor is 0.04
    vector2 sensor_positions[4];
    for(int i = 0; i < 4; ++i)
    {
        sensor_positions[i][0] = m_position[0] + std::cos(m_rotation + PI / 2 * i) * 0.04;
        sensor_positions[i][1] = m_position[1] + std::sin(m_rotation + PI / 2 * i) * 0.04;

        m_sensors[i] = 0.0;
    }

    //Sensor value is sum of food amounts over distance squared
    for(const Food& f : food_sources)
        for(int i = 0; i < 4; ++i)
        {
            double xDiff = sensor_positions[i][0] - f.m_position[0];
            double yDiff = sensor_positions[i][1] - f.m_position[1];
            double sqrDist = xDiff * xDiff + yDiff * yDiff;
            m_sensors[i] += f.m_amount / sqrDist;
        }
}

//Set the genome to be used for the neural network
void Creature::SetDNA(std::string genome)
{
    m_genome.SetGenomeHex(genome);
    m_genome.CreateNetwork(&m_nn);
    m_nn.OptimiseNetwork();
}
void Creature::SetDNA(NN::Genome genome)
{
    m_genome = genome;
    m_genome.CreateNetwork(&m_nn);
    m_nn.OptimiseNetwork();
}

//Has this creature died?
bool Creature::IsDead()
{
    return m_size < 0.0;
}

//Returns the current score
double Creature::GetScore()
{
    return m_score;
}
