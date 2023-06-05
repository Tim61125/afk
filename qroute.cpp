#include "qroute.h"
#include "mainwindow.h"

QRoute::QRoute()
{
    setFlag(QGV::ItemFlag::IgnoreScale);
    resetBoundary();
    scaleCoefficient = 1.0;
}

QPainterPath QRoute::projShape() const
{
    QGVMap* map = getMap();

    QList<QPointF> _points;

    if (points.size() >= 2)
    {
        QPointF startPoint(map->getProjection()->geoToProj(QGV::GeoPos(points.first().x(), points.first().y())));

        _points.append(startPoint);

        for (int i = 1; i < points.size(); i++)
        {
            QPointF endPoint(map->getProjection()->geoToProj(QGV::GeoPos(points[i].x(), points[i].y())));

            _points.append(endPoint);

            startPoint = endPoint;
        }
    }

    QPainterPath path;

    path.addPolygon(QPolygonF::fromList(_points));

    if (_points.size() > 2)
    {
        qInfo() << "QRoute::projShape" << _points.size();

        QRectF _bonds = path.boundingRect();

        qInfo() << "Bonds: " << _bonds.width() << " x " << _bonds.height();
    }

    return path;
}

void QRoute::projPaint(QPainter* painter)
{
    QPen pen;
    QGVMap* map = getMap();

    QGVDrawItem::onProjection(map);

    if (isFlag(QGV::ItemFlag::Highlighted) && isFlag(QGV::ItemFlag::HighlightCustom))
    {
        pen = QPen(QBrush(Qt::black), 5);
    }
    else
    {
        pen = QPen(QBrush(Qt::black), 1);
    }

    pen.setCosmetic(true);
    painter->setPen(pen);

    painter->setBrush(QBrush(Qt::black));

    if (points.size() >= 2)
    {
        // Умножьте ширину пера на scaleCoefficient
        QPen linePen(QBrush(Qt::black), 2 * scaleCoefficient);
        painter->setPen(linePen);

        // Рисуем линии между точками и добавляем полигон-стрелку на каждой точке
        for (int i = 1; i < points.size(); i++)
        {
            QPointF startPoint(getMap()->getProjection()->geoToProj(QGV::GeoPos(points[i - 1].x(), points[i - 1].y())));
            QPointF endPoint(getMap()->getProjection()->geoToProj(QGV::GeoPos(points[i].x(), points[i].y())));

            // Рисуем линию
            painter->drawLine(startPoint, endPoint);

            // Рисуем полигон-стрелку
            QPolygonF arrowPoly;
            // Определите форму полигона стрелки в соответствии с вашими требованиями
            arrowPoly << QPointF(0, 0) << QPointF(-10, 5) << QPointF(-10, -5);
            QTransform transform;
            // Установите угол поворота стрелки, чтобы она указывала в направлении последней точки
            transform.rotate(-QLineF(startPoint, endPoint).angle());
            arrowPoly = transform.map(arrowPoly);
            arrowPoly.translate(endPoint);

            painter->drawPolygon(arrowPoly);
        }
    }
}

void QRoute::addPoint(const QPointF& point)
{
    points.append(point);
    refresh();
    update();
}

QPointF QRoute::getLastPoint() const
{
    if (points.isEmpty())
    {
        return QPointF(); // Возвращаем пустую точку, если список пуст
    }

    return points.last();
}
bool QRoute::containsPoint(double latitude, double longitude) const
{
    for (const QPointF& point : points) {
        if (qFuzzyCompare(point.x(), latitude) && qFuzzyCompare(point.y(), longitude)) {
            return true;
        }
    }
    return false;
}


