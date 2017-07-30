// -----------------------------------------------------------------------------
// NSSQL.CPP
// -----------------------------------------------------------------------------
// $Revision: 1.3 $
// $Author: pameline $
// $Date: 2009/11/01 20:27:46 $
// -----------------------------------------------------------------------------
// Implementation des objets PARADOX de NAUTILUS
// -----------------------------------------------------------------------------
// history :
// PA  - juin 94
// PA  - juillet 93
// -----------------------------------------------------------------------------

#include <cstring>
#include <string>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <libconfig.h++>

#include "enterpriseLus/nsdivfctForCgi.h"
#include "enterpriseLus/superLus.h"

#include "nsdatabaseLus/nsSQL.h"

#include "mysql/errmsg.h"

using namespace std;
using namespace libconfig;

ontologyBaseManager::ontologyBaseManager(string sHost, string sUser, string sPass, string sDB)
    :_sqlConnector((MYSQL * )0), _iInstanceCounter(0)
{
    std::cout << "ontologyBaseManager::ontologyBaseManager(string sHost, string sUser, string sPass, string sDB)" << std::endl;

    //_sqlConnector     = (MYSQL *) 0 ;
    //_iInstanceCounter = 0 ;

    _host             = sHost.c_str();
    _user             = sUser.c_str();
    _password         = sPass.c_str();
    _database         = sDB.c_str();

    initBase();
}

ontologyBaseManager::~ontologyBaseManager()
{
    if ( (MYSQL *) NULL == _sqlConnector ) {
        return;
    }
    closeBase();
}

bool ontologyBaseManager::initBase()
{
    // Initializes the MYSQL structure

    _sqlConnector = mysql_init(NULL);

    if ((MYSQL *) NULL != _sqlConnector){
        std::cout << "_sqlConnector initialized" << std::endl;
        return true;
    }

    _sMysqlError = mysql_error(_sqlConnector);
    std::cout << _sMysqlError << std::endl;

    return false ;
}


bool ontologyBaseManager::openBase()
{
    if((MYSQL *) NULL == _sqlConnector) {
        initBase();
        _iInstanceCounter = 0;
    }

    if((MYSQL *)NULL == _sqlConnector)
            return false;

    _iInstanceCounter++;

    if (_iInstanceCounter > 1)
            return true;

    Config cfg;
    // Read the file. If there is an error, report it and exit.
    try
    {
        cfg.readFile("./config/sql.cfg");
    }
    catch(const FileIOException &fioex)
    {
        std::cerr << "I/O error while reading file." << std::endl;
        return(EXIT_FAILURE);
    }
    catch(const ParseException &pex)
    {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                  << " - " << pex.getError() << std::endl;
        return(EXIT_FAILURE);
    }

    std::map<std::string, std::string> credentials;
    std::vector<string> keys = {"host", "database", "user", "password"};

    for (string key : keys)
    {
        try
        {
            credentials[key] = cfg.lookup(key).c_str();
            std::cout << "Database " << key << ": " << credentials[key] << endl;
        }
        catch(const SettingNotFoundException &nfex)
        {
            cerr << "No " << key << " setting in configuration file." << endl;
        }
    }

    // attempts to establish a connection to a MySQL database engine running on host

    //std::cout << "members " << host << _user << _password << _database << std::endl;

    const char* cHost = credentials["host"].c_str();
    const char* cUser = credentials["user"].c_str();
    const char* cPassword = credentials["password"].c_str();
    const char* cDatabase = credentials["database"].c_str();

    std::cout << "cHost: " << cHost << std::endl;
    std::cout << "cUser: " << cUser << std::endl;
    std::cout << "cPassword: " << cPassword << std::endl;
    std::cout << "cDatabase: " << cDatabase << std::endl;

    try
    {
        if(!mysql_real_connect(_sqlConnector, cHost, cUser,
                               cPassword, cDatabase, 3306, NULL, 0))
        {
            throw 1;
        }
        /*MYSQL_RES *res_set;
        MYSQL_ROW row;*/
        std::cout << "Connection to db established." << std::endl;
        /*mysql_query(_sqlConnector, "show tables;");
        unsigned int i=0;
        res_set=mysql_store_result(_sqlConnector);
        //unsigned int numrows = mysql_num_rows(res_set);
        cout << " Tables in database " << endl;
        while (((row=mysql_fetch_row(res_set)) !=NULL))
            {
                cout << row[i] << endl;
            }
            */
        return true;
    }
    catch (...)
    {
        std::cout << "MySQL error: Failed to connect to database: " << mysql_error(_sqlConnector) << std::endl;
        return false;
    }

    //MYSQL *pReturnedConnector = mysql_real_connect(_sqlConnector, _host, _user, _pass, _database, 0, NULL, 0) ;

    //if ((MYSQL *) NULL == pReturnedConnector)
    //{
    //  _sMysqlError = mysql_error(_sqlConnector) ;
    //  return false ;
    //}

    return true;
}

MYSQL_RES * ontologyBaseManager::executeQuery(const string sQuery,
                                              bool bVerbose,
                                              string *pErrorMessage)
{
    if (sQuery.empty())
    {
        if (pErrorMessage)
            *pErrorMessage = string("Empty query");

        return (MYSQL_RES *) 0;
    }

    if (sQuery.length() > 5100)
    {
        if (pErrorMessage)
            *pErrorMessage = string("Query string is too long");

        return (MYSQL_RES *) 0;
    }

    if ((MYSQL *) NULL == _sqlConnector)
    {
        if (pErrorMessage)
            *pErrorMessage = string("Null MySQL connector");

        return (MYSQL_RES *) 0;
    }

    char szQueryBuffer[5120];
    strcpy(szQueryBuffer, sQuery.c_str());

    if (pErrorMessage)
        *pErrorMessage = string("") ;

    int iAttemptCount = 1;

    int iQueryError = mysql_query(_sqlConnector, szQueryBuffer);
    if (0 == iQueryError)
        return mysql_use_result(_sqlConnector);

    *pErrorMessage = string("Query error ") + std::to_string(iQueryError) + string(" ");

    // Connection lost - try again to reconnect automatically
    //
    if ((CR_SERVER_GONE_ERROR == iQueryError) || (CR_SERVER_LOST == iQueryError))
    {
        iAttemptCount++ ;

        iQueryError = mysql_query(_sqlConnector, sQuery.c_str()) ;
        if (0 == iQueryError)
            return mysql_use_result(_sqlConnector) ;
    }

    string sMysqlErrorMessage = mysql_error(_sqlConnector) ;

    if (pErrorMessage)
    {
        *pErrorMessage += sMysqlErrorMessage ;
        if (iAttemptCount > 1)
            *pErrorMessage += string(" (at second attempt)") ;
    }

    if (bVerbose)
    {
        string sErreurMsg = string("Error for query [") + sQuery + string("] -> ") + sMysqlErrorMessage;
        erreur(sErreurMsg.c_str(), standardError) ;
    }

    return (MYSQL_RES *) 0 ;
}

/*
bool
ontologyBaseManager::insertMessage(messageData message)
{
  if (isMessageInBase(message))
    return false ;

  if ((message.getFName() == "") || (message.getLName() == "") || (message.getID() == "") || (message.getIPP() == "") || (message.getIEP() == "") || (message.getDate() == ""))
    return false ;
  string sFName = string(message.getFName(), 0, 15) ;
  string sLName = string(message.getLName(), 0, 15) ;
  char szQueryBuffer[5120] ;
  sprintf(szQueryBuffer, "insert into %s (msgID, msgDate, msgIPP, msgIEP, msgFName, msgLName, msg) values (\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\")", _table.c_str(), message.getID().c_str(), message.getDate().c_str(), message.getIPP().c_str(), message.getIEP().c_str(), sFName.c_str(), sLName.c_str(), message.getMessage().c_str()) ;
//  IHEGlobal::PrintReport("query to store message is : " + string(szQueryBuffer)) ;
  int mysqlError = mysql_query(_sqlConnector, szQueryBuffer) ;
  if (mysqlError == 0)
    return true ;
  else
    return false ;
}
*/

bool ontologyBaseManager::closeBase()
{
    if ((MYSQL *) NULL == _sqlConnector)
    {
        _iInstanceCounter = 0;
        return false;
    }

    // Pending connections; just decrement counter
    //
    if (_iInstanceCounter > 1)
    {
        _iInstanceCounter--;
        return true ;
    }

    // Closes opened connection and deallocates the connection handle

    if (_sqlConnector != NULL) {
            mysql_close(_sqlConnector);
            _sqlConnector = NULL;
    }

    _iInstanceCounter = 0;

    return true ;
}

