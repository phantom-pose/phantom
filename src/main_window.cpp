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

    BoxNet box = { 100, 100, 100 };
    for (int iz = 0; iz < 100; iz++) {
        for (int iy = 0; iy < 100; iy++) {
            for (int ix = 0; ix < 100; ix++) {
                box.setByXyz(ix, iy, iz, 1);
            }
        }
    }

    // Рисую Layer One
    for (int iz = 15; iz < 55; iz++) {
        for (int iy = 25; iy < 75; iy++) {
            for (int ix = 15; ix < 85; ix++) {
                box.setByXyz(ix, iy, iz, 17);
            }
        }
    }
    // Рисую Layer Two
    for (int iz = 25; iz < 85; iz++) {
        for (int iy = 35; iy < 65; iy++) {
            for (int ix = 25; ix < 35; ix++) {
                box.setByXyz(ix, iy, iz, 77);
            }
        }
    }
    // Рисую Layer Three
    for (int iz = 25; iz < 85; iz++) {
        for (int iy = 35; iy < 65; iy++) {
            for (int ix = 65; ix < 75; ix++) {
                box.setByXyz(ix, iy, iz, 88);
            }
        }
    }
    // Рисую Layer Four
    for (int iz = 30; iz < 50; iz++) {
        for (int iy = 40; iy < 60; iy++) {
            for (int ix = 30; ix < 70; ix++) {
                box.setByXyz(ix, iy, iz, 113);
            }
        }
    }

//    m_phantom = new Phantom( box );

    std::vector <BoundingBox *> costume;
    costume.push_back(new BoundingBox(18.75, 37.5, 25, 150, 112.5, 125, "alpha"));
    costume.push_back(new BoundingBox(37.5, 56.25, 50, 37.5, 75, 175, "beta"));
    costume.push_back(new BoundingBox(112.5, 56.25, 50, 37.5, 75, 175, "gamma"));


    m_phantom = new Phantom();

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
    m_paintAreaX->setProps( 3, 4 );
    m_paintAreaX->setAlignment(Qt::AlignTop);
    m_paintAreaY = new NetPaintArea(0, m_phantom->boxNet());
    m_paintAreaY->setProps( 3, 4 );
    m_paintAreaY->setAlignment(Qt::AlignTop);
    m_paintAreaZ = new NetPaintArea(0, m_phantom->boxNet());
    m_paintAreaZ->setProps( 3, 3 );
    m_paintAreaZ->setAlignment(Qt::AlignTop);

    /////////////////////////////////////////////////////////////////////////


//    CalculationArea area = { m_phantom->boxNet() };

//    std::vector <Line> lines;

//    double * tk = new double [1500];
//    unsigned char * ck = new unsigned char [1500];
//    int k = 0;

//    for (int i = 0; i < 10000; i++) {
//        Line ray = { { 0, 0, -1 }, { 561, 257, 1740 } };
//        int err = area.prepLineOut(ray);
//        area.startIterations(ray, tk, ck, k);
//    }
//    Logger::Instance() << "k = " << k;

//    m_lineArea = new LinePaintArea(0, lines);

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
    upDownSBZ->setMaximum( m_phantom->boxSizeZ() );
    QSpinBox * upDownSBX = new QSpinBox();
    upDownSBX->setMaximum( m_phantom->boxSizeX() );
    QSpinBox * upDownSBY = new QSpinBox();
    upDownSBY->setMaximum( m_phantom->boxSizeY() );

    QPushButton * scenario = new QPushButton("Scen");
    connect(scenario, SIGNAL(clicked()), this, SLOT(loadScenario()));
    QPushButton * setBoxBtnRightKnee = new QPushButton("setBox Right Knee");
    connect(setBoxBtnRightKnee, SIGNAL(clicked()), this, SLOT(setBoxRightKnee()));
    QPushButton * setBoxBtnLeftKnee = new QPushButton("setBox Left Knee");
    connect(setBoxBtnLeftKnee, SIGNAL(clicked()), this, SLOT(setBoxLeftKnee()));

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

    m_tab->addTab(scrollAreaZ, "Horizontal");
    m_tab->addTab(scrollAreaY, "Frontal");
    m_tab->addTab(scrollAreaX, "Profile");

    upDownLayout->addWidget(upDownSBZ);
    upDownLayout->addWidget(upDownSBY);
    upDownLayout->addWidget(upDownSBX);
    upDownLayout->addWidget(mouseLabel);
    upDownLayout->addWidget(scenario);
    upDownLayout->addWidget(setBoxBtnRightKnee);
    upDownLayout->addWidget(setBoxBtnLeftKnee);

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
    BoxNet b1 = m_phantom->boxNet();
    BoxNet b = b1.cut( {148, 43, 175}, {227, 132, 260} );
    b.grow({89,170,130},{0,0,30});
    BoxNet b2 = {89,170,130};

    const float PHI = M_PI/2;

    Plane s1 = {
        {
            1, 4, 115*2.5
        },
        {
            1, 77*1.875, 115*2.5
        },
        {
            76*1.875, 4, 115*2.5
        }
    };
    Plane s2 = {
        {
            4*1.875, 29*1.875, 37*2.5
        },
        {
            70*1.875, 29*1.875, 37*2.5
        },
        {
            4*1.875, 88*1.875, 37*2.5
        }
    };
    Plane e1 = {
        {
            1, 4, 115*2.5
        },
        {
            1, 77*1.875, 115*2.5
        },
        {
            76*1.875, 4, 115*2.5
        }
    };
    Plane e2 = {
            {
                4*1.875, 43*1.875+33*sin(PHI)*2.5-24*cos(PHI)*1.875, (70-33*cos(PHI))*2.5-24*sin(PHI)*1.875
            },
            {
                70*1.875, 43*1.875+33*sin(PHI)*2.5-24*cos(PHI)*1.875, (70-33*cos(PHI))*2.5-24*sin(PHI)*1.875
            },
            {
                4*1.875, 43*1.875+33*sin(PHI)*2.5+45*cos(PHI)*1.875, (70-33*cos(PHI))*2.5+45*sin(PHI)*1.875
            }
        };

    Plane * ep1 = &e1;
    Plane * ep2 = &e2;
    Plane * sp1 = &s1;
    Plane * sp2 = &s2;
    Joint joint = { sp1, sp2, ep1, ep2 };

    int ymin = 0;
    int ymax = int(std::max(88.0, 43+33*sin(PHI)*2.5/1.875+45*cos(PHI))+1);
    int zmin = 0;
    int zmax = int(std::max(115.0, 70-33*cos(PHI)+45*sin(PHI)*1.875/2.5)+1);
    int xmin = 0;
    int xmax = 76;

    for (int iz = zmin; iz < zmax+1; iz++)
    {
        std::cout << iz << std::endl;
        for (int iy = ymin; iy < ymax+1; iy++)
        {
            //std::cout << iz << " " << iy<< std::endl;
            //xmin = 42; xmax=42;
            for (int ix = xmin; ix < xmax+1; ix++)
            {
                //std::cout << iy << " " << ix<< std::endl;
                Point3D <float> end = { (ix+0.5)*1.875, (iy+0.5)*1.875, (iz+0.5)*2.5 };
                Point3D <float> start;
                Point3D <float> * pend = &end;
                Point3D <float> * pstart = &start;
                bool hasDef = joint.getStartPoint( pend, pstart, 3 );
                float x = pstart->x();
                float y = pstart->y();
                float z = pstart->z();
                if (hasDef && x > 0 && x < 89*1.875 && y > 0 && y < 170*1.875 && z > 0 && z < 130*2.5)
                {
                    b2.setByXyz(ix,iy,iz,b.getByXyz(int(x/1.875),int(y/1.875),int(z/2.5)));
                }
                else {
                    b2.setByXyz(ix, iy, iz, 0);
                }
            }
        }
    }
    m_phantom->setBox(b2);
}

void MainWindow::setBoxRightKnee()
{
    BoxNet b1 = m_phantom->boxNet();
    BoxNet b = b1.cut( {66, 43, 175}, {144, 131, 260} );
    b.grow({89,170,130},{0,0,30});
    BoxNet b2 = {89,170,130};

    const float PHI = M_PI/2;

    Plane s1 = {
        {
            0, 0, 115*2.5
        },
        {
            0, 81*1.875, 115*2.5
        },
        {
            77*1.875, 0, 115*2.5
        }
    };
    Plane s2 = {
        {
            8*1.875, 0*1.875, 37*2.5
        },
        {
            73*1.875, 0*1.875, 37*2.5
        },
        {
            8*1.875, 88*1.875, 37*2.5
        }
    };
    Plane e1 = {
        {
            0, 0, 115*2.5
        },
        {
            0, 81*1.875, 115*2.5
        },
        {
            77*1.875, 0, 115*2.5
        }
    };
    Plane e2 = {
            {
                8*1.875, 40*1.875+37*sin(PHI)*2.5-40*cos(PHI)*1.875, (74-37*cos(PHI))*2.5-40*sin(PHI)*1.875
            },
            {
                73*1.875, 40*1.875+37*sin(PHI)*2.5-40*cos(PHI)*1.875, (74-37*cos(PHI))*2.5-40*sin(PHI)*1.875
            },
            {
                8*1.875, 40*1.875+37*sin(PHI)*2.5+48*cos(PHI)*1.875, (74-37*cos(PHI))*2.5+48*sin(PHI)*1.875
            }
        };

    Plane * ep1 = &e1;
    Plane * ep2 = &e2;
    Plane * sp1 = &s1;
    Plane * sp2 = &s2;
    Joint joint = { sp1, sp2, ep1, ep2 };

    int ymin = 0;
    int ymax = int(std::max(88.0, 40+37*sin(PHI)*2.5/1.875+48*cos(PHI))+1);
    int zmin = 0;
    int zmax = int(std::max(115.0, 74-37*cos(PHI)+48*sin(PHI)*1.875/2.5)+1);
    int xmin = 0;
    int xmax = 77;

    for (int iz = zmin; iz < zmax+1; iz++)
    {
        std::cout << iz << std::endl;
        for (int iy = ymin; iy < ymax+1; iy++)
        {
            //std::cout << iz << " " << iy<< std::endl;
            //xmin = 42; xmax=42;
            for (int ix = xmin; ix < xmax+1; ix++)
            {
                //std::cout << iy << " " << ix<< std::endl;
                Point3D <float> end = { (ix+0.5)*1.875, (iy+0.5)*1.875, (iz+0.5)*2.5 };
                Point3D <float> start;
                Point3D <float> * pend = &end;
                Point3D <float> * pstart = &start;
                bool hasDef = joint.getStartPoint( pend, pstart, 3 );
                float x = pstart->x();
                float y = pstart->y();
                float z = pstart->z();
                if (hasDef && x > 0 && x < 89*1.875 && y > 0 && y < 170*1.875 && z > 0 && z < 130*2.5)
                {
                    b2.setByXyz(ix,iy,iz,b.getByXyz(int(x/1.875),int(y/1.875),int(z/2.5)));
                }
                else {
                    b2.setByXyz(ix, iy, iz, 0);
                }
            }
        }
    }
    m_phantom->setBox(b2);
}

void MainWindow::loadScenario()
{
    m_phantom->loadScenario();
}
