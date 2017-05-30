#include "main_window.hpp"

/*!
 * \brief Конструктор MainWindow
 * Вызывает функцию создания основной области
 */

MainWindow::MainWindow()
{
    setWindowTitle("phantom");
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

    QGridLayout * mainLayout = new QGridLayout(paintMainWidget);
    QVBoxLayout * upDownLayout = new QVBoxLayout();

    QSpinBox * upDownSB = new QSpinBox();

    m_netPaintArea = new NetPaintArea();
    connect(upDownSB, SIGNAL( valueChanged(int) ),  m_netPaintArea, SLOT( setSliceNum(int) ));

    upDownLayout->addWidget(upDownSB);
    mainLayout->addLayout(upDownLayout, 0, 0, Qt::AlignTop);
    mainLayout->addWidget(m_netPaintArea, 0, 1);
    mainLayout->setColumnStretch(1, 10);
}
