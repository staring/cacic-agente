#include "ldaphandler.h"

/**
 * @brief LdapHandler::LdapHandler
 */
LdapHandler::LdapHandler()
{
}

/**
 * @brief LdapHandler::LdapHandler
 * @param ldapServer
 */
LdapHandler::LdapHandler(const QString &ldapServer)
{
    this->ldapServer = ldapServer;
}

#if defined(Q_OS_UNIX)
/**
 * @brief LdapHandler::busca
 * @param loginLdap QString&
 * @param passLdap QString&
 * @param baseLdap QString&
 * @param filterLdap QString&
 * @return QString com resultado de busca
 *
 * Realiza uma busca LDAP.
 */
QString LdapHandler::busca(const QString &loginLdap,const QString &passLdap,const QString &baseLdap,const QString &filterLdap, const QString &attrLdap = QString())
{
    std::string login = loginLdap.toStdString();
    std::string passwd = passLdap.toStdString();
    std::string base = baseLdap.toStdString();
    std::string filter = filterLdap.toStdString();

    int rc; // Variável a ser usada para retorno
    ulong version = LDAP_VERSION3; // Versão do LDAP
    char* attrs[2]; // Define atributos a serem requeridos
    LDAPMessage *res = NULL; // Mensagem retornada na pesquisa
    unsigned long numberOfEntries; // Numero de entradas retornada na pesquisa
    LDAPMessage *entry = NULL; // Ponteiro para a entrada
    char* attr = NULL; // Ponteiro para atributo
    char* wantedAttr = new char[3]; // Auxiliar para localizar o atributo desejado
    BerElement *internalPtr = NULL; // Ponteiro interno para busca de atributo
    char* *attrValue = NULL; //Valor do atributo

    attrs[0] = new char[3];
    strcpy(attrs[0],"cn");
    attrs[1] = NULL;

    wantedAttr = new char[3];
    strcpy(wantedAttr, "cn");

    // Seta a versão do protocolo
    rc = ldap_set_option(ldp, LDAP_OPT_PROTOCOL_VERSION, (void*)&version);
    if ( rc != LDAP_SUCCESS){
        qDebug() << "ldap_set_option error;";
    }

    // Faz o bind com o servidor
    rc = ldap_simple_bind_s(ldp,login.c_str(),passwd.c_str());
    if ( rc != LDAP_SUCCESS ){
        qDebug() << "ldap_simple_bind_s error: " << ldap_err2string(rc);
        return QString();
    }

    // Realiza a pesquisa
    rc = ldap_search_s(ldp, base.c_str(), LDAP_SCOPE_SUBTREE,filter.c_str(),attrs,0,&res);
    if ( rc != LDAP_SUCCESS ){
        qDebug() << "ldap_search_s error: " << QString::number(rc,16);
        return QString();
    }

    // Verifica o número de entradas retornadas
    numberOfEntries = ldap_count_entries(ldp, res);
    if(numberOfEntries == -1){
        qDebug() << "Retrieving number of entries failed.";
        return QString();
    }

    // Adquire o ponteiro para entrada
    entry = ldap_first_entry(ldp,res);
    if (entry == NULL) {
        qDebug() << "Error while retrieving entry. Entry is NULL.";
        return QString();
    }

    // Adquire o ponteiro para atributo
    attr = ldap_first_attribute(ldp,entry,&internalPtr);
    //    while (strcmp(attr,wantedAttr) != 0){
    //        attr = ldap_next_attributeA(ldp,res,internalPtr);
    //    }
    if ( attr == NULL ) {
        qDebug() << "Error while retrieving attribute. ATTR is NULL.";
        return QString();
    }

    // // Adquire o ponteiro para valor do atributo requerido
    attrValue = ldap_get_values(ldp,res,attr);
    if (attrValue == NULL) return QString();
    else return QString::fromLatin1(*attrValue);

    return QString();
}
#endif

#if defined(Q_OS_WIN)
/**
 * @brief LdapHandler::busca
 * @param loginLdap QString&
 * @param passLdap QString&
 * @param baseLdap QString&
 * @param filterLdap QString&
 * @return QString com resultado de busca
 *
 * Realiza uma busca LDAP.
 */
QString LdapHandler::busca(const QString &loginLdap,const QString &passLdap,const QString &baseLdap,const QString &filterLdap, const QString &attrLdap)
{

    char *login = (char*)loginLdap.toStdString().c_str();
    char *passwd = (char*)passLdap.toStdString().c_str();
    char *base = (char*)baseLdap.toStdString().c_str();
    char *filter = (char*)filterLdap.toStdString().c_str();
    char *attribute = (char*)attrLdap.toStdString().c_str();

    ulong rc; // Variável a ser usada para retorno
    ulong version = LDAP_VERSION3; // Versão do LDAP
    PCHAR attrs[2]; // Define atributos a serem requeridos
    LDAPMessage *res = NULL; // Mensagem retornada na pesquisa
    ULONG numberOfEntries; // Numero de entradas retornada na pesquisa
    LDAPMessage *entry = NULL; // Ponteiro para a entrada
    PCHAR attr = NULL; // Ponteiro para atributo
    PCHAR wantedAttr = new char[3]; // Auxiliar para localizar o atributo desejado
    BerElement *internalPtr = NULL; // Ponteiro interno para busca de atributo
    PCHAR *attrValue = NULL; //Valor do atributo

    attrs[0] = new char[3];
    strcpy(attrs[0],attribute);
    attrs[1] = NULL;

    wantedAttr = new char[3];
    strcpy(wantedAttr, "cn");

    // Seta a versão do protocolo
    rc = ldap_set_option(ldp, LDAP_OPT_PROTOCOL_VERSION, (void*)&version);
    if ( rc != LDAP_SUCCESS){
        qDebug() << "ldap_set_option error;";
    }

    // Conecta ao servidor
    rc = ldap_connect(ldp,(PLDAP_TIMEVAL)NULL);
    if ( rc != LDAP_SUCCESS ){
        qDebug() << "ldap_connect error: " << QString::number(rc,16);
    }

    // Faz o bind com o servidor
    rc = ldap_simple_bind_sA(ldp,login,passwd);
    if ( rc != LDAP_SUCCESS ){
        qDebug() << "ldap_simple_bind_s error: " << QString::number(rc,16);
    }

    // Realiza a pesquisa
    rc = ldap_search_sA(ldp, base, LDAP_SCOPE_SUBTREE,filter,attrs,0,&res);
    if ( rc != LDAP_SUCCESS ){
        qDebug() << "ldap_search_s error: " << QString::number(rc,16);
        return QString();
    }

    // Verifica o número de entradas retornadas
    numberOfEntries = ldap_count_entries(ldp, res);
    if(numberOfEntries == -1){
        qDebug() << "Retrieving number of entries failed.";
        return QString();
    } else
        qDebug() << "Number of entries: " << QString::number(numberOfEntries);

    // Adquire o ponteiro para entrada
    entry = ldap_first_entry(ldp,res);
    if (entry == NULL) {
        qDebug() << "Error while retrieving entry. Entry is NULL.";
        return QString();
    }

    // Adquire o ponteiro para atributo
    attr = ldap_first_attributeA(ldp,entry,&internalPtr);
    //    while (strcmp(attr,wantedAttr) != 0){
    //        attr = ldap_next_attributeA(ldp,res,internalPtr);
    //    }
    if ( attr == NULL ) {
        qDebug() << "Error while retrieving attribute. ATTR is NULL.";
        return QString();
    }

    // // Adquire o ponteiro para valor do atributo requerido
    attrValue = ldap_get_valuesA(ldp,res,attr);
    qDebug() << "retorno attrValue: " << QString::fromLatin1(*attrValue);
    if (attrValue == NULL) {
        qDebug() << "attrValue == NULL";
        return QString();
    } else {
        qDebug() << QString::fromLatin1(*attrValue);
        return QString::fromLatin1(*attrValue);
    }

    return QString();
}
#endif

/**
 * @brief LdapHandler::inicializar
 * @return bool
 *
 * Inicializa interface com o servidor do LDAP.
 */
bool LdapHandler::inicializar()
{

#if defined(Q_OS_UNIX)
    ldapServer = "ldap://" + ldapServer + ":389";
    std::string host = ldapServer.toStdString();
    qDebug() << host.c_str() << ": " << ldap_err2string(ldap_initialize(&ldp,host.c_str()));

    if (ldap_initialize(&ldp,host.c_str()) != LDAP_SUCCESS){
        qDebug() << "ldap_init error";
        return false;
    }
#elif defined(Q_OS_WIN)
    std::string serverStdString = ldapServer.toStdString();
    PCHAR host = (PCHAR)serverStdString.c_str();

    ldp = ldap_initA(host,389);
    if (ldp == NULL){
        qDebug() << "ldap_initA error";
        return false;
    }
#endif
    return true;
}

/**
 * @brief LdapHandler::setServer
 * @param ldapServer QString&
 *
 * Seta endereço do servidor LDAP.
 */
void LdapHandler::setServer(const QString &ldapServer)
{
    this->ldapServer = ldapServer;
}


