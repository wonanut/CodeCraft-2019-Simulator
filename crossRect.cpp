#include "crossRect.h"
#include <iostream>

#include <QtWidgets>

Chip::Chip(int id, int offCars, int arrCars, int maxArr, int minArr, int maxOff, int minOff, int colorType)
{
    if(maxArr - minArr == 0) maxArr += 1;
    if(maxOff - minOff == 0) maxOff += 1;
    this->color = Qt::green;
    this->arrColor = Qt::white;
    this->arrColor.setGreen(255 - 255 * (arrCars - minArr) / (maxArr - minArr));
    this->offColor = Qt::white;
    this->offColor.setRed(255 - 255 * (offCars - minOff) / (maxOff - minOff));
    this->id = id;
    this->offCars = offCars;
    this->arrCars = arrCars;
    this->curColorType = colorType;
    this->defaultColorType = colorType;

    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF Chip::boundingRect() const
{
    return QRectF(0, 0, 40, 40);
}

QPainterPath Chip::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, 40, 40);
    return path;
}

void Chip::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QBrush b = painter->brush();

    // different with different colorType
    switch(this->curColorType) {
    case 0: painter->setBrush(this->color); break;
    case 1: painter->setBrush(this->offColor); break;
    case 2: painter->setBrush(this->arrColor); break;
    case 3: painter->setBrush(Qt::gray); break;
    case 4: painter->setBrush(Qt::red); break;
    default: painter->setBrush(this->color);
    }

    painter->drawRect(QRect(0, 0, 40, 40));

    QFont font("Arial", 100);
    font.setStyleStrategy(QFont::ForceOutline);
    painter->setFont(font);
    painter->save();
    painter->scale(0.1, 0.1);
    painter->drawText(50, 200, QString::number(this->id));
    painter->restore();

    QFont font2("Arial", 12);
    font2.setStyleStrategy(QFont::ForceOutline);
    painter->setFont(font2);
    painter->save();
    painter->scale(0.1, 0.1);

    int basicX = 50;
    int basicY = 250;
    QString str = QString("Cars will start here: ").append(QString::number(this->offCars));
    painter->drawText(basicX, basicY, str);
    QString str2 = QString("Cars will arrive here: ").append(QString::number(this->arrCars));
    painter->drawText(basicX, basicY + 25, str2);
    QString str3 = QString("Current color mode: ").append(QString::number(this->curColorType));
    painter->drawText(basicX, basicY + 50, str3);
    painter->restore();
}

void Chip::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    this->curColorType = 3;
    update();
}

void Chip::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void Chip::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    this->curColorType = this->defaultColorType;
    update();
}
