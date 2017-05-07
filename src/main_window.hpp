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
