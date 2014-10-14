#ifndef CACICTHREAD_H
#define CACICTHREAD_H
#include <QtCore>
#include <QMutex>
#include "QLogger.h"
#include "ccacic.h"
#include "cacic_comm.h"

class CacicThread : public QThread
{
public:
    CacicThread(QString applicationDirPath);
    void run();
    void setModuloDirPath(const QString &value);
    void setCMutex(QMutex *value);
    void setNomeModulo(const QString &value);
    bool enviarColeta();
    void setCcacic(CCacic *value);
    void setOCacicComm(CacicComm *value);

private:
    QString moduloDirPath;
    QString applicationDirPath;
    QLogger::QLoggerManager *logManager;
    void iniciarInstancias();
    void registraInicioColeta();
    void registraFimColeta(QString msg);
    void iniciarModulo();
    QMutex *cMutex;
    QString nomeModulo;
    CCacic *ccacic;
    CacicComm *OCacicComm;
};

#endif // CACICTHREAD_H