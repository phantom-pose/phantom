#pragma once

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include "slice.h"
#include "line.h"
#include <QMouseEvent>


class LinePaintArea : public QLabel
{
    Q_OBJECT
public:
    explicit LinePaintArea(QWidget *parent, std::vector <Line> const & lines);
    explicit LinePaintArea(QWidget *parent = 0);
    virtual void paintEvent(QPaintEvent * event);

    void setScales(float w, float h);
    void setProps(int w, int h);
    void setScale(float scale);

    void incScale();
    void decScale();

    void fillPalette();

private slots:
    void paintLines();

signals:
    void mouseChanged(QString str);

private:
    QColor m_palette[142];
    std::vector <Line> m_lines;
    int m_wProp = 1, m_hProp = 1;
    float m_scale = 1.0, m_wScale = 1.0, m_hScale = 1.0;
};
