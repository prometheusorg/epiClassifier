/*  +-----------------------------------------------------------------+  */
/*  ¦                  Programme secondaire NSDCODE                   ¦  */
/*  ¦                                                                 ¦  */
/*  ¦             Utilitaires utilisés par les décodeurs              ¦  */
/*  ¦                                                                 ¦  */
/*  ¦             Debut d'ecriture : 17 Juillet 1995 (PA)             ¦  */
/*  ¦             Derniere version : 17 Juillet 1995 (PA)             ¦  */
/*  +-----------------------------------------------------------------+  */
#include <stdlib.h>
#include <string>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "mysql/mysql.h"
#else
  #include "partage/nsdivfct.h"
  #include "nautilus/nssuper.h"
  #include "partage/nscim10.h"
  #include "partage/nsmatfic.h"
#endif

#include "nssavoir/nsguide.h"
#include "nssavoir/nsfilgd.h"
#include "nssavoir/nspatho.h"
#include "nsbb/nstlibre.h"
#include "nsbb/nspatpat.h"
#include "dcodeur/nsdkd.h"

#include "dcodeur/nsphrase.h"
#include "dcodeur/nsgenlan.h"
#include "nsepisod/eptables.h"
#include "nautilus/nscr_amx.h"
#include "nautilus/nscr_ama.h"

//-------------------------------------------------------------------------
//
//                       METHODES DE dkdNoyau
//
//-------------------------------------------------------------------------
dkdNoyau::dkdNoyau(NSContexte* pCtx) : NSRoot(pCtx)
{
  pPatPathoArray = 0;
  itDcode 	     = (PatPathoIter) 0;
  itMDcode 	     = (PatPathoIter) 0;
  codeLexique    = complement = "";

  locLesion 	   = "";

  codeLexUtil    = "";
  codeUtilValid  = false;
  codeLexDecal   = complDecal = "";

  pDkdPhrases    = 0;
  pPhraLes	     = 0;
  pPhrases	     = 0;
  sDcodeur	     = "";
  iBon		       = 1;
  iCorrigeAnnule = 0;
  bRecupFenetre  = false;
}

dkdNoyau::~dkdNoyau()
{
  // Ne surtout pas tuer la PatPathoArray, on n'en est pas propriétaire
  //
  //if (pPatPathoArray)
  //	delete pPatPathoArray;

  /*if (pPhraLes)
   	delete pPhraLes;
	if (pPhrases)
   	delete pPhrases; */
}

//-------------------------------------------------------------------------
//
//                       METHODES DE decodageBase
//
//-------------------------------------------------------------------------

//---------------------------------------------------
//  Constructeur initial (crée un noyau)
//---------------------------------------------------
decodageBase::decodageBase(NSContexte* pCtx, string sLang)
             :NSRoot(pCtx)
{
try
{
	pNoyau 	  = new dkdNoyau(pContexte);
  bCreateur = true;
  setDsFichier(true);
  pPhraseur = new NSPhraseur(pCtx);
  pPropos   = new NsProposition(pContexte, &pPhraseur, NsProposition::notSetType, NsProposition::notSetConjonct);
  sLangue   = sLang;
  //
  // Warning : pGenerateur must be instanciated, we use the french one as default
  //
  if (sLangue == "en")
  	pGenerateur = new NSGenerateurEn(pCtx, pPropos, string("en"));
  else
  	pGenerateur = new NSGenerateurFr(pCtx, pPropos, string("fr"));
}
catch (...)
{
  erreur("Exception à la création de l'objet de décodage.", standardError);
}
}

//---------------------------------------------------
//  Constructeur initial (crée un noyau)
//---------------------------------------------------
decodageBase::decodageBase(decodageBase* ptr)
             :NSRoot(ptr->pContexte)
{
try
{
	pNoyau 	  = ptr->pNoyau;
  bCreateur = false;
  pPhraseur = new NSPhraseur(ptr->pContexte);
  pPropos   = new NsProposition(pContexte, &pPhraseur, NsProposition::notSetType, NsProposition::notSetConjonct);
  sLangue   = ptr->sLangue;
  //
  // Warning : pGenerateur must be instanciated, we use the french one as default
  //
  if (sLangue == "en")
  	pGenerateur = new NSGenerateurEn(ptr->pContexte, pPropos, string("en"));
  else
  	pGenerateur = new NSGenerateurFr(ptr->pContexte, pPropos, string("fr"));
}
catch (...)
{
  erreur("Exception à la création de l'objet de décodage.", standardError);
}
}

//---------------------------------------------------
//  Destructeur
//---------------------------------------------------
decodageBase::~decodageBase()
{
	if (bCreateur)
  	delete pNoyau;

  if (pPropos)
  	delete pPropos;

	// if (pPhraseur)
  //    delete pPhraseur;

  if (pGenerateur)
  	delete pGenerateur;
}

//
// ---------- Avance d'une ou plusieurs lignes -------
//
// ATTENTION : On avance de nbPositions lignes et non de nbPositions éléments
// 				(il peut y avoir plusieurs éléments sur une même ligne)
//
//					l'itérateur reste positionné sur le 1er élément de la ligne
//					afin que getCol puisse renvoyer la bonne colonne
//
void
decodageBase::Avance(int nbPositions)
{
	if (nbPositions <= 0)
  	return;

	if ((NULL == pNoyau->pPatPathoArray) || (pNoyau->pPatPathoArray->empty()))
  	return;

	pNoyau->codeUtilValid = false;
	//
	// On avance de nbPositions lignes
	//
	// itDcode pointe sur le premier élément de la ligne
	//
	int ligne = (*(pNoyau->itDcode))->getLigne();
	int i = 0;
	while ((i < nbPositions) &&
           (pNoyau->itDcode != pNoyau->pPatPathoArray->end()))
	{
  	pNoyau->itDcode++;
    if ((pNoyau->itDcode != pNoyau->pPatPathoArray->end()) &&
      	    ((*(pNoyau->itDcode))->getLigne() != ligne))
    {
    	i++;
      ligne = (*(pNoyau->itDcode))->getLigne();
    }
	}
  if (i < nbPositions)
  {
  	pNoyau->codeLexique = "";
    return;
	}
  //
  // On est positionné sur le premier élément de la ligne, on traite toute
  // la ligne avec un autre itérateur pour laisser itDcode en 1ère position
  // (sinon getCol serait faux)
  //
  pNoyau->codeLexique = "";
  //
  // On synchronise itDcodeTemp avec itDcode
  //

	PatPathoIter itDcodeTemp = pNoyau->pPatPathoArray->begin();
	while ((*itDcodeTemp)->getLocalisation() != (*(pNoyau->itDcode))->getLocalisation())
		itDcodeTemp++;
  //
  // On remplit codeLexique tant qu'on est sur la même ligne
  //
  while ((itDcodeTemp != pNoyau->pPatPathoArray->end()) &&
             ((*itDcodeTemp)->getLigne() == ligne))
	{
		if (!(pNoyau->codeLexique == string("")))
    	pNoyau->codeLexique += string(1, cheminSeparationMARK);

    pNoyau->codeLexique += (*itDcodeTemp)->getLexique();

		if (string("") != (*itDcodeTemp)->getCertitude())
    	pNoyau->codeLexique += string(1, cheminSeparationMARK) +
                        							(*itDcodeTemp)->getCertitude();

    if (string("") != (*itDcodeTemp)->getPluriel())
    	pNoyau->codeLexique += string(1, cheminSeparationMARK) +
                            					(*itDcodeTemp)->getPluriel();
    itDcodeTemp++;
	}
}

void
decodageBase::Recule(int nbPositions)
{
	if (nbPositions <= 0)
		return;

	if (pNoyau->itDcode == pNoyau->pPatPathoArray->begin())
		return;

	pNoyau->codeUtilValid = false;
  //
  // On recule de nbPositions lignes
  //
  // itDcode pointe sur le premier élément de la ligne
  //
	int  ligne = (*(pNoyau->itDcode))->getLigne();
	int  i 		 = 0;
	bool ontourne = true;
	while ((i < nbPositions) && ontourne)
	{
  	pNoyau->itDcode--;
    if ((*(pNoyau->itDcode))->getLigne() != ligne)
    {
    	i++;
      ligne = (*(pNoyau->itDcode))->getLigne();
    }
    if (pNoyau->itDcode == pNoyau->pPatPathoArray->begin())
    	ontourne = false;
	}

	pNoyau->codeLexique = "";

	if (i < nbPositions)
		return;

	//
	// On remplit codeLexique tant qu'on est sur la même ligne
	//
	string sNouvEl = "";
	ontourne = true;
	while (((*(pNoyau->itDcode))->getLigne() == ligne) && ontourne)
	{
  	sNouvEl = (*(pNoyau->itDcode))->getLexique();

    if (string("") != (*(pNoyau->itDcode))->getCertitude())
    	sNouvEl += string(1, cheminSeparationMARK) +
      											(*(pNoyau->itDcode))->getCertitude();

    if (string("") != (*(pNoyau->itDcode))->getPluriel())
    	sNouvEl += string(1, cheminSeparationMARK) +
                  					(*(pNoyau->itDcode))->getPluriel();

    if (!(pNoyau->codeLexique == string("")))
    	pNoyau->codeLexique = string(1, cheminSeparationMARK)
         									 								+ pNoyau->codeLexique;

    pNoyau->codeLexique = sNouvEl + pNoyau->codeLexique;

    if (pNoyau->itDcode == pNoyau->pPatPathoArray->begin())
    	ontourne = false;
    else
    	pNoyau->itDcode--;
	}

	if (ontourne)
		pNoyau->itDcode++;
}

// Passe à l'élément suivant, éventuellement sur la même ligne, alors que
// la fonction Avance() passe à la ligne suivante
void
decodageBase::Suivant()
{
	pNoyau->itDcode++;
	pNoyau->codeLexique = (*(pNoyau->itDcode))->getLexique();
}

// Passe à l'élément précédent, éventuellement sur la même ligne, alors que
// la fonction Recule() se positionne au début de la ligne précédente
void
decodageBase::Precedent()
{
	pNoyau->itDcode--;
	pNoyau->codeLexique = (*(pNoyau->itDcode))->getLexique();
}

//
// ------------- Renvoie le code sens de l'élément en cours ---------
//
string*
decodageBase::getSt()
{
	if (!(pNoyau->codeUtilValid))
	{
		pContexte->getDico()->donneCodeSens(&(pNoyau->codeLexique), &(pNoyau->codeLexUtil));
		pNoyau->codeUtilValid = true;
	}

	return &(pNoyau->codeLexUtil);
}

int
decodageBase::getCol()
{
	if (pNoyau->itDcode != pNoyau->pPatPathoArray->end())
		return (*(pNoyau->itDcode))->getColonne();
	else
		return -1;
}

void
decodageBase::setPPtArray(NSPatPathoArray* pPPtArray)
{
	if (NULL == pPPtArray)
    return;

  pNoyau->pPatPathoArray = pPPtArray;
  pNoyau->itDcode = pNoyau->pPatPathoArray->begin();
}

string*
decodageBase::getCpl()
{
	pNoyau->complement = (*(pNoyau->itDcode))->getComplement();
	return &(pNoyau->complement);
}

void
decodageBase::getCert(string* pCert)
{
	if (NULL == pCert)
		return;

	*pCert = (*(pNoyau->itDcode))->getCertitude();
}

void
decodageBase::getPlur(string* pPlur)
{
	if (NULL == pPlur)
		return;

	*pPlur = (*(pNoyau->itDcode))->getPluriel();
}

void
decodageBase::MetMarque()
{
	pNoyau->itMDcode = pNoyau->itDcode;
}

void
decodageBase::VaMarque()
{
	pNoyau->itDcode = pNoyau->itMDcode;
  pNoyau->codeLexique = (*(pNoyau->itDcode))->getLexique();
  pNoyau->codeUtilValid = false;
  int ligne = (*(pNoyau->itDcode))->getLigne();
  //
  // On est positionné sur le premier élément de la ligne, on traite toute
  // la ligne avec un autre itérateur pour laisser itDcode en 1ère position
  // (sinon getCol serait faux)
  //
  pNoyau->codeLexique = "";
  //
  // On synchronise itDcodeTemp avec itDcode
  //
  PatPathoIter itDcodeTemp = pNoyau->pPatPathoArray->begin();
  while ((*itDcodeTemp)->getLocalisation() != (*(pNoyau->itDcode))->getLocalisation())
  	itDcodeTemp++;
  //
  // On remplit codeLexique tant qu'on est sur la même ligne
  //
  while ((itDcodeTemp != pNoyau->pPatPathoArray->end()) &&
   		   ((*itDcodeTemp)->getLigne() == ligne))
  {
  	if (!(pNoyau->codeLexique == string("")))
    	pNoyau->codeLexique += string(1, cheminSeparationMARK);

    pNoyau->codeLexique += (*itDcodeTemp)->getLexique();

    if (string("") != (*itDcodeTemp)->getCertitude())
    	pNoyau->codeLexique += string(1, cheminSeparationMARK) +
      																	(*itDcodeTemp)->getCertitude();

    if (string("") != (*itDcodeTemp)->getPluriel())
    	pNoyau->codeLexique += string(1, cheminSeparationMARK) +
                  											(*itDcodeTemp)->getPluriel();
    itDcodeTemp++;
  }
}

void
decodageBase::fermeFichier()
{
}

void
decodageBase::initialiseIterateurs()
{
  if ((NULL == pNoyau) || (NULL == pNoyau->pPatPathoArray) || pNoyau->pPatPathoArray->empty())
    return;

	pNoyau->itDcode  = pNoyau->pPatPathoArray->begin();
	pNoyau->itMDcode = pNoyau->pPatPathoArray->begin();

  if (pNoyau->pPatPathoArray->end() != pNoyau->itDcode) {
    pNoyau->codeLexique = (*(pNoyau->itDcode))->getLexique();
  } else {
    pNoyau->codeLexique = "";
  }
    
	set_iBon(1);
}

void
decodageBase::ouvreFichier(char *fichier)
{
}

//---------------------------------------------------------------------------
//  Fonction 	 : void metPhrase(char decal[])
//
//  Description : Met sDcodeur[] dans GASTRODATA
//
//	 Arguments : 	decDeb    -> caractères de début
//						decFin    -> caractères de fin
//						sautLigne -> 0 ligne normale
//										 1 saut d'une ligne
//										 -1 ligne collée à la suivante
//
//	 Retour	  :	Rien
//----------------- Créé le 14/11/1988  Dernière mise à jour le 16/11/1988 --
void
decodageBase::metPhrase(string decDeb, string decFin, int sautLigne)
{
}

bool
decodageBase::metUnCar(char cCar)
{
   return false;
}

void
decodageBase::vidangeDkdPhrases()
{
    NSDkdPhraseArray* pArray = getDkA();
    if (!pArray)
        return;
    if (pArray->empty())
        return;

    // On met de côté les paramètres globaux utilisés
    string BackupLocLesion = pNoyau->locLesion;
    string BackupDecodeur  = pNoyau->sDcodeur;

    // On traite les phrases
    for (NSDkdPhrArrayIter i = pArray->begin(); i != pArray->end(); i++)
    {
        pNoyau->locLesion = (*i)->locLesion;
        pNoyau->sDcodeur  = (*i)->sTexte;

        metPhrase((*i)->decDeb, (*i)->decFin, (*i)->sautLigne);
    }

    // On remet en place les paramètres globaux
    pNoyau->locLesion = BackupLocLesion;
    pNoyau->sDcodeur  = BackupDecodeur;
}

//  +-----------------------------------------------------------------+
//  ¦          Prise du code utile decal caractères plus loin         ¦
//  +-----------------------------------------------------------------+
//  Créé le 30/11/1990 Dernière mise à jour 07/11/1990
string*
decodageBase::getSt(int decal)
{
	pNoyau->itMDcode = pNoyau->itDcode;

	for (int i = 0; (i < decal) && (pNoyau->itMDcode != pNoyau->pPatPathoArray->end()); i++)
		pNoyau->itMDcode++;

	if (pNoyau->itMDcode != pNoyau->pPatPathoArray->end())
		pNoyau->codeLexDecal = (*(pNoyau->itDcode))->getLexique();
	else
		pNoyau->codeLexDecal = string("");

	return &(pNoyau->codeLexDecal);
}

//  +-----------------------------------------------------------------+
//  ¦         Prise du code complet decal caractères plus loin        ¦
//  +-----------------------------------------------------------------+
//  Créé le 30/11/1990 Dernière mise à jour 07/11/1990
string*
decodageBase::getStL(int decal)
{
	pNoyau->itMDcode = pNoyau->itDcode;

	for (int i = 0; (i < decal) && (pNoyau->itMDcode != pNoyau->pPatPathoArray->end()); i++)
		pNoyau->itMDcode++;

	if (pNoyau->itMDcode != pNoyau->pPatPathoArray->end())
		pNoyau->codeLexDecal = (*(pNoyau->itDcode))->getLexique();
	else
		pNoyau->codeLexDecal = "";

	return &(pNoyau->codeLexDecal);
}

//  +-----------------------------------------------------------------+
//  ¦         Prise de la fiche matériel de la fiche en cours         ¦
//  +-----------------------------------------------------------------+
//  Créé le 30/11/1990 Dernière mise à jour 07/11/1990
#ifndef _ENTERPRISE_DLL
DBIResult
decodageBase::getMateriel(NSMaterielInfo* pMatInfo)
{
	if (NULL == pMatInfo)
  	return DBIERR_NONE;

	pMatInfo->reset();

  if (*getSt() != "£SGMA")
    return DBIERR_NONE;

  string sCode = *getCpl();
	pMatInfo->initialiseDepuisObjet(pContexte->getSuperviseur(), sCode);

	return DBIERR_NONE;
}
#endif

//  +-----------------------------------------------------------------+
//  ¦           Prise de la fiche CDAM de la fiche en cours          ¦
//  +-----------------------------------------------------------------+
//  Créé le 12/10/1998 Dernière mise à jour 12/10/1998
#ifndef _ENTERPRISE_DLL
DBIResult
decodageBase::getCIM10(NSCim10Info* pCimInfo, NSCim10* pCim)
{
  if (NULL == pCimInfo)
    return DBIERR_NONE;

	pCimInfo->pDonnees->metAZero();

	if (*getSt() != "£CI")
		return DBIERR_NONE;

	DBIResult Erreur;

	//
	// Ouverture du fichier
	//
	bool bOuvreFerme;
	NSCim10* pCim10;

	if (pCim)
	{
		bOuvreFerme = false;
		pCim10 = pCim;
	}
	else
	{
		bOuvreFerme = true;
		pCim10 = new NSCim10(pContexte->getSuperviseur());
		if (NULL == pCim10)
			return DBIERR_NONE;

		pCim10->lastError = pCim10->open();
		if (pCim10->lastError != DBIERR_NONE)
		{
    	Erreur = pCim10->lastError;
			erreur("Erreur à l'ouverture du fichier CIM10.", standardError, pCim10->lastError);
			delete pCim10;
			return Erreur;
    }
	}
	//
	// Recherche du matériel dont le code est dans le champ complément
	//
	pCim10->lastError = pCim10->chercheClef(getCpl(),
                                          "",
                                       		0,
                                       		keySEARCHEQ,
                                       		dbiWRITELOCK);
	if (pCim10->lastError != DBIERR_NONE)
	{
  	Erreur = pCim10->lastError;
		erreur("Erreur à la recherche du code CIM10.", standardError, pCim10->lastError);
		if (bOuvreFerme)
    {
    	pCim10->close();
      delete pCim10;
    }
		return Erreur;
	}
	pCim10->lastError = pCim10->getRecord();
	if (pCim10->lastError != DBIERR_NONE)
	{
  	Erreur = pCim10->lastError;
		erreur("Erreur à la lecture du code CIM10.", standardError, pCim10->lastError);
		if (bOuvreFerme)
    {
    	pCim10->close();
      delete pCim10;
    }
		return Erreur;
	}
	//
  // Si tout a bien marché, on met à jour pMatInfo
  //
	*(pCimInfo->pDonnees) = *(pCim10->pDonnees);

	if (bOuvreFerme)
	{
		pCim10->close();
		delete pCim10;
	}

	return DBIERR_NONE;
}
#endif

//  +-----------------------------------------------------------------+
//  ¦           Prise de la fiche CCAM de la fiche en cours          ¦
//  +-----------------------------------------------------------------+
//  Créé le 03/10/2004 Dernière mise à jour 03/10/2004
#ifndef _ENTERPRISE_DLL
DBIResult
decodageBase::getCcam(NSCcamInfo* pCcamInfo, NSCcam* pCcam)
{
	if (!pCcamInfo)
		return DBIERR_NONE;

	pCcamInfo->pDonnees->metAZero();

	if (*getSt() != "6CCAM")
		return DBIERR_NONE;

	if (pCcam)
  	return pCcam->getRecordByCode(*(getCpl()), pCcamInfo, false);

  NSCcam CCAM(pContexte->getSuperviseur());
  return CCAM.getRecordByCode(*(getCpl()), pCcamInfo, true);
}
#endif

//  +-----------------------------------------------------------------+
//  ¦       Prise de la fiche correspondant de la fiche en cours      ¦
//  +-----------------------------------------------------------------+
//  Créé le 27/03/1998 Dernière mise à jour 27/03/1998

bool
decodageBase::getCorresp(NSPersonInfo* pPersonInfo)
{
#ifndef _ENTERPRISE_DLL
	// pCorInfo->pDonnees->metAZero();

	if (*getSt() != "£SGDR")
  	return false;

	string sPersonId = *getCpl();
  if (string("") == sPersonId)
		return false;

	NSPersonInfo* pPersonInfoPtr = pContexte->getPersonArray()->getPerson(pContexte, sPersonId, pidsCorresp);
  if (NULL == pPersonInfoPtr)
  	return false;

	*pPersonInfo = *pPersonInfoPtr;
#endif

	return true;
}

//---------------------------------------------------------------------------
//  Function 	 : void et_du_milieu(type, type1, type2, entre)
//
//  Description : Assemblage du type , , et : 1ère étape
//
//	 ARGUMENTS :
//
//	 Retour	  :	Rien
//
//  Créé le 21/06/1993  Dernière mise à jour le 21/06/1993
void
decodageBase::etDuMilieu(string *type, string *type1, string *type2, string entre)
{
	if ((NULL == type) || (NULL == type1) || (NULL == type2))
		return;

	if (string("") == *type2)
		return;

	if (*type != "")
		*type += entre;
	*type += *type1;
	*type1 = *type2;

	*type2 = string("");
}

//---------------------------------------------------------------------------
//  Function 	 : void et_final(type, type1, fin)
//
//  Description : Assemblage du type , , et : 2ème étape
//
//	 ARGUMENTS :
//
//	 Retour	  :	Rien
//
//  Créé le 21/06/1993  Dernière mise à jour le 21/06/1993
void
decodageBase::etFinal(string *type, string *type1, string fin)
{
  if ((NULL == type) || (NULL == type1))
    return;
	if (string("") == *type1)
		return;

	if (string("") != *type)
		*type += fin;
	*type += *type1;

  *type1 = string("");
}

//  +-----------------------------------------------------------------+
//  ¦               Décodage des données numériques                   ¦
//  +-----------------------------------------------------------------+
//  Créé le 07/11/1990 Dernière mise à jour 07/11/1990
void
decodageBase::donneDimension(int colonne, gereNum* pNum)
{
	if (NULL == pNum)
		return;

	if (getCol() <= colonne)
		return;

	string sUnite  = "";
  string sFormat = "";
  string sValeur = "";

  int iIndice = -1;

  // If it is not a numerical information, we have to come back here
  //
  MetMarque();

  // On traite les éventuelles valeurs normales
  //
	int refCol = getCol();

	while ((getCol() > colonne) && iBon())
	{
  	// Value
    //
  	if 	    ((*getSt())[0] == '£')
    {
    	sFormat = (*getitDcode())->getLexique();
      sValeur = (*getitDcode())->getComplement();
      sUnite  = (*getitDcode())->getUnit();
      Avance();

      // sFormat est du type £N0;03
      //
    	if ((sFormat == "") || (sFormat[1] != 'N'))
    	{
      	VaMarque();
      	Recupere();
   	    return;
    	}

      iIndice++;

    	pNum->instancier(&sValeur, &sUnite, &sFormat, iIndice);
    }
    //
    // Normal values
    //
    else if (*getSt() == "VNOMA")
    {
    	Avance();
      gereNum* pNormale = new gereNum(pContexte->getSuperviseur(), sLangue);
      donneDimension(refCol, pNormale);
      if (!pNum->setNormale(pNormale, iIndice))
      	delete pNormale;
    }
    else if (*getSt() == "VNOMI")
    {
    	Avance();
      gereNum* pNormInf = new gereNum(pContexte->getSuperviseur(), sLangue);
      donneDimension(refCol, pNormInf);
      if (!pNum->setNormInf(pNormInf, iIndice))
      	delete pNormInf;
    }
    else if (*getSt() == "VNOMS")
    {
    	Avance();
      gereNum* pNormSup = new gereNum(pContexte->getSuperviseur(), sLangue);
      donneDimension(refCol, pNormSup);
      if (!pNum->setNormSup(pNormSup, iIndice))
      	delete pNormSup;
    }
    //
    // Value time stamp
    //
    else if (*getSt() == "KDARE")
    {
    	Avance();
      gereDate* pDate = new gereDate(pContexte->getSuperviseur(), sLangue);
      donneDate(refCol, pDate);
      pNum->setDate(pDate);
    }
    else
    	break;
  }

  if (iIndice == -1)
  {
  	VaMarque();
  	Recupere();
  }
}

//  +-----------------------------------------------------------------+
//  ¦                     Décodage des dates                          ¦
//  +-----------------------------------------------------------------+
//  Créé le 07/11/1990 Dernière mise à jour 07/11/1990
void
decodageBase::donneDate(int colonne, gereDate* pDate)
{
	if ((getCol() <= colonne) || (NULL == pDate))
    return;

	string sUnite  = "";
	string sFormat = "";
	string sValeur = "";

  string sTemp;

  string sLexique = (*(pNoyau->itDcode))->getLexique();
  if ((string("") != sLexique) && ('£' == sLexique[0]))
  {
  	sTemp   = (*(pNoyau->itDcode))->getLexique();
    pContexte->getDico()->donneCodeSens(&sTemp, &sFormat);
    sValeur = (*(pNoyau->itDcode))->getComplement();
    sTemp   = (*(pNoyau->itDcode))->getUnit();
    pContexte->getDico()->donneCodeSens(&sTemp, &sUnite);
	}

	// sFormat est du type £D0;10 ou bien £T0;19
	if ((sValeur == "") || (sFormat == "") || ((sFormat[1] != 'D') && (sFormat[1] != 'T')))
	{
    Recupere();
  	return;
	}

	pDate->setDate(&sValeur);
	pDate->setFormat(&sFormat);
	pDate->setUnite(&sUnite);

	Avance();
}

//  +-----------------------------------------------------------------+
//  ¦                     Décodage des heures                         ¦
//  +-----------------------------------------------------------------+
//  Créé le 07/11/1990 Dernière mise à jour 07/11/1990
void
decodageBase::donneHeure(int colonne, gereHeure* pHeure)
{
	if ((getCol() <= colonne) || (NULL == pHeure))
		return;

	string sUnite  = "";
	string sFormat = "";
	string sValeur = "";

  string sTemp;

  string sLexique = (*(pNoyau->itDcode))->getLexique();
  if ((string("") != sLexique) && ('£' == sLexique[0]))
  {
  	sTemp   = (*(pNoyau->itDcode))->getLexique();
    pContexte->getDico()->donneCodeSens(&sTemp, &sFormat);
    sValeur = (*(pNoyau->itDcode))->getComplement();
    sTemp   = (*(pNoyau->itDcode))->getUnit();
    pContexte->getDico()->donneCodeSens(&sTemp, &sUnite);
  }

  // sFormat est du type £D0;03
  if ((sFormat == "") || (sFormat[1] != 'H'))
  {
  	Recupere();
    return;
  }

  pHeure->setHeure(&sValeur);
  pHeure->setFormat(&sFormat);
  pHeure->setUnite(&sUnite);

  Avance();
}

//
// Libellé issu d'une classification
// Classification : Lib and Code
//
void
decodageBase::donneCode(int colonne, gereCode* pCode)
{
	if ((getCol() <= colonne) || (NULL == pCode))
		return;

  string sClassif = string("");
  string sCode    = string("");
	NSSuper* pSuper = pContexte->getSuperviseur();

	//
  // Recuperation du code et de la classification
	// Getting code and classification
	//
	string sTemp = (*(pNoyau->itDcode))->getLexique();
	NSDico::donneCodeSens(&sTemp, &sClassif);
	sCode = (*(pNoyau->itDcode))->getComplement();

	// Il faut un code - Must have a code
  if (string("") == sCode)
	{
		Recupere();
    return;
	}
  // La classification doit etre connue - Must have a known classification
  if (false == pSuper->getFilGuide()->VraiOuFaux(sClassif, string("ES"), string("0CODE")))
	{
  	Recupere();
    return;
	}

	pCode->setCode(&sCode);
	pCode->setClassification(&sClassif);

	Avance();
}

void
decodageBase::getGereDate(gereDate** ppDate)
{
	if (sLangue == "fr")
		*ppDate = new gereDateFr(pContexte->getSuperviseur(), sLangue);
	else if (sLangue == "en")
		*ppDate = new gereDateEn(pContexte->getSuperviseur(), sLangue);
}

//  +-----------------------------------------------------------------+
//  ¦                 Donne les caractères libres                     ¦
//  +-----------------------------------------------------------------+
//  Créé le 07/11/1990 Dernière mise à jour 07/11/1990
void
decodageBase::donneCarLibre(int colonne, string* carLibre, GENRE* pGenre)
{
  if (NULL == carLibre)
    return;
  *carLibre = "";

	string sContenu = "";

	NSSuper* pSuper = pContexte->getSuperviseur();

	while ((getCol() > colonne) && pNoyau->iBon)
	{
		// Texte libre
		if (*getSt() == "£??")
    {
    	*carLibre = getTexteLibre();
      Avance();

      if (NULL != pGenre)
      	*pGenre = genreNull;
    }
    else
    {
    	NSPathologData Data;
      bool trouve = pContexte->getDico()->trouvePathologData(pGenerateur->getLang(), getStL(), &Data);
      if (trouve)
      {
      	pGenerateur->donneLibelleAffiche(carLibre, &Data);
        if (NULL != pGenre)
          Data.donneGenre(pGenre);
      }

      Avance();
    }
  }
}

/*  +-----------------------------------------------------------------+  */
/*  ¦                 Donne les caractères libres                     ¦  */
/*  +-----------------------------------------------------------------+  */
/*  Créé le 07/11/1990 Dernière mise à jour 07/11/1990                   */
void
decodageBase::Recupere()
{
#ifndef _ENTERPRISE_DLL
	if (getRecupFen())
	{
  	AideDecode* pAideDecode = new AideDecode(0, this);
    pAideDecode->Execute();
    delete pAideDecode;
	}
#endif
	pNoyau->iBon = false;
}

void
decodageBase::ajLL()
{
	if (pNoyau->itDcode == pNoyau->pPatPathoArray->end())
		return;
	if (NULL == ((*(pNoyau->itDcode))->getDataTank()))
		return;

	if (pNoyau->locLesion != "")
		pNoyau->locLesion += "/";
	pNoyau->locLesion += (*(pNoyau->itDcode))->getLocalisation();

	string sChemin = pNoyau->pPatPathoArray->donneCheminItem(pNoyau->itDcode);
	if (string("") != sChemin)
		pNoyau->locPath += string(1, '[') + sChemin + string(1, ']');
}

bool
decodageBase::CommenceParVoyelle(string* pLib)
{
	if ((NULL == pLib) || (strlen(pLib->c_str()) == 0))
		return false;

	switch ((*pLib)[0])
	{
     case 'È' ://200
     case 'É' :
     case 'Ê' :
     case 'Ë' :
     case 'é' :
     case 'e' :
     case 'E' :
     case 'ë' :
     case 'è' :
     case 'ê' :
     case 'À' :
     case 'Á' :
     case 'a' :
     case 'A' :
     case 'Â' :
     case 'Ã' :
     case 'Ä' :
     case 'Å' : //197
     case 'Æ' :
     case 'à' : //226
     case 'â' :
     case 'ã' :
     case 'ä' :
     case 'å' :
     case 'æ' :
     case 'á' :
     case 'Ì' :
     case 'Í' :
     case 'Î' :
     case 'Ï' :
     case 'í' :
     case 'i' :
     case 'I' :
     case 'î' :
     case 'ï' :
     case 'Ü' ://220
     case 'Û' :
     case 'Ú' :
     case 'Ù' : //216
     case 'ú' :
     case 'û' :
     case 'u' :
     case 'U' :
     case 'ü' :
     case 'ù' :
     case 'Õ' : //213
     case 'Ö' :
     case 'Ô' :
     case 'o' :
     case 'O' :
     case 'Ó' :
     case 'Ò' :
     case 'ò' : //242
     case 'ó' :
     case 'ô' :
     case 'õ' :
     case 'ö' :
     case 'Ý' :
     case 'y' :
     case 'Y' :
     case 'ÿ' :  return true;
	}
	return false;
}

int
decodageBase::donneCertitude(string sCert)
{
	if ((sCert == "") || (strlen(sCert.c_str()) < 5))
		return 100;
	if (string(sCert, 0, 3) != "WCE")
		return 100;
	string sScore = string(sCert, 3, 2);
	if (sScore == "ZZ")
		return -1;
	if ((sScore > "99") || (sScore < "00"))
		return 100;
	return (10 * donneDigit(sScore[0]) + donneDigit(sScore[1]));
}

string
decodageBase::intenseAdjectif(AdjIntens* pAdj1, AdjIntens* pAdj2)
{
  if ((NULL == pAdj1) || (NULL == pAdj2))
    return string("");

  string sPhrase;
  string sNom1 = "";
  string sNom2 = "";
  //
  // Cas avec au moins un non
  //
  // Cas ni ni
  //
  if ((pAdj1->sIntensite == "WCE00") && (pAdj2->sIntensite == "WCE00"))
  {
    sPhrase = "ne paraît ni " + pAdj1->sAdjectif + ", ni " + pAdj2->sAdjectif;
    return sPhrase;
  }
  //
  //
  //
  if ((pAdj1->sIntensite != "WCE00") && (pAdj1->sIntensite != ""))
  {
    if      (pAdj1->sIntensite == "WCEA0")
      sNom1 = pAdj1->sAdjectif;
    else if (pAdj1->sIntensite == "1MINI")
      sNom1 = "discrètement " + pAdj1->sAdjectif;
    else if (pAdj1->sIntensite == "1MODE")
      sNom1 = "modérément " + pAdj1->sAdjectif;
    else if (pAdj1->sIntensite == "1MOYE")
      sNom1 = "nettement " + pAdj1->sAdjectif;
    else if (pAdj1->sIntensite == "1IMPO")
      sNom1 = "très " + pAdj1->sAdjectif;
  }
  if ((pAdj2->sIntensite != "WCE00") && (pAdj2->sIntensite != ""))
  {
    if      (pAdj2->sIntensite == "WCEA0")
      sNom2 = pAdj2->sAdjectif;
    else if (pAdj2->sIntensite == "1MINI")
      sNom2 = "discrètement " + pAdj2->sAdjectif;
    else if (pAdj2->sIntensite == "1MODE")
      sNom2 = "modérément " + pAdj2->sAdjectif;
    else if (pAdj2->sIntensite == "1MOYE")
      sNom2 = "nettement " + pAdj2->sAdjectif;
    else if (pAdj2->sIntensite == "1IMPO")
      sNom2 = "très " + pAdj2->sAdjectif;
  }

  if (pAdj1->sIntensite == "WCE00")
  {
    sPhrase = "ne paraît pas " + pAdj1->sAdjectif;
    if (sNom2 != "")
      sPhrase += ", mais paraît " + sNom2;
    return sPhrase;
  }
  if (pAdj2->sIntensite == "WCE00")
  {
    sPhrase = "ne paraît pas " + pAdj2->sAdjectif;
    if (sNom1 != "")
      sPhrase += ", mais paraît " + sNom1;
    return sPhrase;
  }
  if (sNom1 != "")
  {
    sPhrase = "paraît " + sNom1;
    if (sNom2 != "")
      sPhrase += " et " + sNom2;
    return sPhrase;
  }
  if (sNom2 != "")
    sPhrase = "paraît " + sNom2;
  return sPhrase;
}

bool
decodageBase::isTexteLibre()
{
	return (*getSt() ==	"#TLI#");
}

bool
decodageBase::isChampLibre()
{
	return (*getSt() ==	"£C;");
}

bool
decodageBase::isDimension()
{
	return (isComplementAvecUnite('N'));
}

bool
decodageBase::isDate()
{
	return (isComplementAvecUnite('D'));
}

bool
decodageBase::isHeure()
{
	return (isComplementAvecUnite('H'));
}

bool
decodageBase::isCode()
{
	return (isComplementSansUnite('O'));
}

bool
decodageBase::isComplementAvecUnite(char cCategorie)
{
	int ligne = (*(pNoyau->itDcode))->getLigne();
  //
  // On synchronise itDcodeTemp avec itDcode
  //
  PatPathoIter itDcodeTemp = pNoyau->pPatPathoArray->begin();
  while ((*itDcodeTemp)->getLocalisation() != (*(pNoyau->itDcode))->getLocalisation())
  	itDcodeTemp++;
	//
  // On remplit codeLexique tant qu'on est sur la même ligne
  //
  bool bPresenceUnite = false;
  bool bPresenceValeur = false;
	string sFormat = "";

	while ((itDcodeTemp != pNoyau->pPatPathoArray->end()) &&
           ((*itDcodeTemp)->getLigne() == ligne))
	{
    string sLexique = (*itDcodeTemp)->getLexique();
    if ((string("") != sLexique) && ('£' == sLexique[0]))
    {
    	sFormat = sLexique;
      if ((*itDcodeTemp)->getComplement() != "")
      	bPresenceValeur = true;
      if ((*itDcodeTemp)->getUnit() != "")
      	bPresenceUnite = true;
      break;
    }
    itDcodeTemp++;
	}
	if (!bPresenceUnite || !bPresenceValeur)
		return false;
	if (sFormat[1] != cCategorie)
		return false;
	return true;
}

bool
decodageBase::isComplementSansUnite(char cCategorie)
{
	int ligne = (*(pNoyau->itDcode))->getLigne();
	//
	// On synchronise itDcodeTemp avec itDcode
	//
	PatPathoIter itDcodeTemp = pNoyau->pPatPathoArray->begin();
	while ((*itDcodeTemp)->getLocalisation() != (*(pNoyau->itDcode))->getLocalisation())
		itDcodeTemp++;
	//
	// On remplit codeLexique tant qu'on est sur la même ligne
	//
	bool bPresenceValeur = false;
	string sFormat = "";

	while ((itDcodeTemp != pNoyau->pPatPathoArray->end()) &&
           ((*itDcodeTemp)->getLigne() == ligne))
	{
    string sLexique = (*itDcodeTemp)->getLexique();
    if (string("") != sLexique)
    {
		  switch (sLexique[0])
      {
    	  case '£' :
      	  bPresenceValeur = true;
          sFormat = sLexique;
          break;
        default  : return false;
      }
    }
    itDcodeTemp++;
	}
	if (!bPresenceValeur)
  	return false;
	if (sFormat[1] != cCategorie)
		return false;
	return true;
}

//  -------------------------------------------------------------------
//                      Méthodes de numStorage
//  -------------------------------------------------------------------

numStorage::numStorage()
{
	pNormale  = 0;
  pNormInf  = 0;
  pNormSup  = 0;

	initToBlank();
}

void
numStorage::initToBlank()
{
	if (pNormale)
  	delete pNormale;
	pNormale  = 0;

  if (pNormInf)
  	delete pNormInf;
  pNormInf  = 0;

  if (pNormSup)
  	delete pNormSup;
  pNormSup  = 0;

  sNum      = string("");
  sNumInf   = string("");
  sNumSup   = string("");

  valeur    = 0;
  valeurInf = 0;
  valeurSup = 0;

  bExact    = false;
  bInf      = false;
  bSup      = false;
  bInfEgal  = false;
  bSupEgal  = false;

	sFormatage = string("");
  sUnite     = string("");
}

numStorage::numStorage(const numStorage& src)
{
	init(&src);
}

numStorage::~numStorage()
{
	if (NULL != pNormale)
  	delete pNormale;
  if (NULL != pNormInf)
  	delete pNormInf;
  if (NULL != pNormSup)
  	delete pNormSup;
}

void
numStorage::init(const numStorage* pSrc)
{
	if (NULL == pSrc->pNormale)
		pNormale  = 0;
  else
  	pNormale  = new gereNum(*(pSrc->pNormale));

  if (NULL == pSrc->pNormInf)
		pNormInf  = 0;
  else
  	pNormInf  = new gereNum(*(pSrc->pNormInf));

  if (NULL == pSrc->pNormSup)
		pNormSup  = 0;
  else
  	pNormSup  = new gereNum(*(pSrc->pNormSup));

  sNum      = pSrc->sNum;
  sNumInf   = pSrc->sNumInf;
  sNumSup   = pSrc->sNumSup;

  valeur    = pSrc->valeur;
  valeurInf = pSrc->valeurInf;
  valeurSup = pSrc->valeurSup;

  bExact    = pSrc->bExact;
  bInf      = pSrc->bInf;
  bSup      = pSrc->bSup;
  bInfEgal  = pSrc->bInfEgal;
  bSupEgal  = pSrc->bSupEgal;

	sFormatage = pSrc->sFormatage;
  sUnite     = pSrc->sUnite;
}

void
numStorage::setNormale(gereNum* pNorm)
{
	if (NULL != pNormale)
  	delete pNormale;

	pNormale = pNorm;
}

void
numStorage::setNormInf(gereNum* pNorm)
{
	if (NULL != pNormInf)
  	delete pNormInf;

	pNormInf = pNorm;
}

void
numStorage::setNormSup(gereNum* pNorm)
{
	if (NULL != pNormSup)
  	delete pNormSup;

	pNormSup = pNorm;
}

numStorage&
numStorage::operator =(const numStorage& src)
{
	if (this == &src)
		return *this;

  if (pNormale)
  	delete pNormale;
  if (pNormInf)
  	delete pNormInf;
  if (pNormSup)
  	delete pNormSup;

  init(&src);

  return *this;
}

//  -------------------------------------------------------------------
//                      Méthodes de gereNum
//  -------------------------------------------------------------------

gereNum::gereNum(NSSuper* pSup, string sLangue)
        :NSSuperRoot(pSup)
{
	_sLang    = sLangue;
  _pDateRef = (gereDate*) 0;
  initialise();
}

gereNum::~gereNum()
{
	if (_pDateRef)
		delete _pDateRef;
}

gereNum::gereNum(gereNum& src)
        :NSSuperRoot(src._pSuper)
{
	_aValues = src._aValues;
  _sLang   = src._sLang;

  if (NULL == src._pDateRef)
  	_pDateRef = (gereDate*) 0;
  else
  	_pDateRef = new gereDate(*(src._pDateRef));
}

gereNum&
gereNum::operator=(gereNum& src)
{
	if (this == &src)
  	return *this;

	_aValues = src._aValues;
  _sLang   = src._sLang;

  if (_pDateRef)
  	delete _pDateRef;

  if (NULL == src._pDateRef)
  	_pDateRef = (gereDate*) 0;
  else
  	_pDateRef = new gereDate(*(src._pDateRef));

	return *this;
}

//  +-----------------------------------------------------------------+
//  ¦       Donne le libellé de l'unité (singulier ou pluriel)        ¦
//  +-----------------------------------------------------------------+
//  Créé le 01/07/1997 Dernière mise à jour 01/07/1997
string
gereNum::donneLibelleUnite(NSContexte *pContexte, int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if ((NULL == pStorage) || (string("") == pStorage->sUnite))
  	return string("");

  NSPathologData Data;
  bool trouve = pContexte->getDico()->trouvePathologData(_sLang, &(pStorage->sUnite), &Data);

  if (false == trouve)
  	return string("");

  GENRE iGenre;
	Data.donneGenre(&iGenre);

	bool bPluriel = false;

	if 	  ((pStorage->bExact) && (pStorage->valeur > 1))
		bPluriel = true;
	else if ((pStorage->bInf || pStorage->bSup) &&
           ((pStorage->valeurInf > 1) || (pStorage->valeurSup > 1)))
  	bPluriel = true;

	if (bPluriel)
  	Data.donneGenrePluriel(&iGenre);

	string sUniteLibel;
	if (Data.estNom())
	{
  	if ((genreMP == iGenre) || (genreFP == iGenre) || (genreNP == iGenre))
    	Data.donneLibelleAffiche(&sUniteLibel, 1 /*declinaisonPluriel*/);
    else
    	Data.donneLibelleAffiche(&sUniteLibel, 0 /*declinaisonSingulier*/);
	}
	else
		Data.donneLibelleAffiche(&sUniteLibel, 0 /*declinaisonSingulier*/);

	return sUniteLibel;
}

//  +-----------------------------------------------------------------+
//  ¦                   (re)initialise l'objet                        ¦
//  +-----------------------------------------------------------------+
//  Créé le 01/07/1997 Dernière mise à jour 01/07/1997
void
gereNum::initialise()
{
	if (_pDateRef)
  	delete _pDateRef;
	_pDateRef = (gereDate*) 0;

  _aValues.vider();
}

//  +-----------------------------------------------------------------+
//  ¦                     Initialise l'objet                          ¦
//  +-----------------------------------------------------------------+
//  Créé le 01/07/1997 Dernière mise à jour 01/07/1997
void
gereNum::instancier(string* pNum, string* pUnit, string* pFrmt, int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice, true);
  if (NULL == pStorage)
  	return;

  pStorage->initToBlank();

	pStorage->sUnite 	   = *pUnit;
	pStorage->sFormatage = *pFrmt;

  if (*pNum == string(""))
		return;

	string sValeur = *pNum;

	size_t pos = sValeur.find("<<");
  if (pos == NPOS)
  {
  	pos = sValeur.find("[<");
    if (pos == NPOS)
    {
    	pos = sValeur.find("<[");
      if (pos == NPOS)
      {
      	pos = sValeur.find("[[");
        if (pos != NPOS)
        {
        	pStorage->bInfEgal = true;
          pStorage->bSupEgal = true;
        }
      }
      else
      	pStorage->bSupEgal = true;
    }
    else
    	pStorage->bInfEgal = true;
  }

  // cas des intervalles avec borne inf et borne sup
  if (NPOS != pos)
	{
  	string sSSValeur;

    if (pos > 0)
    {
    	sSSValeur = string(sValeur, 0, pos);
      pStorage->bInf = true;
      strToDble(&sSSValeur, &(pStorage->sNumInf), &(pStorage->valeurInf));
    }

    if (pos < strlen(sValeur.c_str()) - 2)
    {
    	sSSValeur = string(sValeur, pos + 2, strlen(sValeur.c_str())-pos-2);
      pStorage->bSup = true;
      strToDble(&sSSValeur, &(pStorage->sNumSup), &(pStorage->valeurSup));
    }
  }
  //
  // Valeur du type "<1"
  //
  else if (sValeur[0] == '<')
  {
  	sValeur = string(sValeur, 1, strlen(sValeur.c_str()) - 1);
    pStorage->bSup = true;
    strToDble(&sValeur, &(pStorage->sNumSup), &(pStorage->valeurSup));
  }
  //
  // Valeur du type "<=1"
  //
  else if (sValeur[0] == '[')
  {
  	sValeur = string(sValeur, 1, strlen(sValeur.c_str()) - 1);
    pStorage->bSup     = true;
    pStorage->bSupEgal = true;
    strToDble(&sValeur, &(pStorage->sNumSup), &(pStorage->valeurSup));
  }
  //
  // Valeur du type ">1"
  //
  else if (sValeur[0] == '>')
  {
  	sValeur = string(sValeur, 1, strlen(sValeur.c_str()) - 1);
    pStorage->bInf = true;
    strToDble(&sValeur, &(pStorage->sNumInf), &(pStorage->valeurInf));
  }
  //
  // Valeur du type ">=1"
  //
  else if (sValeur[0] == ']')
  {
  	sValeur = string(sValeur, 1, strlen(sValeur.c_str()) - 1);
    pStorage->bInf	   = true;
    pStorage->bInfEgal = true;
    strToDble(&sValeur, &(pStorage->sNumInf), &(pStorage->valeurInf));
  }
  //
  // Valeur exacte (sans intervalle)
  //
  else
  {
  	strToDble(&sValeur, &(pStorage->sNum), &(pStorage->valeur));
    pStorage->bExact = true;
  }
}

void
gereNum::strToDble(string* pNum, string* pBonNum, double* dValeur)
{
	string sEntier = "";
  string sDecima = "";

  *pBonNum = *pNum;

  size_t i = pNum->find(".");
  if (i == NPOS)
  	sEntier = *pNum;
  else
  {
  	if (i > 0)
    	sEntier = string(*pNum, 0, i);
    sDecima = string(*pNum, i+1, strlen(pNum->c_str())-i-1);

    (*pBonNum)[i] = ',';
  }
  //
  // Calcul de la valeur
  //
  *dValeur = 0;
  // Partie entière
  for (size_t i = 0; i < strlen(sEntier.c_str()); i++)
  {
  	if (!isdigit(sEntier[i]))
    	return;
    *dValeur = (*dValeur * double(10)) + double(donneDigit(sEntier[i]));
  }
  // Partie décimale
  double multi = double(1);
  for (size_t i = 0; i < strlen(sDecima.c_str()); i++)
  {
  	if (!isdigit(sDecima[i]))
    	return;
    multi = multi * 0.1;
    *dValeur += double(donneDigit(sDecima[i])) * multi;
	}
}

void
gereNum::setNum(string* nouvNum, int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice, true);
  if (NULL != pStorage)
		pStorage->sNum = *nouvNum;
}

void
gereNum::setUnite(string* nouvUnit, int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice, true);
  if (NULL != pStorage)
		pStorage->sUnite = *nouvUnit;
}

void
gereNum::setFormt(string* nouvFrmt, int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice, true);
  if (NULL != pStorage)
		pStorage->sFormatage = *nouvFrmt;
}

void
gereNum::setVal(double nouVal, int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice, true);
  if (NULL != pStorage)
		pStorage->valeur = nouVal;
}

bool
gereNum::setNormale(gereNum* pNorm, int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice, true);
  if (NULL != pStorage)
  {
		pStorage->setNormale(pNorm);
    return true;
  }
  else
  	return false;
}

bool
gereNum::setNormInf(gereNum* pNorm, int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice, true);
  if (NULL != pStorage)
  {
		pStorage->setNormInf(pNorm);
    return true;
  }
  else
  	return false;
}

bool
gereNum::setNormSup(gereNum* pNorm, int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice, true);
  if (NULL != pStorage)
  {
		pStorage->setNormSup(pNorm);
    return true;
  }
  else
  	return false;
}

string
gereNum::getNum(int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL != pStorage)
		return pStorage->sNum;
  else
  	return string("");
}

string
gereNum::getNumInf(int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL != pStorage)
		return pStorage->sNumInf;
  else
  	return string("");
}

string
gereNum::getNumSup(int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL != pStorage)
		return pStorage->sNumSup;
  else
  	return string("");
}

gereNum*
gereNum::getNorm(int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL != pStorage)
		return pStorage->getNormale();
  else
  	return NULL;
}

gereNum*
gereNum::getNormInf(int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL != pStorage)
		return pStorage->getNormInf();
  else
  	return NULL;
}

gereNum*
gereNum::getNormSup(int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL != pStorage)
		return pStorage->getNormSup();
  else
  	return NULL;
}

double
gereNum::getValeur(int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL != pStorage)
		return pStorage->valeur;
  else
  	return double(0);
}

double
gereNum::getValeurInf(int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL != pStorage)
		return pStorage->valeurInf;
  else
  	return double(0);
}

double
gereNum::getValeurSup(int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL != pStorage)
		return pStorage->valeurSup;
  else
  	return double(0);
}

bool
gereNum::estExact(int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL == pStorage)
		return false;

	return pStorage->bExact;
}

bool
gereNum::estInf(int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL == pStorage)
		return false;

	return pStorage->bInf;
}

bool
gereNum::estSup(int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL == pStorage)
		return false;

	return pStorage->bSup;
}

bool
gereNum::estInfEgal(int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL == pStorage)
		return false;

	return (pStorage->bInf && pStorage->bInfEgal) ;
}

bool
gereNum::estSupEgal(int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL == pStorage)
		return false;

	return (pStorage->bSup && pStorage->bInfEgal) ;
}

string
gereNum::getUnite(int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL == pStorage)
		return string("");

	return pStorage->sUnite;
}

string
gereNum::getFormat(int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL == pStorage)
		return string("");

	return pStorage->sFormatage;
}

bool
gereNum::estVide(int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL == pStorage)
		return true;

	return !(pStorage->bExact || pStorage->bInf || pStorage->bSup) ;
}

numStorage*
gereNum::getStorageFromIndice(int iIndice, bool bAutoCreate)
{
	if (_aValues.empty())
  {
  	if (!bAutoCreate || (iIndice > 0))
			return NULL;
    _aValues.push_back(new numStorage);
  	return _aValues.back();
  }

  IterNumStorage storeIter = _aValues.begin();
  int            i         = 0;
  for (; (storeIter != _aValues.end()) && (i < iIndice); i++, storeIter++);

  if (storeIter != _aValues.end())
  	return *storeIter;

  if (!bAutoCreate || (iIndice > i))
  	return NULL;

  _aValues.push_back(new numStorage);

  return _aValues.back();
}

void
gereNum::setDate(gereDate* pDate)
{
	if (NULL != _pDateRef)
  	delete _pDateRef;
	_pDateRef = pDate;
}

//  +-----------------------------------------------------------------+
//  ¦                   (re)initialise l'objet                        ¦
//  +-----------------------------------------------------------------+
//  Créé le 01/07/1997 Dernière mise à jour 01/07/1997
string
gereNum::getNum(string sFormatage, EXTENSIONS eExtensions, int iIndice)
{
	numStorage* pStorage = getStorageFromIndice(iIndice);
  if (NULL == pStorage) {
  	return string("");
  }

	if (eExtensions == RIEN)
  	return pStorage->sNum;

	if (eExtensions == NORMALES)
	{
  	string sRetour = pStorage->sNum;
    if ((pStorage->getNormInf() != 0) || (pStorage->getNormSup() != 0))
    {
    	string sNI = "";
      string sNS = "";
      if (pStorage->getNormInf() != 0)
      	sNI = pStorage->getNormInf()->getNum(sFormatage);
      if (pStorage->getNormSup() != 0)
      	sNS = pStorage->getNormSup()->getNum(sFormatage);

      if ((sNI != "") && (sNS != ""))
      {
      	if (sNI == sNS)
        	sRetour += " (N=" + sNI + ")";
        else
        	sRetour += " (" + sNI + "<N<" + sNS + ")";
      }
      else if (sNI != "")
      	sRetour += " (N>" + sNI + ")";
      else if (sNS != "")
      	sRetour += " (N<" + sNS + ")";
    }
    return sRetour;
	}
	return string("");
}

//  -------------------------------------------------------------------
//                      Méthodes de gereDate
//  -------------------------------------------------------------------

gereDate::gereDate(NSSuper* pSuper, string sLangue)
         :NSSuperRoot(pSuper)
{
  sLang = sLangue;
  initialise();
}

//  +-----------------------------------------------------------------+
//  ¦                     constructeur copie                          ¦
//  +-----------------------------------------------------------------+
//  Créé le 01/07/1997 Dernière mise à jour 01/07/1997
gereDate::gereDate(const gereDate& src)
         :NSSuperRoot(src._pSuper)
{
  sDate      = src.sDate;
  sFormatage = src.sFormatage;
  sLibelle   = src.sLibelle;
}

//  +-----------------------------------------------------------------+
//  ¦                   opérateur d'affectation                       ¦
//  +-----------------------------------------------------------------+
//  Créé le 01/07/1997 Dernière mise à jour 01/07/1997
gereDate&
gereDate::operator=(const gereDate& src)
{
  if (this == &src)
		return *this;

  sDate      = src.sDate;
  sFormatage = src.sFormatage;
  sLibelle   = src.sLibelle;

  return *this;
}

//  +-----------------------------------------------------------------+
//  ¦                   (re)initialise l'objet                        ¦
//  +-----------------------------------------------------------------+
//  Créé le 01/07/1997 Dernière mise à jour 01/07/1997
void
gereDate::initialise()
{
    sDate       = "";
	sFormatage  = "";
	sLibelle    = "";
}

//  +-----------------------------------------------------------------+
//  ¦               Donne la date au format JJ/MM/AAAA                ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
void
gereDate::donne_date_breve(string* pMessage, string* pIntro)
{
    *pMessage   = "";
    *pIntro     = "";

    if (sDate == "")
        return;

  	//
  	// Date au format AAAAMMJJ
	//
    if (sUnite == "2DA01")
  	{
        // Date vide
        if (sDate == "00000000")
            return;

        if (sLang == "fr")
        {
            // Juste AAAA
            if (string(sDate, 4, 4) == "0000")
            {
      	        *pMessage = string(sDate, 0, 4);
                *pIntro 	 = "en ";
            }
            // MM/AAAA
            else if (string(sDate, 6, 2) == "00")
            {
      	        *pMessage = donne_mois(string(sDate, 4, 2)) + " " + string(sDate, 0, 4);
                *pIntro 	 = "en ";
            }
            // JJ/MM/AAAA
            else
            {
                *pIntro 	 = "le ";
      	        *pMessage = string(sDate, 6, 2) + "/" + string(sDate, 4, 2) + "/" + string(sDate, 0, 4);
            }
            return;
        }
        if (sLang == "en")
        {
            // Juste AAAA
            if (string(sDate, 4, 4) == "0000")
            {
      	        *pMessage   = string(sDate, 0, 4);
                *pIntro     = "in ";
            }
            // MM/AAAA
            else if (string(sDate, 6, 2) == "00")
            {
      	        *pMessage   = donne_mois(string(sDate, 4, 2)) + " " + string(sDate, 0, 4);
                *pIntro 	= "on ";
            }
            // JJ/MM/AAAA
            else
            {
                *pIntro   = "on the ";
      	        *pMessage = string(sDate, 4, 2) + "/" + string(sDate, 6, 2) + "/" + string(sDate, 0, 4);
            }
            return;
        }
        //
        // langue non traitée : on renvoie JJ/MM/AAAA
        //
        *pMessage = string(sDate, 6, 2) + "/" + string(sDate, 4, 2) + "/" + string(sDate, 0, 4);
        return;
    }
    //
  	// Date au format AAAAMMJJhhmmss
	//
    else if (sUnite == "2DA02")
  	{
        // Date vide
        if (sDate == "00000000")
            return;

        if (sLang == "fr")
        {
            // Juste AAAA
            if (string(sDate, 4, 4) == "0000")
            {
      	        *pMessage = string(sDate, 0, 4);
                *pIntro 	 = "en ";
            }
            // MM/AAAA
            else if (string(sDate, 6, 2) == "00")
            {
      	        *pMessage = donne_mois(string(sDate, 4, 2)) + " " + string(sDate, 0, 4);
                *pIntro 	 = "en ";
            }
            // JJ/MM/AAAA
            else
            {
                *pIntro     = "le ";
      	        *pMessage   = string(sDate, 6, 2) + "/" + string(sDate, 4, 2) + "/" + string(sDate, 0, 4);

                string sHeure = "";
                // gestion de l'heure
                if      (strlen(sDate.c_str()) == 10)
                    sHeure = string(sDate, 8, 2) + string("h");
                else if (strlen(sDate.c_str()) == 12)
                    sHeure = string(sDate, 8, 2) + string(":") + string(sDate, 10, 2);
                else if (strlen(sDate.c_str()) == 14)
                    sHeure = string(sDate, 8, 2) + string(":") + string(sDate, 10, 2) + string(":") + string(sDate, 12, 2);

                if (sHeure != "")
                    *pMessage += string(" à ") + sHeure;
            }
            return;
        }
        if (sLang == "en")
        {
            // Juste AAAA
            if (string(sDate, 4, 4) == "0000")
            {
      	        *pMessage   = string(sDate, 0, 4);
                *pIntro     = "in ";
            }
            // MM/AAAA
            else if (string(sDate, 6, 2) == "00")
            {
      	        *pMessage   = donne_mois(string(sDate, 4, 2)) + " " + string(sDate, 0, 4);
                *pIntro 	= "on ";
            }
            // JJ/MM/AAAA
            else
            {
                *pIntro   = "on the ";
      	        *pMessage = string(sDate, 4, 2) + "/" + string(sDate, 6, 2) + "/" + string(sDate, 0, 4);

                string sHeure = "";
                // gestion de l'heure
                if      (strlen(sDate.c_str()) == 10)
                    sHeure = string(sDate, 8, 2) + string("h");
                else if (strlen(sDate.c_str()) == 12)
                    sHeure = string(sDate, 8, 2) + string(":") + string(sDate, 10, 2);
                else if (strlen(sDate.c_str()) == 14)
                    sHeure = string(sDate, 8, 2) + string(":") + string(sDate, 10, 2) + string(":") + string(sDate, 12, 2);

                if (sHeure != "")
                    *pMessage += string(" at ") + sHeure;
            }
            return;
        }
        //
        // langue non traitée : on renvoie JJ/MM/AAAA hh:mm:ss
        //
        *pMessage = string(sDate, 6, 2) + "/" + string(sDate, 4, 2) + "/" + string(sDate, 0, 4);

        string sHeure = "";
        // gestion de l'heure
        if      (strlen(sDate.c_str()) == 10)
            sHeure = string(sDate, 8, 2) + string(":??:??");
        else if (strlen(sDate.c_str()) == 12)
            sHeure = string(sDate, 8, 2) + string(":") + string(sDate, 10, 2) + string(":??");
        else if (strlen(sDate.c_str()) == 14)
            sHeure = string(sDate, 8, 2) + string(":") + string(sDate, 10, 2) + string(":") + string(sDate, 12, 2);

        return;
    }
  	return;
}

//  +-----------------------------------------------------------------+
//  ¦           Donne la date au format "1er janvier 1994"            ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
void
gereDate::donne_date_claire(string* pMessage, string* pIntro)
{
    unsigned int i;

    *pMessage   = "";
    *pIntro     = "";

    if (sDate == "") {
        return;
    }

  	//
  	// Date au format AAAAMMJJ
	//
  	if (sUnite == "2DA01")
  	{
        if (strlen(sDate.c_str()) < 8)
			return;

        if (sLang == "fr")
        {
            *pIntro = "le ";
            string sHeure = "";
		    //
		    // Jour
		    //
		    if (isdigit(sDate[6]) && isdigit(sDate[7]))
		    {
			    if (sDate[6] == '0')
			    {
				    if (sDate[7] == '1')
					    *pMessage = "1er ";
				    else if (sDate[7] != '0')
				    {
					    *pMessage = sDate[7];
					    *pMessage += " ";
				    }
                    else
            	        *pIntro = "en ";
			    }
			    else
			    {
				    *pMessage = sDate[6];
				    *pMessage += sDate[7];
				    *pMessage += " ";
			    }

                // gestion de l'heure
                if      (strlen(sDate.c_str()) == 10)
                    sHeure = string(sDate, 8, 2) + string(" heures");
                else if (strlen(sDate.c_str()) == 12)
                    sHeure = string(sDate, 8, 2) + string(" heures ") + string(sDate, 10, 2);
                else if (strlen(sDate.c_str()) == 14)
                    sHeure = string(sDate, 8, 2) + string(":") + string(sDate, 10, 2) + string(":") + string(sDate, 12, 2);
		    }
		    else
			    *pMessage = "?? ";
		    //
		    // Mois
		    //
            string sMois = donne_mois(string(sDate, 4, 2));
            if (sMois != "")
      	        *pMessage += sMois + " ";
            else
      	        *pMessage += "?? ";
            //
		    // Année
		    //
		    for (i = 0; i < 4; i++)
		    {
			    if (isdigit(sDate[i]))
				    *pMessage += sDate[i];
			    else
				    *pMessage += '?';
		    }

            if (sHeure != "")
                *pMessage += string(" à ") + sHeure;

            return;
        }
        else if (sLang == "en")
        {
            *pIntro = "on ";
            string sHeure = "";
            //
		    // Mois
		    //
            string sMois = donne_mois(string(sDate, 4, 2));
            if (sMois != "")
      	        *pMessage += sMois + " ";
            else
      	        *pMessage = "?? ";
		    //
		    // Jour
		    //
		    if (isdigit(sDate[6]) && isdigit(sDate[7]))
		    {
                *pMessage = " the ";
			    if (sDate[6] == '0')
			    {
				    if      (sDate[7] == '1')
					    *pMessage += "1st ";
                    else if (sDate[7] == '2')
					    *pMessage += "2nd ";
                    else if (sDate[7] == '3')
					    *pMessage += "3rd ";
				    else if (sDate[7] != '0')
				    {
					    *pMessage += sDate[7];
					    *pMessage += "th ";
				    }
                    else
            	        *pIntro = "in ";
			    }
			    else
			    {
				    *pMessage += sDate[6];
				    *pMessage += sDate[7];
				    *pMessage += "th ";
			    }
                // gestion de l'heure
                if      (strlen(sDate.c_str()) == 10)
                    sHeure = string(sDate, 8, 2);
                else if (strlen(sDate.c_str()) == 12)
                    sHeure = string(sDate, 8, 2) + string(":") + string(sDate, 10, 2);
                else if (strlen(sDate.c_str()) == 14)
                    sHeure = string(sDate, 8, 2) + string(":") + string(sDate, 10, 2) + string(":") + string(sDate, 12, 2);
		    }
		    else
			    *pMessage += "?? ";

            //
		    // Année
		    //
		    for (i = 0; i < 4; i++)
		    {
			    if (isdigit(sDate[i]))
				    *pMessage += sDate[i];
			    else
				    *pMessage += '?';
		    }

            if (sHeure != "")
                *pMessage += string(" at ") + sHeure;

            return;
        }
    }
	return;
}

//  +-----------------------------------------------------------------+
//  ¦          Donne le libellé du mois à partir de "MM"              ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
string
gereDate::donne_mois(string mois)
{
    if (strlen(mois.c_str()) < 2)
   	    return "";
    if ((!(isdigit(mois[0]))) || (!(isdigit(mois[1]))))
   	    return "";
        
    int i = 10 * donneDigit(mois[0]) + donneDigit(mois[1]);

    return donne_mois(i);
}

//  +-----------------------------------------------------------------+
//  ¦          Donne le libellé du mois à partir d'un int             ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
string
gereDate::donne_mois(int iMois)
{
    if (sLang == "fr")
    {
        switch (iMois)
        {
   	        case  1 : return "janvier";
		    case  2 : return "février";
		    case  3 : return "mars";
		    case  4 : return "avril";
		    case  5 : return "mai";
		    case  6 : return "juin";
		    case  7 : return "juillet";
		    case  8 : return "août";
		    case  9 : return "septembre";
		    case 10 : return "octobre";
		    case 11 : return "novembre";
		    case 12 : return "décembre";
        }
    }
    if (sLang == "en")
    {
        switch (iMois)
        {
   	        case  1 : return "january";
		    case  2 : return "february";
		    case  3 : return "march";
		    case  4 : return "april";
		    case  5 : return "mai";
		    case  6 : return "june";
		    case  7 : return "jully";
		    case  8 : return "august";
		    case  9 : return "september";
		    case 10 : return "october";
		    case 11 : return "november";
		    case 12 : return "december";
        }
    }
    return "";
}

//  -------------------------------------------------------------------
//                      Méthodes de gereHeure
//  -------------------------------------------------------------------

gereHeure::gereHeure(NSSuper* pSuper, string sLangue)
          :NSSuperRoot(pSuper)
{
  sLang = sLangue;
  initialise();
}

//  +-----------------------------------------------------------------+
//  ¦                     constructeur copie                          ¦
//  +-----------------------------------------------------------------+
//  Créé le 23/11/1998 Dernière mise à jour 23/11/1998
gereHeure::gereHeure(const gereHeure& src)
          :NSSuperRoot(src._pSuper)
{
  sHeure 	   = src.sHeure;
	sFormatage = src.sFormatage;
  sLibelle   = src.sLibelle;
}

//  +-----------------------------------------------------------------+
//  ¦                   opérateur d'affectation                       ¦
//  +-----------------------------------------------------------------+
//  Créé le 23/11/1998 Dernière mise à jour 01/07/1997
gereHeure&
gereHeure::operator=(const gereHeure& src)
{
  if (this == &src)
		return *this;

  sHeure     = src.sHeure;
  sFormatage = src.sFormatage;
  sLibelle   = src.sLibelle;

  return *this;
}

//  +-----------------------------------------------------------------+
//  ¦                   (re)initialise l'objet                        ¦
//  +-----------------------------------------------------------------+
//  Créé le 01/07/1997 Dernière mise à jour 01/07/1997
void
gereHeure::initialise()
{
	sHeure 	   = "";
    sFormatage = "";
	sLibelle   = "";
}

//  +-----------------------------------------------------------------+
//  ¦               Donne la date au format JJ/MM/AAAA                ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
void
gereHeure::donne_heure(string* pMessage)
{
    *pMessage = "";
  	//
  	// Heure au format HHmm
	//
  	if (sUnite == "2HE01")
  	{
        // Date vide
        if (sHeure == "0000")
      	    return;

        if (sHeure[0] == '0') {
            *pMessage = string(sHeure, 1, 1);
        } else {
            *pMessage = string(sHeure, 0, 2);
        }

   	    *pMessage += "h" + string(sHeure, 2, 2);
    }
  	return;
}

//  -------------------------------------------------------------------
//                      Méthodes de gereCode
//  -------------------------------------------------------------------

gereCode::gereCode(NSContexte* pCtx, string sLangue)
         :NSRoot(pCtx)
{
    sLang = sLangue;
    initialise();
}

//  +-----------------------------------------------------------------+
//  ¦                     constructeur copie                          ¦
//  +-----------------------------------------------------------------+
//  Créé le 24/11/1998 Dernière mise à jour 24/11/1998
gereCode::gereCode(const gereCode& src)
         :NSRoot(src.pContexte)
{
    sCode 	        = src.sCode;
	sClassification = src.sClassification;
}

//  +-----------------------------------------------------------------+
//  ¦                   opérateur d'affectation                       ¦
//  +-----------------------------------------------------------------+
//  Créé le 24/11/1998 Dernière mise à jour 24/11/1998
gereCode&
gereCode::operator=(const gereCode& src)
{
  if (this == &src)
		return *this;

  sCode 	        = src.sCode;
  sClassification = src.sClassification;

  return *this;
}

//  +-----------------------------------------------------------------+
//  ¦                   (re)initialise l'objet                        ¦
//  +-----------------------------------------------------------------+
//  Créé le 24/11/1998 Dernière mise à jour 24/11/1998
void
gereCode::initialise()
{
	sCode 	  = "";
	sClassification = "";
}

//  +-----------------------------------------------------------------+
//  ¦                 Donne le code avec son libellé                  ¦
//  +-----------------------------------------------------------------+
//  Créé le 24/11/1998 Dernière mise à jour 24/11/1998
void
gereCode::donne_code(string* pMessage, bool bAvecCode)
{
#ifndef _ENTERPRISE_DLL
  string sLibelle = "";

  NSEpiClassif DBcurseur(pContexte->getSuperviseur());

  // open table
  DBcurseur.lastError = DBcurseur.open();
  if (DBcurseur.lastError != DBIERR_NONE)
  {
    erreur("classif.db -- Erreur à l'ouverture du fichier.", standardError, DBcurseur.lastError, pContexte->GetMainWindow()->GetHandle());
    return;
  }

  // définition de la clé de recherche
  string cle = sClassification + sCode;

  // recherche dans la base
  DBcurseur.lastError = DBcurseur.chercheClef(&cle,
                                              "",
                                              NODEFAULTINDEX,
                                              keySEARCHEQ,
                                              dbiREADLOCK);
  if ((DBcurseur.lastError != DBIERR_NONE) &&
      (DBcurseur.lastError != DBIERR_RECNOTFOUND))
  {
    erreur("classif.db -- erreur à la recherche dans la base.", standardError, DBcurseur.lastError, pContexte->GetMainWindow()->GetHandle());
    DBcurseur.close();
    return;
  }

  // récupération de l'enregistrement
  DBcurseur.lastError = DBcurseur.getRecord();
  if (DBcurseur.lastError != DBIERR_NONE)
  {
    erreur("classif.db -- erreur de lecture dans la base.", standardError, DBcurseur.lastError, pContexte->GetMainWindow()->GetHandle());
    DBcurseur.close();
    return;
  }

  if (bAvecCode)
    sLibelle = sCode + string(" (") + DBcurseur.getLabel() + string(")");
  else
    sLibelle = DBcurseur.getLabel();

  // on ferme la base
  DBcurseur.lastError = DBcurseur.close();
  if (DBcurseur.lastError != DBIERR_NONE)
    erreur("classif.db -- erreur à la fermeture de la base.", standardError, DBcurseur.lastError, pContexte->GetMainWindow()->GetHandle());

  *pMessage = sLibelle;
#endif

  return;
}

#ifndef _ENTERPRISE_DLL
//------------------------------------------------------------------------//
//										TreeView												  //
//------------------------------------------------------------------------//

DEFINE_RESPONSE_TABLE1(AideDecode, TDialog)
  EV_COMMAND(IDC_ANNUL,   CmAnnule),
  EV_COMMAND(IDC_CORRIGER, CmCorrige),
  EV_COMMAND(IDC_AUTOMATIQUE, CmAutomatique),
END_RESPONSE_TABLE;

AideDecode::AideDecode(TWindow* pere,  decodageBase* pDecode)
           :TDialog(pere, IDD_DECODE, pNSDLLModule), NSRoot(pDecode->pContexte)
{
	_pImages = (OWL::TImageList*) 0;
  _Aparent = pere;
  // set client window style
  //
  uint32 style = GetStyle();
  SetStyle(style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

  // set treeview window style
  //
  style = TVS_HASLINES | TVS_HASBUTTONS;
#if defined(BI_PLAT_WIN32)
	style |= TVS_LINESATROOT;
#endif

	_pDecodageBase = pDecode;
	_pTreeWind = new TTreeWindow(this, IDC_TREEV_DECODE, pNSDLLModule);
}


AideDecode::~AideDecode()
{
  if (_pImages)
    delete _pImages;

  if (_pTreeWind)
   	delete _pTreeWind;
}

void
AideDecode::CmCorrige()
{
	_pDecodageBase->setCorrAnnu(iCorrige);
	CloseWindow(IDOK);
}

void
AideDecode::CmAnnule()
{
  _pDecodageBase->setCorrAnnu(iAnnule);
	CloseWindow(IDOK);
}

void
AideDecode::CmAutomatique()
{
  _pDecodageBase->setCorrAnnu(iAutomatique);

  NSPatPathoArray* pPatPathoArray = _pDecodageBase->getPPtArray();
  if (pPatPathoArray->empty())
  {
    CloseWindow(IDOK);
    return;
  }
  PatPathoIter iterTReeView = pPatPathoArray->begin();
  //
  // On passe en revue la PatPathoArray jusqu'à l'élément fautif
  //
	while (pPatPathoArray->end() != iterTReeView)
  {
    if ((*iterTReeView) == *(_pDecodageBase->getitDcode()))
      break;
    iterTReeView++;
  }
  if (pPatPathoArray->end() != iterTReeView)
    pPatPathoArray->SupprimerItem(iterTReeView);

  CloseWindow(IDOK);
}

void
AideDecode::Cmok()
{
  CloseWindow(IDOK);
}

void
AideDecode::SetupWindow()
{
	TDialog::SetupWindow();

/*   for(int j = IDB_NORMAL; j<= IDB_ERREUR; j++)
   {
   	TBitmap img(*pNSDLLModule, j);
   	if(!Images)
   	{
   		TSize ImgSize = TSize(img.Width(), img.Height());
      	Images = new TImageList(ImgSize, true, 2, 2);
      	Images->SetBkColor(RGB(0xff, 0, 0));
   	}
		Images->Add(img);
   }
*/

 	NSPatPathoArray* pPatPathoArray = _pDecodageBase->getPPtArray();
 	if ((NULL == pPatPathoArray) || pPatPathoArray->empty())
 		return;

	string sLang = string("");
	if (pContexte)
		sLang = pContexte->getUserLanguage();

	_pImages = new TImageList(NS_CLASSLIB::TSize(20, 16), ILC_COLOR4, 15, 5);
	_pImages->Add(OWL::TBitmap(*pNSDLLModule, IDB_NORMAL));
	_pImages->Add(OWL::TBitmap(*pNSDLLModule, IDB_ERREUR));
	_pImages->Add(OWL::TBitmap(*pNSDLLModule, IDB_QUESTION));
	_pTreeWind->SetImageList(TTreeWindow::Normal, *_pImages);

	VectTTreeNode* pVectTTreeNode = new VectTTreeNode;

	string      sEtiquettePatPere; //étiquette du  père
	string      sLibellePatPere;
	string      sEtiquettePatFils; //étiquette du  fils
	string      sLibellePatFils;
  string			sUnit;
  string			sUnitLib;
	int         colonne;
	NSSuper*    pSuper = pContexte->getSuperviseur();

	PatPathoIter iterTReeView   = pPatPathoArray->begin();
	PatPathoIter dernierElement = pPatPathoArray->begin();

	//traitement pour GECHY
/*
	sEtiquettePatPere = (*iterTReeView)->getLexique();
	pSuper->getDico()->donneLibelle(sLang, &sEtiquettePatPere, &sLibellePatPere);

	string sComplement;
	if ((*iterTReeView)->getComplement() == string(""))
		sComplement = string("");
	else
		sComplement = string(1, cheminSeparationMARK) +
      		                    (*iterTReeView)->getComplement();

	string sCertitudeProvisoir;
	if ((*iterTReeView)->getCertitude() == string(""))
		sCertitudeProvisoir = string("");
	else
		sCertitudeProvisoir = string(1, cheminSeparationMARK) +
      		                    			(*iterTReeView)->getCertitude();

	string sPlurielProvisoir;
	if ((*iterTReeView)->getPluriel() == string(""))
		sPlurielProvisoir = string("");
	else
		sPlurielProvisoir = string(1, cheminSeparationMARK) +
                                		(*iterTReeView)->getPluriel();


	// Traiter le cas où il ya : certitude et/ou pluriel
	sEtiquettePatPere = sEtiquettePatPere + sPlurielProvisoir + sCertitudeProvisoir + sComplement;
	sLibellePatPere += sPlurielProvisoir + sCertitudeProvisoir + sComplement;
*/

  sLibellePatPere = buildLabel(iterTReeView);

	//NSTreeNode ayant pour étiquette sEtiquettePatPere et dont la colonne est 1
	TTreeNode root  = _pTreeWind->GetRoot();
	TTreeNode GECHY = root.AddChild(TTreeNode(*_pTreeWind, sLibellePatPere.c_str()));

	if ((*iterTReeView) != *(_pDecodageBase->getitDcode()))
	{
		GECHY.SetImageIndex(0);
		GECHY.SetSelectedImageIndex(0, true);
	}
	else
	{
		GECHY.SetImageIndex(1);
		GECHY.SetSelectedImageIndex(1, true);
	}
	iterTReeView++;

	int refLigne;
	int imageIndex = 0;
	//
	// On passe en revue la PatPathoArray
	//
	while (pPatPathoArray->end() != iterTReeView)
	{
  	//
    // Element situé juste sous la racine
    //
    if ((*iterTReeView)->getColonne() == (ORIGINE_PATH_PATHO + 1))
    {
    	//
      // On vide le vecteur de pères
      //
      pVectTTreeNode->vider();
      dernierElement = iterTReeView;

      sEtiquettePatPere = "";
      refLigne = (*iterTReeView)->getLigne();
      //
      // On assemble tous les éléments situés sur la même ligne
      //
      while ((iterTReeView != pPatPathoArray->end()) &&
         		        (refLigne == (*iterTReeView)->getLigne()))
      {
      	//
        // On fabrique l'étiquette
        //
        if (string("") != sEtiquettePatPere)
          sEtiquettePatPere += string(" | ");

        sEtiquettePatPere = buildLabel(iterTReeView);

        //
        // On regarde si l'élément est l'élément fautif
        //
        if ((*iterTReeView) == *(_pDecodageBase->getitDcode()))
        	imageIndex = 1;

        //
        // On avance d'un cran
        //
        if (pPatPathoArray->end() != iterTReeView)
        	iterTReeView++;
      }
      //
      // On ajoute l'étiquette à la treenode (sous la racine)
      //
      TTreeNode* fils = new TTreeNode(*_pTreeWind, sEtiquettePatPere.c_str());
      *fils = GECHY.AddChild(TTreeNode(*_pTreeWind, sEtiquettePatPere.c_str()));
      //
      // On référence le noeud comme futur père
      //
      pVectTTreeNode->push_back(fils);
      //
      // On fixe son icone
      //
      fils->SetImageIndex(imageIndex);
      fils->SetSelectedImageIndex(imageIndex, true);

      if (1 == imageIndex)
      	imageIndex++;
    }
    //
    // Elément ordinaire (situé à distance de la racine)
    //
    else
    {
    	//
      // On ôte de l'array de pères tous les éléments qui ne sont
      // pas plus anciens que l'élément en cours
      //
      colonne = (*iterTReeView)->getColonne();
      int derniereColonne = (*dernierElement)->getColonne();

      if (colonne <= derniereColonne)
      {
        // Take care, if columns are badly set, not to pop_back all fathers
        //
        size_t iDelta = derniereColonne - colonne + 1;
        size_t iVect  = pVectTTreeNode->size();
        size_t iBack  = min(iDelta, iVect - 1);

      	for (int i = 0; i < iBack; i++)
        	pVectTTreeNode->pop_back();
      }

      dernierElement = iterTReeView;
      //
      //
      sEtiquettePatPere = "";
      refLigne = (*iterTReeView)->getLigne();
      //
      // On assemble tous les éléments situés sur la même ligne
      //
      while ((iterTReeView != pPatPathoArray->end()) &&
         		        (refLigne == (*iterTReeView)->getLigne()))
      {
      	//
        // On fabrique l'étiquette
        //
        if (string("") != sEtiquettePatPere)
          sEtiquettePatPere += string(" | ");

        sEtiquettePatPere = buildLabel(iterTReeView);

/*
        if (!(sEtiquettePatPere == string("")))
        {
        	sEtiquettePatPere += string(1, cheminSeparationMARK);
          sEtiquettePatPere += (*iterTReeView)->getLexique();
        }
        else
        {
        	sLibellePatPere = (*iterTReeView)->getLexique();

          bool bSuccess = false;
          if ((string("") != sLibellePatPere) && ('£' != sLibellePatPere[0]))
          	bSuccess = pSuper->getDico()->donneLibelle(sLang, &sLibellePatPere, &sEtiquettePatPere);
          if (false == bSuccess)
          	sEtiquettePatPere = sLibellePatPere;
        }

        if (string("") != (*iterTReeView)->getCertitude())
        	sEtiquettePatPere += string(1, cheminSeparationMARK) +
      																	(*iterTReeView)->getCertitude();

        if (string("") != (*iterTReeView)->getPluriel())
        	sEtiquettePatPere += string(1, cheminSeparationMARK) +
                  			    						(*iterTReeView)->getPluriel();

        if (string("") != (*iterTReeView)->getComplement())
        	sEtiquettePatPere += string("(") +
               				    				(*iterTReeView)->getComplement() +
                                        string(")");
        if ((*iterTReeView)->getUnit() != string(""))
        {
        	sUnit = (*iterTReeView)->getUnit();
          pSuper->getDico()->donneLibelle(sLang, &sUnit, &sUnitLib);
          sEtiquettePatPere += string(" ") + sUnitLib;
        }
*/
        //
        // On regarde si l'élément est l'élément fautif
        //
        if ((*iterTReeView) == *(_pDecodageBase->getitDcode()))
        	imageIndex = 1;
        //
        // On avance d'un cran
        //
        if (pPatPathoArray->end() != iterTReeView)
        	iterTReeView++;
      }
      //
      // On prend comme père le plus jeune des aïeuls
      //
      TTreeNode* pere = pVectTTreeNode->back(); //dernier élément de pVectTTreeNode
      TTreeNode* fils = new TTreeNode(*_pTreeWind, sEtiquettePatPere.c_str());
      //
      // L'élément est ajouté à l'array des pères
      //
      *fils = pere->AddChild(TTreeNode(*_pTreeWind, sEtiquettePatPere.c_str()));
      pVectTTreeNode->push_back(fils );
      //
      // On fixe son icone
      //
      fils->SetImageIndex(imageIndex);
      fils->SetSelectedImageIndex(imageIndex, true);

      if (imageIndex == 1)
      {
      	imageIndex++;
        fils->EnsureVisible();
      }
    }
  }
  delete pVectTTreeNode;
  pVectTTreeNode = 0;
}

string
AideDecode::buildLabel(PatPathoIter iterTReeView)
{
  string sLang = string("");
	if (pContexte)
		sLang = pContexte->getUserLanguage();

  string sEtiquettePatPere = (*iterTReeView)->getLexique();

  string sLibellePatPere = string("");
  bool bSuccess = false;
  if ((string("") != sEtiquettePatPere) && ('£' != sEtiquettePatPere[0]))
    bSuccess = pContexte->getDico()->donneLibelle(sLang, &sEtiquettePatPere, &sLibellePatPere);
  if (false == bSuccess)
    sLibellePatPere = sEtiquettePatPere;

	string sComplement = string("");
	if ((*iterTReeView)->getComplement() != string(""))
		sComplement = string(1, cheminSeparationMARK) +
      		                                (*iterTReeView)->getComplement();

	string sCertitudeProvisoir = string("");
	if ((*iterTReeView)->getCertitude() != string(""))
		sCertitudeProvisoir = string(1, cheminSeparationMARK) +
      		                    			        (*iterTReeView)->getCertitude();

	string sPlurielProvisoir = string("");
	if ((*iterTReeView)->getPluriel() != string(""))
		sPlurielProvisoir = string(1, cheminSeparationMARK) +
                                		         (*iterTReeView)->getPluriel();

	sLibellePatPere += sPlurielProvisoir + sCertitudeProvisoir + sComplement;

  return sLibellePatPere;
}

#endif

AdjIntens::AdjIntens()
{
	sAdjectif  = "";
	sIntensite = "";
}

////////////////// NSDkdPhrase ////////////////////

NSDkdPhrase::NSDkdPhrase()
{
  categorie  = -1;
  sTexte     = "";
  sEtiquette = "";
  decDeb     = "";
  decFin     = "";
  sautLigne  = -1;
  locLesion  = "";
}

NSDkdPhrase::NSDkdPhrase(NSDkdPhrase& rv)
{
  categorie  = rv.categorie;
  sTexte     = rv.sTexte;
  sEtiquette = rv.sEtiquette;
  decDeb     = rv.decDeb;
  decFin     = rv.decFin;
  sautLigne  = rv.sautLigne;
  locLesion  = rv.locLesion;
}

NSDkdPhrase::~NSDkdPhrase()
{
}

NSDkdPhrase&
NSDkdPhrase::operator=(NSDkdPhrase src)
{
	if (this == &src)
		return *this;

	categorie  = src.categorie;
  sTexte     = src.sTexte;
  sEtiquette = src.sEtiquette;
  decDeb     = src.decDeb;
  decFin     = src.decFin;
  sautLigne  = src.sautLigne;
  locLesion  = src.locLesion;

	return *this;
}

int
NSDkdPhrase::operator == (const NSDkdPhrase& o)
{
	if ((sTexte     == o.sTexte) &&
      (sEtiquette == o.sEtiquette))
		return 1;
	return 0;
}





