#ifndef QROUTE_H
#define QROUTE_H

#include <QGVDrawItem.h>
class QListWidgetItem; // Доб
class QRoute : public QGVDrawItem
{
    Q_OBJECT
public:
    QRoute();
    void addPoint(const QPointF& point);
    QPainterPath projShape() const;
    void clear();
    void projPaint(QPainter* painter);
    QList<QPointF> getPoints() const {
        return points;

    }
private:
    QList<QPointF> points;
public:
    // ... другие члены класса ...
public:
    bool containsPoint(const QPointF& point) const; // Добавьте этот метод для проверки, содержит ли QRoute указанную точку
    QListWidgetItem* item() const;
    QPointF getLastPoint() const;
private:
    double scaleCoefficient;
public:
    void setScaleCoefficient(double scale) { scaleCoefficient = scale; }
    bool containsPoint(double latitude, double longitude) const; // Добавленное объявление метода

};

#endif // QROUTE_H
