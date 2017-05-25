#pragma once

#include <QWidget>
#include <QPainter>
#include "boxnet.h"

int constexpr X_SCALE = 3;
int constexpr Y_SCALE = 3;
double constexpr Z_SCALE = 8/3 * 2;

class NetPaintArea : public QWidget
{
    Q_OBJECT
public:
    explicit NetPaintArea(QWidget *parent = 0);
    virtual void paintEvent(QPaintEvent * event);

private slots:
    void setSliceNum(int sliceNum);

private:
    BoxNet m_boxNet = { 299, 137, 348 };
    int m_sliceNum = 0;
};
