#include "linepaintarea.h"
#include "logger.h"

LinePaintArea::LinePaintArea(QWidget *parent)
    : QLabel(parent)
{
    fillPalette();
}

LinePaintArea::LinePaintArea(QWidget *parent, std::vector <Line> const & lines)
    : QLabel(parent), m_lines(lines)
{
    fillPalette();
}

void LinePaintArea::paintEvent(QPaintEvent * event)
{
    float length = 0;
    int height = 300;
    int width = 300;
    QPixmap pixmap = { height, width };
    QPainter painter(&pixmap);
    int j = 1;
    for (auto it = m_lines.begin(); it != m_lines.end(); it++) {
        Line line = *it;
        for (auto s = line.segments.begin();s != line.segments.end(); s++) {
            Segment seg = *s;
            painter.setPen(m_palette[seg.col]);
            painter.drawLine( seg.pos, j , seg.pos + seg.len, j );
            j++;
        }
        float lenMax = line.getMaxLen();
        if (lenMax > length)
            length = lenMax;
    }

    int h = height * m_scale;
    int w = width * m_scale;

    setPixmap(pixmap.scaled( w, h ));
    QLabel::paintEvent(event);
}

void LinePaintArea::fillPalette()
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

void LinePaintArea::setScales(float w, float h)
{
    (h > w) ? m_hScale = h/w : m_wScale = w/h;
}

void LinePaintArea::setProps(int w, int h)
{
    m_wProp = w;
    m_hProp = h;
}

void LinePaintArea::setScale(float scale)
{
    m_scale = scale;
}

void LinePaintArea::incScale()
{
    m_scale += 0.1;
}

void LinePaintArea::decScale()
{
    m_scale -= 0.1;
}

void LinePaintArea::paintLines()
{
    update();
}
