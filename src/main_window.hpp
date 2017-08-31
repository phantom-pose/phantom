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

#include "line.h"
#include "linepaintarea.h"
#include "calculationarea.hpp"

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
    void loadScenario();
    void setBox();
private:
    NetPaintArea *m_paintAreaX, *m_paintAreaY, *m_paintAreaZ;
    LinePaintArea *m_lineArea;
    Phantom * m_phantom;
    QTabWidget * m_tab;
    QWidget * scenario;
};
