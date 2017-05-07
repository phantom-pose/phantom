#include "netpaintarea.h"
#include "logger.h"

NetPaintArea::NetPaintArea(QWidget *parent) : QWidget(parent)
{

}

void NetPaintArea::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);

    int sliceX = m_boxNet.getSizeX();
    int sliceY = m_boxNet.getSizeY();

    for (int j = 0; j < sliceY; j++) {
        for (int i = 0; i < sliceX; i++) {
            Logger::Instance() << i << "\n";
            QRect rectangle = { i * X_SCALE, j * Y_SCALE, X_SCALE, Y_SCALE };
            painter.drawRect(rectangle);
        }
    }
}
