#include "installcacicsa.h"

InstallCacicSA::InstallCacicSA(const std::string &url, const std::string &user, const std::string &pass)
{
    this->pass = pass;
    this->url  = url;
    this->user = user;


}

InstallCacicSA::~InstallCacicSA()
{

}

bool InstallCacicSA::registryExists(HKEY RootKey,LPCTSTR SubKey)
{
    LONG lResult;
    HKEY mTempKey;
    lResult = RegOpenKeyEx(RootKey,SubKey,0,KEY_ALL_ACCESS,&mTempKey);

    switch(lResult)
    {
        case ERROR_SUCCESS: //ok
            RegCloseKey(mTempKey);
            return true;
            break;
        default: //an error code from windows
            return false;
            break;
    }
}

bool InstallCacicSA::downloadService(const std::string &rota, const std::string &path)
{
    return false;
}

bool InstallCacicSA::installService()
{
    return false;
}

bool InstallCacicSA::getConfig()
{
    return false;
}

bool InstallCacicSA::verificaServico()
{
    //Verifica se o serviço está rodando;

    //Verifica se o serviço está atualizado, senão baixa um novo;

    //Confere se a pasta bin existe, senão executa msi novamente.
    return false;
}

bool InstallCacicSA::installCacic()
{
    return false;
}

bool InstallCacicSA::deleteCacic26()
{
    //verifica se há algum processo do cacic rodando e os mata.
    //Deleta arquivos da versão 2.6 da pasta do cacic.
    return false;
}

bool InstallCacicSA::deleteCacic28()
{
    //verifica se há algum processo do cacic rodando e os mata.
    //Deleta arquivos da versão 2.8 da pasta do cacic.
    return false;
}
