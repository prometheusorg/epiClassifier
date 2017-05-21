// --------------------------------------------------------------------------
// NSEPISODVIEW.H		Document/Vues Capture Donnees Utilisateur
// --------------------------------------------------------------------------
// Fabrice LE PERRU - Aout 2001
// source : Rémi SPAAK - Mai 1997
// --------------------------------------------------------------------------

#if !defined(__NSSOAPDIV_H)
#define __NSSOAPDIV_H

class BBFilsItem ;
#ifndef __linux__
class NSCapture ;
#endif

#include "nsepisod/eptables.h"

#ifndef __linux__
#if defined(_EPISODUSDLL)
	#define _EPISODUS __export
#else
	#define _EPISODUS __import
#endif
#endif

#ifndef __linux__

#define _NSSOAPVIEWCLASS _USERCLASS
#define IDC_SOAPVIEW     0x0042

#define NS_EV_TLISTWIND_NOTIFY_BOOL(notifyCode, method)\
  {notifyCode, UINT_MAX, (TAnyDispatcher)::B_LPARAM_Dispatch,\
  (TMyPMF)b_LISTWIND_NOTIFY_Sig(&TMyClass::method)}
#define NS_LVN_BEGINDRAG(method) NS_EV_TLISTWIND_NOTIFY_BOOL(LVN_BEGINDRAG, method)

#endif // #ifndef __linux__

// enum category_type {subjective = 0, objective, assesment, plan} ;

#ifndef __linux__
class _EPISODUS ClassificationPrinciple
#else
class ClassificationPrinciple
#endif
{
  public:

    ClassificationPrinciple(string sType, string sClassif = "C2", bool bUnik = false, string sLibelle = "") ;
    ClassificationPrinciple(const ClassificationPrinciple &rv) ;
    ~ClassificationPrinciple() ;

    ClassificationPrinciple& operator=(const ClassificationPrinciple& src) ;
    bool operator==(ClassificationPrinciple src) ;

    string  sCase ;              // case : S, A, P...
    string  sClassification ;    // classification utilisée
    bool    bUnique ;            // code unique (ou multiple)
    string  sLibelle ;           // libellé
};

typedef vector<ClassificationPrinciple*> ArrayPrinciples ;
typedef ArrayPrinciples::iterator        PrinciplesIter ;
typedef ArrayPrinciples::const_iterator  PrinciplesConstIter ;

#ifndef __linux__
class _EPISODUS PrinciplesArray : public ArrayPrinciples, public NSSuperRoot
#else
class PrinciplesArray : public ArrayPrinciples, public NSSuperRoot
#endif
{
  public:

    PrinciplesArray(NSSuper* pSuper) : ArrayPrinciples(), NSSuperRoot(pSuper) {}
    PrinciplesArray(const PrinciplesArray &rv) ;
    ~PrinciplesArray() ;

    ClassificationPrinciple* trouvePrincipe(string sCase) ;
    ClassificationPrinciple* trouvePrincipe(string sCase, string sExcesPath) ;

    void vider() ;

    PrinciplesArray& operator=(const PrinciplesArray& src) ;
};

#ifndef __linux__
class _EPISODUS SOAPObject
#else
class SOAPObject
#endif
{
  public:

    string  sTexteInitial;      // texte capture
    string  sCode;              // code dans la classification
    string  sClassification;    // classification utilisee
    int     iConfidence;        // degre de certitude du codage

    string  sCase;              // case S, A, P...
    int     iImageIndex;

    int     iListParam, iOldListParam; // index ListWindow

    // Le SOAPObject peut se referer a un element capture OU a un noeud
    //
#ifndef __linux__
    NSCapture*  pCaptElemnt;    // Pour un element externe capture
#endif
    string      sNoeud;         // Pour un noeud deja enregistre
    BBFilsItem* pFilsItem;      // Pour un noeud interne en cours de saisie

    bool        bFloculable ;

    SOAPObject() ;
#ifndef __linux__
    SOAPObject(NSCapture* pCapt) ;
#endif
    SOAPObject(string sTI, string sCd, string sCl, int iCf, string sNoeud) ;
    SOAPObject(string sTI, string sCd, string sCl, int iCf, BBFilsItem* pItem) ;
    SOAPObject(const SOAPObject &rv) ;
    SOAPObject(SOAPObject *pNewSOAPObject) ;
    ~SOAPObject() ;

    string  donneLibelle() ;
    string  donneChapitre() ;

#ifndef __linux__
    void    initFromCapture(NSCapture* pCapt) ;
#endif

    bool    estFloculable()             { return bFloculable ; }
    void    setFloculable(bool bFloc)   { bFloculable = bFloc ; }

    SOAPObject& operator=(const SOAPObject& src) ;
};

typedef vector<SOAPObject*>             ArraySOAPObject ;
typedef ArraySOAPObject::iterator       ArraySOAPObjectIter ;
typedef ArraySOAPObject::const_iterator ArraySOAPObjectConstIter ;

#ifndef __linux__
class _EPISODUS SOAPObjectArray : public ArraySOAPObject
#else
class SOAPObjectArray : public ArraySOAPObject
#endif
{
  public:

    ClassificationPrinciple* pPrincipe ;

    SOAPObjectArray(ClassificationPrinciple* pPrinc = NULL, bool bOwnPointers = true) ;
    SOAPObjectArray(const SOAPObjectArray &rv) ;
    SOAPObjectArray(SOAPObjectArray *pNewSOAPObjectArray) ;
    ~SOAPObjectArray() ;

    SOAPObject* donneObjet(int iListIndex) ;
    int         donneNextListParam() ;

    void        vider() ;

    SOAPObjectArray& operator=(const SOAPObjectArray& src) ;

  protected:

    bool bOwner ; // L'array est-elle proprietaire des objets references
};

typedef vector<SOAPObjectArray*> ArraySOAPObjectArray ;
typedef ArraySOAPObjectArray::iterator       SOAPBasketIter ;
typedef ArraySOAPObjectArray::const_iterator SOAPBasketConstIter ;

#ifndef __linux__
class _EPISODUS SOAPBasket : public ArraySOAPObjectArray
#else
class SOAPBasket : public ArraySOAPObjectArray
#endif
{
  public:

    PrinciplesArray* pPrincipes ;

    SOAPBasket(PrinciplesArray* pPrincips, SOAPObject* pInitObject = NULL) ;
    SOAPBasket(const SOAPBasket &rv) ;
    ~SOAPBasket();

    SOAPObjectArray*    trouveCase(ClassificationPrinciple* pPrincipe) ;
    SOAPObject*         donneDiagObject() ;
    string              donneChapitre() ;
    bool                isEmpty() ;

    void                vider() ;

    SOAPBasket& operator=(const SOAPBasket& src) ;
};

typedef vector<SOAPBasket*> ArraySOAPBasket ;
typedef ArraySOAPBasket::iterator       SOAPBasketArrayIter ;
typedef ArraySOAPBasket::const_iterator SOAPBasketArrayConstIter ;

#ifndef __linux__
class _EPISODUS SOAPBasketArray : public ArraySOAPBasket
#else
class SOAPBasketArray : public ArraySOAPBasket
#endif
{
  public:

    SOAPBasketArray();
    SOAPBasketArray(const SOAPBasketArray &rv);
    ~SOAPBasketArray();

    void    vider();

    SOAPBasketArray& operator=(const SOAPBasketArray& src);
};

#endif		// __NSSOAPDIV_H
