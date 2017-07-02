#pragma once

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include "slice.h"
#include "boxnet.h"
#include <QMouseEvent>

/*!
 * \brief NetPaintArea - Виждет области отрисовки двухмерного изображения
 */

class NetPaintArea : public QLabel
{
    Q_OBJECT
public:
    explicit NetPaintArea(QWidget *parent, BoxNet const & boxNet);
    explicit NetPaintArea(QWidget *parent = 0);
    virtual void paintEvent(QPaintEvent * event);

    void setScales(float w, float h);
    void setProps(int w, int h);
    void setScale(float scale);

    void incScale();
    void decScale();

    void fillPalette();

private slots:
    void paintX(int sliceNum);
    void paintY(int sliceNum);
    void paintZ(int sliceNum);

protected:
    void mousePressEvent(QMouseEvent *e);

signals:
    void mouseChanged(QString str);

private:
    int m_sliceNum = 0;
    QColor m_palette[142];
    Slice m_slice;
    BoxNet const * m_boxNet;
    int m_wProp = 1, m_hProp = 1;
    float m_scale = 1.0, m_wScale = 1.0, m_hScale = 1.0;
};
