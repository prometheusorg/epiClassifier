// -----------------------------------------------------------------------------
// NSPATHO.H
//
//  Contient les definitions des objets PATIENT et UTILISATEUR de NAUTILUS
//
//  1ere version : KRZISCH Ph. janvier 92   Dernière modif : 12/08/93
// -----------------------------------------------------------------------------

#ifndef __NSPATHO_H
# define __NSPATHO_H

# include <cstring>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#ifndef _ENTERPRISE_DLL
  # include "ns_sgbd\nsfiche.h"
#else
  # include "mysql/mysql.h"
#endif

# include "nssavoir/nspatbas.h"

#ifndef __linux__
# if defined(_DANSLEXIDLL)
#  define _CLASSELEXI __export
# else
#  define _CLASSELEXI __import
# endif
#endif

// Strings utiles a chercheGrammaire

# define ADJ_POSITION			"P"
# define AV_NOM      			"Pv"
# define AP_NOM						"Pp"
# define VALEUR_DE_H			"H"
# define H_MUET						"Hh"
# define H_SONORE					"HH"
# define PREPOSITION			"R"
# define POSTPOSITION			"O"

// Strings pour forceNombre

# define FORCE_NBR				"N"
# define FORCE_PLUR				"Np"
# define FORCE_SING				"Ns"
# define FORCE_X					"Nx"

// Taille des champs du fichier LEXIQUE

# define PATHO_LIBELLE_LEN    120
# define PATHO_CODE_LEN       BASE_LEXI_LEN
# define PATHO_GRAMMAIRE_LEN  3
# define PATHO_FREQ_LEN       1

// Indice des champs du fichier LEXIQUE

# define PATHO_LIBELLE_FIELD			1
# define PATHO_CODE_FIELD        	2
# define PATHO_GRAMMAIRE_FIELD   	3
# define PATHO_FREQ_FIELD        	4

// Taille des champs du fichier SAVOIR

# define SAVOIR_CODE_LEN   				6
# define SAVOIR_QUALIFIE_LEN     	5
# define SAVOIR_LIEN_LEN  				2
# define SAVOIR_QUALIFIANT_LEN   	5
# define SAVOIR_DEGRE_LEN  				5
# define SAVOIR_CLASSE_LEN  	    3
# define SAVOIR_SCENARIO_LEN  		6

// Indice des champs du fichier SAVOIR

# define SAVOIR_CODE_FIELD   			1
# define SAVOIR_QUALIFIE_FIELD   	2
# define SAVOIR_LIEN_FIELD  	    3
# define SAVOIR_QUALIFIANT_FIELD 	4
# define SAVOIR_DEGRE_FIELD  			5
# define SAVOIR_CLASSE_FIELD  		6
# define SAVOIR_SCENARIO_FIELD   	7

// -----------------------------------------------------------------------------
// Classe NSPatholog
// -----------------------------------------------------------------------------

// Objet contenant les données

#ifndef __linux__
class _CLASSELEXI NSPathologData
#else
class NSPathologData
#endif
{
 public:

#ifndef _ENTERPRISE_DLL

	// Variables de stockage des valeurs
	char libelle[PATHO_LIBELLE_LEN + 1] ;
	char code[PATHO_CODE_LEN + 1] ;
	char grammaire[PATHO_GRAMMAIRE_LEN + 1] ;
	char freq[PATHO_FREQ_LEN + 1] ;

  bool estNom()        { return ((grammaire[0] == 'M') || (grammaire[0] == 'F') || (grammaire[0] == 'N')) ; }
	bool estAdjectif()   { return (strcmp(grammaire, "ADJ") == 0) ; }
	bool estVerbe()      { return (grammaire[0] == 'V') ; }
	bool estInvariable() { return (grammaire[0] == 'I') ; }
	bool estAdverbe()    { return (strcmp(grammaire, "ADV") == 0) ; }

	bool estMasculin()   { return ((strcmp(grammaire, "MS") == 0) || (strcmp(grammaire, "MP") == 0)) ; }
	bool estFeminin()    { return ((strcmp(grammaire, "FS") == 0) || (strcmp(grammaire, "FP") == 0)) ; }
	bool estNeutre()     { return ((strcmp(grammaire, "NS") == 0) || (strcmp(grammaire, "NP") == 0)) ; }
	bool estSingulier()  { return ((strcmp(grammaire, "MS") == 0) || (strcmp(grammaire, "FS") == 0) || (strcmp(grammaire, "NS") == 0)) ; }
	bool estPluriel()    { return ((strcmp(grammaire, "MP") == 0) || (strcmp(grammaire, "FP") == 0) || (strcmp(grammaire, "NP") == 0)) ; }

  string getLabel()     { return string(libelle) ; }
  string getCode()      { return string(code) ; }
  string getGrammar()   { return string(grammaire) ; }
  string getFrequency() { return string(freq) ; }

#else

  string sLabel ;
  string sCode ;
  string sGrammar ;
  string sFrequency ;

  bool estNom()        { return (('M' == sLabel[0]) || ('F' == sLabel[0]) || ('N' == sLabel[0])) ; }
	bool estAdjectif()   { return (string("ADJ") == sGrammar) ; }
	bool estVerbe()      { return ('V' == sGrammar[0]) ; }
	bool estInvariable() { return ('I' == sGrammar[0]) ; }
	bool estAdverbe()    { return (string("ADV") == sGrammar) ; }

	bool estMasculin()   { return ((string("MS") == sGrammar) || (string("MP") == sGrammar)) ; }
	bool estFeminin()    { return ((string("FS") == sGrammar) || (string("FP") == sGrammar)) ; }
	bool estNeutre()     { return ((string("NS") == sGrammar) || (string("NP") == sGrammar)) ; }
	bool estSingulier()  { return ((string("MS") == sGrammar) || (string("FS") == sGrammar) || (string("NS") == sGrammar)) ; }
	bool estPluriel()    { return ((string("MP") == sGrammar) || (string("FP") == sGrammar) || (string("NP") == sGrammar)) ; }

#endif

	string libelleLong ;

	NSPathologData() ;
	NSPathologData(NSPathologData& rv) ;
	~NSPathologData() ;

	NSPathologData& operator=(NSPathologData src) ;
	int 						operator==(const NSPathologData& o) ;

	void						metABlanc() ;
	void						fabriqueLibelleLong() ;
	void						donneLibelleChoix(string *pLibelle) ;
	void						donneLibelleAffiche(string *pLibelle, int iDeclinaison = 0) ;
	void						traiteOrthographe(string *pMot, string *pOrthographe, int iDeclinaison) ;

	void						donneGenre(GENRE *pGenre) ;
	void 						donneGenrePluriel(GENRE *pGenre) ;
	void 						donneGenreSingulier(GENRE *pGenre) ;

	string					chercheGrammaire(string sAChercher) ;

	bool						estReel() ;

#ifdef _ENTERPRISE_DLL

  string getLabel()     { return sLabel ; }
  string getCode()      { return sCode ; }
  string getGrammar()   { return sGrammar ; }
  string getFrequency() { return sFrequency ; }

  void   setLabel(string sSt)     { sLabel     = sSt ; }
  void   setCode(string sSt)      { sCode      = sSt ; }
  void   setGrammar(string sSt)   { sGrammar   = sSt ; }
  void   setFrequency(string sSt) { sFrequency = sSt ; }

#endif
} ;

// Objet dérivé de NSFiche servant aux accès de base de données

#ifndef _ENTERPRISE_DLL
class _CLASSELEXI NSPatholog : public NSFiche
#else
#ifndef __linux__
class _CLASSELEXI NSPatholog : public NSSuperRoot
#else
class NSPatholog : public NSSuperRoot
#endif
#endif
{
 public:
	// Objet qui contient les données
	NSPathologData _Donnees ;

	NSPatholog(NSSuper* pSuper, string sLangue);
	NSPatholog(NSPatholog& rv);
	~NSPatholog();

	void					metABlanc() 					{ _Donnees.metABlanc() ; }

	void					alimenteFiche() ;
	void					videFiche() ;

#ifndef _ENTERPRISE_DLL
	DBIResult			open() ;
	DBIResult			getPatRecord() ;

  bool					trouvePathologData(string *pCode, NSPathologData *pData, bool afficheErreur = true) ;
	DBIResult			trouveCode(string *pCode, DBISearchCond searchMode = keySEARCHEQ, DBILockType Blocage = dbiWRITELOCK, bool bErrMessage = true) ;
	DBIResult			trouveLibelle(string *pLibelle, DBISearchCond searchMode = keySEARCHEQ, DBILockType Blocage = dbiWRITELOCK) ;
#else
  bool					trouvePathologData(string sLang, string *pCode, NSPathologData *pData, bool afficheErreur = true) ;
  bool          trouveCode(string sLang, string *pCode, bool bVerbose = true) ;
	bool          trouveLibelle(string sLang, string *pLibelle) ;
  bool          trouveLibelleApproche(string sLang, string *pLibelle) ;
#endif

  string				*donneLibelle()				{ return &(_Donnees.libelleLong) ; }

#ifndef _ENTERPRISE_DLL
	string  			donneLibelleLexique() { return string(_Donnees.libelle) ; }
	char					*donneCode() 	 				{ return _Donnees.code ; }
#else
  string  			donneLibelleLexique() { return _Donnees.getLabel() ; }

  string getLabel()     { return _Donnees.getLabel() ; }
  string getCode()      { return _Donnees.getCode() ; }
  string getGrammar()   { return _Donnees.getGrammar() ; }
  string getFrequency() { return _Donnees.getFrequency() ; }
#endif

	virtual bool	Create() ;
	virtual bool	Modify() ;

	NSPatholog& 	operator=(NSPatholog src) ;
	int						operator==( const NSPatholog& o ) ;

 protected:

	string _sLangue ;

#ifdef _ENTERPRISE_DLL
  string getTableName(string sLanguage) ;
  void alimenteFiche(MYSQL_ROW *pRow) ;
  bool executeQuery(string sQuery, bool bVerbose) ;

  string _sTableName ;
#endif
} ;

//---------------------------------------------------------------------------
//  Classe NSPatInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
#ifndef __linux__
class _CLASSELEXI NSPathologInfo
#else
class NSPathologInfo
#endif
{
 public:
	// Objet qui contient les données
	NSPathologData _Donnees ;

	NSPathologInfo() ;
	NSPathologInfo(NSPatholog *) ;
	NSPathologInfo(NSPathologInfo& rv) ;
	~NSPathologInfo() ;

	string					*donneLibelle() { return &(_Donnees.libelleLong) ; }

#ifndef _ENTERPRISE_DLL
	char						*donneCode() 	  { return _Donnees.code ; }
#endif

	NSPathologInfo&	operator=(NSPathologInfo src) ;
	int							operator==( const NSPathologInfo& o) ;

#ifdef _ENTERPRISE_DLL
  string getLabel()     { return _Donnees.getLabel() ; }
  string getCode()      { return _Donnees.getCode() ; }
  string getGrammar()   { return _Donnees.getGrammar() ; }
  string getFrequency() { return _Donnees.getFrequency() ; }
#endif
} ;

//---------------------------------------------------------------------------
//  Classe NSSavoir
//---------------------------------------------------------------------------

// Objet contenant les données

#ifndef __linux__
class _CLASSELEXI NSSavoirData
#else
class NSSavoirData
#endif
{
 public:

#ifndef _ENTERPRISE_DLL

	// Variables de stockage des valeurs
	char	code[SAVOIR_CODE_LEN + 1] ;
	char	qualifie[SAVOIR_QUALIFIE_LEN + 1] ;
	char	lien[SAVOIR_LIEN_LEN + 1] ;
	char	qualifiant[SAVOIR_QUALIFIANT_LEN + 1] ;
	char	degre[SAVOIR_DEGRE_LEN + 1] ;
	char	classe[SAVOIR_CLASSE_LEN + 1] ;
	char	scenario[SAVOIR_SCENARIO_LEN + 1] ;

  string getCode()      { return string(code) ; }
	string getQualified() { return string(qualifie) ; }
	string getLink()      { return string(lien) ; }
	string getQualifier() { return string(qualifiant) ; }
	string getLevel()     { return string(degre) ; }
	string getClass()     { return string(classe) ; }
	string getScenario()  { return string(scenario) ; }

#else

  string _sCode ;
	string _sQualified ;
	string _sLink ;
	string _sQualifier ;
	string _sLevel ;
	string _sClass ;
	string _sScenario ;

  string getCode()      { return _sCode ; }
	string getQualified() { return _sQualified ; }
	string getLink()      { return _sLink ; }
	string getQualifier() { return _sQualifier ; }
	string getLevel()     { return _sLevel ; }
	string getClass()     { return _sClass ; }
	string getScenario()  { return _sScenario ; }

  void   setCode(string sC)      { _sCode      = sC ; }
	void   setQualified(string sC) { _sQualified = sC ; }
	void   setLink(string sC)      { _sLink      = sC ; }
	void   setQualifier(string sC) { _sQualifier = sC ; }
	void   setLevel(string sC)     { _sLevel     = sC ; }
	void   setClass(string sC)     { _sClass     = sC ; }
	void   setScenario(string sC)  { _sScenario  = sC ; }

#endif

	NSSavoirData() ;
	NSSavoirData(NSSavoirData& rv) ;
	~NSSavoirData() ;

	NSSavoirData& operator=(NSSavoirData src) ;
	int 		  		operator==(const NSSavoirData& o) ;

	void					metABlanc() ;
} ;

// accés à la base des médicaments

#ifndef __linux__
class _CLASSELEXI NSLexiMed : public NSPatholog
#else
class NSLexiMed : public NSPatholog
#endif
{
 public:
	// Objet qui contient les données
	NSLexiMed(NSSuper* pSuper, string sLangue) ;
	NSLexiMed(NSLexiMed& rv) ;
	~NSLexiMed() ;
#ifndef _ENTERPRISE_DLL
	DBIResult	open() ;		//ne redefénir qu cette fonction
#endif
} ;

// Objet dérivé de NSFiche servant aux accès de base de données

#ifndef _ENTERPRISE_DLL
class _CLASSELEXI NSSavoir : public NSFiche
#else
#ifndef __linux__
class _CLASSELEXI NSSavoir : public NSSuperRoot
#else
class NSSavoir : public NSSuperRoot
#endif
#endif
{
 public:
	// Objet qui contient les données
	NSSavoirData	*pDonnees ;

	NSSavoir(NSSuper* pSuper) ;
	NSSavoir(NSSavoir& rv) ;
	~NSSavoir() ;

	void 					metABlanc() { pDonnees->metABlanc() ; }

	void 					alimenteFiche() ;
	void 					videFiche() ;
#ifndef _ENTERPRISE_DLL
	DBIResult 		open() ;
	DBIResult 		getPatRecord() ;
#else
  void          alimenteFiche(MYSQL_ROW *pRow) ;
  bool          executeQuery(string sQuery, bool bVerbose) ;
#endif

	virtual bool	Create();
	virtual bool	Modify();

	NSSavoir&			operator=(NSSavoir src) ;
	int						operator==(const NSSavoir& o) ;

  string getCode()      { return pDonnees->getCode() ; }
	string getQualified() { return pDonnees->getQualified() ; }
	string getLink()      { return pDonnees->getLink() ; }
	string getQualifier() { return pDonnees->getQualifier() ; }
	string getLevel()     { return pDonnees->getLevel() ; }
	string getClass()     { return pDonnees->getClass() ; }
	string getScenario()  { return pDonnees->getScenario() ; }
} ;


// -----------------------------------------------------------------------------
//  Classe NSPatInfo  (destinée à être stockée dans une Array)
// -----------------------------------------------------------------------------
#ifndef __linux__
class _CLASSELEXI NSSavoirInfo
#else
class NSSavoirInfo
#endif
{
 public:
	// Objet qui contient les données
	NSSavoirData	*pDonnees ;

	NSSavoirInfo() ;
	NSSavoirInfo(NSSavoir *) ;
	NSSavoirInfo(NSSavoirInfo& rv) ;
	~NSSavoirInfo() ;

	NSSavoirInfo& operator=(NSSavoirInfo src) ;
	int						operator==(const NSSavoirInfo& o) ;
} ;

#endif    // __NSPATHO_H

