
#include "carRect.h"
#include <iostream>

#include <QtWidgets>

CarRect:: CarRect(int id, float width, float height, QString info, QColor color)
{
    this->id = id;
    this->width = width;
    this->height = height;
    this->color = color;
    this->info = info;
    this->status = 0;
    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF CarRect::boundingRect() const
{
    return QRectF(0, 0, width, height);
}

QPainterPath CarRect::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, width, height);
    return path;
}

void CarRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    QPen pen(this->color);
    QBrush brush(this->color);

    painter->save();
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(QRect(0, 0, width, height));
    painter->restore();

//    QFont font2("Arial", 8);
//    font2.setStyleStrategy(QFont::ForceOutline);
//    painter->setFont(font2);
//    painter->save();
//    painter->scale(0.1, 0.1);
//    QString str = QString::number(this->id);
//    painter->drawText(0, 20, str);
//    painter->restore();

    if(this->status == 3) {
        QFont font("Arial", 8);
        font.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font);
        painter->save();
        painter->scale(0.1, 0.1);
        QString str = QString::number(this->id);
        painter->drawText(0, 20, str);
        painter->restore();

        QFont font2("Arial", 48);
        font2.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font2);
        painter->save();
        painter->scale(0.1, 0.1);
        painter->drawText(80, -15, str);

        QFont font3("Arial", 32);
        font2.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font3);
        painter->drawText(80, 45, info);
        painter->restore();
    }
}

void CarRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    this->status = 3;
    update();
}

void CarRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void CarRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    this->status = 0;
    update();
}
