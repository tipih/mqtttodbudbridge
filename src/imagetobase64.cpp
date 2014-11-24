#include "imagetobase64.h"
#include <QFile>
#include <QImage>
#include <QBuffer>
#include <QDir>
#include <QDebug>
#include <QUrl>

static QString _lastError;


ImageToBase64::ImageToBase64(QObject *parent) :
    QObject(parent)
{
}

bool ImageToBase64::convertImage(QString filename, QString &dataString)
{
    if (filename.contains("file://",Qt::CaseInsensitive))
    {
        filename=QUrl(filename).toLocalFile();


    }

    QFile* file = new QFile();
    file->setFileName(filename);
    if(!file->open(QIODevice::ReadOnly))
        return(false);


    QByteArray image = file->readAll();
    int originalSize = image.length();
    QString encoded = QString(image.toBase64());
    int encodedSize = encoded.size();
    dataString=encoded;

    return(true);
}

bool ImageToBase64::convertImage(const QImage &image, QString &dataString)
{


    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "jpg"); // writes image into ba in PNG format

    if (ba.length()==0)
        return(false);

    dataString = QString(ba.toBase64());

   return(true);
}



