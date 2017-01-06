#include "global.h"
#include "genome.h"
#include <sstream>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>

using namespace NN;

std::string binaryToHexString(uint8_t* bytes, unsigned size)
{
    const char HEX_CHARS[] = "0123456789abcdef";

    std::string ret;
    ret.resize(size * 2);

    for(unsigned i = 0; i < size; ++i)
    {
        ret[2 * i] = HEX_CHARS[(bytes[i] >> 4) & 0x0f];
        ret[2 * i + 1] = HEX_CHARS[bytes[i] & 0x0f];
    }

    return ret;
}

void concatHexToSS(std::stringstream& ss, uint8_t* bytes, unsigned size)
{
    ss << binaryToHexString(bytes, size);
}

bool hexStringToBinary(const std::string& str, uint8_t* bytes, unsigned size)
{
    if(size != str.size() / 2)
        return false;

    for(int i = 0; i < str.size(); i += 2)
    {
        uint8_t nibble0 = str[i] <= '9' ?
                            str[i] - '0'
                            : str[i] - 'W';
        uint8_t nibble1 = str[i + 1] <= '9' ?
                            str[i + 1] - '0'
                            : str[i + 1] - 'W';

        bytes[i / 2] = (nibble0 << 4) + nibble1;
    }

    return true;
}

Genome::Genome(const Genome& other)
{
    m_connection_genes = other.m_connection_genes;
    m_neuron_genes = other.m_neuron_genes;
    SortGenes();
}

//Genome hexadecimal representation:
//0-4 Neuron count
//4-8 Connection count
//8-... Neuron data
//... Connection data

//Neuron data format:
//0-8 Bias
//8-12 ID
//12-16 NeuronType (Last 2 bits represent type)

//Connection data format:
//0-8 Weight
//8-12 Innovation Number
//12-16 Source neuron ID
//16-20 Destination neuron ID
//20-21 Gene active
//[21-24 Padding]

//Sets the genome from a given hexadecimal string
void Genome::SetGenomeHex(std::string new_genome)
{
    //Buffer to store the binary data
    uint8_t* bytes = new uint8_t[new_genome.size() / 2];
    hexStringToBinary(new_genome, bytes, new_genome.size() / 2);

    //Get number of neurons/connection genes in network
    uint32_t neuron_count = *reinterpret_cast<uint32_t*>(bytes);
    uint32_t connection_count = *reinterpret_cast<uint32_t*>(bytes + 4);

    //Add genes to respective vectors
    NeuronGene* pN = reinterpret_cast<NeuronGene*>(bytes + 8);
    m_neuron_genes.clear();
    for(int i = 0; i < neuron_count; ++i)
        m_neuron_genes.push_back(pN[i]);

    ConnectionGene* pC = reinterpret_cast<ConnectionGene*>(bytes + 8 + neuron_count * sizeof(NeuronGene));
    m_connection_genes.clear();
    for(int i = 0; i < connection_count; ++i)
        m_connection_genes.push_back(pC[i]);

    //Clean up
    delete[] bytes;

    SortGenes();
}

//Gets a hexadecimal representation of the current genome
std::string Genome::GetGenomeHex()
{
    std::stringstream ss;

    uint32_t neuron_count = m_neuron_genes.size();
    concatHexToSS(ss, reinterpret_cast<uint8_t*>(&neuron_count), 4);
    uint32_t conn_count = m_connection_genes.size();
    concatHexToSS(ss, reinterpret_cast<uint8_t*>(&conn_count), 4);

    //Add neuron data
    NeuronGene* pNeuron_genes = new NeuronGene[m_neuron_genes.size()];
    for(int i = 0; i < m_neuron_genes.size(); ++i)
        pNeuron_genes[i] = m_neuron_genes[i];
    concatHexToSS(ss, reinterpret_cast<uint8_t*>(pNeuron_genes),
                  sizeof(NeuronGene) * m_neuron_genes.size());

    //Add connection data
    ConnectionGene* pConnection_genes = new ConnectionGene[m_connection_genes.size()];
    for(int i = 0; i < m_connection_genes.size(); ++i)
        pConnection_genes[i] = m_connection_genes[i];
    concatHexToSS(ss, reinterpret_cast<uint8_t*>(pConnection_genes),
                  sizeof(ConnectionGene) * m_connection_genes.size());

    return ss.str();
}

//Sets the genome from the given genes
void Genome::SetGenomeGenes(std::vector<NeuronGene> neurons, std::vector<ConnectionGene> connections)
{
    m_neuron_genes = neurons;
    m_connection_genes = connections;
    SortGenes();
}

//Creates the network at pNetwork with the given genes
void Genome::CreateNetwork(Network* pNetwork)
{
    for(auto g : m_neuron_genes)
        pNetwork->AddNeuron(g.type, g.id, g.bias);
    for(auto g : m_connection_genes)
        pNetwork->MakeConnection(g.source, g.dest, g.weight);
}

//Mate two genomes
//The genome this is called on it the MOTHER
Genome Genome::Mate(const Genome &other, const MatingSettings& settings)
{
    enum Parent {MOTHER, FATHER};
    Parent best = m_score > other.m_score ? MOTHER : FATHER;

    std::vector<ConnectionGene> new_connections;

    //Iterators to go through both genomes
    auto mother_gene = m_connection_genes.begin();
    auto father_gene = other.m_connection_genes.begin();

    ConnectionGene to_add;
    bool should_add;
    while(mother_gene != m_connection_genes.end()
          || father_gene != other.m_connection_genes.end())
    {
        //Mother excess genes
        if(father_gene == other.m_connection_genes.end())
        {
            to_add = *mother_gene;
            ++mother_gene;
            should_add = best == MOTHER;
        }
        //Father excess genes
        else if(mother_gene == m_connection_genes.end())
        {
            to_add = *father_gene;
            ++father_gene;
            should_add = best == FATHER;
        }
        //Disjoint handling
        //Mum's is different
        else if(mother_gene->innovation < father_gene->innovation)
        {
            to_add = *mother_gene;
            ++mother_gene;
            should_add = best == MOTHER;
        }
        //Dad's is different
        else if(mother_gene->innovation > father_gene->innovation)
        {
            to_add = *father_gene;
            ++father_gene;
            should_add = best == FATHER;
        }
        //Genes match
        else
        {
            //Genes match
            //=> select random weight but use better genome's io neurons
            to_add = best == MOTHER ? *mother_gene : *father_gene;

            if(settings.average_parents)
                to_add.weight = (father_gene->weight + mother_gene->weight) / 2;
            else
                to_add.weight = rand() % 2 == 0 ? father_gene->weight : mother_gene->weight;

            ++mother_gene;
            ++father_gene;
            should_add = true;
        }

        //Override should_add
        if(settings.all_genes)
        {
            //Check if this connection already exists (possible evolved separately)
            bool already_exists = false;
            for(const ConnectionGene& g : new_connections)
                if(g.dest == to_add.dest
                   && g.source == to_add.source)
                {
                    already_exists = true;
                    break;
                }

            should_add = !already_exists;
        }

        //Add the selected gene to the new genome
        if(should_add)
            new_connections.push_back(to_add);
    }

    //Add required neurons at random
    std::vector<NeuronGene> new_neurons;

    for(auto g : new_connections)
    {
        bool contains_i = false;
        bool contains_o = false;
        for(const NeuronGene& n : new_neurons)
        {
            if(n.id == g.source)
                contains_i = true;
            if(n.id == g.dest)
                contains_o = true;
        }

        if(!contains_i)
        {
            const NeuronGene* father = nullptr;
            const NeuronGene* mother = nullptr;

            for(const NeuronGene& n : other.m_neuron_genes)
                if(n.id == g.source)
                {
                    father = &n;
                    break;
                }

            for(const NeuronGene& n : m_neuron_genes)
                if(n.id == g.source)
                {
                    mother = &n;
                    break;
                }

            //One genome must contain the neuron
            assert(mother!=nullptr || father!=nullptr);

            if(mother != nullptr && father != nullptr)
            {
                NeuronGene new_neuron = *mother;
                if(settings.average_parents)
                    new_neuron.bias = (father->bias + mother->bias) / 2;
                else
                    new_neuron.bias = rand() % 2 == 0 ? father->bias : mother->bias;
                new_neurons.push_back(new_neuron);
            }
            else if(mother != nullptr)
                new_neurons.push_back(*mother);
            else
                new_neurons.push_back(*father);
        }

        if(!contains_o)
        {
            const NeuronGene* father = nullptr;
            const NeuronGene* mother = nullptr;

            for(const NeuronGene& n : other.m_neuron_genes)
                if(n.id == g.dest)
                {
                    father = &n;
                    break;
                }

            for(const NeuronGene& n : m_neuron_genes)
                if(n.id == g.dest)
                {
                    mother = &n;
                    break;
                }

            //One genome must contain the neuron
            assert(mother!=nullptr || father!=nullptr);

            if(mother != nullptr && father != nullptr)
            {
                NeuronGene new_neuron = *mother;
                if(settings.average_parents)
                    new_neuron.bias = (father->bias + mother->bias) / 2;
                else
                    new_neuron.bias = rand() % 2 == 0 ? father->bias : mother->bias;
                new_neurons.push_back(new_neuron);
            }
            else if(mother != nullptr)
                new_neurons.push_back(*mother);
            else
                new_neurons.push_back(*father);
        }
    }

    Genome n_genome;
    n_genome.SetGenomeGenes(new_neurons, new_connections);
    return n_genome;
}

//Mate two genomes with default settings
Genome Genome::Mate(const Genome& other)
{
    MatingSettings s;
    return Mate(other, s);
}
Genome Genome::operator+(const Genome &other)
{
    return Mate(other);
}

//Returns a random double in the range 0.0-1.0
double rand01()
{
    return static_cast<double>(rand()) / RAND_MAX;
}

//Returns the delta to modify weight/bias by with the given settings
double getDelta(const MutationSettings& settings)
{
    if(settings.use_function_delta)
        return std::tanh(settings.function_a * rand01() + settings.function_b) * settings.max_delta;
    else
        return rand() % 2 == 0 ? settings.max_delta : -settings.max_delta;
}

void Genome::Mutate(const MutationSettings& settings)
{
    //Modify weight / Add neurons
    std::vector<ConnectionGene> added_genes;
    for(auto& gene : m_connection_genes)
    {
        if(settings.enable_structural_changes
           && rand01() < settings.add_neuron_chance)
        {
            //Disable the broken connection
            gene.active = false;

            //Create the connections
            ConnectionGene new_gene;
            new_gene.source = gene.source;
            new_gene.dest = N_ID;
            new_gene.innovation = INNOVATION++;

            added_genes.push_back(new_gene);

            new_gene.innovation = INNOVATION++;
            new_gene.source = N_ID;
            new_gene.dest = gene.dest;
            new_gene.weight = gene.weight;

            added_genes.push_back(new_gene);

            //Create the new neuron gene
            NeuronGene new_neuron;
            new_neuron.id = N_ID++;
            m_neuron_genes.push_back(new_neuron);
        }

        else if(rand01() < settings.modify_weight_chance)
            gene.weight += getDelta(settings);
    }

    //Add new genes
    for(auto g : added_genes)
        m_connection_genes.push_back(g);

    //Modify bias / Add connections
    for(auto& gene : m_neuron_genes)
    {
        if(settings.enable_modify_bias
           && rand01() < settings.modify_bias_chance)
            gene.bias += getDelta(settings);

        if(settings.enable_structural_changes
           && gene.type != OUTPUT
           && rand01() < settings.add_connection_chance)
        {
            ConnectionGene new_gene;
            new_gene.innovation = INNOVATION++;
            new_gene.source = gene.id; //New connection connects to current neuron

            //Get all valid destination gene IDs
            std::vector<int> valid;
            for(int i = 0; i < m_neuron_genes.size(); ++i)
            {
                //Destination can't be sensor or the source
                if(m_neuron_genes[i].type == SENSOR
                   || m_neuron_genes[i].id == new_gene.source)
                    continue;

                //Don't connect  genes that are already connected
                bool already_connected = false;
                for(const ConnectionGene& test : m_connection_genes)
                    if(test.source == new_gene.source
                       && test.dest == m_neuron_genes[i].id)
                    {
                        already_connected = true;
                        break;
                    }

                if(already_connected) continue;

                valid.push_back(m_neuron_genes[i].id);
            }

            //Can't add connection
            if(valid.size() == 0)
                continue;

            //Choose a random destination
            new_gene.dest = valid[rand() % valid.size()];

            //Add the gene
            m_connection_genes.push_back(new_gene);
        }
    }

    if(settings.auto_trim)
        Trim();
}

//Removes all inactive genes
void Genome::Trim()
{
    std::vector<ConnectionGene> new_genes;
    for(auto g : m_connection_genes)
        if(g.active)
            new_genes.push_back(g);

    m_connection_genes = new_genes;
}

//Set the fitness of this genome (will be handled by the GeneticAlgorithm class automatically)
void Genome::SetScore(double score)
{
    m_score = score;
}

bool compareNeuron(NeuronGene a, NeuronGene b)
{
    return a.id < b.id;
}

bool compareConnection(ConnectionGene a, ConnectionGene b)
{
    return a.innovation < b.innovation;
}

//Orders neurons by ID and connections by innovation
void Genome::SortGenes()
{
    std::sort(m_neuron_genes.begin(), m_neuron_genes.end(), compareNeuron);
    std::sort(m_connection_genes.begin(), m_connection_genes.end(), compareConnection);
}

//Measure how incompatible two genomes are
double Genome::GetIncompatibility(const Genome& other, const CompatibilitySettings& settings)
{
    //Genomes shouldn't be empty
    assert(m_connection_genes.size() > 0 && other.m_connection_genes.size() > 0);

    //Calculate connection differences

    unsigned disjoint_cnt = 0;
    unsigned excess_cnt = 0;
    unsigned connection_match_cnt = 0;
    double weight_diff = 0.0;

    //'Mother' is this genome, 'father' is other
    //Iterators to go through both genomes
    auto mother_gene = m_connection_genes.begin();
    auto father_gene = other.m_connection_genes.begin();

    while(mother_gene != m_connection_genes.end()
          || father_gene != other.m_connection_genes.end())
    {
        //Excess father genes
        if(mother_gene == m_connection_genes.end())
        {
            ++excess_cnt;
            ++father_gene;
        }

        //Excess mother genes
        else if(father_gene == other.m_connection_genes.end())
        {
            ++excess_cnt;
            ++mother_gene;
        }

        //Disjoint genes
        else if(mother_gene->innovation < father_gene->innovation)
        {
            ++disjoint_cnt;
            ++mother_gene;
        }
        else if(mother_gene->innovation > father_gene->innovation)
        {
            ++disjoint_cnt;
            ++father_gene;
        }

        //Genes match => compute weight difference
        else
        {
            weight_diff += std::fabs(father_gene->weight - mother_gene->weight);
            ++connection_match_cnt;
            ++mother_gene;
            ++father_gene;
        }
    }
    //Calculate neuron differences

    unsigned neuron_match_cnt = 0;
    double bias_diff = 0.0;

    //Iterators to go through both parent's neurons
    auto mother_neuron = m_neuron_genes.begin();
    auto father_neuron = other.m_neuron_genes.begin();

    while(mother_neuron != m_neuron_genes.end()
          && father_neuron != other.m_neuron_genes.end())
    {
        //Neurons don't match
        if(father_neuron->id < mother_neuron->id)
            ++father_neuron;
        else if(father_neuron->id > mother_neuron->id)
            ++mother_neuron;

        //Neurons match => compute bias difference
        else
        {
            bias_diff += std::fabs(father_neuron->bias - mother_neuron->bias);
            ++neuron_match_cnt;
            ++father_neuron;
            ++mother_neuron;
        }
    }

    double disjoint_normalized = disjoint_cnt;
    double excess_normalized = excess_cnt;
    if(settings.normalize)
    {
        double average_size = (m_connection_genes.size() + other.m_connection_genes.size()) / 2.0;
        disjoint_normalized /= average_size;
        excess_normalized /= average_size;

        if(connection_match_cnt)
            weight_diff /= connection_match_cnt;
        if(neuron_match_cnt)
            bias_diff /= neuron_match_cnt;
    }

    return settings.disjointWeight * disjoint_normalized
            + settings.excessWeight * excess_normalized
            + settings.connectionWeightWeight * weight_diff
            + settings.neuronBiasWeight * bias_diff;
}

double Genome::GetIncompatibility(const Genome& other)
{
    CompatibilitySettings settings;
    return GetIncompatibility(other, settings);
}

double Genome::operator^(const Genome& other)
{
    return GetIncompatibility(other);
}

Genome Genome::GetBase(int sensors, int outputs)
{
    Genome g;

    //Add neurons
    NeuronGene n;
    n.type = SENSOR;
    for(int i = 0; i < sensors; ++i)
    {
        n.id = N_ID++;
        g.m_neuron_genes.push_back(n);
    }
    n.type = OUTPUT;
    for(int i = 0; i < outputs; ++i)
    {
        n.id = N_ID++;
        g.m_neuron_genes.push_back(n);
    }

    //Add connections
    ConnectionGene c;
    for(int i = 0; i < sensors * outputs; ++i)
    {
        c.source = i % sensors;
        c.dest = sensors + (i / sensors);
        c.innovation = INNOVATION++;
        g.m_connection_genes.push_back(c);
    }

    return g;
}

double Genome::GetScore()
{
    return m_score;
}
