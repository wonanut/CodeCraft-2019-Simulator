#ifndef CODECRAFT_H
#define CODECRAFT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <random>
#include "iostream"
using namespace std;

#define STATE_WAIT 0
#define STATE_STOP 1
#define STATE_ON 2

#define STOP 0
#define LEFT 1
#define STRAIGHT 2
#define RIGHT 3

#define MAX_BLOCKED_TIME 10

class Cross;
class Car;
class Road;

// element of router table
class RouterTableElement {
public:
        Cross * dst;
        double weight;
        int port;
        RouterTableElement(Cross * pDst, double pWeight, int pPort) {
                dst = pDst;
                weight = pWeight;
                port = pPort;
        }
        RouterTableElement() {
                dst = NULL;
                weight = -1;
                port = -1;
        }
};

// element of car list
class CarSequenceElement {
public:
        Car * car;
        int carId;
        double timeStart, timeEnd;
        CarSequenceElement(Car * pCar, int pCarId, double pTimeStart, double pTimeEnd) {
                car = pCar;
                carId = pCarId;
                timeStart = pTimeStart;
                timeEnd = pTimeEnd;
        }
        string stateString() {
                string tmp;
                char sCarId[10], sTimeStart[10], sTimeEnd[10];
                sprintf(sCarId, "%d", carId);
                sprintf(sTimeStart, "%f", timeStart);
                sprintf(sTimeEnd, "%f", timeEnd);
                tmp += sTimeStart;
                tmp += "->";
                tmp += sTimeEnd;
                tmp += ":";
                tmp += sCarId;
                return tmp;
        }
};

class CarPosition {
public:
        int lane;
        int position;
        bool isSame;
        /*
        CarPosition(int p_lane, int p_position) {
                lane = p_lane;
                position = p_position;
        }
        */
};

class Cross {
public:
        // simulation
        int blocked;
        // router table : form port to dst cross need weight
        vector<RouterTableElement> routerTable;
        RouterTableElement finalRouter;

        // find path control element
        bool isFound;
        double time;

        double minOffTime; // earliest off time from current cross

        // off car list (from current cross)
        vector<Car *> offCarList;
        int currOffCar;
        // base element
        int crossId, roadId[4];
        Cross * next;
        Road * road[4];

        int relativePosX, relativePosY;
        int carsOffHere, carsArrHere;

        Cross(int * parameters) {
                crossId = parameters[0];
                roadId[0] = parameters[1];
                roadId[1] = parameters[2];
                roadId[2] = parameters[3];
                roadId[3] = parameters[4];
                next = NULL;
                road[0] = road[1] = road[2] = road[3] = NULL;

                this->relativePosX = -1;
                this->relativePosY = -1;
                this->carsArrHere = 0;
                this->carsOffHere = 0;
        }

         // link road to current cross.road[position]
        void linkRoad(int position, Road * pRoad) { road[position] = pRoad; }
        void simulationInit() { blocked = false; }
        void addCarsOffHere() { this->carsOffHere += 1; }
        void addCarsArrHere() { this->carsArrHere += 1; }
};

// base class : road
class Road {
public:
        // simulation structure (current not use)
        vector<vector<Car *> > laneSame, laneInvert; // road[0] aim to cross

        // save car time sequence (which car when use current road)
        vector<CarSequenceElement> carSameSequence; // src to dst
        vector<CarSequenceElement> carInvertSequence; // dst to src

        // base element
        int roadId, length, limitSpeed, numberOfLanes, srcId, dstId, isDoubleSide;
        Road * next;
        Cross * cross[2];
        int sumCross;
        Road(int * parameters) {
                roadId = parameters[0];
                length = parameters[1];
                limitSpeed = parameters[2];
                numberOfLanes = parameters[3];
                srcId = parameters[4];
                dstId = parameters[5];
                isDoubleSide = parameters[6];
                next = NULL;
                cross[0] = cross[1] = NULL;
                sumCross = 0;
                simulationInit();
        }

        bool linkCross(Cross * pCross);
        Cross * getNextCross(Cross * src);
        int getSumCar(double time, bool isSameSequence);
        vector<Car *> getCars(double time, bool isSameSequence);

        // return the max speed the can run
        int getSpeed(int carSpeed) { return this->limitSpeed < carSpeed ? limitSpeed : carSpeed; }

        // return the time the car may use
        double getTimeWeight(int carSpeed) { return (double)this->length / (double)getSpeed(carSpeed); }
        // return weight (not suggest to use)

        double getWeight(int carSpeed) { return getTimeWeight(carSpeed); }
        // simulation init (current not use)
        void simulationInit();
};

// base class : car
class Car {
public:
        int state;
        bool isSame; // dir
        int currRoad;
        CarPosition currPos;
        // static router path
        vector<Road *> staticPath;
        // final router path
        vector<Road *> path; // simulation use

        // for simulator ui use
        int green;
        int red;
        int blue;

        // base element
        int id, src, dst, maxSpeed, departureTime, proriority, pre;
        Car * next;
        int offTime; // simulation use
        Car(int * parameters) {
                id = parameters[0];
                src = parameters[1];
                dst = parameters[2];
                maxSpeed = parameters[3];
                departureTime = parameters[4];
                proriority = parameters[5];
                pre = parameters[6];
                next = NULL;

                if(proriority) {
                    green = 0;
                    red = rand() % 200 + 50;
                    blue = 0;
                }
                else {
                    green = rand() % 200 + 50;
                    red = 0;
                    blue = 0;
                }
        }

        void printPath();
        void simulationInit();
        int getNextTurn();
};

class RoadList {
public:
        Road * head;
        Road * tail;
        RoadList() {
                head = tail = NULL;
        }

        Road *addRoad(int * parameters);
        void printList();
        Road *findRoadById(int id);
        void printLinks();
};

class CrossList {
public:
        Cross * head;
        Cross * tail;
        CrossList() {
                head = tail = NULL;
        }

        Cross * addCross(int * parameters);
        void printList();
        void linkRoadList(map<int, Road*> &roadMap);
        void printLinks();
        Cross * find(int dstId);
};

class CarList {
public:
        Car * head;
        Car * tail;
        CarList() {
                head = tail = NULL;
        }

        Car * addCar(int * parameters);
        Car * find(int id);
        void printList();
};


class SimulatorAlorithm {
public:
    vector<Road *> roadVector;
    vector<Cross *> crossVector;

    vector<Car *> onRoadVector;
    vector<Car *> inGarageVector;
    int systemTime = 0;
    vector<Car *> prCars; // prorioty = 1
    vector<Car *> orCars; // prorioty = 0

    SimulatorAlorithm(vector<Road *> roadList, vector<Cross *> crossList, vector<Car *> carList) {
        inGarageVector.assign(carList.begin(), carList.end());
        roadVector.assign(roadList.begin(), roadList.end());
        crossVector.assign(crossList.begin(), crossList.end());
    }

    Car * getCarInLane(Road * road, CarPosition carPos);
    Car * getFirstCarInLane(Road * road, int lane, bool isSame);
    Car * getFirstCar(Road * road, bool isSame);
    Car * getLastCar(Road * road, bool isSame);
    CarPosition getLastPosition(Road * road, bool isSame, bool & lastCarStop);
    void init();
    void printRoad(Road * road, bool isSame);
    bool runCarInRoad(Car * car, Road * road, bool isSame);
    bool forwardCarInLane(Road * road, bool isSame, int lane, int state, int & numberOfCars);
    bool forwardCarOn(Road * road, bool isSame, int & sumCar);
    bool forwardCarWait(Road * fromRoad, Road * toRoad, Cross * midCross, Car * car);
    void forwardRoad();
    bool forwardCross(Cross * cross);
    bool startCar(Car * car);
    Cross * simulatorCore();
    Cross * simulatorStep();
    bool isFinished();
};

bool sortByRoadId(Road * road1, Road * road2);
bool sortByCrossId(Cross * cross1, Cross * cross2);
bool sortByCarTime(Car * car1, Car * car2);
bool sortByCarId(Car * car1, Car * car2);

bool bigger(double x, double y);
void initFindPathFinal(CrossList & crossList);
double findPathFinal(Cross * src, Cross * dst, Cross * pre, double time, Car * car, int numberOfNodePass);
void addFinalPathToCar(Car * car, map<int, Cross*> &crossMap);

void outputPathFile(string fileName, CarList & carList);
void loadPreFile(string fileName, map<int, Car*> &carMap, map<int, Road*> &roadMap);
void setPreCarPath(vector<int> &parameters, map<int, Car*> &carMap, map<int, Road*> &roadMap);
void loadCarFile(string fileName, CarList & carList, map<int, Car*> &carMap, vector<Car *> &carList_pVector);
void loadCrossFile(string fileName, CrossList & crossList, map<int, Cross*> &crossMap, vector<Cross *> &crossList_pVector);
void loadRoadFile(string fileName, RoadList & roadList, map<int, Road*> &roadMap, vector<Road *> &roadList_pVector);

void correctCrossDirection(map<int, Cross*> &crossMap, map<int, Road*> &roadMap);
void DFS(map<int, Cross*> &crossMap, map<int, Road*> &roadMap, map<int, bool> &visitDone, int crossID, int direction, int preCrossID, int posX, int posY, int &minX, int &minY);
void calcOffArrCross(map<int, Car*> &carMap, map<int, Cross*> &crossMap);
void printPosition(map<int, Cross*> &crossMap);
void printLink(map<int, Cross*> &crossMap);

#endif // CODECRAFT_H
