#ifndef TRACKLISTTOJSONSTRING_H
#define TRACKLISTTOJSONSTRING_H

#include <QObject>

class TrackListToJsonString : public QObject
{
    Q_OBJECT
public:
    explicit TrackListToJsonString(QObject *parent = 0);
    QString static encodeToJason(QStringList _tracks);

signals:

public slots:

private:



};

#endif // TRACKLISTTOJSONSTRING_H
