// -----------------------------------------------------------------------------
//    flechiesDB.CPP
//    NAUTILUS janvier 2002
//
//    Implementation des objets de base de donnees pour "flechies.db"
// -----------------------------------------------------------------------------

#include <cstring>
#include "nsepisod/flechiesDB.h"

// -----------------------------------------------------------------------------
// Implémentation des méthodes NSFlechies
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//  Constructeur
// -----------------------------------------------------------------------------
NSFlechiesData::NSFlechiesData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero();
}

// -----------------------------------------------------------------------------
//  Constructeur copie
// -----------------------------------------------------------------------------
NSFlechiesData::NSFlechiesData(NSFlechiesData& rv)
{
	sId			 = rv.sId;
	sLibelle = rv.sLibelle;
	sCode  	 = rv.sCode;
}

// -----------------------------------------------------------------------------
//  Destructeur
// -----------------------------------------------------------------------------
NSFlechiesData::~NSFlechiesData()
{
}

// -----------------------------------------------------------------------------
//  Opérateur =
// -----------------------------------------------------------------------------
NSFlechiesData&
NSFlechiesData::operator=(NSFlechiesData src)
{
  if (this == &src)
    return *this;

	sId			 = src.sId;
	sLibelle = src.sLibelle;
	sCode  	 = src.sCode;

	return (*this);
}

// -----------------------------------------------------------------------------
//  Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSFlechiesData::operator==(const NSFlechiesData& o)
{
	if ((sId			== o.sId)      &&
			(sLibelle == o.sLibelle) &&
			(sCode    == o.sCode))
		return 1;
	else
		return 0;
}

// -----------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
// -----------------------------------------------------------------------------
void
NSFlechiesData::metAZero()
{
	sId      = string("");
	sLibelle = string("");
	sCode    = string("");
}

// -----------------------------------------------------------------------------
//  Constructeur
// -----------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
NSFlechies::NSFlechies(NSSuper* pSuper, int iBase)
           :NSFlechiesInfo(),
		        NSFiche(pSuper),
		        iTypeBase(iBase)
{
	switch (iTypeBase)
	{
		case flechies	:	sFileName   = string("flechies.db");
										sFileLib    = string("formes fléchies");
										break;

		case ortho		:	sFileName   = string("flxortho.db");
										sFileLib    = string("fautes d'orthographes classiques");
										break;

		case local		:	sFileName   = string("flxlocal.db");
										sFileLib    = string("termes locaux");
										break;

		case express	: sFileName   = string("flxexpre.db");
										sFileLib    = string("expressions classiques");
										break;

		default				: sFileName   = string("flechies.db");
										sFileLib    = string("formes fléchies");
										break;
  }
}
#else
NSFlechies::NSFlechies(NSSuper* pSuper, int iBase)
           :NSFlechiesInfo(),
		        NSSuperRoot(pSuper),
		        iTypeBase(iBase)
{
	switch (iTypeBase)
	{
		case flechies	:	sFileName   = string("flechies");
										sFileLib    = string("formes fléchies");
										break;

		case ortho		:	sFileName   = string("flxortho");
										sFileLib    = string("fautes d'orthographes classiques");
										break;

		case local		:	sFileName   = string("flxlocal");
										sFileLib    = string("termes locaux");
										break;

		case express	: sFileName   = string("flxexpre");
										sFileLib    = string("expressions classiques");
										break;

		default				: sFileName   = string("flechies");
										sFileLib    = string("formes fléchies");
										break;
  }
}
#endif

// -----------------------------------------------------------------------------
//  Constructeur copie
// -----------------------------------------------------------------------------
NSFlechies::NSFlechies(NSFlechies& rv)
           :NSFlechiesInfo(rv),
#ifndef _ENTERPRISE_DLL
		        NSFiche(rv._pSuper)
#else
		        NSSuperRoot(rv._pSuper)
#endif
{
  iTypeBase = rv.iTypeBase;
  sFileName = rv.sFileName;
}

// -----------------------------------------------------------------------------
//  Destructeur
// -----------------------------------------------------------------------------
NSFlechies::~NSFlechies()
{
}

#ifdef _ENTERPRISE_DLL
// -----------------------------------------------------------------------------
//  Transfère le contenu de pRecBuff dans les variables de la fiche
// -----------------------------------------------------------------------------
void
NSFlechies::alimenteFiche(MYSQL_ROW *pRow)
{
	pDonnees->sId			 = (*pRow)[FLECHIES_ID_FIELD - 1];
	pDonnees->sLibelle = (*pRow)[FLECHIES_LIBELLE_FIELD - 1];
	pDonnees->sCode		 = (*pRow)[FLECHIES_CODE_FIELD - 1];
}
#else
void
NSFlechies::alimenteFiche()
{
  char id[FLECHIES_ID_LEN + 1];
	char libelle[FLECHIES_LIBELLE_LEN + 1];
	char code[FLECHIES_CODE_LEN + 1];

	alimenteChamp(id,      FLECHIES_ID_FIELD,      FLECHIES_ID_LEN);
	alimenteChamp(libelle, FLECHIES_LIBELLE_FIELD, FLECHIES_LIBELLE_LEN);
	alimenteChamp(code,    FLECHIES_CODE_FIELD,    FLECHIES_CODE_LEN);

  pDonnees->setId(id);
  pDonnees->setLabel(libelle);
  pDonnees->setCode(code);
}
// -----------------------------------------------------------------------------
//  Transfère le contenu des valeurs de la fiche dans pRecBuff
// -----------------------------------------------------------------------------
void
NSFlechies::videFiche()
{
	videChamp((char*)pDonnees->getId().c_str(),    FLECHIES_ID_FIELD,      FLECHIES_ID_LEN);
	videChamp((char*)pDonnees->getLabel().c_str(), FLECHIES_LIBELLE_FIELD, FLECHIES_LIBELLE_LEN);
	videChamp((char*)pDonnees->getCode().c_str(),  FLECHIES_CODE_FIELD,    FLECHIES_CODE_LEN);
}

// -----------------------------------------------------------------------------
//  Ouvre la table primaire et les index secondaires
// -----------------------------------------------------------------------------
DBIResult
NSFlechies::open(bool bOpenShared)
{
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(sFileName.c_str(), NSF_GUIDES, bOpenShared);

	return (lastError);
}
#endif

// -----------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool
NSFlechies::Create()
{
	return true;
}

// -----------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool
NSFlechies::Modify()
{
	return true;
}

// -----------------------------------------------------------------------------
//  Opérateur d'affectation
// -----------------------------------------------------------------------------
NSFlechies&
NSFlechies::operator=(NSFlechies src)
{
	*pDonnees = *(src.pDonnees);
	iTypeBase = src.iTypeBase;
	return *this;
}

// -----------------------------------------------------------------------------
//  Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSFlechies::operator==(const NSFlechies& o)
{
	 return (*pDonnees == *(o.pDonnees));
}

// -----------------------------------------------------------------------------
//  Constructeur par défaut
// -----------------------------------------------------------------------------
NSFlechiesInfo::NSFlechiesInfo()
{
	// Crée l'objet de données
	pDonnees = new NSFlechiesData();
}

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------------------
//  Constructeur à partir d'un NSFlechies
// -----------------------------------------------------------------------------
NSFlechiesInfo::NSFlechiesInfo(NSFlechies *pFlechies)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSFlechiesData();
	//
	// Copie les valeurs du NSEpiContact
	//
	*pDonnees = *(pFlechies->pDonnees);
}
#endif

// -----------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSFlechiesInfo::NSFlechiesInfo(NSFlechiesInfo& rv)
{
	// Crée l'objet de données
	pDonnees = new NSFlechiesData();

	// Copie les valeurs du NSFlechiesInfo d'origine
	*pDonnees = *(rv.pDonnees);
}

// -----------------------------------------------------------------------------
//  Destructeur
// -----------------------------------------------------------------------------
NSFlechiesInfo::~NSFlechiesInfo()
{
	delete pDonnees;
}

// -----------------------------------------------------------------------------
//  Opérateur d'affectation
// -----------------------------------------------------------------------------
NSFlechiesInfo&
NSFlechiesInfo::operator=(NSFlechiesInfo src)
{
  if (this == &src)
    return *this;

	*pDonnees = *(src.pDonnees);
  
	return *this;
}

// -----------------------------------------------------------------------------
//  Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSFlechiesInfo::operator==(const NSFlechiesInfo& o)
{
	 return (*pDonnees == *(o.pDonnees));
}

#ifdef _ENTERPRISE_DLL
void
NSFlechiesInfo::alimenteFiche(MYSQL_ROW *pRow)
{
  if ((MYSQL_ROW*) NULL == pRow)
    return;

  pDonnees->setId((*pRow)[FLECHIES_ID_FIELD - 1]);
  pDonnees->setLabel((*pRow)[FLECHIES_LIBELLE_FIELD - 1]);
	pDonnees->setCode((*pRow)[FLECHIES_CODE_FIELD - 1]);
}
#endif

//////////////////////////// fin du fichier flechiesDB.cpp

