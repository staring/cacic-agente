#include "chksystimer.h"

chksysTimer::chksysTimer(QObject *parent) :
    QObject(parent)
{
}

chksysTimer::chksysTimer(QString cacicFolder){
    this->cacicFolder = cacicFolder;
    log = new LogCacic(CHKSYS, this->cacicFolder+"/Logs");

    timerCheckService = new QTimer();
    QObject::connect(timerCheckService, SIGNAL(timeout()), this, SLOT(onTimerCheckService()));
}

bool chksysTimer::start(int msecCheckService)
{

    //timer para verificar se o serviço está instalado e rodando.
    timerCheckService->setInterval(msecCheckService);
    timerCheckService->start();

    return true;
}

void chksysTimer::onTimerCheckService()
{
    this->timerCheckService->stop();
#ifdef Q_OS_WIN
    QFile fileService(cacicFolder+"/cacic-service.exe");
    ServiceController service(QString(CACIC_SERVICE_NAME).toStdWString());
    //Tenta instalar o serviço
    if (!service.isInstalled() || !service.isRunning()){
        if ((!fileService.exists())) {
            log->escrever(LogCacic::ErrorLevel, "Não foi possível localizar o módulo do cacicdaemon.");
            fileService.close();

            this->downloadService();
        }
        this->verificarModulos();

        if (!service.isInstalled()){
            if(!service.install(QString(this->cacicFolder + "/cacic-service.exe").toStdWString(),
                                QString("Cacic Daemon").toStdWString()))
                log->escrever(LogCacic::ErrorLevel, "Falha ao reinstalar o serviço: " +
                                                    QString::fromStdString(service.getLastError()));
        }
    }
    if (!service.isRunning()){
        if (service.start()){
        } else {
            log->escrever(LogCacic::ErrorLevel, "Falha ao iniciar o serviço: " +
                                                                    QString::fromStdString(service.getLastError()));
        }
    }
#else
    QFile fileService(cacicFolder+"/cacic-service");
    if ((!fileService.exists() || !fileService.size() > 0)) {
        log->escrever(LogCacic::ErrorLevel, "Não foi possível logalizar o módulo do cacicdaemon.");
        fileService.close();

        this->downloadService();
    } else {
        ConsoleObject console;
        console("/etc/init.d/cacic3 start").toStdString();
    }
#endif
    this->timerCheckService->start();
}

bool chksysTimer::downloadService()
{
    QFile *modulo;
    bool downloadOk = false;
    //pega o arquivo do módulo selecionado

#ifdef Q_OS_WIN
    QString moduloName = "cacic-service.exe";
#else
    QString moduloName = "cacic-service";
#endif
    modulo = new QFile(this->cacicFolder + moduloName);
    modulo->open(QFile::ReadOnly);

    QJsonObject metodoDownload;
    //verifica o tipo de download e tenta baixar o módulo para a pasta temporária.
    metodoDownload = CCacic::getJsonFromFile(this->cacicFolder + "/getConfig.json")
            ["agentcomputer"].toObject()
            ["metodoDownload"].toObject();

    if (!metodoDownload.isEmpty()){
        //verifica se já possuía o módulo atualizado na pasta temporária, se não baixa um novo.
        if (!modulo->exists()){
            if (modulo->size() < 0 && !modulo->remove()){
              log->escrever(LogCacic::ErrorLevel, "Falha ao remover módulo corrompido.");
            }


            CacicComm *oCacicComm = new CacicComm(CHKSYS, this->cacicFolder);
            oCacicComm->setFtpUser(metodoDownload["usuario"].toString());
            oCacicComm->setFtpPass(metodoDownload["senha"].toString());

            downloadOk = oCacicComm->fileDownload(metodoDownload["tipo"].toString(),
                                                  metodoDownload["url"].toString(),
                                                  metodoDownload["path"].toString() +
                                                  (metodoDownload["path"].toString().endsWith("/") ? moduloName : "/" + moduloName),
                                                  this->cacicFolder);

            if (downloadOk){
                QFile *novoModulo;
                novoModulo = new QFile(this->cacicFolder + moduloName);

                //faz uma verificação do novo módulo.
                if (!(novoModulo->exists() && novoModulo->size()>1)){
                    log->escrever(LogCacic::ErrorLevel,
                                       QString("Falha ao baixar " + moduloName +
                                               "("+metodoDownload["tipo"].toString()+ "://" +
                                       metodoDownload["url"].toString() + metodoDownload["path"].toString() +
                            (metodoDownload["path"].toString().endsWith("/") ? moduloName : "/" + moduloName)+")"));
                    novoModulo->remove();
                    return false;
                } else {
                    return true;
                }
            } else {
                log->escrever(LogCacic::ErrorLevel, QString("Problemas durante o download de " + moduloName));
                return false;
            }
        } else {
            return true;
        }
    } else {
        log->escrever(LogCacic::ErrorLevel, QString("Não foi possível recuperar json de " +
                                                         this->cacicFolder + "/getConfig.json ao tentar baixar " +
                                                         moduloName));
        return false;
    }
}

bool chksysTimer::verificarModulos()
{
    QDir dir(this->cacicFolder + "/temp");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Executable);
    dir.setSorting(QDir::Size | QDir::Reversed);
    bool serviceUpdate= false;
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i<list.size(); i++){
        //Se o módulo for install-cacic, deverá ficar na pasta "/bin"
        QFile novoModulo(list.at(i).filePath());
        if((list.at(i).fileName().contains("cacic-service"))){
            serviceUpdate = true;
            log->escrever(LogCacic::InfoLevel, "Parando serviço para atualização.");
#ifdef Q_OS_WIN
            ServiceController *service = new ServiceController(QString("cacicdaemon").toStdWString());
            if(service->isRunning()){
                QProcess stopService;
                QStringList args;
                args << "stop" << list.at(i).fileName();
                stopService.execute("SC", args);
            }
#else
            ConsoleObject console;
            console("/etc/init.d/cacic3 stop");
#endif
        }
        if (QFile::exists(this->cacicFolder + "/" + (list.at(i).fileName().contains("install-cacic") ?
                                                     "bin/" + list.at(i).fileName() :
                                                     list.at(i).fileName()))){
            QFile::remove(this->cacicFolder + "/" + (list.at(i).fileName().contains("install-cacic") ?
                                                          "bin/" + list.at(i).fileName() :
                                                           list.at(i).fileName()));
            //Garante a exclusão. às vezes o SO demora a reconhecer, dunno why.
            QThread::sleep(1);
        }

        if (!QFile::exists(this->cacicFolder + "/" + (list.at(i).fileName().contains("install-cacic") ?
                                                       "bin/" + list.at(i).fileName() :
                                                        list.at(i).fileName()))){
            novoModulo.copy(this->cacicFolder + "/" + (list.at(i).fileName().contains("install-cacic") ?
                                                        "bin/" + list.at(i).fileName() :
                                                         list.at(i).fileName()));
            if (!novoModulo.remove())
                log->escrever(LogCacic::ErrorLevel, "Falha ao excluir "+list.at(i).fileName()+" da pasta temporária.");
            else {
                log->escrever(LogCacic::InfoLevel, "Módulo \"" + list.at(i).filePath() + "\" atualizado.");
                if (list.at(i).fileName().contains("cacic-service") && serviceUpdate){
    #ifdef Q_OS_WIN
                    ServiceController *service = new ServiceController(QString("cacicdaemon").toStdWString());
                    if (!service->isRunning()) service->start();
    #else
                    ConsoleObject console;
                    console("/etc/init.d/chksys start");
    #endif
                }
            }
        } else {
            log->escrever(LogCacic::ErrorLevel, "Falha ao excluir módulo antigo"+list.at(i).fileName()+" da pasta temporária.");
        }

        novoModulo.close();
    }
    list.clear();
    return true;
}