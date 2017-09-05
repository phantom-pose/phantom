#include "main_window.hpp"
#include <qdesktopwidget.h>
#include "joint.h"

/*!
 * \brief main - основная функция программы
 * Отсюда происходит вызов основного окна - MainWindow и задание его начальных параметров
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mw;
    mw.resize(800,600);
    mw.move(QApplication::desktop()->screen()->rect().center() - mw.rect().center());
    mw.show();
    return a.exec();
}
