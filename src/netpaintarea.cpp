#include "netpaintarea.h"
#include "logger.h"

/*!
 * \brief Конструктор NetPaintArea
 * \param parent Родительский виджет
 * При создании области отрисовки вызывается функция заполнения палитры из файла
 * \todo На данный момент класс и конструкто содержит BoxNet, впоследствии будет убрано
 */

NetPaintArea::NetPaintArea(QWidget *parent)
    : QLabel(parent)
{
    fillPalette();
}

NetPaintArea::NetPaintArea(QWidget *parent, BoxNet const & boxNet)
    : QLabel(parent), m_boxNet(&boxNet)
{
    fillPalette();
}

/*!
 * \brief Метод отрисовки, вызывается при вызове update()
 */

void NetPaintArea::paintEvent(QPaintEvent * event)
{
    unsigned char value;
    int width = (int)(m_wProp * m_slice.getSizeX());
    int height = (int)(m_hProp * m_slice.getSizeY());

    QPixmap pixmap = { width, height };
    QPainter painter(&pixmap);

    for (int iy = 0; iy < m_slice.getSizeY(); iy++)
    {
        for (int ix = 0; ix < m_slice.getSizeX(); ix++)
        {
            value = m_slice.getValue(ix, iy);
            painter.fillRect( m_wProp * ix, m_hProp * (m_slice.getSizeY() - iy - 1), m_wProp, m_hProp, m_palette[value] );
        }
    }

    int h = height * m_scale;
    int w = width * m_scale;

    setPixmap(pixmap.scaled( w, h ));
    QLabel::paintEvent(event);
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

void NetPaintArea::paintX(int sliceNum)
{
    m_slice = m_boxNet->getSliceX(sliceNum);
    update();
}

void NetPaintArea::paintY(int sliceNum)
{
    m_slice = m_boxNet->getSliceY(sliceNum);
    update();
}

void NetPaintArea::paintZ(int sliceNum)
{
    m_slice = m_boxNet->getSliceZ(sliceNum);
    update();
}

void NetPaintArea::setScales(float w, float h)
{
    (h > w) ? m_hScale = h/w : m_wScale = w/h;
}

void NetPaintArea::setProps(float w, float h)
{
    // будем искать приближенные целые значения для пропорций
    // На попытку нахождения нужной точности будет 7 итераций
    float eps = 0.1;
    float attitude = h / w;
    for (int i = 1; i < 8; i++) {
        float cur_attitude = attitude * i;
        int bot_edge = int(cur_attitude);
        int top_edge = int(cur_attitude + 1);
//        std::cout << "bot_edge = " << bot_edge << "top_edge = " << top_edge << std::endl;
        float bot_diff = cur_attitude - bot_edge;
        if (bot_diff < eps) {
            m_wProp = i;
            m_hProp = bot_edge;
            break;
        }
        float top_diff = top_edge - cur_attitude;
        if (top_diff < eps) {
            m_wProp = i;
            m_hProp = top_edge;
            break;
        }
    }
//    std::cout << "Выставлены значения w = " << m_wProp << " h = " << m_hProp << std::endl;
}

void NetPaintArea::setScale(float scale)
{
    m_scale = scale;
}

void NetPaintArea::incScale()
{
    m_scale += 0.1;

//    int width = m_wProp * m_slice.getSizeX();
//    int height = m_hProp * m_slice.getSizeY();

//    QPixmap pixmap = { width, height };
//    QPainter painter(&pixmap);
//    painter.fillRect( 0, 0, width, height, m_palette[0] );
//    int h = height * m_scale;
//    int w = width * m_scale;

//    setPixmap(pixmap.scaled( w, h ));
}

void NetPaintArea::decScale()
{
    m_scale -= 0.1;
}

void  NetPaintArea::mousePressEvent(QMouseEvent *e)
{
    int x = e->x();
    int y = e->y();

    float width = m_wProp * m_scale;
    float height = m_hProp * m_scale;

    int _x = x/width;
    int _y = m_slice.getSizeY() - y/height;

    QString str = QString("%1:%2").arg(_x).arg(_y);
    emit mouseChanged(str);
}

void NetPaintArea::saveImage()
{
    std::cout << "Inside saveimage" << "\n";
    QPixmap const * px = pixmap();
    QDateTime time = QDateTime::currentDateTime();
    QString _time = time.toString();
    _time.append(".png");
    QFile file(_time);
    file.open(QIODevice::WriteOnly);

    unsigned char value;
    int width = (int)(m_wProp * m_slice.getSizeX());
    int height = (int)(m_hProp * m_slice.getSizeY());

    QPixmap pixmap = { width, height };
    QPainter painter(&pixmap);

    for (int iy = 0; iy < m_slice.getSizeY(); iy++)
    {
        for (int ix = 0; ix < m_slice.getSizeX(); ix++)
        {
            value = m_slice.getValue(ix, iy);
            painter.fillRect( m_wProp * ix, m_hProp * (m_slice.getSizeY() - iy - 1), m_wProp, m_hProp, m_palette[value] );
        }
    }

    int h = height * m_scale;
    int w = width * m_scale;
    QPixmap p = px->scaled( w, h );

    bool suc = p.save(&file, "PNG");
    if (suc) {
        std::cout << "SUCCESS" << "\n";
        file.close();
    }
}
