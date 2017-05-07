#include "main_window.hpp"

MainWindow::MainWindow()
{
    setWindowTitle("phantom");
    createMainArea();
}

void MainWindow::createMainArea()
{
    QWidget * paintMainWidget = new QWidget(this);
    setCentralWidget(paintMainWidget);

    QGridLayout * mainLayout = new QGridLayout(paintMainWidget);
    QVBoxLayout * upDownLayout = new QVBoxLayout();

    QPushButton * upButton = new QPushButton("Up");
    QPushButton * downButton = new QPushButton("Down");
    m_netPaintArea = new NetPaintArea();

    upDownLayout->addWidget(upButton);
    upDownLayout->addWidget(downButton);
    mainLayout->addLayout(upDownLayout, 0, 0, Qt::AlignTop);
    mainLayout->addWidget(m_netPaintArea, 0, 1);
    mainLayout->setColumnStretch(1, 10);
}
