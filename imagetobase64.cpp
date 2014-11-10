#include "imagetobase64.h"
#include <QFile>
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
    if(!file->open(QIODevice::ReadOnly)) return(false);


    QByteArray image = file->readAll();
    int originalSize = image.length();
    QString encoded = QString(image.toBase64());
    int encodedSize = encoded.size();

    dataString=encoded;
   return(true);
}

QString ImageToBase64::getLastError(){
    return(_lastError);
}

