#include "mainwindow.h"
#include"second_window.h"
#include "samples/background.h"
#include "samples/customtiles.h"
#include "samples/flags.h"
#include "samples/items.h"
#include "samples/mouse.h"
#include "samples/utilities.h"
#include "samples/widgets.h"
#include "samples/placemark.h"
#include "ui_mainwindow.h"
#include <QtLocation>
#include <QAction>
#include <QClipboard>
#include <QDir>
#include <QGVDrawItem.h>
#include <QQuickView>
#include <QMessageBox>
#include "on_map_click.h"
#include "QGVMap.h"
#include "QGVImage.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QGVLayer.h>
#include <QPainter>
#include <QGVLayer.h>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QCamera>
#include <QVideoWidget>
#include <QQuickWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include "qbatterywidget.h"
#include <QPushButton> // Добавьте эту строку
#include "samples/placemark.h"


using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : ui(new Ui::MainWindow)


{
    ui->setupUi(this);

    QQuickWidget *camera = new QQuickWidget;
    camera->setSource(QUrl("qrc:/camera.qml"));
    ui->horizontalLayout->addWidget(camera);

    QMetaObject::invokeMethod(this, "init", Qt::QueuedConnection);
    //connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &MainWindow::on_pushButton_8_clicked);
    listWidget = new QListWidget(this);
    listWidget->setFont(QFont("Arial", 12)); // Настройка шрифта
    ui->verticalLayout->addWidget(listWidget);

    // Добавление QListWidget в verticalLayout
    ui->groupBox_2->setHidden(true);
    //ui->groupBox->setHidden(true);

    listWidget_2 = new QListWidget(this);
    listWidget_2->setFont(QFont("Arial", 12)); // Настройка шрифта
    ui->verticalLayout_2->addWidget(listWidget_2);
    ui->lineEdit->hide();
    ui->lineEdit_2->hide();
    ui->widget->setValue(50);
    //  ui->widget->setCharging(true);
    connect(ui->lineEdit, &QLineEdit::editingFinished, this, &MainWindow::on_lineEdit_editingFinished);
    pendingRoute = new QRoute();
    addMarkersEnabled = false;
    addMarkersEnabled_2 = false;
    ui->groupBox->setVisible(false); // Скрыть groupBox при запуске
    struct Route {
        QListWidgetItem* item;
        QRoute* route;
    };
    connect(ui->pushButton_11, &QPushButton::clicked, this, &MainWindow::on_pushButton_11_clicked);
    QMap<int, QGV::GeoPos> coordinatesMap;
    QMap<int, Placemark*> placemarksMap;
    // В классе MainWindow объявите новый список для хранения маршрутов
    QList<QRoute*> routes;
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{

    QDir("cacheDir").removeRecursively();
    mCache = new QNetworkDiskCache(this);
    mCache->setCacheDirectory("cacheDir");
    mManager = new QNetworkAccessManager(this);
    mManager->setCache(mCache);
    QGV::setNetworkManager(mManager);
    DemoItem* item = new BackgroundDemo(ui->geoMap, this);
    DemoItem* mouseItem = new MouseDemo(ui->geoMap, this);
    const auto target =
        QGV::GeoRect(QGV::GeoPos(41.988577, 74.964334), QGV::GeoPos(43.888577, 76.964334));

    item->init();
    mouseItem->init();
    ui->geoMap->cameraTo(QGVCameraActions(ui->geoMap).scaleTo(target));

    drawingLayer= new QGVLayer();
    drawingLayer->setName("Route layer");
    drawingLayer->setDescription("");
    drawingLayer->bringToFront();
    drawingLayer->setVisible(true);

    ui->geoMap->addItem(drawingLayer);

    sloi= new QGVLayer();
    sloi->setName("Route layer");
    sloi->setDescription("");
    sloi->bringToFront();
    sloi->setVisible(true);

    ui->geoMap->addItem(sloi);
    connect(ui->geoMap, static_cast<void (QGVMap::*)(QPointF)>(&QGVMap::mapMousePress), this, &MainWindow::mousePress);
    listWidget = new QListWidget(this);
    listWidget->setFont(QFont("Arial", 12)); // Настройка шрифта
    listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection); // Разрешить выбор нескольких элементов
    ui->verticalLayout->addWidget(listWidget);
}
void MainWindow::mousePress(QPointF coords) {
    if (drawingRoute) {
        auto base = ui->geoMap->getProjection()->projToGeo(coords);
        QPointF realCoords = { base.latitude(), base.longitude() };
        pendingRoute->addPoint(realCoords);
        // Добавление маршрута в список
        //routesList.append(pendingRoute);
        auto based = ui->geoMap->getProjection()->projToGeo(coords);
        Placemark* placemark = new Placemark(QGV::GeoPos(based.latitude(), based.longitude()));
        drawingLayer->addItem(placemark);
        on_pushButton_clicked(coords);
        qInfo() << "Added point: " << realCoords;
        QListWidgetItem* item = new QListWidgetItem();
        item->setTextAlignment(Qt::AlignCenter);
        item->setData(Qt::UserRole, QVariant::fromValue(pendingRoute));

        listWidget->addItem(item);
        update();

    } else if (addMarkersEnabled) {
    auto basede = ui->geoMap->getProjection()->projToGeo(coords);
    QString coordsString = QString("Координаты: %1, %2").arg(basede.latitude()).arg(basede.longitude());

    QListWidgetItem* item = new QListWidgetItem();
    item->setTextAlignment(Qt::AlignCenter);

    QWidget* listItemWidget = new QWidget;
    QHBoxLayout* listItemLayout = new QHBoxLayout(listItemWidget);
    listItemLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* coordsLabel = new QLabel(coordsString);
    coordsLabel->setStyleSheet("background-color: white; padding: 5px;");

    listItemLayout->addWidget(coordsLabel);
    listItemWidget->setLayout(listItemLayout);
    item->setSizeHint(listItemWidget->sizeHint());

    listWidget_2->addItem(item);
    listWidget_2->setItemWidget(item, listItemWidget);

    // Добавить отступ между прямоугольниками
    QSpacerItem* spacerItem = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    listItemLayout->addItem(spacerItem);

    Placemark* placemark = new Placemark(QGV::GeoPos(basede.latitude(), basede.longitude()));
    sloi->addItem(placemark);
    update();
}


else if (addMarkersEnabled_2 && sloi->mItems.isEmpty()) {
    auto basedes = ui->geoMap->getProjection()->projToGeo(coords);
    QString coordsString = QString("Координаты: %1, %2").arg(basedes.latitude()).arg(basedes.longitude());

    QListWidgetItem* item = new QListWidgetItem();
    item->setTextAlignment(Qt::AlignCenter);

    QWidget* listItemWidget = new QWidget;
    QHBoxLayout* listItemLayout = new QHBoxLayout(listItemWidget);
    listItemLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* coordsLabel = new QLabel(coordsString);
    coordsLabel->setStyleSheet("background-color: white; padding: 5px;");

    listItemLayout->addWidget(coordsLabel);
    listItemWidget->setLayout(listItemLayout);
    item->setSizeHint(listItemWidget->sizeHint());

    listWidget_2->addItem(item);
    listWidget_2->setItemWidget(item, listItemWidget);

    // Добавить отступ между прямоугольниками
    QSpacerItem* spacerItem = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    listItemLayout->addItem(spacerItem);

    Placemark* placemark = new Placemark(QGV::GeoPos(basedes.latitude(), basedes.longitude()));
    sloi->addItem(placemark);
    update();

    // Отключение возможности добавления дополнительных меток
    addMarkersEnabled_2 = false;
    int markerId = listWidget_2->count();  // Идентификатор метки
    coordinatesMap.insert(markerId, QGV::GeoPos(basedes.latitude(), basedes.longitude()));
    placemarksMap.insert(markerId, placemark);
}


}
void MainWindow::on_pushButton_5_clicked(QPointF coords)
{
    auto basede = ui->geoMap->getProjection()->projToGeo(coords);
    QString coordsString = QString("Координаты: %1, %2").arg(basede.latitude()).arg(basede.longitude());

    QListWidgetItem* item = new QListWidgetItem();
    item->setTextAlignment(Qt::AlignCenter);

    QWidget* listItemWidget = new QWidget;
    QVBoxLayout* listItemLayout = new QVBoxLayout(listItemWidget);
    listItemLayout->setContentsMargins(0, 0, 0, 0);
    listItemLayout->setSpacing(3); // Отступ между прямоугольниками

    QLabel* coordsLabel = new QLabel(coordsString);
    coordsLabel->setStyleSheet("background-color: white; padding: 5px;");

    listItemLayout->addWidget(coordsLabel);
    listItemWidget->setLayout(listItemLayout);
    item->setSizeHint(listItemWidget->sizeHint());

    listWidget_2->addItem(item);
    listWidget_2->setItemWidget(item, listItemWidget);

    listWidget_2->scrollToBottom();
}

void MainWindow::on_pushButton_6_clicked()
{
    sloiy = !sloiy;
    mediaPlayer = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setSource(QUrl::fromLocalFile("C:/Users/acer/Documents/NanoSpace/home.mp3"));
    audioOutput->setVolume(50);
    mediaPlayer->play();
    if (sloiy) {
        ui->groupBox_2->show();
        addMarkersEnabled_2 = true;
        // Очистка listWidget_2 перед отображением новых координат
        listWidget_2->clear();

        // Получение координат меток и их отображение в listWidget_2
        for (int i = 0; i < sloi->mItems.count(); i++) {
            Placemark* placemark = dynamic_cast<Placemark*>(sloi->mItems.at(i));
            if (placemark) {
                QPointF coords = placemark->mCoords;
                QString coordinateStr = QString::number(coords.x()) + ",lllllll " + QString::number(coords.y());
                QListWidgetItem* item = new QListWidgetItem(coordinateStr);
                listWidget_2->addItem(item);

                // Добавление метки на карту
                drawingLayer->addItem(placemark);
            }
        }

        // Установка флага addMarkersEnabled_2 в true для разрешения добавления новых меток
        addMarkersEnabled_2 = true;
    } else {
        // Скрытие groupBox_2
        ui->groupBox_2->hide();

        // Очистка listWidget_2
        listWidget_2->clear();

        // Удаление всех меток с карты
        drawingLayer->clear();

        // Сброс флага addMarkersEnabled_2, чтобы метки перестали ставиться
        addMarkersEnabled_2 = false;
    }
    ui->groupBox_2->setVisible(sloiy);

}

void MainWindow::on_pushButton_2_clicked()
{

    mediaPlayer = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setSource(QUrl::fromLocalFile("C:/Users/acer/Documents/NanoSpace/fly.mp3"));
    audioOutput->setVolume(50);
    mediaPlayer->play();

}


void MainWindow::on_pushButton_4_clicked()
{
    if(!drawingRoute)
    {
        drawingRoute = true;
        pendingRoute = new QRoute();
        pendingRoute->setVisible(true);
        drawingLayer->addItem(pendingRoute);


    }
    else
        drawingRoute = false;
    mediaPlayer = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    mediaPlayer->setAudioOutput(audioOutput);
    // ...
    mediaPlayer->setSource(QUrl::fromLocalFile("C:/Users/acer/Documents/NanoSpace/mission.mp3"));
    audioOutput->setVolume(50);
    mediaPlayer->play();

}






void MainWindow::on_pushButton_3_clicked()
{
     mediaPlayer = new QMediaPlayer;
     audioOutput = new QAudioOutput;
     mediaPlayer->setAudioOutput(audioOutput);
     // ...
     mediaPlayer->setSource(QUrl::fromLocalFile("C:/Users/acer/Documents/NanoSpace/posad.mp3"));
     audioOutput->setVolume(50);
     mediaPlayer->play();

}


void MainWindow::on_pushButton_9_clicked()
{

     if (ui->lineEdit->isHidden()) {
        // Показываем lineEdit
        ui->lineEdit->show();
     } else {
        // Скрываем lineEdit
        ui->lineEdit->hide();
     }
}



// В методе MainWindow::on_pushButton_8_clicked()
void MainWindow::on_pushButton_8_clicked()
{
     QList<QListWidgetItem*> selectedItems = listWidget->selectedItems();
     for (QListWidgetItem* selectedItem : selectedItems) {
        // Получение указателя на маршрут из элемента списка
        QRoute* route = selectedItem->data(Qt::UserRole).value<QRoute*>();
        if (route) {
            // Удаление маршрута с карты
            drawingLayer->removeItem(route);
            delete route;

            // Удаление элемента списка
            delete listWidget->takeItem(listWidget->row(selectedItem));
        }
     }
}




void MainWindow::on_pushButton_clicked(QPointF coords)
{
    auto basede = ui->geoMap->getProjection()->projToGeo(coords);
    QString coordsString = QString("Координаты: %1, %2").arg(basede.latitude()).arg(basede.longitude());

    // Создание элемента списка
    QListWidgetItem* item = new QListWidgetItem();
    item->setTextAlignment(Qt::AlignCenter);

    QWidget* listItemWidget = new QWidget;
    QVBoxLayout* listItemLayout = new QVBoxLayout(listItemWidget);

    QLabel* coordsLabel = new QLabel(coordsString);
    coordsLabel->setStyleSheet("background-color: white; padding: 5px; margin: 3px;");

    listItemLayout->addWidget(coordsLabel);
    listItemLayout->setContentsMargins(0, 0, 0, 0);
    listItemLayout->setSpacing(0);

    listItemWidget->setLayout(listItemLayout);
    item->setSizeHint(listItemWidget->sizeHint());

    // Сохранение указателя на маршрут
    item->setData(Qt::UserRole, QVariant::fromValue(pendingRoute));

    listWidget->addItem(item);
    listWidget->setItemWidget(item, listItemWidget);

    listWidget->scrollToBottom();
}




void MainWindow::on_pushButton_12_clicked()
{
    if (ui->lineEdit_2->isHidden()) {
        // Показываем lineEdit
        ui->lineEdit_2->show();
    } else {
        // Скрываем lineEdit
        ui->lineEdit_2->hide();
    }
}


//void MainWindow::on_lineEdit_textChanged(const QString &arg1)
//{

//}

void MainWindow::on_lineEdit_editingFinished()
{
    QObject* senderObject = sender();
    if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(senderObject)) {
        QString arg1 = lineEdit->text().trimmed();
        if (!arg1.isEmpty()) {
            QStringList tokens = arg1.split(",");
            if (tokens.size() == 2) {
                QString latString = tokens[0].trimmed();
                QString lonString = tokens[1].trimmed();

                bool conversionOK;
                qreal latitude = latString.toDouble(&conversionOK);

                if (conversionOK) {
                    qreal longitude = lonString.toDouble(&conversionOK);

                    if (conversionOK) {
                        QString coordinatesText = "Координаты: " + latString + ", " + lonString;
                        QPointF point(latitude, longitude);
                        pendingRoute->addPoint(point);

                        // Создать виджет для координат
                        QFrame* frame = new QFrame;
                        frame->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
                        frame->setStyleSheet("background-color: white; margin: 3px;");

                        QVBoxLayout* layout = new QVBoxLayout(frame);

                        QLabel* coordinatesLabel = new QLabel(coordinatesText);
                        coordinatesLabel->setStyleSheet("margin: 0;");

                        layout->addWidget(coordinatesLabel);
                        layout->addStretch();

                        // Добавить виджет с координатами в ListWidget
                        QListWidgetItem* item = new QListWidgetItem();
                        item->setSizeHint(frame->sizeHint());
                        listWidget->addItem(item);
                        listWidget->setItemWidget(item, frame);
                        QGV::GeoPos geoPos(latitude, longitude);
                        Placemark* placemark = new Placemark(geoPos);
                        drawingLayer->addItem(placemark);
                        lineEdit->clear();
                    } else {
                        QMessageBox::critical(this, "Ошибка", "Неверный формат долготы.");
                    }
                } else {
                    QMessageBox::critical(this, "Ошибка", "Неверный формат широты.");
                }
            } else {
                QMessageBox::critical(this, "Ошибка", "Неверный формат координат.");
            }
        }
    }
}


void MainWindow::on_lineEdit_2_returnPressed()
{
    QString coordinates = ui->lineEdit_2->text().trimmed();
    if (!coordinates.isEmpty()) {
        QStringList tokens = coordinates.split(",");
        if (tokens.size() == 2) {
            QString latString = tokens[0].trimmed();
            QString lonString = tokens[1].trimmed();

            bool conversionOK;
            qreal latitude = latString.toDouble(&conversionOK);

            if (conversionOK) {
                qreal longitude = lonString.toDouble(&conversionOK);

                if (conversionOK) {
                    QPointF point(latitude, longitude);

                    // Создайте кастомный виджет для отображения координат
                    QWidget* listItemWidget = new QWidget;
                    QVBoxLayout* listItemLayout = new QVBoxLayout(listItemWidget);

                    QLabel* coordinatesLabel = new QLabel("Координаты: " + coordinates);
                    coordinatesLabel->setStyleSheet("background-color: white; padding: 5px; margin: 3px;");

                    listItemLayout->addWidget(coordinatesLabel);
                    listItemLayout->setContentsMargins(0, 0, 0, 0);
                    listItemLayout->setSpacing(0);

                    listItemWidget->setLayout(listItemLayout);

                    // Создайте элемент списка и установите кастомный виджет
                    QListWidgetItem* item = new QListWidgetItem();
                    item->setSizeHint(listItemWidget->sizeHint());
                    listWidget_2->addItem(item);
                    listWidget_2->setItemWidget(item, listWidget_2);

                    // Создайте метку на карте
                    Placemark* placemark = new Placemark(QGV::GeoPos(latitude, longitude));
                    ui->geoMap->addItem(placemark);

                    ui->lineEdit_2->clear();
                } else {
                    QMessageBox::critical(this, "Ошибка", "Неверный формат долготы.");
                }
            } else {
                QMessageBox::critical(this, "Ошибка", "Неверный формат широты.");
            }
        } else {
            QMessageBox::critical(this, "Ошибка", "Неверный формат координат.");
        }
    }
}

// Пример обновления scaleCoefficient при изменении масштаба
void MainWindow::setScale(double scale)
{
    // Обновите значение scaleCoefficient на основе нового масштаба
    if (pendingRoute != nullptr) {
        pendingRoute->setScaleCoefficient(scale);
    }
    // Другой код для изменения масштаба карты...
}



void MainWindow::on_pushButton_clicked()
{
    bool isVisible = ui->groupBox->isVisible();
    ui->groupBox->setVisible(!isVisible); // Изменить видимость groupBox
}

void MainWindow::on_pushButton_5_clicked()
{
    sloiy = !sloiy;
    mediaPlayer = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setSource(QUrl::fromLocalFile("C:/Users/acer/Documents/NanoSpace/roi.mp3"));
    audioOutput->setVolume(50);
    mediaPlayer->play();
    if (sloiy) {
        // ...
        addMarkersEnabled = true; // Установка флага для разрешения добавления меток

        // Очистка listWidget_2 перед отображением новых координат
        listWidget_2->clear();
    } else {
        // Сброс флага addMarkersEnabled, чтобы метки перестали ставиться
        addMarkersEnabled = false;
    }

    // Изменение видимости groupBox_2 в зависимости от значения флага sloiy
    ui->groupBox_2->setVisible(sloiy);
}


void MainWindow::on_pushButton_11_clicked() {
    // Получение выбранных элементов из listWidget_2
    QList<QListWidgetItem*> selectedItems = listWidget_2->selectedItems();

    foreach (QListWidgetItem* selectedItem, selectedItems) {
        // Получение индекса выбранного элемента
        int row = listWidget_2->row(selectedItem);

        // Удаление элемента из listWidget_2
        listWidget_2->takeItem(row);

        // Удаление соответствующих координат и метки из контейнеров
        if (coordinatesMap.contains(row)) {
            coordinatesMap.remove(row);
        }

        if (placemarksMap.contains(row)) {
            Placemark* placemark = placemarksMap.value(row);
            sloi->removeItem(placemark);  // Удаление метки из QGraphicsView
            delete placemark;  // Освобождение памяти
            placemarksMap.remove(row);
        }

        // Обновление индексов в placemarksMap
        for (int i = row; i < placemarksMap.count(); ++i) {
            placemarksMap[i] = placemarksMap[i + 1];
        }
        placemarksMap.remove(placemarksMap.count());
    }
}


void MainWindow::on_pushButton_7_clicked()
{
    mediaPlayer = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setSource(QUrl::fromLocalFile("C:/Users/acer/Documents/NanoSpace/off.mp3"));
    audioOutput->setVolume(50);
    mediaPlayer->play();
}



