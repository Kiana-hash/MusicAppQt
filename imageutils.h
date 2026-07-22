#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <QString>

class QLabel;

class ImageUtils
{
public:
    static void displayImage(QLabel* label,const QString& imagePath);
};

#endif