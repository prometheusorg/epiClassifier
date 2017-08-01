// -----------------------------------------------------------------------------
//    FLECIHES.H
//    NAUTILUS - janvier 2002
//
//    Définition des objets de base de donnees
// -----------------------------------------------------------------------------

#ifndef __FLECHIESDB_H__
# define __FLECHIESDB_H__

#  include <vector>

#ifndef _ENTERPRISE_DLL
  #include "partage/nsglobal.h"
  #include "ns_sgbd/nsfiche.h"
#else
  #include "enterpriseLus/nsglobalLus.h"
  #include "nsdatabaseLus/nsSQL.h"
#endif

#ifndef __linux__
# if defined(_EPISODUSDLL)
#  define _EPISODUS __export
# else
#  define _EPISODUS __import
# endif
#endif

// Taille des champs du fichier FLECHIES
# define FLECHIES_ID_LEN							 4
# define FLECHIES_LIBELLE_LEN        255
# define FLECHIES_CODE_LEN             6

// Indice des champs du fichier FLECHIES
# define FLECHIES_ID_FIELD					 	 1
# define FLECHIES_LIBELLE_FIELD      	 2
# define FLECHIES_CODE_FIELD         	 3

// **************************** Table flechies.DB ****************************

// -----------------------------------------------------------------------------
//  Classe NSFlechiesData
//
// Objet contenant les données
//---------------------------------------------------------------------------
#ifndef __linux__
class _EPISODUS NSFlechiesData
#else
class NSFlechiesData
#endif
{
  public:

	  NSFlechiesData() ;
	  NSFlechiesData(NSFlechiesData& rv) ;
	  ~NSFlechiesData() ;

	  NSFlechiesData&	operator=(NSFlechiesData src) ;
	  int							operator==(const NSFlechiesData& o) ;

	  void 						metAZero() ;

    string getId()    { return sId ; }
    string getLabel() { return sLibelle ; }
    string getCode()  { return sCode ; }

    void setId(string sSt)    { sId      = sSt ; }
    void setLabel(string sSt) { sLibelle = sSt ; }
    void setCode(string sSt)  { sCode    = sSt ; }

	  // variables
	  string sId ;
	  string sLibelle ;
	  string sCode ;
} ;

#ifndef _ENTERPRISE_DLL
class NSFlechies ;
#endif

// -----------------------------------------------------------------------------
//  Classe NSFlechiesInfo  (destinée à être stockée dans une Array)
// -----------------------------------------------------------------------------
#ifndef __linux__
class _EPISODUS NSFlechiesInfo
#else
class NSFlechiesInfo
#endif
{
  public:

	  NSFlechiesInfo() ;
#ifndef _ENTERPRISE_DLL
	  NSFlechiesInfo(NSFlechies *) ;
#endif
	  NSFlechiesInfo(NSFlechiesInfo& rv) ;
	  ~NSFlechiesInfo() ;

	  NSFlechiesInfo& operator=(NSFlechiesInfo src) ;
	  int 		      	operator==(const NSFlechiesInfo& o) ;

#ifdef _ENTERPRISE_DLL
	  void					  alimenteFiche(MYSQL_ROW *pRow) ;
#endif

    string getID()    { return pDonnees->sId ; }
    string getLabel() { return pDonnees->sLibelle ; }
    string getCode()  { return pDonnees->sCode ; }

	  // variables
	  NSFlechiesData	*pDonnees ;
} ;

// -----------------------------------------------------------------------------
//  Classe NSEpiContact
//
// Objet dérivé de NSFiche servant aux accès de base de données
// -----------------------------------------------------------------------------
#ifndef __linux__
class _EPISODUS NSFlechies : public NSFlechiesInfo,
#else
class NSFlechies : public NSFlechiesInfo,
#endif
#ifndef _ENTERPRISE_DLL
                             public NSFiche
#else
                             public NSSuperRoot
#endif
{
  public:

 	  enum BASEFLEX { flechies = 1, ortho, local, express } ;

	  int						iTypeBase ;
	  string				sFileName ;
	  string				sFileLib ;

	  NSFlechies(NSSuper *pSuper, int iBase = flechies) ;
	  NSFlechies(NSFlechies& rv) ;
	  ~NSFlechies() ;

#ifndef _ENTERPRISE_DLL
	  void					alimenteFiche() ;
	  void					videFiche() ;
	  DBIResult			open(bool bOpenShared = true) ;
#else
    void					alimenteFiche(MYSQL_ROW *pRow) ;
#endif

	  virtual bool	Create() ;
	  virtual bool	Modify() ;

	  NSFlechies&		operator=(NSFlechies src) ;
	  int 	      	operator==(const NSFlechies& o) ;
} ;

#endif    // __FLECHIESDB_H__
