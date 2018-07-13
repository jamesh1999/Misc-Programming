#include "neuron.h"
#include <cmath>

using namespace NN;

void Neuron::Update(std::vector<Neuron>::iterator m_network)
{
    double input = 0.0;
    if(m_type != SENSOR)
        for(int i = 0; i < m_connection_neurons.size(); ++i)
            input += (m_network+m_connection_neurons[i])->m_output
                    * m_connection_weights[i];


    m_output = 1 / (1 + std::pow(E, -(m_type == SENSOR ? m_sense / 500 : input) - m_bias));
}
