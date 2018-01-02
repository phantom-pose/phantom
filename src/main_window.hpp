#pragma once

#include <QMainWindow>
#include <QApplication>
#include <QObject>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QtWidgets>
#include <QKeyEvent>

#include "netpaintarea.h"
#include "logger.h"
#include "phantom.h"
#include "point3d.hpp"

#include "json/json.h"
#include <fstream>
#include <string>
#include "knee.h"
#include "elbow.h"
#include "hip.h"
#include "plane.h"

#include "line.h"
#include "calculationarea.hpp"
#include <ctime>
#include <stdio.h>
#include "timetest.h"

/*!
 * \brief Класс MainWindow - основное окно программы
 * Содержит виждеты для управления программой и задаёт их параметры
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    void keyPressEvent( QKeyEvent * event );

private slots:
    void createMainArea();
    void showScenario();
    void executeScenario();
    void setBoxLeftKnee();
    void setBoxRightKnee();
    void setBoxRightElbow();
    void setBoxRightHip();
private:
    NetPaintArea *m_paintAreaX, *m_paintAreaY, *m_paintAreaZ;
    Phantom * m_phantom;
    QTabWidget * m_tab;
    QWidget * scenario;
};
