#ifndef __NSVARRAY_H
#define __NSVARRAY_H

#include <string>
#include "partage/nsvector.h"
#include "partage/ntarray.h"

#ifndef __linux__
#if defined(_DANSLEXIDLL)
	#define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif
#endif

class BBItemData;
class BBItemInfo;
class BBDecodeData;
class BBChemData;
//vecteur de string

typedef vector<string*> EquiItem;
typedef EquiItem::iterator       EquiItemIter;
typedef EquiItem::const_iterator ConstEquiItemIter;

#ifndef __linux__
class _CLASSELEXI VecteurString : public EquiItem
#else
class VecteurString : public EquiItem
#endif
{
  public:

    VecteurString();
    VecteurString(char* iniTab[]); // Initialisation a partir d'un tableau de char*
    ~VecteurString();

    VecteurString(VecteurString& src);
    // Surcharge de l'operateur d'affectation
    VecteurString& operator=(const VecteurString& src);
    void vider();
    bool ItemDansUnVecteur(string sItem);
    string ItemStrictementInferieur(string sItem);
    string ItemStrictementSuperieur(string sItem);
    //donner le nombre d'items communs a deux chaines
    int NbElementCommun(VecteurString* pVectItemCible);
};

//vecteur de BBItemData
typedef vector<BBItemData*> EquiData;
typedef EquiData::iterator EquiDataIter;
typedef NTArray<BBItemData> VecteurData;

/*
class VecteurData : public EquiData
{
    public:

        VecteurData();
        ~VecteurData();
        VecteurData(VecteurData& src);
        // Surcharge de l'opérateur d'affectation
        VecteurData& operator=(VecteurData src);
        void vider();
};    */

//
// Definition de BBFicheArray (Array de BBItemInfo*)
//
typedef vector<BBItemInfo*> BBFicheVector;
typedef BBFicheVector::iterator BBFicheIter;

#ifndef __linux__
class _CLASSELEXI BBFicheArray : public BBFicheVector
#else
class BBFicheArray : public BBFicheVector
#endif
{
    public :

	    // Constructeurs
	    BBFicheArray() : BBFicheVector() {}
	    BBFicheArray(BBFicheArray& rv);
	    // Destructeur
	    virtual ~BBFicheArray();

        BBFicheArray& operator=(BBFicheArray src);
        void vider();
};

//vecteur de BBDecodeData
typedef vector<BBDecodeData*> EquiDecode;
typedef EquiDecode::iterator EquiDecodeIter;

class VecteurDecode : public EquiDecode
{
    public:

        VecteurDecode();
        ~VecteurDecode();
        VecteurDecode(VecteurDecode& src);
        // Surcharge de l'opérateur d'affectation
        VecteurDecode& operator=(VecteurDecode src);
        void vider();
};

//vecteur de vecteur de string
typedef vector <VecteurString*> EquiItemVector;
typedef EquiItemVector::iterator EquiItemVectorIter;

class VecteurItem : public EquiItemVector
{
    public:

        VecteurItem();
        ~VecteurItem();
        VecteurItem(VecteurItem& src);
        // Surcharge de l'operateur d'affectation
        VecteurItem& operator=(VecteurItem src);
        void    vider();
        size_t  NbElementSemantiqueCommun(VecteurString* pVectItemCible);
};

// RechercheSelonCritere : Critere	->	recherche semantique
//												-> recherche par objet
//	vecteur contenant la string du BBFilsItem a trouver,
//	un booleen (trouve ou non)
// un vecteur de vecteurs d'equivalents semantiques ou par objet
// un pointeur sur les donnees de la fiche trouvee

class  RechercheMere
{
    public:

        string 		    sEtiquette;
        bool	        trouve;
        VecteurItem*    pEquivalentTrie; //vecteur des equivalents tries
        VecteurItem*	pEquivalentBrut; //vecteur des equivalents non tries

        RechercheMere(string sEtiquette = "", bool trouve = false);
        ~RechercheMere();
        RechercheMere(const RechercheMere& src);
        RechercheMere& operator=(RechercheMere src);
        string ChaineStrictementSuperieur(VecteurString* pVectItemCible, string sDernierItemPere);
};

class  RechercheSelonCritereData : public RechercheMere
{
    public:

        BBItemData*		pDonnees;	  // Donnees de la fiche PARADOX GUIDE
        VecteurData*	pVecteurData;

        RechercheSelonCritereData(string sEtiquette = "", bool trouve = false);
        ~RechercheSelonCritereData();
        RechercheSelonCritereData(const RechercheSelonCritereData& src);
        RechercheSelonCritereData& operator=(RechercheSelonCritereData src);
        string ChaineStrictementSuperieur(VecteurString* pVectItemCible, string sDernierItemPere);
};

class  RechercheSelonCritereDecode : public RechercheMere
{
	public:
   BBDecodeData*	pDonnees;	  // Donnees de la fiche PARADOX DECODE
   VecteurDecode*	pVecteurData;
   RechercheSelonCritereDecode(string sEtiquette = "", bool trouve = false);
   ~RechercheSelonCritereDecode();
   RechercheSelonCritereDecode(const RechercheSelonCritereDecode& src);
   RechercheSelonCritereDecode& operator=(RechercheSelonCritereDecode src);
   string ChaineStrictementSuperieur(VecteurString* pVectItemCible, string sDernierItemPere);
};

//
//vecteur de RechercheSelonCritere : il y a autant d'elements dans ce vecteur que de
//BBFilsItem a trouver
//
typedef vector <RechercheMere*> VecRechSelonCritere;
typedef VecRechSelonCritere::iterator IterCritere;

#ifndef __linux__
class _CLASSELEXI VecteurRechercheSelonCritere : public VecRechSelonCritere
#else
class VecteurRechercheSelonCritere : public VecRechSelonCritere
#endif
{
	public:
   string sTypeBase;
   VecteurRechercheSelonCritere(string sTypeBase);
   ~VecteurRechercheSelonCritere();
   VecteurRechercheSelonCritere(VecteurRechercheSelonCritere& src);
   // Surcharge de l'operateur d'affectation
	VecteurRechercheSelonCritere& operator=(VecteurRechercheSelonCritere src);
   bool FilsAtraiter();
   void vider();
   bool ItemDansUnVecteur(int rang, string sItem);
   string BorneInf(int Rang);
   string ItemStrictementSuperieur(int Rang, string sItem);
   string ItemStrictementInferieur(int Rang, string sItem);
   string BorneInf();
   string ChaineStrictementSuperieur(VecteurString* pVectItemCible, string sDernierItemPere);
   void SetData(string sEtiquette, bool* trouve, BBChemData* pDonnees);
   void AjouteEtiquette(string sEtiquette);
};

/*
//											BBItemData
//vecteur de RechercheSelonCritere : il y a autant d'elements dans ce vecteur que de
//BBFilsItem a trouver
typedef vector <RechercheSelonCritere*> VecRechSelonCritereData;
typedef VecRechSelonCritereData::iterator IterCritereData;
class _CLASSELEXI VecteurRechercheSelonCritereData : public VecRechSelonCritereData, public VecteurRechercheSelonCritere
{
	public:
   VecteurRechercheSelonCritereData();
   ~VecteurRechercheSelonCritereData();
   VecteurRechercheSelonCritereData(VecteurRechercheSelonCritereData& src);
   // Surcharge de l'operateur d'affectation
	VecteurRechercheSelonCritereData& operator=(VecteurRechercheSelonCritereData src);
   void SetData(string sEtiquette, bool* trouve, BBItemData*	pDonnees);
   void AjouteEtiquette(string sEtiquette);
   void vider();
};


//											BBDecodeData
//vecteur de RechercheSelonCritere : il y a autant d'elements dans ce vecteur que de
//BBFilsItem a trouver
typedef vector <RechercheSelonCritereDecode*> VecRechSelonCritereDecode;
typedef VecRechSelonCritereDecode::iterator IterCritereDecode;
class _CLASSELEXI VecteurRechercheSelonCritereDecode : public VecRechSelonCritereDecode
{
	public:
   VecteurRechercheSelonCritereDecode();
   ~VecteurRechercheSelonCritereDecode();
   VecteurRechercheSelonCritereDecode(VecteurRechercheSelonCritereDecode& src);
   // Surcharge de l'opérateur d'affectation
	VecteurRechercheSelonCritereDecode& operator=(VecteurRechercheSelonCritereDecode src);
   void SetData(string sEtiquette, bool* trouve, BBDecodeData*	pDonnees);
   void AjouteEtiquette(string sEtiquette);
   void vider();
};
*/

#endif
