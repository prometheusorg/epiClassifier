//---------------------------------------------------------------------------
//    NSPERSON.CPP
//    KRZISCH PH.   janvier 92
//  Implémentation de objets NAUTILUS
//---------------------------------------------------------------------------

#include <stdio.h>
#include <string>

#include "enterpriseLus/nsglobalLus.h"

#include "enterpriseLus/nsdivfctForCgi.h"

#include "enterpriseLus/superLus.h"
#include "enterpriseLus/personLus.h"
#ifndef __linux__
#include "enterpriseLus/histdoLus.h"
#endif
// #include "enterpriseLus\nspathoLus.h"
#include "nssavoir/nspatho.h"

#ifndef __linux__
#include "nautilus/nsLdv_Vars.h"
#include "nsbb/nsmanager.h"
#include "nsbb/nsbbtran.h"
#include "nsbb/nspatpat.h"
#include "nssavoir/nsgraphe.h"
#include "nssavoir/nsHealthTeam.h"
#include "nsdn/nsdochis.h"
#include "nsdn/nsframedocs.h"
#include "partage/ns_search.h"
#endif
#include "nssavoir/nsfilgd.h"
#include "nsepisod/nsldvuti.h"
#include "dcodeur/nsgen.h"

#ifndef __linux__
#include "pilot\NautilusPilot.hpp"
#endif
#include "nsbb/tagNames.h"

//***************************************************************************
// Implémentation des méthodes NSPatient
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:    NSPatientData::NSPatientData()
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
Person::Person(NSContexte* pCtx)
{
  _pContexte = pCtx ;

  init() ;
}

void
Person::init()
{
  _sID                = string("") ;
  _sPseudo            = string("") ;
  _sFamilyName        = string("") ;
  _sFirstName         = string("") ;
  _sMaidenName        = string("") ;
  _sGender            = string("") ;
  _sAddress           = string("") ;
  _sBirthDate         = string("") ;
  _sMobilePhoneNumber = string("") ;
  _sOfficePhoneNumber = string("") ;
  _sFamilyStatus      = string("") ;
  _sLang              = string("") ;
  _sCivility          = string("") ;
  _sFullName          = string("") ;

#ifndef __linux__
  _pGraphPerson     = new NSPersonGraphManager(_pContexte->getSuperviseur()) ;
  _pDocHis          = (NSHISTODocument *) 0 ;
  _pHealthTeam      = (NSHealthTeam *) 0 ;
  _pFramesDocuments = (NSFrameDocumentsArray*) 0 ;
#endif
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatientData::NSPatientData(NSPatientData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
Person::Person(Person& rv)
{
  _pContexte     = rv._pContexte ;

	_sID                = rv._sID ;
  _sPseudo            = rv._sPseudo ;
	_sFamilyName        = rv._sFamilyName ;
	_sFirstName         = rv._sFirstName ;
	_sGender            = rv._sGender ;
	_sAddress           = rv._sAddress ;
	_sBirthDate         = rv._sBirthDate ;
	_sMobilePhoneNumber = rv._sMobilePhoneNumber ;
	_sOfficePhoneNumber = rv._sOfficePhoneNumber ;
	_sFamilyStatus      = rv._sFamilyStatus ;
	_sLang              = rv._sLang ;
	_sCivility          = rv._sCivility ;
	_sFullName          = rv._sFullName ;
	_sMaidenName        = rv._sMaidenName ;

#ifndef __linux__
  _pGraphPerson       = new NSPersonGraphManager(_pContexte->getSuperviseur()) ;

  if (rv._pFramesDocuments)
    _pFramesDocuments = new NSFrameDocumentsArray(*(rv._pFramesDocuments)) ;
  else
    _pFramesDocuments = (NSFrameDocumentsArray*) 0 ;
#endif
}

//---------------------------------------------------------------------------
//  Fonction:		Destructor
//---------------------------------------------------------------------------
Person::~Person()
{
#ifndef __linux__
  if (_pFramesDocuments)
    delete _pFramesDocuments ;
  delete _pGraphPerson ;
#endif
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatientData::operator==(NSPatientData src)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
Person::operator == ( Person& o )
{
  if ((_sID                == o._sID)         &&
      (_sFamilyName        == o._sFamilyName) &&
      (_sFirstName         == o._sFirstName)  &&
      (_sGender            == o._sGender)     &&
      (_sAddress           == o._sAddress)    &&
      (_sBirthDate         == o._sBirthDate)  &&
      (_sMobilePhoneNumber == o._sMobilePhoneNumber) &&
      (_sOfficePhoneNumber == o._sOfficePhoneNumber) &&
      (_sFullName          == o._sFullName)
      )
      return 1 ;
  else
      return 0 ;
}

//---------------------------------------------------------------------------
//  Function:    Person::buildFullName()
//
//  Description: Initialise nom_long
//
//  Note : on suppose que les données naissance, nom, prenom et sexe
//  sont deja initialisées
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void Person::buildFullName(string sLang)
{
/*
	if (string("") == sLang)
		sLang = _pContexte->getUser()->donneLang() ;
*/

	// calcul de l'age du patient
	bool bIsChild = false ;

	if ((string("") != _sBirthDate) && (string("00000000") != _sBirthDate))
	{
    string sToday = string("") ;
    get_current_date(&sToday) ;
		int age = get_age(&sToday, &_sBirthDate) ;

		if ((age >= 0) && (age <= 14))
			bIsChild = true ;
	}

	if (string("fr") == sLang)
	{
		if (bIsChild)
			_sFullName = string("Enfant ") ;
		else
		{
			if (_sGender[0] == '2')
				_sFullName = string("Mme ") ;
			else
				_sFullName = string("M. ") ;
		}
	}

  if (string("en") == sLang)
	{
		if (bIsChild)
			_sFullName = string("Child ");
		else
		{
			if (_sGender[0] == '2')
				_sFullName = string("Ms ");
			else
				_sFullName = string("Mr ");
		}
	}

	_sFullName += _sFamilyName ;
	_sFullName += string(" ") ;
	_sFullName += _sFirstName ;
}

string
Person::buildCivility(bool /* bShort */, string sLang)
{
/*
	if (string("") == sLang)
		sLang = _pContexte->getUser()->donneLang() ;
*/

	if (string("") != _sCivility)
	{
		string sLibelleTrouve ;
    _pContexte->getSuperviseur()->getDico()->donneLibelle(sLang, &_sCivility, &sLibelleTrouve) ;
    if (string("") != sLibelleTrouve)
    {
    	sLibelleTrouve += string(" ") ;
      return sLibelleTrouve ;
    }
  }

	return string("") ;
}

//---------------------------------------------------------------------------
//  Function:    Person::getBirthDate()
//
//  Description: Retourne une string jj/mm/aaaa contenant la date de naissance
//---------------------------------------------------------------------------
string
Person::getTranslatedBirthDate(string sLang)
{
  if (string("") == _sBirthDate)
		return string("") ;

  string sFormat = _pContexte->getSuperviseur()->getText("0localInformation", "dateFormat", sLang, _pContexte) ;

  return getFormatedDate(_sBirthDate, sLang, sFormat) ;
}

//---------------------------------------------------------------------------
//  Function:    NSPatientData::donneTitre()
//
//  Description: Retourne une string correspondant a nom_long s'il existe
//               sinon refabrique le nom_long
//---------------------------------------------------------------------------
string
Person::getTitle(string sLang)
{
/*
	if (string("") == sLang)
		sLang = _pContexte->getUser()->donneLang() ;
*/

	if (string("") == _sFullName)
		buildFullName() ;

	return _sFullName ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatInfo::initGraphs()
//  Description:	Initialise les graphes du patient.
//  Retour:			true->les graphes sont initialisés false->echec
//---------------------------------------------------------------------------
bool Person::initGraphs()
{
#ifndef __linux__
  NSSuper *pSuper = _pContexte->getSuperviseur() ;

  NSBasicAttributeArray    AttrArray ;
  NSPersonsAttributesArray List ;

  // ouverture standard
  AttrArray.push_back(new NSBasicAttribute(PERSON, _sID)) ;
  AttrArray.push_back(new NSBasicAttribute(OPERATOR, _pContexte->getUserID())) ;
  AttrArray.push_back(new NSBasicAttribute(CONSOLE, string("__"))) ;
  AttrArray.push_back(new NSBasicAttribute(INSTANCE, string("0"))) ;

  string ps = string("Calling Pilot service \"searchPatient\"") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  bool res = pSuper->getPilot()->searchPatient(NautilusPilot::SERV_SEARCH_PATIENT.c_str(),
                                    _pGraphPerson->getDataGraph(), &List, &AttrArray) ;
  if (false == res)
  {
  	std::string tempMessage = pSuper->getPilot()->getWarningMessage() ;
    std::string tempError   = pSuper->getPilot()->getErrorMessage() ;
    if (string("") != tempMessage)
      pSuper->trace(&tempMessage, 1, NSSuper::trWarning) ;
    if (string("") != tempError)
      pSuper->trace(&tempError, 1, NSSuper::trError) ;
    return false ;
  }

	if ((NULL == _pGraphPerson->getDataGraph()) ||
    	(false == _pGraphPerson->graphPrepare()))
  {
    if (NULL == _pGraphPerson->getDataGraph())
      ps = string("Empty graph.") ;
    else
      ps = string("Failure of the graphPrepare function.") ;
    pSuper->trace(&ps, 1, NSSuper::trWarning) ;
		return false ;
  }

	_pGraphPerson->setNeedUnlock(false) ;
	_pGraphPerson->setReadOnly(true) ;

	if (false == List.empty())
	{
		string sIsLocked = List.getAttributeValue("locked") ;
		if (sIsLocked == "ok")
			_pGraphPerson->setNeedUnlock(true) ;
		string sOperationType	= List.getAttributeValue("operationType") ;
		if (sOperationType == "readWrite")
			_pGraphPerson->setReadOnly(false) ;
	}

  if (true == _pGraphPerson->getReadOnly())
  {
    // string sCaption = pContexte->getSuperviseur()->getAppName() ;
    // string sWarnText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    // MessageBox(sWarnText.c_str(), sCaption.c_str(), MB_OK) ;
  }

	_pGraphPerson->setInfoPids(&AttrArray) ;
	_pGraphPerson->getDataGraph()->setLastTree() ;

  // Init HistoDoc (creates and fills)
  //
	_pDocHis = new NSHISTODocument(_pContexte) ;

  // Init demographic information
  //
  NSPatPathoArray PatPathoAdmin(_pContexte->getSuperviseur()) ;
	_pDocHis->DonnePatPathoDocument("ZADMI1", &PatPathoAdmin) ;
  _pGraphPerson->ChargeDonneesAdmin(_pContexte, &PatPathoAdmin, _sFamilyName, _sFirstName, _sCode, _sGender, _sBirthDate, _sCivility) ;

  return true ;
#else
  return false ;
#endif
}

//---------------------------------------------------------------------------
//  bloquer()
//
//  Efface le patient du fichier de blocage
//---------------------------------------------------------------------------
void
Person::debloquer()
{
try
{
#ifndef __linux__
	if (!getADebloquer())
		return ;
#endif

/*
	NSBasicAttributeArray BAttrArray ;
  char szInstance[128] ;
  sprintf(szInstance, "%d", _pContexte->getSuperviseur()->getInstance()) ;
  BAttrArray.push_back(new NSBasicAttribute(CONSOLE,	string(pContexte->getSuperviseur()->getConsole()))) ;
  BAttrArray.push_back(new NSBasicAttribute(INSTANCE,	string(szInstance))) ;
  BAttrArray.push_back(new NSBasicAttribute(PERSON,		getNss())) ;
  if (!pContexte->pPilot->unlockPatient(NautilusPilot::SERV_UNLOCK.c_str(), &BAttrArray))
  {
    string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "unlockError") ;
    return ;
  }
*/

	// setADebloquer(false) ;
}
catch (...)
{
	erreur("Exception debloquer.", standardError) ;
}
}

bool
Person::LookForPath(string sChemin, NSSearchStruct* pSearchStruct, bool reinit_structure)
{
#ifndef __linux__
	if ((NULL == _pDocHis) || (_pDocHis->getVectDocument()->empty()))
		return false ;
#else
  return false ;
#endif
}

#ifndef __linux__

bool
Person::SearchInSynthesis(string sChemin, NSSearchStruct *pSearchStruct)
{
  if (NULL == pSearchStruct)
    return false ;

  string sTrace = string("NSPatInfo::SearchInSynthesis Looking for path ") + sChemin ;
  _pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  NSPatPathoArray patho_synthese(_pContexte->getSuperviseur()) ;   // Tempory patpath
  DonnePathoSynthese(&patho_synthese) ;

  sTrace = string("NSPatInfo::SearchInSynthesis Leaving search for path ") + sChemin ;
  _pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  return true ;
}

//
// Recherche dans les Index de santé - Searching in health indexes
bool
Person::SearchInIndexes(string sChemin, NSSearchStruct *pSearchStruct)
{
  if ((NULL == pSearchStruct) || (string("") == sChemin))
    return false ;

  string sTrace = string("NSPatInfo::SearchInIndexes Looking for path ") + sChemin ;
  _pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  // Health indexes are supposed to be dated as today
  //
  ClasseStringVector aPathElements ;
  DecomposeChaine(&sChemin, &aPathElements, string(1, cheminSeparationMARK)) ;
  if (aPathElements.empty())
    return false ;

  string sPathRoot = (*(aPathElements.begin()))->getItem() ;

  for (int iF = 0 ; iF < FRAMECOUNT ; iF++)

  sTrace = string("NSPatInfo::SearchInIndexes Leaving search for path ") + sChemin ;
  _pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  return true ;
}

bool
Person::SearchInIndexExtensions(string sChemin, vector<NSDocumentHisto*> *pConcernDocs, NSSearchStruct *pSearchStruct)
{
  string sTrace = string("NSPatInfo::SearchInIndexExtensions Entering for path ") + sChemin ;
  _pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  if ((string("") == sChemin) || (NULL == pConcernDocs) || (NULL == pSearchStruct))
  {
    sTrace = string("NSPatInfo::SearchInIndexExtensions Bad parameters, leaving") ;
    _pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return false ;
  }

  if (pConcernDocs->empty())
  {
    sTrace = string("NSPatInfo::SearchInIndexExtensions: No extension found, leaving") ;
    _pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
    return true ;
  }

  // Health indexes are supposed to be dated as today
  //
  for (DocumentIter docIt = pConcernDocs->begin() ; pConcernDocs->end() != docIt ; docIt++)
  {
    NSPatPathoArray PathoExtension(_pContexte->getSuperviseur()) ;
    (*docIt)->initFromPatPatho(&PathoExtension) ;

    if (false == PathoExtension.empty())
    }
  }

  sTrace = string("NSPatInfo::SearchInIndexExtensions: Leaving") ;
  _pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  return true ;
}

bool
Person::SearchInDocument(string sChemin, NSSearchStruct *pSearchStruct, NSDocumentHisto* pDocument)
{
  if ((NULL == pSearchStruct) || (NULL == pDocument))
    return false ;

  NSPatPathoArray PPT(_pContexte->getSuperviseur()) ;
  pDocument->initFromPatPatho(&PPT) ;

  if (PPT.empty())
    return false ;

  string sDateDoc = pDocument->getDateDoc() ;

  PatPathoIter      pptIt      = PPT.begin() ;
  PatPathoConstIter pptItBegin = PPT.begin() ;

// Check if a node (whose path is ok) is valid for NSSearchStruct
//
bool
Person::isValidIndexNode(NSPatPathoArray *pPPT, PatPathoIter pptIter, NSSearchStruct *pSearchStruct, string &sRefDate)
{
  string sDateDoc = donne_date_duJour() ;

  return isValidRegularNode(pPPT, pptIter, pSearchStruct, sDateDoc, sRefDate) ;
}

// Check if a node from index tree (whose path is ok) is valid for NSSearchStruct
//
bool
Person::isValidRegularNode(NSPatPathoArray *pPPT, PatPathoConstIter pptIter, NSSearchStruct *pSearchStruct, string sDateDoc, string &sRefDate)
{
  if (((NSPatPathoArray *) NULL == pPPT) || (NULL == pptIter) || (pPPT->end() == pptIter))
    return false ;

  if (NULL == pSearchStruct)
    return true ;

  // Check Episode dates
  //

  // Check peremption date
  //
  string sDate = pPPT->getNodeDate(pptIter) ;
  if (string("") == sDate)
  return true ;
}

bool
Person::IsDocumentAFrameIndex(NSDocumentHisto* pDocument)
{
  if (NULL == pDocument)
    return false ;

  string name     = pDocument->getContent() ;
  string nameSens = string("") ;

  return false ;
}

bool
Person::IsDocumentASynthesis(NSDocumentHisto* pDocument)
{
  if (NULL == pDocument)
    return false ;

  string name     = pDocument->getContent() ;
  string nameSens = string("") ;
  return (string("ZSYNT") == nameSens) ;
}

#endif  // #ifndef __linux__

#ifndef __linux__
bool
Person::getReadOnly()
{
  return _pGraphPerson->getReadOnly() ;
}

bool
Person::getADebloquer()
{
  return _pGraphPerson->getNeedUnlock() ;
}

NSDocumentHisto*
Person::GetDocument(string sRootSens)
{
  if ((NULL == _pDocHis) || (_pDocHis->getVectDocument()->empty()))
		return (NSDocumentHisto*) 0 ;

  if (string("") == sRootSens)
    return (NSDocumentHisto*) 0 ;

  DocumentReverseIter iterReverseDocEnd = _pDocHis->getVectDocument()->rend() ;
	for (DocumentReverseIter iterReverseDoc = _pDocHis->getVectDocument()->rbegin() ; iterReverseDoc != iterReverseDocEnd ; iterReverseDoc++)
	{
    NSPatPathoArray PPT(_pContexte->getSuperviseur()) ;
    (*iterReverseDoc)->initFromPatPatho(&PPT) ;

    if (false == PPT.empty())
    {
      PatPathoIter iter = PPT.begin() ;
      string sDocRootSens = (*iter)->getLexiqueSens(_pContexte->getSuperviseur()) ;

      if (sDocRootSens == sRootSens)
        return *iterReverseDoc ;
		}
	}

  return (NSDocumentHisto*) 0 ;
}

void
Person::GetDocPatho(NSPatPathoArray *pPatho, string sRootSens)
{
  if (NULL == pPatho)
		return ;

  pPatho->vider() ;

  NSDocumentHisto* pDoc = GetDocument(sRootSens) ;
  if (NULL == pDoc)
    return ;

  pDoc->initFromPatPatho(pPatho) ;
}

// -----------------------------------------------------------------------------
// Renvoie la patpatho fille du noeud
// -----------------------------------------------------------------------------
// ajout fab - 2003/08/13 -- on cherche également dans la patpatho META
// -----------------------------------------------------------------------------
bool
Person::getSubArray(string sNode, NSPatPathoArray *pPPT)
{
  if (NULL == pPPT)
    return false ;
}

// -----------------------------------------------------------------------------
// Renvoie une patpatho qui contient le noeud, sa fille et les même données
// pour les frères du noeud qui lui sont identiques
// -----------------------------------------------------------------------------
// ajout fab - 2003/08/13 -- on cherche également dans la patpatho META
// -----------------------------------------------------------------------------
bool
Person::getArray(string sNode, NSPatPathoArray *pPPT)
{
  if (NULL == pPPT)
    return false ;
}
#endif // #ifndef __linux__

/*
//---------------------------------------------------------------------------
//  Fonction:		NSPatInfo::initCorrespArray()
//  Description:	Recherche les correspondants du patient.
//  Retour:			true->le tableau des corresp est initialisé false->echec
//---------------------------------------------------------------------------
bool
Person::initCorrespArray(NSPatPathoArray* pPatPathoArray)
{
	if ((!pPatPathoArray) || (pPatPathoArray->empty()))
  	return false ;

  pCorrespArray->vider() ;
  //
  // On part du principe que les données qui nous intéressent sont dans un
  // sous chapitre LCTAC (contacts)
  //
  // We suppose that the values we need are in a sub-chapter LCTAC (contact)
  //
  PatPathoIter    iter ;
  string          sElemLex, sSens, sType ;
  bool            bCorresp;

  string          sFonction = "" ;
  string          sCode     = "" ;

  string          sTemp   = "" ;

  iter = pPatPathoArray->begin();
  int iColBase = (*iter)->pDonnees->getColonne();
  iter++;

  while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColBase))
  {
      sElemLex = string((*iter)->pDonnees->lexique);
      pContexte->getDico()->donneCodeSens(&sElemLex, &sSens);

      // Chapitre "contacts" / Contact chapter
      if (sSens == string("LCTAC"))
      {
          int iColContact = (*iter)->pDonnees->getColonne() ;
          iter++ ;

          while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColContact))
          {
              sElemLex = string((*iter)->pDonnees->lexique);
              pContexte->getDico()->donneCodeSens(&sElemLex, &sSens);

              // Liste des correspondants
              if (sSens == string("LCORR"))
              {
                  iter++;
                  while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColContact + 1))
                  {
                      sElemLex = string((*iter)->pDonnees->lexique);
                      pContexte->getDico()->donneCodeSens(&sElemLex, &sSens);

                      // Correspondant : comprend fonction et person_id
                      if (sSens == string("DCORR"))
                      {
                          iter++;
                          bCorresp = false;
                          sFonction = "";
                          sCode = "";

                          while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColContact + 2))
                          {
                              sElemLex = string((*iter)->pDonnees->lexique);
                              pContexte->getDico()->donneCodeSens(&sElemLex, &sSens);

                              // Correspondant : comprend fonction et person_id
                              if (sSens == string("LFONC"))
                              {
                                  iter++;
                                  while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColContact + 3))
                                  {
                                      // on cherche ici un texte libre
                                      sElemLex = (*iter)->getLexique() ;
                                      if (string(sElemLex, 0, 3) == string("£CL"))
                                      {
                                          sFonction = (*iter)->pDonnees->getTexteLibre();
                                      }
                                      iter++;
                                  }
                              }
                              else if (sSens == string("HHUMA"))
                              {
                                  iter++;
                                  while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColContact + 3))
                                  {
                                      // on cherche ici un texte libre
                                      sElemLex = (*iter)->getLexique() ;
                                      if (string(sElemLex, 0, 5) == string("£SPID"))
                                      {
                                          sCode = string((*iter)->pDonnees->complement);
                                          bCorresp = true;
                                      }
                                      iter++;
                                  }
                              }
                              else
                                  iter++;
                          }

                          if (bCorresp)
                          {
                              NSPidsInfo* pPidsInfo = new NSPidsInfo(pContexte);
                              initCorrespPIDS(pPidsInfo, sCode);
                              pPidsInfo->sFonction = sFonction;

                              pCorrespArray->push_back(new NSPidsInfo(*pPidsInfo));
                              delete pPidsInfo;
                          }
                      }
                      else
                          iter++;
                  }
              }
              else
                  iter++;
          }
      }
      else
          iter++;
  }

	return true ;
}
*/

#ifndef __linux__
bool
Person::getFoldersPatho(NSPatPathoArray* pPatPathoArray)
{
  if (NULL == pPatPathoArray)
    return false ;

  pPatPathoArray->vider() ;

  if ((NULL != _pDocHis) && (NULL != _pDocHis->getLibChem()))
  {
    _pDocHis->getLibChem()->initFromPatPatho(pPatPathoArray) ;
    return true ;
  }

  string sDocRoot = _pGraphPerson->getRootTree() ;
  NSLinkManager* pGraphe = _pGraphPerson->getLinkManager() ;
  if (NULL == pGraphe)
    return false ;

  VecteurString aVecteurString ;
  pGraphe->TousLesVrais(sDocRoot, NSRootLink::personFolderLibrary, &aVecteurString) ;
  if (aVecteurString.empty())
    return false ;

  string sCodeDocLibChem = *(*(aVecteurString.begin())) ;
  NSDocumentInfo docInf(sCodeDocLibChem, _pContexte, _pGraphPerson) ;
  docInf.LoadMetaAndData(_pGraphPerson) ;
  docInf.DonnePatPatho(pPatPathoArray, _pGraphPerson) ;

  return true ;
}

bool
Person::getFoldersArray(NSChemiseArray* pChemisesArray)
{
  if (NULL == pChemisesArray)
    return false ;

  NSPatPathoArray FoldersArray(_pContexte->getSuperviseur()) ;

  if (false == getFoldersPatho(&FoldersArray))
    return false ;

  if (FoldersArray.empty())
    return false ;

  PatPathoIter iter = FoldersArray.begin() ;
  int iColBase = (*iter)->getColonne() ;
  iter++ ;

  string sNodeChem = "" ;
  string sNom      = "" ;
  string sDate     = "" ;

  string sLang = "" ;
/*
	if (_pContexte->getUser())
		sLang = _pContexte->getUser()->donneLang() ;
*/

  while ((FoldersArray.end() != iter) && ((*iter)->getColonne() > iColBase))
  {
    string sSens = (*iter)->getLexiqueSens(_pContexte->getSuperviseur()) ;

    if (string("0CHEM") == sSens)
    {
      string sNodeChem = (*iter)->getNode() ;
      sNom  = "" ;
      sDate = "" ;
      iter++ ;

      // on charge les données de la chemise
      while ((FoldersArray.end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
        sSens = (*iter)->getLexiqueSens(_pContexte->getSuperviseur()) ;

        // Folder name
        //
        if (string("0INTI") == sSens)
        {
          iter++ ;

          while ((FoldersArray.end() != iter) && ((*iter)->getColonne() > iColBase + 2))
          {
            // must be a free text
            string sElemLex = (*iter)->getLexique() ;

            if (string("£?????") == sElemLex)
              sNom = (*iter)->getTexteLibre() ;

            iter++ ;
          }
        }
        // Dates
        else if (string("KOUVR") == sSens)
        {
          iter++ ;
          int iLigneBase = (*iter)->getLigne() ;
          // gereDate* pDate = new gereDate(pContexte);
          string sUnite  = "" ;
          string sFormat = "" ;
          string sValeur = "" ;
          string sTemp   = "" ;

          while ((iter != FoldersArray.end()) &&
                         ((*iter)->getLigne() == iLigneBase))
          {
            string sLexi = (*iter)->getLexique() ;
            if ('£' == sLexi[0])
            {
              sTemp   = (*iter)->getLexique() ;
              _pContexte->getSuperviseur()->getDico()->donneCodeSens(&sTemp, &sFormat) ;
              sValeur = (*iter)->getComplement() ;
              sTemp   = (*iter)->getUnit() ;
              _pContexte->getSuperviseur()->getDico()->donneCodeSens(&sTemp, &sUnite) ;
              iter++ ;
              break ;
            }

            iter++ ;
          }

          // sFormat est du type £D0;03
          if ((sFormat != "") && (sFormat[1] == 'T') &&
                      (sValeur != ""))
          {
            if (sUnite == "2DA02")
              sDate = string(sValeur, 0, 8);
          }
        }
        else
          iter++ ;
      }

      if (string("") != sNom)
      {
        NSChemiseInfo *pChemInfo = new NSChemiseInfo() ;
        
        pChemInfo->_sNodeChemise = sNodeChem ;
        pChemInfo->setNom(sNom) ;
        pChemInfo->setCreation(sDate) ;

        pChemisesArray->push_back(pChemInfo) ;
      }
    }
    else
      iter++ ;
  }
  return true ;
}

bool
Person::CreeContribution(bool bInitiale)
{
try
{
	string sDateJour = donne_date_duJour() + donne_heure() ;
	string sPidsUtil = _pContexte->getUserID() ;

	// Construction de la patpatho de la nouvelle contribution
	NSPatPathoArray PatPathoArray(_pContexte->getSuperviseur()) ;

	PatPathoArray.ajoutePatho("LCTRI1", 0) ;

  Message Msg ;

	// Utilisateur
	PatPathoArray.ajoutePatho("HUTIL1", 1) ;
	Msg.SetComplement(sPidsUtil.c_str()) ;
	PatPathoArray.ajoutePatho("£SPID1", &Msg, 2) ;

  Msg.Reset() ;

	// Date d'ouverture
	PatPathoArray.ajoutePatho("KOUVR1", 1) ;
	Msg.SetUnit("2DA021") ;
	Msg.SetComplement(sDateJour) ;
	PatPathoArray.ajoutePatho("£T0;19", &Msg, 2) ;

	//
	// On crée la liaison de la contribution avec le document root
	//

	// La contribution initiale est enregistrée dans ::createPatient
	string sNodeRoot = _pGraphPerson->getRootTree() ;
	_sContribution = _pGraphPerson->setTree(&PatPathoArray, "") ;

	NSLinkManager Link(_pContexte->getSuperviseur(), _pGraphPerson->getDataGraph()) ;
	Link.etablirLien(sNodeRoot, NSRootLink::personContribution, _sContribution) ;

	if (false == bInitiale)
	{
		// pour les contributions ultérieures on update directement
		NSVectPatPathoArray VectU ;
		VectU.push_back(new NSPatPathoArray(PatPathoArray)) ;
		bool bContribCreated = _pGraphPerson->updateTrees(_pContexte, &VectU, &_sContribution) ;
		if (false == bContribCreated)
		{
			_sContribution = string("") ;
			return false ;
		}
		// sContribution = pGraphPerson->pDataGraph->getLastContribution();
	}

	return true ;
}
catch (...)
{
	erreur("Exception NSPatInfo::CreeContribution.", standardError) ;
	return false;
}
}

bool
Person::FermeContribution()
{
try
{
  string sDateJour = donne_date_duJour() + donne_heure() ;

  //
  // on charge l'arbre qui correspond à la contribution en cours
  //
  NSPatPathoArray PatPathoArray(_pContexte->getSuperviseur()) ;
  string sRosace = "" ;
  if (false == _pGraphPerson->getTree(_sContribution, &PatPathoArray, &sRosace))
    return false ;

  NSPatPathoArray PatPatho(_pContexte->getSuperviseur()) ;
  PatPatho.ajoutePatho("KFERM1", 0) ;
  Message	Msg ;
  Msg.SetUnit("2DA021") ;
  Msg.SetComplement(sDateJour) ;
  PatPatho.ajoutePatho("£T0;19", &Msg, 1) ;

  PatPathoArray.InserePatPatho(PatPathoArray.end(), &PatPatho, 1) ;

  NSVectPatPathoArray VectU ;
  VectU.push_back(new NSPatPathoArray(PatPathoArray)) ;
  string sNewTreeId ;
  if (false == _pGraphPerson->updateTrees(_pContexte, &VectU, &sNewTreeId))
    return false ;

  _sContribution = "";

  return true ;
}
catch (...)
{
	erreur("Exception NSPatInfo::FermeContribution.", standardError) ;
	return false ;
}
}
#endif // #ifndef __linux__

/*
bool
NSPatInfo::initCorrespPIDS(NSPidsInfo* pPidsInfo, string sPersonID)
{
	//
	// Création d'une fiche d'accès au fichier PIDS
	//
	NSPids* pPids = new NSPids(pContexte, pidsCorresp) ;
	//
	// Ouverture du fichier
	//
	pPids->lastError = pPids->open() ;
	if (pPids->lastError != DBIERR_NONE)
	{
		erreur("Il est impossible d'ouvrir le fichier des PIDS.", standardError, pPids->lastError) ;
		delete pPids ;
		return false ;
	}

    pPids->lastError = pPids->chercheClef(&sPersonID,
                                                "",
                                                0,
                                                keySEARCHEQ,
                                                dbiWRITELOCK);

    if (pPids->lastError != DBIERR_NONE)
    {
        erreur("Impossible de trouver le PIDS du correspondant en cours.", standardError, pPids->lastError) ;
        pPids->close() ;
        delete pPids;
        return false;
    }

    //
    // On récupère l'enregistrement
    //
    pPids->lastError = pPids->getRecord();

    if (pPids->lastError != DBIERR_NONE)
    {
        erreur("Le fichier des PIDS correspondants est endommagé.", standardError, pPids->lastError) ;
        pPids->close() ;
        delete pPids;
        return false;
    }

    // Chargement des données du nouveau PIDS (qui garde les mêmes NSS et RootDoc)
    strcpy(pPidsInfo->pDonnees->nss,        pPids->pDonnees->nss);
    strcpy(pPidsInfo->pDonnees->rootdoc,    pPids->pDonnees->rootdoc);
    strcpy(pPidsInfo->pDonnees->nom,        pPids->pDonnees->nom) ;
    strcpy(pPidsInfo->pDonnees->prenom,     pPids->pDonnees->prenom) ;
    strcpy(pPidsInfo->pDonnees->code,       pPids->pDonnees->code) ;
    strcpy(pPidsInfo->pDonnees->sexe,       pPids->pDonnees->sexe) ;
    strcpy(pPidsInfo->pDonnees->naissance,  pPids->pDonnees->naissance) ;

    pPids->close() ;
    delete pPids ;

    return true ;
}

string
NSPatInfo::getIPPEnCours(string sSite, string* psOuvre, string* psFerme)
{
	if (NULL != psOuvre)
		*psOuvre = string("") ;
  if (NULL != psFerme)
		*psFerme = string("") ;

	// Get IDs Library
	//
  NSPatPathoArray pptIdent(pContexte) ;
	if (false == pGraphPerson->getLibIDsPpt(&pptIdent))
		return string("") ;

	string sCurrentIpp ;
	bool bExistIPP = pGraphPerson->IPPEnCours(&pptIdent, sSite, &sCurrentIpp, psOuvre, psFerme) ;
	if (false == bExistIPP)
  	return string("") ;

	return sCurrentIpp ;
}
*/

Person&
Person::operator=(Person src)
{
	if (this == &src)
		return *this ;

	_pContexte          = src._pContexte;

	_sID                = src._sID ;
  _sPseudo            = src._sPseudo ;
	_sFamilyName        = src._sFamilyName ;
	_sFirstName         = src._sFirstName ;
	_sGender            = src._sGender ;
	_sAddress           = src._sAddress ;
	_sBirthDate         = src._sBirthDate ;
	_sMobilePhoneNumber = src._sMobilePhoneNumber ;
	_sOfficePhoneNumber = src._sOfficePhoneNumber ;
	_sFamilyStatus      = src._sFamilyStatus ;
	_sLang              = src._sLang ;
	_sCivility          = src._sCivility ;
	_sFullName          = src._sFullName ;
	_sMaidenName        = src._sMaidenName ;

#ifndef __linux__
  if (_pHealthTeam)
  {
    delete _pHealthTeam ;
    _pHealthTeam = 0 ;
  }
  if (src._pHealthTeam)
    _pHealthTeam = new NSHealthTeam(*(src._pHealthTeam)) ;

  if (_pFramesDocuments)
  {
    delete _pFramesDocuments ;
    _pFramesDocuments = (NSFrameDocumentsArray*) 0 ;
  }
  if (src._pFramesDocuments)
    _pFramesDocuments = new NSFrameDocumentsArray(*(src._pFramesDocuments)) ;
#endif

	return *this ;
}

string
Person::fabriqueAgeLabel(string sLang)
{
  if (string("") == _sBirthDate)
    return string("") ;

  NVLdVTemps tNaissance ;
  if (false == tNaissance.initFromDate(_sBirthDate))
    return string("") ;

  NVLdVTemps tNow ;
  tNow.takeTime() ;

  unsigned long lDaysOfLife = tNow.daysBetween(tNaissance) ;

  if (lDaysOfLife < 3)
    return fabriqueAgeLabelHours(sLang) ;
  if (lDaysOfLife < 90)
    return fabriqueAgeLabelDays(sLang) ;
  if (lDaysOfLife < 700)
    return fabriqueAgeLabelMonths(sLang) ;

  return fabriqueAgeLabelYears(sLang) ;
}

string
Person::fabriqueAgeLabelYears(string sLang)
{
  if (string("") == _sBirthDate)
    return string("") ;

  string sCurrentDate = donne_date_duJour() ;

  int iCurrentAge = donne_age(sCurrentDate, _sBirthDate) ;
  if (iCurrentAge <= 0)
    return string("") ;

  // Get the lib of "year(s)"
  //
  string sCodeLexAn = string("2DAT33") ;
  NSPathologData Data ;
  if (false == _pContexte->getSuperviseur()->getDico()->trouvePathologData(sLang, &sCodeLexAn, &Data))
    return string("") ;

  GENRE iGenre ;
  Data.donneGenre(&iGenre) ;
  if (iCurrentAge > 1)
    Data.donneGenrePluriel(&iGenre) ;
  string sLibel ;
  NSGenerateur* pGene = _pContexte->getSuperviseur()->getDico()->getGenerateur() ;
  pGene->donneLibelleAffiche(&sLibel, &Data, iGenre) ;

  string sTitre = IntToString(iCurrentAge) + string(" ") + sLibel ;

  return sTitre ;
}

string
Person::fabriqueAgeLabelMonths(string sLang)
{
  if (string("") == _sBirthDate)
    return string("") ;

  string sCurrentDate = donne_date_duJour() ;

  int iCurrentAge = donne_age_mois(sCurrentDate, _sBirthDate) ;
  if (iCurrentAge <= 0)
    return string("") ;

  // Get the lib of "month(s)"
  //
  string sCodeLexAn = string("2DAT21") ;
  NSPathologData Data ;
  if (false == _pContexte->getSuperviseur()->getDico()->trouvePathologData(sLang, &sCodeLexAn, &Data))
    return string("") ;

  GENRE iGenre ;
  Data.donneGenre(&iGenre) ;
  if (iCurrentAge > 1)
    Data.donneGenrePluriel(&iGenre) ;
  string sLibel ;
  NSGenerateur* pGene = _pContexte->getSuperviseur()->getDico()->getGenerateur() ;
  pGene->donneLibelleAffiche(&sLibel, &Data, iGenre) ;

  string sTitre = IntToString(iCurrentAge) + string(" ") + sLibel ;

  return sTitre ;
}

string
Person::fabriqueAgeLabelDays(string sLang)
{
  NVLdVTemps tNaissance ;
  if (false == tNaissance.initFromDate(_sBirthDate))
    return string("") ;

  NVLdVTemps tNow ;
  tNow.takeTime() ;

  unsigned long lDaysOfLife = tNow.daysBetween(tNaissance) ;
  if (lDaysOfLife <= (unsigned long)0)
    return string("") ;

  // Get the lib of "day(s)"
  //
  string sCodeLexAn = string("2DAT01") ;
  NSPathologData Data ;
  if (false == _pContexte->getSuperviseur()->getDico()->trouvePathologData(sLang, &sCodeLexAn, &Data))
    return string("") ;

  int iCurrentAge = int(lDaysOfLife) ;

  GENRE iGenre ;
  Data.donneGenre(&iGenre) ;
  if (iCurrentAge > 1)
    Data.donneGenrePluriel(&iGenre) ;
  string sLibel ;
  NSGenerateur* pGene = _pContexte->getSuperviseur()->getDico()->getGenerateur() ;
  pGene->donneLibelleAffiche(&sLibel, &Data, iGenre) ;

  string sTitre = IntToString(iCurrentAge) + string(" ") + sLibel ;

  return sTitre ;
}

string
Person::fabriqueAgeLabelHours(string sLang)
{
  return string("") ;
}

#ifndef __linux__
// -----------------------------------------------------------------------------
// Renvoie une patpatho qui contient le noeud, sa fille et les même données
// pour les frères du noeud qui lui sont identiques
// -----------------------------------------------------------------------------
// ajout fab - 2003/08/13 -- on cherche également dans la patpatho META
// -----------------------------------------------------------------------------
/*
bool
Person::DonneArray(string sNoeud, NSPatPathoArray *pPPT)
{
  if (NULL == pPPT)
    return false ;

	if ((NULL == _pDocHis) || (NULL == _pDocHis->getDocumentsArrays()) ||
                                  (_pDocHis->getDocumentsArrays()->empty()))
		return false ;

  if (strlen(sNoeud.c_str()) != PAT_NSS_LEN + DOC_CODE_DOCUM_LEN + PPD_NOEUD_LEN)
    return false ;

	string sDocument = string(sNoeud, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;

  NSDocHistoArray* pDocuments = _pDocHis->getDocumentsArrays() ;

	DocumentIter DocIt = pDocuments->begin() ;
	for ( ; pDocuments->end() != DocIt ; DocIt++)
	{
    NSPatPathoArray pptDoc(_pContexte->getSuperviseur()) ;
    (*DocIt)->initFromPatPatho(&pptDoc) ;

		if (false == pptDoc.empty())
		{
			PatPathoIter pptIt = pptDoc.begin() ;
      if ((*pptIt)->getDoc() == sDocument)
      {
        for ( ; (pptDoc.end() != pptIt) && ((*pptIt)->getNode() != sNoeud) ; pptIt++)
          ;
        if (pptDoc.end() != pptIt)
        {
          pptDoc.ExtrairePatPathoFreres(pptIt, pPPT) ;
          return true ;
        }
      }
      else
      {
        NSPatPathoArray pptDocMeta(_pContexte->getSuperviseur()) ;
        (*DocIt)->initFromMeta(&pptDocMeta) ;

        if (false == pptDocMeta.empty())
        {
          PatPathoIter pptMetaIter = pptDocMeta.begin() ;
          if ((*pptMetaIter)->getDoc() == sDocument)
          {
            for ( ; (pptDocMeta.end() != pptMetaIter) && ((*pptMetaIter)->getNode() != sNoeud) ; pptMetaIter++)
              ;
            if (pptDocMeta.end() != pptMetaIter)
            {
              pptDocMeta.ExtrairePatPathoFreres(pptMetaIter, pPPT) ;
              return true ;
            }
          }
        }
      }
		}
	}
	return false ;
}
*/
#endif // #ifndef __linux__

//***************************************************************************
//
// Fonction globale StringTitre (utilisée par NSUtilisateur et NSCorrespondant)
// -> Renvoie la string titre en fonction du type (caractère)
//
//***************************************************************************

string StringTitre(NSContexte* pContexte, string sTypeTitre, string sLang)
{
	string titre("");

	if (string("") == sTypeTitre)
		return titre ;

	pContexte->getSuperviseur()->getDico()->donneLibelle(sLang, &sTypeTitre, &titre) ;
  titre[0] = pseumaj(titre[0]) ;
  titre += string(" ") ;

	return titre ;
}

//***************************************************************************
//
// Fonction globale StringAvantTitre (utilisée par NSUtilisateur et NSCorrespondant)
// -> Renvoie la string avant titre en fonction du type (caractère)
//
//***************************************************************************

string StringAvantTitre(string sTypeTitre, string sLang)
{
	string avant("") ;

	if (string("") == sTypeTitre)
		return avant ;

	if ((string("") == sLang) || (string("fr") == string(sLang, 0, 2)))
	{
  	if ((string("HDOCT1") == sTypeTitre) ||
        (string("HPROF1") == sTypeTitre))
    	avant = string("au") ;
		else
    	avant = string("à") ;
	}
  else if (string("en") == string(sLang, 0, 2))
  {
  	avant = string("to") ;
  }

	return avant ;
}

//////////////////////// ending personLus.cpp //////////////////////////////
