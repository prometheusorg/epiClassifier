#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "mysql/mysql.h"
#else
  #include "partage\nsdivfct.h"
  #include "nautilus\nssuper.h"
#endif

#include "nssavoir/nspatho.h"
#include "nssavoir/nsfilgd.h"
#include "dcodeur/nsgen.h"
#include "dcodeur/nsphrase.h"
#include "dcodeur/nsdkd.h"

// -------------------------------------------------------------------------
// ---------------------- METHODES DE generateur ---------------------------
// -------------------------------------------------------------------------
NSGenerateur::NSGenerateur(NSContexte* pCtx, NsProposition* pPropos, string sLangue)
             :NSRoot(pCtx)
{
  // pPhraseur->nettoye();
  pCCArray     = new NSGenereCompArray(pCtx, this);
  pProposition = pPropos;
  initialise();

  sPhrase      = "";
  sLang        = sLangue;
  pPh          = 0;
  if (pProposition && (NsProposition::isPhraseur == pProposition->iObjectType))
    pPh = (static_cast<NSPhraseur*>(pProposition->pProposition));
}

NSGenerateur::NSGenerateur(NSGenerateur& rv)
             :NSRoot(rv.pContexte)
{
  pCCArray = new NSGenereCompArray(*(rv.pCCArray));
  initialise();
  sPhrase = "";
  copieTout(&rv);
}

NSGenerateur::~NSGenerateur()
{
  delete pCCArray;

  if (pAdjEpitheteAvPos)
      delete pAdjEpitheteAvPos;
  if (pAdjEpitheteAvNeg)
      delete pAdjEpitheteAvNeg;
  if (pAdjEpitheteApPos)
      delete pAdjEpitheteApPos;
  if (pAdjEpitheteApNeg)
      delete pAdjEpitheteApNeg;
  if (pCompNomPos)
      delete pCompNomPos;
  if (pCompNomNeg)
      delete pCompNomNeg;
  if (pAdverbePos)
      delete pAdverbePos;
  if (pAdverbeNeg)
      delete pAdverbeNeg;
  if (pCODPos)
      delete pCODPos;
  if (pCODNeg)
      delete pCODNeg;
  if (pAttSujetPos)
      delete pAttSujetPos;
  if (pAttSujetNeg)
      delete pAttSujetNeg;
  if (pAttCODPos)
      delete pAttCODPos;
  if (pAttCODNeg)
      delete pAttCODNeg;
}

void
NSGenerateur::initialise()
{
  pAdjEpitheteAvPos = 0;
  pAdjEpitheteAvNeg = 0;
  pAdjEpitheteApPos = 0;
  pAdjEpitheteApNeg = 0;
  pCompNomPos       = 0;
  pCompNomNeg       = 0;
  pAdverbePos       = 0;
  pAdverbeNeg       = 0;
  pCODPos           = 0;
  pCODNeg           = 0;
  pAttSujetPos      = 0;
  pAttSujetNeg      = 0;
  pAttCODPos        = 0;
  pAttCODNeg        = 0;

  pCCArray->initialise();
}

void
NSGenerateur::reinitialise()
{
  if (pAdjEpitheteAvPos)
      delete pAdjEpitheteAvPos;
  if (pAdjEpitheteAvNeg)
      delete pAdjEpitheteAvNeg;
  if (pAdjEpitheteApPos)
      delete pAdjEpitheteApPos;
  if (pAdjEpitheteApNeg)
      delete pAdjEpitheteApNeg;
  if (pCompNomPos)
      delete pCompNomPos;
  if (pCompNomNeg)
      delete pCompNomNeg;
  if (pAdverbePos)
      delete pAdverbePos;
  if (pAdverbeNeg)
      delete pAdverbeNeg;
  if (pCODPos)
      delete pCODPos;
  if (pCODNeg)
      delete pCODNeg;
  if (pAttSujetPos)
      delete pAttSujetPos;
  if (pAttSujetNeg)
      delete pAttSujetNeg;
  if (pAttCODPos)
      delete pAttCODPos;
  if (pAttCODNeg)
      delete pAttCODNeg;

  initialise();
}

void
NSGenerateur::copieTout(NSGenerateur* rv)
{
  if (rv->pProposition)
  {
    pProposition = new NsProposition(*(rv->pProposition));
    if (pProposition->iObjectType == NsProposition::isPhraseur)
      pPh = (static_cast<NSPhraseur*>(pProposition->pProposition));
  }
  else
    pProposition = 0;

  sPhrase = rv->sPhrase;
  sLang   = rv->sLang;

  if (rv->pAdjEpitheteAvPos)
    pAdjEpitheteAvPos = new NSPhraseMotArray(*(rv->pAdjEpitheteAvPos));
  else
    pAdjEpitheteAvPos = 0;

    if (rv->pAdjEpitheteAvNeg)
        pAdjEpitheteAvNeg = new NSPhraseMotArray(*(rv->pAdjEpitheteAvNeg));
    else
        pAdjEpitheteAvNeg = 0;

    if (rv->pAdjEpitheteApPos)
        pAdjEpitheteApPos = new NSPhraseMotArray(*(rv->pAdjEpitheteApPos));
    else
        pAdjEpitheteApPos = 0;

    if (rv->pAdjEpitheteApNeg)
        pAdjEpitheteApNeg = new NSPhraseMotArray(*(rv->pAdjEpitheteApNeg));
    else
        pAdjEpitheteApNeg = 0;

    if (rv->pCompNomPos)
        pCompNomPos = new NSPhraseMotArray(*(rv->pCompNomPos));
    else
        pCompNomPos = 0;

    if (rv->pCompNomNeg)
        pCompNomNeg = new NSPhraseMotArray(*(rv->pCompNomNeg));
    else
        pCompNomNeg = 0;

    if (rv->pAdverbePos)
        pAdverbePos = new NSPhraseMotArray(*(rv->pAdverbePos));
    else
        pAdverbePos = 0;

    if (rv->pAdverbeNeg)
        pAdverbeNeg = new NSPhraseMotArray(*(rv->pAdverbeNeg));
    else
        pAdverbeNeg = 0;

    if (rv->pCODPos)
        pCODPos = new NSPhraseMotArray(*(rv->pCODPos));
    else
        pCODPos = 0;

    if (rv->pCODNeg)
        pCODNeg = new NSPhraseMotArray(*(rv->pCODNeg));
    else
        pCODNeg = 0;

    if (rv->pAttSujetPos)
        pAttSujetPos = new NSPhraseMotArray(*(rv->pAttSujetPos));
    else
        pAttSujetPos = 0;

    if (rv->pAttSujetNeg)
        pAttSujetNeg = new NSPhraseMotArray(*(rv->pAttSujetNeg));
    else
        pAttSujetNeg = 0;

    if (rv->pAttCODPos)
        pAttCODPos = new NSPhraseMotArray(*(rv->pAttCODPos));
    else
        pAttCODPos = 0;

    if (rv->pAttCODNeg)
        pAttCODNeg = new NSPhraseMotArray(*(rv->pAttCODNeg));
    else
        pAttCODNeg = 0;


    *pCCArray         = *(rv->pCCArray);
}

bool
NSGenerateur::genereProposition(DCODETYPE iStyle, NsProposition* pPropos)
{
  NsProposition* pCurrentProp;
  if (pPropos)
    pCurrentProp = pPropos;
  else
    pCurrentProp = pProposition;

  if (NULL == pCurrentProp)
    return false;
try
{
  pCurrentProp->sPhrase = string("");
  pCurrentProp->sLang   = sLang;

  //
  // Proposition isolee : on se contente de la decoder
  // Lone proposition : we just generate it
  //
  if (NsProposition::isPhraseur == pCurrentProp->iObjectType)
  {
    pPh = (static_cast<NSPhraseur*>(pCurrentProp->pProposition));
    bool bResult = generePhrase(iStyle);
    if (bResult)
      pCurrentProp->sPhrase = sPhrase;
    return bResult;
  }
  //
  // Propositions multiples : on decode, puis on assemble
  // Multiple propositions : we generate each one, then we build the sentence
  //
  if (NsProposition::isPropositionArray == pCurrentProp->iObjectType)
  {
    NSPropositionArray* pPropArray = (static_cast<NSPropositionArray*>(pCurrentProp->pProposition));
    if (pPropArray->empty())
      return true;
    for (iterProposition i = pPropArray->begin(); pPropArray->end() != i; i++)
    {
      bool bResult = genereProposition(iStyle, *i);
      if (false == bResult)
        return false;
    }
    return assembleProposition(iStyle, pCurrentProp);
  }
  return false;
}
catch (...)
{
	erreur("Exception NSGenerateurFr::generePhrase.", standardError);
  return false;
}
}

bool
NSGenerateur::ajouteMot(NSPhraseMotArray** ppMotArray, NSPhraseMot* pMot)
{
try
{
	if (NULL == pMot)
  	return true;

	if (NULL == *ppMotArray)
  	*ppMotArray = new NSPhraseMotArray();

	(*ppMotArray)->push_back(pMot);

	return true;
}
catch (...)
{
	erreur("Exception NSGenerateur::ajouteMot.", standardError);
	return false;
}
}

bool
NSGenerateur::ajouteCopieMot(NSPhraseMotArray** ppMotArray, NSPhraseMot* pMot)
{
try
{
  if (NULL == pMot)
    return true;

  if (NULL == *ppMotArray)
    *ppMotArray = new NSPhraseMotArray();

  //
  // NSPhraseMotTime ?
  //
  NSPhraseMotTime* pDate = dynamic_cast<NSPhraseMotTime*>(pMot);
  if (pDate)
  {
    NSPhraseMotTime *pNewDate = new NSPhraseMotTime(*pDate);
    (*ppMotArray)->push_back(pNewDate);
  }
  else
  {
    NSPhraseMotTimeCycle* pCycle = dynamic_cast<NSPhraseMotTimeCycle*>(pMot);
    if ( pCycle )
    {
      NSPhraseMotTimeCycle *pNewCycle = new NSPhraseMotTimeCycle(*pCycle);
      (*ppMotArray)->push_back(pNewCycle);
    }
    else
      (*ppMotArray)->push_back(new NSPhraseMot(*pMot));
  }

  return true;
}
catch (...)
{
	erreur("Exception NSGenerateur::ajouteCopieMot.", standardError);
	return false;
}
}

bool
NSGenerateur::terminePhrase()
{
  if (string("") != sPhrase)
  {
    sPhrase[0] = pseumaj(sPhrase[0]);
    sPhrase += string(".");
  }
  return true;
}

bool
NSGenerateur::termineProposition(NsProposition* pPropos)
{
  string* pPhrase = 0;
  if (NULL != pPropos)
    pPhrase = &(pPropos->sPhrase);
  else
    pPhrase = &(pProposition->sPhrase);

  if ((NULL != pPhrase) && (string("") != *pPhrase))
  {
    (*pPhrase)[0] = pseumaj((*pPhrase)[0]);
    *pPhrase += string(".");
  }
  return true;
}

bool
NSGenerateur::CommenceParVoyelle(string* pLib)
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

void
NSGenerateur::etDuMilieu(string *type, string *type1, string *type2)
{
	if ((NULL == type) || (NULL == type1) || (NULL == type2) || (string("") == *type2))
		return;
	if (string("") != *type)
		*type += string(", ");
	*type += *type1;
	*type1 = *type2;
	*type2 = string("");
}

void
NSGenerateur::contracting(string *pSentence, string sFrom, string sTo)
{
  if ((NULL == pSentence) || (string("") == *pSentence) || (string("") == sFrom))
    return;

  size_t posit1 = pSentence->find(sFrom);
  if (NPOS == posit1)
    return;

  size_t iFromSize = strlen(sFrom.c_str());

  while (NPOS != posit1)
  {
    if (0 == posit1)
      pSentence->replace(0, iFromSize, sTo);
    else if (' ' == (*pSentence)[posit1-1])
      pSentence->replace(posit1, iFromSize, sTo);

    posit1 = pSentence->find(sFrom, posit1 + 1);
  }
}

//
// Dispatch an array of NSPhraseMot into an array of affirmative elements
// and an array of negative elements
//
// Can also dispatch only elements which are of proper type
//
void
NSGenerateur::dispatchArray(NSPhraseMotArray* pMots, NSPhraseMotArray** ppYesArray, NSPhraseMotArray** ppNoArray, WORDTYPE iWordType)
{
  if (NULL == pMots)
    return;

  if (pMots->empty())
    return;

  for (iterPhraseMot iterMots = pMots->begin(); pMots->end() != iterMots; iterMots++)
  {
    bool bFound = true;

    if (false == (*iterMots)->estTexteLibre())
    {
      NSSuper* pSuper = pContexte->getSuperviseur();

      string sLexique = (*iterMots)->getLexique();

      NSPathologData Data;
      bFound = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data);
      if (false == bFound)
      {
        string sErrorText = string("NSGenerateur::dispatchArray discarding ") + sLexique + string(" which cannot be found in Lexique for langage ") + sLang;
        pSuper->trace(&sErrorText, 1, NSSuper::trWarning);
      }
      else
      {
        switch(iWordType)
        {
          case isNoun :
          {
            if (false == Data.estNom())
            {
              string sErrorText = string("NSGenerateurFr::classeTout discarding ") + sLexique + string(" because it is no a noun.");
              pSuper->trace(&sErrorText, 1, NSSuper::trWarning);
              bFound = false;
            }
            break;
          }
          case isAdjective :
          {
            if (false == Data.estAdjectif())
            {
              string sErrorText = string("NSGenerateurFr::classeTout discarding ") + sLexique + string(" because it is not an adjective.");
              pSuper->trace(&sErrorText, 1, NSSuper::trWarning);
              bFound = false;
            }
            break;
          }
          case isAdverb :
          {
            if (false == Data.estAdverbe())
            {
              string sErrorText = string("NSGenerateurFr::classeTout discarding ") + sLexique + string(" because it is not an adverb.");
              pSuper->trace(&sErrorText, 1, NSSuper::trWarning);
              bFound = false;
            }
            break;
          }
          case isVerb :
          {
            if (false == Data.estVerbe())
            {
              string sErrorText = string("NSGenerateurFr::classeTout discarding ") + sLexique + string(" because it is not a verb.");
              pSuper->trace(&sErrorText, 1, NSSuper::trWarning);
              bFound = false;
            }
            break;
          }
          case isInvar :
          {
            if (false == Data.estInvariable())
            {
              string sErrorText = string("NSGenerateurFr::classeTout discarding ") + sLexique + string(" because it is not an invariable.");
              pSuper->trace(&sErrorText, 1, NSSuper::trWarning);
              bFound = false;
            }
            break;
          }
        }
      }
    }

    if (bFound)
      dispatchYesNo(*iterMots, ppYesArray, ppNoArray);
  }
}

//
// Dispatch a NSPhraseMot into the array of affirmative elements
// or the array of negative elements
//
void
NSGenerateur::dispatchYesNo(NSPhraseMot* pMot, NSPhraseMotArray** ppYesArray, NSPhraseMotArray** ppNoArray)
{
  if (NULL == pMot)
    return;

  string sCertitude = pMot->getCertitude();
  if (string("") != sCertitude)
  {
    if      ((string(sCertitude, 0, 3) == "WCE") &&
             (string(sCertitude, 0, 5) < "WCE50"))
      ajouteCopieMot(ppNoArray, pMot);

    else if ((string(sCertitude, 0, 3) == "WCE") &&
             (string(sCertitude, 0, 5) >= "WCE50"))
      ajouteCopieMot(ppYesArray, pMot);
  }
  else
    ajouteCopieMot(ppYesArray, pMot);
}

// -------------------------------------------------------------------------
// -------------------- METHODES DE NSGenComplement ------------------------
// -------------------------------------------------------------------------
NSGenComplement::NSGenComplement(NSContexte* pCtx, NSGenerateur* pGener)
                :NSRoot(pCtx)
{
  pGenerateur = pGener;

  sType       = string("");
  sPositif    = string("");
  sNegatif    = string("");
  pMotsPos    = (NSPhraseMotArray*) 0;
  pMotsNeg    = (NSPhraseMotArray*) 0;
  iPriorite   = 50;
}

NSGenComplement::NSGenComplement(NSGenComplement& rv)
                :NSRoot(rv.pContexte)
{
try
{
  sType        = rv.sType;
  sPositif     = rv.sPositif;
  sNegatif     = rv.sNegatif;
  iPriorite    = rv.iPriorite;
  pGenerateur  = rv.pGenerateur;

  if (rv.pMotsPos)
    pMotsPos = new NSPhraseMotArray(*(rv.pMotsPos));
  else
    pMotsPos = (NSPhraseMotArray*) 0;
  if (rv.pMotsNeg)
    pMotsNeg = new NSPhraseMotArray(*(rv.pMotsNeg));
  else
    pMotsNeg = (NSPhraseMotArray*) 0;
}
catch (...)
{
	erreur("Exception NSGenComplement copy ctor.", standardError);
}
}

NSGenComplement::~NSGenComplement()
{
	if (pMotsPos)
  	delete pMotsPos;
	if (pMotsNeg)
  	delete pMotsNeg;
}

void
NSGenComplement::donnePhrElements(NSPhraseMot** ppLiaison, NSPhraseMot** ppPreposition, NSPhraseMotArray** ppMots)
{
  if ((NULL == pGenerateur) || (!(pGenerateur->getCurrentPhraseur())))
  {
    *ppLiaison     = 0;
    *ppPreposition = 0;
    *ppMots        = 0;
    return;
  }

  NSPhraseur* pPhra = pGenerateur->getCurrentPhraseur();

  if      (sType == string(STR_CCLIEU))
  {
    *ppLiaison     = &(pPhra->LiaisonLieu);
    *ppPreposition = &(pPhra->PrepositionLieu);
    *ppMots        = &(pPhra->CCLieu);
  }
  else if (sType == string(STR_CCTEMPS))
  {
    *ppLiaison     = &(pPhra->LiaisonTemps);
    *ppPreposition = &(pPhra->PrepositionTemps);
    *ppMots        = &(pPhra->CCTemps);
  }
  else if (sType == string(STR_CCMANIERE))
  {
    *ppLiaison     = &(pPhra->LiaisonManiere);
    *ppPreposition = &(pPhra->PrepositionManiere);
    *ppMots        = &(pPhra->CCManiere);
  }
  else if (sType == string(STR_CCMOYEN))
  {
    *ppLiaison     = &(pPhra->LiaisonMoyen);
    *ppPreposition = &(pPhra->PrepositionMoyen);
    *ppMots        = &(pPhra->CCMoyen);
  }
  else if (sType == string(STR_CCCHIFFRE))
  {
    *ppLiaison     = &(pPhra->LiaisonChiffre);
    *ppPreposition = &(pPhra->PrepositionChiffre);
    *ppMots        = &(pPhra->CCChiffre);
  }
  else if (sType == string(STR_CCBUT))
  {
    *ppLiaison     = &(pPhra->LiaisonBut);
    *ppPreposition = &(pPhra->PrepositionBut);
    *ppMots        = &(pPhra->CCBut);
  }
  else if (sType == string(STR_CCCAUSE))
  {
    *ppLiaison     = &(pPhra->LiaisonCause);
    *ppPreposition = &(pPhra->PrepositionCause);
    *ppMots        = &(pPhra->CCCause);
  }
  else if (sType == string(STR_CCTYPE))
  {
    *ppLiaison     = &(pPhra->LiaisonType);
    *ppPreposition = &(pPhra->PrepositionType);
    *ppMots        = &(pPhra->CCType);
  }
  else if (sType == string(STR_CCHYPOTH))
  {
    *ppLiaison     = &(pPhra->LiaisonHypoth);
    *ppPreposition = &(pPhra->PrepositionHypoth);
    *ppMots        = &(pPhra->CCHypoth);
  }
  return;
}

bool
NSGenComplement::donnePhrase()
{
  bool bSucces;

  NSPhraseMot*      pLiaison;
  NSPhraseMot*      pPreposition;
  NSPhraseMotArray* pMots;

  donnePhrElements(&pLiaison, &pPreposition, &pMots);

  if (STR_CCCHIFFRE == sType)
  {
    if ((pMotsPos) && (false == pMotsPos->empty()))
    {
      sPositif = pGenerateur->donnePhraseChiffree(pLiaison, pPreposition,
                                                        pMotsPos, &bSucces);
      if (false == bSucces)
        return false;
    }

    if ((pMotsNeg) && (false == pMotsNeg->empty()))
    {
      sNegatif = pGenerateur->donnePhraseChiffree(pLiaison, pPreposition,
                                                        pMotsNeg, &bSucces);
      if (false == bSucces)
        return false;
    }
  }
  else
  {
    if ((pMotsPos) && (false == pMotsPos->empty()))
    {
      sPositif = pGenerateur->donnePhraseComplement(pLiaison, pPreposition,
                                                          pMotsPos, &bSucces);
      if (false == bSucces)
        return false;
    }

    if ((pMotsNeg) && (false == pMotsNeg->empty()))
    {
      sNegatif = pGenerateur->donnePhraseComplement(pLiaison, pPreposition,
                                                          pMotsNeg, &bSucces);
      if (false == bSucces)
        return false;
    }
  }
  return true;
}

void
NSGenComplement::initialise()
{
  sType     = "";
  sPositif  = "";
  sNegatif  = "";
  iPriorite = 0;

  if (pMotsPos)
    delete pMotsPos;
  if (pMotsNeg)
    delete pMotsNeg;
}

void
NSGenComplement::classe()
{
  iPriorite = 0;
  sPositif  = "";
  sNegatif  = "";
  if (pMotsPos)
    delete pMotsPos;
  if (pMotsNeg)
    delete pMotsNeg;

  NSPhraseMot*      pLiaison;
  NSPhraseMot*      pPreposition;
  NSPhraseMotArray* pMots;

  donnePhrElements(&pLiaison, &pPreposition, &pMots);

  if (NULL == pMots)
    return;

  iterPhraseMot  iterMots;
  NSPathologData Data;
  NSSuper* pSuper = pContexte->getSuperviseur();

  for (iterMots = pMots->begin(); iterMots != pMots->end(); iterMots++)
  {
    string sLexique = (*iterMots)->getLexique();

    if ((sLexique != "") && (false == (*iterMots)->estTexteLibre()))
    {
      bool trouve = pContexte->getDico()->trouvePathologData(pGenerateur->getLang(), &sLexique, &Data);
      if (false == trouve)
        return;
      // if (!Data.estNom())
      //    return;

      string sCertitude = (*iterMots)->getCertitude();

      if ((string(sCertitude, 0, 3) == "WCE") &&
              (string(sCertitude, 0, 5) < "WCE50"))
        pGenerateur->ajouteCopieMot(&pMotsNeg, *iterMots);
      else if (((string(sCertitude, 0, 3) == "WCE") &&
                  (string(sCertitude, 0, 5) >= "WCE50")) || (sCertitude == ""))
        pGenerateur->ajouteCopieMot(&pMotsPos, *iterMots);
    }
    else
      pGenerateur->ajouteCopieMot(&pMotsPos, *iterMots);

    iPriorite = max(iPriorite, (*iterMots)->getPriority());
  }
}

NSGenComplement&
NSGenComplement::operator=(const NSGenComplement& rv)
{
  if (&rv == this)
    return *this;

  sType       = rv.sType;
  sPositif    = rv.sPositif;
  sNegatif    = rv.sNegatif;
  iPriorite   = rv.iPriorite;
  pGenerateur = rv.pGenerateur;

  if (rv.pMotsPos)
  {
    if (pMotsPos)
      *pMotsPos = *(rv.pMotsPos);
    else
      pMotsPos = new NSPhraseMotArray(*(rv.pMotsPos));
  }
  else if (pMotsPos)
    delete pMotsPos;

  if (rv.pMotsNeg)
  {
    if (pMotsNeg)
      *pMotsNeg = *(rv.pMotsNeg);
    else
      pMotsNeg = new NSPhraseMotArray(*(rv.pMotsNeg));
  }
  else if (pMotsNeg)
    delete pMotsNeg;

  return *this;
}

// -------------------------------------------------------------------------
// ------------------- METHODES DE NSGenereCompArray -----------------------
// -------------------------------------------------------------------------

NSGenereCompArray::NSGenereCompArray(NSContexte* pCtx, NSGenerateur* pGener)
                  :NSGenCompArray()
{
  pGenerateur = pGener;
  pContexte   = pCtx;
}

//--------------------------------------------------------------------------
//  Constructeur copie
//--------------------------------------------------------------------------
NSGenereCompArray::NSGenereCompArray(NSGenereCompArray& rv)
                  :NSGenCompArray()
{
  pGenerateur = rv.pGenerateur;
  pContexte   = rv.pContexte;

try
{
  if (false == rv.empty())
    for (GenCompIter i = rv.begin(); i != rv.end(); i++)
      push_back(new NSGenComplement(*(*i)));
}
catch (...)
{
	erreur("Exception NSGenereCompArray copy ctor.", standardError);
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSGenereCompArray::vider()
{
  if (empty())
    return;

  for (GenCompIter i = begin(); i != end(); )
  {
    delete *i;
    erase(i);
  }
}

NSGenereCompArray::~NSGenereCompArray()
{
	vider();
}

void
NSGenereCompArray::initialise()
{
  vider();

    /*if (empty())
        return true;

    for (GenCompIter i = begin(); i != end(); i++)
        (*i)->initialise(); */

  return;
}

void
NSGenereCompArray::classeTout()
{
try
{
  vider();

  if ((NULL == pGenerateur) || (NULL == pGenerateur->getCurrentPhraseur()))
    return;

  NSPhraseur* pPh = pGenerateur->getCurrentPhraseur();

  if (false == pPh->CCLieu.empty())
  {
    NSGenComplement* pCCLieu = new NSGenComplement(pContexte, pGenerateur);
    pCCLieu->sType = STR_CCLIEU;
    pCCLieu->classe();
    push_back(pCCLieu);
  }
  if (false == pPh->CCTemps.empty())
  {
    NSGenComplement* pCCTemps = new NSGenComplement(pContexte, pGenerateur);
    pCCTemps->sType = STR_CCTEMPS;
    pCCTemps->classe();
    push_back(pCCTemps);
  }
  if (false == pPh->CCManiere.empty())
  {
    NSGenComplement* pCCManiere = new NSGenComplement(pContexte, pGenerateur);
    pCCManiere->sType = STR_CCMANIERE;
    pCCManiere->classe();
    push_back(pCCManiere);
  }
  if (false == pPh->CCMoyen.empty())
  {
    NSGenComplement* pCCMoyen = new NSGenComplement(pContexte, pGenerateur);
    pCCMoyen->sType = STR_CCMOYEN;
    pCCMoyen->classe();
    push_back(pCCMoyen);
  }
  if (false == pPh->CCCause.empty())
  {
    NSGenComplement* pCCCause = new NSGenComplement(pContexte, pGenerateur);
    pCCCause->sType = STR_CCCAUSE;
    pCCCause->classe();
    push_back(pCCCause);
  }
  if (false == pPh->CCBut.empty())
  {
    NSGenComplement* pCCBut = new NSGenComplement(pContexte, pGenerateur);
    pCCBut->sType = STR_CCBUT;
    pCCBut->classe();
    push_back(pCCBut);
  }
  if (false == pPh->CCType.empty())
  {
    NSGenComplement* pCCType = new NSGenComplement(pContexte, pGenerateur);
    pCCType->sType = STR_CCTYPE;
    pCCType->classe();
    push_back(pCCType);
  }
  if (false == pPh->CCChiffre.empty())
  {
    NSGenComplement* pCCChiffre = new NSGenComplement(pContexte, pGenerateur);
    pCCChiffre->sType = STR_CCCHIFFRE;
    pCCChiffre->classe();
    push_back(pCCChiffre);
  }
  if (false == pPh->CCHypoth.empty())
  {
    NSGenComplement* pCCHypoth = new NSGenComplement(pContexte, pGenerateur);
    pCCHypoth->sType = STR_CCHYPOTH;
    pCCHypoth->classe();
    push_back(pCCHypoth);
  }
}
catch (...)
{
	erreur("Exception NSGenereCompArray::classeTout", standardError);
	return;
}
}

NSGenComplement*
NSGenereCompArray::donneComplement(string sType)
{
  if (empty())
    return 0;

  for (GenCompIter i = begin(); i != end(); i++)
    if ((*i)->sType == sType)
      return (*i);

  return 0;
}

bool
NSGenereCompArray::donnePhrase()
{
  if (empty())
    return true;

  for (GenCompIter i = begin(); i != end(); i++)
    if (false == (*i)->donnePhrase())
      return false;

  return true;
}

NSGenereCompArray&
NSGenereCompArray::operator=(NSGenereCompArray src)
{
	if (this == &src)
		return *this;

	pGenerateur = src.pGenerateur;
	pContexte   = src.pContexte;

try
{
	vider();

	if (false == src.empty())
  	for (GenCompIter i = src.begin(); src.end() != i; i++)
    	push_back(new NSGenComplement(*(*i)));

	return *this;
}
catch (...)
{
	erreur("Exception NSGenereCompArray::operator=", standardError);
	return *this;
}
}

