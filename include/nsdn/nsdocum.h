//---------------------------------------------------------------------------
//     NSDOCUM.H
//
//  Contient les definitions des objets document
//
//  1ere version : PA juillet 94   Derniere modif : 18/07/94
//---------------------------------------------------------------------------
#ifndef __NSDOCUM_H
#define __NSDOCUM_H

#include <vector>
#include <cstring>

class NSPatInfo ;
class NSPersonGraphManager ;

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "partage/nsvector.h"
#include "nsbb/nspatpat.h"

#ifndef __linux__
#if defined(_DANSNSDNDLL)
	#define _CLASSENSDN __export
#else
	#define _CLASSENSDN __import
#endif
#endif

//
// Taille des champs du fichier DOCUMENT
//

// #define DOC_CODE_DOCUM_LEN     6  // defined inside nsglobal.h
#define DOC_CREATEUR_LEN       PAT_NSS_LEN
#define DOC_CREATION_LEN	     14
#define DOC_NOM_LEN			       1024
#define DOC_ACCES_LEN		       2
#define DOC_TYPE_LEN		       5
#define DOC_LOCALISATION_LEN   4
#define DOC_FICHIER_LEN		     100  // pour stocker les noms longs avec Pathname
#define DOC_ENTETE_LEN		     50
#define DOC_EMPLACEMENT_LEN    14
#define DOC_INTERET_LEN		     1
#define DOC_COMMENTAIRE_LEN	   4
#define DOC_VISIBLE_LEN		     1
#define DOC_TRAN_LEN		       5
#define DOC_CRC_LEN			       4

//
// Ordre des champs du fichier DOCUMENT
//
#define DOC_PAT_FIELD  		  	  1
#define DOC_CODE_FIELD  		  2
#define DOC_NOM_FIELD			  3
#define DOC_CREATION_FIELD		  4
#define DOC_ACCES_FIELD			  5
#define DOC_CREATEUR_FIELD		  6
#define DOC_TYPE_FIELD			  7
#define DOC_LOCALISATION_FIELD    8
#define DOC_FICHIER_FIELD		  9
#define DOC_ENTETE_FIELD		 10
#define DOC_EMPLACEMENT_FIELD    11
#define DOC_INTERET_FIELD		 12
#define DOC_COMMENTAIRE_FIELD	 13
#define DOC_VISIBLE_FIELD		 14
#define DOC_TRAN_NEW_FIELD		 15
#define DOC_TRAN_DEL_FIELD		 16
#define DOC_CRC_FIELD	 		 17

//
// Taille des champs du fichier CHEMISES
//
#ifndef _MUE
#define CHE_CODE_LEN  		   9
#else
#define CHE_CODE_LEN          10
#endif

#define CHE_NOM_LEN			  35
#define CHE_CREATION_LEN	   8
#define CHE_ACCES_LEN		   2
#define CHE_CREATEUR_LEN	   3
#define CHE_COMMENTAIRE_LEN	   4

//
// Ordre des champs du fichier CHEMISES
//
#define CHE_CODE_FIELD  		 1
#define CHE_NOM_FIELD			 2
#define CHE_CREATION_FIELD		 3
#define CHE_ACCES_FIELD			 4
#define CHE_CREATEUR_FIELD		 5
#define CHE_COMMENTAIRE_FIELD	 6

//
// Taille des champs du fichier SEJOUR
//
#define CSEJ_NSS_LEN         6
#define CSEJ_CODE_LEN        4
#define CSEJ_NUMERO_LEN     20
#define CSEJ_DEBUT_LEN      12
#define CSEJ_FIN_LEN 	    12
#define CSEJ_UNITE_LEN      10

//
// Ordre des champs du fichier SEJOUR
//
#define CSEJ_NSS_FIELD      1
#define CSEJ_CODE_FIELD     2
#define CSEJ_NUMERO_FIELD   3
#define CSEJ_DEBUT_FIELD    4
#define CSEJ_FIN_FIELD      5
#define CSEJ_UNITE_FIELD    6

//
// Taille des champs du fichier ARMOIRES
//
#define ARM_CODE_LEN  		  	3
#define ARM_NOM_LEN			   35
#define ARM_CREATION_LEN		8
#define ARM_ACCES_LEN			2
#define ARM_CREATEUR_LEN		3
#define ARM_COMMENTAIRE_LEN	    4

//
// Ordre des champs du fichier ARMOIRES
//
#define ARM_CODE_FIELD  		 1
#define ARM_NOM_FIELD			 2
#define ARM_CREATION_FIELD		 3
#define ARM_ACCES_FIELD			 4
#define ARM_CREATEUR_FIELD		 5
#define ARM_COMMENTAIRE_FIELD	 6

//
// Taille des champs du fichier CHEM_DOC
//
#ifndef _MUE
#define CDO_CHEMISE_LEN    9
#else
#define CDO_CHEMISE_LEN   10
#endif

#define CDO_ORDRE_LEN 	   2

#ifndef _MUE
#define CDO_DOCUMENT_LEN  11
#else
#define CDO_DOCUMENT_LEN  13
#endif

//
// Ordre des champs du fichier CHEM_DOC
//
#define CDO_CHEMISE_FIELD   1
#define CDO_ORDRE_FIELD 	2
#define CDO_DOCUMENT_FIELD	3

//
// Taille des champs du fichier ARM_CHEM
//
#define ACH_CHEMISE_LEN  	9
#define ACH_ARMOIRE_LEN 	3

//
// Ordre des champs du fichier ARM_CHEM
//
#define ACH_CHEMISE_FIELD  1
#define ACH_ARMOIRE_FIELD  2

//
// Taille des champs du fichier DOCMODEL
//
#define DMO_TYPE_LEN      	   5
#define DMO_NOM_LEN      	  20
#define DMO_AJOUT_DATE_LEN     1
#define DMO_ARMOIRE_LEN        3
#define DMO_CHEMISE_LEN        3
#define DMO_CREE_CHEMISE_LEN   1
#define DMO_ACCES_LEN      	   2
#define DMO_LOCALISATION_LEN   4

//
// Indice des champs du fichier DOCMODEL
//
#define DMO_TYPE_FIELD      	1
#define DMO_NOM_FIELD      	    2
#define DMO_AJOUT_DATE_FIELD    3
#define DMO_ARMOIRE_FIELD       4
#define DMO_CHEMISE_FIELD       5
#define DMO_CREE_CHEMISE_FIELD  6
#define DMO_ACCES_FIELD      	7
#define DMO_LOCALISATION_FIELD  8


//---------------------------------------------------------------------------
//  Classe NSDocument
//---------------------------------------------------------------------------

//
// Objet données
//
#ifndef __linux__
class _CLASSENSDN NSDocumentData
#else
class NSDocumentData
#endif
{
	public :
		//
		// Variables de stockage des valeurs
		//
/*
		char codePatient[PAT_NSS_LEN + 1];
		char codeDocument[DOC_CODE_DOCUM_LEN + 1];
		char nom[DOC_NOM_LEN + 1];
		char creation[DOC_CREATION_LEN + 1];
		char acces[DOC_ACCES_LEN + 1];
		char createur[DOC_CREATEUR_LEN + 1];
		char type[DOC_TYPE_LEN + 1];
		char localisation[DOC_LOCALISATION_LEN + 1];
// on enlève le define pour compatibilité avec nsm_version
// #ifndef _MUE
		char fichier[DOC_FICHIER_LEN + 1];
		char entete[DOC_ENTETE_LEN + 1];
// #endif
		char emplacement[DOC_EMPLACEMENT_LEN + 1];
		char interet[DOC_INTERET_LEN + 1];
		char commentaire[DOC_COMMENTAIRE_LEN + 1];
		char visible[DOC_VISIBLE_LEN + 1];
		char tranNew[DOC_TRAN_LEN + 1];
		char tranDel[DOC_TRAN_LEN + 1];
		char crc[DOC_CRC_LEN + 1];
*/

		string _sCodePatient ;
		string _sCodeDocument ;
		string _sNom ;
		string _sDateCreation ;
		string _sAcces ;
		string _sCreateur ;     // The user when document is created/imported
    string _sAuteur ;		   // The genuine author
    string _sDestinataire ; // The target
		string _sType ;
    string _sTypeComplet ;
		string _sLocalisation ;
		// string sFichier ;
		// string sEntete ;
		string _sEmplacement ;
		string _sInteret ;
		string _sCommentaire ;
		string _sVisible ;
		string _sTranNew ;
		string _sTranDel ;
		string _sCRC ;

		string _sFichier ;
		string _sTemplate ;
		string _sEnTete ;
		string _sTypeContenu ;
		string _sTypeContenuSens ;
		string _sDateExamen ;

		string 	_sRights ;

		void   metAZero() ;
		string donneDateCreation(string sLang = string("")) ;

		bool   estVisible() { return ((string("") != _sVisible) && ('1' == _sVisible[0])) ; }

		void   rendVisible()   { _sVisible = string("1") ; }
		void   rendInvisible() { _sVisible = string("0") ; }

		NSDocumentData() { metAZero() ; }
		NSDocumentData(const NSDocumentData& rv) ;

		string getID() { return _sCodePatient + _sCodeDocument ; }
    void   setID(string sId) ;

		NSDocumentData& operator=(const NSDocumentData& src) ;
		int             operator==(const NSDocumentData& o) ;
};

//
// Objet "Info" correspondant à NSDocument
// (destiné à être utilisé dans les containers)
//

#ifndef __linux__
class _CLASSENSDN NSDocumentInfo : public NSRoot
#else
class NSDocumentInfo : public NSRoot
#endif
{
  private:

 	  static long lObjectCount ;

	protected:

		NSDocumentData  _Donnees ;

    NSPatPathoArray _Meta ;
    NSPatPathoArray _Pres ;
    string 				  _sCodeDocMeta ;
    string          _sCodeDocPres ;

	public:

		NSDocumentInfo(NSContexte* pCtx = 0) ; // on a besoin d'un constructeur par défaut
		// NSDocumentInfo(string sMeta, NSContexte* pCtx, NSPatInfo* pPatInfo = 0) ;
    NSDocumentInfo(string sMeta, NSContexte* pCtx, NSPersonGraphManager* pGraphManager) ;
    NSDocumentInfo(const NSDocumentInfo& rv) ;
    ~NSDocumentInfo() ;

		bool getNomFichier(string& sNomFichier) ;
    // bool DonnePatPatho(NSPatPathoArray* pPatPathoArray, NSPatInfo* pPatInfo = 0) ;
    bool DonnePatPatho(NSPatPathoArray* pPatPathoArray, NSPersonGraphManager* pGraphManager) ;

    // bool LoadMetaAndData(NSPatInfo* pPatInfo = 0) ;
    // bool GetDataInformation(NSPatInfo* pPatInfo = 0) ;
    // bool ChargeDocMeta(NSPatInfo* pPatInfo = 0) ;
    bool LoadMetaAndData(NSPersonGraphManager* pGraphManager) ;
    bool GetDataInformation(NSPersonGraphManager* pGraphManager) ;
    bool ChargeDocMeta(NSPersonGraphManager* pGraphManager) ;
    bool ParseMetaDonnees() ;

    bool CommitMetaDonnees() ;
    bool InitDocumentBrut(NSDocumentInfo** ppDocument) ;

		string getID() 		   { return _Donnees.getID() ; }
    string getPatient()	 { return _Donnees._sCodePatient ; }
    string getDocument() { return _Donnees._sCodeDocument ; }
    string getRights()	 { return _Donnees._sRights ; }
    string getTypeCplt() { return _Donnees._sTypeComplet ; }
    string getTypeSem()  { return _Donnees._sType ; }
    string getInteret()	 { return _Donnees._sInteret ; }
    string getVisible()	 { return _Donnees._sVisible ; }
    string getCreator()	 { return _Donnees._sCreateur ; }
    string getAuthor()	 { return _Donnees._sAuteur ; }
    string getContent()	 { return _Donnees._sTypeContenu ; }
    string getSemCont()	 { return _Donnees._sTypeContenuSens ; }
    string getDocName()  { return _Donnees._sNom ; }
    string getCreDate()  { return _Donnees._sDateCreation ; }
    string getDateExm()  { return _Donnees._sDateExamen ; }
    string getLocalis()  { return _Donnees._sLocalisation ; }
    string getFichier()  { return _Donnees._sFichier ; }
    string getTemplate() { return _Donnees._sTemplate ; }
    string getEntete()   { return _Donnees._sEnTete ; }
    string getDestinat() { return _Donnees._sDestinataire ; }

    bool   estVisible()  { return _Donnees.estVisible() ; }

    // NSDocumentData* getData()         { return &_Donnees ; }
    void             setData(const NSDocumentData* pData) ;
    void             initFromData(NSDocumentData* pData) const ;

    // NSPatPathoArray* getMeta()        { return &_Meta ; }
    void             setMeta(const NSPatPathoArray* pPpt) ;
    void             initFromMeta(NSPatPathoArray* pPpt) const ;

    // NSPatPathoArray* getPres()        { return &_Pres ; }
    void             setPres(const NSPatPathoArray* pPpt) ;
    void             initFromPres(NSPatPathoArray* pPpt) const ;

    string 				   getCodeDocMeta() { return _sCodeDocMeta ; }
    string           getCodeDocPres() { return _sCodeDocPres ; }

    string getDocTitleWithDate() ;

    void   setID(string sId)       { _Donnees.setID(sId) ; }
    void   setPatient(string sPa)  { _Donnees._sCodePatient  = sPa ; }
    void   setDocument(string sDo) { _Donnees._sCodeDocument = sDo ; }
    void   setType(string sTy) ;
    void   setInteret(string sIn)  { _Donnees._sInteret      = sIn ; }
    void   setVisible(string sVi)  { _Donnees._sVisible      = sVi ; }
    void   setCreator(string sCr)  { _Donnees._sCreateur     = sCr ; }
    void   setAuthor(string sAu)   { _Donnees._sAuteur       = sAu ; }
    void   setNom(string sNo)      { _Donnees._sNom          = sNo ; }
    void   setCreDate(string sCD)  { _Donnees._sDateCreation = sCD ; }
    void   setDateExm(string sDa)  { _Donnees._sDateExamen   = sDa ; }
    void   setLocalisa(string sLo) { _Donnees._sLocalisation = sLo ; }
    void   setFichier(string sFi)  { _Donnees._sFichier      = sFi ; }
    void   setContent(string sCo) ;
    void   setTemplate(string sTp) { _Donnees._sTemplate     = sTp ; }
    void   setEnTete(string sET)   { _Donnees._sEnTete       = sET ; }
    void   setDestinat(string sDe) { _Donnees._sDestinataire = sDe ; }

    // Obsolete
    //
    void   setAccess(string sAc)   { _Donnees._sAcces        = sAc ; }
    void   setEmplacmt(string sEm) { _Donnees._sEmplacement  = sEm ; }
    void   setComment(string sCo)  { _Donnees._sCommentaire  = sCo ; }
    void   setTranNew(string sTr)  { _Donnees._sTranNew      = sTr ; }
    void   setTranDel(string sTr)  { _Donnees._sTranDel      = sTr ; }
    void   setCRC(string sCR)      { _Donnees._sCRC          = sCR ; }

    void   rendVisible()   { _Donnees.rendVisible() ; }
		void   rendInvisible() { _Donnees.rendInvisible() ; }

    void 	 setCodeDocMeta(string sCode) { _sCodeDocMeta = sCode ; }
    void   setCodeDocPres(string sCode) { _sCodeDocPres = sCode ; }

		NSDocumentInfo& operator=(const NSDocumentInfo src) ;
		int             operator==(const NSDocumentInfo& o) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

//---------------------------------------------------------------------------
//  Classe NSChemise
//---------------------------------------------------------------------------

//
// Objet données
//
#ifndef __linux__
class _CLASSENSDN NSChemiseData
#else
class NSChemiseData
#endif
{
	public :

		//
    // Variables de stockage des valeurs
    //
/*
    char code[CHE_CODE_LEN + 1] ;
    char nom[CHE_NOM_LEN + 1] ;
    char creation[CHE_CREATION_LEN + 1] ;
    char acces[CHE_ACCES_LEN + 1] ;
    char createur[CHE_CREATEUR_LEN + 1] ;
    char commentaire[CHE_COMMENTAIRE_LEN + 1] ;
*/

    string _sCode ;
    string _sNom ;
    string _sCreation ;
    string _sAcces ;
    string _sCreateur ;
    string _sCommentaire ;

    NSChemiseData() { metAZero() ; }
    NSChemiseData(const NSChemiseData& rv) ;

    NSChemiseData& operator=(const NSChemiseData& src) ;
    int            operator==(const NSChemiseData& o) const ;

    void metAZero() ;

    string donneIntitule(string sLang) ;
};

//
// Objet "Info" correspondant à NSChemise
// (destiné à être utilisé dans les containers)
//
#ifndef __linux__
class _CLASSENSDN NSChemiseInfo
#else
class NSChemiseInfo
#endif
{
	public:

		string        _sNodeChemise ;
		NSChemiseData _Donnees ;

		NSChemiseInfo() ;

		NSChemiseInfo(const NSChemiseInfo& rv) ;
		~NSChemiseInfo() ;

    string getNom()      { return _Donnees._sNom ; }
    string getCreation() { return _Donnees._sCreation ; }

    void   setNom(string sN)      { _Donnees._sNom      = sN ; }
    void   setCreation(string sC) { _Donnees._sCreation = sC ; }

		string donneIntitule(string sLang) ;

		NSChemiseInfo& operator=(const NSChemiseInfo& src) ;
		int            operator==(const NSChemiseInfo& o) const ;
};

//---------------------------------------------------------------------------
//  Classe NSSejour
//---------------------------------------------------------------------------

//
// Objet données
//
#ifndef __linux__
class _CLASSENSDN NSSejourData
#else
class NSSejourData
#endif
{
	public :

		//
		// Variables de stockage des valeurs
		//
    string sTreeID ;
    string sNumero ;
		string sDateDeb ;
		string sDateFin ;
		string sUnit ;
    string sCarePlace ;

		NSSejourData() { metAZero() ; }
		NSSejourData(const NSSejourData& rv) ;

		NSSejourData& operator=(const NSSejourData& src) ;
		int           operator==(const NSSejourData& o) const ;

		void metAZero() ;

		void donneIntitule(char *intitule) ;
};

//
// Objet "Info" correspondant à NSSejour
// (destiné à être utilisé dans les containers)
//
#ifndef __linux__
class _CLASSENSDN NSSejourInfo
#else
class NSSejourInfo
#endif
{
	public:

		NSSejourInfo() ;
		NSSejourInfo(NSSejourInfo& rv) ;
		~NSSejourInfo() ;

		void donneIntitule(char *intitule) ;

		NSSejourInfo& operator=(NSSejourInfo src) ;
		int           operator==(const NSSejourInfo& o) ;

    string getTreeID()    { return _Donnees.sTreeID ; }
    string getNumero()    { return _Donnees.sNumero ; }
		string getBeginDate() { return _Donnees.sDateDeb ; }
		string getEndDate()   { return _Donnees.sDateFin ; }
		string getUnit()      { return _Donnees.sUnit ; }
    string getCarePlace() { return _Donnees.sCarePlace ; }

    void   setTreeID(string sI)    { _Donnees.sTreeID    = sI ; }
    void   setNumero(string sI)    { _Donnees.sNumero    = sI ; }
		void   setBeginDate(string sI) { _Donnees.sDateDeb   = sI ; }
		void   setEndDate(string sI)   { _Donnees.sDateFin   = sI ; }
		void   setUnit(string sI)      { _Donnees.sUnit      = sI ; }
    void   setCarePlace(string sI) { _Donnees.sCarePlace = sI ; }

    bool   initFromPatPatho(NSPatPathoArray *pPPT, NSSuper* pSuper) ;
    bool   buildPatPatho(NSPatPathoArray *pPPT, NSSuper* pSuper) ;
    bool   updatePatPatho(NSPatPathoArray *pPPT, NSSuper* pSuper) ;

  protected:

    NSSejourData _Donnees ;
};

// Définition de l'array des séjours
typedef vector<NSSejourInfo*> NSSejourArray ;
typedef NSSejourArray::iterator NSSejourIter ;
//Reverse_iter
typedef NSSejourArray::reverse_iterator NSSejourReverseIter ;

//---------------------------------------------------------------------------
//  Classe NSChemDoc
//---------------------------------------------------------------------------

//
// Objet données
//
#ifndef __linux__
class _CLASSENSDN NSChemDocData
#else
class NSChemDocData
#endif
{
  public :

    //
    // Variables de stockage des valeurs
    //
    char chemise[CDO_CHEMISE_LEN + 1];
    char ordre[CDO_ORDRE_LEN + 1];
    char document[CDO_DOCUMENT_LEN + 1];

    NSChemDocData();

    NSChemDocData& operator=(NSChemDocData src);
    int 		       operator==(const NSChemDocData& o);
};

#ifndef __linux__
class _CLASSENSDN NSChemDocInfo
#else
class NSChemDocInfo
#endif
{
  public :

    NSChemDocData* pDonnees ;

    NSChemDocInfo() ;
    NSChemDocInfo(NSChemDocInfo& rv) ;
    ~NSChemDocInfo() ;

    NSChemDocInfo& operator=(NSChemDocInfo src) ;
    int 		   operator==(const NSChemDocInfo& o) ;
};

#ifndef __linux__
class _CLASSENSDN NSDocStatus
#else
class NSDocStatus
#endif
{
  public :

    enum DOCSTATUS { StatusUndefined = 0, StatusNew, StatusUnchanged, StatusModified, StatusDeleted } ;

    NSDocStatus() ;
    NSDocStatus(NSDocStatus& rv) ;
    ~NSDocStatus() ;

    NSDocStatus& operator=(NSDocStatus src) ;

    DOCSTATUS getStatus()              { return _iStatus ; }
    void      setStatus(DOCSTATUS iSt) { _iStatus = iSt ; }

    DOCSTATUS getCptaStatus()              { return _iCptaStatus ; }
    void      setCptaStatus(DOCSTATUS iSt) { _iCptaStatus = iSt ; }

    string    getDocId()              { return _sDocId ; }
    void      setDocId(string sSt)    { _sDocId = sSt ; }

    string    getNumCompt()           { return _sNumCompt ; }
    void      setNumCompt(string sSt) { _sNumCompt = sSt ; }

  protected :

    DOCSTATUS _iStatus ;
    DOCSTATUS _iCptaStatus ;

    string    _sDocId ;
    string    _sNumCompt ;
};

//---------------------------------------------------------------------------
#endif    // __NSDOCUM_H
