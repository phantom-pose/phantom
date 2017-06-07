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
private:
    NetPaintArea *m_paintAreaX, *m_paintAreaY, *m_paintAreaZ;
    Phantom * m_phantom;
    QTabWidget * m_tab;
};
