#include <QMainWindow>
#include <QWidget>
#include <QQuickWidget>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QGVWidgetScale.h>
#include <QGVWidgetZoom.h>
#include <QGraphicsView>
#include "demoitem.h"
#include "samples/rectangle.h"
#include <QGVLayer.h>
#include "qroute.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include<QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton> // Добавьте эту строку
#include "samples/placemark.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

public:
    ~MainWindow();

private slots:
    void init();
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();
    void mousePress(QPointF coords);
    void on_pushButton_5_clicked(QPointF coords);

    void on_pushButton_3_clicked();

    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();

    //void on_pushButton_10_clicked();
public:
    void on_lineEdit_editingFinished();

private slots:
    void on_pushButton_clicked(QPointF coords);
void deleteListItem(QListWidgetItem* item);

private:
private:
    QList<QRoute*> routesList; // Список маршрутов для отображения на карте
    QList<QPointF> coordinatesArray;
    QList<QRoute*> routes; // Объявление списка маршрутов
    QListWidgetItem* currentItem;
    QList<QRoute*>* pendingRoutes; // Список для хранения маршрутов
    QList<Placemark*>* placedMarks; // // Добавьте это объявление здесь
    QPushButton *deleteButton;
    QListWidget *listWidget;
    QListWidget *listWidget_2;
    bool addMarkersEnabled = false;
    bool addMarkersEnabled_2 = false;
    QLabel* label;
    QVBoxLayout* layout1;
    QHBoxLayout* layout2;
    QGridLayout* mainLayout;
    bool isLayout1Active = true;
    int xCoord;
    int yCoord;
private:
    QMediaPlayer* mediaPlayer;
    QAudioOutput* audioOutput;

private slots:
    void on_listWidget_2_itemClicked(QListWidgetItem *item);

    void on_widget_customContextMenuRequested(const QPoint &pos);

    void on_pushButton_12_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_returnPressed(const QString &arg1);

    void setScale(double scale);
    void on_lineEdit_2_returnPressed();

    void on_pushButton_6_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_7_clicked();

    void on_lineEdit_returnPressed();

private:
    QMap<int, QGV::GeoPos> coordinatesMap;
    QMap<int, Placemark*> placemarksMap;
    QVector<QPointF> storedCoordinates; // Объявление переменной для хранения координат
    Ui::MainWindow* ui;
    QNetworkAccessManager* mManager;
    QNetworkDiskCache* mCache;
    QGVMap* map;
    bool drawingRoute = false;
    QRoute* pendingRoute = nullptr;
    QGVLayer* drawingLayer = nullptr;
    bool sloiy = false;
    QRoute* pending = nullptr;
    QGVLayer* sloi = nullptr;
};
