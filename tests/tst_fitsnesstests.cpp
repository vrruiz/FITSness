#include <QString>
#include <QtTest>

class FITSnessTests : public QObject
{
    Q_OBJECT

public:
    FITSnessTests();

private Q_SLOTS:
    void testCase1();
};

FITSnessTests::FITSnessTests()
{
}

void FITSnessTests::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(FITSnessTests)

#include "tst_fitsnesstests.moc"
