#ifndef SIZEIMAGE_H
#define SIZEIMAGE_H

#include <QSize>
#include <QDebug>

QSize setSizeImage(QSize sizeImage, QSize sizeWindow)
{
    //размеры окна для изображения
    int H = sizeWindow.height();
    int W = sizeWindow.width();

    //размеры изображения
    int h = sizeImage.height();
    int w = sizeImage.width();

    double k; //коофициент изменения размеров

    if (h < H and w < W)
    {
        if ((double) h/H > (double) w/W)
        {
            k = (double) H/h;
        }
        else
        {
            k = (double) W/w;
        }
    }

    if (h < H and w > W)
    {
        k = (double) W/w;
    }
    if (h > H and w < W)
    {
        k = (double) H/h;
    }

    if (h > H and w > W)
    {
        if ((double) H/h < (double) W/w)
        {
            k = (double) H/h;
        }
        else
        {
            k = (double) W/w;
        }
    }

    return QSize(w*k, h*k);
}

#endif // SIZEIMAGE_H
