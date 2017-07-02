#include "main_window.hpp"

/*!
 * \brief Конструктор MainWindow
 * Вызывает функцию создания основной области
 */

MainWindow::MainWindow()
{
    setWindowTitle("phantom");
    m_phantom = new Phantom();

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

    QSpinBox * testSB = new QSpinBox();

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
