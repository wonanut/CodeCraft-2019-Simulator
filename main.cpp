#include "mainwindow.h"
#include <QApplication>

#define roadPath "./file/road.txt"
#define crossPath "./file/cross.txt"
#define carPath "./file/car.txt"
#define presetAnswerPath "./file/presetAnswer.txt"
#define answerPath "./file/answer.txt"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    string roadFileName(roadPath);
    string crossFileName(crossPath);
    string carFileName(carPath);
    string presetAnswerFileName(presetAnswerPath);
    string answerFileName(answerPath);

    RoadList roadList;
    CrossList crossList;
    CarList carList;

    map<int, Car*> carMap;
    map<int, Road*> roadMap;
    map<int, Cross*> crossMap;

    vector<Road *> roadList_pVector;
    vector<Car *> carList_pVector;
    vector<Cross *> crossList_pVector;

    // TODO:read input filebuf

    cout << "loading car file...";
    loadCarFile(carPath, carList, carMap, carList_pVector);
    cout << " done." << endl;

    cout << "loading road file...";
    loadRoadFile(roadPath, roadList, roadMap, roadList_pVector);
    cout << " done." << endl;

    cout << "loading cross file...";
    loadCrossFile(crossPath, crossList, crossMap, crossList_pVector);
    cout << " done." << endl;

    cout << "loading presetAnswer file...";
    loadPreFile(presetAnswerPath, carMap, roadMap);
    cout << " done." << endl;

    cout << "correcting the cross direction...";
    correctCrossDirection(crossMap, roadMap);
    cout << " done." << endl;

    cout << "creating linking map...";
    crossList.linkRoadList(roadMap);
    cout << " done." << endl;

    calcOffArrCross(carMap, crossMap);

    MainWindow window(roadMap, crossMap, roadList_pVector, crossList_pVector, carList_pVector);
    window.show();

    return app.exec();
}
