// -----------------------------------------------------------------------------
//     NSPATPAT.H
//
//  Contient les definitions des arbres NAUTILUS
//
//  1ere version : PA juillet 94   Derniere modif : 18/07/94
// -----------------------------------------------------------------------------

#ifndef __NSPATPAT_H__
# define __NSPATPAT_H__

class Message;
class VecteurItem;
class NSPatholog;
class VectString;

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

# include "../partage/nsvector.h"
# include "../partage/ntarray.h"
# include "nsbb/nsexport.h"
# include "nsbb/nsbb_glo.h"
// # include "nsbb\nsbbtran.h"
# include "nssavoir/nspatbas.h"
# include "nssavoir/nsvarray.h"
// # include "nssavoir\ns_fic.h"
# include "nssavoir/nsRootLink.h"
// # include "nsdn\nsdocum.h"

# define	ORIGINE_PATH_PATHO	0

// -----------------------------------------------------------------------------
// La taille des champs du fichier PATPATHO
// est dans nssavoir/nspatbas.h

# define PPT_TRAN_NEW_LEN     5
# define PPT_TRAN_DEL_LEN     5


// -----------------------------------------------------------------------------
// Indice des champs du fichier PATPATHO

# define PPT_PAT_FIELD  	  1
# define PPT_DOCU_FIELD  	  2
# define PPT_LOC_FIELD        3
# define PPT_TYPE_FIELD   	  4
# define PPT_LEXIQUE_FIELD    5
# define PPT_COMPLEMENT_FIELD 6
# define PPT_CERTITUDE_FIELD  7
# define PPT_INTERET_FIELD    8
# define PPT_PLURIEL_FIELD    9
# define PPT_VISIBLE_FIELD   10
# define PPT_TRAN_NEW_FIELD  11
# define PPT_TRAN_DEL_FIELD  12
# define PPT_UNIT_FIELD      13

enum PPTTYPE {pptIndef, pptLex, pptNum, pptLibre};

#ifndef __linux__
void _NSBBCLASSE ComposeNoeudsChemin(string sCheminBrut, string& sCheminPatho);
#else
void ComposeNoeudsChemin(string sCheminBrut, string& sCheminPatho);
#endif

// -----------------------------------------------------------------------------
//  Classe NSPatPatho
// -----------------------------------------------------------------------------

#ifndef __linux__
class _NSBBCLASSE NSPatPathoData : public NSPathoBaseData
#else
class NSPatPathoData : public NSPathoBaseData
#endif
{
 private:

 	static long lObjectCount;

  string _sTexteLibre;
  string _sArchetype;
  string _sNodeRight; // Right for Node and document

  string _sTreeId;
  string _sNode;

  string _sPatientId;
  string _sDocumentId;

 protected:

  void parseTreeId();
  void buildTreeId();

 public:
  // Variables de stockage des valeurs specifiques
  // char    codePatient[PAT_NSS_LEN + 1];
  // char    codeDocument[DOC_CODE_DOCUM_LEN + 1];

  // char    treeID[PIDS_NSS_LEN + DOC_CODE_DOCUM_LEN + 1];
  // char    noeud[PPD_NOEUD_LEN + 1];

  NSPatPathoData();
  virtual ~NSPatPathoData();

  NSPatPathoData(const NSPatPathoData& rv);

  NSPatPathoData& operator=(const NSPatPathoData& src);
  int             operator==(const NSPatPathoData& o);

  void            metAZero();
  bool            sameContent(NSPatPathoData* pPathoData);
  bool            estEgal(NSPatPathoData* pPathoData);
  bool            estMemeNode(NSPatPathoData* pPathoData);

  void            getOtherNodeContent(NSPatPathoData* pPathoData);

  int	 	          getType();
  void 		        setType(int newType);

  string          getTexteLibre() const 		{ return _sTexteLibre; }
  void            setTexteLibre(string sTL)	{ _sTexteLibre = sTL; }
  string          getArchetype() const      { return _sArchetype; }
  void            setArchetype(string sArc) { _sArchetype = sArc; }

  // string          getDoc()    { return string(codePatient) + string(codeDocument); }
  // string 	        getNode()   { return string(codePatient) + string(codeDocument) + string(noeud); }
  string          getDoc()    const { return _sTreeId; }
  string 	        getNode()   const { return _sTreeId + _sNode; }
  string 	        getNodeID() const { return _sNode; }

  // Fonctions utiles lorsque l'object est un noeud rattache à une personne
  //
  string          getPerson() const { return string(_sTreeId, 0, PIDS_NSS_LEN); }
  string          getDocum()  const;
  void            setPerson(string sPat);
  void            setDocum(string sDocum);
  void            setTreeID(string sTreeID);
  void            setNodeID(string sNodeID);

  //Fonction de gestion des droits
  inline string   getNodeRight() const				{ return _sNodeRight; }
  void            setNodeRight(string right)	{ _sNodeRight = right; }

  static long     getNbInstance()     				{ return lObjectCount; }
  static void     initNbInstance() 						{ lObjectCount = 0; }
};

inline string
NSPatPathoData::getDocum() const
{
  if (strlen(_sTreeId.c_str()) == PIDS_NSS_LEN + DOC_CODE_DOCUM_LEN)
    return string(_sTreeId, PIDS_NSS_LEN, DOC_CODE_DOCUM_LEN);
  else
    return string("");
}

class NSPatPathoInfo;

// classe de liaison entre le noeud en cours et un autre noeud
#ifndef __linux__
class _NSBBCLASSE NSLinkedNode
#else
class NSLinkedNode
#endif
{
 public:
  // enum NODELINKDIRECTION { dirFleche = 1, dirEnvers }; -> now in nsbb_glo.h

  string          sOtherNode;
  NSPatPathoInfo* pOtherTemporaryNode;

  NSRootLink::NODELINKTYPES iLinkType;
  NODELINKDIRECTION         iLinkDirection;
  bool                      bLinkWithDoc;

  NSLinkedNode(string sNode, NSRootLink::NODELINKTYPES iLkType, NODELINKDIRECTION iLkDirection, bool bLkWithDoc = false);
  NSLinkedNode(NSPatPathoInfo* pOtherTemporaryNode, NSRootLink::NODELINKTYPES iLkType, NODELINKDIRECTION iLkDirection, bool bLkWithDoc = false);
  ~NSLinkedNode();
  NSLinkedNode(NSLinkedNode& rv);
  NSLinkedNode& operator=(const NSLinkedNode& src);
};

//array de liaison

typedef vector<NSLinkedNode*>					      NSLinkedNodeVector;
typedef NSLinkedNodeVector::iterator	      NSLinkedNodeIter;
typedef NSLinkedNodeVector::const_iterator	ConstNSLinkedNodeIter;

#ifndef __linux__
class _NSBBCLASSE NSLinkedNodeArray : public NSLinkedNodeVector
#else
class NSLinkedNodeArray : public NSLinkedNodeVector
#endif
{
 public:
  // Constructeurs
  NSLinkedNodeArray() : NSLinkedNodeVector() {}
  NSLinkedNodeArray(NSLinkedNodeArray& rv);
  NSLinkedNodeArray& operator=(const NSLinkedNodeArray& src);

  // Destructeur
  virtual ~NSLinkedNodeArray();
  void vider();
};

class NSLinkManager;

#ifndef __linux__
class _NSBBCLASSE NSPatPathoInfo
#else
class NSPatPathoInfo
#endif
{
 private:
 	static long lObjectCount;

 protected:

  // Objet qui contient les donnees
  NSPatPathoData _Donnees;

 public:

  // Identificateur unique
#ifndef __linux__
  LPARAM			 _ID;
#else
  unsigned int _ID;
#endif

  // Liens entre ce noeud (pas encore enregistre) et d'autres noeuds
  NSLinkedNodeArray* _pTemporaryLinks;

  NSPatPathoInfo();
#ifndef __linux__
  NSPatPathoInfo(NSSavFiche* pSavFiche);
#endif
  NSPatPathoInfo(const NSPatPathoInfo& rv);

  ~NSPatPathoInfo();

  void initFromMessage(Message *pMsg);

  NSPatPathoData* getDataTank()           { return &_Donnees; }

  static long getNbInstance()							{ return lObjectCount; }
  static void initNbInstance() 						{ lObjectCount = 0; }

  string		    	getDoc()    const    		{ return _Donnees.getDoc(); }
  string 		    	getNode()   const    		{ return _Donnees.getNode(); }
  string 	        getNodeID() const     	{ return _Donnees.getNodeID(); }

  string          getNodeLabel();

  string          getLexique()      const { return _Donnees.getLexique(); }
  string          getLexiqueSens(NSSuper* pSuper);
  string          getComplement()   const { return _Donnees.getComplement(); }
  string          getUnit()         const { return _Donnees.getUnit(); }
  string          getUnitSens(NSSuper* pSuper) const;
  string          getCertitude()    const { return _Donnees.getCertitude(); }
  string          getCertitudeSens(NSSuper* pSuper) const;
  string          getInteret()      const { return _Donnees.getInteret(); }
  string          getInteretSens(NSSuper* pSuper) const;
  string          getPluriel()      const { return _Donnees.getPluriel(); }
  string          getPlurielSens(NSSuper* pSuper) const;
  string          getVisible()      const { return _Donnees.getVisible(); }
  string          getType()         const { return _Donnees.getTypeString(); }
  string          getLocalisation() const { return _Donnees.getLocalisation(); }

  int  	        	getLigne()      	const { return _Donnees.getLigne(); }
  int  		    		getColonne()    	const { return _Donnees.getColonne(); }

  void 	        	setLigne(int valeur)    { _Donnees.setLigne(valeur); }
  void 	        	setColonne(int valeur)  { _Donnees.setColonne(valeur); }

  void            setLexique(string sLexique)      { _Donnees.setLexique(sLexique); }
  void            setComplement(string sCplmnt)    { _Donnees.setComplement(sCplmnt); }
  void            setUnit(string sUnit)            { _Donnees.setUnit(sUnit); }
  void            setCertitude(string sCertitude)  { _Donnees.setCertitude(sCertitude); }
  void            setInteret(string sInteret)      { _Donnees.setInteret(sInteret); }
  void            setPluriel(string sPluriel)      { _Donnees.setPluriel(sPluriel); }
  void            setVisible(string sVisible)      { _Donnees.setVisible(sVisible); }
  void            setType(string sType)            { _Donnees.setTypeString(sType); }
  void            setLocalisation(string sLocalisation) { _Donnees.setLocalisation(sLocalisation); }

  string          getTexteLibre() 		  const { return _Donnees.getTexteLibre(); }
  void            setTexteLibre(string sTL)		{ _Donnees.setTexteLibre(sTL); }
  string          getArchetype()        const { return _Donnees.getArchetype(); }
  void            setArchetype(string sArc)   { _Donnees.setArchetype(sArc); }

  // Management of Node Right
  inline string		getNodeRight()       const { return _Donnees.getNodeRight(); }
  inline void     setNodeRight(string right) { _Donnees.setNodeRight(right); }

  bool            sameContent(NSPatPathoInfo* pPathoInfo);
  bool 	        	estEgal(NSPatPathoInfo* pPathoInfo);
  bool            estMemeNode(NSPatPathoInfo* pPathoInfo);

  void            getOtherNodeContent(NSPatPathoInfo* pPathoInfo);

  void            addTemporaryLink(string sNode, NSRootLink::NODELINKTYPES iLkType, NODELINKDIRECTION iLkDirection, bool bLkWithDoc = false);
  void            addTemporaryLink(NSPatPathoInfo* pOtherTemporaryNode, NSRootLink::NODELINKTYPES iLkType, NODELINKDIRECTION iLkDirection, bool bLkWithDoc = false);
  bool            numberTemporaryNodes(NSLinkManager* pLinkManager = 0);

  NSPatPathoInfo& operator=(const NSPatPathoInfo& src);
  int		        	operator==(const NSPatPathoInfo& o);

  // Fonctions utiles lorsque l'object est un noeud rattache à une personne
  string          getPerson() const           { return _Donnees.getPerson(); }
  string          getDocum()  const           { return _Donnees.getDocum(); }
  void            setPerson(string sPat)      { _Donnees.setPerson(sPat); }
  void            setDocum(string sDocum)     { _Donnees.setDocum(sDocum); }
  void            setTreeID(string sTreeID)   { _Donnees.setTreeID(sTreeID); }
  void            setNodeID(string sNodeID)   { _Donnees.setNodeID(sNodeID); }

  NSLinkedNodeArray* getTemporaryLinks()      { return _pTemporaryLinks; }
};

#ifndef __linux__
// -----------------------------------------------------------------------------
// fonction globale
// -----------------------------------------------------------------------------
bool
infLoc(NSPatPathoInfo* s, NSPatPathoInfo* b)
{
  if ((NULL == s) || (NULL == b))
    return false;

  return (s->getLigne() < b->getLigne());

/*

	// ATTENTION : ne pas utiliser
	// return (strcmp(s->pDonnees->codeLocalisation, b->pDonnees->codeLocalisation) < 0);
  // qui ne distingue pas majuscules et minuscules

  string sCodeLocForS = s->getLocalisation();
  string sCodeLocForB = b->getLocalisation();

  for (int i = 0; i < BASE_LOCALISATION_LEN; i++)
  {
    if (sCodeLocForS[i] < sCodeLocForB[i])
    	return true;
    else if (sCodeLocForS[i] > sCodeLocForB[i])
    	return false;
  }
  return false;

*/
}
#endif

class NSSmallBrother;

// -----------------------------------------------------------------------------
// Définition de NSPatPathoArray (Vecteur de NSPatPathoInfo)
typedef vector<NSPatPathoInfo*>	NSPatPathoInfoArray;
typedef NSPatPathoInfoArray::iterator       PatPathoIter;
typedef NSPatPathoInfoArray::const_iterator PatPathoConstIter;

class NSVectPatPathoArray;

#ifndef __linux__
class _NSBBCLASSE NSPatPathoArray : public NSPatPathoInfoArray, public NSSuperRoot
#else
class NSPatPathoArray : public NSPatPathoInfoArray, public NSSuperRoot
#endif
{
  private:

    static long lObjectCount;

  public:

    int iArrayType;

    // Constructeurs
    NSPatPathoArray(NSSuper* pSuper, int iType = graphPerson);
    NSPatPathoArray(const NSPatPathoArray& rv);

    // Destructeur
    virtual ~NSPatPathoArray();

    void							vider();
    void							clearAllIDs();

    // Operateurs
    NSPatPathoArray&	operator=(const NSPatPathoArray& src);
    int		            operator==(NSPatPathoArray& o);

    static long getNbInstance()  { return lObjectCount; }
    static void initNbInstance() { lObjectCount = 0; }

    // Fonctions de gestion du vecteur
    bool	        	ajouteElement(NSPatPathoInfo* pPatPatho);
    bool	        	detruitElement(NSPatPathoInfo* pPatPatho);
    int			    		compteElement(string* pLocalisation);
#ifndef __linux__
    NSPatPathoInfo* litElement(LPARAM elementID);
#else
    NSPatPathoInfo* litElement(unsigned int elementID);
#endif
    NSPatPathoInfo* litElement(string* pLocalisation);
    NSPatPathoInfo* litElementPrec(NSPatPathoInfo* pPatPatho, string* pLocalisation);
    NSPatPathoInfo* litElementSuiv(NSPatPathoInfo* pPatPatho, string* pLocalisation);

    // Ajout d'un element dans une patpatho
    void	        ajouteVecteur(NSPatPathoArray* pPathParho, int decalage);
    void 	        ajoutePatho(string sEtiquette, Message* pMessage, int colonne);
    void 		    	ajoutePatho(PatPathoConstIter iterAvant, string sEtiquette, Message* pMessage, int colonne, int decalageLigne = 1, bool bPushBlock = false);
    void 	        ajoutePatho(PatPathoConstIter iterAvant, string sEtiquette, int colonne, int decalageLigne);
    void 	        ajoutePatho(string sEtiquette, int colonne);
    void 	        ajoutePatho(PatPathoConstIter iter, int ligne, int colonne);

    void          parseBlock(string sNodeBlock, Message* pMessage);
    void 	        ajoutePathoBlock(string sNodeBlock, int colonne, PatPathoIter iterAvant = (PatPathoIter) 0);

    void 	        ajouteTitre(string sTitre, string sNoeud); //ajoute titre su CR

    // Ajout et suppression d'un chemin dans une patpatho
    bool 		    	CheminDansPatpatho(string sChaine, string separateur = string(1, cheminSeparationMARK), PatPathoIter* pIter = (PatPathoIter*) 0, PatPathoConstIter* startIter = (PatPathoConstIter*) 0);
    bool 		    	CheminDansPatpatho(NSSmallBrother *pBigBoss, string sChaine, string* pValeur, string* pUnite = 0, string separateur = string(1, cheminSeparationMARK)) const;
    bool 		    	CheminSemantiqueDansPatpatho(PatPathoIter* pIter, VecteurItem* pVecEquivalents, VectString* pInterdits);
    void 	        AjouterChemin(string sChaine, Message* pMessage, bool bEcraser = true, string separateur = string(1, cheminSeparationMARK));
    void 		    	AjouterChemin(string sChaine, string sValeur, bool bEcraser = true, string separateur = string(1, cheminSeparationMARK));
    void 		    	SupprimerChemin(string sChaine, string sValeur, string separateur = string(1, cheminSeparationMARK));
    void 		    	SupprimerChemin(string sChaine, string separateur = string(1, cheminSeparationMARK));
    PatPathoIter	ChaineDansPatpatho(string sChaine, string* pLocalisation, string separateur = string(1, cheminSeparationMARK), PatPathoIter* iterFrom = (PatPathoIter*) 0);
    void 	        SousPatpatho(NSPatPathoArray* pPatNew , string sChaine, string sValeur, string separateur);

    string     		donneCheminItem(PatPathoConstIter iter, char cSubSeparator = cheminSeparationMARK);

    void          mergePatpatho(NSPatPathoArray* pMergingPpt, PatPathoIter* pLocalFatherIter);
    void          mergePatpathoDirected(NSPatPathoArray* pMergingPpt, PatPathoIter* pLocalFatherIter, vector<string*> *pSubPaths);

    // Egalite de 2 patpathos
    bool 	        estEgal(NSPatPathoArray* pPatho);
    bool 	        hasSameContent(NSPatPathoArray* pPatho);

    // Recherche dans une patpatho
    PatPathoIter 	    ChercherItem(string sItem, bool bPrepareNext = false, PatPathoIter iterFrom = (PatPathoIter) 0);
    PatPathoIter	    ChercherItem(int ligne, int colonne);
    PatPathoConstIter	ChercherItem(int ligne, int colonne) const;
    PatPathoIter 	    ChercherItem(string sItem, PatPathoIter iterPere);
    PatPathoIter	    ChercherItem(PatPathoConstIter iterFrere);
    PatPathoIter	    ChercherItem(string sChemin, string separateur);
    bool 		    	    InteretFils(PatPathoIter iterPere, string sImportance);

    PatPathoIter	    ChercherPere(PatPathoConstIter iter);
    PatPathoIter	    ChercherFrere(PatPathoConstIter iter);
    PatPathoIter	    ChercherPremierFils(PatPathoConstIter iter);

    PatPathoIter	    ChercherNoeud(string sNodeId);

    // Suppression dans une patpatho
    void 		    	SupprimerItem(PatPathoIter Supprimer);
    void 		    	SupprimerFils(PatPathoIter iPptPere);
    void	        SupprimerPere(int ligneSupprimer, int colonneSupprimer);

    void 		    	FormeLocalisation(PatPathoIter PereSupprimer, string* pLocalisation, int lignefils, int colonnefils, bool bCodeSens = true, string separateur = "/");

    // Extract the sub-patpatho of element, without this element
    void       		ExtrairePatPatho(PatPathoConstIter iterPere, NSPatPathoArray* pPatpathoPere) const;
    void 	        ExtrairePatPatho(int ligneElement, int colonneElement, NSPatPathoArray* pPatpathoPere) const;

    // Extract the sub-patpatho of element, without this element as root
    void       		ExtrairePatPathoWithRoot(PatPathoConstIter iterPere, NSPatPathoArray* pSubPatpatho) const;
    void 	        ExtrairePatPathoWithRoot(int ligneElement, int colonneElement, NSPatPathoArray* pSubPatpatho) const;

    void		    	ExtrairePatPathoFreres(PatPathoIter iterRef, NSPatPathoArray* pPatpathoDest) const;
    void        	ExtraireVecteurPatPathoFreres(PatPathoIter iterRef, NSVectPatPathoArray* pVect) const;

    void	    		donnePatpatho(NSPatPathoArray* pPatPatho, string sImportance);
    void		  		InserePatPatho(PatPathoIter iterAvant, NSPatPathoArray* pPatpathoSource, int decalageColonne = 1, bool bWithTreeID = false);
    void        	InserePatPathoFille(PatPathoIter iterPere, NSPatPathoArray* pPatpathoSource);

    // Semantic functions
    string        GetDocDate();

    // autres
    int		        Taille();   // taille d'une patpatho en comptant les elements d'une meme ligne comme seul element

    // Fonctions de representation sous forme d'une chaine
    void	        genereChaine(NSContexte *pContexte, string* pChaine);
    void	        initialiseDepuisChaine(NSContexte *pContexte, string* pChaine, string sPersonID);
    void	        importNodeForMaterial(NSPatPathoInfo* pInfo, string sCplt);
    void	        importNodeForCorresp(NSContexte *pContexte, NSPatPathoInfo* pInfo, string sCplt);
    void          getCodeAndLabel(string sCplt, string sCode, string &sCodeTxt, string &sLabel);

    // Fonctions de representation sous forme XML
    string       	genereXML();
    bool         	initialiseDepuisXML(string* pXMLstring);
    bool          addElementFromXMLnode(string * pXMLnodeString, int iCol);

    string 				getNodeDate(PatPathoConstIter iNode) const;      	 // return the reference date of a node
    string 				getNodeSpecificDate(PatPathoConstIter iNode, string sDateTypeSens) const;
    string 				getNodeOpenDate(PatPathoConstIter iNode) const  { return getNodeSpecificDate(iNode, string("KOUVR")); }
    string 				getNodeCloseDate(PatPathoConstIter iNode) const { return getNodeSpecificDate(iNode, string("KFERM")); }
    string 				getNodeChirDate(PatPathoConstIter iNode) const  { return getNodeSpecificDate(iNode, string("KCHIR")); }

    string        getDirectArchetypeId(PatPathoConstIter iNode);

    string  			isPatientData(PatPathoIter iNode);

    //
    bool          isExpressionTrue(string sExpression, bool *pbExpressionIsValid);
    bool          isNumericExpressionTrue(string sOperator, string sValue, string sUnit, bool *pbExpressionIsValid);
    bool          isNonNumericExpressionTrue(string sOperator, string sOperand, bool *pbExpressionIsValid);
    PatPathoIter  getFirstNumericalValue(char cValueType = ' ', PatPathoIter iterFrom = (PatPathoIter) 0, int iBaseCol = 0);
    //bool          getNumericalValueForUnit(string sUnit, double *pdValue, string sConcept);
};

// VECTEUR DE PATPATHO
typedef vector<NSPatPathoArray*>					      NSVectPatPathoInfoArray;
typedef NSVectPatPathoInfoArray::iterator	      PatPathoIterVect;
typedef NSVectPatPathoInfoArray::const_iterator	ConstPatPathoIterVect;

#ifndef __linux__
class _NSBBCLASSE NSVectPatPathoArray : public NSVectPatPathoInfoArray
#else
class NSVectPatPathoArray : public NSVectPatPathoInfoArray
#endif
{
 private:
 	static long lObjectCount;

 public:
  // Constructeurs
  NSVectPatPathoArray();
  NSVectPatPathoArray(const NSVectPatPathoArray& rv);

  // Destructeur
  virtual ~NSVectPatPathoArray();
  void vider();
  bool estVide();
  bool MultiestVide();

  // Operateurs
  NSVectPatPathoArray& operator=(const NSVectPatPathoArray& src);

  static long getNbInstance() { return lObjectCount; }
  static void initNbInstance() { lObjectCount = 0; }
};

#ifndef __linux__
class _NSBBCLASSE NSFatheredPatPathoArray : public NSSuperRoot
#else
class NSFatheredPatPathoArray : public NSSuperRoot
#endif
{
 private:

 	static long lObjectCount;

 public:

  // Constructeurs
  NSFatheredPatPathoArray(NSSuper* pSuper, NSPatPathoInfo* pFatherNode, NSPatPathoArray* pPatPatho);
  NSFatheredPatPathoArray(const NSFatheredPatPathoArray& rv);

  // Destructeur
  ~NSFatheredPatPathoArray();

  // Operateurs
  NSFatheredPatPathoArray& operator=(const NSFatheredPatPathoArray& src);

  NSPatPathoArray* getPatPatho()   { return _pPatPatho;   }
  NSPatPathoInfo*  getFatherNode() { return _pFatherNode; }

  void             referenceFatherNode(NSPatPathoInfo* pModel);

  static long getNbInstance()  { return lObjectCount; }
  static void initNbInstance() { lObjectCount = 0; }

 protected:

  NSPatPathoArray* _pPatPatho;
  NSPatPathoInfo*  _pFatherNode;
};

// VECTEUR DE PATPATHO
typedef vector<NSFatheredPatPathoArray*>	NSVectFatheredPatPathoInfoArray;
typedef NSVectFatheredPatPathoInfoArray::iterator	      FatheredPatPathoIterVect;
typedef NSVectFatheredPatPathoInfoArray::const_iterator	ConstFatheredPatPathoIterVect;

class NSVectFatheredPatPathoArray : public NSVectFatheredPatPathoInfoArray
{
 private:
 	static long lObjectCount;

 public:
  // Constructeurs
  NSVectFatheredPatPathoArray();
  NSVectFatheredPatPathoArray(const NSVectFatheredPatPathoArray& rv);

  // Destructeur
  virtual ~NSVectFatheredPatPathoArray();
  void vider();
  bool estVide();
  bool MultiEstVide();

  // Operateurs
  NSVectFatheredPatPathoArray& operator=(const NSVectFatheredPatPathoArray& src);

  static long getNbInstance() { return lObjectCount; }
  static void initNbInstance() { lObjectCount = 0; }
};

// Classes utilisées pour le copier/coller NAUTILUS
class NSCutPastTL
{
 public:
  string sIndice;
  string sTexte;
};

typedef vector<NSCutPastTL*>					NSCutPastTLIBArray;
typedef NSCutPastTLIBArray::iterator	CutPastTLIter;

/*
class _NSBBCLASSE NSCutPastTLArray : public NSCutPastTLIBArray
{
 public:
  // Constructeurs
  NSCutPastTLArray();
  NSCutPastTLArray(NSCutPastTLArray& rv);

  // Destructeur
  ~NSCutPastTLArray();
  void vider();

  // Operateurs
  NSCutPastTLArray& NSCutPastTLArray::operator=(NSCutPastTLArray src);
};
*/

typedef NTArray<NSCutPastTL> NSCutPastTLArray;

class NSCutPaste : public NSSuperRoot
{
  protected:

    bool _bUnique;

  public:

    NSPatPathoArray  _PatPatho;
    NSCutPastTLArray _TextesLibres;
    VecteurString    _aConcerns;
    bool             _bConsiderAsUnique;  // If true, mean don't duplicate

    NSCutPaste(NSSuper* pSuper);
    NSCutPaste(const NSCutPaste& rv);
    ~NSCutPaste();

    bool empty() { return _PatPatho.empty(); }
    void vider();

    bool isUnique()         { return _bConsiderAsUnique; }
    void setUnique(bool bU) { _bConsiderAsUnique = bU; }

    // NSPatPathoArray*  getPatho()    { return &_PatPatho; }
    void              setPatPatho(const NSPatPathoArray* pPpt);
    void              initFromPatPatho(NSPatPathoArray* pPpt) const;
    NSCutPastTLArray* getTL()       { return &_TextesLibres; }
    VecteurString*    getConcerns() { return &_aConcerns; }

    NSCutPaste& operator=(const NSCutPaste& src);
};

#endif // !__NSPATPAT_H__
