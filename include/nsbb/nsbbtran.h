//---------------------------------------------------------------------------
//    NSBigBro.H
//
//  Contient la declaration de NSBigBrother, gestionnaire general des
//  applications NAUTILUS.
//  KP
//  Creation : sept 93         Derniere mise a jour : mars 94
//---------------------------------------------------------------------------
#ifndef __NSBBTRAN_H
#define __NSBBTRAN_H

class BBFilsItem;
class NSControle;
class NSPatPathoArray;
class NSLinkedNodeArray;

#include <cstring>
#include <string>
#include <vector>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "nsbb/nspatpat.h"

using namespace std;

// valeurs pour szType
const char nbMARK        = 'N';   // Saisie de chiffres pour l'Edit
const char charMARK      = 'C';   // Saisie de caractères pour l'Edit
const char dateMARK      = 'D';   // Saisie de date pour l'Edit
const char dateHeureMARK = 'T';   // Saisie de date et heure pour l'Edit
const char heureMARK     = 'H';   // Saisie d'une heure pour l'Edit

// valeurs pour Type
const char LibreMARK2    = 'L';   // libelle
const char codMARK2      = 'C';   // code

class Message //xportable car utilisée dans KE
{
  private:

 	  static long lObjectCount;

  protected :

    string sTreeID;
    string sNoeud;
    string sLexique;
    string sComplement;	  // Données à transférer : complement
    string sCertitude;
    string sUnit;
    string sInteret;
    string sPluriel;
    string sVisible;
    string sType;        // type des donnees
    string sTexteLibre;  // texte du TL
    string sArchetype;   // archetype
    string sRights;      // Right of the pathPatho

    NSLinkedNodeArray* pTemporaryLinks;

  public :

    // Constructeurs
    Message(string Noeud, string Lexique = string(""), string Complement = string(""),
                  string sType      = string(""), string sInteret = string(""),
                  string sCertitude = string(""), string sPluriel = string(""),
                  string sVisible   = string(""), string sTL      = string(""),
                  string sArc       = string(""), string sUnite   = string(""),
                  string srig       = string(""));
    Message();
    Message(const Message& src);

    // Destructeur
    ~Message();

    void Reset();

    // Operateurs Get - Set
    string GetTreeID (){ return sTreeID; }
    void   SetTreeID (const string treeID) { sTreeID = treeID; }
    void   SetTreeID (const char* treeID)  { sTreeID = string(treeID); }

    string GetNoeud (){ return sNoeud; }
    void   SetNoeud (const string Noeud) { sNoeud = Noeud; }
    void   SetNoeud (const char* Noeud)  { sNoeud = string(Noeud); }

    string GetLexique (){ return sLexique; }
    void   SetLexique (const string Lexique) { sLexique = Lexique; }
    void   SetLexique (const char* Lexique)  { sLexique = string(Lexique); }

    string GetCertitude (){ return sCertitude; }
    void   SetCertitude (const string Certitude) { sCertitude = Certitude; }
    void   SetCertitude (const char* Certitude)  { sCertitude = string(Certitude); }

    string GetInteret (){ return sInteret; }
    void   SetInteret (const string Interet) { sInteret = Interet; }
    void   SetInteret (const char* Interet)  { sInteret = string(Interet); }

    string GetComplement (){ return sComplement; }
    void   SetComplement (const string Complement) { sComplement = Complement; }
    void   SetComplement (const char* Complement)  { sComplement = string(Complement); }

    string GetPluriel (){ return sPluriel; }
    void   SetPluriel (const string Pluriel) { sPluriel = Pluriel; }
    void   SetPluriel (const char* Pluriel)  { sPluriel = string(Pluriel); }

    string GetType () {return sType; }
    void   SetType (const string type) { sType = type; }
    void   SetType (const char* type)  { sType = string(type); }

    string GetTexteLibre () {return sTexteLibre; }
    void   SetTexteLibre (const string tl) { sTexteLibre = tl; }
    void   SetTexteLibre (const char* tl)  { sTexteLibre = string(tl); }

    string GetArchetype () { return sArchetype; }
    void   SetArchetype (const string arc) { sArchetype = arc; }
    void   SetArchetype (const char* arc)  { sArchetype = string(arc); }

    string GetVisible (){ return sVisible; }
    void   SetVisible (const string Visible) { sVisible = Visible; }
    void   SetVisible (const char* Visible)  { sVisible = string(Visible); }

    string GetUnit (){ return sUnit; }
    void   SetUnit (const string sUnite) { sUnit = sUnite; }
    void   SetUnit (const char* szUnite) { sUnit = string(szUnite); }

    inline string GetNodeRight (){ return sRights; }
    inline void   SetNodeRight (const string sRig) { sRights = sRig; }
    inline void   SetNodeRight (const char* srig)  { sRights = string(srig); }

    NSLinkedNodeArray*  GetTemporaryLinks() { return pTemporaryLinks; }
    void                SetTemporaryLinks(NSLinkedNodeArray* pArray);

    void MettreAJourCertitudePluriel(string sEtiquette);
    void InitFromEtiquette(string sEtiquette);

    // Surcharges de l'operateur
    Message& operator=(Message src);

    static long getNbInstance() { return lObjectCount; }
    static void initNbInstance() { lObjectCount = 0; }
};

#ifndef _ENTERPRISE_DLL

//--------------------------------------------------------------------------
// Structure qui sert au transfert pour chaque controle
//--------------------------------------------------------------------------
class _NSBBCLASSE NSTransferInfo
{
  private:

 	  static long lObjectCount;

  public:

    BBFilsItem*	_pBBFilsItem;		    // BBItem de pilotage du contrôle

    int					_iActif;               // Contrôle ON/OFF
    Message     _TransfertMessage;    // données patpatho complement, certitude,...

    int					_iTmpActif;            // Informations temps réel
    Message*    _pTmpTransfertMessage; // avant validation

    NSVectFatheredPatPathoArray _TransPatpatho;       // Patpatho "vraie"
    NSVectFatheredPatPathoArray _TempoPatpatho;       // Patpatho temporaire

    NSControle* _pControle;	        // Pointeur sur l'objet NSControle

    // Constructeurs
    NSTransferInfo(BBFilsItem* pFilsItem, int actif = 0,
                   string noeud     = string(""), string complement = string(""),
                   string type      = string(""), string interet    = string(""),
                   string certitude = string(""), string pluriel    = string(""),
                   string visible   = string(""), string sTL        = string(""),
                   string sArc      = string(""), string sUnite     = string(""));

    NSTransferInfo(BBFilsItem* pFilsItem, const NSTransferInfo& src);
    NSTransferInfo(const NSTransferInfo& src);
    // Destructeur
    ~NSTransferInfo();

    // lecture/écriture sur l'objet contrôle de iActif et sDonnees
    uint Transfer(TTransferDirection direction);
    uint TransferFinal(TTransferDirection direction);

    uint TempTransfer();

    // Fonction d'activation/désactivation du contrôle Windows
    void activeControle(int activation, Message* pMessage = 0);
    // par défaut, envoie ctrlNotification à tous les fils de pTransPatpatho (indexFils == -1)
    // sinon, envoie ctrlNotification au fils indexFils du vecteur pTransPatpatho
    void ctrlNotification(int etatInitial, int etatSouhaite, Message* pMessage = 0, int indexFils = -1);
    void ctrlAjout();
    void ctrlDestruction();

    // Fonction de référencement/déréférencement du pointeur pControle
    void referenceControle(NSControle* pCtrl);
    void detacheControle();
    void Desactive();
    void Active();

    BBFilsItem*			             getFilsItem()   { return _pBBFilsItem; }
    NSVectFatheredPatPathoArray* getPatPatho()   { return &_TransPatpatho; }
    NSVectFatheredPatPathoArray* getTmpPatho()   { return &_TempoPatpatho; }

    NSControle* getControl()                     { return _pControle; }
    void        setControl(NSControle* pCt)      { _pControle = pCt; }

    int	        isActif()                        { return _iActif; }
    void        setActif(int iA)                 { _iActif = iA; }

    Message*    getTransfertMessage()            { return &_TransfertMessage; }

    int	        isTmpActif()                     { return _iTmpActif; }
    void        setTmpActif(int iA)              { _iTmpActif = iA; }

    Message*    getTmpTransfertMessage()            { return _pTmpTransfertMessage; }
    void        setTmpTransfertMessage(Message* pM) { _pTmpTransfertMessage = pM; }

    // Surcharges deS l'opérateurS
    NSTransferInfo& operator=(NSTransferInfo src);
    int             operator==(const NSTransferInfo& x);

    static long getNbInstance() { return lObjectCount; }
    static void initNbInstance() { lObjectCount = 0; }
};

//--------------------------------------------------------------------------
// Classe des matrices dynamiques de transfert qui est systèmatiquement mis
// en place pour chaque boîte de dialogue NSDialog
// (On est obligé de faire la déclaration de classe pour que la matrice
//   soit dynamique puisque dans CLASSLIB delta est par défaut égal à 0.)
//--------------------------------------------------------------------------
typedef vector<NSTransferInfo*> TransferArray;
typedef TransferArray::iterator iterTransferArray;

class NSTransferArray : public  TransferArray
{
	int indexCurr;    // Index courant

 public :

	TTransferDirection  Direction; // Direction de transfert

	// Constructeurs
	NSTransferArray() : TransferArray() {}
	NSTransferArray(NSTransferArray& rv);

	// Destructeur
	virtual ~NSTransferArray();

	// Fonctions de manipulation de l'index courant
	void initIndex() { indexCurr = 0; }
	void incIndex()  { indexCurr++; }
	int  index()     { return indexCurr; }

	// Fonction de positionnement de Direction
	// ( tdGetData, tdSetData, tdSizeData)
	void setDirection(TTransferDirection direction) { Direction = direction; }
};

#endif // #ifndef _ENTERPRISE_DLL

#endif // #ifndef __NSBBTRAN_H

