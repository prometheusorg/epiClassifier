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

// #include "enterpriseLus\nspathoLus.h"
#include "nssavoir/nspatho.h"

#include "nssavoir/nsfilgd.h"
#include "dcodeur/nsgen.h"

#include "nsbb/tagNames.h"

//***************************************************************************
// Implémentation des méthodes NSPatient
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:    NSPatientData::NSPatientData()
//
//  Description: Constructor
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
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatientData::NSPatientData(NSPatientData& rv)
//  Description:	Copy constructor
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
}

//---------------------------------------------------------------------------
//  Fonction:		Destructor
//---------------------------------------------------------------------------
Person::~Person()
{
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

    return *this ;
}

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
