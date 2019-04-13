#include "roadline.h"
#include <iostream>

#include <QtWidgets>

RoadLine:: RoadLine(int id, int width, int height, int start, int end, int channel, int length, int speed, QString carInfo)
{
    this->width = width;
    this->height = height;
    this->startCrossID = start;
    this->endCrossID = end;
    this->color = Qt::lightGray;
    this->id = id;
    this->channel = channel;
    this->length = length;
    this->status = 0;
    this->speed = speed;
    this->carInfo = carInfo;

    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF RoadLine::boundingRect() const
{
    return QRectF(0, 0, width, height);
}

QPainterPath RoadLine::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, width, height);
    return path;
}

void RoadLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    painter->save();
    QPen pen;
    pen.setWidth(1);
    pen.setColor(this->color);
    switch(this->status) {
    case 0: painter->setBrush(this->color); break;
    case 1: painter->setBrush(Qt::gray); break;
    case 3: painter->setBrush(Qt::lightGray); break;
    default: painter->setBrush(Qt::lightGray);
    }
    painter->setPen(pen);
    painter->drawRect(QRect(0, 0, width, height));
    painter->restore();

    QFont font("Arial", 6);
    font.setStyleStrategy(QFont::ForceOutline);
    painter->setFont(font);
    painter->save();
    if(this->status == 3) {
        QString roadBasicInfo = QString::number(this->id).append(" | Chan: ").append(QString::number(this->channel)).append(" | Len: ").append(QString::number(this->length));
        QString info = QString(QString::number(this->startCrossID)).append(QString("→")).append(QString::number(this->endCrossID)).append(" | Speed: ").append(QString::number(this->speed));
        if(this->width < 50) {
            painter->drawText(10, this->height / 2, roadBasicInfo);
            painter->drawText(10, this->height / 2 + 10, info);
        }
        else {
            painter->drawText(5, 0, roadBasicInfo);
            painter->drawText(5, 10, info);
        }
    }
    painter->restore();
}

void RoadLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    this->status = 3;
    update();
}

void RoadLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void RoadLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    this->status = 0;
    update();
}
