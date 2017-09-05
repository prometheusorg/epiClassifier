// --------------------------------------------------------------------------
// NSEPISODVIEW.H		Document/Vues Capture Donnees Utilisateur
// --------------------------------------------------------------------------
// Fabrice LE PERRU - Aout 2001
// source : Rémi SPAAK - Mai 1997
// --------------------------------------------------------------------------

#if !defined(__NSSOAPDIV_H)
#define __NSSOAPDIV_H

class BBFilsItem ;

#include "nsepisod/eptables.h"

// enum category_type {subjective = 0, objective, assesment, plan} ;

class ClassificationPrinciple

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

class PrinciplesArray : public ArrayPrinciples, public NSSuperRoot

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

class SOAPObject

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
    string      sNoeud;         // Pour un noeud deja enregistre
    BBFilsItem* pFilsItem;      // Pour un noeud interne en cours de saisie

    bool        bFloculable ;

    SOAPObject() ;
    SOAPObject(string sTI, string sCd, string sCl, int iCf, string sNoeud) ;
    SOAPObject(string sTI, string sCd, string sCl, int iCf, BBFilsItem* pItem) ;
    SOAPObject(const SOAPObject &rv) ;
    SOAPObject(SOAPObject *pNewSOAPObject) ;
    ~SOAPObject() ;

    string  donneLibelle() ;
    string  donneChapitre() ;

    bool    estFloculable()             { return bFloculable ; }
    void    setFloculable(bool bFloc)   { bFloculable = bFloc ; }

    SOAPObject& operator=(const SOAPObject& src) ;
};

typedef vector<SOAPObject*>             ArraySOAPObject ;
typedef ArraySOAPObject::iterator       ArraySOAPObjectIter ;
typedef ArraySOAPObject::const_iterator ArraySOAPObjectConstIter ;

class SOAPObjectArray : public ArraySOAPObject
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


class SOAPBasket : public ArraySOAPObjectArray
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

class SOAPBasketArray : public ArraySOAPBasket
{
  public:

    SOAPBasketArray();
    SOAPBasketArray(const SOAPBasketArray &rv);
    ~SOAPBasketArray();

    void    vider();

    SOAPBasketArray& operator=(const SOAPBasketArray& src);
};

#endif		// __NSSOAPDIV_H
