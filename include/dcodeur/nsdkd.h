//-------------------------------------------------------------------------
//                    BIBLIOTHEQUE : NSDKD.H
//
//                     Fonctions utilitaires des decodeurs
//
//             Debut d'ecriture : 30 Juin 1988 (PA)
//             Derniere version : 28 Juillet 1994 (PA)
//-------------------------------------------------------------------------
#ifndef __NSDKD_H
#define __NSDKD_H

#include <vector>

class NSPatPathoArray;
class NSPatPathoInfo;
typedef std::vector<NSPatPathoInfo*>::iterator PatPathoIter;

class NSPhraseur;
class NsProposition;
class NSGenerateur;

class NSCim10Info;
class NSCim10;
class NSCcamInfo;
class NSCcam;

class NSMaterielInfo;

class NSPersonInfo;

#ifndef _ENTERPRISE_DLL
#include "partage/ns_vector.h"
typedef std::vector<OWL::TTreeNode*>   VectorTTreeNode;
typedef VectorTTreeNode::iterator IterTTreeNode;
#include "partage/NTArray.h"
typedef NTArray<OWL::TTreeNode>   VectTTreeNode;
#endif

# include "nautilus/nscr_ama.h"
# include "nsbb/nspatpat.h"

//
// Enumeration des types de fenetres
//
enum AIDEDECODTYPE { iOk = 0, iAnnule, iCorrige, iErreur, iAutomatique };

// enum GENRE { genreMS = 1, genreFS, genreMP, genreFP, genreIT};

class decodageBase;

class AdjIntens
{
 public:

	string sAdjectif;
  string sIntensite;

  AdjIntens();
};

/***************************************************************************/

class gereDate : public NSSuperRoot
{
 public :

	gereDate(NSSuper* pSuper, string sLangue);
  gereDate(const gereDate&);
  ~gereDate() {}

  gereDate& operator =(const gereDate&);

  void    initialise();

  void    setDate(string* nouvDate)  { sDate      = *nouvDate; }
  void    setFormat(string* nouvFmt) { sFormatage = *nouvFmt; }
  void    setUnite(string* nouvUnit) { sUnite 		= *nouvUnit; }

  string  getDate()       { return sDate; }
  string  getUnit()       { return sUnite; }
  string  getLibelle()    { return sLibelle; }

  bool    estVide()       { return sDate == ""; }

  void    donne_date_breve(string* pMessage, string* pIntro);
  void    donne_date_claire(string* pMessage, string* pIntro);
  string  donne_mois(string mois);
  string  donne_mois(int iMois);

  void    setLang(string sLng) { sLang = sLng; }
  string  getLang()            { return sLang; }

	//protected :

  string  sDate;
  string  sFormatage;

  string  sUnite;
  string  sLibelle;
  string  sLang;
};

class gereNum;

class numStorage
{
 public :

  string    sNum, sNumInf, sNumSup;
  double    valeur, valeurInf, valeurSup;
  bool      bExact, bInf, bSup;
  bool      bInfEgal, bSupEgal;

  string    sFormatage;
  string    sUnite;

  numStorage();
  numStorage(const numStorage&);
  ~numStorage();

  void     initToBlank();
  void     init(const numStorage* pSrc);

  void     setNormale(gereNum* pNorm);
  void     setNormInf(gereNum* pNorm);
  void     setNormSup(gereNum* pNorm);

  gereNum* getNormale() { return pNormale; }
  gereNum* getNormInf() { return pNormInf; }
  gereNum* getNormSup() { return pNormSup; }

  numStorage& operator =(const numStorage&);

 protected :

	gereNum*  pNormale;
  gereNum*  pNormInf;
  gereNum*  pNormSup;
};

typedef vector<numStorage*>        VectorNumStorage;
typedef VectorNumStorage::iterator IterNumStorage;
typedef NTArray<numStorage>        VectNumStorage;

class gereNum : public NSSuperRoot
{
 public :

	enum EXTENSIONS { RIEN = 0, NORMALES };

	gereNum(NSSuper* pSuper, string sLangue);
  gereNum(gereNum&);
  ~gereNum();

  gereNum& operator =(gereNum&);

  string  donneLibelleUnite(NSContexte *pContexte, int iIndice = 0);

  void    initialise();
  void    instancier(string* pNum, string* pUnit, string* pFrmt, int iIndice = 0);
  void    strToDble(string* pNum, string* pBonNum, double* dValeur);

  void    setNum(string* nouvNum, int iIndice = 0);
  void    setUnite(string* nouvUnit, int iIndice = 0);
  void    setFormt(string* nouvFrmt, int iIndice = 0);
  void    setVal(double nouVal, int iIndice = 0);

  bool    setNormale(gereNum* pNorm, int iIndice = 0);
  bool    setNormInf(gereNum* pNorm, int iIndice = 0);
  bool    setNormSup(gereNum* pNorm, int iIndice = 0);

  string  getNum(int iIndice = 0);
  string  getNumInf(int iIndice = 0);
  string  getNumSup(int iIndice = 0);
  string  getNum(string sFormatage, EXTENSIONS eExtensions = RIEN, int iIndice = 0);
  string  getNormale(int iIndice = 0);

  gereNum* getNorm(int iIndice = 0);
  gereNum* getNormInf(int iIndice = 0);
  gereNum* getNormSup(int iIndice = 0);

  double  getValeur(int iIndice = 0);
  double  getValeurInf(int iIndice = 0);
  double  getValeurSup(int iIndice = 0);

  bool    estExact(int iIndice = 0);
  bool    estInf(int iIndice = 0);
  bool    estSup(int iIndice = 0);
  bool    estInfEgal(int iIndice = 0);
  bool    estSupEgal(int iIndice = 0);

  string  getUnite(int iIndice = 0);
  string  getFormat(int iIndice = 0);

  bool    estVide(int iIndice = 0);

  void    setLang(string sLng)       { _sLang = sLng; }
  string  getLang()                  { return _sLang; }

  gereDate* getDate()                { return _pDateRef; }
  void      setDate(gereDate* pDate);

  numStorage* getStorageFromIndice(int iIndice, bool bAutoCreate = false);

 protected :

 	VectNumStorage _aValues;
  gereDate*      _pDateRef;
  string         _sLang;
};

class gereHeure : public NSSuperRoot
{
 public :

	gereHeure(NSSuper* pSuper, string sLangue);
  gereHeure(const gereHeure&);
  ~gereHeure() {}

  gereHeure& operator =(const gereHeure&);

  void   initialise();

  void   setHeure(string* nouvDate) { sHeure = *nouvDate; }
  void   setFormat(string* nouvFmt) { sFormatage = *nouvFmt; }
  void   setUnite(string* nouvUnit) { sUnite 		= *nouvUnit; }

  string getHeure()   { return sHeure; }
  string getLibelle() { return sLibelle; }

  bool   estVide()    { return sHeure == ""; }

  void   donne_heure(string* pMessage);

  void   setLang(string sLng) { sLang = sLng; }
  string getLang()            { return sLang; }

  //protected :

	string sHeure;
  string sFormatage;

  string sUnite;
  string sLibelle;
  string sLang;
};

class gereCode : public NSRoot
{
 public :

	gereCode(NSContexte* pCtx, string sLangue);
  gereCode(const gereCode&);
  ~gereCode() {}

  gereCode& operator =(const gereCode&);

  void   initialise();

  void   setCode(string* nouvCode) { sCode = *nouvCode; }
  void   setClassification(string* nouvClass) { sClassification = *nouvClass; }

  string getCode()    { return sCode; }
  string getLibelle() { return sCode; }

  bool   estVide()    { return sCode == ""; }

  void   donne_code(string* pMessage, bool bAvecCode = true);

  void   setLang(string sLng) { sLang = sLng; }
  string getLang()            { return sLang; }

  //protected :

  string sCode;
  string sClassification;
  string sLang;
};

//
// Phrases de decodage : le decodage en memoire permet un tri dynamique
//
class NSDkdPhrase
{
 public:

	int     categorie;      // titre, sstitre, phrase ordinaire...
  string  sTexte;

  string  sEtiquette;     // pour le tri ulterieur

  string  decDeb;
  string  decFin;
  int     sautLigne;
  string  locLesion;

  NSDkdPhrase();
  NSDkdPhrase(NSDkdPhrase& rv);
  ~NSDkdPhrase();
  NSDkdPhrase& operator=(NSDkdPhrase src);
  int operator == (const NSDkdPhrase& o);
  int operator != (const NSDkdPhrase& o) { return !(*this==o); }
};

typedef vector<NSDkdPhrase*> NSDkdPhrArray;
typedef NSDkdPhrArray::iterator NSDkdPhrArrayIter;
typedef NTArray<NSDkdPhrase> NSDkdPhraseArray;

//-----------------------------------------------------------------
// classe decodage
//
// Classe de base des classes de decodage
//-----------------------------------------------------------------
class dkdNoyau : public NSRoot
{
 public:
  NSPatPathoArray*  pPatPathoArray;
  PatPathoIter      itDcode, itMDcode;
  string			      codeLexique, complement;

  string			      locLesion;
  string			      locPath;

  string 			      codeLexUtil;
  bool			        codeUtilValid;
  string 			      codeLexDecal, complDecal;

  NSDkdPhraseArray* pDkdPhrases;

  NSCRPhraLesArray* pPhraLes;
  NSCRPhraseArray*  pPhrases;
  string 			      sDcodeur;
  int 	  		      iBon, iCorrigeAnnule;
  bool			        bRecupFenetre;
  bool              bDansFichier;

  dkdNoyau(NSContexte* pCtx);
  ~dkdNoyau();

  void ouvreFichier(char *fichier);
  void fermeFichier();
};

//-----------------------------------------------------------------
// classe decodage
//
// Classe de base des classes de decodage
//-----------------------------------------------------------------
class decodageBase : public NSRoot
{
 private:

	dkdNoyau* pNoyau;
  bool	    bCreateur;

 public:

	NSPhraseur*    pPhraseur;
  string         sLangue;
  NSGenerateur*  pGenerateur;
  NsProposition* pPropos;

  // Methodes
  //
  decodageBase(NSContexte* pCtx, string sLangue = "fr");

  // Constructeur copie : duplique le noyau
  decodageBase(decodageBase& rv);
  // Constructeur filiation : partage le meme noyau
  decodageBase(decodageBase* ptr);

  ~decodageBase();

  string*          sDcodeur() { return &(pNoyau->sDcodeur); }

  void             setPPtArray(NSPatPathoArray* pPPtArray);
  NSPatPathoArray* getPPtArray() { return pNoyau->pPatPathoArray; }

  void              setDkA(NSDkdPhraseArray* pDkA) { pNoyau->pDkdPhrases = pDkA; }
  NSDkdPhraseArray* getDkA() { return pNoyau->pDkdPhrases; }

  void              setPLA(NSCRPhraLesArray* pPLA) { pNoyau->pPhraLes = pPLA; }
  NSCRPhraLesArray* getPLA() { return pNoyau->pPhraLes; }

  void              setPhA(NSCRPhraseArray* pPhA) { pNoyau->pPhrases = pPhA; }
  NSCRPhraseArray*  getPhA() { return pNoyau->pPhrases; }

  void              set_iBon(int valeur) { pNoyau->iBon = valeur; }
  int               iBon() { return pNoyau->iBon; }

  void              setCorrAnnu(int valeur) { pNoyau->iCorrigeAnnule = valeur; }
  int               getCorrAnnu() { return pNoyau->iCorrigeAnnule; }

  void              setRecupFen(bool valeur) { pNoyau->bRecupFenetre = valeur; }
  bool              getRecupFen() { return pNoyau->bRecupFenetre; }

  void              setDsFichier(bool valeur) { pNoyau->bDansFichier = valeur; }
  bool              getDsFichier() { return pNoyau->bDansFichier; }

  PatPathoIter      getitDcode()  { return pNoyau->itDcode; }

  void			  Avance(int nbPositions = 1);
  void			  Recule(int nbPositions = 1);
  void			  MetMarque();
  void			  VaMarque();

  void			  Suivant();
  void			  Precedent();

  string*		 	getSt(); // Code utile
  string*			getStL() { return &(pNoyau->codeLexique); } // Code complet

  void		 	  getCert(string* pCert); // Certitude
  void		 	  getPlur(string* pPlur); // Pluriel

  string			getCert()       { return (*(pNoyau->itDcode))->getCertitude(); }
  string			getPlur()       { return (*(pNoyau->itDcode))->getPluriel(); }
  string			getLexique()    { return (*(pNoyau->itDcode))->getLexique(); }
  string			getTexteLibre() { return (*(pNoyau->itDcode))->getTexteLibre(); }

  string*			getSt(int decal);  // Code utile
  string*			getStL(int decal); // Code complet

  int 			  getCol();

  string*			getCpl();

  void			  ouvreFichier(char *fichier);
  void			  fermeFichier();
  void 			  metPhrase(string decDeb = "", string decFin = "", int sautLigne = 0);
  bool 			  metUnCar(char cCar);
  void     		vidangeDkdPhrases();

  void 			  etDuMilieu(string *type, string *type1, string *type2, string entre);
  void 			  etFinal(string *type, string *type1, string fin);
  void 			  ajoutSeparateur(string *phrase, unsigned int niveau);

  void			  donneDimension(int colonne, gereNum* pNum);
  void			  donneDate(int colonne, gereDate* pDate);
  void			  donneHeure(int colonne, gereHeure* pHeure);
  void			  donneCode(int colonne, gereCode* pCode);

  void    		getGereDate(gereDate** ppDate);

#ifndef _ENTERPRISE_DLL
  DBIResult		getMateriel(NSMaterielInfo* pMatInfo);
  DBIResult		getCIM10(NSCim10Info* pMatInfo, NSCim10* pMat = 0);
  DBIResult		getCcam(NSCcamInfo* pCcamInfo, NSCcam* pCcam = 0);
#endif

  bool        getCorresp(NSPersonInfo* pPersonInfo);
  // DBIResult		getCorresp(NSCorrespondantInfo* pCorInfo);

  //void			  donneCarLibre(string* carLibre);
  void 			  donneCarLibre(int colonne, string* carLibre, GENRE* pGenre = 0);

  void			  initialiseIterateurs();

  void 			  ajLL();

  void 			  Recupere(void);

  bool   			CommenceParVoyelle(string* pLib);
  int  				donneCertitude(string sCert);
  //
  string			intenseAdjectif(AdjIntens* pAdj1, AdjIntens* pAdj2);

  //
  bool			  isTexteLibre();
  bool			  isChampLibre();
  bool			  isDimension();
  bool			  isDate();
  bool			  isHeure();
  bool			  isCode();
  bool			  isComplementAvecUnite(char cCategorie);
  bool			  isComplementSansUnite(char cCategorie);
};

#endif 		// __DIVDCODE_H
