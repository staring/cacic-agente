#ifndef TESTINSTALLCACIC_H
#define TESTINSTALLCACIC_H
#include "../src/CACIC_comm.h"
#include "../src/ccacic.h"
#include <QtTest/QtTest>
#include <QDebug>

class CTestCacic : public QObject
{
    Q_OBJECT
public:
    explicit CTestCacic(QObject *parent = 0);

private:
    CacicComm OCacicComm;
    CCacic OCacic;

signals:

public slots:

private slots:
    void testCommStatus();
    void testComm();
    void testGetValueFromFile();
    void testGetValueFromTags();
};

#endif // TESTINSTALLCACIC_H