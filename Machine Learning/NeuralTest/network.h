#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include "neuron.h"

namespace NN
{
    class Network
    {
    public:
        void ClearNetwork();

        void AddNeuron(NeuronType, int, double);
        void MakeConnection(int, int, double);
        void Evaluate();
        void Reset();
        void OptimiseNetwork();

        void SetSensors(std::vector<double>);
        std::vector<double> GetOutputs();

    private:
        std::vector<Neuron> m_network;
    };
}

#endif // NETWORK_H
