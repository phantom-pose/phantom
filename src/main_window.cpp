#include "main_window.hpp"

/*!
 * \brief Конструктор MainWindow
 * Вызывает функцию создания основной области
 */

MainWindow::MainWindow()
{
    setWindowTitle("phantom");
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
    upDownSBZ->setMaximum( m_phantom->boxSizeZ() - 1 );
    QSpinBox * upDownSBX = new QSpinBox();
    upDownSBX->setMaximum( m_phantom->boxSizeX() - 1 );
    QSpinBox * upDownSBY = new QSpinBox();
    upDownSBY->setMaximum( m_phantom->boxSizeY() - 1 );

    QPushButton * scenario = new QPushButton("Scenario");
    connect(scenario, SIGNAL(clicked()), this, SLOT(showScenario()));
    QPushButton * setBoxBtn = new QPushButton("setBox");
    connect(setBoxBtn, SIGNAL(clicked()), this, SLOT(setBox()));

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

    mainLayout->addLayout(upDownLayout, 0, 0, Qt::AlignTop);
    mainLayout->addWidget(m_tab, 0, 1);
    mainLayout->addWidget(scenario, 0, 0);
    mainLayout->addWidget(setBoxBtn, 0, 0);
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

void MainWindow::setBox()
{
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
    box.grow( { 200, 200, 200 }, { 50, 50, 50 } );
    m_phantom->setBox(box);

    /* //3D shoulder rotation
    Plane s1 = { { 76 * 1.875, 76 * 1.875, 50 * 2.5 }, { 124 * 1.875, 76 * 1.875, 50 * 2.5 }, { 76 * 1.875, 124 * 1.875, 50 * 2.5 } };
    Plane e1 = { { 76 * 1.875, -(124-76)*0.5*0.866 - 45 * 2.5 * 0.5 + 76 * 1.875, 50 * 2.5 + 45 * 2.5 * 0.1339 + (124-76)/4 }, { 124 * 1.875, -(124-76)*0.5*0.866 - 45 * 2.5 * 0.5 + 76 * 1.875, 50 * 2.5 + 45 * 2.5 * 0.1339 + (124-76)/4 }, { 76 * 1.875, (124-76)*0.5*0.866 - 45 * 2.5 * 0.5 + 76 * 1.875, 50 * 2.5 + 45 * 2.5 * 0.1339 - (124-76)/4 } };
    Plane s2 = { { 76 * 1.875, 76 * 1.875, 50 * 2.5 }, { 76 * 1.875, 124 * 1.875, 50 * 2.5 }, { 76 * 1.875, 76 * 1.875, 140 * 2.5 } };
    Plane e2 = { { 76 * 1.875, 76 * 1.875, 50 * 2.5 }, { 76 * 1.875, 124 * 1.875, 50 * 2.5 }, { 76 * 1.875, 76 * 1.875, 140 * 2.5 } };
    */
    /* //2D shoulder rotation
    Plane s1 = {
        {
            76 * 1.875,
            76 * 1.875,
            50 * 2.5
        },
        {
            124 * 1.875,
            76 * 1.875,
            50 * 2.5
        },
        {
            76 * 1.875,
            124 * 1.875,
            50 * 2.5
        }
    };
    Plane e1 = {
        {
            76 * 1.875 + 2.5*45/2 + 1.875*(124-76)*(1-0.866)/2,
            76 * 1.875,
            50 * 2.5 + 2.5*45*(1-0.866) - 1.875*(124-76)/4
        },
        {
            76 * 1.875 + 2.5*45/2 + 1.875*(124-76)*(1+0.866)/2,
            76 * 1.875,
            50 * 2.5 + 2.5*45*(1-0.866) + 1.875*(124-76)/4
        },
        {
            76 * 1.875 + 2.5*45/2 + 1.875*(124-76)*(1-0.866)/2,
            124 * 1.875,
            50 * 2.5 + 2.5*45*(1-0.866) - 1.875*(124-76)/4
        }
    };
    Plane s2 = {
        {
            76 * 1.875,
            76 * 1.875,
            50 * 2.5
        },
        {
            76 * 1.875,
            124 * 1.875,
            50 * 2.5
        },
        {
            76 * 1.875,
            76 * 1.875,
            140 * 2.5
        }
    };
    Plane e2 = {
        {
            76 * 1.875,
            76 * 1.875,
            50 * 2.5
        },
        {
            76 * 1.875,
            124 * 1.875,
            50 * 2.5
        },
        {
            76 * 1.875,
            76 * 1.875,
            140 * 2.5
        }
    };*/

   /* Plane s2 = {
        {
            50, 50, 50
        },
        {
            50, 150, 50
        },
        {
            150, 50, 50
        }
    };
    Plane s1 = {
        {
            50, 50, 150
        },
        {
            150, 50, 150
        },
        {
            50, 150, 150
        }
    };
    Plane e2 = {
        {
            81.7, 50, 31.7
        },
        {
            81.7, 150, 31.7
        },
        {
            168.3, 50, 81.7
        }
    };
    Plane e1 = {
        {
            50, 50, 150
        },
        {
            150, 50, 150
        },
        {
            50, 150, 150
        }
    };*/

    Plane s1 = {
            {
                50, 50, 50
            },
            {
                150, 50, 50
            },
            {
                50, 150, 50
            }
        };
    Plane s2 = {
        {
            50,50,50
        },
        {
            50, 150, 50
        },
        {
            50, 50, 150
        }
    };
    Plane e1 = {
            {
                50, 50, 50
            },
            {
                150, 50, 50
            },
            {
                50, 150, 50
            }
        };
    Plane e2 = {
        {
            50,50,50
        },
        {
            50, 150, 50
        },
        {
            50, 50, 150
        }
    };
    Plane * ep1 = &e1;
    Plane * ep2 = &e2;
    Plane * sp1 = &s1;
    Plane * sp2 = &s2;
    Joint joint = { sp1, sp2, ep1, ep2 };

    Point3D <float> end = { 110, 110, 71.6 };
    Point3D <float> start;
    Point3D <float> * pend = &end;
    Point3D <float> * pstart = &start;

    bool hasDef = joint.getStartPoint( pend, pstart );
    std::cout << hasDef << std::endl;
    std::cout << start << std::endl;

    /*for (int iz = 100; iz < 101; iz++)
    {
        std::cout << iz << std::endl;
        for (int iy = 0; iy < 200; iy++)
        {
            for (int ix = 0; ix < 200; ix++)
            {
                Point3D <float> end = { ix+0.5, iy+0.5, iz+0.5 };
                Point3D <float> start;
                Point3D <float> * pend = &end;
                Point3D <float> * pstart = &start;
                bool hasDef = joint.getStartPoint( pend, pstart );
                if (hasDef)
                {
                    float x = pstart->x();
                    float y = pstart->y();
                    float z = pstart->z();
                    //std::cout << ix << " " << iy << " " << iz << " "<<x<<" "<<y<<" "<<z<< std::endl;
                    if ((x-100)*(x-100) + (y-100)*(y-100) < 400 && z >= 50 && z <= 150)
                        box.setByXyz(ix, iy, iz, 141);
                    else if ((x-100)*(x-100) + (y-100)*(y-100) < 625 && z >= 50 && z <= 150)
                        box.setByXyz(ix, iy, iz, 50);
                    else
                        box.setByXyz(ix, iy, iz, 0);
                }
                else {
                    box.setByXyz(ix, iy, iz, 0);
                }
            }
        }
    }
    m_phantom->setBox(box);*/
}
