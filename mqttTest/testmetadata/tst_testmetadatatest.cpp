#include <QString>
#include <QtTest>
#include "metadata.h"
#include "meta.h"
#include <QMetaType>
#include <QDBusArgument>
#include <QDBusMetaType>
#include <mpreis_interface.h>
#include "mediaModalityController.h"
#include <QProcess>
#include <QObject>
#include <QtDBus/QDBusInterface>
#include <QtCore/QFile>
#include <stdio.h>
#include <mpreis_interface.h>
#include <QList>
#include "meta.h"
#include "mqttbridge.h"



class TestmetadataTest : public QObject
{
    Q_OBJECT

public:
    TestmetadataTest();

    // Marshall the MyData data into a D-Bus argument
    // append a dictionary that associates ints to MyValue types



private:
    org::mpris::MediaPlayer2::Player *musicPlayer2PlayerProxy;
    org::mpris::MediaPlayer2::TrackList *musicPlayer2TracklistProxy;
    org::freedesktop::DBus::Properties *musicPlayer2property;
    MediaModalityController* MediaControllerService;

private Q_SLOTS:
    void testCase1();
};







TestmetadataTest::TestmetadataTest()
{
    qDBusRegisterMetaType<Metadata>();

    qDBusRegisterMetaType<TrackMetadata>();
    qDebug()<<"In init";

    qDBusRegisterMetaType<TrackMetadata>();
    musicPlayer2PlayerProxy = new org::mpris::MediaPlayer2::Player("org.mpris.MediaPlayer2.vlc", "/org/mpris/MediaPlayer2",
                                                                   QDBusConnection::sessionBus(), this);

    musicPlayer2property = new org::freedesktop::DBus::Properties("org.mpris.MediaPlayer2.vlc", "/org/mpris/MediaPlayer2",
                                                                  QDBusConnection::sessionBus(), this);

    musicPlayer2TracklistProxy = new org::mpris::MediaPlayer2::TrackList("org.mpris.MediaPlayer2.vlc", "/org/mpris/MediaPlayer2",
                                                                         QDBusConnection::sessionBus(), this);




}



void TestmetadataTest::testCase1()
{
    //As it is not really possible to create a QDbusQrgument.
    //We test using the Vlc player. We are going to start the Vlc player
    //using a known playlist.

    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface dbus_iface("org.freedesktop.DBus", "/org/freedesktop/DBus",
                              "org.freedesktop.DBus", bus);


    MediaModalityController a;


    QProcess process;
    qDebug()<<"Starting VlC";
    process.start("vlc Musicplaylist.xspf");

    QTest::qWait(1000);


    QVERIFY2(musicPlayer2PlayerProxy->isValid(), "Failure");
    QVERIFY2(musicPlayer2property->isValid(), "Failure");
    QVERIFY2(musicPlayer2TracklistProxy->isValid(), "Failure");

    QList<QDBusObjectPath> _trackList;

    QMetaObject::invokeMethod(&a, "getTracklist",Q_RETURN_ARG(QList<QDBusObjectPath>, _trackList));

    qDebug()<<"Tracklist "<<_trackList.count();
   // QList<QDBusObjectPath> trackListObjectPath = MediaModalityController.getTracklist();
   // qDebug()<<trackListObjectPath.count();
    QTest::qWait(500);
    process.terminate();
}



QTEST_MAIN(TestmetadataTest)


#include "tst_testmetadatatest.moc"
