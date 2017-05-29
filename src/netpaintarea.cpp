#include "netpaintarea.h"
#include "logger.h"

NetPaintArea::NetPaintArea(QWidget *parent) : QWidget(parent)
{
    m_boxNet.fillFromBin("data/AF_bin.dat");
    fillPalette();
}

void NetPaintArea::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    unsigned char value;

    Slice slice = m_boxNet.getSliceZ(m_sliceNum);

    for (int iy = 0; iy < m_boxNet.getSizeY(); iy++)
    {
        for (int ix = 0; ix < m_boxNet.getSizeX(); ix++)
        {
            value = slice.getValue(ix, iy);
            painter.fillRect(X_SCALE * ix, Y_SCALE * iy, X_SCALE, Y_SCALE, m_palette[value]);
        }
    }
}

void NetPaintArea::setSliceNum(int sliceNum)
{
    m_sliceNum = sliceNum;
    update();
}

void NetPaintArea::fillPalette()
{
    FILE *fp;
    fp = fopen("data/palette.dat", "r");
    char line[20];
    int id, color;
    if(fp != NULL)
    {
        while (fgets ( line, 14, fp ))
        {
            if (sscanf(line, "%d%x", &id, &color))
            {
                m_palette[id] = QColor(color);
            }
        }
        fclose(fp);
    } else {
        printf("Cant open file\n");
    }
}
