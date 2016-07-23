#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QPen>
#include <QBrush>
#include <QTimer>

#include "global.h"
#include "food.h"
#include "creature.h"
#include "geneticalgorithm.h"

#define FOOD_COUNT 20

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    Creature m_creature;
    std::vector<Food> m_food;
    QPen outlinePen;
    QBrush foodBrush;
    QBrush creatureBrush;
    QTimer *m_updateTimer;
    QTimer *m_foodTimer;
    GeneticAlgorithm ga;

    void initScene();

public slots:
    void update();
    void addFood();
private slots:
    void on_actionRestart_triggered();
    void on_actionExit_triggered();
};

#endif // MAINWINDOW_H
