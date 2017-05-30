#include "netpaintarea.h"
#include "logger.h"

/*!
 * \brief Конструктор NetPaintArea
 * \param parent Родительский виджет
 * При создании области отрисовки вызывается функция заполнения палитры из файла
 * \todo На данный момент класс и конструкто содержит BoxNet, впоследствии будет убрано
 */

NetPaintArea::NetPaintArea(QWidget *parent) : QWidget(parent)
{
    m_boxNet.fillFromBin("data/AF_bin.dat");
    fillPalette();
}

/*!
 * \brief Метод отрисовки, вызывается при вызове update()
 * \todo На данный момент отрисовывает Slice вдоль Z, впоследствии slice станет полем класса
 */

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

/*!
 * \brief Установка значения m_sliceNum, метод будет вскоре переработан
 * \param sliceNum Положение среза вдоль оси
 * \todo В связи с переработкой класса он будет привязан к определенному Slice, а не BoxNet.
 */

void NetPaintArea::setSliceNum(int sliceNum)
{
    m_sliceNum = sliceNum;
    update();
}

/*!
 * \brief Метод заполнения палитры
 * Заполняет палитру из файла data/palette.dat
 */

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

/*!
 * \brief Метод установки среза
 * \param slice Срез, который затем рисует данный класс
 */

void NetPaintArea::setSlice(Slice slice)
{
    m_slice = std::move(slice);
}
