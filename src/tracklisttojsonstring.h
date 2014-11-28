#ifndef TRACKLISTTOJSONSTRING_H
#define TRACKLISTTOJSONSTRING_H

#include <QObject>

class TrackListToJsonString : public QObject
{
    Q_OBJECT



public:
    typedef struct{
    QString	title;
    QString	album;
    QString	id;
    } trackSet;



    explicit TrackListToJsonString(QObject *parent = 0);

    QString static getTrackList();
    static void clearTrackList();
    static void addToTrackList(trackSet);

    static QList<trackSet> trackList;



signals:

public slots:

private:



};

#endif // TRACKLISTTOJSONSTRING_H
