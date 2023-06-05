#include "placemark.h"

Placemark::Placemark(const QGV::GeoPos& geoPos)
{
    setFlag(QGV::ItemFlag::IgnoreScale);
    setFlag(QGV::ItemFlag::IgnoreAzimuth);
    setFlag(QGV::ItemFlag::Highlightable);
    setFlag(QGV::ItemFlag::HighlightCustom);
    setFlag(QGV::ItemFlag::Highlightable);
    setFlag(QGV::ItemFlag::Transformed);
    setGeometry(geoPos, QSize(32, 32), QPoint(16, 32));
    const QString url = "http://maps.google.com/mapfiles/kml/paddle/blu-circle.png";
    load(url);
    mCoords = QPointF(geoPos.longitude(), geoPos.latitude());

}

QTransform Placemark::projTransform() const
{
    return isFlag(QGV::ItemFlag::Highlighted) ? QGV::createTransfromScale(projAnchor(), 1.2) : QTransform();
}

void Placemark::projOnFlags()
{
    setOpacity(isFlag(QGV::ItemFlag::Highlighted) ? 0.3 : 1.0);
}
