#pragma once

#include <QMainWindow>
#include <QApplication>
#include <QObject>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QtWidgets>

#include "netpaintarea.h"
#include "logger.h"

/*!
 * \brief Класс MainWindow - основное окно программы
 * Содержит виждеты для управления программой и задаёт их параметры
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
private slots:
    void createMainArea();
private:
    NetPaintArea * m_netPaintArea;
};
