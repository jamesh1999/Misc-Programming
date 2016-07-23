#include "network.h"
#include <algorithm>
#include <cassert>

using namespace NN;

//Clears the network of all neurons
void Network::ClearNetwork()
{
    m_network.clear();
}

//Adds a neuron to the network
void Network::AddNeuron(NeuronType type, int id, double bias)
{
    m_network.push_back(Neuron());
    (m_network.end() - 1)->m_type = type;
    (m_network.end() - 1)->m_id = id;
    (m_network.end() - 1)->m_bias = bias;
}

//Makes a connection between two neurons
void Network::MakeConnection(int source, int dest, double weight)
{
    //Get indexes on neurons involved
    int sourceIndex = -1;
    int destIndex = -1;
    for(int i = 0; i < m_network.size(); ++i)
    {
        if(m_network[i].m_id == source)
            sourceIndex = i;
        if(m_network[i].m_id == dest)
            destIndex = i;
    }

    //Should be valid neurons
    assert(sourceIndex != -1);
    assert(destIndex != -1);

    //Add connection to neuron in question
    m_network[destIndex].m_connection_weights.push_back(weight);
    m_network[destIndex].m_connection_neurons.push_back(sourceIndex);
}

//Evaluates the network
//Will evaluate multiple times to account for reverse connections
void Network::Evaluate()
{
    for(int i = 0; i < 10; ++i)
        for(auto& n : m_network)
            n.Update(m_network.begin());
}

//Clears the network values for next evaluation
void Network::Reset()
{
    for(auto& n: m_network)
        n.m_output = 0.0;
}

//Optimises the structure so that fewer evaluations are required
void Network::OptimiseNetwork()
{
    std::vector<Neuron> new_network;
    std::vector<int> map; //Map of changes so connections can be fixed
    map.resize(m_network.size(), -1);

    //Add SENSORS first
    for(int i = 0; i < m_network.size(); ++i)
        if(m_network[i].m_type == SENSOR)
        {
            map[i] = new_network.size();
            new_network.push_back(m_network[i]);
        }

    //Now mark all transferred nodes as SENSOR to ignore them
    //Each time add the neuron that either has all inputs already or greatest number of inputs
    while(new_network.size() < m_network.size())
    {
        //Add neuron with least inputs not already added
        int min = INT_MAX;
        int cur;
        for(auto& n : m_network)
        {
            //Skip added neurons
            if(n.m_type == SENSOR)
                continue;

            cur = 0;
            for(int c : n.m_connection_neurons)
                if(map[c] == -1)
                    ++cur;

            if(cur < min)
                min = cur;
        }

        for(int i = 0; i < m_network.size(); ++i)
        {
            //Skip added neurons
            if(m_network[i].m_type == SENSOR)
                continue;

            cur = 0;
            for(auto c : m_network[i].m_connection_neurons)
                if(map[c] == -1)
                    ++cur;

            if(cur == min)
            {
                map[i] = new_network.size();
                new_network.push_back(m_network[i]);
                m_network[i].m_type = SENSOR;
            }
        }
    }

    //Swap with new network
    m_network = new_network;

    //Fix connections between neurons
    for(auto& n : m_network)
        for(auto& c: n.m_connection_neurons)
            c = map[c];
}

//Comparison function to sort neurons by ID (ascending order)
bool compareNeurons(Neuron *a, Neuron *b)
{
    return a->m_id < b->m_id;
}

//Set the sensor neurons to their desired values
void Network::SetSensors(std::vector<double> inputs)
{
    std::vector<Neuron*> sensors;
    for(int i = 0; i < m_network.size(); ++i)
        if(m_network[i].m_type == SENSOR)
            sensors.push_back(&m_network[i]);

    //Sort by ID ascending order
    std::sort(sensors.begin(), sensors.end(), compareNeurons);

    for(int i = 0; i < sensors.size(); ++i)
        sensors[i]->m_sense = inputs[i];
}

//Get the values of the output neurons
std::vector<double> Network::GetOutputs()
{
    std::vector<Neuron*> outputs;
    for(int i = 0; i < m_network.size(); ++i)
        if(m_network[i].m_type == OUTPUT)
            outputs.push_back(&m_network[i]);

    //Sort by ID ascending order
    std::sort(outputs.begin(), outputs.end(), compareNeurons);

    std::vector<double> result;
    for(int i = 0; i < outputs.size(); ++i)
        result.push_back(outputs[i]->m_output);

    return result;
}
