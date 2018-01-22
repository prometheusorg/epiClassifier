//---------------------------------------------------------------------------
//     NsFilGd.H
//
//  PA Aout 1993  && Hafedh Kaddachi Mai 97
//---------------------------------------------------------------------------
#ifndef __NSFILGD_H
#define __NSFILGD_H

#include <vector>

typedef char DBITBLNAME[261] ;
  
#define	GUIDE  string("GUIDE")
#define	DECODE string("DECODE")

class NSContexte ;
class BBChem ;
class BBFiche ;
class BBDecode ;
class NSPatPathoArray ;
class ClasseStringVector ;
class VectString ;
class VecteurItem ;
class VecteurString ;
class NSPhraseur ;
class NsProposition ;
class NSGenerateur ;
class ChoixPathoDialog ;
class NSPatholog ;
class NSLexiMed ;
class NSPathologData ;
class NSSavoir ;

#include "nssavoir/nsvarray.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
#else
  #include "partage\nsglobal.h"
  #include "partage\nsdivfct.h"
  // #include "nautilus\nssuper.h"
#endif

bool inferieure(string* s, string* b) ;

//
// Classe d'acces au dictionnaire
//
#ifndef __linux__
class _CLASSELEXI NSDico : public NSRoot
#else
class NSDico : public NSRoot
#endif
{
	public:

  	enum DCODETYPE { dcPhrase, dcTiret } ;

    // Boite de dialogue de choix des elements du lexique
    ChoixPathoDialog* _pDicoDialog ;

    // Objet d'acces a la base de donnees Lexique pour l'utilisateur
    string            _sUserLang ;
    NSPhraseur*       _pPhraseur ;
    NsProposition*    _pProposition ;
    NSGenerateur*     _pGenerateur ;
    NSPatholog* 	    _pPatho ;         // lexique
    NSLexiMed* 		    _pLexiMed ;       // base des medicaments

    // Objet d'acces pour une langue autre que celle de l'utilisateur
    string            _sLangBis ;
    NSPatholog* 	    _pPathoBis ;      // lexique
    NSLexiMed* 		    _pLexiMedBis ;    // base des medicaments

    bool   			      _bOuvreFerme ;

    // Constructeur et destructeur
    NSDico(NSContexte* pCtx) ;
    ~NSDico() ;

    // Fonction de prise de main
    bool Prend() ;
    void Relache() ;
    bool UserChanged() ;

    // Fonctions utilitaires

    enum LibelleType { brut, choisi, affiche } ;

    // Trouve le bon Lexique
    NSPatholog* donnePatholog(string sLang, string* pCodeLexique) ;
    // Trouve le libelle d'un element dont on connait le code
    bool donneLibelle(string sLang, string* codeLexique, string* libelleTrouve, int precision = affiche, GENRE *pGenre = 0) ;
    // Trouve le libelle brut du lexique
    bool donneLibelleLexique(string sLang, string* codeLexique, string* libelleTrouve) ;
    // Trouve le libelle d'un chemin de codes lexiques
    bool donneLibelleChemin(string sLang, string chemin, string* libelle, string sep = "/") ;

    // Trouve le plus court libelle d'un element dont on connait le code
    bool donneShortLibelle(string sLang, string* codeLexique, string* libelleTrouve, int precision = affiche, int *pGenre = 0) ;

    bool trouvePathologData(string sLang, string* pCode, NSPathologData* pData, bool afficheErreur = true) ;
    // Trouve le code d'un element dont on connait le libelle
    bool donneCode(string sLang, string* libelleLexique, string* codeTrouve) ;

    // Donne la categorie
    static string CodeCategorie(string sChaine) ;

    bool   isDrug(string* pCodeLexiqueOrSens) ;
    bool   isDrugOrTreatment(string* pCodeLexiqueOrSens) ;

    // Donne le code Sens en fonction du code lexique
    static void   donneCodeSens(string* pCodeLexique, string* pCodeSens) ;
    static void   donneCodeSens(string& sCode) ;
    static string donneCodeSens(string* pCodeLexique) ;
    static void   donneCodeComplet(string& sCode) ;

    // chercher dans la patpatho de synthese le complement correspondant a sItem
    // exemple si sItem = VPOIDS retourner la valeur du poids
    static string SetData(string sItem, string* pUnite, NSPatPathoArray* pPathoDocEnCours) ;

    // Decompose PGABA1/WCE501/WPLUR1 en ses composants
    void separe(string* psChaine, string* psLexique, string* psCertitude, string* psPluriel) ;

    bool Decoder(string sLang, string sChemin, NSPatPathoArray* pSousPatPatho,
                       ClasseStringVector* pVect, int precision = dcTiret) ;

    NSPatholog*       getPatholog()	  { return _pPatho ; }
    NSLexiMed*        getLexiMed()    { return _pLexiMed ; }
    NSGenerateur*     getGenerateur() { return _pGenerateur ; }
    ChoixPathoDialog* getDicoDialog() { return _pDicoDialog ; }
    void              setDicoDialog(ChoixPathoDialog* pPD) { _pDicoDialog = pPD ; }
};

#define	pathDistanceExact              0  // term to term
#define	pathDistanceGeneric         1000  // just jokers (*, ?)
#define	pathDistanceSemantic        2000  // just semantic equivalents
#define	pathDistanceSemanticGeneric 3000  // semantic + jokers
#define	pathDistanceInfinite        4000  // semantic + jokers

//
// Base block of a semantic search: search criteria plus result
//
#ifndef __linux__
class _CLASSELEXI SemanticSearchElement
#else
class SemanticSearchElement
#endif
{
  protected:

    string     _sCode ;
    string     _sRelation ;
    DBITBLNAME _sensCle ;

    VectString _resultVecteurString ;

  public:

    SemanticSearchElement() ;
    SemanticSearchElement(string sCode, string sRelation, DBITBLNAME sensCle, VectString *pVectString) ;
    SemanticSearchElement(SemanticSearchElement& rv) ;
    ~SemanticSearchElement() ;

    bool isSameSearch(const SemanticSearchElement& o) ;
    bool isSameSearch(const string sCode, const string sRelation, const DBITBLNAME sensCle) ;

    VectString* getResultVecteurString() { return &_resultVecteurString ; }

	  SemanticSearchElement& operator=(SemanticSearchElement src) ;
	  int 			  	         operator==(const SemanticSearchElement& o) ;
} ;

// array of base blocks
//
typedef vector<SemanticSearchElement *>       SemanticSearchElementVector ;
typedef SemanticSearchElementVector::iterator SemanticSearchElementIter ;
typedef NTArray<SemanticSearchElement>        NSSemanticSearchElementArray ;

#define maxSearchElement 20

#ifndef __linux__
class _CLASSELEXI SemanticSearchElementArray : public NSSemanticSearchElementArray
#else
class SemanticSearchElementArray : public NSSemanticSearchElementArray
#endif
{
  protected:

  void addElement(string sCode, string sRelation, DBITBLNAME sensCle, VectString* pVectString) ;

  public:

  // Constructeurs
	SemanticSearchElementArray() : NSSemanticSearchElementArray() {}
	SemanticSearchElementArray(SemanticSearchElementArray& rv) ;
  SemanticSearchElementArray& operator=(SemanticSearchElementArray src) ;

  VectString* getExistingResult(string sCode, string sRelation, DBITBLNAME sensCle) ;
  void        addResult(string sCode, string sRelation, DBITBLNAME sensCle, VectString* pVectString) ;

	// Destructeur
  ~SemanticSearchElementArray() ;
} ;

//
// Classe d'acces aux fils guides et au reseau semantique
//
#ifndef __linux__
class _CLASSELEXI NSFilGuide : public NSSuperRoot
#else
class NSFilGuide : public NSSuperRoot
#endif
{
  protected:

    SemanticSearchElementArray aSearchEltsArray ;

	public:

  	string sTypeBase; //GUIDE.DB ou DECODE.DB

    // Objet d'acces a la base de donnees des Fils guides
    BBChem*	 		 pBBFiche;

    // Objet d'acces a la base de donnees du reseau semantique
    NSSavoir* 	 pSavoir ;
    VecteurItem* pVecEquivalentItemDuPere ; //
                                            //vecteur contenant les vecteurs des
                                            //equivalents au sens semantique des
                                            //items dans la chaine du pere
                                            //

    bool   	 		bOuvreFerme ;
    enum compTYPE { compStrict, compReseau, compObjet } ;

    // Constructeur et destructeur
    NSFilGuide(NSSuper* pSuper, string sTypeBase) ;
    ~NSFilGuide() ;

    // Fonction de prise de main
    bool Prend() ;
    void Relache() ;

    // inverser une chaine
    void InverseChaine(string* Localisation, string* pInverse) ;

    int  pathDistance(string* psTarget, string* psSource, bool bWithSemantic = true, DBITBLNAME sensCle = "FLECHE") ;
    int  specificPathDistance(VecteurString* pVectItemTarget,
                                    VecteurItem* pEquivalentSource,
                                        VecteurString* pVectItemSource) ;

    // egalite au sens reseau
    bool testeEgaliteChemin(VecteurString* pVectItemCible, EquiItemIter IterCible,
                     VecteurItem* EquivalentSource, EquiItemVectorIter vect) ;

    void DecomposeChaine(string* pChaine, VecteurString* pVect ) ;
    bool comparerItem(string sItem2, VecteurString* pEquivalentItem) ; //comparaison de deux items

#ifndef _ENTERPRISE_DLL
    void chercheEquivalent(string sQualifie, VecteurString* pEquivalentItem,
                               string sLien, DBITBLNAME sensCle = "FLECHE") ;
#else
    void chercheEquivalent(string sQualifie, VecteurString* pEquivalentItem,
                               string sLien, string sensCle = string("FLECHE")) ;
#endif
                               // ranger dans pEquivalent
                               // les equivalents au sens reseau de
                               // sQualifie

    //cas de sous chaines
    void testeEgaliteChaineCible(VecteurRechercheSelonCritere* pVecteurSelonCritere,
                               int NBSeparateur, string* pCheminRecherche, string pCible) ;

    void chercheCheminReseau(string* pChaine, VecteurRechercheSelonCritere* VecteurSelonCriter) ; //chercher dans le filguide

    //cherche chemin : semantique
    void chercheChemin(string* pChemin,
                           VecteurRechercheSelonCritere* VecteurSelonCriter,
                           int precision = compReseau) ;

    //par objet
    void chercheCheminObjet(string* pChemin,
                                VecteurRechercheSelonCritere* VecteurSelonCriter) ;

    //egalite stricte
    void chercheCheminEgaliteStricte(string* pChaine, VecteurRechercheSelonCritere* VecteurSelonCriter) ;

    void ChoisirMeilleureProposition(VecteurRechercheSelonCritere* pVecteurSelonCritere) ;

    // Fonction de comparaison entre chemins
    bool estEgal(string* pChaine, string* pModele, int niveau = compStrict) ;

    void MettreAjourEquivalent(int NombreElementEnCommun) ;
    int  NombreSeparateurDansUneChaine(string *pChaine) ;

    bool RechercheFicheSuivante(VecteurString* pVectItemPere,
                    string sCible, VecteurRechercheSelonCritere* pVecteurSelonCritere,
                    int NombreElementEnCommun) ;

    string NouvelleCible(VecteurString* pVectItemPere, int NombreElementEnCommun,
                        VecteurRechercheSelonCritere* pVecteurSelonCritere) ;

    //savoir si sCode1 et sCode2 sont lies par sRelation
    bool VraiOuFaux(string sCode1, string sRelation, string sCode2, DBITBLNAME sensCle = "FLECHE", bool bPumpMessages = true) ;
    bool InverseVraiOuFaux(string sCode1, string sRelation, string sCode2, DBITBLNAME sensCle = "ENVERS", bool bPumpMessages = true) ;

    //mettre tous les items lies à sCode1 par sRelation dans pVecteurString
    void TousLesVrais(string sCode1, string sRelation, VectString* pVecteurString, DBITBLNAME sensCle = "FLECHE", bool bPumpMessages = true) ;
    void InverseTousLesVrais(string sCode1, string sRelation, VectString* pVecteurString, DBITBLNAME sensCle = "ENVERS", bool bPumpMessages = true) ;

    //premier item lie a sCode1 par sRelation
    string PremierVrai(string sCode1, string sRelation, DBITBLNAME sensCle = "FLECHE") ;
    string InversePremierVrai(string sCode1, string sRelation, DBITBLNAME sensCle = "ENVERS") ;

    string chercheQualifiant(string sQualifie, string sLien, DBITBLNAME sensCle = "FLECHE") ;
};

#endif

// __NSSAVOIR_H
