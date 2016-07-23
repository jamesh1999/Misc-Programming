#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "genome.h"
#include <iostream>
#include <cmath>
#include <vector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Set up everything for the scene
    scene = new QGraphicsScene();
    ui->world->setScene(scene);
    outlinePen = QPen(Qt::black);
    outlinePen.setWidth(2);
    foodBrush = QBrush(Qt::green);
    creatureBrush = QBrush(Qt::red);

    //Create base DNA
    std::vector<NN::NeuronGene> neurons;
    for(int i = 0; i < 7; ++i)
    {
        NN::NeuronGene gene;
        gene.id = i;
        gene.type = i < 4 ? NN::SENSOR : NN::OUTPUT;
        neurons.push_back(gene);
    }

    std::vector<NN::ConnectionGene> connections;
    for(int i = 0; i < 12; ++i)
    {
        NN::ConnectionGene g;
        g.weight = 1.0;
        g.innovation = i;
        g.source = i/3;
        g.dest = 4 + (i%3);
        g.active = true;
        connections.push_back(g);
    }

    NN::Genome g;
    g.SetGenomeGenes(neurons, connections);

    NN::MutationSettings settings;
    settings.use_function_delta = true;
    settings.max_delta = 0.2;
    ga.m_mutation_settings = settings;

    ga.Start(g);

    initScene();

    //Timer set up
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    m_updateTimer->start(5);

    m_foodTimer = new QTimer(this);
    connect(m_foodTimer, SIGNAL(timeout()), this, SLOT(addFood()));
    m_foodTimer->start(80);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initScene()
{
    static int i = -1;

    if(i >= 0)
        ga.m_species[0].m_scores[i] = m_creature.GetScore();

    if(i == 9)
    {
        ga.Epoch();
        i = -1;
    }

    m_food.clear();
    m_creature = Creature();
    m_creature.SetDNA(ga.m_species[0].m_individuals[++i]);
    addFood();addFood();addFood();addFood();addFood();
}

void MainWindow::update()
{
    scene->clear();

    //Update creature
    m_creature.Senses(m_food);
    m_creature.Update();

    if(m_creature.m_position[0] > 1.1f
       || m_creature.m_position[0] < -0.1f
       || m_creature.m_position[1] > 1.1f
       || m_creature.m_position[1] < -0.1f)
        m_creature.m_size = -999.9f;


    if(m_creature.IsDead())
    {
        initScene();
    }
    else
    {
        //Check if ate food
        for(Food& f : m_food)
        {
            float xDiff = (m_creature.m_position[0] - f.m_position[0]) * ui->world->width();
            float yDiff = (m_creature.m_position[1] - f.m_position[1]) * ui->world->height();
            float sqrDist = xDiff * xDiff + yDiff * yDiff;

            float threshold = f.m_amount + m_creature.m_size;
            threshold *= 0.8; //Amount to be overlapped before count as eaten
            threshold = threshold * threshold;

            //If collided
            if(sqrDist < threshold)
            {
                m_creature.m_size = std::sqrt(f.m_amount * f.m_amount + m_creature.m_size * m_creature.m_size);
                f.m_amount = 0.0f;
            }
        }
        //Draw creature
        vector2 pos;
        pos[0] = m_creature.m_position[0] * ui->world->width() - m_creature.m_size/2;
        pos[1] = m_creature.m_position[1] * ui->world->height() - m_creature.m_size/2;
        scene->addEllipse(pos[0], pos[1], m_creature.m_size, m_creature.m_size, outlinePen, creatureBrush);

        //Draw food
        for(Food& f : m_food)
        {
            vector2 pos;
            pos[0] = f.m_position[0] * ui->world->width() - f.m_amount/2;
            pos[1] = f.m_position[1] * ui->world->height() - f.m_amount/2;
            scene->addEllipse(pos[0], pos[1], f.m_amount, f.m_amount, outlinePen, foodBrush);
        }
    }
}

void MainWindow::addFood()
{
    if(m_food.size() < FOOD_COUNT)
    {
        m_food.push_back(Food());
        return;
    }

    for(Food& f : m_food)
    {
        if(f.m_amount == 0.0f)
        {
            f = Food();
            return;
        }
    }
}

void MainWindow::on_actionRestart_triggered()
{

}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}
