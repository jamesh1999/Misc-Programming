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

    NN::MutationSettings settings;
    settings.use_function_delta = true;
    settings.max_delta = 1.0;
    ga.m_mutation_settings = settings;

    ga.Start(NN::Genome::GetBase(4,3));

    initScene();

    //Timer set up
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    m_updateTimer->start(5);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initScene()
{
    static int species = -1;

    if(species >= 0)
        for(int i = 0; i < m_creature.size(); ++i)
            ga.m_species[species].SetScore(i, m_creature[i].GetScore());

    species += 1;

    if(species == ga.m_species.size())
    {
        species = 0;
        ga.Epoch();
    }

    m_food.clear();
    m_creature.clear();
    for(int i = 0; i < ga.m_species[species].GetSize(); ++i)
    {
        m_creature.push_back(Creature());
        m_creature[m_creature.size()-1].SetDNA(ga.m_species[species].GetIndividual(i));
    }
    addFood();
}

void MainWindow::update()
{
    scene->clear();

    //Update creature
    bool all_dead = true;
    for(Creature& c : m_creature)
    {
        if(c.IsDead()) continue;
        all_dead = false;

        c.Senses(m_food);
        c.Update();

        if(c.m_position[0] > 1.1f
           || c.m_position[0] < -0.1f
           || c.m_position[1] > 1.1f
           || c.m_position[1] < -0.1f)

            c.m_size = -999.9f;
    }

    if(all_dead)
        initScene();
    else
    {
        for(Creature& c : m_creature)
        {
            if(c.IsDead()) continue;

            //Check if ate food
            for(Food& f : m_food)
            {
                double xDiff = (c.m_position[0] - f.m_position[0]) * ui->world->width();
                double yDiff = (c.m_position[1] - f.m_position[1]) * ui->world->height();
                double sqrDist = xDiff * xDiff + yDiff * yDiff;

                double threshold = f.m_amount + c.m_size;
                threshold *= 0.8; //Amount to be overlapped before count as eaten
                threshold = threshold * threshold;

                //If collided
                if(sqrDist < threshold)
                {
                    c.m_size = std::sqrt(f.m_amount * f.m_amount + c.m_size * c.m_size);
                    f.m_amount = 0.0f;
                }
            }
            //Draw creature
            vector2 pos;
            pos[0] = c.m_position[0] * ui->world->width() - c.m_size/2;
            pos[1] = c.m_position[1] * ui->world->height() - c.m_size/2;
            scene->addEllipse(pos[0], pos[1], c.m_size, c.m_size, outlinePen, creatureBrush);
        }

        addFood();

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
    while(m_food.size() < FOOD_COUNT)
        m_food.push_back(Food());

    for(Food& f : m_food)
        if(f.m_amount == 0.0f)
            f = Food();
}

void MainWindow::on_actionRestart_triggered()
{

}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}
