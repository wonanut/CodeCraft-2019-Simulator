#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QKeyEvent>
#include "CodeCraft.h"
#include "view.h"
class QGraphicsScene;
class QSplitter;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    map<int, Road*> roadMap;
    map<int, Cross*> crossMap;
    vector<Road *> roadList;
    vector<Cross *> crossList;
    vector<Car *> carList;
    int simStepLength;
    int curStep;
    bool isFinished;
    int blockedCrossID;
    Cross *deadLockCross;
    QString simStatus;

    MainWindow(map<int, Road*> &roadMap, map<int, Cross*> &crossMap, vector<Road *> &roadList, vector<Cross *> &crossList, vector<Car *> &carList, QWidget *parent = 0);
    void update();
    void calcMaxMinArrOff(map<int, Cross*> &crossMap, int &maxArrive, int &minArrive, int &maxOff, int &minOff);
    void simStep();

protected:
    void mousePressEvent(QMouseEvent * event);
    void keyReleaseEvent(QKeyEvent * event);

private:
   void setupMatrix();
   void populateScene();

   QGraphicsScene *scene;
   QSplitter *h1Splitter;
   View *view;
   SimulatorAlorithm *simulator;

};

#endif // MAINWINDOW_H
