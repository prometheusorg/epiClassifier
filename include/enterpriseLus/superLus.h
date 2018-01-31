//---------------------------------------------------------------------------
//     superLus.h
//
//  Contient les définitions du Superviseur
//
//
//  PA September 2009
//---------------------------------------------------------------------------

#ifndef __NSSUPER_H__
#define __NSSUPER_H__

#include "../dti/sockets/sock.h"

class Person;
class PersonArray;

class BB1BBInterface;
class BB1BBInterfaceForKs;
class BB1BB;

class NSDico;
class NSFilGuide;
class NSArcManager;
class NSLocalChapterArray;

class NSBasicAttributeArray;

class NautilusPilot;

class ontologyBaseManager;
class classifExpert;

class NSSuper;
// class NSContexte;

// class NSContextArray;

#include "../partage/ntarray.h"

// # include "nsdivfctForCgi.h"    // fonction erreur
#include "personLus.h"

// # include "pilot\NautilusPilot.hpp"

//
// Definition of ToDoTask
//
class NSToDoTask
{
  public:

 	  //! constructor
    NSToDoTask();

    //! copy constructor
    NSToDoTask(NSToDoTask&);

    //! destructor
    ~NSToDoTask();

    //! operator =
    NSToDoTask& operator=(NSToDoTask& src);

    void sendMe(NSSuper *pSuper, bool bSend = true);
    void sendBB1BBInterface(BB1BBInterfaceForKs* pKsDesc, NSSuper *pSuper, bool bSend = true, bool bDeleteKsDesc = false);
    void sendBB1BBDecisionTree(BB1BBInterfaceForKs* pKsDesc, NSSuper *pSuper, bool bSend = true, bool bDeleteKsDesc = false);

    void initFromTask(NSToDoTask *pSrc);
    void reinit();

    std::string getWhatToDo()  { return _sWhatToDo; }
    std::string getParam1()    { return _sParam1;   }
    std::string getParam2()    { return _sParam2;   }

    void*  getPointer1()  { return _pPointer1; }
    void*  getPointer2()  { return _pPointer2; }
    bool   mustDeleteP1() { return _deleteP1;  }
    bool   mustDeleteP2() { return _deleteP2;  }

    void   setWhatToDo(std::string sWTD) { _sWhatToDo = sWTD; }
    void   setParam1(std::string sP1)    { _sParam1   = sP1; }
    void   setParam2(std::string sP2)    { _sParam2   = sP2; }

    void   setPointer1(void* pP1)   { _pPointer1 = pP1; }
    void   setPointer2(void* pP2)   { _pPointer2 = pP2; }
    void   setToDeleteP1(bool bP1)  { _deleteP1  = bP1; }
    void   setToDeleteP2(bool bP2)  { _deleteP2  = bP2; }

  protected:

    std::string  _sWhatToDo;
    std::string  _sParam1;
    std::string  _sParam2;

    void*   _pPointer1;
    bool    _deleteP1;
    void*   _pPointer2;
    bool    _deleteP2;
};

typedef std::vector<NSToDoTask *> NSTaskVector;
typedef NSTaskVector::iterator  TaskIter;
typedef NTArray<NSToDoTask>     NSTaskArray;

typedef std::vector<NSContexte *> NSContextVector;
typedef NSContextVector::iterator       CtxIter;
typedef NSContextVector::const_iterator CtxConstIter;

class NSContextArray : public NSContextVector
{
  public:

    NSContextArray() : NSContextVector() {}
	  NSContextArray(const NSContextArray& rv);
	  virtual ~NSContextArray();

    NSContexte* getContextFromToken(std::string sToken);
    NSContexte* getContextFromUser(std::string sUserId);
    NSContexte* getContextFromPatient(std::string sPatId);

    void vider();
};

// -----------------------------------------------------------------------------
// NSSuper class
// Main Big Brother: "Supervisor"
// -----------------------------------------------------------------------------

class NSSuper
{
  friend class NSContexte;

  public:

    enum CLASSDOCTYPES { isTree = 0, isFile, isCompta, isHTML, isText, isImage, isImageFixe, isImageAnimee, isURL };

    // trace mode
    enum TRACETYPE { trNone = -1, trError = 0, trWarning, trSteps, trSubSteps, trDetails, trSubDetails };

  protected:

    short                 _statut;                      //!< Statut du superviseur : 0 = OK 1 = Anormal
    bool                  _bJavaOk;                     //!< Is Java ok ?

    ontologyBaseManager*  _ontologyManager;

    TRACETYPE             _iTrace;                      //!< trace mode
    TRACETYPE             _iTraceConsole;               //!< console trace mode

    NSDico*               _pDico;                       //!< Pointer to current dictionary

    bool                  _bToDoLocked;
    NSTaskArray           _aToDo;

    // Pointeur sur le reseau semantique et fonctions associees
    NSFilGuide*           _pFilGuide;	                  //!< codeur
    NSFilGuide*           _pFilDecode;                  //!< decodeur

    // Pointeur sur l'objet de gestion du Graphe entre noeuds d'arbres
    // Note : ce pointeur est desactive pour eviter les pbs multitache sur la base NSPatLink
    // NSGraphe*       pGraphe;

    NautilusPilot*        _pPilot;

    classifExpert*        _pClassifExpert;

    string                _sIppSite;                    //!< site for permanent patient identifier
    string                _sOperationalSite;            //!< site (sub-site) of the operational unit

    NSContextArray*       _aContextArray;

  public:

    //! default constructor
    NSSuper(short sStatus);

    //! copy constructor
    NSSuper(NSSuper& srcNSSuper);

    //! destructor
    ~NSSuper();

    //! operator =
    NSSuper& operator=(NSSuper& srcNSSuper);

    NSDico*               getDico()            { return _pDico; }
    NSFilGuide*           getFilGuide()        { return _pFilGuide; }
    NSFilGuide*           getFilDecode()       { return _pFilDecode; }
    classifExpert*        getClassifExpert()   { return _pClassifExpert; }
    NSContextArray*       getContextArray()    { return _aContextArray; }
    ontologyBaseManager*  getDatabaseManager() { return _ontologyManager; }
    NautilusPilot*        getPilot()           { return _pPilot; }
    string                getIppSite()         { return _sIppSite; }
    string                getOperationSite()   { return _sOperationalSite; }

    void                  setTraceLevel(TRACETYPE iTrLev)          { _iTrace = iTrLev; }
    void                  setClassifExpert(classifExpert* pClExp)  { _pClassifExpert = pClExp; }

    std::string           _sNumVersion;

    void                  StartDebugConsole(int nWidth, int nHeight,const char* pszfname);
    void                  voidDebugPrintf(TRACETYPE traceLevel, const char *szfmt, ...);
    void                  DebugNewLine(void);

    // Pour les chemins
    bool                  estEgal(std::string* chaine, std::string* modele/*, int precision = compReseau*/);

    bool                  InitialiseJava();
    bool                  isJavaOk() { return _bJavaOk; }

    void                  InitPointers();
    bool                  InitDatabase();
    void                  creePilot();

    bool                  isToDoLocked()           { return _bToDoLocked; }
    void                  lockToDo()               { _bToDoLocked = true; }
    void                  unlockToDo()             { _bToDoLocked = false; }

    void                  addToDoTask(NSToDoTask *t) { _aToDo.push_back(t); }
    // void                  addToDo(NSToDoTask* pTask, bool bSend = true);

    // Localization functions
    bool loadLocalisationString(std::string sLang, NSLocalChapterArray* pLocStrings = 0);
    std::string getText(std::string sChapter, std::string sCode, std::string sLang = std::string(""), NSContexte* pCtx = 0);

    // Document types semantic
    bool typeDocument(std::string sType, CLASSDOCTYPES categ);

    void initInstanceCounters();

    void trace(std::string[], int nbString = -1, TRACETYPE iLevel = trDetails);

    void CheckZeroObjectCount(string sObjectName, long lInstanceCounter);
};

//
//
//

/*
class NSUserPatRelation
{
 public:
  // User privileges for this patient
  bool bPatientIsTheUser;
  bool bUserActiveInHealthTeam;
  bool bUserPresentInHealthTeam;
  bool bUserIsAdministrator;
  bool bUserIsRootAdministrator;

  int	 adminPosit[24][3];
  int	 existPosit[24][3];

  NSUserPatRelation();

  bool isAdministred(int iAngle, int iDistance);
  bool isPresent(int iAngle, int iDistance);

  void reinit();
  void init(NSContexte* pContexte);
};
*/

class NSContexte
{
  protected:

    Person*                _pPerson;
    Person*                _pUser;
    NSSuper*               _pSuper;

    // Number of current transaction
    std::string 	         _sTransaction;

    std::string            _sToken;
    // NSPersonGraphManager*  _pGraphPerson;
    NSBasicAttributeArray* _pAllTraits;

    // User privileges for this patient
    // NSUserPatRelation 		 _relationship;

    BB1BBInterface*       _blackboardInterface;         //!< pointer to blackboard interface
    BB1BB*                _bb;                          //!< pointer to blackboard

    NSTaskArray           _aBBKToDo;
    bool                  _bBBKToDoLocked;

  public:

    NSBasicAttributeArray* getTraitList()             { return _pAllTraits; }
    void setTraitList(NSBasicAttributeArray *pTraits) { _pAllTraits = pTraits; }

    NSContexte();
    NSContexte(NSSuper* pSup);
    NSContexte(NSSuper* pSup, std::string sPersonId);

    NSContexte(NSContexte& rv);

    void    setPerson(Person* pPat) { _pPerson = pPat; }
    Person* getPerson()             { return _pPerson; }

    std::string  getPersonID();
    void         setPersonID(std::string sId);

    void    setUser(Person* pUti);
    Person* getUser()               { return _pUser; }

    std::string  getUserID();
    void         setUserID(std::string sId);

    bool    isPersonAlsoUser() { return (getPersonID() == getUserID()); }

    enum PERSONCLASSES { isUnknown = 0, isMemory, isLocal, isGlobal };

    PERSONCLASSES 			  getPersonClass(std::string sPersonID);
    // NSUserPatRelation*	  getRelationship() { return &_relationship; }

    void                  setSupervisor(NSSuper* pSup)     { _pSuper = pSup; }
    NSSuper*              getSuperviseur()                 { return _pSuper; }

    void                  setTransaction(std::string sTran) { _sTransaction = sTran; }
    std::string           getTransaction()                  { return _sTransaction; }

    void                  setToken(std::string sTok)       { _sToken = sTok; }
    std::string           getToken()                       { return _sToken; }

    BB1BBInterface*       getBBinterface() { return _blackboardInterface; }
    void									setBBinterface(BB1BBInterface* pBBi) { _blackboardInterface = pBBi; }

    BB1BB*                getBlackboard() { return _bb; }
    void									setBlackboard(BB1BB* pBB1) { _bb = pBB1; }

    NSTaskArray*          getBBKToDo()       { return &_aBBKToDo; }
    bool                  getBBKToDoLocked() { return _bBBKToDoLocked; }

    string                getUserLanguage();

    void addToDoBbk(NSToDoTask* pTask, bool bSend = true);

    // Blackboard functions
    bool BbkAskUser(std::string sArchetype, int iTypeArc = 0);
    bool Ob1AskUser(BB1BBInterfaceForKs* pKsDesc);
    bool bbkToDo(int iTypeArc, std::string ssWhatToDo, std::string ssParam1, std::string ssParam2, void* ppPointer1, bool p1, void* ppPointer2, bool p2);
    bool BbkShowReferential(std::string sReferential);
    bool TraiteTask();

/*
  NSPersonGraphManager* getGraphPerson() { return _pGraphPerson; }
  void                  resetGraphPerson()
  {
  	if (NULL == _pGraphPerson)
    	_pGraphPerson = new NSPersonGraphManager(this);
    else if (NULL != _pGraphPerson->getDataGraph())
    	_pGraphPerson->graphReset();
  }
*/

    NSDico*         getDico()           { return _pSuper->getDico(); }
    NSFilGuide*     getFilGuide()       { return _pSuper->getFilGuide(); }
    NSFilGuide*     getFilDecode()      { return _pSuper->getFilDecode(); }
    // PersonArray*    getPersonArray()    { return _pSuper->getPersonArray(); }

    void creePilot();

    //
    // Privileges management
    //
    enum	RIGHTSTO
    {
      openPatient = 0,
      openDocument ,
      modifyDocument ,
      createDocument ,
      suppresDocument ,
      modifyLdV ,
      viewConcerns ,
      createConcern ,
      modifyConcern ,
      viewDrug ,
      createDrug ,
      modifyDrug ,
      viewGoal ,
      createGoal ,
      modifyGoal ,
      viewProcess ,
      createProcess ,
      modifyProcess ,
      viewSynthesis ,
      modifySynthesis ,
      viewAdminData ,
      modifyAdminData ,
      viewProData ,
      modifyProData ,
      viewHealthTeam ,
      createHTMember ,
      createMandate ,
      modifyMandate ,
      createRosace ,
      modifyRosace ,
      createUser
    };

    bool userHasPrivilege(RIGHTSTO doThat, int i1, int i2, std::string s1, std::string s2, void* p1, void* p2);

    // Compatibility functions
    //
    Person*     getPatient()       { return getPerson(); }
    Person*     getUtilisateur()   { return getUser(); }
    std::string getUtilisateurID() { return getUserID(); }
};

#endif // !__NSSUPER_H__
