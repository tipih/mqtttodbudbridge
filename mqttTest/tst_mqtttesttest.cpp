#include <QString>
#include <QtTest>
#include "imagetobase64.h"

class MqttTestTest : public QObject
{
    Q_OBJECT

public:
    MqttTestTest();

private Q_SLOTS:
    void initTestCase();
    void testimageConvert();
    void cleanupTestCase();


};

MqttTestTest::MqttTestTest()
{


}

void MqttTestTest::initTestCase()
{
}

void MqttTestTest::cleanupTestCase()
{
}

void MqttTestTest::testimageConvert()
{
    QImage testImage(":/image/testData/1.jpg");

    QString imageString;
    bool test1 = ImageToBase64::convertImage(":/image/testData/1.jpg",imageString);

    QCOMPARE(test1,true);
}

QTEST_APPLESS_MAIN(MqttTestTest)

#include "tst_mqtttesttest.moc"
