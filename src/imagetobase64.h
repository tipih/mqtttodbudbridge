#ifndef IMAGETOBASE64_H
#define IMAGETOBASE64_H


#include <QObject>


class ImageToBase64 : public QObject
{
    Q_OBJECT
public:
    explicit ImageToBase64(QObject *parent = 0);
    static bool convertImage(QString filename,QString &dataString);
    static bool convertImage(const QImage &image,QString &dataString);


protected:


private:

signals:

public slots:

};

#endif // IMAGETOBASE64_H
