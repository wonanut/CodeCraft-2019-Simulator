#include "codecraft.h"

/***************************************functions of class Road***************************************/

bool Road::linkCross(Cross * pCross) { // link road with cross
        if (sumCross >= 2) return false;
        else {
                cross[sumCross++] = pCross;
        }
}

Cross * Road::getNextCross(Cross * src) { // get the next cross by given the current cross
        if (isDoubleSide == 1) { // double side
                if (src->crossId == cross[0]->crossId) { // cross[0] is current cross so next cross is cross[1]
                        return cross[1];
                }
                else return cross[0];
        }
        else { // only one side
                if (cross[0]->crossId == srcId) { // cross[0] is source
                        if (src->crossId == cross[0]->crossId) { // current cross is cross[0] so next cross is cross[1] else no next cross
                                return cross[1];
                        }
                }
                else { // cross[1] is source
                        if (src->crossId == cross[1]->crossId) { // current cross is cross[1] so next cross is cross[1] else no next cross
                                return cross[0];
                        }
                }
                return NULL;
        }
}

// return the number of the car at current time
int Road::getSumCar(double time, bool isSameSequence) {
        int sumCar = 0;
        if (isSameSequence) {
                for (unsigned int i = 0; i < carSameSequence.size(); i++) {
                        if (time >= carSameSequence[i].timeStart && time <= carSameSequence[i].timeEnd) {
                                sumCar++;
                        }
                }
        }
        else {
                for (unsigned int i = 0; i < carInvertSequence.size(); i++) {
                        if (time >= carInvertSequence[i].timeStart && time <= carInvertSequence[i].timeEnd) {
                                sumCar++;
                        }
                }
        }
        return sumCar;
}

// return the car pointer list at current time
vector<Car *> Road::getCars(double time, bool isSameSequence) {
        vector<Car *> car_pList;
        if (isSameSequence) {
                for (unsigned int i = 0; i < carSameSequence.size(); i++) {
                        if (time >= carSameSequence[i].timeStart && time <= carSameSequence[i].timeEnd) {
                                car_pList.push_back(carSameSequence[i].car);
                        }
                }
        }
        else {
                for (unsigned int i = 0; i < carInvertSequence.size(); i++) {
                        if (time >= carInvertSequence[i].timeStart && time <= carInvertSequence[i].timeEnd) {
                                car_pList.push_back(carInvertSequence[i].car);
                        }
                }
        }
        return car_pList;
}

void Road::simulationInit() {
        laneSame.resize(numberOfLanes);
        for (unsigned int i = 0; i < laneSame.size(); i++) {
                laneSame[i].resize(length);
        }
        if (isDoubleSide == 1) {
                laneInvert.resize(numberOfLanes);
                for (unsigned int i = 0; i < laneInvert.size(); i++) {
                        laneInvert[i].resize(length);
                }
        }
}


/***************************************functions of class Car***************************************/

void Car::printPath() {
        cout << "car_" << id << ": ( ";
        for (int i = 0; i < path.size(); i++) {
                cout << path[i]->roadId;
                if (i != path.size() - 1) cout << ",";
                cout << " ";
        }
        cout << ")" << endl;
}
void Car::simulationInit() {
        currRoad = -1;
        if (path.size() == 0) {
                return;
        }
        Road * road = path[0];
        if (road->srcId == src) {
                isSame = true;
        }
        else {
                isSame = false;
        }
}
int Car::getNextTurn() {
        if (currRoad == -1) {
                return STRAIGHT;
        }
        else if (currRoad == path.size() - 1) {
                return STRAIGHT;
        }
        else {
                Road * road = path[currRoad];
                Cross * nextCross = NULL;
                Road * nextRoad = path[currRoad + 1];
                int portIn, portOut;
                for (int i = 0; i < 2; i++) {
                        for (int j = 0; j < 2; j++) {
                                if (road->cross[i]->crossId == nextRoad->cross[j]->crossId) {
                                        nextCross = road->cross[i];
                                        break;
                                }
                        }
                }
                if (!nextCross) return STOP;
                for (int i = 0; i < 4; i++) {
                        if (!nextCross->road[i]) continue;
                        if (nextCross->road[i]->roadId == road->roadId) {
                                portIn = i;
                        }
                        if (nextCross->road[i]->roadId == nextRoad->roadId) {
                                portOut = i;
                        }
                }
                if ((portIn + LEFT) % 4 == portOut) {
                        return LEFT;
                }
                else if ((portIn + RIGHT) % 4 == portOut) {
                        return RIGHT;
                }
                else if ((portIn + STRAIGHT) % 4 == portOut) {
                        return STRAIGHT;
                }
                else return STOP;
        }
}


/***************************************functions of class RoadList***************************************/

Road * RoadList::addRoad(int * parameters) {
        if (head != NULL) {
                tail->next = new Road(parameters);
                tail = tail->next;
        }
        else {
                head = new Road(parameters);
                tail = head;
        }
        return tail;
}

void RoadList::printList() {
        Road * p = head;
        while (p) {
                cout << p->roadId << " " << p->length << " " << p->limitSpeed << " " << p->numberOfLanes << " " << p->srcId << " " << p->dstId << " " << p->isDoubleSide << endl;
                p = p->next;
        }
}

Road * RoadList::findRoadById(int id) {
        Road * p = head;
        while (p) {
                if (p->roadId == id) {
                        return p;
                }
                p = p->next;
        }
        return NULL;
}

void RoadList::printLinks() {
        Road * p = head;
        while (p) {
                cout << p->roadId << " ";
                for (int i = 0; i < 2; i++) {
                        cout << p->cross[i]->crossId << " ";
                }
                cout << endl;
                p = p->next;
        }
}

/***************************************functions of class CrossList***************************************/

Cross * CrossList::addCross(int * parameters) {
        if (head != NULL) {
                tail->next = new Cross(parameters);
                tail = tail->next;
        }
        else {
                head = new Cross(parameters);
                tail = head;
        }
        return tail;
}

void CrossList::printList() {
        Cross * p = head;
        while (p) {
                cout << p->crossId << " " << p->roadId[0] << " " << p->roadId[1] << " " << p->roadId[2] << " " << p->roadId[3] << endl;
                p = p->next;
        }
}

void CrossList::printLinks() {
        Cross * p = head;
        while (p) {
                cout << p->crossId << " ";
                for (int i = 0; i < 4; i++) {
                        if (p->road[i]) {
                                cout << p->road[i]->roadId << " ";
                        }
                        else {
                                cout << "NULL ";
                        }
                }
                cout << endl;
                p = p->next;
        }
}

Cross * CrossList::find(int dstId) {
        Cross * p = head;
        while (p) {
                if (p->crossId == dstId) return p;
                p = p->next;
        }
        return NULL;
}

void CrossList::linkRoadList(map<int, Road *> &roadMap) {
    Cross * p = head;
    Road * tmpRoad;
    while (p) {
        for (int i = 0; i < 4; i++) {
            if (p->roadId[i] == -1) {
                p->linkRoad(i, NULL);
            }
            else {
                tmpRoad = roadMap[p->roadId[i]];
                p->linkRoad(i, tmpRoad);
                tmpRoad->linkCross(p);
            }
        }
        p = p->next;
    }
}

/***************************************functions of class CarList***************************************/

Car * CarList::addCar(int * parameters) {
        if (head != NULL) {
                tail->next = new Car(parameters);
                tail = tail->next;
        }
        else {
                head = new Car(parameters);
                tail = head;
        }
        return tail;
}
Car * CarList::find(int id) {
        for (Car * car = head; car; car = car->next) {
                if (car->id == id) {
                        return car;
                }
        }
        return NULL;
}
void CarList::printList() {
        Car * p = head;
        while (p) {
                cout << p->id << " " << p->src << " " << p->dst << " " << p->maxSpeed << " " << p->departureTime << endl;
                p = p->next;
        }
}



/***************************************functions of class SimulatorAlorithm***************************************/

Car* SimulatorAlorithm::getCarInLane(Road * road, CarPosition carPos) {
    if (carPos.position == -1) return NULL;
    else {
        if (carPos.isSame) {
            return road->laneSame[carPos.lane][carPos.position];
        }
        else {
            return road->laneInvert[carPos.lane][carPos.position];
        }
    }
}

Car * SimulatorAlorithm::getFirstCarInLane(Road * road, int lane, bool isSame) {
    if (isSame) {
        for (int position = 0; position < road->length; position++) {
            Car * car = road->laneSame[lane][position];
            if (car != NULL) {
                if (car->state == STATE_WAIT) {
                    car->currPos.isSame = isSame;
                    car->currPos.lane = lane;
                    car->currPos.position = position;
                    return car;
                }
                else {
                    return NULL;
                }
            }
        }
    }
    else {
        if (road->isDoubleSide == 0) return NULL;
        for (int position = 0; position < road->length; position++) {
            Car * car = road->laneInvert[lane][position];
            if (car != NULL) {
                if (car->state == STATE_WAIT) {
                    car->currPos.isSame = isSame;
                    car->currPos.lane = lane;
                    car->currPos.position = position;
                    return car;
                }
                else {
                    return NULL;
                }
            }
        }
    }
    return NULL;
}

Car* SimulatorAlorithm::getFirstCar(Road * road, bool isSame) {
    vector<Car *> firstCars;
	for (int lane = 0; lane < road->numberOfLanes; lane++) {
		Car * car = getFirstCarInLane(road, lane, isSame);
		firstCars.push_back(car);
	}
	Car * mostPriorityCar = NULL;
	for (int i = 0; i < firstCars.size(); i++) {
		Car * car = firstCars[i];
		if (car) {
			if (mostPriorityCar == NULL) {
				mostPriorityCar = car;
			}
			else {
				if (mostPriorityCar->proriority == 1) { // most_prorioty_car is prorioty = 1
					if (car->proriority != 1) { // car is prorioty = 0
						continue;
					}
					else { // car is prorioty = 1
						if (car->currPos.position < mostPriorityCar->currPos.position) {
							mostPriorityCar = car;
						}
					}
				}
				else { // most_prorioty_car is prorioty = 0
					if (car->proriority != 1) { // car is prorioty = 0
						if (car->currPos.position < mostPriorityCar->currPos.position) {
							mostPriorityCar = car;
						}
					}
					else { // car is prorioty = 1
						mostPriorityCar = car;
					}
				}
			}
		}
	}
	return mostPriorityCar;
}

Car* SimulatorAlorithm::getLastCar(Road * road, bool isSame) {
	Car * lastCar = NULL;
	if (isSame) {
		for (int lane = 0; lane < road->numberOfLanes; lane++) {
			for (int position = road->length - 1; position >= 0; position--) {
				if (road->laneSame[lane][position]) {
					Car * car = road->laneSame[lane][position];
					if (lastCar == NULL) {
						lastCar = car;
					}
					else {
						if (lastCar->currPos.position == road->length - 1) { // prev lane last car at last position
							lastCar = car;
						}
					}
					break;
				}
			}
		}
	}
	else {
		if (road->isDoubleSide == 0) return NULL;
		for (int lane = 0; lane < road->numberOfLanes; lane++) {
			for (int position = road->length - 1; position >= 0; position--) {
				if (road->laneInvert[lane][position]) {
					Car * car = road->laneInvert[lane][position];
					if (lastCar == NULL) {
						lastCar = car;
					}
					else {
						if (lastCar->currPos.position == road->length - 1) { // prev lane last car at last position
							lastCar = car;
						}
					}
					break;
				}
			}
		}
	}
	return lastCar;
}

CarPosition SimulatorAlorithm::getLastPosition(Road * road, bool isSame, bool & lastCarStop) {
	lastCarStop = true;
	CarPosition carPos;
	carPos.lane = 0;
	carPos.position = -1;
	carPos.isSame = isSame;
	if (isSame) {
		for (int lane = 0; lane < road->numberOfLanes; lane++) {
			for (int position = road->length - 1; position >= 0; position--) {
				if (road->laneSame[lane][position]) {
					Car * car = road->laneSame[lane][position];
					if (car->state == STATE_WAIT) {
						lastCarStop = false;
						carPos.position = -1;
						return carPos;
					}
					lastCarStop = true;
					if (position == road->length - 1) {
						break;
					}
					carPos.lane = lane;
					carPos.position = position + 1;
					return carPos;
				}
				if (position == 0) {
					carPos.lane = lane;
					carPos.position = position;
					return carPos;
				}
			}
		}
	}
	else {
		for (int lane = 0; lane < road->numberOfLanes; lane++) {
			for (int position = road->length - 1; position >= 0; position--) {
				if (road->laneInvert[lane][position]) {
					Car * car = road->laneInvert[lane][position];
					if (car->state == STATE_WAIT) {
						lastCarStop = false;
						carPos.position = -1;
						return carPos;
					}
					lastCarStop = true;
					if (position == road->length - 1) {
						break;
					}
					carPos.lane = lane;
					carPos.position = position + 1;
					return carPos;
				}
				if (position == 0) {
					carPos.lane = lane;
					carPos.position = position;
					return carPos;
				}
			}
		}
	}
	return carPos;
}

void SimulatorAlorithm::init() {
    sort(roadVector.begin(), roadVector.end(), sortByRoadId);
    sort(crossVector.begin(), crossVector.end(), sortByCrossId);
    for (unsigned int i = 0; i < roadVector.size(); i++) {
        roadVector[i]->simulationInit();
    }
    for (unsigned int i = 0; i < inGarageVector.size(); i++) {
         inGarageVector[i]->simulationInit();
    }
    for (unsigned int i = 0; i < crossVector.size(); i++) {
         crossVector[i]->simulationInit();
    }
}

// working ...
// up has been test over
// working ...
void SimulatorAlorithm::printRoad(Road * road, bool isSame) {
    if (isSame) {
        for (int lane = 0; lane < road->numberOfLanes; lane++) {
            for (int position = 0; position < road->length; position++) {
                if (road->laneSame[lane][position]) {
                    cout << road->laneSame[lane][position]->id << "|";
                }
                else {
                    cout << "    |";
                }
                if (position == road->length - 1) cout << endl;
            }
        }
    }
    else {
        for (int lane = 0; lane < road->numberOfLanes; lane++) {
            for (int position = 0; position < road->length; position++) {
                if (road->laneInvert[lane][position]) {
                     cout << road->laneInvert[lane][position]->id << "|";
                }
                else {
                     cout << "    |";
                }
                if (position == road->length - 1) cout << endl;
            }
        }
    }
}

bool SimulatorAlorithm::runCarInRoad(Car * car, Road * road, bool isSame) {
    int distance = road->getSpeed(car->maxSpeed);
	int lane = car->currPos.lane;
	if (car->currPos.position == 0) {
		if (car->currRoad == car->path.size() - 1) {
			for (int i = 0; i < onRoadVector.size(); i++) {
				if (onRoadVector[i]->id == car->id) {
					onRoadVector.erase(onRoadVector.begin() + i);
					break;
				}
			}
			if (isSame) {
				road->laneSame[lane][0] = NULL;
			}
			else {
				road->laneInvert[lane][0] = NULL;
			}
			return true;
		}
		car->state = STATE_WAIT;
		return false;
	}
	if (isSame) {
		for (int position = car->currPos.position - 1; distance > 0; distance--) {
			if (road->laneSame[lane][position]) { // have block car
				Car * blockCar = road->laneSame[lane][position];
				if (blockCar->state == STATE_WAIT) {
					car->state = STATE_WAIT;
					return false;
				}
				else if (blockCar->state == STATE_STOP) {
					road->laneSame[lane][car->currPos.position] = NULL;
					car->currPos.isSame = isSame;
					car->currPos.lane = lane;
					car->currPos.position = position + 1;
					car->state = STATE_STOP;
					road->laneSame[car->currPos.lane][car->currPos.position] = car;
					return true;
				}
			}
			if (distance == 1) { // run over
				road->laneSame[lane][car->currPos.position] = NULL;
				car->currPos.isSame = isSame;
				car->currPos.lane = lane;
				car->currPos.position = position;
				car->state = STATE_STOP;
				road->laneSame[car->currPos.lane][car->currPos.position] = car;
				return true;
			}
			if (position == 0) { // at cross but still can run
				if (car->currRoad == car->path.size() - 1) { // car at dst
					for (int i = 0; i < onRoadVector.size(); i++) {
						if (onRoadVector[i]->id == car->id) {
							onRoadVector.erase(onRoadVector.begin() + i);
							break;
						}
					}
					road->laneSame[car->currPos.lane][car->currPos.position] = NULL;
					return true;
				}
				car->state = STATE_WAIT;
				return false;
			}
			position--;
		}
	}
	else {
		if (road->isDoubleSide == 0) return true;
		for (int position = car->currPos.position - 1; distance > 0; distance--) {
			if (road->laneInvert[lane][position]) { // have block car
				Car * blockCar = road->laneInvert[lane][position];
				if (blockCar->state == STATE_WAIT) {
					car->state = STATE_WAIT;
					return false;
				}
				else if (blockCar->state == STATE_STOP) {
					road->laneInvert[lane][car->currPos.position] = NULL;
					car->currPos.isSame = isSame;
					car->currPos.lane = lane;
					car->currPos.position = position + 1;
					car->state = STATE_STOP;
					road->laneInvert[car->currPos.lane][car->currPos.position] = car;
					return true;
				}
			}
			if (distance == 1) { // run over
				road->laneInvert[lane][car->currPos.position] = NULL;
				car->currPos.isSame = isSame;
				car->currPos.lane = lane;
				car->currPos.position = position;
				car->state = STATE_STOP;
				road->laneInvert[car->currPos.lane][car->currPos.position] = car;
				return true;
			}
			if (position == 0) { // at cross but still can run
				if (car->currRoad == car->path.size() - 1) { // car at dst
					for (int i = 0; i < onRoadVector.size(); i++) {
						if (onRoadVector[i]->id == car->id) {
							onRoadVector.erase(onRoadVector.begin() + i);
							break;
						}
					}
					road->laneInvert[car->currPos.lane][car->currPos.position] = NULL;
					return true;
				}
				car->state = STATE_WAIT;
				return false;
			}
			position--;
		}
	}
}

bool SimulatorAlorithm::forwardCarInLane(Road * road, bool isSame, int lane, int state, int & numberOfCars) { // forward all car in lane
	bool unblocked = false;
	int sumCar = 0;
	if (isSame) {
		for (int position = 0; position < road->length; position++) {
			if (road->laneSame[lane][position]) {
				Car * car = road->laneSame[lane][position];
				sumCar++;
				if (car->state == state) {
					if (runCarInRoad(car, road, true)) {
						unblocked = true;
					}
				}
				else {
					continue;
				}
			}
		}
	}
	else {
		for (int position = 0; position < road->length; position++) {
			if (road->laneInvert[lane][position]) {
				Car * car = road->laneInvert[lane][position];
				sumCar++;
				if (car->state == state) {
					if (runCarInRoad(car, road, false)) {
						unblocked = true;
					}
				}
				else {
					continue;
				}
			}
		}
	}
	numberOfCars = sumCar;
	return unblocked;
}

bool SimulatorAlorithm::forwardCarOn(Road * road, bool isSame, int & sumCar) { // push cars (STATE_ON -> STATE_WAIT || STATE_STOP) (ban go through cross)
    bool moved = false;
	int numberOfCar;
	if (isSame) {
		for (int lane = 0; lane < road->numberOfLanes; lane++) {
			if (forwardCarInLane(road, true, lane, STATE_ON, numberOfCar)) {
				moved = true;
			}
			sumCar += numberOfCar;
		}
	}
	else {
		for (int lane = 0; lane < road->numberOfLanes; lane++) {
			if (forwardCarInLane(road, false, lane, STATE_ON, numberOfCar)) {
				moved = true;
			}
			sumCar += numberOfCar;
		}
	}
	return moved;
}

bool SimulatorAlorithm::forwardCarWait(Road * fromRoad, Road * toRoad, Cross * midCross, Car * car) { // push the car (try STATE_WAIT -> STATE_STOP)
    bool fromSame, toSame;
	if (fromRoad->dstId == midCross->crossId) {
		fromSame = true;
	}
	else {
		fromSame = false;
	}
	if (toRoad->srcId == midCross->crossId) {
		toSame = true;
	}
	else {
		toSame = false;
	}
	bool lastCarStop;
	CarPosition carPos = getLastPosition(toRoad, toSame, lastCarStop);
	if (carPos.position == -1 && lastCarStop == false) {
		return false;
	}
	// update from road
	if (fromSame) { // clear <from road> car position
		fromRoad->laneSame[car->currPos.lane][car->currPos.position] = NULL;
	}
	else {
		fromRoad->laneInvert[car->currPos.lane][car->currPos.position] = NULL;
	}
	if (carPos.position == -1) {
		int distance = toRoad->getSpeed(car->maxSpeed);
		int nextDistance = distance - car->currPos.position;
		if (nextDistance <= 0) {
			car->currPos.position = car->currPos.position - distance;
			car->currPos.isSame = fromSame;
			car->state = STATE_STOP;
			if (fromSame) {
				fromRoad->laneSame[car->currPos.lane][car->currPos.position] = car;
			}
			else {
				fromRoad->laneInvert[car->currPos.lane][car->currPos.position] = car;
			}
			return true;
		}
		else {
			car->currPos.position = 0;
			car->currPos.isSame = fromSame;
			car->state = STATE_STOP;
			if (fromSame) {
				fromRoad->laneSame[car->currPos.lane][car->currPos.position] = car;
			}
			else {
				fromRoad->laneInvert[car->currPos.lane][car->currPos.position] = car;
			}
			return true;
		}
	}
	// update to road
	int distance = toRoad->getSpeed(car->maxSpeed);
	int nextDistance = distance - car->currPos.position;
	if (nextDistance <= 0) { // move car at current road
		car->currPos.position = car->currPos.position - distance;
		car->isSame = fromSame;
		car->state = STATE_STOP;
		if (fromSame) {
			fromRoad->laneSame[car->currPos.lane][car->currPos.position] = car;
		}
		else {
			fromRoad->laneInvert[car->currPos.lane][car->currPos.position] = car;
		}
	}
	else {
		if (toRoad->length - carPos.position > nextDistance) { // can go straightly
			car->currPos.position = toRoad->length - nextDistance;
		}
		else { // have to stop behind the block car
			car->currPos.position = carPos.position;
		}
		car->currPos.lane = carPos.lane;
		car->currPos.isSame = toSame;
		car->state = STATE_STOP;
		if (toSame) { // add car position to <to road>
			toRoad->laneSame[car->currPos.lane][car->currPos.position] = car;
		}
		else {
			toRoad->laneInvert[car->currPos.lane][car->currPos.position] = car;
		}
		car->currRoad++;
	}
	return true;
}

void SimulatorAlorithm::forwardRoad() {
    for (int i = 0; i < roadVector.size(); i++) {
		Road * road = roadVector[i];
		int sumCar;
		bool moved = forwardCarOn(road, true, sumCar);
		if (road->isDoubleSide) {
			moved = forwardCarOn(road, false, sumCar);
		}
	}
}

bool SimulatorAlorithm::forwardCross(Cross * cross) {
    vector<Car *> firstCars;
	// vector<int> firstCarsFrom;

	vector<bool> noCar;
	vector<bool> block;
	noCar.resize(4);
	block.resize(4);
	for (int i = 0; i < 4; i++) {
		noCar[i] = false;
		block[i] = false;
	}
	bool over = false;
	bool crossBlock = false;
	while (!(over || crossBlock)) {
		for (int roadP = 0; roadP < 4; roadP++) {
			if (block[roadP]) {
				continue;
			}
			if (cross->road[roadP]) {
				while (1) {
					bool isSame;
					vector<Car *> firstCars;
					Car * firstCar = NULL;
					int roadPort = (roadP + STRAIGHT) % 4;
					if (cross->road[roadPort]) {
						Road * road = cross->road[roadPort];
						if (road->dstId == cross->crossId) {
							isSame = true;
						}
						else {
							isSame = false;
						}
						Car *  car = getFirstCar(road, isSame);
						if (car) {
							int willGoRoad = car->currRoad + 1;
							if (car->path[willGoRoad]->roadId != cross->road[roadP]->roadId) {
								car = NULL;
							}
						}
						firstCars.push_back(car);
					}
					roadPort = (roadP + RIGHT) % 4;
					if (cross->road[roadPort]) { // left turn
						Road * road = cross->road[roadPort];
						if (road->dstId == cross->crossId) {
							isSame = true;
						}
						else {
							isSame = false;
						}
						Car * car = getFirstCar(road, isSame);
						if (car) {
							int willGoRoad = car->currRoad + 1;
							if (car->path[willGoRoad]->roadId != cross->road[roadP]->roadId) {
								car = NULL;
							}
						}
						firstCars.push_back(car);
					}
					roadPort = (roadP + LEFT) % 4;
					if (cross->road[roadPort]) { // right turn
						Road * road = cross->road[roadPort];
						if (road->dstId == cross->crossId) {
							isSame = true;
						}
						else {
							isSame = false;
						}
						Car * car = getFirstCar(road, isSame);
						if (car) {
							int willGoRoad = car->currRoad + 1;
							if (car->path[willGoRoad]->roadId != cross->road[roadP]->roadId) {
								car = NULL;
							}
						}
						firstCars.push_back(car);
					}
					for (int i = 0; i < firstCars.size(); i++) {
						if (firstCar == NULL) {
							firstCar = firstCars[i];
						}
						if (firstCars[i]) {
							if (firstCars[i]->proriority == 1) {
								firstCar = firstCars[i];
								break;
							}
						}
					}
					bool moved = false;
					if (firstCar) {
						noCar[roadP] = false;
						int lane = firstCar->currPos.lane;
						isSame = firstCar->currPos.isSame;
						int numberOfCars;
						Road * fromRoad = firstCar->path[firstCar->currRoad];
						moved = forwardCarWait(fromRoad, cross->road[roadP], cross, firstCar);
						if (moved) {
							forwardCarInLane(fromRoad, isSame, lane, STATE_WAIT, numberOfCars);
						}
					}
					if (firstCar == NULL) {
						noCar[roadP] = true;
						break;
					}
					if (moved == false) {
						block[roadP] = true;
						break;
					}
				}
			}
		}
		over = true;
		crossBlock = false;
		for (int i = 0; i < 4; i++) {
			if (cross->road[i]) {
				if (noCar[i] || block[i]) {
					if (over) {
						over = true;
					}
					else {
						over = false;
					}
				}
				if (block[i]) {
					crossBlock = true;
				}
			}
		}
	}
	if (crossBlock) {
		if (cross->blocked == MAX_BLOCKED_TIME) return false;
		cross->blocked++;
//		cout << "block cross:" << cross->crossId << " " << cross->blocked << endl;
	}
	else {
		cross->blocked = 0;
	}
	return true;
}

bool SimulatorAlorithm::startCar(Car * car) {
    if (car->path.size() == 0) {
		return true;
	}
	Road * road = car->path[0];
	bool isSame;
	if (road->srcId == car->src) {
		isSame = true;
	}
	else {
		isSame = false;
	}
	int distance = road->getSpeed(car->maxSpeed);
	bool lastCarStop;
	CarPosition carPos = getLastPosition(road, isSame, lastCarStop);
	if (carPos.position == -1) return false;
	int finalPosition, finalLane = carPos.lane;
	if (distance >= road->length - carPos.position) {
		finalPosition = carPos.position;
	}
	else {
		finalPosition = road->length - distance;
	}
	if (isSame) {
		road->laneSame[finalLane][finalPosition] = car;
	}
	else {
		road->laneInvert[finalLane][finalPosition] = car;
	}
	car->currPos.lane = finalLane;
	car->currPos.position = finalPosition;
	car->isSame = isSame;
	car->currRoad = 0;
	car->state = STATE_STOP;
	return true;
}

Cross* SimulatorAlorithm::simulatorCore() {
    sort(inGarageVector.begin(), inGarageVector.end(), sortByCarId);
    sort(inGarageVector.begin(), inGarageVector.end(), sortByCarTime);
    for (int i = 0; i < inGarageVector.size(); i++) {
        Car * car = inGarageVector[i];
        if (car->proriority == 1) {
            prCars.push_back(car);
        }
        else {
            orCars.push_back(car);
        }
    }

    return NULL;
}

Cross* SimulatorAlorithm::simulatorStep() {
    cout << systemTime << endl;
    for (int i = 0; i < onRoadVector.size(); i++) {
        onRoadVector[i]->state = STATE_ON;
    }
    forwardRoad();
    bool over = false;
    if (onRoadVector.size() == 0) over = true;
    while (!over) {
        for (int crossP = 0; crossP < crossVector.size(); crossP++) {
			bool success = forwardCross(crossVector[crossP]);
			if (!success) {
				return crossVector[crossP];
			}
		}
		for (int carP = 0; carP < onRoadVector.size(); carP++) {
			
			Car * car = onRoadVector[carP];
			if (car->state != STATE_STOP) {
				break;
			}
			if (carP == onRoadVector.size() - 1) {
				over = true;
			}
		}
    }
    systemTime++;
    for (int i = 0; i < prCars.size(); i++) {
		if (prCars[i]->offTime <= systemTime) {
			Car * car = prCars[i];
			if (startCar(car) == true) {
				onRoadVector.push_back(car);
				prCars.erase(prCars.begin() + i);
				i--;
			}
		}
		else {
			break;
		}
	}
	for (int i = 0; i < orCars.size(); i++) {
		if (orCars[i]->offTime <= systemTime) {
			Car * car = orCars[i];
			if (startCar(car) == true) {
				onRoadVector.push_back(car);
				orCars.erase(orCars.begin() + i);
				i--;
			}
		}
		else {
			break;
		}
	}
    return NULL;
}

bool SimulatorAlorithm::isFinished() {
    if(prCars.size() != 0 || orCars.size() != 0 || onRoadVector.size() != 0) return false;
    return true;
}

/***************************************algorithm defination***************************************/
bool sortByRoadId(Road * road1, Road * road2) { return road1->roadId < road2->roadId; }
bool sortByCrossId(Cross * cross1, Cross * cross2) { return cross1->crossId < cross2->crossId; }
bool sortByCarTime(Car * car1, Car * car2) { return car1->offTime < car2->offTime; }
bool sortByCarId(Car * car1, Car * car2) { return car1->id < car2->id; }

bool bigger(double x, double y) { // compare positive double (-1 means max)
        if (x == -1) {
                return true;
        }
        if (y == -1) {
                return false;
        }
        return x > y;
}
void initFindPathFinal(CrossList & crossList) {
        for (Cross * cross = crossList.head; cross; cross = cross->next) {
                cross->finalRouter.port = -1;
                cross->time = -1;
        }
}
double findPathFinal(Cross * src, Cross * dst, Cross * pre, double time, Car * car, int numberOfNodePass) {
        if (src == NULL) return -1;
        if (src->crossId == dst->crossId) return 0;
        if (bigger(time, src->time)) return -1;
        src->time = time;
        double weightList[4];
        for (int i = 0; i < 4; i++) {
                weightList[i] = -1;
                if (src->road[i]) {
                        Road * nextRoad = src->road[i];
                        Cross * nextCross = nextRoad->getNextCross(src);
                        if (pre) {
                                if (nextCross->crossId == pre->crossId) {
                                        continue;
                                }
                        }
                        weightList[i] = findPathFinal(nextCross, dst, src, time + nextRoad->getTimeWeight(car->maxSpeed), car, numberOfNodePass + 1);
                        if (weightList[i] != -1) {
                                weightList[i] += nextRoad->getTimeWeight(car->maxSpeed);
                        }
                }
        }
        if (weightList[0] == -1 && weightList[1] == -1 && weightList[2] == -1 && weightList[3] == -1) {
                return -1;
        }
        int minPort = -1;
        double minWeight = -1;
        for (int i = 0; i < 4; i++) {
                if (weightList[i] == -1) continue;
                if (bigger(minWeight, weightList[i])) {
                        minWeight = weightList[i];
                        minPort = i;
                }
        }
        if (src->finalRouter.port == -1) {
                src->finalRouter.port = minPort;
                src->finalRouter.weight = minWeight;
        }
        else {
                if (bigger(src->finalRouter.weight, minWeight)) {
                        src->finalRouter.port = minPort;
                        src->finalRouter.weight = minWeight;
                }
        }
        return minWeight;
}

void addFinalPathToCar(Car * car, map<int, Cross*> &crossMap) {
    cout << car->id << ":";
    for (Cross * cross = crossMap[car->src]; cross->crossId != car->dst; ) {
        Road * nextRoad = cross->road[cross->finalRouter.port];
        car->path.push_back(nextRoad);
        cout << " " << cross->crossId;
        cross = nextRoad->getNextCross(cross);
    }
    cout << endl;
}

/***************************************functions of basic IO***************************************/

void loadRoadFile(string fileName, RoadList & roadList, map<int, Road*> &roadMap, vector<Road *> &roadList_pVector) {
    char currentChar;
    string word = "";
    int parameters[7]; // a road have 7 parameters
    int sumParameters = 0;
    bool isReading = false;
    bool isComment = false;

    FILE * filePointer = fopen(fileName.data(), "r"); // use road file

    currentChar = fgetc(filePointer);
    while (currentChar != EOF) {
        if (isComment) {
            if (currentChar == '\n') {
                isComment = false;
            }
            currentChar = fgetc(filePointer);
            continue;
        }
        if (currentChar == '#') { // comment
            isComment = true;
            currentChar = fgetc(filePointer);
            continue;
        }
        else if (currentChar == '(') { // new line
                //cout << endl;
        }
        else if (currentChar == ')' || currentChar == ',' || currentChar == ' ') { // end of a word
            if (isReading) {
                isReading = false;
                parameters[sumParameters] = atoi(word.c_str());
                //cout << parameters[sumParameters] << " ";
                sumParameters++;
                word = "";
                if (currentChar == ')') { // end of a line
                    Road * road = roadList.addRoad(parameters);
                    roadList_pVector.push_back(road);
                    roadMap[road->roadId] = road;
                    sumParameters = 0;
                }
            }
        }
        else if (currentChar != '\n') { // not a symbol
            isReading = true;
        }
        if (isReading) { // recording word
            word += currentChar;
        }
        currentChar = fgetc(filePointer);
    }
    fclose(filePointer);
}

void loadCrossFile(string fileName, CrossList & crossList, map<int, Cross*> &crossMap, vector<Cross *> &crossList_pVector) {
    char currentChar;
    string word = "";
    int parameters[5]; // a cross have 5 parameters
    int sumParameters = 0;
    bool isReading = false;
    bool isComment = false;

    FILE * filePointer = fopen(fileName.data(), "r"); // use cross file

    currentChar = fgetc(filePointer);
    while (currentChar != EOF) {
        if (isComment) {
            if (currentChar == '\n') {
                    isComment = false;
            }
            currentChar = fgetc(filePointer);
            continue;
        }
        if (currentChar == '#') { // comment
            isComment = true;
            currentChar = fgetc(filePointer);
            continue;
        }
        else if (currentChar == '(') { // new line
                //cout << endl;
        }
        else if (currentChar == ')' || currentChar == ',' || currentChar == ' ') { // end of a word
            if (isReading) {
                isReading = false;
                parameters[sumParameters] = atoi(word.c_str());
                //cout << parameters[sumParameters] << " ";
                sumParameters++;
                word = "";
                if (currentChar == ')') { // end of a line
                    Cross * cross = crossList.addCross(parameters);
                    crossList_pVector.push_back(cross);
                    crossMap[cross->crossId] = cross;
                    sumParameters = 0;
                }
            }
        }
        else if (currentChar != '\n') { // not a symbol
                isReading = true;
        }
        if (isReading) { // recording word
                word += currentChar;
        }
        currentChar = fgetc(filePointer);
    }
    fclose(filePointer);
}

void loadCarFile(string fileName, CarList & carList, map<int, Car*> &carMap, vector<Car *> &carList_pVector) {
    char currentChar;
    string word = "";
    int parameters[7]; // a car have 7 parameters
    int sumParameters = 0;
    bool isReading = false;
    bool isComment = false;

    FILE * filePointer = fopen(fileName.data(), "r"); // use car file

    currentChar = fgetc(filePointer);
    srand((unsigned)time(NULL));
    while (currentChar != EOF) {
        if (isComment) {
            if (currentChar == '\n') {
                isComment = false;
            }
            currentChar = fgetc(filePointer);
            continue;
        }
        if (currentChar == '#') { // comment
            isComment = true;
            currentChar = fgetc(filePointer);
            continue;
        }
        else if (currentChar == '(') { // new line
                //cout << endl;
        }
        else if (currentChar == ')' || currentChar == ',' || currentChar == ' ') { // end of a word
            if (isReading) {
                isReading = false;
                parameters[sumParameters] = atoi(word.c_str());
                //cout << parameters[sumParameters] << " ";
                sumParameters++;
                word = "";
                if (currentChar == ')') { // end of a line
                    Car * car = carList.addCar(parameters);
                    carList_pVector.push_back(car);
                    carMap[car->id] = car;
                    sumParameters = 0;
                }
            }
        }
        else if (currentChar != '\n') { // not a symbol
                isReading = true;
        }
        if (isReading) { // recording word
                word += currentChar;
        }
        currentChar = fgetc(filePointer);
    }
    fclose(filePointer);
}

void setPreCarPath(vector<int> &parameters, map<int, Car*> &carMap, map<int, Road*> &roadMap) {
    Car * car = carMap[parameters[0]];
    car->offTime = parameters[1];
    for (int i = 2; i < parameters.size(); i++) {
            car->path.push_back(roadMap[parameters[i]]);
    }
}

void loadPreFile(string fileName, map<int, Car*> &carMap, map<int, Road*> &roadMap) {
    char currentChar;
    string word = "";
    vector<int> parameters;
    bool isReading = false;
    bool isComment = false;

    FILE * filePointer = fopen(fileName.data(), "r"); // use car file

    currentChar = fgetc(filePointer);
    while (currentChar != EOF) {
            if (isComment) {
                    if (currentChar == '\n') {
                            isComment = false;
                    }
                    currentChar = fgetc(filePointer);
                    continue;
            }
            if (currentChar == '#') { // comment
                    isComment = true;
                    currentChar = fgetc(filePointer);
                    continue;
            }
            else if (currentChar == '(') { // new line
                    //cout << endl;
            }
            else if (currentChar == ')' || currentChar == ',' || currentChar == ' ') { // end of a word
                    if (isReading) {
                            isReading = false;
                            parameters.push_back(atoi(word.c_str()));
                            //cout << parameters[sumParameters] << " ";
                            word = "";
                            if (currentChar == ')') { // end of a line
                                    setPreCarPath(parameters, carMap, roadMap);
                                    parameters.clear();
                            }
                    }
            }
            else if (currentChar != '\n') { // not a symbol
                    isReading = true;
            }
            if (isReading) { // recording word
                    word += currentChar;
            }
            currentChar = fgetc(filePointer);
    }
    fclose(filePointer);
}

void outputPathFile(string fileName, CarList & carList) {
    FILE * filePointer = fopen(fileName.data(), "w+");
    Car * car = carList.head;
    string outputString;
    char tmpString[100];
    while (car) {
            outputString = "(";
            sprintf(tmpString, "%d", car->id);
            outputString += tmpString;
            outputString += ", ";
            sprintf(tmpString, "%d", car->offTime);
            outputString += tmpString;
            outputString += ", ";
            for (int i = 0; i < car->path.size(); i++) {
                    sprintf(tmpString, "%d", car->path[i]->roadId);
                    outputString += tmpString;
                    if (i != car->path.size() - 1) outputString += ", ";
            }
            outputString += ")\n";
            fputs(outputString.data(), filePointer);
            car = car->next;
    }
    fclose(filePointer);
}


/***************************************functions for correcting directions***************************************/

void DFS(map<int, Cross*> &crossMap, map<int, Road*> &roadMap, map<int, bool> &visitDone, int crossID, int direction, int preCrossID, int posX, int posY, int &minX, int &minY) {
    if (visitDone[crossID]) {
        return;
    }
    visitDone[crossID] = true;
    crossMap[crossID]->relativePosX = posX;
    crossMap[crossID]->relativePosY = posY;
    minX = min(minX, posX);
    minY = min(minY, posY);
    int *road = crossMap[crossID]->roadId;
    if (preCrossID != -1) {
        int i;
        for (i = 0; i < 4; i++) {
            int roadID = road[i];
            if (roadID != -1) {
                int pcID = roadMap[roadID]->srcId != crossID ? roadMap[roadID]->srcId : roadMap[roadID]->dstId;
                if (pcID == preCrossID) {
                    break;
                }
            }
        }
        int dirCorrection = direction == 1 || direction == 3 ? direction : (direction + 2) % 4;
        int shift = (dirCorrection + i) % 4;
        for (int j = 0; j < shift; j++) {
            int temp0 = road[1];
            int temp1 = road[2];
            int temp2 = road[3];
            int temp3 = road[0];

            road[0] = temp0;
            road[1] = temp1;
            road[2] = temp2;
            road[3] = temp3;
        }
    }

    int constPosX = posX;
    int constPosY = posY;
    for (int i = 0; i < 4; i++) {
        int roadID = road[i];
        posX = constPosX;
        posY = constPosY;
        if (roadID != -1) {
            int ncID = roadMap[roadID]->srcId != crossID ? roadMap[roadID]->srcId : roadMap[roadID]->dstId;
            switch (i) {
                case 0:posY -= 1; break;
                case 1:posX += 1; break;
                case 2:posY += 1; break;
                case 3:posX -= 1; break;
            }

            DFS(crossMap, roadMap, visitDone, ncID, i, crossID, posX, posY, minX, minY);
        }
    }
}

void correctCrossDirection(map<int, Cross*> &crossMap, map<int, Road*> &roadMap) {
    map<int, bool> visitDone;
    int minX = 0;
    int minY = 0;
    for (auto cross : crossMap) {
        visitDone.insert(pair<int, bool>(cross.first,false));
    }
    DFS(crossMap, roadMap, visitDone, crossMap.begin()->first, -1, -1, 0, 0, minX, minY);

    int modifyX = abs(minX);
    int modifyY = abs(minY);
    for (auto &iter : crossMap) {
        Cross *cross = iter.second;
        cross->relativePosX = cross->relativePosX + modifyX;
        cross->relativePosY = cross->relativePosY + modifyY;
    }
}

void calcOffArrCross(map<int, Car*> &carMap, map<int, Cross*> &crossMap) {
    for (auto iter : carMap) {
        Car *car = iter.second;
        crossMap[car->src]->addCarsOffHere();
        crossMap[car->dst]->addCarsArrHere();
    }
}

void printPosition(map<int, Cross*> &crossMap) {
    for(auto iter : crossMap) {
        Cross *cross = iter.second;
        cout << "corssId: " << cross->crossId << "(" << cross->relativePosX << "," << cross->relativePosY << ")" << endl;
    }
}

void printLink(map<int, Cross*> &crossMap) {
    for(auto iter : crossMap) {
        Cross *cross = iter.second;
        cout << "corssId: " << cross->crossId << endl;
        int *directions = cross->roadId;
        for (int i = 0; i < 4; i++) {
            if (directions[i] != -1) {
                cout << "cross.direction[" << i << "].roadID: " << directions[i] << endl;
            }
            else {
                cout << "cross.direction[" << i << "].roadID: -1" << endl;
            }
        }
    }
}
