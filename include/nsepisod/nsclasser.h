// --------------------------------------------------------------------------
// NSCLASSER.H
// --------------------------------------------------------------------------
// Fabrice LE PERRU - Aout 2001
// source : Remi SPAAK - Mai 1997
// --------------------------------------------------------------------------

#if !defined(__NSCLASSER_H__)
#define __NSCLASSER_H__

class ParseElem ;
class ParseElemArray ;
class NSEpiFlechiesDB ;
class NVLdVTemps ;
class NSThesaurus ;
class NSThesaurusInfo ;
class NSPatholog ;
class NSPhraseur ;
class NsProposition ;
class NSGenerateurFr ;
class NSEpiClassifInfoArray ;
class NSEpiClassifInfo ;
class NSEpiClassif ;
class VectString ;
class NSPathologData ;

#include "partage/nsvector.h"

#ifndef _ENTERPRISE_DLL
  #include <owl\edit.h>
  #include "partage\nsglobal.h"
  #include "nsepisod/eptables.h"
#else
  #include "enterpriseLus/nsglobalLus.h"
  using namespace std ;
  // #include "enterpriseLus/nsglobalLus.h"
#endif

#ifndef __linux__
# if defined(_EPISODUSDLL)
#  define _EPISODUS __export
# else
#  define _EPISODUS __import
# endif
#endif

enum INDBLEVEL { INDBdatabaseError = 1, INDBnotAtAll, INDBasABegining, INDBcompleteTerm } ;

#ifndef __linux__
class _EPISODUS Classify : public NSRoot
#else
class Classify : public NSRoot
#endif
{
  public:

    Classify(NSContexte* pCtx, string* psClassif, string* psConcept, string* psPath) ;
    ~Classify() ;

    void             computeParsing( string  *psClassifResultO,
                                     string  *psClassifResultP,
                                     string  *psClassifResultI) ;

    void             searchTermsInLexique(string *sLibelle, string *psClassifResultO, string *psClassifResultP, std::string *psClassifResultI) ;
    vector<string *> *Purge(string *sLibelle) ;

    bool             findInLexiq(string s2Find) ;
    string           getCodeLexiq(string s2Find) ;

    ParseElem        *findGroup(ParseElem *pElem, ParseElemArray *pArray) ;

  protected:

    std::string pResultCodeClassif ;
    std::string *psClassification ;     // Classification
    std::string *psSOAPChemin ;
    std::string *psSOAP2Parse ;
    std::string *psResultParsing ;

    std::string *pElement ;
    std::string *pChemin ;
} ;

#ifndef __linux__
class _EPISODUS ParseSOAP : public NSRoot
#else
class ParseSOAP : public NSRoot
#endif
{
	public:

		ParseSOAP(NSContexte* pCtx, string *sClassifier);
		~ParseSOAP();

		void                ChangeSOAPEdit(std::string *psSOAPEdit) ;

		bool                ParsingEq();
		std::string         *getLibelleParsing();
		std::string         *getCodeParsing();
    void                ConvertitMajuscule(char* chaine);
    std::string         ConvertitMajuscule(string sChaine) ;

    void    computeParsing( std::string  *sLibelle,
                            std::string  *sChemin,
                            std::string  *psClassifResultO,
                            std::string  *psClassifResultP,
                            std::string  *psClassifResultI,
                            std::string  *psClassifResult3) ;

    bool    computeParsingElem( std::string  *sLibelle,
                                std::string  *sChemin,
                                std::string  *psClassifResultO,
                                std::string  *psClassifResultP,
                                std::string  *psClassifResultI,
                                std::string  *psClassifResult3) ;

  	void                searchTermsInLexique(std::string *sLibelle, std::string *psClassifResultO, std::string *psClassifResultP, std::string *psClassifResultI, std::string *psClassifResult3, bool bUse3BT = true) ;
		vector<string *>*   Purge(string *sLibelle) ;
    void								initParseElemArray(string* psLibelle, ParseElemArray* pParseResult) ;
    void								initGroupElemArray(ParseElemArray* pLoneWords, ParseElemArray* pGroups) ;
    void                cleanGroupElemArray(ParseElemArray* pGroups) ;

		bool                findInLexiq(string s2Find, NSEpiFlechiesDB* pFlechies = 0, INDBLEVEL* pLevel = 0) ;
		string              getCodeLexiq(string s2Find) ;

		ParseElem*          findLargerGroup(ParseElem *pElem, ParseElemArray *pArray, NSEpiFlechiesDB* pFlechies = 0) ;
    void          			addAllGroups(ParseElemArray *pLoneArray, ParseElemArray *pGroupArray, NSEpiFlechiesDB* pFlechies = 0) ;

	protected:

		string              pResultCodeClassif ;
		string              *psClassifier ;         // Classification
		string              *psSOAPChemin ;
		string              *psSOAP2Parse ;
		string              *psResultParsing ;

		NSPatholog          *pPatho ;
} ;

class ElemSet ;
class ElemSetArray ;

typedef vector<string *> pstringVector ;
typedef pstringVector::iterator pstringIterator ;

#ifndef __linux__
class _EPISODUS ParseCategory
#else
class ParseCategory
#endif
{
  public:

//        ParseCategory(string sObligatoire, string sPossible, string sInterdit);
    ParseCategory(int nb, string label, string pattern) ;
    ~ParseCategory() ;

    void             DefObligatoire(string sDefine) ;
    void             DefObligatoire(ElemSetArray *pElemSetArray) ;

    void             DefPossible(string sDefine) ;
    void             DefPossible(ElemSetArray *pElemSetArray) ;

    void             DefInterdit(string sDefine) ;
    void             DefInterdit(ElemSetArray *pElemSetArray) ;

    void             Def3BT(string sDefine) ;
    void             Def3BT(ElemSetArray *pElemSetArray) ;

    void             DefCritere(string sDefO, string sDefP, string sDefI, string sDef3) ;

    ElemSetArray     *DefDomain(string sDomain) ;

    ElemSetArray     *getObligatoireDomain() { return pDomainObligatoire ; }
    string           getsObligatoire()       { return sObligatoire ; }
    ElemSetArray     *getPossibleDomain()    { return pDomainPossible ; }
    string           getsPossible()          { return sPossible ; }
    ElemSetArray     *getInterditDomain()    { return pDomainInterdit ; }
    string           getsInterdit()          { return sInterdit ; }
    ElemSetArray     *get3btDomain()         { return pDomain3BT ; }
    string           get3BT()                { return s3BT ; }

    vector<string *> *getPattern()           { return psPattern ; }

  protected:

    int              nbDigit ;
    string           sLabel ;
    vector<string *> *psPattern ;

    string           sObligatoire ;
    ElemSetArray     *pDomainObligatoire ;
    string           sPossible ;
    ElemSetArray     *pDomainPossible ;
    string           sInterdit ;
    ElemSetArray     *pDomainInterdit ;
    string           s3BT ;
    ElemSetArray     *pDomain3BT ;
} ;

//
// Represente un element de classification ou un intervalle
//
#ifndef __linux__
class _EPISODUS ElemSet
#else
class ElemSet
#endif
{
  private:

    static long lObjectCount ;

  public:

    ElemSet(string sElem) ;
    ElemSet(string sElemBegin, string sElemEnd) ;
    ElemSet(string sElemBegin, string sElemEnd, bool bElemEmpty) ;

    ElemSet(ElemSet& rv) ;

    ~ElemSet() { lObjectCount-- ; } ;

//        int                 CountJoker(string sElem);
//        ElemSetArray        *ComputeJoker(vector<string *> *psPattern);
    void         ComputeJoker(vector<string *> psPattern, ElemSetArray *pElemSetResult) ;
//        void                ComputeJokerInterval();
//        void                ReplaceJoker(string sBorn, string s2Replace);

    bool         HasJoker() ;

    string       getBegin()                 { return sBegin ; }
    void         setBegin(string s2Replace) { sBegin = s2Replace ; }
    string       getEnd()                   { return sEnd ; } ;
    void         setEnd(string s2Replace)   { sEnd = s2Replace ; }

    bool         isEmptySet()                { return bEmptySet ; }
    void         setEmptySet(bool b2Replace) { bEmptySet = b2Replace ; }

    string       toString() ;

    bool         intersect(ElemSet *pElem) ;
    ElemSet      *Smaller(ElemSet* pElem) ;
    ElemSetArray *inter(ElemSetArray *Elems) ;

    ElemSetArray *minus(ElemSet *pElem) ;

    void         ComputeUnion(ElemSetArray *pElems) ;

    bool         doesContain(string sCode) ;

    void         reorder() ;

    ElemSet&     operator=(ElemSet src) ;
    bool         operator==(ElemSet src) ;

    static long  getNbInstance()  { return lObjectCount ; }
    static void  initNbInstance() { lObjectCount = 0 ; }

  protected:

    string sBegin ;
    string sEnd ;
    bool   bEmptySet ;
} ;

typedef vector<ElemSet*> ElemSetVector ;
typedef ElemSetVector::iterator ElemSetIterator ;

//
// Represente un ensemble d'elements ou d'intervalles
//
#ifndef __linux__
class _EPISODUS ElemSetArray : public ElemSetVector
#else
class ElemSetArray : public ElemSetVector
#endif
{
  private:

    static long lObjectCount ;

  public:

    ElemSetArray() : ElemSetVector() { lObjectCount++ ; } ;
    ElemSetArray(string sDomaine);
    ~ElemSetArray() ;

    ElemSetArray(ElemSetArray& rv) ;

    ElemSetArray *ComputeIntersection(ElemSetArray *pElems) ;
    void         ComputeUnion() ;

    void         minus(ElemSet *pTerm) ;
    void         minus(ElemSetArray *pTerms) ;

    void         print(char *) ;
    string       toString() ;

    bool         isEmpty() ;

    bool         doesContain(string sCode);

    void         vider() ;

    ElemSetArray& operator=(ElemSetArray src) ;

    static long  getNbInstance()  { return lObjectCount ; }
    static void  initNbInstance() { lObjectCount = 0 ; }
} ;

typedef vector<ParseCategory *>       ParseCategoryVector ;
typedef ParseCategoryVector::iterator ParseCategoryIterator ;

#ifndef __linux__
class _EPISODUS TermsParser : public ParseCategoryVector, public NSRoot
#else
class TermsParser : public ParseCategoryVector, public NSRoot
#endif
{
  private:

    static long lObjectCount ;

  public:

    TermsParser(NSContexte* pCtx, string sClassif, bool bUse3bt = true) ;
    ~TermsParser() ;

    ElemSetArray *Compute() ;
    ElemSetArray *ComputeWithPIntersection() ;
    ElemSetArray *ComputeWithPUnion() ;

    ElemSetArray *Compute(ParseElemArray *) ;
    ElemSetArray *ComputeWithPIntersection(ParseElemArray *) ;
    ElemSetArray *ComputeWithPUnion(ParseElemArray *) ;
    bool         ComputeElements(ParseElemArray *) ;

#ifndef _ENTERPRISE_DLL
    bool         FindClinicalTerm(NSThesaurus *pClinicalBase, string* pElement) ;
    string       FindAllClinicalTermsStartingWith(NSThesaurus *pClinicalBase, string* pElement, string sClassif) ;
    string       FindAllSentinelTermsStartingWith(NSThesaurus *pClinicalBase, string* pElement, string sClassif) ;
    bool         FindFlexClinicalTerm(NSThesaurus *pClinicalBase, string* pLexiqueCode) ;
#else
    bool         FindClinicalTerm(NSThesaurusInfo *pClinicalBase, string* pElement) ;
    string       FindAllClinicalTermsStartingWith(NSThesaurusInfo *pClinicalBase, string* pElement, string sClassif) ;
    string       FindAllSentinelTermsStartingWith(NSThesaurusInfo *pClinicalBase, string* pElement, string sClassif) ;
    bool         FindFlexClinicalTerm(NSThesaurusInfo *pClinicalBase, string* pLexiqueCode) ;
#endif

    void         ComputePUnion() ;
    void         ComputePIntersection() ;

    void         ComputeIUnion() ;
    void         ComputeOIntersection() ;
    void         Compute3btIntersection() ;
    void         CheckI(ElemSetArray *pResult) ;

    string       getsObligatoire() { return pObligatoire->toString() ; }
    string       getsPossible() { return pPossible->toString() ; }
    string       getsInterdit() { return pInterdit->toString() ; }

    void         vider() ;

    string       sBasicOnlyAddNewcodes(string sExistingSet, string incomingSet) ;

    static long  getNbInstance()  { return lObjectCount ; }
    static void  initNbInstance() { lObjectCount = 0 ; }

  protected:

    string       sClassification ;

    ElemSetArray *pObligatoire ;
    ElemSetArray *pPossible ;
    ElemSetArray *pInterdit ;

    bool         _bUse3bt ;
} ;

#ifndef __linux__
class _EPISODUS NSEpiFlechiesDB : public NSRoot
#else
class NSEpiFlechiesDB : public NSRoot
#endif
{
  public:

    NSEpiFlechiesDB(NSContexte* pCtx) ;
    ~NSEpiFlechiesDB() ;

#ifndef _ENTERPRISE_DLL
    void   CreateDatabaseFlechies(bool* pContinuer = 0, OWL::TEdit* pAffichage = 0) ;
    void   rempliDatabase(NSPathologData *pDonnees) ;
#endif
    bool   traiteFormeFlechie(string sAvant, string sApres, string sCode, int iDep = 0) ;
    bool   rempliDatabaseFlechies(string sLibelle, string sCode) ;
    bool   findElem(string sLibelle, string sCode) ;

    bool   isInDB(string *sLibelle, INDBLEVEL* pLevel = 0) ;
    bool   isInDB(string *sLibelle, char cTypeCode) ;
    string getCodeLexiq(string sLibelle) ;
    string getCodeLexiq(string sLibelle, char cTypeCode) ;

    bool   getAllLabelsForCode(string sCode, VectString *pLabels) ;

    void   ConvertitMajuscule(char* chaine) ;
    string ConvertitMajuscule(string sChaine) ;
    string getCodeSens(string sLibelle) ;

    void	 incrementeIdCpt() ;

  private:

    NSPatholog*     pPatho ;
    NSPhraseur*     pPhraseur ;
    NsProposition*  pPropos ;
    NSGenerateurFr* pGenFR ;

    string          sIdCpt ;
} ;

#ifndef __linux__
class _EPISODUS ParseElem
#else
class ParseElem
#endif
{
  private:

    static long lObjectCount ;

  public:

    ParseElem(string sElem) { lObjectCount++ ; sContent = sElem ; bNotInFirst = false ; nbOfTerms = 1 ; iPos = 0 ; }
    ParseElem(string sElem, int nbTerms, int Pos) { lObjectCount++ ; sContent = sElem ; bNotInFirst = false ; nbOfTerms = nbTerms ; iPos = Pos ; }
    ParseElem(ParseElem& rv) ;
    ~ParseElem() { lObjectCount-- ; } ;

    ParseElem& operator=(ParseElem src) ;

    bool    getNotInFirst()            { return bNotInFirst ; }
    void    setNotInFirst(bool bValue) { bNotInFirst = bValue ; }

    string  getContent()               { return sContent ; }

    int     getNbOfTerms()             { return nbOfTerms ; }
    void    setNbOfTerms(int i)        { nbOfTerms = i ; }

    int     getPos()                   { return iPos ; }
    void    setPos(int Pos)            { iPos = Pos ; }

    static long  getNbInstance()  { return lObjectCount ; }
    static void  initNbInstance() { lObjectCount = 0 ; }

  protected:

    bool   bNotInFirst ;
    string sContent ;
    int    nbOfTerms ;
    int    iPos ;
} ;

typedef vector<ParseElem *>         ParseElemVector ;
typedef ParseElemVector::iterator   ParseElemIter ;

#ifndef __linux__
class _EPISODUS ParseElemArray : public ParseElemVector
#else
class ParseElemArray : public ParseElemVector
#endif
{
  private:

    static long lObjectCount ;

	public:

		ParseElemArray() { lObjectCount++ ; } ;
		ParseElemArray(vector<string *> *pList) ;
    ParseElemArray(ParseElemArray& rv) ;
		~ParseElemArray() ;

		void vider() ;
    void setAllPos() ;

		ParseElemArray& operator=(ParseElemArray src) ;

    static long  getNbInstance()  { return lObjectCount ; }
    static void  initNbInstance() { lObjectCount = 0 ; }
} ;

#ifndef __linux__
class _EPISODUS ClassifElem
#else
class ClassifElem
#endif
{
  private:

    static long lObjectCount ;

    string  sCode ;
		string  sLibelle ;

	public:

		ClassifElem(string s2Code, string s2Libelle) { lObjectCount++ ; sCode = s2Code ; sLibelle = s2Libelle ; }
		~ClassifElem() { lObjectCount-- ; }

		string  getCode() ;
		string  getLibelle() { return sLibelle ; }

    static long  getNbInstance()  { return lObjectCount ; }
    static void  initNbInstance() { lObjectCount = 0 ; }
} ;

typedef vector<ClassifElem *>       ClassifElemVector ;
typedef ClassifElemVector::iterator ClassifElemIter ;

#ifndef __linux__
class _EPISODUS ClassifElemArray : public ClassifElemVector
#else
class ClassifElemArray : public ClassifElemVector
#endif
{
  private:

    static long lObjectCount ;

	public:

		ClassifElemArray() { lObjectCount++ ; }
		~ClassifElemArray() ;

		void    vider() ;

    static long  getNbInstance()  { return lObjectCount ; }
    static void  initNbInstance() { lObjectCount = 0 ; }
} ;

#ifndef __linux__
class _EPISODUS NSEpiClassifDB : public NSRoot
#else
class NSEpiClassifDB : public NSRoot
#endif
{
  public:

    NSEpiClassifDB(NSContexte* pCtx) ;
    ~NSEpiClassifDB() ;

    // rempli la table avec les codes CISP
    void    rempliClassifCISP(string sFileCISP) ;
    void    rempliClassifCIM(string sFileCIM) ;

    void    searchDomainInClassif(string sClassification, ElemSetArray *pDomain, NSEpiClassifInfoArray *pArrayClassif) ;
//        cette methode doit a partir d'un domaine ajouter a l'array pArrayClassif (s'ils n'y sont pas deja)
//        les NSEpiClassifInfo des codes contenus dans le pDomain, en verifiant qu'ils existent dans classif.db
//        d'ou array de NSepiClassifInfo

    bool    searchCode(string sClassification, string sCode, NSEpiClassifInfo *pClassifInfo) ;

  private:

    ClassifElemArray *pChap ;
    ClassifElemArray *pLibelle ;
} ;

class SOAPObject;

#ifndef __linux__
class _EPISODUS classifExpert : public NSRoot
#else
class classifExpert : public NSRoot
#endif
{
  public:

    enum NIVEAU { niveauPreselection = 1, niveauAutorises, niveauTous} ;

    classifExpert(NSContexte* pCtx) ;
    ~classifExpert() ;

    string  donnePattern(string sClassification) ;
    int     donneCodeSize(string sClassification) ;

    void    donneInterdits(string sClassification, string sCase,
                           ElemSetArray *pForbidenDomain) ;

    void    donneClassifArray(SOAPObject* pObjet,
                              NSEpiClassifInfoArray *pArrayClassif,
                              int iNiveau) ;
    void    fillList(string sClassification, ElemSetArray *pDomain,
                                    NSEpiClassifInfoArray *pArrayClassif,
                                    string sCase) ;

    /*string  chooseCode(string sClassification, ElemSetArray* pDomain,
                                                            string sCase);

    string  chooseCode(string sClassif, string sDomain, string sCase); */

    string  chooseCode(SOAPObject* pObjet) ;

    void    setDomain(string sClassification, string sDomain, ElemSetArray* pDomain) ;

    void    setControlString(string* pCtrlData, string sClassification, string sCase,
                        string sTexteInitial, int iInitialLevel,
                        NSEpiClassifInfoArray* pArrayClassif,
                        NVLdVTemps* pTpsDebut, NVLdVTemps* pTpsFin,
                        string sResult, string sCodeSens = "") ;
    void    setControlString(string* pCtrlData, SOAPObject* pObjet,
                        int iInitialLevel, NSEpiClassifInfoArray* pArrayClassif,
                        NVLdVTemps* pTpsDebut, NVLdVTemps* pTpsFin,
                        string sResult, string sCodeSens = "") ;
    void    storeControlData(string sCtrlData) ;

  protected:

#ifndef _ENTERPRISE_DLL
    NSEpiClassif _DBcurseur ;
#endif
} ;

#endif		// __NSCLASSER_H__
