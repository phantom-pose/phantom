#include "main_window.hpp"
#include "bodypart.h"
#include <iostream>

using namespace std;

/*!
 * \brief Конструктор MainWindow
 * Вызывает функцию создания основной области
 */

MainWindow::MainWindow()
{
    setWindowTitle("phantom");
    m_phantom = new Phantom();
//    m_phantom->combineBin("data/bodyparts/leftHand-1.bin", "data/bodyparts/leftHand-2.bin", "data/bodyparts/leftHand-3.bin", "data/bodyparts/leftHand.bin");
//    m_phantom->combineBin("data/bodyparts/rightHand-1.bin", "data/bodyparts/rightHand-2.bin", "data/bodyparts/rightHand-3.bin", "data/bodyparts/rightHand.bin");
//    m_phantom->combineBin("data/bodyparts/leftLeg-1.bin", "data/bodyparts/leftLeg-2.bin", "data/bodyparts/leftLeg-3.bin", "data/bodyparts/leftLeg.bin");
//    m_phantom->combineBin("data/bodyparts/rightLeg-1.bin", "data/bodyparts/rightLeg-2.bin", "data/bodyparts/rightLeg-3.bin", "data/bodyparts/rightLeg.bin");
//    m_phantom = new Phantom( box );
//    m_phantom->grow( { 200, 200, 200 }, { 50, 50, 50 } );


//    m_phantom->pickLeftLeg();
//    m_phantom->pickRightHand();
//    m_phantom->pickLeftHand();
//    m_phantom->pickRightLeg();
//    m_phantom->readBin("rightLeg.bin");
//    m_phantom->cutBin("rightHand.bin", 451, 498);
//    m_phantom->cutBin("leftHand.bin", 457, 508);
//    m_phantom->checkBin("data/bodyparts/leftLeg-1.bin");
//    m_phantom->cutBin("rightLeg.bin", 177, 256);
//    m_phantom->cutBin("leftLeg.bin", 177, 256);

//    m_phantom->readBin("leftHand.bin");
//    m_phantom->pickLeftHand();

//    m_phantom->pickRightHand();
//    m_phantom->readBin("rightHand.bin");

//    m_phantom->pickLeftLeg();
//    m_phantom->readBin("leftLeg.bin");
//    Point3D <int> sizes = { 500, 300, 800 };
//    Point3D <int> position = { 42, 13, 62 };
//    m_phantom->transliterate(sizes, position);

//    m_phantom->pickLegs();
//    m_phantom->readBin("rightLeg.bin");
    m_paintAreaX = new NetPaintArea(0, m_phantom->boxNet());
    m_paintAreaX->setProps( m_phantom->yScale(), m_phantom->zScale() );
    m_paintAreaX->setAlignment(Qt::AlignTop);
    m_paintAreaY = new NetPaintArea(0, m_phantom->boxNet());
    m_paintAreaY->setProps( m_phantom->xScale(), m_phantom->zScale() );
    m_paintAreaY->setAlignment(Qt::AlignTop);
    m_paintAreaZ = new NetPaintArea(0, m_phantom->boxNet());
    m_paintAreaZ->setProps( m_phantom->xScale(), m_phantom->yScale() );
    m_paintAreaZ->setAlignment(Qt::AlignTop);

    /////////////////////////////////////////////////////////////////////////
    BoxNet box = { 100, 100, 100 };
    for (int iz = 0; iz < 100; iz++) {
        for (int iy = 0; iy < 100; iy++) {
            for (int ix = 0; ix < 100; ix++) {
                int x = ix - 50;
                int y = iy - 50;
                float hyp = sqrt(x*x + y*y);

                if (hyp < 20) {
                    box.setByXyz(ix, iy, iz, 141);
                } else if (hyp < 25 && hyp >= 20) {
                    box.setByXyz(ix, iy, iz, 50);
                }
            }
        }
    }

    CalculationArea area = { m_phantom->boxNet() };

    std::vector <Line> lines;
//    for (int i = 0; i < 185; i++) {
//        Line ray = { { float(i), -10, 50 }, { 0, 3, 0 } };
//        int err = area.prepLineOut(ray);
//        if (!err) {
//            area.startIterations(ray);
//            lines.push_back(ray);
//        }
//    }

    double * tk = new double [1500];
    unsigned char * ck = new unsigned char [1500];
    int k = 0;

    for (int i = 0; i < 10000; i++) {
        Line ray = { { 0, 0, -1 }, { 561, 257, 1740 } };
        int err = area.prepLineOut(ray);
        area.startIterations(ray, tk, ck, k);
    }
//    lines.push_back(ray);
    Logger::Instance() << "k = " << k;

    m_lineArea = new LinePaintArea(0, lines);

    createMainArea();
}

/*!
 * \brief Функция создания основной области
 * Создаёт и располагает виждеты внутри основного окна
 */

void MainWindow::createMainArea()
{
    QWidget * paintMainWidget = new QWidget(this);
    setCentralWidget(paintMainWidget);
    m_tab = new QTabWidget(this);

    QGridLayout * mainLayout = new QGridLayout(paintMainWidget);
    QVBoxLayout * upDownLayout = new QVBoxLayout();

    QSpinBox * upDownSBZ = new QSpinBox();
    upDownSBZ->setMaximum( 500 );
    QSpinBox * upDownSBX = new QSpinBox();
    upDownSBX->setMaximum( 500 );
    QSpinBox * upDownSBY = new QSpinBox();
    upDownSBY->setMaximum( 500 );

    QPushButton * scenario = new QPushButton("Scen");
    connect(scenario, SIGNAL(clicked()), this, SLOT(executeScenario()));
    QPushButton * setBoxBtnRightKnee = new QPushButton("setBox Right Knee");
    connect(setBoxBtnRightKnee, SIGNAL(clicked()), this, SLOT(setBoxRightKnee()));
    QPushButton * setBoxBtnLeftKnee = new QPushButton("setBox Left Knee");
    connect(setBoxBtnLeftKnee, SIGNAL(clicked()), this, SLOT(setBoxLeftKnee()));
    QPushButton * setBoxBtnRightElbow = new QPushButton("setBox RightElbow");
    connect(setBoxBtnRightElbow, SIGNAL(clicked()), this, SLOT(setBoxRightElbow()));
    QPushButton * setBoxBtnRightHip = new QPushButton("setBox RightHip");
    connect(setBoxBtnRightHip, SIGNAL(clicked()), this, SLOT(setBoxRightHip()));

    connect(upDownSBZ, SIGNAL( valueChanged(int) ),  m_paintAreaZ, SLOT( paintZ(int) ));
    connect(upDownSBX, SIGNAL( valueChanged(int) ),  m_paintAreaX, SLOT( paintX(int) ));
    connect(upDownSBY, SIGNAL( valueChanged(int) ),  m_paintAreaY, SLOT( paintY(int) ));

    QLabel * mouseLabel = new QLabel();

    connect(m_paintAreaZ, SIGNAL( mouseChanged(QString) ), mouseLabel, SLOT( setText(QString) ));
    connect(m_paintAreaX, SIGNAL( mouseChanged(QString) ), mouseLabel, SLOT( setText(QString) ));
    connect(m_paintAreaY, SIGNAL( mouseChanged(QString) ), mouseLabel, SLOT( setText(QString) ));


    QScrollArea * scrollAreaZ = new QScrollArea();
    scrollAreaZ->setWidget(m_paintAreaZ);
    scrollAreaZ->setBackgroundRole(QPalette::Dark);
    scrollAreaZ->setWidgetResizable( true );

    QScrollArea * scrollAreaX = new QScrollArea();
    scrollAreaX->setWidget(m_paintAreaX);
    scrollAreaX->setBackgroundRole(QPalette::Dark);
    scrollAreaX->setWidgetResizable( true );

    QScrollArea * scrollAreaY = new QScrollArea();
    scrollAreaY->setWidget(m_paintAreaY);
    scrollAreaY->setBackgroundRole(QPalette::Dark);
    scrollAreaY->setWidgetResizable( true );

    QScrollArea * scrollLinesArea = new QScrollArea();
    scrollLinesArea->setWidget(m_lineArea);
    scrollLinesArea->setBackgroundRole(QPalette::Dark);
    scrollLinesArea->setWidgetResizable( true );

    m_tab->addTab(scrollAreaZ, "Horizontal");
    m_tab->addTab(scrollAreaY, "Frontal");
    m_tab->addTab(scrollAreaX, "Profile");
    m_tab->addTab(scrollLinesArea, "Lines");

    upDownLayout->addWidget(upDownSBZ);
    upDownLayout->addWidget(upDownSBY);
    upDownLayout->addWidget(upDownSBX);
    upDownLayout->addWidget(mouseLabel);
    upDownLayout->addWidget(scenario);
    upDownLayout->addWidget(setBoxBtnRightKnee);
    upDownLayout->addWidget(setBoxBtnLeftKnee);
    upDownLayout->addWidget(setBoxBtnRightElbow);
    upDownLayout->addWidget(setBoxBtnRightHip);

    mainLayout->addLayout(upDownLayout, 0, 0, Qt::AlignTop);
    mainLayout->addWidget(m_tab, 0, 1);
    mainLayout->setColumnStretch(1, 10);
}

void MainWindow::keyPressEvent( QKeyEvent * event )
{
    if (event->key() == Qt::Key_Plus)
    {
        if ( m_tab->currentIndex() == 0 ) {
            m_paintAreaZ->incScale();
        }
        else if ( m_tab->currentIndex() == 1 ) {
            m_paintAreaY->incScale();
        }
        else if ( m_tab->currentIndex() == 2 ) {
            m_paintAreaX->incScale();
        }
        else if ( m_tab->currentIndex() == 3 ) {
            m_lineArea->incScale();
        }
    }
    if (event->key() == Qt::Key_Minus)
    {
        if ( m_tab->currentIndex() == 0 ) {
            m_paintAreaZ->decScale();
        }
        else if ( m_tab->currentIndex() == 1 ) {
            m_paintAreaY->decScale();
        }
        else if ( m_tab->currentIndex() == 2 ) {
            m_paintAreaX->decScale();
        }
        else if ( m_tab->currentIndex() == 3 ) {
            m_lineArea->decScale();
        }
    }
    if (event->key() == Qt::Key_S) {
        std::cout << "press save";
        if ( m_tab->currentIndex() == 0 ) {
            m_paintAreaZ->saveImage();
        }
        else if ( m_tab->currentIndex() == 1 ) {
            m_paintAreaY->saveImage();
        }
        else if ( m_tab->currentIndex() == 2 ) {
            m_paintAreaX->saveImage();
        }
    }
}

void MainWindow::showScenario()
{
    scenario = new QWidget;
    QGridLayout * scGrid = new QGridLayout(scenario);
    QHBoxLayout * mainScLo = new QHBoxLayout();
    QPushButton * addButton = new QPushButton();

    Json::Value val;
    std::ifstream file("data/rotPoints.json", std::ifstream::binary);
    file >> val;
    cout << val;
    file.close();

//    mainScLo->addWidget(addButton);
//    mainScLo->addLayout(scGrid);
    scenario->show();
}

void MainWindow::setBoxLeftKnee()
{
    Point3D <int> point;
    BoxNet b = LeftKnee(m_phantom->boxNet(), M_PI/2, M_PI/2, &point);
    m_phantom->setBox(b);
}

void MainWindow::setBoxRightKnee()
{
    Point3D <int> point;
    BoxNet b = RightKnee(m_phantom->boxNet(), 0, -M_PI/2, &point);
    m_phantom->setBox(b);
}

void MainWindow::setBoxRightElbow()
{
    Point3D <int> point;
    BoxNet b = RightElbow(m_phantom->boxNet(), M_PI/2, 0, 0, &point);
    m_phantom->setBox(b);
}

void MainWindow::setBoxRightHip()
{
    Point3D <int> point;
    BoxNet b = RightHip(m_phantom->boxNet(), M_PI/7, &point);
    m_phantom->setBox(b);
}

void MainWindow::executeScenario()
{
    m_phantom->executeScenario();
}
