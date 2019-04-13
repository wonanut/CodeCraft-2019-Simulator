#ifndef CAR_H
#define CAR_H
#include <QColor>
#include <QGraphicsItem>
class CarRect : public QGraphicsItem
{
public:
    CarRect(int id, float width, float height, QString info, QColor color);

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
    float width;
    float height;
    int status = 0;
    QColor color;
    QString info;
    QVector<QPointF> stuff;
};

#endif // CAR_H
