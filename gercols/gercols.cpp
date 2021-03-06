#include "gercols.h"

Gercols::Gercols(QObject *parent)
{
    oColeta = new CColeta;
    //Pega chave do registro, que será pega na instalação.
    cacicMainFolder = CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
//    oCacic.setChaveCrypt(oCacic.getValueFromRegistry("Lightbase", "Cacic", "key").toString());
    CCacic::salvarVersao("gercols");
    QObject::connect(this, SIGNAL(iniciaConfiguracao()), oColeta, SLOT(configuraColetas()));
    QObject::connect(this, SIGNAL(iniciaColeta()), oColeta, SLOT(run()));
    logcacic = new LogCacic(LOG_GERCOLS, cacicMainFolder+"/Logs");
}

void Gercols::run()
{
    emit iniciaConfiguracao();
    //emite sinal para começar a coleta
    emit iniciaColeta();

    oColeta->waitToCollect();
    //salva json em arquivo
    if (!oColeta->toJsonObject().isEmpty()){
        QJsonObject oldColeta;
        oldColeta = CCacic::getJsonFromFile(cacicMainFolder + "/coleta.json");
        QVariantMap enviaColeta;
        CCacic::setJsonToFile(oColeta->toJsonObject(), cacicMainFolder + "/coleta.json");
        //verificando quantidade de chaves. Se for diferente, envia a coleta.
        if (oldColeta.isEmpty() || this->verificaColeta(oldColeta, oColeta->toJsonObject())) {
            enviaColeta["enviaColeta"] = true;
            CCacic::setValueToRegistry("Lightbase", "Cacic", enviaColeta);
            logcacic->escrever(LogCacic::InfoLevel, QString("Novas informações prontas para o envio ao gerente."));
        } else {
            logcacic->escrever(LogCacic::InfoLevel, QString("Coleta sem alterações."));
            enviaColeta["enviaColeta"] = false;
            CCacic::setValueToRegistry("Lightbase", "Cacic", enviaColeta);
        }
    } else {
        logcacic->escrever(LogCacic::ErrorLevel, QString("Falha ao realizar coleta."));
    }
    emit finished();
}

/****************************************************************************************
 * Verifica a diferença de coleta entre 2 json's e grava em um arquivo chamado coletaDiff
 * os dados removidos e os dados inseridos.
 * o qual ainda não tem propósito, mas vai ter.
 ****************************************************************************************/
bool Gercols::verificaColeta(const QJsonObject &coletaAntiga, const QJsonObject &novaColeta){
    bool retorno = false;
    QJsonObject primeiroJson, segundoJson, diferencaColeta, coletaDiff;
    primeiroJson = coletaAntiga;
    segundoJson = novaColeta;
    for (int i = 0; i<2; i++){
        if (!(primeiroJson.isEmpty() && segundoJson.isEmpty())){
            foreach (QString key, primeiroJson.keys()){
                if(key != "computador"){
                    QJsonValue jsonRetorno;
                    if (!segundoJson[key].isNull()){
                        if (this->percorreColeta(primeiroJson[key], segundoJson[key], jsonRetorno)){
                            diferencaColeta[key] = jsonRetorno;
                            if (i == 0){
                                /*o primeiro json é o antigo, então o que for inserido *
                                 * no 'diferencaColeta' é o que foi acrescido à coleta */
                                coletaDiff["coletasRetiradas"] = diferencaColeta;
                            }
                            else {
                                coletaDiff["coletasInseridas"] = diferencaColeta;
                            }
                            retorno = true;
                        }
                    }
                }
            }
        }
        primeiroJson = novaColeta;
        segundoJson = coletaAntiga;
    }
    if (!diferencaColeta.isEmpty()){
        coletaDiff["computador"] = oColeta->getOComputer().toJsonObject();
        if (QFile::exists(cacicMainFolder + "/coletaDiff.json")){
            CCacic::deleteFile(cacicMainFolder + "/coletaDiff.json");
        }
        CCacic::setJsonToFile(coletaDiff, cacicMainFolder + "/coletaDiff.json");
    }
    //true se houver diferença.
    return retorno;
}
/*************************************************************************************************
* Aqui percorro o json de maneira recursiva pegando "primeiroValor" e comparo com o "segundoValor"
* sendo ele objeto (json), array ou valor comum.
**************************************************************************************************/
bool Gercols::percorreColeta(const QJsonValue &primeiroValor, const QJsonValue &segundoValor, QJsonValue &jsonRetorno){
    //Se o primeiro valor for nulo, o segundo não vai ser, então deverá ser gravado.
    bool diferenca = false;
    if(primeiroValor.isNull() && !segundoValor.isNull()){
        jsonRetorno = segundoValor;
        return true;
    } else if (primeiroValor.isObject()) {
        //Se for um objeto, verifico as chaves.
        QJsonObject jsonObj;
        foreach(QString key, primeiroValor.toObject().keys()){
            //Tirando algumas variáveis que modificam sempre.
            if (    key != "FreeSpace" &&
                    key != "clock" &&
                    key != "CurrentClockSpeed" &&
                    key != "ipv4" &&
                    key != "ipv6" &&
                    key != "MaxClockSpeed") {
                QJsonValue subRetorno;
                //Mando percorrer cada key do objeto também.
                if (this->percorreColeta(primeiroValor.toObject()[key], segundoValor.toObject()[key], subRetorno)){
                    jsonObj[key] = subRetorno;
                    diferenca = true;
                }
            }
        }
        if (diferenca){
            jsonRetorno = jsonObj;
            return true;
        } else {
            return false;
        }
    } else if (primeiroValor.isArray()) {
        //Se for um array, verificar completo.
        QJsonArray jsonArray;
        for (int i = 0; i<primeiroValor.toArray().size();i++){
            QJsonValue subRetorno;
            //a cada valor do array, verifica o jsonValue repassado (sendo ele subJson ou atributo).
            if (this->percorreColeta(primeiroValor.toArray().at(i), segundoValor.toArray().at(i), subRetorno)){
                jsonArray.append(subRetorno);
                diferenca = true;
            }
        }
        if (diferenca){
            jsonRetorno = jsonArray;
            return true;
        } else {
            return false;
        }
    } else {
        //Se encontrar um atributo, no caso vai ser a última instancia, ele compara os valores.
        if (primeiroValor.toVariant() == segundoValor.toVariant()){
            return false;
        } else {
            //            jsonRetorno = !segundoValor.isNull() ? segundoValor : primeiroValor;
            jsonRetorno = primeiroValor;
            return true;
        }
    }
}
