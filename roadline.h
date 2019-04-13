#ifndef ROADLINE_H
#define ROADLINE_H

#include <QColor>
#include <QGraphicsItem>

class RoadLine : public QGraphicsItem
{
public:
    RoadLine(int id, int width, int height, int startCrossID, int endCrossID, int channel, int length, int speed, QString carInfo);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int width;
    int height;
    int id;
    int startCrossID;
    int endCrossID;
    int status;
    int channel;
    int speed;
    int length;
    QString carInfo;
    QColor roadUsageColor;
    QColor color;
    QVector<QPointF> stuff;
};

#endif // ROADLINE_H
