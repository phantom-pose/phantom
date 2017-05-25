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

//    QPushButton * upButton = new QPushButton("Up");
//    QPushButton * downButton = new QPushButton("Down");
    QSpinBox * upDownSB = new QSpinBox();
    m_netPaintArea = new NetPaintArea();
    connect(upDownSB, SIGNAL( valueChanged(int) ),  m_netPaintArea, SLOT( setSliceNum(int) ));

//    upDownLayout->addWidget(upButton);
//    upDownLayout->addWidget(downButton);
    upDownLayout->addWidget(upDownSB);
    mainLayout->addLayout(upDownLayout, 0, 0, Qt::AlignTop);
    mainLayout->addWidget(m_netPaintArea, 0, 1);
    mainLayout->setColumnStretch(1, 10);
}
