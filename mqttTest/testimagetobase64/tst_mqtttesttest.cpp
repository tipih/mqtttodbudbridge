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
    void testimageConvertQImage();
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
    qDebug()<<"Testing image convert on a filename";

    QImage testImage(":/image/testData/1.jpg");
    QString imageString;
    bool test1 = ImageToBase64::convertImage(":/image/testData/1.jpg",imageString);
    QCOMPARE(test1,true);

    test1 = ImageToBase64::convertImage(":/image/testData/invalid.jpg",imageString);
    QCOMPARE(test1,false);

    test1 = ImageToBase64::convertImage(":/image/testData/2.jpg",imageString);
    QCOMPARE(test1,true);

    test1 = ImageToBase64::convertImage(":/image/testData/3.jpg",imageString);
    QCOMPARE(test1,true);

    test1 = ImageToBase64::convertImage(":/image/testData/4.jpg",imageString);
    QCOMPARE(test1,true);

    test1 = ImageToBase64::convertImage(":/image/testData/5.jpg",imageString);
    QCOMPARE(test1,true);
}

void MqttTestTest::testimageConvertQImage(){
    QImage testImage(":/image/testData/1.jpg");
    QString imageString;
    bool test1 = ImageToBase64::convertImage(testImage,imageString);
    QCOMPARE(test1,true);

    testImage.load(":/image/testData/invalid.jpg");
    test1 = ImageToBase64::convertImage(testImage,imageString);
    QCOMPARE(test1,false);

    testImage.load(":/image/testData/2.jpg");
    test1 = ImageToBase64::convertImage(testImage,imageString);
    QCOMPARE(test1,true);

    testImage.load(":/image/testData/3.jpg");
    test1 = ImageToBase64::convertImage(testImage,imageString);
    QCOMPARE(test1,true);

    testImage.load(":/image/testData/4.jpg");
    test1 = ImageToBase64::convertImage(testImage,imageString);
    QCOMPARE(test1,true);

    testImage.load(":/image/testData/5.jpg");
    test1 = ImageToBase64::convertImage(testImage,imageString);
    QCOMPARE(test1,true);


}

QTEST_APPLESS_MAIN(MqttTestTest)

#include "tst_mqtttesttest.moc"
