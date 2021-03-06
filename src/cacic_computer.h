/*-----------------------------------------------------------------------------------------
 *
 * Project Cacic Agente
 *    File created by Lightbase
 *
 * Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
 *             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
 *
 *-----------------------------------------------------------------------------------------*/

#ifndef CACIC_COMPUTER_H
#define CACIC_COMPUTER_H

#include <QObject>
#include <QStringList>
//#include <QtSerialPort/QtSerialPort>
//#include <QtSerialPort/QSerialPortInfo>
#include <QtNetwork/QtNetwork>
#include <QSysInfo>
#include "operatingsystem.h"
#include "console.h"
#include "identificadores.h"
#include "ccacic.h"
#ifdef Q_OS_LINUX
    #include <sys/utsname.h>
#endif
class CACIC_Computer
{
public:
    CACIC_Computer();

    OperatingSystem getOs() const;
    std::string getUser() const;
    QList<QVariantMap> getNetworkInterface() const;
    QList<QVariantMap> networkInterfacesRunning();
    QJsonObject toJsonObject();
    void coletaDados();
private:

    OperatingSystem oOperatingSystem;
    std::string pegarUsu();

    ConsoleObject console;

    QList<QVariantMap> networkInterface;
    std::string usuario;
    std::string computerName;

};

Q_DECLARE_METATYPE(CACIC_Computer)
Q_DECLARE_METATYPE(CACIC_Computer*)
#endif // CACIC_COMPUTER_H
