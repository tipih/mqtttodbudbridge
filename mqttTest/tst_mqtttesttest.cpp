#include <QString>
#include <QtTest>

class MqttTestTest : public QObject
{
    Q_OBJECT

public:
    MqttTestTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
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

void MqttTestTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(MqttTestTest)

#include "tst_mqtttesttest.moc"
