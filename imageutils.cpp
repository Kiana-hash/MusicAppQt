#include "imageutils.h"

#include <QLabel>
#include <QPixmap>


void ImageUtils::displayImage( QLabel* label,const QString& imagePath)
{
    if (label == nullptr)
    {
        return;
    }

    QPixmap pixmap(imagePath);

    if (pixmap.isNull())
    {
        label->clear();
        label->setText("No Image");

        return;
    }


    label->setPixmap(pixmap.scaled(label->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            )
        );

    label->setAlignment(Qt::AlignCenter);
}