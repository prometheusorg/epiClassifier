// -----------------------------------------------------------------------------
// messagesDB.h
// -----------------------------------------------------------------------------
// temporary messages Database managment 
// -----------------------------------------------------------------------------
// $Revision: 1.1 $
// $Author: pameline $
// $Date: 2009/07/23 21:13:43 $
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2004-2005
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------

#ifndef __NSSQL_H__
# define __NSSQL_H__

# include <vector>
# include <string>

#include  "mysql/mysql.h"

using namespace std ;

#ifndef __linux__
# if defined(_DANSSGBDDLL)
#  define _CLASSESGBD __export
# else
#  define _CLASSESGBD __import
# endif
#endif

#ifndef __linux__
class _CLASSESGBD ontologyBaseManager
#else
class ontologyBaseManager
#endif
{
 public:

  ontologyBaseManager(string sHost, string sUser, string sPass, string sDB);
  ~ontologyBaseManager();

  bool initBase() ;
  // bool insertMessage(messageData message) ;
  // bool isMessageInBase(messageData message) ;
  bool openBase() ;
  bool closeBase() ;

  MYSQL * getConnector() { return _sqlConnector ; }
  string  getMysqlError() { return _sMysqlError ; }

  MYSQL_RES * executeQuery(const string sQuery, bool bVerbose = true, string *pErrorMessage = (string*) 0) ;

 protected:

  MYSQL * _sqlConnector ;
  int     _iInstanceCounter ;

  const char *  _host ;
  const char *  _user ;
  const char *  _pass ;
  const char *  _database ;

  string  _sMysqlError ;
} ;

#endif // !__NSSQL_H__

