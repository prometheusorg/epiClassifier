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

#include "enterpriseLus/nsdivfctForCgi.h"
#include "enterpriseLus/superLus.h"

#include "nsdatabaseLus/nsSQL.h"

#include "mysql/errmsg.h"

ontologyBaseManager::ontologyBaseManager(string sHost, string sUser, string sPass, string sDB)
{
  _sqlConnector     = (MYSQL *) 0 ;
  _iInstanceCounter = 0 ;

  _host             = sHost ;
  _user             = sUser ;
  _pass             = sPass ;
  _database         = sDB ;

  initBase() ;
}

ontologyBaseManager::~ontologyBaseManager()
{
  if ((MYSQL *) NULL == _sqlConnector)
    return ;

  closeBase() ;
}

bool
ontologyBaseManager::initBase()
{
  // Initializes the MYSQL structure

  _sqlConnector = mysql_init(NULL) ;

  if ((MYSQL *) NULL != _sqlConnector)
    return true ;

  _sMysqlError = mysql_error(_sqlConnector) ;

  return false ;
}


bool
ontologyBaseManager::openBase()
{
  if ((MYSQL *) NULL == _sqlConnector)
  {
    initBase() ;
    _iInstanceCounter = 0 ;
  }

  if ((MYSQL *) NULL == _sqlConnector)
    return false ;

  _iInstanceCounter++ ;

  // Pending connections; just increment counter
  //
  if (_iInstanceCounter > 1)
    return true ;

  // attempts to establish a connection to a MySQL database engine running on host

  MYSQL *pReturnedConnector = mysql_real_connect(_sqlConnector, _host.c_str(), _user.c_str(), _pass.c_str(), _database.c_str(), 0, NULL, 0) ;

  if ((MYSQL *) NULL == pReturnedConnector)
  {
    _sMysqlError = mysql_error(_sqlConnector) ;
    return false ;
  }

  return true ;
}

MYSQL_RES *
ontologyBaseManager::executeQuery(const string sQuery, bool bVerbose, string *pErrorMessage)
{
  if (string("") == sQuery)
  {
    if (pErrorMessage)
      *pErrorMessage = string("Empty query") ;

    return (MYSQL_RES *) 0 ;
  }

  if (strlen(sQuery.c_str()) > 5100)
  {
    if (pErrorMessage)
      *pErrorMessage = string("Query string is too long") ;

    return (MYSQL_RES *) 0 ;
  }

  if ((MYSQL *) NULL == _sqlConnector)
  {
    if (pErrorMessage)
      *pErrorMessage = string("Null MySQL connector") ;

    return (MYSQL_RES *) 0 ;
  }

  char szQueryBuffer[5120] ;
  strcpy(szQueryBuffer, sQuery.c_str()) ;

  if (pErrorMessage)
    *pErrorMessage = string("") ;

  int iAttemptCount = 1 ;

  int iQueryError = mysql_query(_sqlConnector, szQueryBuffer) ;
  if (0 == iQueryError)
    return mysql_use_result(_sqlConnector) ;

  *pErrorMessage = string("Query error ") + IntToString(iQueryError) + string(" ") ;

  // Connection lost - try again to reconnect automatically
  //
  if ((CR_SERVER_GONE_ERROR == iQueryError) || (CR_SERVER_LOST == iQueryError))
  {
    iAttemptCount++ ;

    iQueryError = mysql_query(_sqlConnector, szQueryBuffer) ;
    if (0 == iQueryError)
      return mysql_use_result(_sqlConnector) ;
  }

  string sMysqlErrorMessage = mysql_error(_sqlConnector) ;

  if (pErrorMessage)
  {
    *pErrorMessage += sMysqlErrorMessage ;
    if (iAttemptCount > 1)
      *pErrorMessage += string(" (at secont attempt)") ;
  }

  if (bVerbose)
  {
    string sErreurMsg = string("Error for query [") + sQuery + string("] -> ") + sMysqlErrorMessage ;
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

bool
ontologyBaseManager::closeBase()
{
  if ((MYSQL *) NULL == _sqlConnector)
  {
    _iInstanceCounter = 0 ;
    return false ;
  }

  // Pending connections; just decrement counter
  //
  if (_iInstanceCounter > 1)
  {
    _iInstanceCounter-- ;
    return true ;
  }

  // Closes opened connection and deallocates the connection handle

  mysql_close(_sqlConnector) ;

  _iInstanceCounter = 0 ;

  return true ;
}

