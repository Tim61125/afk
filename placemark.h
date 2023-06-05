#pragma once

#include <QGVImage.h>

class Placemark : public QGVImage
{
    Q_OBJECT

public:
    explicit Placemark(const QGV::GeoPos& geoPos);
    void Roi(const QGV::GeoPos& geoPos);
    QPointF mCoords;

protected:
    QTransform projTransform() const override;
    void projOnFlags() override;
};
