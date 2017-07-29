//---------------------------------------------------------------------------
//
//  		Implementation des methodes de la recherche semantique (IA)
//   									Kaddachi Hafedh
//
//---------------------------------------------------------------------------

#include <algorithm>
// #include <deque.h>
// #include <owl\statusba.h>

#define	SousChemin        string("~~***") //peut remplacer n'importe quel chemin
                                          //, peut être vide
#define	SousCheminNonVide string("~~**1") //peut remplacer n'importe quel chemin
                                          //, ne peut pas être vide

#define	ItemGenerique     string("~????") // peut remplacer n'importe quel item
#define	ItemPsGenerique   string("~???~") // peut remplacer n'importe quel item
                                          // d'une catégorie donnee

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/personLus.h"
  #include "nsdatabaseLus/nsSQL.h"
  #include "mysql/mysql.h"
#else
  #include "partage/nsglobal.h"
  #include "partage/nsdivfct.h"
  #include "nautilus/nssuper.h"
#endif

#include "nssavoir/nsguide.h"
#include "nssavoir/nsfilgd.h"
#include "nssavoir/nsfilgdEngine.h"
#include "dcodeur/nsgenlan.h"

//class _CLASSELEXI NSPatholog ;
//***************************************************************************
// 							Implémentation des mèthodes NSDico
//***************************************************************************

//---------------------------------------------------------------------------
//   Constructeur
//---------------------------------------------------------------------------
NSDico::NSDico(NSContexte* pCtx)
       :NSRoot(pCtx)
{
  _pDicoDialog = (ChoixPathoDialog*) 0 ;
  //
  // Si pPatho est initialisé à 0, DicoDialog doit ouvrir puis fermer
  // le fichier. Si pPatho est initialisé, il reste ouvert jusqu'à la
  // fin du programme.
  //
  if (pContexte && pContexte->getUtilisateur())
    _sUserLang = pContexte->getUtilisateur()->donneLang() ;
  else
    _sUserLang = string("") ;

  _pPhraseur    = new NSPhraseur(pContexte) ;
  _pProposition = new NsProposition(pContexte, &_pPhraseur, NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
  _pGenerateur  = (NSGenerateur*) 0 ;

  _pPatho       = (NSPatholog*) 0 ;
  _pLexiMed     = (NSLexiMed*)  0 ;

  _sLangBis     = "none" ;
  _pPathoBis    = (NSPatholog*) 0 ;
  _pLexiMedBis  = (NSLexiMed*)  0 ;

  _bOuvreFerme  = true ;
}

//---------------------------------------------------------------------------
//   Destructeur
//---------------------------------------------------------------------------
NSDico::~NSDico()
{
  if (_pGenerateur)
    delete _pGenerateur ;
  if (_pPhraseur)
    delete _pPhraseur ;
  if (_pProposition)
    delete _pProposition ;
  if (_pPatho)
    delete _pPatho ;
  if (_pLexiMed)
    delete _pLexiMed ;
  if (_pPathoBis)
    delete _pPathoBis ;
  if (_pLexiMedBis)
    delete _pLexiMedBis ;
}

//---------------------------------------------------------------------------
//   Appropriation du dictionnaire par un objet
//
//	  Si le dictionnaire est en mode Ouvre/Ferme, il faut ouvrir le fichier
//---------------------------------------------------------------------------
bool
NSDico::Prend()
{
#ifndef _ENTERPRISE_DLL
	// Lexique général - All purposes lexicon
	//
	if ((_bOuvreFerme) && (NULL == _pPatho))
	{
		_pPatho = new NSPatholog(pContexte->getSuperviseur(), _sUserLang) ;
		//
		// Ouverture du fichier
		//
		_pPatho->lastError = _pPatho->open() ;
		if (DBIERR_NONE != _pPatho->lastError)
		{
    	string sErrorText = pContexte->getSuperviseur()->getText("lexiqueErrors", "errorOpeningLexique") ;
      sErrorText += string(" (") + _sUserLang + string(")") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
			erreur(sErrorText.c_str(), standardError, _pPatho->lastError) ;

			delete _pPatho ;
      _pPatho = (NSPatholog*) 0 ;

			return false ;
		}
	}

  // Médicaments - Drugs lexicon
  //
  if ((_bOuvreFerme) && (NULL == _pLexiMed))
  {
  	_pLexiMed = new NSLexiMed(pContexte->getSuperviseur(), _sUserLang) ;
		//
		// Ouverture du fichier
		//
		_pLexiMed->lastError = _pLexiMed->open() ;
		if (DBIERR_NONE != _pLexiMed->lastError)
		{
			string sErrorText = pContexte->getSuperviseur()->getText("lexiqueErrors", "errorOpeningDrugsLexique") ;
      sErrorText += string(" (") + _sUserLang + string(")") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
			erreur(sErrorText.c_str(), standardError, _pPatho->lastError) ;

			delete _pLexiMed ;
      _pLexiMed = (NSLexiMed*) 0 ;

			return false ;
    }
	}
#else

  _pPatho   = new NSPatholog(pContexte->getSuperviseur(), _sUserLang) ;
  _pLexiMed = new NSLexiMed(pContexte->getSuperviseur(), _sUserLang) ;

#endif

	// Générateur - Langage generator
	//
	if ("en" == _sUserLang)
		_pGenerateur = new NSGenerateurEn(pContexte, _pProposition, string("en")) ;
  else
  	_pGenerateur = new NSGenerateurFr(pContexte, _pProposition, string("fr")) ;

	return true ;
}

//---------------------------------------------------------------------------
//   Relâche du dictionnaire par un objet qui se l'était approprié
//
//	  Si le dictionnaire est en mode Ouvre/Ferme, il faut fermer le fichier
//---------------------------------------------------------------------------
void
NSDico::Relache()
{}

bool
NSDico::UserChanged()
{
  if (NULL == pContexte->getUtilisateur())
    return true ;

  string sNewLang = pContexte->getUtilisateur()->donneLang() ;

  if (sNewLang == _sUserLang)
    return true;

  // Changement de code langue - Langage code changed
  //
  _sUserLang = sNewLang ;

  // Fermeture des lexiques - Lexicons are closed
  //
  if (_pPatho)
  {
    delete _pPatho ;
    _pPatho = (NSPatholog*) 0 ;
  }
  if (_pLexiMed)
  {
    delete _pLexiMed ;
    _pLexiMed = (NSLexiMed*) 0 ;
  }
  //
  // Fermeture du générateur - Langage generator is deleted
  //
  if (_pGenerateur)
  {
    delete _pGenerateur ;
    _pGenerateur = (NSGenerateur*) 0 ;
  }

  //
  // Réouverture des lexiques - Lexicons are opened again
  return Prend() ;
}

//---------------------------------------------------------------------------
// Décoder  pSousPatPatho selon precision est mettre les phrases dans pVect
// retourner false si le décodage a échoué
//---------------------------------------------------------------------------
bool
NSDico::Decoder(string /* sLang */, string /* sChemin */, NSPatPathoArray* /* pSousPatPatho */,
                               ClasseStringVector* /* pVect */, int /* precision */)
{
	return true ;
}

NSPatholog*
NSDico::donnePatholog(string sLang, string* pCodeLexique)
{
	if ((NULL == pCodeLexique) || (string("") == *pCodeLexique))
		return _pPatho ;

  string sCategorie = CodeCategorie(*pCodeLexique) ;

#ifndef _ENTERPRISE_DLL

	// If required langage is user langage, no problem
	//
	if (sLang == _sUserLang)
	{
		if ("_" == sCategorie) //code lexique médicaments
    	return _pLexiMed ;
    else
    	return _pPatho ;
	}
	// If required langage is already opened, no problem either
	//
	if (sLang == _sLangBis)
	{
  	if ("_" == sCategorie) //code lexique médicaments
		{
    	if (_pLexiMedBis)
      	return _pLexiMedBis ;
      else
      	return _pLexiMed ;
    }
    else
    {
    	if (_pPathoBis)
      	return _pPathoBis ;
      else
      	return _pPatho ;
    }
	}
	// Fermeture des lexiques bis - Alternative lexicons are closed
	//
	if (_pPathoBis)
	{
		delete _pPathoBis ;
		_pPathoBis = (NSPatholog*) 0 ;
	}
	if (_pLexiMedBis)
	{
		delete _pLexiMedBis ;
		_pLexiMedBis = (NSLexiMed*) 0 ;
	}
	// Puis ouverts avec la langue voulue - Then opened with accurate langage
	//
	_sLangBis = sLang ;

	// Ouverture lexique général - Opening general lexicon
	//
	_pPathoBis = new NSPatholog(pContexte->getSuperviseur(), _sLangBis) ;
	_pPathoBis->lastError = _pPathoBis->open() ;
	if (DBIERR_NONE != _pPathoBis->lastError)
	{
  	string sErreur = string("Erreur à l'ouverture du lexique + (Langue = ")
                                + _sLangBis + string(")") ;
    erreur(sErreur.c_str(), standardError, _pPathoBis->lastError) ;

    delete _pPathoBis ;
    _pPathoBis = (NSPatholog*) 0 ;
	}

	// Ouverture lexique médicament - Opening drugs lexicon
	//
	_pLexiMedBis = new NSLexiMed(pContexte->getSuperviseur(), _sLangBis) ;
	_pLexiMedBis->lastError = _pLexiMedBis->open() ;
	if (DBIERR_NONE != _pLexiMedBis->lastError)
	{
  	string sErreur = string("Erreur à l'ouverture de la base des médicaments + (Langue = ")
                                + _sLangBis + string(")") ;
    erreur(sErreur.c_str(), standardError, _pLexiMedBis->lastError) ;

    delete _pLexiMedBis ;
    _pLexiMedBis = (NSLexiMed*) 0 ;
	}

	if (string("_") == sCategorie) //code lexique médicaments
	{
		if (_pLexiMedBis)
			return _pLexiMedBis ;
    else
			return _pLexiMed ;
	}
	else
	{
		if (_pPathoBis)
    	return _pPathoBis ;
    else
    	return _pPatho ;
	}
#else
  if (string("_") == sCategorie) //code lexique médicaments
    return _pLexiMed ;
	else
    return _pPatho ;
#endif
}

//---------------------------------------------------------------------------
//   Trouve le libellé brut (du lexique) d'un élément dont on connaît le code
//---------------------------------------------------------------------------
bool
NSDico::donneLibelleLexique(string sLang, string* pCodeLexique, string* pLibelleTrouve)
{
	if (((string *) NULL == pCodeLexique) || ((string *) NULL == pLibelleTrouve))
		return false ;

	NSPatholog* pPathoLexique = donnePatholog(sLang, pCodeLexique) ;

  *pLibelleTrouve = string("") ;

	if ((NSPatholog *) NULL == pPathoLexique)
    return false ;

#ifndef _ENTERPRISE_DLL
	if (pPathoLexique->trouveCode(pCodeLexique) != DBIERR_NONE)
#else
  if (false == pPathoLexique->trouveCode(sLang, pCodeLexique))
#endif
		return false ;

	*pLibelleTrouve = pPathoLexique->donneLibelleLexique() ;

	return true ;
}

bool
NSDico::donneLibelleChemin(string sLang, string chemin, string* psLibelle, string sep)
{
	if (!psLibelle)
		return false ;

	string sousChaine ;
  string sLibelleTrouve ;

	size_t fin   = chemin.find(sep) ;
	size_t debut = 0 ;

  string sLibelle = "";

  while (fin != string::npos)
  {
  	sousChaine = string(chemin, debut, fin - debut) ;
    donneLibelle(sLang, &sousChaine, &sLibelleTrouve) ;
    if (sLibelle != "")
    	sLibelle += sep ;
    sLibelle += sLibelleTrouve ;
    debut = fin + 1 ;
    fin   = chemin.find(sep, debut) ;
	}

	sousChaine = string(chemin, debut, strlen(chemin.c_str()) - debut) ;
  donneLibelle(sLang, &sousChaine, &sLibelleTrouve) ;
  if (sLibelle != "")
  	sLibelle += sep ;
  sLibelle += sLibelleTrouve ;

	*psLibelle = sLibelle ;

  return true ;
}

//---------------------------------------------------------------------------
//   Trouve le libellé d'un élément dont on connaît le code
//---------------------------------------------------------------------------
bool
NSDico::donneLibelle(string sLang, string* pCodeLexique, string* pLibelleTrouve, int precision, GENRE *pGenre)
{
	if ((NULL == pCodeLexique) || (NULL == pLibelleTrouve))
  	return false ;

	*pLibelleTrouve = string("") ;

  if ((*pCodeLexique == string("")) || (strlen(pCodeLexique->c_str()) > PATHO_CODE_LEN))
  	return false ;

	if (strlen(pCodeLexique->c_str()) < PATHO_CODE_LEN)
  {
  	donneCodeComplet(*pCodeLexique) ;
    if (strlen(pCodeLexique->c_str()) < PATHO_CODE_LEN)
    	return false ;
	}

	NSPatholog* pPathoLexique = donnePatholog(sLang, pCodeLexique) ;
  if ((NSPatholog *) NULL == pPathoLexique)
    return false ;

#ifndef _ENTERPRISE_DLL
	if (pPathoLexique->trouveCode(pCodeLexique) != DBIERR_NONE)
#else
  if (false == pPathoLexique->trouveCode(sLang, pCodeLexique))
#endif
		return false ;

	if (precision == brut)
		*pLibelleTrouve = *(pPathoLexique->donneLibelle()) ;
	else if(precision == choisi)
		pPathoLexique->_Donnees.donneLibelleChoix(pLibelleTrouve) ;
	else if(precision == affiche)
		pPathoLexique->_Donnees.donneLibelleAffiche(pLibelleTrouve) ;

	size_t positionAccolades = pLibelleTrouve->find("{") ;
	size_t positionVideAccolades = pLibelleTrouve->find(" {") ;
	if (positionVideAccolades != string::npos)
		*pLibelleTrouve = string(*pLibelleTrouve, 0, positionVideAccolades) ;
	else if (positionAccolades != string::npos)
		*pLibelleTrouve = string(*pLibelleTrouve, 0, positionAccolades) ;

	strip(*pLibelleTrouve, stripRight) ;

	if (NULL != pGenre)
		_pPatho->_Donnees.donneGenre(pGenre) ;

	return true ;
}

//---------------------------------------------------------------------------
//   Trouve le libellé d'un élément dont on connaît le code
//---------------------------------------------------------------------------
bool
NSDico::donneShortLibelle(string sLang, string* pCodeLexique, string* pLibelleTrouve, int precision, int *pGenre)
{
	if (!pCodeLexique || !pLibelleTrouve)
  	return false ;

	*pLibelleTrouve = string("") ;

  if ((*pCodeLexique) == string(""))
  	return false ;

	size_t iShortestLength = 1000 ;
  string sShortestLib    = string("") ;
  int    iShortestGenre ;

  string sSens = donneCodeSens(pCodeLexique) ;
	char   cEnd  = '1' ;
  while (cEnd < 'Z')
  {
  	string sTest = sSens + string(1, cEnd) ;
    string sResult ;
    GENRE  iGenreTest ;

    // Prevents error warning
    NSPatholog* pPathoLexique = donnePatholog(sLang, &sTest) ;
    if ((NSPatholog *) NULL == pPathoLexique)
      return false ;

#ifndef _ENTERPRISE_DLL
		if (pPathoLexique && (pPathoLexique->trouveCode(&sTest, keySEARCHEQ, dbiWRITELOCK, false) == DBIERR_NONE))
#else
    if (pPathoLexique && (pPathoLexique->trouveCode(sLang, &sTest, false)))
#endif
    {
    	bool bSuccess = donneLibelle(sLang, &sTest, &sResult, precision, &iGenreTest) ;

    	size_t iResultLength = strlen(sResult.c_str()) ;
    	if (bSuccess && (iResultLength < iShortestLength))
    	{
    		iShortestLength = iResultLength ;
      	sShortestLib    = sResult ;
      	iShortestGenre  = iGenreTest ;
    	}
    }

    cEnd++ ;
    if ((cEnd > '9') && (cEnd < 'A'))
    	cEnd = 'A' ;
  }

  *pLibelleTrouve = sShortestLib ;
  if (NULL != pGenre)
  	*pGenre = iShortestGenre ;

  return (sShortestLib != string("")) ;
}

//---------------------------------------------------------------------------
//   Trouve le libellé d'un élément dont on connaît le code
//---------------------------------------------------------------------------
bool
NSDico::trouvePathologData(string sLang, string* pCodeLexique, NSPathologData* pData, bool afficheErreur)
{
	if ((NULL == pCodeLexique) || (NULL == pData) || (0 == strlen(pCodeLexique->c_str())))
		return false ;

	if ('£' == (*pCodeLexique)[0])
		return true ;

	NSPatholog* pPathoLexique = donnePatholog(sLang, pCodeLexique) ;
  if ((NSPatholog *) NULL == pPathoLexique)
    return false ;

#ifndef _ENTERPRISE_DLL
	return pPathoLexique->trouvePathologData(pCodeLexique, pData, afficheErreur) ;
#else
  return pPathoLexique->trouvePathologData(sLang, pCodeLexique, pData, afficheErreur) ;
#endif
}

//---------------------------------------------------------------------------
//   Trouve le code d'un élément dont on connaît le libellé
//---------------------------------------------------------------------------
bool
NSDico::donneCode(string sLang, string* pLibelleLexique, string* pCodeTrouve)
{
	if ((NULL == pCodeTrouve) || (NULL == pLibelleLexique) || (NULL == _pPatho))
		return false ;

	*pCodeTrouve = string("") ;

#ifndef _ENTERPRISE_DLL
	if (_pPatho->trouveLibelle(pLibelleLexique) != DBIERR_NONE)
#else
  if (false == _pPatho->trouveLibelle(sLang, pLibelleLexique))
#endif
  	return false ;

#ifndef _ENTERPRISE_DLL
	*pCodeTrouve = string(_pPatho->donneCode()) ;
#else
  *pCodeTrouve = _pPatho->getCode() ;
#endif
  return true ;
}

//---------------------------------------------------------------------------
//   Donne la catégorie
//---------------------------------------------------------------------------
string
NSDico::CodeCategorie(string sChaine)
{
	if (string("") != sChaine)
  	return string(sChaine, 0, 1) ;
  else
  	return string("") ;
}

bool
NSDico::isDrug(string* pCodeLexiqueOrSens)
{
  if (NULL == pCodeLexiqueOrSens)
    return false ;

  string sCategoryElement = CodeCategorie(*pCodeLexiqueOrSens) ;
  if ((string("I") == sCategoryElement) || (string("_") == sCategoryElement))
    return true ;

  return false ;
}

bool
NSDico::isDrugOrTreatment(string* pCodeLexiqueOrSens)
{
  if (NULL == pCodeLexiqueOrSens)
    return false ;

  if (isDrug(pCodeLexiqueOrSens))
    return true ;

  string sCategoryElement = CodeCategorie(*pCodeLexiqueOrSens) ;
  if ((string("N") == sCategoryElement) ||
      (string("O") == sCategoryElement) ||
      (string("G") == sCategoryElement))
    return true ;

  return false ;
}

//---------------------------------------------------------------------------
//   Donne le code Sens en fonction du code lexique
//---------------------------------------------------------------------------
void
NSDico::donneCodeSens(string& sCode)
{
	string sCodeCat = CodeCategorie(sCode) ;
  size_t iSize    = strlen(sCode.c_str()) ;

  if ((string("£") == sCodeCat) && (iSize > BASE_FORMAT_SENS_LEN))
  {
  	if (string("£SG") != string(sCode, 0, 3))
    {
  		sCode = string(sCode, 0, BASE_FORMAT_SENS_LEN) ;
      return ;
    }
  }

	if ((string("$") != sCodeCat) && (iSize >= BASE_LEXI_LEN))
  {
  	sCode = string(sCode, 0, BASE_SENS_LEN) ;
    return ;
  }
}

//---------------------------------------------------------------------------
//   Donne le code Sens en fonction du code lexique
//---------------------------------------------------------------------------
void
NSDico::donneCodeSens(string* pCodeLexique, string* pCodeSens)
{
	if (NULL == pCodeSens)
  	return ;

  *pCodeSens = string("") ;

  if ((NULL == pCodeLexique) || (string("") == *pCodeLexique))
  	return ;

  string sBuffer = string("") ;

  // Chercher la position de cheminSeparationMARK dans la chaine
  size_t positChem = pCodeLexique->find(string(1, cheminSeparationMARK)) ;
  // Chercher la position de nodeSeparationMARK dans la chaine
  size_t positNode = pCodeLexique->find(string(1, nodeSeparationMARK)) ;
  // Chercher la position de intranodeSeparationMARK dans la chaine
  size_t positINod = pCodeLexique->find(string(1, intranodeSeparationMARK)) ;

  if ((string::npos == positNode) && (string::npos == positChem) && (string::npos == positINod))
  {
  	sBuffer = *pCodeLexique ;
    donneCodeSens(sBuffer) ;
    *pCodeSens = sBuffer ;
    return ;
	}

  size_t posit = min(positChem, positNode) ;
  posit = min(posit, positINod) ;
  size_t debut = 0 ;

  char cSepare ;

	while (((string::npos != positNode) || (string::npos != positChem) || (string::npos != positINod)) &&
                                    (posit < strlen(pCodeLexique->c_str())))
  {
  	cSepare = (*pCodeLexique)[posit] ;

    sBuffer = string(*pCodeLexique, debut, posit - debut) ;
    donneCodeSens(sBuffer) ;
    *pCodeSens = *pCodeSens + sBuffer + string(1, cSepare) ;

    debut = posit + 1 ;
    positChem = pCodeLexique->find(string(1, cheminSeparationMARK), debut) ;
    positNode = pCodeLexique->find(string(1, nodeSeparationMARK), debut) ;
    positINod = pCodeLexique->find(string(1, intranodeSeparationMARK), debut) ;

    posit = min(positChem, positNode) ;
    posit = min(posit, positINod) ;
  }

	sBuffer = string(*pCodeLexique, debut, strlen(pCodeLexique->c_str()) - debut) ;
  donneCodeSens(sBuffer) ;
  *pCodeSens = *pCodeSens + sBuffer ;
}

string
NSDico::donneCodeSens(string* pCodeLexique)
{
	string sCodeSens = string("") ;

  donneCodeSens(pCodeLexique, &sCodeSens) ;

  return sCodeSens ;
}

void
NSDico::donneCodeComplet(string& sCode)
{
	size_t iStringLen = strlen(sCode.c_str()) ;
	if ((BASE_SENS_LEN != iStringLen) && (BASE_FORMAT_SENS_LEN != iStringLen))
		return ;

	if (BASE_SENS_LEN == iStringLen)
  {
  	sCode += string("1") ;
    return ;
  }

  if (BASE_FORMAT_SENS_LEN == iStringLen)
  {
  	sCode += string("001") ;
    return ;
  }
}

//---------------------------------------------------------------------------
//chercher dans la patpatho de synthèse le complément correspondant à sItem
//exemple si sItem = VPOIDS retourner la valeur du poids
//---------------------------------------------------------------------------
string
NSDico::SetData(string sItem, string* pUnite, NSPatPathoArray* pPathoDocEnCours)
{
  if (NULL == pPathoDocEnCours)
    return string("") ;

	//chercher dans le document en cours
	string sValeur = string("") ;

  pPathoDocEnCours->CheminDansPatpatho(0, sItem, &sValeur, pUnite) ;

  return sValeur ;
}

void NSDico::separe(string* psChaine, string* psLexique, string* psCertitude, string* psPluriel)
{
  *psCertitude = "" ;
  *psPluriel   = "" ;

  size_t pos = psChaine->find("/") ;
  if (pos == NPOS)
  {
  	*psLexique = *psChaine ;
    return ;
  }
  // Le code lexique est toujours au début
  *psLexique = string(*psChaine, 0, pos);

  string sReste = string(*psChaine, pos+1, strlen(psChaine->c_str())-pos-1) ;
  pos = sReste.find("/") ;

  string sChaine1, sChaine2 ;

  if (pos == NPOS)
  {
  	if      (string(sReste, 0, 3) == "WCE")
    	*psCertitude = sReste ;
    else if (string(sReste, 0, 4) == "WPLU")
    	*psPluriel = sReste ;
  }
  else
  {
  	sChaine1 = string(sReste, 0, pos) ;
    if      (string(sChaine1, 0, 3) == "WCE")
    	*psCertitude = sChaine1 ;
    else if (string(sChaine1, 0, 4) == "WPLU")
    	*psPluriel = sChaine1 ;

    sChaine2 = string(sReste, pos+1, strlen(sReste.c_str())-pos-1) ;
    if      (string(sChaine2, 0, 3) == "WCE")
    	*psCertitude = sChaine2 ;
    else if (string(sChaine2, 0, 4) == "WPLU")
    	*psPluriel = sChaine2 ;
	}
}

//-----------------------------------------------------------------------
// fonction globale
//-----------------------------------------------------------------------
bool inferieure(string* s, string* b)
{
	return (*s < *b) ;
}

// -----------------------------------------------------------------------------
//
// Implémentation des mèthodes NSFilGuide
//
// -----------------------------------------------------------------------------

NSFilGuide::NSFilGuide(NSSuper* pSuper, string sTypeBD)
           :NSSuperRoot(pSuper)
{
  sTypeBase = sTypeBD ;
  pVecEquivalentItemDuPere = 0 ;
  pVecEquivalentItemDuPere = new VecteurItem() ;

  pBBFiche = 0 ;

#ifndef _ENTERPRISE_DLL
  pSavoir = 0 ;
#else
  pSavoir = new NSSavoir(_pSuper) ;
#endif

  bOuvreFerme = true ;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
NSFilGuide::~NSFilGuide()
{
  if (pBBFiche)
  {
    delete pBBFiche ;
    pBBFiche = 0 ;
  }

  if (pSavoir)
  {
    delete pSavoir ;
    pSavoir = 0 ;
  }

  if (pVecEquivalentItemDuPere)
  {
    delete pVecEquivalentItemDuPere ;
    pVecEquivalentItemDuPere = 0 ;
  }
}

#ifndef _ENTERPRISE_DLL
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool
NSFilGuide::Prend()
{
	if (!bOuvreFerme)
		return true ;

	// Ouverture du fichier
try
{
	if (pBBFiche == 0)
  {
    if (sTypeBase == GUIDE)
    {
      pBBFiche = new BBFiche(_pSuper) ;
      BBFiche* pBBCommeFiche = static_cast<BBFiche *>(pBBFiche) ;
      pBBCommeFiche->lastError = pBBCommeFiche->open() ;
      if (pBBCommeFiche->lastError != DBIERR_NONE)
      {
        erreur("Erreur à l'ouverture du Fil guide.", standardError, pBBCommeFiche->lastError) ;
        delete pBBFiche ;
        pBBFiche = 0 ;
        return false ;
      }
    }
    else if (sTypeBase == DECODE)
    {
      pBBFiche = new BBDecode(_pSuper) ;
      BBDecode* pBBCommeDecode = static_cast<BBDecode *>(pBBFiche) ;
      pBBCommeDecode->lastError = pBBCommeDecode->open() ;
      if (pBBCommeDecode->lastError != DBIERR_NONE)
      {
        erreur("Erreur à l'ouverture du Décodeur.", standardError, pBBCommeDecode->lastError) ;
        delete pBBFiche ;
        pBBFiche = 0 ;
        return false ;
      }
    }
	}
}
catch (...)
{
	erreur("NSFilGuide::Prend : ouverture de la table 1.", standardError, 0) ;
  return false ;
}

try
{
	if (pSavoir != 0)
		return true ;

	pSavoir = new NSSavoir(_pSuper) ;
  pSavoir->lastError = pSavoir->open() ;
  if (pSavoir->lastError != DBIERR_NONE)
  {
  	erreur("Erreur à l'ouverture du réseau sémantique : pSavoir", standardError, pSavoir->lastError) ;
    delete pSavoir ;
    pSavoir = 0 ;
    return false ;
  }
}
catch (...)
{
	erreur("NSFilGuide::Prend : ouverture de la table 2.", standardError, 0) ;
  return false ;
}
	return true ;
}
#endif

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void NSFilGuide::Relache()
{
	if (!bOuvreFerme)
		return ;

	delete pBBFiche ;
  pBBFiche = 0 ;
  delete pSavoir ;
  pSavoir = 0 ;
}


// -----------------------------------------------------------------------------
// Egalité au sens stricte
// -----------------------------------------------------------------------------
void NSFilGuide::chercheCheminEgaliteStricte(string* pCheminPere,
                                             VecteurRechercheSelonCritere* pVecteurSelonCritere)
{
#ifndef _ENTERPRISE_DLL
  bool continuer = true ;
  IterCritere iter = pVecteurSelonCritere->begin() ;
  for ( ; iter != pVecteurSelonCritere->end() ; iter++)
  {
    if (false == (*iter)->trouve)
    {
      string sCheminFils ;
      if (*pCheminPere == string(""))
        sCheminFils = (*iter)->sEtiquette ;
      else
        sCheminFils = (*pCheminPere) + string(1, cheminSeparationMARK) + (*iter)->sEtiquette ;

      pBBFiche->lastError = pBBFiche->chercheClef(&sCheminFils, "CHEMIN", NODEFAULTINDEX, keySEARCHGEQ, dbiWRITELOCK) ;
      if (pBBFiche->lastError != DBIERR_NONE)
      {
        (*iter)->trouve = false ;
        continuer = false ; //passer au fils suivant
      }

      if (true == continuer)
      {
        pBBFiche->lastError = pBBFiche->getRecord() ;
        if (pBBFiche->lastError != DBIERR_NONE)
        {
          erreur("Le fichier de script est inutilisable (guides.db).", standardError, pBBFiche->lastError) ;
          (*iter)->trouve = false ;
          return ;
        }
        // if (strncmp(pBBFiche->pDonnees->chemin, sCheminFils.c_str(), strlen(sCheminFils.c_str())) == 0 )
        if (sCheminFils == string(pBBFiche->pDonnees->chemin))
        {
          // (*iter)->trouve = true ;

          if (sTypeBase == GUIDE)
          {
            BBFiche* pBBCommeFiche = static_cast<BBFiche *>(pBBFiche) ;
            RechercheSelonCritereData* pRSC = static_cast<RechercheSelonCritereData *>(*iter) ;
            BBItemData* pDo = static_cast<BBItemData *>(pBBCommeFiche->pDonnees) ;
            *(pRSC->pDonnees) = *pDo ;
          }
          else if (sTypeBase == DECODE)
          {
            BBDecode* pBBCommeDecode = static_cast<BBDecode *>(pBBFiche) ;
            RechercheSelonCritereDecode* pRSC = static_cast<RechercheSelonCritereDecode *>(*iter) ;
            BBDecodeData* pDo = static_cast<BBDecodeData *>(pBBCommeDecode->pDonnees) ;
            *(pRSC->pDonnees) = *pDo ;
          }
          (*iter)->trouve = true ;


/*
          //savoir si cette fiche correspond à iter
          EquiItemVectorIter equivalent = (*iter)->pEquivalentBrut->begin() ;
          for ( ; (equivalent != (*iter)->pEquivalentBrut->end()) && ((*iter)->trouve == false) ; equivalent++)
          {
            EquiItemIter iterEqui = (*equivalent)->begin() ;
            string sEquivalent = *(*iterEqui) ;
            string sChemin = pBBFiche->pDonnees->chemin ;
            string sCheminFilGuide ;
            size_t posit = sChemin.find(string(1, cheminSeparationMARK)) ;
            if (posit == string::npos)
            	sCheminFilGuide = sChemin ;
            else
            {
              size_t debut = 0 ;
              while (posit != string::npos)
              {
                debut = posit ;
                posit = sChemin.find(string(1, cheminSeparationMARK), debut + 1) ;
              }
              sCheminFilGuide = string(sChemin, debut + 1, strlen(sChemin.c_str()) - debut - 1) ;
            }
            if (sEquivalent == sCheminFilGuide)
            {
              if (sTypeBase == GUIDE)
              {
                BBFiche* pBBCommeFiche = static_cast<BBFiche *>(pBBFiche) ;
                RechercheSelonCritereData* pRSC = static_cast<RechercheSelonCritereData *>(*iter) ;
                BBItemData* pDo = static_cast<BBItemData *>(pBBCommeFiche->pDonnees) ;
                *(pRSC->pDonnees) = *pDo ;
              }
              else if (sTypeBase == DECODE)
              {
                BBDecode* pBBCommeDecode = static_cast<BBDecode *>(pBBFiche) ;
                RechercheSelonCritereDecode* pRSC = static_cast<RechercheSelonCritereDecode *>(*iter) ;
                BBDecodeData* pDo = static_cast<BBDecodeData *>(pBBCommeDecode->pDonnees) ;
                *(pRSC->pDonnees) = *pDo ;
              }
              (*iter)->trouve = true ;
            }
          }

          if ((*iter)->trouve == false)
          {
            if (sTypeBase == GUIDE)
            {
              RechercheSelonCritereData* pRSC = static_cast<RechercheSelonCritereData *>(*iter) ;
              BBItemData* pDo = static_cast<BBItemData *>(pRSC->pDonnees) ;
              pRSC->pVecteurData->push_back(new BBItemData(*pDo)) ;
            }
            else if(sTypeBase == DECODE)
            {
              RechercheSelonCritereDecode* pRSC = static_cast<RechercheSelonCritereDecode *>(*iter) ;
              BBDecodeData* pDo = static_cast<BBDecodeData *>(pRSC->pDonnees) ;
              pRSC->pVecteurData->push_back(new BBDecodeData(*pDo)) ;
            }
          }
*/
        }
        else
        	(*iter)->trouve = false ;
      }
      continuer = true ;
    }
  }
#endif
}


// -----------------------------------------------------------------------------
// Egalité au sens réseau
// -----------------------------------------------------------------------------
// Décomposer pChaine et mettre ses items dans le vecteur pVect
// -----------------------------------------------------------------------------
void
NSFilGuide::DecomposeChaine(string* pChaine, VecteurString* pVect)
{
	if ((NULL == pChaine) || (NULL == pVect))
		return ;

  size_t debut = 0 ;
  size_t posit = pChaine->find(string(1, cheminSeparationMARK)) ;

  if (posit == string::npos)
  	pVect->push_back(new string(*pChaine)) ;
  else
  {
    while ((posit != string::npos) && (posit < strlen(pChaine->c_str())))
    {
      pVect->push_back(new string(*pChaine, debut, posit - debut)) ;
      debut = posit + 1 ;
      posit = pChaine->find(string(1, cheminSeparationMARK), debut + 1) ;
    }
    pVect->push_back(new string(*pChaine, debut, strlen(pChaine->c_str()) - debut)) ;
  }
}


// -----------------------------------------------------------------------------
// Trouver les items équivalents à sQualifie au sens réseau sémantique selon
// le lien sLien et les ranger dans pEquivalentItem
// Par défaut sensCle == "FLECHE"
// -----------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
void
NSFilGuide::chercheEquivalent(string sQualifie, VecteurString* pEquivalentItem, string sLien, DBITBLNAME sensCle)
{
	if (((VecteurString *) NULL == pEquivalentItem) ||
      ((NSSavoir *) NULL == pSavoir) ||
      (string("") == sQualifie) || (string("") == sLien))
  	return ;

try
{
	if (sLien == "ES")
  	pEquivalentItem->push_back(new string(sQualifie));
  //
  // Préparation de ItemPsGenerique, on remplace le caractère final par
  // le caractère qui donne le type de sQualifie
  //
  string sItemPseudoGenerique = ItemPsGenerique;
  sItemPseudoGenerique[strlen(sItemPseudoGenerique.c_str())-1] = sQualifie[0];
  //
  // Recherche dans le fichier
  //
  DBIResult  ErrDBI;
  CURProps curProps;
  DBIResult lastError = DbiGetCursorProps(pSavoir->PrendCurseur(), curProps);
  if (lastError != DBIERR_NONE)
  {
  	erreur("Le fichier nssavoir.db est inutilisable.", standardError, lastError);
    return ;
  }
	Byte* pIndexRec = new Byte[curProps.iRecBufSize] ;
	memset(pIndexRec, 0, curProps.iRecBufSize) ;
  //
  // Préparation de la clé à trouver
  //
  DbiPutField(pSavoir->PrendCurseur(), SAVOIR_LIEN_FIELD, pIndexRec,(Byte*)(sLien.c_str()));
  if (strcmp(sensCle, "FLECHE") == 0)
		DbiPutField(pSavoir->PrendCurseur(), SAVOIR_QUALIFIE_FIELD , pIndexRec, (Byte*)(sQualifie.c_str())) ;
  if (strcmp(sensCle, "ENVERS") == 0)
  {
		DbiPutField(pSavoir->PrendCurseur(), SAVOIR_QUALIFIANT_FIELD , pIndexRec, (Byte*)(sQualifie.c_str())) ;
		DbiPutField(pSavoir->PrendCurseur(), SAVOIR_DEGRE_FIELD , pIndexRec, (Byte*)("")) ;
  }

  ErrDBI = pSavoir->chercheClefComposite(sensCle,
                                        NODEFAULTINDEX,
                                        keySEARCHEQ,
                                        dbiWRITELOCK,
                                        pIndexRec) ;
  //
  // Si on ne trouve pas dans le réseau sémantique et que le lien cherché est
  // du type "est un", on met les génériques
  //
	if (ErrDBI != DBIERR_NONE)
	{
  	if ((sLien == "ES") && (strcmp(sensCle, "FLECHE") == 0))
    {
      pEquivalentItem->push_back(new string(sItemPseudoGenerique)) ;
      pEquivalentItem->push_back(new ItemGenerique ) ;
      pEquivalentItem->push_back(new SousChemin ) ;
      pEquivalentItem->push_back(new SousCheminNonVide ) ;
    }
    if (ErrDBI != DBIERR_RECNOTFOUND)
    	erreur("Le fichier nssavoir.db est inutilisable.", standardError, ErrDBI) ;

    delete[] pIndexRec ;
    return ;
	}

	pSavoir->lastError = pSavoir->getRecord() ;
	if (pSavoir->lastError != DBIERR_NONE)
	{
    erreur("Le fichier nssavoir.db est inutilisable.", standardError, pSavoir->lastError) ;
    delete[] pIndexRec ;
    return ;
	}
  //
  // Première passe : on stocke les éléments en relation directe avec sQualifie
  //
  while ((pSavoir->lastError == DBIERR_NONE) &&
         (strcmp(pSavoir->pDonnees->lien, sLien.c_str()) == 0) &&
         (((strcmp(sensCle, "FLECHE") == 0) &&
           (strcmp(pSavoir->pDonnees->qualifie, sQualifie.c_str()) == 0)
          ) ||
          ((strcmp(sensCle, "ENVERS") == 0) &&
           (strcmp(pSavoir->pDonnees->qualifiant, sQualifie.c_str()) == 0)
          )
         )
        )
	{
    // Vérifier que l'élément n'appartient pas au vecteur
    if ((strcmp(sensCle, "FLECHE") == 0) &&
        (pEquivalentItem->ItemDansUnVecteur(string(pSavoir->pDonnees->qualifiant)) == false) &&
        (string(pSavoir->pDonnees->degre) == ""))
      pEquivalentItem->push_back( new string((string(pSavoir->pDonnees->qualifiant)))) ;

    if ((strcmp(sensCle, "ENVERS") == 0) &&
        (pEquivalentItem->ItemDansUnVecteur(string(pSavoir->pDonnees->qualifie)) == false) &&
        (string(pSavoir->pDonnees->degre) == ""))
      pEquivalentItem->push_back( new string((string(pSavoir->pDonnees->qualifie)))) ;

    pSavoir->lastError = pSavoir->suivant(dbiWRITELOCK) ;
    if (pSavoir->lastError == DBIERR_NONE)
    {
    	pSavoir->lastError = pSavoir->getRecord() ;
      if (pSavoir->lastError != DBIERR_NONE)
      {
      	erreur("Le fichier nssavoir.db est inutilisable.", standardError, pSavoir->lastError) ;
        delete[] pIndexRec ;
        return ;
      }
    }
  }
  //
  // Deuxième passe : on prend les éléments en relation avec des éléments en
  // relation. Comme tout élément nouveau est ajouté à la fin de l'array,
  // on n'atteint end() que lorsqu'on a trouvé tous les élément quelque soit
  // leur niveau
  //
  if ((!(pEquivalentItem->empty())) && (sLien != "E0"))
  {
    EquiItemIter iter = pEquivalentItem->begin();
    while (iter != pEquivalentItem->end())
    {
      //DBIResult lastError = DbiGetCursorProps(pSavoir->PrendCurseur(), curProps);
      DbiPutField(pSavoir->PrendCurseur(), SAVOIR_LIEN_FIELD, pIndexRec,(Byte*)(sLien.c_str())) ;
      if (strcmp(sensCle, "FLECHE") == 0)
        DbiPutField(pSavoir->PrendCurseur(), SAVOIR_QUALIFIE_FIELD, pIndexRec, (Byte*)((*iter)->c_str())) ;
      if (strcmp(sensCle, "ENVERS") == 0)
      {
        DbiPutField(pSavoir->PrendCurseur(), SAVOIR_QUALIFIANT_FIELD, pIndexRec, (Byte*)((*iter)->c_str())) ;
        DbiPutField(pSavoir->PrendCurseur(), SAVOIR_DEGRE_FIELD, pIndexRec, (Byte*)("")) ;
      }

      ErrDBI = pSavoir->chercheClefComposite(sensCle,
                                            NODEFAULTINDEX,
                                            keySEARCHEQ,
                                            dbiWRITELOCK,
                                            pIndexRec);
      if (ErrDBI == DBIERR_NONE)
      {
        pSavoir->lastError = pSavoir->getRecord() ;

        while ((pSavoir->lastError == DBIERR_NONE) &&
                 (strcmp(pSavoir->pDonnees->lien, sLien.c_str()) == 0) &&
                 (((strcmp(sensCle, "FLECHE") == 0) &&
                   (strcmp(pSavoir->pDonnees->qualifie, (*iter)->c_str()) == 0)
                  ) ||
                  ((strcmp(sensCle, "ENVERS") == 0) &&
                   (strcmp(pSavoir->pDonnees->qualifiant, (*iter)->c_str()) == 0)
                  )
                 )
                )
        {
          // Redimensionnement préventif
          size_t iCurrentSize  = pEquivalentItem->size() ;
          size_t iCurrentCapa  = pEquivalentItem->capacity() ;
          if (iCurrentCapa < 10 + iCurrentSize)
          {
            string sIndex = **iter ;

            pEquivalentItem->reserve(100 + iCurrentSize) ;

            iter = pEquivalentItem->begin() ;
            for ( ; (iter != pEquivalentItem->end()) && (**iter != sIndex); iter++) ;
            if (iter == pEquivalentItem->end())
            {
              erreur("Erreur lors du redimensionnement : le noeud d'insertion n'est plus présent.", standardError, 0) ;
              return ;
            }
          }

                // Vérifier que l'élément n'appartient pas au vecteur
          if ((strcmp(sensCle, "FLECHE") == 0) &&
                      (pEquivalentItem->ItemDansUnVecteur(string(pSavoir->pDonnees->qualifiant)) == false) &&
                  (string(pSavoir->pDonnees->degre) == "") )
            pEquivalentItem->push_back( new string((string(pSavoir->pDonnees->qualifiant))) ) ;
          if ((strcmp(sensCle, "ENVERS") == 0) &&
                      (pEquivalentItem->ItemDansUnVecteur(string(pSavoir->pDonnees->qualifie)) == false) &&
                  (string(pSavoir->pDonnees->degre) == "") )
            pEquivalentItem->push_back( new string((string(pSavoir->pDonnees->qualifie))) ) ;

          pSavoir->lastError = pSavoir->suivant(dbiWRITELOCK);
          if (pSavoir->lastError == DBIERR_NONE)
          {
            pSavoir->lastError = pSavoir->getRecord() ;
            if (pSavoir->lastError != DBIERR_NONE)
            {
              erreur("Le fichier nssavoir.db est inutilisable.", standardError, pSavoir->lastError) ;
              delete[] pIndexRec ;
              return ;
            }
          }
        }
      }
      else if (ErrDBI != DBIERR_RECNOTFOUND)
      {
        erreur("Le fichier nssavoir.db est inutilisable.", standardError, ErrDBI) ;
        delete[] pIndexRec ;
        return ;
      }

      iter++ ;
    }
  }

  if ((sLien == "ES") && (strcmp(sensCle, "FLECHE") == 0))
  {
    pEquivalentItem->push_back(new string(sItemPseudoGenerique));
    pEquivalentItem->push_back(new ItemGenerique );
    pEquivalentItem->push_back(new SousChemin );
    pEquivalentItem->push_back(new SousCheminNonVide );
  }
  delete[] pIndexRec ;
}
catch (...)
{
	erreur("Exception NSFilGuide::chercheEquivalent", standardError, 0);
  return ;
}
}

#else

void
NSFilGuide::chercheEquivalent(string sQualifie, VecteurString* pEquivalentItem, string sLien, string sensCle)
{
	if (((VecteurString *) NULL == pEquivalentItem) ||
      ((NSSavoir *) NULL == pSavoir) ||
      (string("") == sQualifie) || (string("") == sLien) || (string("") == sensCle))
  	return ;

try
{
	if (string("ES") == sLien)
  	pEquivalentItem->push_back(new string(sQualifie)) ;
  //
  // Préparation de ItemPsGenerique, on remplace le caractère final par
  // le caractère qui donne le type de sQualifie
  //
  string sItemPseudoGenerique = ItemPsGenerique ;
  sItemPseudoGenerique[strlen(sItemPseudoGenerique.c_str())-1] = sQualifie[0] ;

  string sQuery = string("SELECT * FROM savoir WHERE ") ;

  if      (string("FLECHE") == sensCle)
  {
    sQuery += string("QUALIFIE = \"") + sQualifie + string("\" AND ") ;
    sQuery += string("LIEN = \"") + sLien + string("\"") ;
  }
  else if (string("ENVERS") == sensCle)
  {
    sQuery += string("QUALIFIANT = \"") + sQualifie + string("\" AND ") ;
    sQuery += string("LIEN = \"") + sLien + string("\" AND ") ;
    sQuery += string("DEGRE = \"\"") ;
  }
  else
    return ;

  MYSQL_RES *pResult = _pSuper->getDatabaseManager()->executeQuery(sQuery, true) ;

  if ((MYSQL_RES *) NULL == pResult)
	{
  	if ((string("ES") == sLien) && (string("FLECHE") == sensCle))
    {
      pEquivalentItem->push_back(new string(sItemPseudoGenerique)) ;
      pEquivalentItem->push_back(new ItemGenerique ) ;
      pEquivalentItem->push_back(new SousChemin ) ;
      pEquivalentItem->push_back(new SousCheminNonVide ) ;
    }
    return ;
	}

  MYSQL_ROW Row = mysql_fetch_row(pResult) ;
  if (Row)
    pSavoir->alimenteFiche(&Row) ;
  else
  {
    string sErreurMsg = string("Error when fetching row for query [") + sQuery + string("] -> ") + mysql_error(_pSuper->getDatabaseManager()->getConnector()) ;
    erreur(sErreurMsg.c_str(), standardError) ;

    if ((string("ES") == sLien) && (string("FLECHE") == sensCle))
    {
      pEquivalentItem->push_back(new string(sItemPseudoGenerique)) ;
      pEquivalentItem->push_back(new ItemGenerique ) ;
      pEquivalentItem->push_back(new SousChemin ) ;
      pEquivalentItem->push_back(new SousCheminNonVide ) ;
    }

    mysql_free_result(pResult) ;

    return ;
  }

  //
  // Première passe : on stocke les éléments en relation directe avec sQualifie
  //
  while (Row && (pSavoir->getLink() == sLien) &&
         (((string("FLECHE") == sensCle) &&
           (pSavoir->getQualified() == sQualifie)
          ) ||
          ((string("ENVERS") == sensCle) &&
           (pSavoir->getQualifier() == sQualifie)
          )
         )
        )
	{
    // Vérifier que l'élément n'appartient pas au vecteur
    if ((string("FLECHE") == sensCle) &&
        (pEquivalentItem->ItemDansUnVecteur(pSavoir->getQualifier()) == false) &&
        (pSavoir->pDonnees->getLevel() == ""))
      pEquivalentItem->push_back( new string(pSavoir->getQualifier())) ;

    if ((string("ENVERS") == sensCle) &&
        (pEquivalentItem->ItemDansUnVecteur(pSavoir->getQualified()) == false) &&
        (pSavoir->pDonnees->getLevel() == ""))
      pEquivalentItem->push_back( new string(pSavoir->getQualified())) ;

    Row = mysql_fetch_row(pResult) ;
    if (Row)
      pSavoir->alimenteFiche(&Row) ;
  }

  mysql_free_result(pResult) ;

  //
  // Deuxième passe : on prend les éléments en relation avec des éléments en
  // relation. Comme tout élément nouveau est ajouté à la fin de l'array,
  // on n'atteint end() que lorsqu'on a trouvé tous les élément quelque soit
  // leur niveau
  //
  if ((false == pEquivalentItem->empty()) && (string("E0") != sLien))
  {
    EquiItemIter iter = pEquivalentItem->begin() ;
    while (pEquivalentItem->end() != iter)
    {
      sQuery = string("SELECT * FROM savoir WHERE ") ;

      if      (string("FLECHE") == sensCle)
      {
        sQuery += string("QUALIFIE = \"") + **iter + string("\" AND ") ;
        sQuery += string("LIEN = \"") + sLien + string("\"") ;
      }
      else if (string("ENVERS") == sensCle)
      {
        sQuery += string("QUALIFIANT = \"") + **iter + string("\" AND ") ;
        sQuery += string("LIEN = \"") + sLien + string("\" AND ") ;
        sQuery += string("DEGRE = \"\"") ;
      }

      MYSQL_RES *pSubResult = _pSuper->getDatabaseManager()->executeQuery(sQuery, true) ;

      if (pSubResult)
      {
        MYSQL_ROW subRow = mysql_fetch_row(pSubResult) ;
        if (subRow)
          pSavoir->alimenteFiche(&subRow) ;

        while (subRow && (pSavoir->getLink() == sLien) &&
                 (((string("FLECHE") == sensCle) &&
                   (pSavoir->getQualified() == **iter)
                  ) ||
                  ((string("ENVERS") == sensCle) &&
                   (pSavoir->getQualifier() == **iter)
                  )
                 )
                )
        {
          // Redimensionnement préventif
          size_t iCurrentSize  = pEquivalentItem->size() ;
          size_t iCurrentCapa  = pEquivalentItem->capacity() ;
          if (iCurrentCapa < 10 + iCurrentSize)
          {
            string sIndex = **iter ;

            pEquivalentItem->reserve(100 + iCurrentSize) ;

            iter = pEquivalentItem->begin() ;
            for ( ; (iter != pEquivalentItem->end()) && (**iter != sIndex); iter++) ;
            if (iter == pEquivalentItem->end())
            {
              erreur("Erreur lors du redimensionnement : le noeud d'insertion n'est plus présent.", standardError) ;
              return ;
            }
          }

                // Vérifier que l'élément n'appartient pas au vecteur
          if ((string("FLECHE") == sensCle) &&
                      (pEquivalentItem->ItemDansUnVecteur(pSavoir->getQualifier()) == false) &&
                  (pSavoir->getLevel() == "") )
            pEquivalentItem->push_back( new string(pSavoir->getQualifier()) ) ;
          if ((string("ENVERS") == sensCle) &&
                      (pEquivalentItem->ItemDansUnVecteur(pSavoir->getQualified()) == false) &&
                  (pSavoir->getLevel() == "") )
            pEquivalentItem->push_back( new string(pSavoir->getQualified()) ) ;

          subRow = mysql_fetch_row(pSubResult) ;
          if (subRow)
            pSavoir->alimenteFiche(&subRow) ;
        }
      }
      else if ((MYSQL_RES *) NULL == pSubResult)
        return ;

      mysql_free_result(pSubResult) ;

      iter++ ;
    }
  }

  if ((string("ES") == sLien) && (string("FLECHE") == sensCle))
  {
    pEquivalentItem->push_back(new string(sItemPseudoGenerique)) ;
    pEquivalentItem->push_back(new ItemGenerique) ;
    pEquivalentItem->push_back(new SousChemin) ;
    pEquivalentItem->push_back(new SousCheminNonVide) ;
  }
}
catch (...)
{
	erreur("Exception NSFilGuide::chercheEquivalent", standardError) ;
  return ;
}
}
#endif

//-------------------------------------------------------------------
//pour un fils cherché, choisir dans la liste des fiches trouvées, celle
//qui correspond à l'élément qui lui est le plus proche dans le guide
//-------------------------------------------------------------------
void
NSFilGuide::ChoisirMeilleureProposition(VecteurRechercheSelonCritere* pVecteurSelonCritere)
{
	if ((NULL == pVecteurSelonCritere) || (pVecteurSelonCritere->empty()))
		return ;

	IterCritere iterFils = pVecteurSelonCritere->begin() ;
	for (; iterFils != pVecteurSelonCritere->end(); iterFils++)
	{
  	//
    // On ne cherche que les éléments qui n'ont pas déjà été trouvés
    //
    if (false == (*iterFils)->trouve)
    {
    	EquiItemVectorIter equivalent = (*iterFils)->pEquivalentBrut->begin() ;
      // Choisir la fiche qui correspond à l'équivalent le plus proche du
      // fils recherché
      for (; (equivalent != (*iterFils)->pEquivalentBrut->end()) &&
                                ((*iterFils)->trouve == false); equivalent++)
      {
      	if (false == (*equivalent)->empty())
        {
      		EquiItemIter iterEqui = (*equivalent)->begin() ;
          for (; (iterEqui != (*equivalent)->end()) && ((*iterFils)->trouve == false);
				            iterEqui++)
          {
          	string sEquivalent = *(*iterEqui) ;
            if (sTypeBase == GUIDE)
            {
            	RechercheSelonCritereData* pRSC = static_cast<RechercheSelonCritereData*>(*iterFils) ;
              EquiDataIter iterData = pRSC->pVecteurData->begin() ;

              for (; (iterData != pRSC->pVecteurData->end()) && (pRSC->trouve == false);
      	         		                                            iterData++)
              {
              	string sChemin = (*iterData)->chemin ;
                string sCheminFilGuide ;
                size_t posit = sChemin.find(string(1, cheminSeparationMARK)) ;
                if (string::npos == posit)
                	sCheminFilGuide = sChemin ;
                else
                {
                	size_t debut = 0 ;
                  while (string::npos != posit)
                  {
                  	debut = posit ;
                    posit = sChemin.find(string(1,cheminSeparationMARK), debut+1) ;
                  }
                  sCheminFilGuide = string(sChemin, debut+1, strlen(sChemin.c_str())-debut-1) ;
                }

                if (sEquivalent == sCheminFilGuide)
                {
                	*(pRSC->pDonnees) = *(*iterData) ;
                  (*iterFils)->trouve = true ;
                }
              }
            }
            else if (sTypeBase == DECODE)
            {
            	RechercheSelonCritereDecode* pRSC = static_cast<RechercheSelonCritereDecode*>(*iterFils) ;
              EquiDecodeIter iterData = pRSC->pVecteurData->begin() ;
              for (; (iterData != pRSC->pVecteurData->end()) && (pRSC->trouve == false);
				      	         		iterData++)
              {
              	string sChemin = (*iterData)->chemin ;
                string sCheminFilGuide ;
                size_t posit = sChemin.find(string(1,cheminSeparationMARK)) ;
    						if (string::npos == posit)
                	sCheminFilGuide = sChemin ;
                else
                {
                	size_t debut = 0 ;
    							while (string::npos != posit)
    							{
                  	debut = posit ;
                    posit = sChemin.find(string(1,cheminSeparationMARK), debut+1) ;
    							}
    							sCheminFilGuide = string(sChemin, debut+1, strlen(sChemin.c_str())-debut-1);
                }
                if (sEquivalent == sCheminFilGuide)
                {
                	*(pRSC->pDonnees) = *(*iterData) ;
                  (*iterFils)->trouve = true ;
                }
              }
            }
          }
        }
      }
    }
	}
}

//---------------------------------------------------------------------------
// Comparer les deux items sItem1 et sItem2
// pEquivalentItem : vecteur contenant les équivalents sémantiques de sItem1
//---------------------------------------------------------------------------
bool
NSFilGuide::comparerItem(string sItem2, VecteurString* pEquivalentItem)
{
	if ((!pEquivalentItem) || (pEquivalentItem->empty()))
  	return false ;

  //égalité au sens réseau
  for (EquiItemIter i = pEquivalentItem->begin(); i != pEquivalentItem->end(); i++)
  	if ((*(*i)) == sItem2)
    	return true ;

  return false ;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
int
NSFilGuide::NombreSeparateurDansUneChaine(string *pChaine)
{
	if ((!pChaine) || (*pChaine == ""))
  	return 0 ;

  string sSeparator = string(1, cheminSeparationMARK) ;

  int nb = 0 ;
  size_t posit = pChaine->find(sSeparator) ;

  if (posit == string::npos)
  	return 0 ;

	while (posit != string::npos)
  {
    nb++ ;
    posit = pChaine->find(sSeparator, posit + 1) ;
  }
  return nb ;
}

// Distance between a genuine source path (without jokers) and
// a "generic" target path
//
int
NSFilGuide::pathDistance(string* psTarget, string* psSource, bool bWithSemantic, DBITBLNAME sensCle)
{
	// Trivial cases
	//
	if (!psSource || !psTarget)
  	return pathDistanceInfinite ;

  if (*psSource == *psTarget)
  	return pathDistanceExact ;

  if ((*psSource == string("")) || (*psTarget == string("")))
  	return pathDistanceInfinite ;

  // More complex cases
  //

  bool bJokerInTarget = false ;

  // Not to waste time : if no joker and no semantics, leave now
  //
  if ((psTarget->find(string("*")) != string::npos) ||
      (psTarget->find(string("?")) != string::npos))
  	bJokerInTarget = true ;

  if (!bWithSemantic && !bJokerInTarget)
  	return pathDistanceInfinite ;

  // Target parsing
  //
  VecteurString VectItemTarget ;
  DecomposeChaine(psTarget, &VectItemTarget) ;
  // Should never occur ; just by security
  if (VectItemTarget.empty())
  	return pathDistanceInfinite ;

  // Source semantic parsing
  //
  VecteurString VectItemSource ;
  DecomposeChaine(psSource, &VectItemSource) ;
  // Should never occur ; just by security
  if (VectItemSource.empty())
  	return pathDistanceInfinite ;

  VecteurItem SourceVecEquivalentItem ;

  // First, if there are jokers in the target, we try a pure generic approach
  // in order not to waste time in the semantic network
  //
  if (bJokerInTarget)
  {
  	// Building the vector with only the generic concepts of source path elements
  	for (EquiItemIter iterSource = VectItemSource.begin(); iterSource != VectItemSource.end(); iterSource++)
  	{
  		VecteurString* pEquivalentItem = new VecteurString() ;

      pEquivalentItem->push_back(new string(*(*iterSource))) ;
      pEquivalentItem->push_back(new ItemGenerique ) ;
      pEquivalentItem->push_back(new SousChemin ) ;
      pEquivalentItem->push_back(new SousCheminNonVide ) ;

    	sort(pEquivalentItem->begin(), pEquivalentItem->end(), inferieure) ;
    	SourceVecEquivalentItem.push_back(pEquivalentItem) ;
  	}

  	int iPureGenericDistance = specificPathDistance(&VectItemTarget, &SourceVecEquivalentItem, &VectItemSource) ;
    if (iPureGenericDistance < pathDistanceInfinite)
    	return pathDistanceGeneric + iPureGenericDistance ;

    SourceVecEquivalentItem.vider() ;
  }

  if (!bWithSemantic)
  	return pathDistanceInfinite ;

  // Building the vector with all the semantic related concepts of source path elements
  //
  for (EquiItemIter iterSource = VectItemSource.begin(); iterSource != VectItemSource.end(); iterSource++)
  {
  	VecteurString* pEquivalentItem = new VecteurString() ;

    chercheEquivalent(*(*iterSource), pEquivalentItem, string("ES"), sensCle) ;
    sort(pEquivalentItem->begin(), pEquivalentItem->end(), inferieure) ;

    SourceVecEquivalentItem.push_back(pEquivalentItem) ;
  }

  // If can we find jokers in the target, it means generic + semantic
  //
  if (bJokerInTarget)
  {
		int iGenericSemantic = specificPathDistance(&VectItemTarget, &SourceVecEquivalentItem, &VectItemSource) ;
    if (iGenericSemantic < pathDistanceInfinite)
    	return pathDistanceSemanticGeneric + iGenericSemantic ;
    else
    	return pathDistanceInfinite ;
  }

  int iPureSemantic = specificPathDistance(&VectItemTarget, &SourceVecEquivalentItem, &VectItemSource) ;
  if (iPureSemantic < pathDistanceInfinite)
  	return pathDistanceSemantic + iPureSemantic ;
  else
  	return pathDistanceInfinite ;
}

int
NSFilGuide::specificPathDistance(VecteurString* pVectItemTarget, VecteurItem* pEquivalentSource, VecteurString* pVectItemSource)
{
	//
  // Trivial cases
  //
	if (!pVectItemTarget || !pEquivalentSource)
  	return pathDistanceInfinite ;

  if ((pVectItemTarget->empty()) && (pEquivalentSource->empty()))
  	return pathDistanceExact ;

  if ((pVectItemTarget->empty()) || (pEquivalentSource->empty()))
  	return pathDistanceInfinite ;

  // Time to work
  //
  EquiItemIter       iterTarget = pVectItemTarget->begin() ;
  EquiItemVectorIter iterVect   = pEquivalentSource->begin() ;

  if (!(testeEgaliteChemin(pVectItemTarget, iterTarget, pEquivalentSource, iterVect)))
  	return pathDistanceInfinite ;

  // A améliorer
  //
  if (!pVectItemSource || (pVectItemSource->empty()))
		return (pEquivalentSource->size() - pVectItemTarget->size()) ;
  else
  	return (pEquivalentSource->size() - pVectItemTarget->size()) ;
}

//---------------------------------------------------------------------------
//
// Tester l'égalité de deux chemins (stricte ou sémantique) item par item
//    sChaineCible     : chaine du chemin cible
//    EquivalentSource : vecteur contenant tous les équivalents des items
//                      (du père + du fils)
//---------------------------------------------------------------------------
bool
NSFilGuide::testeEgaliteChemin(VecteurString* pVectItemCible, EquiItemIter IterCible,
                     VecteurItem* pEquivalentSource, EquiItemVectorIter vect)
{
	if ((!pVectItemCible) || (pVectItemCible->empty()))
  	return false ;
  if ((!pEquivalentSource) || (pEquivalentSource->empty()))
  	return false ;

  // ----- just to ease debugging -----
  // string sCible = "" ;
  // EquiItemIter cible = pVectItemCible->begin() ;
  // for (; cible != pVectItemCible->end(); cible++)
  //  	sCible += *(*cible) + string("/") ;

  EquiItemIter IterCibleSuivant = pVectItemCible->begin() ;

  bool encore = true ;
  while ((IterCible != pVectItemCible->end()) && (vect != pEquivalentSource->end()))
  {
  	//
    // The current concept in the target (IterCible) is not a multi-concepts
    // generic concept so, we simply look if it belongs to the current semantic
    // equivalent of the current concept in the source (vect)
    //
  	if ((*(*IterCible)).find(string("*")) == string::npos)  // != de souschemin
    {
    	if (comparerItem((*(*IterCible)), (*vect)))
      {
      	vect++ ;
        IterCible++ ;
      }
      else  //IterCible n'est pas équivalent à son homologue dans la chaîne source
      	return false ;
    }
    //
    // The current concept in the target (IterCible) is a multi-concepts generic concept
    //
    else
    {
    	//
      // SouscheminNonVide  : incrémenter vect
      //
      // If the current concept in the target (IterCible) is a generic concept
      // that can't be empty, we consider that it replace at least the current
      // concept in the source, so we skip it
      //
      if ((*(*IterCible)) == SousCheminNonVide)
      	vect++ ;

      IterCible++;

      if ((vect == pEquivalentSource->end()) && (IterCible != pVectItemCible->end()))
      	return false ;

      if (IterCible == pVectItemCible->end())
      	return true ;

      //                   1 2 3 4 5 6 7 8 9 10 11
    	//exemple 	source = A/B/C/D/E/F/G/D/H/C/Z
      //          Cible  = */D/L/N

      //
      // Chercher les équivalents de IterCible (D) dans source
      //

      //
      // s'il y en a plusieurs, le but est de chercher l'égalité avec chaque
      // équivalent de D C.A.D avec (E/F/G/D/H/C/Z, ...) et (H/C/Z, ...)
      //

      //
      // si IterCible = D alors on trouve 2 équivalents dans source et on prend le 1°
      // (numéro 4)
      //
      IterCibleSuivant = IterCible ;
      IterCibleSuivant++ ;

      while (encore)
      {
      	while ((vect  != pEquivalentSource->end()) && !(comparerItem((*(*IterCible)),(*vect))))
        	vect++ ;

        if (vect != pEquivalentSource->end())
        	vect++ ; //E
        else
        	return false ;

        //
        // 1er appel recursif avec *pChaineCible = E/F/G/D/H/C/Z
        // et pEquivalentSource -> vecteurs des équivalents de E, F, G , H, C et Z
        //
        if (testeEgaliteChemin(pVectItemCible, IterCibleSuivant, pEquivalentSource, vect))
        	return true ;
      }
    }
  }

  if ((IterCible == pVectItemCible->end()) && (vect == pEquivalentSource->end()))
  	return true ;
  else
  	return false ;
}

//---------------------------------------------------------------------------
//		tester l'égalité de deux chemins (stricte ou sémantique)
//  		pVectItemSource 	: 	les items du chemin source
//			pCheminPere			:	chemin source
//  		sCible     			:  chaine du chemin cible
//			SousChemin			:	sous chaine
//---------------------------------------------------------------------------
void
NSFilGuide::testeEgaliteChaineCible(VecteurRechercheSelonCritere* pVecteurSelonCritere,
												int NBSeparateur, string* pCheminPere, string sCible)
{
try
{
	if ((!pVecteurSelonCritere) || (pVecteurSelonCritere->empty()))
  	return ;

  IterCritere iter = pVecteurSelonCritere->begin() ;
  int TraiterCefils = 1 ; //continuer à traiter le fils en cours
  int separ ;			        //nombre de séparateurs total dans cheminsource : pere + fils
  int nbSeparateurChaineCible = NombreSeparateurDansUneChaine(&sCible) ;

  for (; iter != pVecteurSelonCritere->end(); iter++)
  {
    if ((*iter)->trouve == false)
    {
    	string sCheminFils ;
      if (*pCheminPere == string (""))
      	sCheminFils = (*iter)->sEtiquette ;
      else
      	sCheminFils = *pCheminPere + string(1,cheminSeparationMARK) +
                                                        (*iter)->sEtiquette ;

      if (sCheminFils.find(string(1,cheminSeparationMARK)) == string::npos)
      	separ = 0 ;
      else
      	// Calculer le nombre de séparateurs dans (*iter)->sEtiquette
        separ = NBSeparateur +
                    NombreSeparateurDansUneChaine(&(*iter)->sEtiquette) + 1 ;

      if ((nbSeparateurChaineCible != separ) &&
          (sCible.find(SousChemin) == string::npos) &&
            (sCible.find(SousCheminNonVide) == string::npos))  //chaine cible (non sous chemin)plus longue que la chaine source
      {
      	(*iter)->trouve = false ;
        TraiterCefils = 0 ;
      }
      if (TraiterCefils)
      {
      	VecteurItem EquivalentSource ;
        // sur le père
        EquiItemVectorIter vect = pVecEquivalentItemDuPere->begin();
        for (; vect != pVecEquivalentItemDuPere->end(); vect++)
        	EquivalentSource.push_back(*vect) ;
        // sur les fils
        EquiItemVectorIter vec = ((*iter)->pEquivalentTrie)->begin();
        for (; vec != ((*iter)->pEquivalentTrie)->end(); vec++)
        	EquivalentSource.push_back(*vec) ;

        EquiItemVectorIter VectEqui = EquivalentSource.begin() ;

        VecteurString VectItemCible ;  // contient les items de sChaineCible
        DecomposeChaine(&sCible, &VectItemCible) ;
        EquiItemIter IterCible = VectItemCible.begin() ;

        if (testeEgaliteChemin(&VectItemCible, IterCible, &EquivalentSource, VectEqui))
        {
        	// Savoir si cette fiche correspond à iter
          EquiItemVectorIter equivalent = (*iter)->pEquivalentBrut->begin() ;
          for (; (equivalent != (*iter)->pEquivalentBrut->end()) &&
                                          ((*iter)->trouve == false);
                                                    equivalent++)
          {
          	EquiItemIter iterEqui = (*equivalent)->begin() ;
            string sEquivalent = *(*iterEqui) ;
            string sChemin = pBBFiche->pDonnees->chemin;
            string sCheminFilGuide;
            size_t posit = sChemin.find(string(1,cheminSeparationMARK));
            if (posit == string::npos)
            	sCheminFilGuide = sChemin ;
            else
            {
            	size_t debut = 0 ;
              while (posit != string::npos)
              {
              	debut = posit ;
                posit = sChemin.find(string(1,cheminSeparationMARK), debut+1) ;
              }
              sCheminFilGuide = string(sChemin, debut+1, strlen(sChemin.c_str())-debut-1) ;
            }
            if (sEquivalent == sCheminFilGuide)
            {
            	if (sTypeBase == GUIDE)
              {
              	BBFiche* pBBCommeFiche = static_cast<BBFiche*>(pBBFiche) ;
                RechercheSelonCritereData* pRSC = static_cast<RechercheSelonCritereData*>(*iter) ;
                BBItemData* pDo = static_cast<BBItemData*>(pBBCommeFiche->pDonnees) ;
                *(pRSC->pDonnees) = *pDo ;
              }
              else if (sTypeBase == DECODE)
              {
              	BBDecode* pBBCommeDecode = static_cast<BBDecode*>(pBBFiche) ;
                RechercheSelonCritereDecode* pRSC = static_cast<RechercheSelonCritereDecode*>(*iter) ;
                BBDecodeData* pDo = static_cast<BBDecodeData*>(pBBCommeDecode->pDonnees) ;
                *(pRSC->pDonnees) = *pDo ;
              }
              //*((*iter)->pDonnees) = *(pBBFiche->pDonnees);
              (*iter)->trouve = true ;
            }
          }
          if ((*iter)->trouve == false)
          {
          	if (sTypeBase == GUIDE)
            {
            	/*RechercheSelonCritereData* pRSC = static_cast<RechercheSelonCritereData*>(*iter);
                BBItemData* pDo = static_cast<BBItemData*>(pRSC->pDonnees);
                pRSC->pVecteurData->push_back(new BBItemData(*pDo));*/

              BBFiche* pBBCommeFiche = static_cast<BBFiche*>(pBBFiche) ;
              RechercheSelonCritereData* pRSC = static_cast<RechercheSelonCritereData*>(*iter) ;
              BBItemData* pDo = static_cast<BBItemData*>(pBBCommeFiche->pDonnees) ;
              pRSC->pVecteurData->push_back(new BBItemData(*pDo)) ;
            }
            else if (sTypeBase == DECODE)
            {
            	BBDecode* pBBCommeDecode = static_cast<BBDecode*>(pBBFiche);
              RechercheSelonCritereDecode* pRSC = static_cast<RechercheSelonCritereDecode*>(*iter);
              BBDecodeData* pDo = static_cast<BBDecodeData*>(pBBCommeDecode->pDonnees);
              pRSC->pVecteurData->push_back(new BBDecodeData(*pDo));
            }
          }
          //if((*iter)->trouve == false)
          //  (*iter)->pVecteurData->push_back(new BBItemData(*(pBBFiche->pDonnees)));
        }

        // comme le vecteur ne contient que des copies de pointeurs
        // il ne faut pas le "deleter" avant de l'avoir "vidé" à la main
        EquiItemVectorIter i = EquivalentSource.begin() ;
        while (i != EquivalentSource.end())
        	EquivalentSource.erase(i) ;
      }
      TraiterCefils = 1 ;
    }
	}
}
catch (...)
{
	erreur("Exception NSFilGuide::testeEgaliteChaineCible", standardError) ;
  return ;
}
}

//---------------------------------------------------------------------------
// 								Recherche sémantique
//---------------------------------------------------------------------------
void
NSFilGuide::chercheCheminReseau(string* pCheminPere,
						VecteurRechercheSelonCritere* pVecteurSelonCritere)
{
#ifndef _ENTERPRISE_DLL
try
{
	if ((NULL == pCheminPere) || (NULL == pVecteurSelonCritere))
		return ;
	if ((string("") == *pCheminPere) && (pVecteurSelonCritere->empty()))
		return ;

	bool bLimitSemanticSearch = false ;

	// Recherche exacte
	chercheCheminEgaliteStricte(pCheminPere, pVecteurSelonCritere) ;

	// autres fils à traiter
	if (false == pVecteurSelonCritere->FilsAtraiter())
		return ;

	IterCritere iter ;
	string      ch ;
	int         NombreElementEnCommun ;  // nombre d'items en commun entre les deux chemins
   													           // au sens réseau sémantique
	string      sCible ; //chemin en cours dans la BD

	pBBFiche->lastError = pBBFiche->chercheClef(pCheminPere,
                                                "CHEMIN",
                                                NODEFAULTINDEX,
                                                keySEARCHGEQ,
                                                dbiWRITELOCK) ;
	if (pBBFiche->lastError != DBIERR_NONE)
	{
  	pBBFiche->lastError = pBBFiche->fin(dbiWRITELOCK) ;
    if (pBBFiche->lastError != DBIERR_NONE)
    {
    	for( iter = pVecteurSelonCritere->begin() ; iter != pVecteurSelonCritere->end(); iter++)
      	(*iter)->trouve = false ;
      erreur("Le fichier des guides semble vide.", standardError, pBBFiche->lastError) ;
      return ;
    }
	}
	pBBFiche->lastError = pBBFiche->getRecord() ;
	if (pBBFiche->lastError != DBIERR_NONE)
	{
		erreur("Le fichier nsguides.db inutilisable.", standardError, pBBFiche->lastError);
    for (iter = pVecteurSelonCritere->begin(); iter != pVecteurSelonCritere->end(); iter++)
    	(*iter)->trouve = false ;
    return ;
	}
  //
  //limite pour la montée : 	à partir d'une cible on procéde à la montée,
  //                         on calcule la nouvelle chaîne sNouvelleChiane et on fait
  //									la descente jusqu'à sLimiteMonter
  //exemple	:
  //
  //
  //
  //                    sNouvelleChaine
  //                           .	^
  //                           .   ^   montée
  //                           .   ^
  //				  sLimiteMonter <- sFutureLimiteMonter
  //				sNouvelleChiane <- sFutureLimiteMonter
  //									  .   ^
  //                           .   ^   montée
  //                           .   ^
  //							chaîne cible A/B/C/D/E
  //
  //
	string sLimiteMonter ;
	string sFutureLimiteMonter = string(pBBFiche->pDonnees->chemin) ;

	// Nombre de séparateurs dans pCheminPere
	int NBSeparateur = NombreSeparateurDansUneChaine(pCheminPere) ;

	// Initialisation
	pVecEquivalentItemDuPere->vider() ;

	// pVectItemPere est un vecteur qui contient les items de pCheminPere
  VecteurString VectItemPere ;
  DecomposeChaine(pCheminPere, &VectItemPere) ;
  EquiItemIter iterPere = VectItemPere.begin() ;

	// Nombre maximal d'items communs à pCheminPere et la chaîne cible
	VecteurString VectItemCible ;  // contient les items de pCheminPere
	DecomposeChaine(&string(pBBFiche->pDonnees->chemin), &VectItemCible) ;
	NombreElementEnCommun = VectItemPere.NbElementCommun(&VectItemCible) ;

	int nbElement = NombreElementEnCommun ;
	int cont      = 1 ;
	// Construire le vecteur contenant tous les équivalents des items du chemin Père
	for (iterPere = VectItemPere.begin() ; VectItemPere.end() != iterPere ; iterPere++)
	{
  	VecteurString EquivalentItem ; //les équivalents au sens réseau d'un item donné

    // En cas de limite sémantique, on ne prend les équivalents sémantiques
    // qu'au delà de NombreElementEnCommun
    // 
    if ((false == bLimitSemanticSearch) || (cont > NombreElementEnCommun))
    	chercheEquivalent(*(*iterPere), &EquivalentItem, string("ES")) ; // constituer les équivalents au sens réseau de (*iter)
    else
    	EquivalentItem.push_back(new string(*(*iterPere))) ;

    sort(EquivalentItem.begin(), EquivalentItem.end(), inferieure) ;
    pVecEquivalentItemDuPere->push_back(new VecteurString(EquivalentItem)) ;
    cont++ ;
	}

	// constituer les équivalents au sens réseau des fils
	for (iter = pVecteurSelonCritere->begin() ; iter != pVecteurSelonCritere->end() ; iter++)
	{
		if (!(*iter)->trouve)
    {
    	VecteurString VectItemFils ;  // contient les items de pCheminPere
      DecomposeChaine(&(*iter)->sEtiquette, &VectItemFils) ;
      EquiItemIter iterFils = VectItemFils.begin() ;
      for (; iterFils != VectItemFils.end() ; iterFils++)
      {
      	VecteurString EquivalentItem ; //les équivalents au sens réseau d'un item donné
        chercheEquivalent(*(*iterFils), &EquivalentItem, string("ES")) ; // constituer les équivalents au sens réseau de (*iter)
        (*iter)->pEquivalentBrut->push_back(new VecteurString(EquivalentItem)) ;
        sort(EquivalentItem.begin(), EquivalentItem.end(), inferieure) ;
        (*iter)->pEquivalentTrie->push_back(new VecteurString(EquivalentItem)) ;
      }
    }
  }
	testeEgaliteChaineCible(pVecteurSelonCritere, NBSeparateur,
                            pCheminPere, string(pBBFiche->pDonnees->chemin)) ;
	// Autres fils à traiter ?
	if (false == pVecteurSelonCritere->FilsAtraiter())
		return ;
	//
	// Initialisation
	//
	nbElement = NombreElementEnCommun ;
	bool result ;
	string sNouvelleCible = "" ;
	//
	// Création de 2 bookMark
	//
	pBBFiche->creerBookMarks(2) ;
	bool descendre, monter ;
	pBBFiche->lastError = pBBFiche->placeBookMark(2) ;
	if (pBBFiche->lastError == DBIERR_NONE)
  	monter = true ;
	else
		monter = false ;
	//   pBBFiche->lastError = pBBFiche->suivant(dbiWRITELOCK);
	pBBFiche->lastError = pBBFiche->placeBookMark(1) ;
	if (pBBFiche->lastError == DBIERR_NONE)
  	descendre = true ;
	else
		descendre = false ;
	//
	// au moins le premier élément ( à gauche ) en commun
	//
	while ((NombreElementEnCommun >= 0) &&
           (pBBFiche->lastError == DBIERR_NONE) && (monter || descendre))
	{
		//
    // Commencer par la montée pour ne pas tomber sur ??? avant d'avoir
    // scanné tous les équivalents sémantiques
    //
		if (monter)
    {
    	//
      // Initialiser sLimiteMonter à la chaîne limite (sFutureLimiteMonter)
      // à ne pas depasser lors de la descente
      //
			sLimiteMonter = sFutureLimiteMonter ;
      string sNouvelleCible = NouvelleCible(&VectItemPere,
                                                  // bLimitSemanticSearch ? NombreElementEnCommun : 0,
                                                  NombreElementEnCommun,
                                                  pVecteurSelonCritere) ;

      pBBFiche->lastError = pBBFiche->chercheClef(&sNouvelleCible,
                                                        "CHEMIN",
                                                        NODEFAULTINDEX,
                                                        keySEARCHGEQ,
                                                        dbiWRITELOCK) ;
			if (pBBFiche->lastError == DBIERR_NONE)
      {
      	pBBFiche->lastError = pBBFiche->getRecord() ;
        if (pBBFiche->lastError != DBIERR_NONE)
        {
        	erreur("Le fichier de script est inutilisable (guides.db).", standardError, pBBFiche->lastError) ;
          pBBFiche->detruireBookMarks(2) ;
          return ;
        }
        sCible = string(pBBFiche->pDonnees->chemin) ;
        VecteurString VectItemCible ;  // contient les items de pCheminPere
        DecomposeChaine(&sCible, &VectItemCible) ;
        nbElement = VectItemPere.NbElementCommun(&VectItemCible) ;
        sFutureLimiteMonter = string(pBBFiche->pDonnees->chemin) ;
      }

      while ((sCible < sLimiteMonter) &&
                   (nbElement >= NombreElementEnCommun) &&
                   (pBBFiche->lastError == DBIERR_NONE))
      {
      	testeEgaliteChaineCible(pVecteurSelonCritere, NBSeparateur,
                                                        pCheminPere, sCible) ;
      	//
        // Autres fils à traiter ?
        //
        if (false == pVecteurSelonCritere->FilsAtraiter())
        {
        	pBBFiche->detruireBookMarks(2) ;
          return ;
        }

        result = RechercheFicheSuivante(&VectItemPere, sCible, pVecteurSelonCritere,
                                            NombreElementEnCommun) ;
        if (result)
        {
        	sCible = string(pBBFiche->pDonnees->chemin) ;
          VecteurString VectItemCible ;  // contient les items de pCheminPere
          DecomposeChaine(&sCible, &VectItemCible) ;
          nbElement = VectItemPere.NbElementCommun(&VectItemCible) ;
        }
        // On n'arrive pas à imaginer un nouveau chemin
        //
        else
        	sCible = sLimiteMonter ;
      }

      if (pBBFiche->lastError != DBIERR_NONE)
      	monter = false ;
    }
    if (descendre)
    {
    	pBBFiche->lastError = pBBFiche->retrouveBookMark(1) ;
      if ((pBBFiche->lastError == DBIERR_NONE))
      {
      	pBBFiche->lastError = pBBFiche->getRecord() ;
        sCible = string(pBBFiche->pDonnees->chemin) ;
        VecteurString VectItemCible ;  // contient les items de pCheminPere
        DecomposeChaine(&sCible, &VectItemCible) ;
        nbElement = VectItemPere.NbElementCommun(&VectItemCible) ;
      }
      result = true ;
      while ((result) && (nbElement >= NombreElementEnCommun) && (pBBFiche->lastError == DBIERR_NONE))
      {
      	testeEgaliteChaineCible(pVecteurSelonCritere, NBSeparateur,
                                                        pCheminPere, sCible) ;
        //
        // Autres fils à traiter ?
        //
        if (false == pVecteurSelonCritere->FilsAtraiter())
        {
        	pBBFiche->detruireBookMarks(2) ;
          return ;
        }
        result = RechercheFicheSuivante(&VectItemPere, sCible, pVecteurSelonCritere,
                                            NombreElementEnCommun);
        if (result)
        {
        	sCible = string(pBBFiche->pDonnees->chemin) ;
          VecteurString VectItemCible ;  // contient les items de pCheminPere
          DecomposeChaine(&sCible, &VectItemCible) ;
          nbElement = VectItemPere.NbElementCommun(&VectItemCible) ;
        }
      }
      if (pBBFiche->lastError == DBIERR_NONE)
      	pBBFiche->lastError = pBBFiche->placeBookMark(1) ;

      if (pBBFiche->lastError != DBIERR_NONE)
      	descendre = false ;
    }
    ChoisirMeilleureProposition(pVecteurSelonCritere) ;
    //
    // Don't do this because changing pVecEquivalentItemDuPere would give weird
    // results when calling testeEgaliteChaineCible (since the initial element
    // is first to be removed, exact equality no longer works)
    //
    // MettreAjourEquivalent(NombreElementEnCommun) ;
    //
    NombreElementEnCommun-- ;
	}
	pBBFiche->detruireBookMarks(2) ;
}
catch (...)
{
	erreur("Exception NSFilGuide::chercheCheminReseau", standardError, 0) ;
}
#endif
}

NSFilGuideEngine::NSFilGuideEngine(NSSuper* pSuper)
                 :NSSuperRoot(pSuper)
{
	_bCherchePivot      = true ;
  _iRangPivotPere     = 0 ;
	_iRangPivotCible    = 0 ;
  _iNbCommonElements  = 0 ;
	_sCibleAudelaDuPere = string("") ;
  _sDernierItemPere   = string("") ;

	_sPivot             = string("") ;
  _sChaineFils        = string("") ;
	_sPrePivot          = string("") ;

  _sNouvelleCible     = string("") ;

  _pVecteurSelonCritere     = 0 ;
  _pVecEquivalentItemDuPere = 0 ;
  _itEquivalentItemDuPere   = (EquiItemVectorIter) 0 ;
  _pVectItemPere            = 0 ;
  _iterPere                 = (EquiItemIter) 0 ;
}

NSFilGuideEngine::~NSFilGuideEngine()
{
}

void
NSFilGuideEngine::initTargetVector(string sTargetString)
{
	DecomposeChaine(&sTargetString, &_VectItemCible) ;
  _IterCible = _VectItemCible.begin() ;
}

void
NSFilGuideEngine::initVectItemPere(VecteurString* pVectPere)
{
	_pVectItemPere = pVectPere ;

  _sDernierItemPere = *(_pVectItemPere->back()) ;
  _iterPere         = _pVectItemPere->begin() ;
}

void
NSFilGuideEngine::setEquiItemPere(VecteurItem* pEqItemPere)
{
	_pVecEquivalentItemDuPere = pEqItemPere ;

  if ((NULL != _pVecEquivalentItemDuPere) && (false == _pVecEquivalentItemDuPere->empty()))
		_itEquivalentItemDuPere = _pVecEquivalentItemDuPere->begin() ;
	else
    _itEquivalentItemDuPere = (EquiItemVectorIter) 0 ;
}

// -----------------------------------------------------------------------------
// Egalité au sens réseau
// -----------------------------------------------------------------------------
// Décomposer pChaine et mettre ses items dans le vecteur pVect
// -----------------------------------------------------------------------------
void
NSFilGuideEngine::DecomposeChaine(string* pChaine, VecteurString* pVect)
{
	if ((NULL == pChaine) || (NULL == pVect))
		return ;

  size_t debut = 0 ;
  size_t posit = pChaine->find(string(1, cheminSeparationMARK)) ;

  if (posit == string::npos)
  	pVect->push_back(new string(*pChaine)) ;
  else
  {
    while ((posit != string::npos) && (posit < strlen(pChaine->c_str())))
    {
      pVect->push_back(new string(*pChaine, debut, posit - debut)) ;
      debut = posit + 1 ;
      posit = pChaine->find(string(1, cheminSeparationMARK), debut + 1) ;
    }
    pVect->push_back(new string(*pChaine, debut, strlen(pChaine->c_str()) - debut)) ;
  }
}

//
// Chercher les pivots et leurs rangs dans sCible et dans la chaîne du père
//
void
NSFilGuideEngine::getPivotsAndRanks()
{
	if ((NULL == _pVectItemPere) || (NULL == _pVecEquivalentItemDuPere))
		return ;

	EquiItemIter       PereItereTemp              = _pVectItemPere->begin() ;
  EquiItemVectorIter itEquivalentItemDuPereTemp = _pVecEquivalentItemDuPere->begin() ;

  _itEquivalentItemDuPere = _pVecEquivalentItemDuPere->begin() ;

	bool avance = true ;

	while ((avance) &&
           (_VectItemCible.end() != _IterCible) &&
           (_pVectItemPere->end() != _iterPere) &&
           (_pVecEquivalentItemDuPere->end() != _itEquivalentItemDuPere))
	{
		if ((*(*_IterCible)).find(string("*")) == string::npos)  // != de souschemin
		{
    	// Si équivalence sémantique avancer, sinon sortir
      if ((*_itEquivalentItemDuPere)->ItemDansUnVecteur((*(*_IterCible))))
      {
      	// _IndiceSource.push_back(_iRangPivotPere);
        if (_pVecEquivalentItemDuPere->end() != _itEquivalentItemDuPere)
        	_itEquivalentItemDuPere++ ;
        if (_VectItemCible.end() != _IterCible)
        	_IterCible++ ;
        if (_pVectItemPere->end() != _iterPere)
        	_iterPere++ ;
        PereItereTemp = _iterPere ;
        itEquivalentItemDuPereTemp = _itEquivalentItemDuPere ;
      }
      else
      	avance = false ;
    }
    else
    {
    	//
      // SouscheminNonVide : équivalent à l'item du père en cours
      //
      if (SousCheminNonVide == (*(*_IterCible)))
      {
      	// Avancer d'un cran chez le père
        if (_pVecEquivalentItemDuPere->end() != _itEquivalentItemDuPere)
        	_itEquivalentItemDuPere++ ;
        if (_pVectItemPere->end() != _iterPere)
        	_iterPere++ ;

        PereItereTemp = _iterPere ;
        itEquivalentItemDuPereTemp = _itEquivalentItemDuPere ;
      }
      _IterCible++ ;
      //
      // Commencer à comparer avec _IterCible en commençant _iterPere
      //
      // Avancer tant que pas d'équivalence sémantique avec _IterCible
      if (_VectItemCible.end() != _IterCible)
      {
      	// Garder le dernier pivot trouvé chez le père
        PereItereTemp = _iterPere ;
        itEquivalentItemDuPereTemp = _itEquivalentItemDuPere ;
        while ((_pVecEquivalentItemDuPere->end() != _itEquivalentItemDuPere) &&
               (_pVectItemPere->end() != _iterPere) &&
               (false == (*_itEquivalentItemDuPere)->ItemDansUnVecteur((*(*_IterCible)))))
        {
        	if (_pVecEquivalentItemDuPere->end() != _itEquivalentItemDuPere)
          	_itEquivalentItemDuPere++ ;
          if (_pVectItemPere->end() != _iterPere)
          	_iterPere++ ;
        }
        // à la fin de la chaîne père
        if (_pVectItemPere->end() == _iterPere)
        	//_IndiceSource.push_back(0);
        	avance = false ;
        else
        {
        	_iterPere++ ;
          _itEquivalentItemDuPere++ ;
          PereItereTemp = _iterPere ;
          itEquivalentItemDuPereTemp = _itEquivalentItemDuPere ;
          //_IndiceSource.push_back(_iRangPivotPere);
          if (_IterCible != _VectItemCible.end())
          	_IterCible++ ;
        }
      }
    }
	}

  _iterPere = PereItereTemp ;
	_itEquivalentItemDuPere = itEquivalentItemDuPereTemp ;
}

void
NSFilGuideEngine::getRanks()
{
	EquiItemIter it   = _pVectItemPere->begin() ;
	EquiItemIter iter = _VectItemCible.begin() ;

	if (_iterPere == _pVectItemPere->end())
  	_iRangPivotPere = _pVectItemPere->size() + 1 ;
  else
  	for (_iRangPivotPere = 1;
                (it != _pVectItemPere->end()) && (it != _iterPere);
                            it++, _iRangPivotPere++) ;

	if (_IterCible == _VectItemCible.end())
  	_iRangPivotCible = _VectItemCible.size() + 1 ;
	else
		for (_iRangPivotCible = 1;
                (iter != _VectItemCible.end()) && (iter != _IterCible);
                           iter++, _iRangPivotCible++) ;
}

//
// vecteur des indices des items de la source ayant des équivalents dans sCible
// mettre 0 si pas d'équivalence
// exemple : source : A/B/C/D/E/F/G/H
//           sCible : A/J/*/L/O
//				J - is a -> B
//				L - is a -> F
//				donne : IndiceSource = [1,2,0,6,0]
void
NSFilGuideEngine::getPivotsVector()
{
	EquiItemIter iter = _VectItemCible.begin() ;

	size_t i ;
	bool continuer = true ;
	for (i = 1; (iter != _VectItemCible.end()) && (continuer); i++, iter++)
	{
		if (iter != _IterCible)
		{
    	if ((*(*iter)).find(string("*")) != string::npos)
      	_IndiceSource.push_back(0) ;
      else
      	_IndiceSource.push_back(i) ;
    }
    else
    {
    	if (_VectItemCible.end() != _IterCible)
      	_IndiceSource.push_back(_iRangPivotPere) ;
      continuer = false ;
    }
	}
	// Remplir _IndiceSource avec des 0
	for (i = _iRangPivotCible + 1; i <= _VectItemCible.size(); i++)
		_IndiceSource.push_back(0) ;

	if (_iRangPivotCible)
		_bCherchePivot = true ;
}

void
NSFilGuideEngine::getTargetBeyondFather()
{
	_sCibleAudelaDuPere = "" ;

	if (_iterPere == _pVectItemPere->end())
	{
		EquiItemIter iter = _VectItemCible.begin() ;
    iter = _IterCible ;

    if (_VectItemCible.end() != iter)
    {
    	_sCibleAudelaDuPere = *(*iter) ;
      iter++ ;
    }
		while (_VectItemCible.end() != iter)
    {
    	_sCibleAudelaDuPere += "/" + *(*iter) ;
      iter++ ;
    }
	}
}

bool
NSFilGuideEngine::findPivot()
{
	while ((_iRangPivotPere) && (_bCherchePivot))
	{
		//
    // On cherche le pivot au niveau du père
    //
    if ((_iRangPivotPere) && (_iterPere != _pVectItemPere->end()))
    {
    	if (_VectItemCible.end() != _IterCible)
      {
      	if (_VectItemCible.begin() != _IterCible)
        {
        	_IterCible-- ;
          _sPrePivot = *(*_IterCible) ;
          _IterCible++ ;
        }
        //
        // ATTENTION : si l'élément ne suit pas un ~~** on cherche
        // l'élément strictement supérieur pour le rang en cours.
        //
				if (_sPrePivot.find(string("*")) == string::npos)
        {
        	_sPivot = (*_itEquivalentItemDuPere)->ItemStrictementSuperieur(*(*_IterCible)) ;

          // In order not to start an infinite loop, refuse the exact concept
          //
          if ((_iNbCommonElements == _iRangPivotPere) && (_sPivot == *(*_iterPere)))
          	_sPivot = (*_itEquivalentItemDuPere)->ItemStrictementSuperieur(*(*_iterPere)) ;

          if (_sPivot != "")
          	_bCherchePivot = false ;
        }
        //
        // Si l'élément suit un ~~** (qui peut remplacer plusieurs items),
        // on cherche le plus petit des éléments strictement supérieurs
        // pour tous les éléments situés après
        //
        else
        {
        	EquiItemVectorIter Tempvect = _pVecEquivalentItemDuPere->begin() ;
          Tempvect = _itEquivalentItemDuPere ;

          size_t  rangSup = 0 ;
          size_t  rang    = _iRangPivotPere ;
          string  sSup    = "~~~~~~~~~~~~~" ;
          string sPivotSupLocal ;
          //
          // On énumère tous les éléments >= _iRangPivotPere pour
          // trouver celui qui possède le plus petit des équivalents
          // d'_IterCible
          //
          while (rang <= _pVectItemPere->size())
          {
          	sPivotSupLocal = (*Tempvect)->ItemStrictementSuperieur(*(*_IterCible)) ;
            if ((sPivotSupLocal != "") && (sPivotSupLocal < sSup))
            {
            	char chFirst = sPivotSupLocal[0] ;
              char chLast  = sPivotSupLocal[strlen(sPivotSupLocal.c_str())-1] ;

              if ((chFirst != '~') ||
                  ((chLast >= '0') && (chLast <= '9')) ||
                  ((chLast >= 'A') && (chLast <= 'Z')))
              {
              	sSup = sPivotSupLocal ;
                rangSup = rang ;
              }
            }
            if (rang < _pVectItemPere->size())
            	Tempvect++ ;
            rang++ ;
          }
          //
          // même recherche chez les fils
          //
          VecteurString VectTemp ;  // contient les items de sChaineCible
					DecomposeChaine((*_IterCible), &VectTemp) ;
          sPivotSupLocal = _pVecteurSelonCritere->ChaineStrictementSuperieur(&VectTemp, SousChemin) ;
          //
          // Le pivot est chez les fils
          //
          if ((string("") != sPivotSupLocal) && (sPivotSupLocal < sSup))
          {
          	char chFirst = sPivotSupLocal[0] ;
            char chLast  = sPivotSupLocal[strlen(sPivotSupLocal.c_str())-1] ;

            if ((chFirst != '~') ||
                ((chLast >= '0') && (chLast <= '9')) ||
                ((chLast >= 'A') && (chLast <= 'Z')))
            {
            	_iRangPivotPere = _pVectItemPere->size() + 1 ;
              _iterPere       = _pVectItemPere->end() ;
              _itEquivalentItemDuPere = _pVecEquivalentItemDuPere->end() ;
              _sChaineFils = sPivotSupLocal ;
              _bCherchePivot = false ;
            }
          }
          //
          // Le pivot est chez le père
          //
          else if (sSup != "~~~~~~~~~~~~~")
          {
          	_sPivot = sSup ;
            _bCherchePivot = false ;
            while (_iRangPivotPere < rangSup)
            {
            	_itEquivalentItemDuPere++ ;
              _iRangPivotPere++ ;
              _iterPere++ ;
            }
          }
        }
        //
        // Si on n'a pas trouvé de pivot, on recule d'un cran
        //
        if ((_sPivot == "") && (_bCherchePivot))
        {
        	_iRangPivotCible-- ;
          if (_VectItemCible.begin() != _IterCible)
          	_IterCible-- ;
          else
            return false ;

          //
          // trouver la position de (_iterPere,_iRangPivotPere)
          // correspondant à (_IterCible, _iRangPivotCible)
          //

          indiceIterator Indice = _IndiceSource.begin() ;
          // trouver _iRangPivotPere = _IndiceSource[_iRangPivotCible]
          for (size_t iCible = 1; (Indice != _IndiceSource.end()) &&
                            (iCible != _iRangPivotCible) ; Indice++, iCible++) ;

          if (Indice != _IndiceSource.end())
          {
          	_iRangPivotPere = *Indice ;

            // si on tombe sur 0, il faut reculer encore d'un cran
            while (*Indice == 0)
            {
            	if (Indice != _IndiceSource.begin())
              {
              	Indice-- ;
                _iRangPivotCible-- ;
                _IterCible-- ;
                _iRangPivotPere = *Indice ;
              }
              else
                return false ;
            }

            _itEquivalentItemDuPere = _pVecEquivalentItemDuPere->begin() ;
            _iterPere = _pVectItemPere->begin() ;
            size_t con ;
            // trouver _iterPere correspondant à _iRangPivotPere
            for (con = 1; (_iterPere != _pVectItemPere->end()) && (con != _iRangPivotPere)
                           && (_pVecEquivalentItemDuPere->end() != _itEquivalentItemDuPere);
                           _iterPere++, _itEquivalentItemDuPere++, con++) ;
          }

                    /*iterator Indice = _IndiceSource.begin();
                    // trouver RangPivotPere
                    for (RangPivotPere = 1; (Indice != _IndiceSource.end()) &&
                        (*Indice != _iRangPivotCible) ; Indice++, RangPivotPere++);

                    if (Indice != _IndiceSource.end())
                    {
                        // si on tombe sur 0, il faut reculer encore d'un cran
                        if (*Indice == 0)
                        {
                            if (Indice != _IndiceSource.begin())
                            {
               		            Indice--;
                  	            RangPivotCible--;
               		            IterCible--;
                            }
                            else
                            {
                  	            delete pVectItemCible;
               		            return false;
                            }
                        }

                        _itEquivalentItemDuPere = pVecEquivalentItemDuPere->begin();
               	        _iterPere = _pVectItemPere->begin();
                        int con;
                        // trouver _iterPere correspondant à RangPivotPere
                        for (con = 1; (_iterPere != _pVectItemPere->end()) && (con != RangPivotPere)
                           && (_itEquivalentItemDuPere != pVecEquivalentItemDuPere->end());
                           _iterPere++, _itEquivalentItemDuPere++, con++);
                    } */
        	if (_iRangPivotPere <= _iNbCommonElements)
            return false ;
        }
      }
      //
      // On est dans le père mais au delà de la cible : prendre le min des
      // équivalents du pivot chez le père(_iterPere)
      //
      else
      {
      	_sPivot 		 =  *(*(*_itEquivalentItemDuPere)->begin()) ;
        _bCherchePivot = false ;
      }
    }
    else
    {
      // traiter les fils
      // chaine cible
      VecteurString VectItemChaine ;  // contient les items de sChaineCible
      DecomposeChaine(&_sCibleAudelaDuPere, &VectItemChaine) ;
      _sChaineFils = _pVecteurSelonCritere->ChaineStrictementSuperieur(&VectItemChaine, _sDernierItemPere) ;
      if (_sChaineFils != "")
        _bCherchePivot = false ;
      else
      {
      	// passer au père
        _iRangPivotPere-- ;
        if (_iterPere != _pVectItemPere->begin())
        {
          _iterPere-- ;
          _itEquivalentItemDuPere-- ;
        }
        else
          return false ;
      }
    }
  }
  return true ;
}

void
NSFilGuideEngine::buildNewTargetString()
{
	_sNouvelleCible = string("") ;

	EquiItemIter iter = _VectItemCible.begin() ;

	//
	// Si le pivot est au niveau du père
	//
	if (_sChaineFils == "")
	{
    // on recopie tous les éléments de la cible situés avant le pivotCible
    //
    for (size_t cont = 1; (_VectItemCible.end() != iter) && (cont < _iRangPivotCible); iter++, cont++)
    	_sNouvelleCible += *(*iter) + string(1,cheminSeparationMARK) ;
		//
    // On ajoute le pivot
    //
    _sNouvelleCible += _sPivot ;
    //
    // On complète par les min des éléments au dela de pivotPere
    //
    // Rappel : _iterPere et _itEquivalentItemDuPere sont positionnés
    // 			sur le rang du pivoPere
    //
    size_t rangComplement = _iRangPivotPere ;
    //
    if (_sPivot.find(string("*")) != string::npos)
  	{
    	//
    	// le pivot est un ~~**
    	// On trouve le min des équivalents sémantiques situés au delà du pivot
    	// et on concatène le min des éléments situés au delà
    	// calcul du Rang du min des mins
    	//
    	EquiItemVectorIter Tempvect = _pVecEquivalentItemDuPere->begin() ;
    	size_t rangMin = 0 ;
    	size_t rang 	 = 1 ;
    	string sMin    = "~~~~~~~~~~~~~" ;
    	string sMinLocal ;
    	while (rang <= _pVectItemPere->size())
    	{
        if (rang > _iRangPivotPere)
        {
        	sMinLocal = *(*((*Tempvect)->begin())) ;
        	if (sMinLocal < sMin)
        	{
        		sMin = sMinLocal ;
        		rangMin = rang ;
        	}
    		}
    		if (rang < _pVectItemPere->size())
    			Tempvect++ ;
    		rang++ ;
    	}
    	sMinLocal = _pVecteurSelonCritere->BorneInf() ;
    	if (sMinLocal < sMin)
      	rangMin = _pVectItemPere->size() + 1 ;

    	while (rangComplement < rangMin-1)
    	{
        _itEquivalentItemDuPere++ ;
        rangComplement++ ;
    		_iterPere++ ;
    	}
    }

    while (rangComplement < _pVectItemPere->size())
    {
    	_itEquivalentItemDuPere++ ;
      _iterPere++ ;
    	rangComplement++ ;
    	if (_pVecEquivalentItemDuPere->end() != _itEquivalentItemDuPere)
      	_sNouvelleCible += string(1,cheminSeparationMARK) + *(*(*_itEquivalentItemDuPere)->begin()) ;
    }
    string sBI = _pVecteurSelonCritere->BorneInf() ;
  	_sNouvelleCible += string(1,cheminSeparationMARK) + sBI ;
	}
  //
  // Si le pivot est chez les fils
  //
	else
	{
    for (size_t cont = 1; (_VectItemCible.end() != iter)
                                && (cont < _iRangPivotCible); iter++, cont++)
    	_sNouvelleCible += *(*iter) + string(1,cheminSeparationMARK) ;

		_sNouvelleCible += _sChaineFils ;
	}
}

//-----------------------------------------------------------------------------------
// 						Trouver la fiche suivante à traiter
//								( au sens sémantique) entre  deux chaines
//							pVectItemPere : Vecteurdes items du père
//                    				sCible : chaine cible
//						pNouvelleCible : nouvelle chaine cible calculée
//
//-----------------------------------------------------------------------------------
bool
NSFilGuide::RechercheFicheSuivante(VecteurString* pVectItemPere, string sCible,
						VecteurRechercheSelonCritere* pVecteurSelonCritere, int NombreElementEnCommun)
{
#ifndef _ENTERPRISE_DLL
try
{
	if ((NULL == pVectItemPere) || (true == pVectItemPere->empty()))
		return false ;

	NSFilGuideEngine FGE(_pSuper) ;
  FGE.setEquiItemPere(pVecEquivalentItemDuPere) ;
  FGE.initTargetVector(sCible) ;
  FGE.initVectItemPere(pVectItemPere) ;
  FGE.setVectSelonCriteres(pVecteurSelonCritere) ;
  FGE.setNbCommonElements(NombreElementEnCommun) ;

	string sPere = "" ;
	EquiItemIter pere = pVectItemPere->begin() ;
	for (; pere != pVectItemPere->end(); pere++)
		sPere += *(*pere) + string("/") ;

  //
  // vecteur des indices des items de la source ayant des équivalents dans sCible
  // mettre 0 si pas d'équivalence
  // exemple : source : A/B/C/D/E/F/G/H
  //           sCible : A/J/*/L/O
  //				J - is a -> B
  //				L - is a -> F
  //				donne : IndiceSource = [1,2,0,6,0]

	//
	// Chercher les pivots et leurs rangs dans sCible et dans la chaîne du père
	//
  FGE.getPivotsAndRanks() ;

	//
	// Determiner les rangs des pivots trouvés
	//
  FGE.getRanks() ;

	//
	// Mettre à jour le vecteur des indices
	//
  FGE.getPivotsVector() ;

	//
	// _sCibleAudelaDuPere n'est utilisée que dans le cas où _iterPere est audelà
	// de sPere -> le pivot est dans les fils
	//
  FGE.getTargetBeyondFather() ;

	//
  // Recherche du pivot
  //
  if (false == FGE.findPivot())
  	return false ;

	//
	// Fabrication de sNouvelleCible
	//
	FGE.buildNewTargetString() ;

  string sNouvelleCible = FGE.getNewTarget() ;

  //
  // Si la nouvelle cible formée est <= à sCible alors c'est qu'on
  // n'est pas capable de trouver une cible plus grande donc sortir
	//
	if (!(sCible < sNouvelleCible))
		return false ;

	pBBFiche->lastError = pBBFiche->chercheClef(&sNouvelleCible,
                                                "CHEMIN",
                                                NODEFAULTINDEX,
                                                keySEARCHGEQ,
                                                dbiWRITELOCK) ;
	if (pBBFiche->lastError == DBIERR_NONE)
	{
    pBBFiche->lastError = pBBFiche->getRecord() ;
    if (pBBFiche->lastError != DBIERR_NONE)
  	{
    	erreur("Le fichier de script est inutilisable (guides.db).", standardError, pBBFiche->lastError) ;
    	return false ;
		}
	}
	else
		return false ;

	return true ;
}
catch (...)
{
	erreur("Exception NSFilGuide::RechercheFicheSuivante", standardError, 0) ;
	return false ;
}
#else
  return false ;
#endif
}

//----------------------------------------------------------------------------
//  						Trouver la fiche suivante à traiter
// 			  		NombreElementEnCommun le nombre d'items communs
//								( au sens exact) entre  deux chaines
//                   	 pVectItemPere : items du père
//       Retour : sCile(0, NombreElementEnCommun) + min(sur le père et les fils)
//----------------------------------------------------------------------------
string
NSFilGuide::NouvelleCible(VecteurString* pVectItemPere, int NombreElementEnCommun,
                               VecteurRechercheSelonCritere* pVecteurSelonCritere)
{
	if ((NULL == pVectItemPere) || (pVectItemPere->empty()))
		return "" ;
	if ((NULL == pVecteurSelonCritere) || (pVecteurSelonCritere->empty()))
		return "" ;

	if ((NULL == pVecEquivalentItemDuPere) || (pVecEquivalentItemDuPere->empty()))
		return "" ;

	string sNouvelleChaine = "" ;

	//équivalents des items du père
	EquiItemVectorIter vect = pVecEquivalentItemDuPere->begin() ;

	//chaine cible
	EquiItemIter IterPere = pVectItemPere->begin() ;
	for (int i = 1; (IterPere != pVectItemPere->end()) && (i <= NombreElementEnCommun) &&
                  (vect != pVecEquivalentItemDuPere->end()) ;
                   IterPere++, vect++, i++)
		sNouvelleChaine += *(*IterPere) + string(1,cheminSeparationMARK) ;

	// prendre les minimums des équivalents des items du père
	while (vect != pVecEquivalentItemDuPere->end())
	{
    if (false == (*vect)->empty())
		  sNouvelleChaine += *(*(*vect)->begin()) + string(1, cheminSeparationMARK) ;
    vect++ ;
	}
	sNouvelleChaine += pVecteurSelonCritere->BorneInf() ;

	return sNouvelleChaine ;
}

//------------------------------------------------------------------------------
// mettre à jour les équivalents de l'item à la position NombreElementEnCommun
//------------------------------------------------------------------------------
void
NSFilGuide::MettreAjourEquivalent(int NombreElementEnCommun)
{
	if ((NULL == pVecEquivalentItemDuPere) || (pVecEquivalentItemDuPere->empty()))
		return ;

	int cont = 1 ;
	EquiItemVectorIter iter = pVecEquivalentItemDuPere->begin();
	for (; iter != pVecEquivalentItemDuPere->end(); iter++)
	{
		if (cont == NombreElementEnCommun)
		{
    	string sItem = *(*((*iter)->begin())) ;
      (*iter)->vider() ;
      chercheEquivalent(sItem, (*iter), string("ES")) ; // constituer les équivalents au sens réseau de (*iter)
      sort((*iter)->begin(), (*iter)->end(), inferieure) ;
      return ;
    }
    cont++ ;
	}
}

//-----------------------------------------------------------------------------
// 						recherche par objet
//-----------------------------------------------------------------------------
void
NSFilGuide::chercheCheminObjet(string* /* pChemin */, VecteurRechercheSelonCritere* /* VecteurSelonCriter */)
{
}

//---------------------------------------------------------------------------
// chercher pChemin dans le filguide selon la précision donnée :
//         	compStrict	:	égalité strice
//         	compReseau	:	égalité au sens réseau
//          compObjet	:  égalité au sens objet
//	pVecteurSelonCritere :  contient (pour chaque fils à trouver) :
//										->	son étiquette
//										->	booléen 	: trouvé ou non
//										-> pDonnees : pointeur sur BBItemData
//										-> ses équivalents sémantiques
//---------------------------------------------------------------------------
void NSFilGuide::chercheChemin(string* pCheminPere,
                               VecteurRechercheSelonCritere* pVecteurSelonCritere,
                               int precision)
{
	switch (precision)
	{
  	case compStrict	:
    	chercheCheminEgaliteStricte(pCheminPere, pVecteurSelonCritere) ;
      break ;
    case compReseau	:
    	chercheCheminReseau(pCheminPere, pVecteurSelonCritere) ;
      break;
    case compObjet :
    	chercheCheminObjet(pCheminPere, pVecteurSelonCritere) ;
      break;
	}
}

//---------------------------------------------------------------------------
// Inverser une chaine.
//	Exemple : OCONT/GECHY donne GECHY/OCONT
//---------------------------------------------------------------------------
void
NSFilGuide::InverseChaine(string* pLocalisation, string* pInverse)
{
	if (NULL == pInverse)
		return ;
	*pInverse = string("") ;

	if ((NULL == pLocalisation) || (string("") == *pLocalisation))
		return ;

	size_t posit = pLocalisation->find(string(1, cheminSeparationMARK)) ;
  if (posit == string::npos)
	{
		*pInverse = *pLocalisation ;
    return ;
	}

	size_t debut = 0 ;
	int    i = 1 ;

	while (posit != string::npos)
	{
		if (i == 1 )
			*pInverse = string(1, cheminSeparationMARK) + string(*pLocalisation, debut, posit - debut) + *pInverse ;
		else
			*pInverse = string(*pLocalisation, debut, posit - debut) + *pInverse;

		debut = posit ;
		posit = pLocalisation->find(string(1, cheminSeparationMARK), debut+1) ;
		i++ ;
	}
	*pInverse = string(*pLocalisation, debut+1, strlen(pLocalisation->c_str())-debut-1) + *pInverse ;
}


// -----------------------------------------------------------------------------
//
// Fonctions annexes
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Fonction qui renvoie si le code sCode1 est lié par la relation sRelation au
// code sCode2.
// Modif FLP : si les codes sont des codes Lexique, on les transforme en codes
// sens (avant la fonction renvoyait faux systématiquement si les codes n'était
// pas des codes Sens).
// --
// Does code sCode1 has a relation sRelation to code sCode2.
// Modified by FLP : now it can take both LexicalCode than MeaningCode.
// -----------------------------------------------------------------------------
bool
NSFilGuide::VraiOuFaux(string sCode1, string sRelation, string sCode2, DBITBLNAME sensCle, bool bPumpMessages)
{
try
{
	// récupération des codes Sens.
	// Retrieving MeaningCodes.
	string sCodeSens1 = string("") ;
	NSDico::donneCodeSens(&sCode1, &sCodeSens1) ;
	string sCodeSens2 = string("") ;
	NSDico::donneCodeSens(&sCode2, &sCodeSens2) ;

  // First, check if there is a ready answer
  //
  VectString* pReadyResult = aSearchEltsArray.getExistingResult(sCodeSens1, sRelation, sensCle) ;
  if (pReadyResult)
  {
    if (pReadyResult->contains(sCodeSens2))
      return true ;
    else
      return false ;
  }

  // If no ready answer exists, go search in semantic network
  //
	VectString VecteurString ;
	TousLesVrais(sCodeSens1, sRelation, &VecteurString, sensCle, bPumpMessages) ;

  // And add this search to searches buffer
  //
  aSearchEltsArray.addResult(sCodeSens1, sRelation, sensCle, &VecteurString) ;

	if (VecteurString.contains(sCodeSens2))
    return true ;

	return false ;
}
catch (...)
{
	erreur("Exception NSFilGuide::VraiOuFaux", standardError) ;
	return false ;
}
}

//---------------------------------------------------------------------------
// Mettre tous les items liés à sCode1 par sRelation dans pVecteurString
//
// Si la relation est "ES", TousLesVrais est équivalent à chercheEquivalent,
// pour les autres relations, il permet, pour chaque élément valide, d'ajouter
// ses équivalents à la liste
//---------------------------------------------------------------------------
void
NSFilGuide::TousLesVrais(string sCode, string sRelation, VectString* pVecteurString, DBITBLNAME sensCle, bool bPumpMessages)
{
try
{
	if (NULL == pVecteurString)
		return ;

  string ps = "NSFilGuide::TousLesVrais: entering (" + sCode + " " + sRelation + " " + sensCle + ")" ;
  _pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

	bool ok ;
	//équivalents sémantiques de sCode
	VecteurString EquivalentsCode ;
	chercheEquivalent(sCode, &EquivalentsCode, "ES", sensCle) ;

  if (EquivalentsCode.empty())
  {
    ps = "NSFilGuide::TousLesVrais: nothing found, leaving." ;
    _pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;
		return ;
  }

  int iVectorSize = 0 ;

	EquiItemIter EquiCode = EquivalentsCode.begin() ;
	for (; EquiCode != EquivalentsCode.end(); EquiCode++)
	{
#ifndef _ENTERPRISE_DLL
    if (bPumpMessages)
      _pSuper->getApplication()->PumpWaitingMessages() ;
#endif

		string sLie ;
    //trouver tous les items liés à EquiCode par sRelation
    VecteurString EquivalentItemsLie ;
    chercheEquivalent(*(*EquiCode), &EquivalentItemsLie, sRelation, sensCle) ;
    if (false == EquivalentsCode.empty())
    {
    	EquiItemIter EquiItemLie = EquivalentItemsLie.begin() ;
      for (; EquiItemLie != EquivalentItemsLie.end(); EquiItemLie++)
      {
#ifndef _ENTERPRISE_DLL
        if (bPumpMessages)
          _pSuper->getApplication()->PumpWaitingMessages() ;
#endif

      	sLie = *(*EquiItemLie) ;
        //chercher les équivalents sémantiques de sLie
        VecteurString EquivalentsLie ;
        chercheEquivalent(sLie, &EquivalentsLie, "ES", sensCle) ;
        if (false == EquivalentsLie.empty())
  			{
        	EquiItemIter EquiLie = EquivalentsLie.begin() ;
          for (; EquiLie != EquivalentsLie.end(); EquiLie++)
          {
          	string sEquivalent = *(*EquiLie) ;
            ok = true ;
            //vérifier que *(*EquiLie) ne fait pas partie déjà de pVecteurString
            for (IterString i = pVecteurString->begin(); (i != pVecteurString->end())
                    && ok; i++)
            	if (((*(*i)) == sEquivalent))
              	ok = false ;

            if (ok && (!(sEquivalent == SousChemin)) &&
            		   (!(sEquivalent == ItemGenerique)) &&
                     (!(sEquivalent == SousCheminNonVide)))
            {
            	pVecteurString->push_back(new string(*(*EquiLie))) ;
              iVectorSize++ ;
            }
          }
        }
      }
    }
	}
  ps = "NSFilGuide::TousLesVrais: done, leaving (" + std::to_string(iVectorSize) + " elements found)";
  _pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;
}
catch (...)
{
	erreur("Exception NSFilGuide::TousLesVrais", standardError) ;
	return ;
}
}

//---------------------------------------------------------------------------
//premier item lié à sCode1 par sRelation
//---------------------------------------------------------------------------
string
NSFilGuide::PremierVrai(string sCode1, string sRelation, DBITBLNAME sensCle)
{
	return chercheQualifiant(sCode1, sRelation, sensCle) ;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
string
NSFilGuide::InversePremierVrai(string /* sCode1 */, string /* sRelation */, DBITBLNAME /* sensCle */)
{
	return "" ;
}

//---------------------------------------------------------------------------
//mettre tous les items liés à sCode1 par sRelation dans pVecteurString
//---------------------------------------------------------------------------
void
NSFilGuide::InverseTousLesVrais(string /* sCode */, string /* sRelation */, VectString* /* pVecteurString */, DBITBLNAME /* sensCle */, bool /* bPumpMessages */)
{
}

//---------------------------------------------------------------------------
//savoir si sCode1 et sCode2 sont liés par sRelation
//---------------------------------------------------------------------------
bool
NSFilGuide::InverseVraiOuFaux(string /* sCode1 */, string /* sRelation */, string /* sCode2 */, DBITBLNAME /* sensCle */, bool /* bPumpMessages */)
{
    return false ;
}

//--------------------------------------------------------------------------
//recherche composite selon le lien sLien et le qualifié sQualifie, retourner
//le qualifiant
//---------------------------------------------------------------------------
string
NSFilGuide::chercheQualifiant(string sQualifie, string sLien, DBITBLNAME sensCle)
{
#ifndef _ENTERPRISE_DLL
try
{
  DBIResult  ErrDBI;
  CURProps curProps;
  DBIResult lastError = DbiGetCursorProps(pSavoir->PrendCurseur(), curProps);
  if (lastError != DBIERR_NONE)
  {
    erreur("Le fichier nssavoir.db inutilisable.", standardError, lastError);
    return "";
  }
  Byte* pIndex = new Byte[curProps.iRecBufSize];
  memset(pIndex, 0, curProps.iRecBufSize);
  DbiPutField(pSavoir->PrendCurseur(), SAVOIR_LIEN_FIELD, pIndex,(Byte*)(sLien.c_str()));
  DbiPutField(pSavoir->PrendCurseur(), SAVOIR_QUALIFIE_FIELD , pIndex, (Byte*)(sQualifie.c_str()));
  ErrDBI = pSavoir->chercheClefComposite(sensCle,
                                        NODEFAULTINDEX,
                                        keySEARCHEQ,
                                        dbiWRITELOCK,
                                        pIndex);
  if (ErrDBI != DBIERR_NONE)
  {
    delete[] pIndex;
    return "";
  }

  pSavoir->lastError = pSavoir->getRecord();
  if (pSavoir->lastError != DBIERR_NONE)
  {
    erreur("Le fichier nssavoir.db inutilisable.", standardError, pSavoir->lastError);
    delete[] pIndex;
    return "";
  }
  string sQualifiant = string(pSavoir->pDonnees->qualifiant);
  delete[] pIndex;
  return sQualifiant;
}
catch (...)
{
	erreur("Exception NSFilGuide::chercheQualifiant", standardError, 0) ;
  return "" ;
}
#else
  return string("") ;
#endif
}

SemanticSearchElement::SemanticSearchElement()
{
  _sCode     = string("") ;
  _sRelation = string("") ;
  strcpy(_sensCle, "FLECHE") ;
}

SemanticSearchElement::SemanticSearchElement(string sCode, string sRelation, DBITBLNAME sensCle, VectString *pVectString)
{
  _sCode     = sCode ;
  _sRelation = sRelation ;
  strcpy(_sensCle, sensCle) ;

  if (pVectString)
    _resultVecteurString = *pVectString ;
}

SemanticSearchElement::SemanticSearchElement(SemanticSearchElement& rv)
{
  _sCode     = rv._sCode ;
  _sRelation = rv._sRelation ;
  strcpy(_sensCle, rv._sensCle) ;

  _resultVecteurString = rv._resultVecteurString ;
}

SemanticSearchElement::~SemanticSearchElement()
{
}

bool
SemanticSearchElement::isSameSearch(const SemanticSearchElement& o)
{
  return isSameSearch(o._sCode, o._sRelation, o._sensCle) ;
}

bool
SemanticSearchElement::isSameSearch(const string sCode, const string sRelation, const DBITBLNAME sensCle)
{
  return ((_sCode     == sCode)     &&
          (_sRelation == sRelation) &&
          (strcmp(_sensCle, sensCle) == 0)) ;
}

SemanticSearchElement&
SemanticSearchElement::operator=(SemanticSearchElement src)
{
  if (&src == this)
  	return *this ;

  _sCode     = src._sCode ;
  _sRelation = src._sRelation ;
  strcpy(_sensCle, src._sensCle) ;

  _resultVecteurString = src._resultVecteurString ;

  return *this ;
}

int
SemanticSearchElement::operator==(const SemanticSearchElement& o)
{
  return ((_sCode     == o._sCode)     &&
          (_sRelation == o._sRelation) &&
          (strcmp(_sensCle, o._sensCle) == 0) &&
          (_resultVecteurString == o._resultVecteurString)) ;
}

SemanticSearchElementArray::SemanticSearchElementArray(SemanticSearchElementArray& rv)
{
  if (false == rv.empty())
    for (SemanticSearchElementIter iter = rv.begin() ; rv.end() != iter ; iter++)
      push_back(new SemanticSearchElement(**iter)) ;
}

SemanticSearchElementArray&
SemanticSearchElementArray::operator=(SemanticSearchElementArray src)
{
  if (this == &src)
    return *this ;

  vider() ;

  if (false == src.empty())
    for (SemanticSearchElementIter iter = src.begin() ; src.end() != iter ; iter++)
      push_back(new SemanticSearchElement(**iter)) ;

  return *this ;
}

VectString*
SemanticSearchElementArray::getExistingResult(string sCode, string sRelation, DBITBLNAME sensCle)
{
  if (empty())
    return (VectString*) 0 ;

  for (SemanticSearchElementIter iter = begin() ; end() != iter ; iter++)
    if ((*iter)->isSameSearch(sCode, sRelation, sensCle))
      return (*iter)->getResultVecteurString() ;

  return (VectString*) 0 ;
}

void
SemanticSearchElementArray::addResult(string sCode, string sRelation, DBITBLNAME sensCle, VectString* pVectString)
{
  if (NULL == getExistingResult(sCode, sRelation, sensCle))
    addElement(sCode, sRelation, sensCle, pVectString) ;
}

void
SemanticSearchElementArray::addElement(string sCode, string sRelation, DBITBLNAME sensCle, VectString* pVectString)
{
  if (size() >= maxSearchElement)
  {
    SemanticSearchElementIter iter = begin() ;
    delete(*iter) ;
    erase(iter) ;
  }

  push_back(new SemanticSearchElement(sCode, sRelation, sensCle, pVectString)) ;
}

SemanticSearchElementArray::~SemanticSearchElementArray()
{
  vider() ;
}

