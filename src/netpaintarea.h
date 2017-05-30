#pragma once

#include <QWidget>
#include <QPainter>
#include "boxnet.h"
#include "slice.h"

int constexpr X_SCALE = 3;
int constexpr Y_SCALE = 3;
double constexpr Z_SCALE = 8/3 * 2;

/*!
 * \brief NetPaintArea - Виждет области отрисовки двухмерного изображения
 */

class NetPaintArea : public QWidget
{
    Q_OBJECT
public:
    explicit NetPaintArea(QWidget *parent = 0);
    virtual void paintEvent(QPaintEvent * event);

    void fillPalette();
    void setSlice(Slice slice);

private slots:
    void setSliceNum(int sliceNum);

private:
    BoxNet m_boxNet = { 299, 137, 348 };
    int m_sliceNum = 0;
    QColor m_palette[141];
    Slice m_slice;
};
