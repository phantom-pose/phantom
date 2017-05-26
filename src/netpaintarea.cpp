#include "netpaintarea.h"
#include "logger.h"

NetPaintArea::NetPaintArea(QWidget *parent) : QWidget(parent)
{
    m_boxNet.fillFromBin("data/AF_bin.dat");
}

void NetPaintArea::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);

//    unsigned char ** slice = m_boxNet.getSliceZ(m_sliceNum);
//    slice = m_boxNet.getSliceZ(m_sliceNum);

//    for (int iy = 0; iy < m_boxNet.getSizeY(); iy++)
//    {
//        for (int ix = 0; ix < m_boxNet.getSizeX(); ix++)
//        {
//            if (slice.getValue(ix, iy) != 0)
//            {
//                painter.drawRect(X_SCALE * ix, Y_SCALE * iy, X_SCALE, Y_SCALE);
//            }
//        }
//    }
}

void NetPaintArea::setSliceNum(int sliceNum)
{
    m_sliceNum = sliceNum;
    update();
}
