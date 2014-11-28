#include <QString>
#include <QtTest>
#include "metadata.h"
#include "meta.h"
#include <QMetaType>
#include <QDBusArgument>
#include <QDBusMetaType>



class TestmetadataTest : public QObject
{
    Q_OBJECT

public:
    TestmetadataTest();

    // Marshall the MyData data into a D-Bus argument
    // append a dictionary that associates ints to MyValue types





private Q_SLOTS:
    void testCase1();
};

QDBusArgument& marshal(QDBusArgument &argument, const Metadata &mList)
{
    argument.beginMap(QVariant::String,QVariant::String);

    QMap<QString, QString>::const_iterator i = mList.constBegin();

    QString key;
    QVariant value;

    while (i != mList.constEnd()) {
        {
            qDebug() << i.key() << ": " << i.value() << endl;

            key = i.key();
            value = i.value();
            argument.beginMapEntry();
            argument << i.key() << "value";
            argument.endMapEntry();
            i++;
        }


    }
    argument.endMap();
    qDebug()<<argument.currentType();
    return argument;
}






TestmetadataTest::TestmetadataTest()
{
    qDBusRegisterMetaType<Metadata>();
}

void TestmetadataTest::testCase1()
{
    Metadata mList;
    mList["1"] = "Test 1" ;
    mList["2"] = "Test 2" ;
    mList["3"] = "Test 3" ;
    mList["4"] = "Test 4" ;
    mList["5"] = "Test 5" ;
    mList["6"] = "Test 6" ;
    mList["7"] = "Test 7" ;
    mList["8"] = "Test 8" ;

QVariant mData;

//mData = mList;



    QVERIFY2(true, "Failure");
}




QTEST_APPLESS_MAIN(TestmetadataTest)

#include "tst_testmetadatatest.moc"
