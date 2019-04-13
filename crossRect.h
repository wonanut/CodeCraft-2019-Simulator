#ifndef CHIP_H
#define CHIP_H

#include <QColor>
#include <QGraphicsItem>

class Chip : public QGraphicsItem
{
public:
    Chip(int id, int offCars, int arrCars, int maxArr, int minArr, int maxOff, int minOff, int colorType);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void setToolTip();

private:
    int id;
    int offCars;
    int arrCars;
    int curColorType;
    int defaultColorType;
    QColor arrColor;
    QColor offColor;
    QColor color;
    QVector<QPointF> stuff;
};

#endif // CHIP_H
