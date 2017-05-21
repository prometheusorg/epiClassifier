// -----------------------------------------------------------------------------
// EPTABLES.H
// NAUTILUS juillet 2001
//
// Definition des objets de base de donnee
// -----------------------------------------------------------------------------

#ifndef __EPTABLES_H
# define __EPTABLES_H

# include "partage/nsvector.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "nsdatabaseLus/nsSQL.h"
#else
  #include "partage\nsdivfct.h"
  #include "ns_sgbd\nsfiche.h"
#endif

# if defined(_EPISODUSDLL)
#  define _EPISODUS __export
# else
#  define _EPISODUS __import
# endif

// Taille des champs du fichier CLASSER
// Criteres d'inclusion et d'exclusion de classification
# define CLASSER_CODE_LEN 		       5  // Code NAUTILUS
# define CLASSER_CLASSIF_LEN 		     5  // Classification
# define CLASSER_CRITERE_LEN  	     1  // Obligatoire Possible Interdit
# define CLASSER_RELATION_LEN  	    20  // éléments de classification

// Indice des champs du fichier CLASSER
# define CLASSER_CODE_FIELD  		     1
# define CLASSER_CLASSIF_FIELD 		   2
# define CLASSER_CRITERE_FIELD  		 3
# define CLASSER_RELATION_FIELD  	   4

// Taille des champs du fichier CLASSIF
// Classifications : code, libelle, chapitre
# define CLASSIF_CLASSIF_LEN 		     5  // Classification
# define CLASSIF_CODE_LEN 		      10  // Code NAUTILUS
# define CLASSIF_LIBELLE_LEN 	     200  // Classification
# define CLASSIF_CHAPITRE_LEN  	     4  // Obligatoire Possible Interdit


// Indice des champs du fichier CLASSER
# define CLASSIF_CLASSIF_FIELD 		   1
# define CLASSIF_CODE_FIELD 		     2
# define CLASSIF_LIBELLE_FIELD 	     3
# define CLASSIF_CHAPITRE_FIELD  	   4

// Taille des champs du fichier THESAURUS
# define THESAURUS_ORIGINE_LEN 		    1
# define THESAURUS_IBUI_LEN 		    	8
# define THESAURUS_LABEL_LEN 		    255
# define THESAURUS_ICPC2_LEN 	       10
# define THESAURUS_ICD10_LEN  		   10

// Indice des champs du fichier THESAURUS
#ifndef _ENTERPRISE_DLL

# define THESAURUS_IBUI_FIELD					1
# define THESAURUS_FR_LABEL_FIELD			2
# define THESAURUS_NL_LABEL_FIELD		  3
# define THESAURUS_ICPC2_FIELD				4
# define THESAURUS_ICD10_FIELD				5
# define THESAURUS_ICPC2_2_FIELD			6
# define THESAURUS_ICD10_2_FIELD			7
# define THESAURUS_ICPC2_1X_FIELD		  8
# define THESAURUS_ICD10_1X_FIELD		  9
# define THESAURUS_ICPC2_2X_FIELD		 10
# define THESAURUS_ICD10_2X_FIELD		 11
# define THESAURUS_ICPC2_1Y_FIELD		 12
# define THESAURUS_ICD10_1Y_FIELD		 13
# define THESAURUS_ICPC2_2Y_FIELD		 14
# define THESAURUS_ICD10_2Y_FIELD		 15

#else

# define THESAURUS_ID_FIELD		  			1
# define THESAURUS_ORIGIN_FIELD				2
# define THESAURUS_IBUI_FIELD					3
# define THESAURUS_FR_LABEL_FIELD			4
# define THESAURUS_NL_LABEL_FIELD		  5
# define THESAURUS_ICPC2_FIELD				6
# define THESAURUS_ICD10_FIELD				7
# define THESAURUS_ICPC2_2_FIELD			8
# define THESAURUS_ICD10_2_FIELD			9
# define THESAURUS_ICPC2_1X_FIELD		 10
# define THESAURUS_ICD10_1X_FIELD		 11
# define THESAURUS_ICPC2_2X_FIELD		 12
# define THESAURUS_ICD10_2X_FIELD		 13
# define THESAURUS_ICPC2_1Y_FIELD		 14
# define THESAURUS_ICD10_1Y_FIELD		 15
# define THESAURUS_ICPC2_2Y_FIELD		 16
# define THESAURUS_ICD10_2Y_FIELD		 17

#endif

// -----------------------------------------------------------------------------
//
// Table CLASSER.DB
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Classe NSEpiClasserData
// Objet contenant les donnees
// -----------------------------------------------------------------------------
#ifndef __linux__
class _EPISODUS NSEpiClasserData
#else
class NSEpiClasserData
#endif
{
  protected:

    // variables
    string _sCode ;
    string _sClassification ;
    string _sCriteria ;
    string _sRelation ;

  public:

    // constructors/destructor
    NSEpiClasserData() ;
    NSEpiClasserData(const NSEpiClasserData& rv) ;
    ~NSEpiClasserData() ;

    // operators
    NSEpiClasserData& operator=(const NSEpiClasserData& src) ;
    int 		          operator==(const NSEpiClasserData& o) ;

    // functions
    void                metAZero() ;

    string getCode()           { return _sCode ; }
    string getClassification() { return _sClassification ; }
    string getCriteria()       { return _sCriteria ; }
    string getRelation()       { return _sRelation ; }

    void   setCode(string sSt)           { _sCode           = sSt ; }
    void   setClassification(string sSt) { _sClassification = sSt ; }
    void   setCriteria(string sSt)       { _sCriteria       = sSt ; }
    void   setRelation(string sSt)       { _sRelation       = sSt ; }
} ;

#ifndef _ENTERPRISE_DLL
class NSEpiClasser ;
#endif

// -----------------------------------------------------------------------------
// Classe NSEpiClasserInfo  (destinee a etre stockee dans une Array)
// -----------------------------------------------------------------------------
#ifndef __linux__
class _EPISODUS NSEpiClasserInfo
#else
class NSEpiClasserInfo
#endif
{
  public:

    NSEpiClasserData _Donnees ;

    // constructors/destructor
    NSEpiClasserInfo() ;
#ifndef _ENTERPRISE_DLL
    NSEpiClasserInfo(NSEpiClasser *) ;
#endif
    NSEpiClasserInfo(NSEpiClasserInfo& rv) ;
    ~NSEpiClasserInfo() ;

#ifdef _ENTERPRISE_DLL
    void alimenteFiche(MYSQL_ROW *pRow) ;
    bool donneRelations(string *pResult, string sCode, string sClassification, string sCritere, ontologyBaseManager* pOntologyManager) ;
#endif

    string getCode()           { return _Donnees.getCode() ; }
    string getClassification() { return _Donnees.getClassification() ; }
    string getCriteria()       { return _Donnees.getCriteria() ; }
    string getRelation()       { return _Donnees.getRelation() ; }

    void   setCode(string sSt)           { _Donnees.setCode(sSt) ; }
    void   setClassification(string sSt) { _Donnees.setClassification(sSt) ; }
    void   setCriteria(string sSt)       { _Donnees.setCriteria(sSt) ; }
    void   setRelation(string sSt)       { _Donnees.setRelation(sSt) ; }

    // operators
    NSEpiClasserInfo& operator=(NSEpiClasserInfo src) ;
    int 		          operator==(const NSEpiClasserInfo& o) ;
} ;

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------------------
// Classe NSEpiClasser
// Objet derive de NSFiche servant aux acces de base de donnees
// -----------------------------------------------------------------------------
class _EPISODUS NSEpiClasser : public NSEpiClasserInfo,
                               public NSFiche
{
  public:

    // constructors/destructor
    NSEpiClasser(NSSuper *pSuper) ;
    NSEpiClasser(NSEpiClasser& rv) ;
    ~NSEpiClasser() ;

    // operators
    NSEpiClasser& operator=(NSEpiClasser src) ;
    int 	        operator==(const NSEpiClasser& o) ;

    // functions
    DBIResult     donneRelations(string *pResult, string sCode, string sClassification, string sCritere) ;
    void          alimenteFiche() ;
    void          videFiche() ;
    DBIResult     open() ;

    virtual bool  Create() ;
    virtual bool  Modify() ;
} ;
#endif

// -----------------------------------------------------------------------------
//
// Table THESAURUS.DB
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Classe NSThesaurusData
// Objet contenant les donnees
// -----------------------------------------------------------------------------
#ifndef __linux__
class _EPISODUS NSThesaurusData
#else
class NSThesaurusData
#endif
{
  public:

    int    _iId ;
    string _sOrigine ;
    string _ibui ;
    string _labelFr ;
    string _labelNl ;
    string _icpc2 ;
    string _icd10 ;
    string _icpc2_2 ;
    string _icd10_2 ;
    string _icpc2_1X ;
    string _icd10_1X ;
    string _icpc2_2X ;
    string _icd10_2X ;
    string _icpc2_1Y ;
    string _icd10_1Y ;
    string _icpc2_2Y ;
    string _icd10_2Y ;

    // constructors/destructor
    NSThesaurusData() ;
    NSThesaurusData(NSThesaurusData& rv) ;
    ~NSThesaurusData() ;

    // operators
    NSThesaurusData& operator=(NSThesaurusData src) ;
    int 		         operator==(const NSThesaurusData& o) ;

    // functions
    void             metAZero() ;
} ;

#ifndef _ENTERPRISE_DLL
class NSThesaurus ;
#endif

// -----------------------------------------------------------------------------
// Classe NSThesaurusInfo  (destinee a etre stockee dans une Array)
// -----------------------------------------------------------------------------
#ifndef __linux__
class _EPISODUS NSThesaurusInfo
#else
class NSThesaurusInfo
#endif
{
  public:

    // variables
    NSThesaurusData _Donnees ;

    // constructors/destructor
    NSThesaurusInfo() ;
#ifndef _ENTERPRISE_DLL
    NSThesaurusInfo(NSThesaurus *) ;
#endif
    NSThesaurusInfo(NSThesaurusInfo& rv) ;
    ~NSThesaurusInfo() ;

    // arrange the result "as a" Classer record for a given classification
    //
    string           getClasserString(string sClassif) ;

    string           getLabelFr() { return _Donnees._labelFr ; }

    // operators
    NSThesaurusInfo& operator=(NSThesaurusInfo src) ;
    int 		       	 operator==(const NSThesaurusInfo& o) ;

#ifdef _ENTERPRISE_DLL
    void             alimenteFiche(MYSQL_ROW *pRow) ;
    void             alimenteFicheSentinelle(MYSQL_ROW *pRow) ;
#endif

  protected:

    void             addALaClasser(string* pResult, string sCode) ;
} ;

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------------------
// Classe NSThesaurus
// Objet derive de NSFiche servant aux acces de base de donnees
// -----------------------------------------------------------------------------
class _EPISODUS NSThesaurus : public NSThesaurusInfo,
                              public NSFiche
{
  public:

    // constructors/destructor
    NSThesaurus(NSSuper *pSuper) ;
    NSThesaurus(NSThesaurus& rv) ;
    ~NSThesaurus() ;

    // operators
    NSThesaurus&    		operator=(NSThesaurus src) ;
    int 	           		operator==(const NSThesaurus& o) ;

    // functions
    void                alimenteFiche() ;
    void                videFiche() ;
    DBIResult           open() ;

    virtual bool        Create() ;
    virtual bool        Modify() ;
} ;
#endif

// -----------------------------------------------------------------------------
// Classe NSEpiClassifData
// Objet contenant les donnees
// -----------------------------------------------------------------------------
#ifndef __linux__
class _EPISODUS NSEpiClassifData
#else
class NSEpiClassifData
#endif
{
  protected:

    string _sClassification ;
    string _sCode ;
    string _sLabel ;
    string _sChapter ;

  public:

    // constructors/destructor
    NSEpiClassifData() ;
    NSEpiClassifData(const NSEpiClassifData& rv) ;
    ~NSEpiClassifData() ;

    // operators
    NSEpiClassifData&   operator=(const NSEpiClassifData& src) ;
    int 		            operator==(const NSEpiClassifData& o) ;

    // functions
    void                metAZero() ;

    string getClassification() { return _sClassification ; }
    string getCode()           { return _sCode ; }
    string getLabel()          { return _sLabel ; }
    string getChapter()        { return _sChapter ; }

    void   setClassification(string sSt) { _sClassification = sSt ; }
    void   setCode(string sSt)           { _sCode           = sSt ; }
    void   setLabel(string sSt)          { _sLabel          = sSt ; }
    void   setChapter(string sSt)        { _sChapter        = sSt ; }
} ;

#ifndef _ENTERPRISE_DLL
class NSEpiClassif ;
#endif

enum ICPC_CATEGORY  { IcpcUndefined = 0, IcpcSymptom, IcpcProcedure, IcpcPathology } ;
enum ICPC_PATHOLOGY { IcpcUndefinedPatho = 0, IcpcInfection, IcpcNeoplasm, IcpcTraumatism, IcpcCongenital, IcpcOtherPatho } ;

// -----------------------------------------------------------------------------
// Classe NSEpiClassifInfo  (destinee a etre stockee dans une Array)
// -----------------------------------------------------------------------------
#ifndef __linux__
class _EPISODUS NSEpiClassifInfo
#else
class NSEpiClassifInfo
#endif
{
  friend class NSEpiClassifArray ;

  protected:

    // variables
    NSEpiClassifData _Donnees ;

  public:

    // constructors/destructor
    NSEpiClassifInfo() ;
#ifndef _ENTERPRISE_DLL
    NSEpiClassifInfo(NSEpiClassif *) ;
#endif
    NSEpiClassifInfo(NSEpiClassifInfo& rv) ;
    ~NSEpiClassifInfo() ;

    string getClassification() { return _Donnees.getClassification() ; }
    string getCode()           { return _Donnees.getCode() ; }
    string getLabel()          { return _Donnees.getLabel() ; }
    string getChapter()        { return _Donnees.getChapter() ; }

    void   setClassification(string sSt) { _Donnees.setClassification(sSt) ; }
    void   setCode(string sSt)           { _Donnees.setCode(sSt) ; }
    void   setLabel(string sSt)          { _Donnees.setLabel(sSt) ; }
    void   setChapter(string sSt)        { _Donnees.setChapter(sSt) ; }

    ICPC_CATEGORY  getIcpcCategory() ;
    ICPC_PATHOLOGY getIcpcPathology() ;

    // operators
    NSEpiClassifInfo&   operator=(const NSEpiClassifInfo& src) ;
#ifndef _ENTERPRISE_DLL
    NSEpiClassifInfo&   operator=(const NSEpiClassif& src) ;
#endif
    int 		            operator==(const NSEpiClassifInfo& o) ;

#ifdef _ENTERPRISE_DLL
    void                alimenteFiche(MYSQL_ROW *pRow) ;
#endif
} ;

typedef vector<NSEpiClassifInfo *>          NSEpiClassifInfoVector ;
typedef NSEpiClassifInfoVector::iterator    NSEpiClassifInfoIter ;

#ifndef __linux__
class _EPISODUS NSEpiClassifInfoArray : public NSEpiClassifInfoVector
#else
class NSEpiClassifInfoArray : public NSEpiClassifInfoVector
#endif
{
  public:

    NSEpiClassifInfoArray() : NSEpiClassifInfoVector() {} ;
    ~NSEpiClassifInfoArray() ;

    bool ajouteElement(NSEpiClassifInfo *pElement) ;
    void vider() ;
} ;

#ifndef __linux__
class _EPISODUS NSEpiClassifArray : public NSEpiClassifInfoVector
#else
class NSEpiClassifArray : public NSEpiClassifInfoVector
#endif
{
  public:

    // constructors/destructor
    NSEpiClassifArray() ;
    NSEpiClassifArray(string sFile) ;
    ~NSEpiClassifArray() ;

    // functions
    bool fillArray() ;
    bool searchKey(string sClassif, string sCode) ;
    bool vider() ;

    string getClassification() { return _Donnees.getClassification() ; }
    string getCode()           { return _Donnees.getCode() ; }
    string getLabel()          { return _Donnees.getLabel() ; }
    string getChapter()        { return _Donnees.getChapter() ; }

    // variables
    string           _sFileName ;
    NSEpiClassifData _Donnees ;
} ;

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------------------
// Classe NSEpiClassif
// Objet derive de NSFiche servant aux acces de base de donnees
// -----------------------------------------------------------------------------
class _EPISODUS NSEpiClassif : public NSEpiClassifInfo, public NSFiche
{
  public:

    // constructors/destructor
    NSEpiClassif(NSSuper *pSuper) ;
    NSEpiClassif(NSEpiClassif& rv) ;
    ~NSEpiClassif() ;

    // operators
    NSEpiClassif& operator=(NSEpiClassif src) ;
    int           operator==(const NSEpiClassif& o) ;

  // functions
#ifdef _ENTERPRISE_DLL
    void          alimenteFiche(MYSQL_ROW *pRow) ;
#else
    void          alimenteFiche() ;
    void          videFiche() ;
    DBIResult     open() ;
#endif

    virtual bool  Create() ;
    virtual bool  Modify() ;

    bool          initArrayFromCodeList(string sClassification, ClasseStringVector* pVectCodes, NSEpiClassifArray* pResultArray) ;
} ;
#endif

// -----------------------------------------------------------------------------
// Définition de NSCodPrestArray (Array de NSCodPrestInfo(s))
// -----------------------------------------------------------------------------
/*
typedef vector<NSCodPrestInfo *>            NSFicheCodPrestArray ;
typedef NSFicheCodPrestArray::iterator      NSCodPrestIter ;

class NSCodPrestArray : public NSFicheCodPrestArray
{
 public:
  // constructors/destructor
  NSCodPrestArray() : NSFicheCodPrestArray() {}
  NSCodPrestArray(NSCodPrestArray& rv) ;
  virtual ~NSCodPrestArray() ;

  void                vider() ;
} ;
*/

// -----------------------------------------------------------------------------
#endif // __EPTABLES_H

