#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include "global.h"

namespace NN
{
    enum NeuronType {SENSOR, OUTPUT, HIDDEN};

    class Neuron
    {
    public:
        NeuronType m_type;
        unsigned m_id;
        double m_output;
        double m_bias;
        double m_sense;

        std::vector<double> m_connection_weights;
        std::vector<int> m_connection_neurons;

        void Update(std::vector<Neuron>::iterator);
    };
}

#endif // NEURON_H
