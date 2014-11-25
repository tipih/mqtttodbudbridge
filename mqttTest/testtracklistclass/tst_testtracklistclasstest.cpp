#include <QString>
#include <QtTest>
#include <tracklisttojsonstring.h>

class TesttracklistclassTest : public QObject
{
    Q_OBJECT

public:
    TesttracklistclassTest();

private Q_SLOTS:
    void testCase1();
};

TesttracklistclassTest::TesttracklistclassTest()
{
}

void TesttracklistclassTest::testCase1()
{

    TrackListToJsonString::trackSet trackset;

    //Fill in data
    for (int i=0;i<10;i++){
    trackset.album="Album "+QString::number(i);
    trackset.id=i;
    trackset.title="Title "+QString::number(i);
    TrackListToJsonString::addToTrackList(trackset);
    }
    QString test= TrackListToJsonString::getTrackList();
    qDebug()<<test;
    QCOMPARE(test.count(),616);

    TrackListToJsonString::clearTrackList();
    test=TrackListToJsonString::getTrackList();
    QCOMPARE(test,QString("{\"trackList\": []}"));

    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(TesttracklistclassTest)

#include "tst_testtracklistclasstest.moc"
