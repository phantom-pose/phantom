#include "netpaintarea.h"
#include "logger.h"

NetPaintArea::NetPaintArea(QWidget *parent) : QWidget(parent)
{
    m_boxNet.fillFromBin("data/AF_bin.dat");
}

void NetPaintArea::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);

//    int sliceX = m_boxNet.getSizeX();
//    int sliceY = m_boxNet.getSizeY();

//    for (int j = 0; j < sliceY; j++) {
//        for (int i = 0; i < sliceX; i++) {
//            QRect rectangle = { i * X_SCALE, j * Y_SCALE, X_SCALE, Y_SCALE };
//            painter.drawRect(rectangle);
//        }
//    }


    unsigned char ** slice = m_boxNet.getSliceZ(30);

    for (int iy = 0; iy < m_boxNet.getSizeY(); iy++)
    {
        for (int ix = 0; ix < m_boxNet.getSizeX(); ix++)
        {
            if (slice[iy][ix] != 0)
            {
                painter.drawRect(X_SCALE * ix, Y_SCALE * iy, X_SCALE, Y_SCALE);
            }
        }
        delete slice[iy];
    }
    delete slice;
}
