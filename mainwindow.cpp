#include "crossRect.h"
#include "roadline.h"
#include "carRect.h"
#include "mainwindow.h"

#include <QHBoxLayout>
#include <QSplitter>

#include <QDebug>

#define NODE_DISTANCE 150

MainWindow::MainWindow(map<int, Road*> &roadMap, map<int, Cross*> &crossMap, vector<Road *> &roadList, vector<Cross *> &crossList, vector<Car *> &carList, QWidget *parent)
    : QWidget(parent),
      roadMap(roadMap),
      crossMap(crossMap),
      roadList(roadList),
      crossList(crossList),
      carList(carList)
{
    populateScene();
    simulator = new SimulatorAlorithm(roadList, crossList, carList);
    simulator->init();
    simulator->simulatorCore();

    simStepLength = 1;
    curStep = -1;
    blockedCrossID = -1;
    isFinished = false;
    deadLockCross = NULL;
    simStatus.append("OK");

    h1Splitter = new QSplitter;

    QSplitter *vSplitter = new QSplitter;
    vSplitter->setOrientation(Qt::Vertical);
    vSplitter->addWidget(h1Splitter);

    view = new View("");
    view->view()->setScene(scene);
    h1Splitter->addWidget(view);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(vSplitter);
    setLayout(layout);

    setWindowTitle(tr("CodeCraft-2019 Simulator-0.1.3"));
}

void MainWindow::populateScene()
{
    scene = new QGraphicsScene(this);

    int maxArrive = 0, minArrive = 0, maxOff = 0, minOff = 0;
    int roadStyle = -1;
    calcMaxMinArrOff(crossMap, maxArrive, minArrive, maxOff, minOff);

    for(auto &iter : roadMap) {
        Road* curRoad = iter.second;
        Cross* startCross = crossMap[curRoad->srcId];
        Cross* endCross = crossMap[curRoad->dstId];
        vector<vector<Car *> > carForward = curRoad->laneSame;
        vector<vector<Car *> > carBackward = curRoad->laneInvert;
        int startCrossID = startCross->crossId;
        int endCrossId = endCross->crossId;
        int numberOfLanes = curRoad->numberOfLanes;
        int roadLength = curRoad->length;
        int speed = curRoad->limitSpeed;

        int temp1X = startCross->relativePosX;
        int temp1Y = startCross->relativePosY;
        int temp2X = endCross->relativePosX;
        int temp2Y = endCross->relativePosY;
        int startX = std::min(temp1X, temp2X);
        int startY = std::min(temp1Y, temp2Y);
        int startPosX = temp1Y == temp2Y ? startX * NODE_DISTANCE + 40 : startX * NODE_DISTANCE + 20;
        int startPosY = temp1X == temp2X ? startY * NODE_DISTANCE + 40 : startY * NODE_DISTANCE + 20;
        int width = temp1Y == temp2Y ? abs(temp1X - temp2X) * NODE_DISTANCE - 40 : 3 * numberOfLanes;
        int height = temp1X == temp2X ? abs(temp1Y - temp2Y) * NODE_DISTANCE - 40 : 3 * numberOfLanes;
        int roadID = curRoad->roadId;

        int finalStartPosXForward, finalStartPosYForward, finalStartPosXBack, finalStartPosYBack;
        QString forwardCarList;
        QString backwardCarList;

        for(int i = 0; i < numberOfLanes; i++) {
            for(int j = 0; j < roadLength; j++) {
                if(carForward[i][j]) {
                    forwardCarList.append(QString::number(carForward[i][j]->id));
                    forwardCarList.append("(");
                    forwardCarList.append(QString::number(i));
                    forwardCarList.append(",");
                    forwardCarList.append(QString::number(j));
                    forwardCarList.append("), ");
                }
                if(curRoad->isDoubleSide && carBackward[i][j]) {
                    backwardCarList.append(QString::number(carBackward[i][j]->id));
                    backwardCarList.append("(");
                    backwardCarList.append(QString::number(i));
                    backwardCarList.append(",");
                    backwardCarList.append(QString::number(j));
                    backwardCarList.append("), ");
                }
            }
        }

        // 横向
        if(temp1Y == temp2Y) {
            // 开始节点在左边
            if(startX == temp1X) {
                roadStyle = 0;
                QGraphicsItem *roadItemForward = new RoadLine(roadID, width, height, startCrossID, endCrossId, numberOfLanes, roadLength, speed, forwardCarList);
                finalStartPosXForward = startPosX;
                finalStartPosYForward = startPosY + 3;
                roadItemForward->setPos(finalStartPosXForward, finalStartPosYForward);
                scene->addItem(roadItemForward);
                if(curRoad->isDoubleSide) {
                    QGraphicsItem *roadItemBackward = new RoadLine(roadID, width, height, endCrossId, startCrossID, numberOfLanes,  roadLength, speed, backwardCarList);
                    finalStartPosXBack = startPosX;
                    finalStartPosYBack = startPosY - 17;
                    roadItemBackward->setPos(finalStartPosXBack, finalStartPosYBack);
                    scene->addItem(roadItemBackward);
                }
            }
            // 开始节点在右边
            else {
                roadStyle = 1;
                QGraphicsItem *roadItemForward = new RoadLine(roadID, width, height, startCrossID, endCrossId, numberOfLanes, roadLength, speed, forwardCarList);
                finalStartPosXForward = startPosX;
                finalStartPosYForward = startPosY - 17;
                roadItemForward->setPos(finalStartPosXForward, finalStartPosYForward);
                scene->addItem(roadItemForward);
                if(curRoad->isDoubleSide) {
                    QGraphicsItem *roadItemBackward = new RoadLine(roadID, width, height, endCrossId, startCrossID, numberOfLanes, roadLength, speed, backwardCarList);
                    finalStartPosXBack = startPosX;
                    finalStartPosYBack = startPosY + 3;
                    roadItemBackward->setPos(finalStartPosXBack, finalStartPosYBack);
                    scene->addItem(roadItemBackward);
                }
            }
        }
        // 竖向
        else if(temp1X == temp2X){
            // 如果开始的节点在上边
            if(startY == temp1Y) {
                roadStyle = 2;
                QGraphicsItem *roadItemForward = new RoadLine(roadID, width, height, startCrossID, endCrossId, numberOfLanes,  roadLength, speed, forwardCarList);
                finalStartPosXForward = startPosX - 17;
                finalStartPosYForward = startPosY;
                roadItemForward->setPos(finalStartPosXForward, finalStartPosYForward);
                scene->addItem(roadItemForward);
                if(curRoad->isDoubleSide) {
                    QGraphicsItem *roadItemBackward = new RoadLine(roadID, width, height, endCrossId, startCrossID, numberOfLanes, roadLength, speed, backwardCarList);
                    finalStartPosXBack = startPosX + 3;
                    finalStartPosYBack = startPosY;
                    roadItemBackward->setPos(finalStartPosXBack, finalStartPosYBack);
                    scene->addItem(roadItemBackward);
                }
            }
            // 开始的节点在下边
            else {
                roadStyle = 3;
                QGraphicsItem *roadItemForward = new RoadLine(roadID, width, height, startCrossID, endCrossId, numberOfLanes, roadLength, speed, forwardCarList);
                finalStartPosXForward = startPosX + 3;
                finalStartPosYForward = startPosY;
                roadItemForward->setPos(finalStartPosXForward, finalStartPosYForward);
                scene->addItem(roadItemForward);
                if(curRoad->isDoubleSide) {
                    QGraphicsItem *roadItemBackward = new RoadLine(roadID, width, height, endCrossId, startCrossID, numberOfLanes, roadLength, speed, backwardCarList);
                    finalStartPosXBack = startPosX - 17;
                    finalStartPosYBack = startPosY;
                    roadItemBackward->setPos(finalStartPosXBack, finalStartPosYBack);
                    scene->addItem(roadItemBackward);
                }
            }
        }

        float carWidth = temp1Y == temp2Y ? float(width) / float(roadLength) : 3;
        float carHeight = temp1X == temp2X ? float(height) / float(roadLength) : 3;
        for(int i = 0; i < numberOfLanes; i++) {
            for(int j = 0; j < roadLength; j++) {
                if(carForward[i][j] != NULL) {
                    Car* curCar = carForward[i][j];
                    QString carInfo(QString::number(curCar->src).append("→").append(QString::number(curCar->dst)).append(" Speed:").append(QString::number(curCar->maxSpeed)));
                    QGraphicsItem *carItemForward = new CarRect(curCar->id, carWidth, carHeight, carInfo, QColor(curCar->red, curCar->green, curCar->blue, 255));
                    switch(roadStyle) {
                    case 0: carItemForward->setPos(finalStartPosXForward + (roadLength - j - 1) * carWidth, finalStartPosYForward + i * carHeight); break; //
                    case 1: carItemForward->setPos(finalStartPosXForward + j * carWidth, finalStartPosYForward + (numberOfLanes - i - 1) * carHeight); break;
                    case 2: carItemForward->setPos(finalStartPosXForward + i * carWidth, finalStartPosYForward + (roadLength - j - 1) * carHeight); break;
                    case 3: carItemForward->setPos(finalStartPosXForward + i * carWidth, finalStartPosYForward + j * carHeight); break; //
                    default:break;
                    }

                    scene->addItem(carItemForward);
                }
                if(curRoad->isDoubleSide) {
                    if(carBackward[i][j] != NULL) {
                        Car* curCar = carBackward[i][j];
                        QString carInfo(QString::number(curCar->src).append("→").append(QString::number(curCar->dst)).append(" Speed:").append(QString::number(curCar->maxSpeed)));
                        QGraphicsItem *carItemBackward = new CarRect(curCar->id, carWidth, carHeight, carInfo, QColor(curCar->red, curCar->green, curCar->blue, 255));
                        switch(roadStyle) {
                        case 0: carItemBackward->setPos(finalStartPosXBack + j * carWidth, finalStartPosYBack + (numberOfLanes - i - 1) * carHeight); break; //
                        case 1: carItemBackward->setPos(finalStartPosXBack + (roadLength - j - 1) * carWidth, finalStartPosYBack + i * carHeight); break;
                        case 2: carItemBackward->setPos(finalStartPosXBack + i * carWidth, finalStartPosYBack + j * carHeight); break;
                        case 3: carItemBackward->setPos(finalStartPosXBack + (numberOfLanes - i - 1)  * carWidth, finalStartPosYBack + (roadLength - j - 1) * carHeight); break; //
                        default:break;
                        }
                        scene->addItem(carItemBackward);
                    }
                }
            }
        }
    }

    for(auto &iter : crossMap) {
        Cross *curCross = iter.second;
        if(blockedCrossID == curCross->crossId) {
            QGraphicsItem *item = new Chip(curCross->crossId, curCross->carsOffHere, curCross->carsArrHere, maxArrive, minArrive, maxOff, minOff, 4);
            int startPosX = curCross->relativePosX;
            int startPosY = curCross->relativePosY;
            item->setPos(startPosX * NODE_DISTANCE, startPosY * NODE_DISTANCE);
            scene->addItem(item);
        }
        else {
            QGraphicsItem *item = new Chip(curCross->crossId, curCross->carsOffHere, curCross->carsArrHere, maxArrive, minArrive, maxOff, minOff, 2);
            int startPosX = curCross->relativePosX;
            int startPosY = curCross->relativePosY;
            item->setPos(startPosX * NODE_DISTANCE, startPosY * NODE_DISTANCE);
            scene->addItem(item);
        }

    }
}

void MainWindow::calcMaxMinArrOff(map<int, Cross*> &crossMap, int &maxArrive, int &minArrive, int &maxOff, int &minOff) {
    for(auto &iter : crossMap) {
        Cross *cross = iter.second;
        maxArrive = std::max(maxArrive, cross->carsArrHere);
        minArrive = std::min(minArrive, cross->carsArrHere);
        maxOff = std::max(maxOff, cross->carsOffHere);
        minOff = std::min(minOff, cross->carsOffHere);
    }
}

void MainWindow::simStep() {
    if(!isFinished) {
        for(int i = 0; i < simStepLength; i++) {
            curStep++;
            deadLockCross = simulator->simulatorStep();
            if(deadLockCross != NULL) {
                isFinished = true;
                blockedCrossID = deadLockCross->crossId;
                simStatus.clear();
                simStatus.append("DEADLOCK ID:").append(QString::number(deadLockCross->crossId)).append("  BLOCKED:").append(QString::number(deadLockCross->blocked));
                cout << "DEADLOCK" << endl;
                break;
            }
            if(simulator->isFinished()) {
                isFinished = true;
                simStatus.clear();
                simStatus.append("FINISHED");
                break;
            }
        }
        scene->clear();
        populateScene();
        view->view()->setScene(scene);
        QString titleString;
        titleString.append("[Status: ").append(simStatus).append("] ").append("Current Steps: ").append(QString::number(curStep)).append("  x").append(QString::number(simStepLength));
        setWindowTitle(titleString);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    simStep();
}

void MainWindow::keyReleaseEvent(QKeyEvent *ev) {
    if(ev->key() == Qt::Key_2) {
        if(simStepLength == 1) simStepLength = 5;
        else simStepLength += 5;
    }
    else if(ev->key() == Qt::Key_1) {
        if(simStepLength == 5) simStepLength = 1;
        else if(simStepLength >= 5) simStepLength -= 5;
    }
    else if(ev->key() == Qt::Key_3) {
        simStep();
    }
    QString titleString;
    titleString.append("[Status: ").append(simStatus).append("] ").append("Current Steps: ").append(QString::number(curStep)).append("  x").append(QString::number(simStepLength));
    setWindowTitle(titleString);
}

