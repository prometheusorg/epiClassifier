// -----------------------------------------------------------------------------
//    EPTABLES.CPP
//    NAUTILUS 2001-07
//
//    Implement database objects
// -----------------------------------------------------------------------------

#include <cstring>
#include <fstream>

#include "nsepisod/eptables.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/superLus.h"
#else
  #include "nautilus\nssuper.h"
#endif

#include "nssavoir/nspatbas.h"

/*
// -----------------------------------------------------------------------------
//
// Implémentation des méthodes NSEpiContact
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSEpiContactData::NSEpiContactData()
{
	// Met les champs de données à zéro
	metAZero();
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiContactData::NSEpiContactData(NSEpiContactData& rv)
{
	strcpy(patient, 		  rv.patient);
	strcpy(code,  		    rv.code);
	strcpy(dateHeureDeb, 	rv.dateHeureDeb);
	strcpy(dateHeureFin,  rv.dateHeureFin);
	strcpy(session,  		  rv.session);
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiContactData::~NSEpiContactData()
{
}


// -----------------------------------------------------------------------------
// copy assignment operator
// -----------------------------------------------------------------------------
NSEpiContactData&
NSEpiContactData::operator=(NSEpiContactData src)
{
  if (this == &src)
    return *this;

	strcpy(patient, 		  src.patient);
	strcpy(code,  		    src.code);
	strcpy(dateHeureDeb, 	src.dateHeureDeb);
	strcpy(dateHeureFin,  src.dateHeureFin);
	strcpy(session,  		  src.session);

	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiContactData::operator==(const NSEpiContactData& o)
{
	if ((strcmp(patient, o.patient) == 0) &&
      (strcmp(code,    o.code)    == 0))
		return 1;
	else
		return 0;
}

// -----------------------------------------------------------------------------
// Met à zéro les variables de la fiche
// -----------------------------------------------------------------------------
void
NSEpiContactData::metAZero()
{
	// Met les champs de données à zéro
	memset(patient, 	  	0, CONTACT_PATIENT_LEN + 1);
	memset(code,   		    0, CONTACT_CODE_LEN + 1);
	memset(dateHeureDeb, 	0, CONTACT_DATEHEURE_DEB_LEN + 1);
	memset(dateHeureFin,  0, CONTACT_DATEHEURE_FIN_LEN + 1);
	memset(session,  		  0, CONTACT_SESSION_LEN + 1);
}

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSEpiContact::NSEpiContact(NSContexte *pCtx)
  : NSEpiContactInfo(),
    NSFiche(pCtx)
{
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiContact::NSEpiContact(NSEpiContact& rv)
  : NSEpiContactInfo(rv),
    NSFiche(rv.pContexte)
{
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiContact::~NSEpiContact()
{
}


// -----------------------------------------------------------------------------
// Transfère le contenu de pRecBuff dans les variables de la fiche
// -----------------------------------------------------------------------------
void
NSEpiContact::alimenteFiche()
{
  alimenteChamp(pDonnees->patient, 	    CONTACT_PATIENT_FIELD,  	    CONTACT_PATIENT_LEN);
  alimenteChamp(pDonnees->code, 		    CONTACT_CODE_FIELD, 	        CONTACT_CODE_LEN);
  alimenteChamp(pDonnees->dateHeureDeb, CONTACT_DATEHEURE_DEB_FIELD,  CONTACT_DATEHEURE_DEB_LEN);
  alimenteChamp(pDonnees->dateHeureFin, CONTACT_DATEHEURE_FIN_FIELD,  CONTACT_DATEHEURE_FIN_LEN);
  alimenteChamp(pDonnees->session,  	  CONTACT_SESSION_FIELD, 	      CONTACT_SESSION_LEN);
}


// -----------------------------------------------------------------------------
// Transfère le contenu des valeurs de la fiche dans pRecBuff
// -----------------------------------------------------------------------------
void
NSEpiContact::videFiche()
{
	videChamp(pDonnees->patient, 	    CONTACT_PATIENT_FIELD,  	    CONTACT_PATIENT_LEN);
  videChamp(pDonnees->code, 		    CONTACT_CODE_FIELD, 	        CONTACT_CODE_LEN);
  videChamp(pDonnees->dateHeureDeb, CONTACT_DATEHEURE_DEB_FIELD,  CONTACT_DATEHEURE_DEB_LEN);
  videChamp(pDonnees->dateHeureFin, CONTACT_DATEHEURE_FIN_FIELD,  CONTACT_DATEHEURE_FIN_LEN);
  videChamp(pDonnees->session,  	  CONTACT_SESSION_FIELD, 	      CONTACT_SESSION_LEN);
}


// -----------------------------------------------------------------------------
// Ouvre la table primaire et les index secondaires
// -----------------------------------------------------------------------------
DBIResult
NSEpiContact::open()
{
	char tableName[] = "CONTACT.DB";

	// Appelle la fonction open() de la classe de base pour ouvrir l'index primaire
  lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return(lastError);
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool
NSEpiContact::Create()
{
	return true;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool
NSEpiContact::Modify()
{
	return true;
}


// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiContact&
NSEpiContact::operator=(NSEpiContact src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiContact::operator==(const NSEpiContact& o)
{
  return (*pDonnees == *(o.pDonnees));
}


// -----------------------------------------------------------------------------
// Constructor par défaut
// -----------------------------------------------------------------------------
NSEpiContactInfo::NSEpiContactInfo()
{
	// Crée l'objet de données
	pDonnees = new NSEpiContactData();
}


// -----------------------------------------------------------------------------
// Constructor à partir d'un NSEpiContact
// -----------------------------------------------------------------------------
NSEpiContactInfo::NSEpiContactInfo(NSEpiContact *pContact)
{
	// Crée l'objet de données
	pDonnees = new NSEpiContactData();

	// Copie les valeurs du NSEpiContact
	*pDonnees = *(pContact->pDonnees);
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiContactInfo::NSEpiContactInfo(NSEpiContactInfo& rv)
{
	// Crée l'objet de données
	pDonnees = new NSEpiContactData();

	// Copie les valeurs du NSEpiContactInfo d'origine
	*pDonnees = *(rv.pDonnees);
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiContactInfo::~NSEpiContactInfo()
{
	delete pDonnees;
}


// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiContactInfo&
NSEpiContactInfo::operator=(NSEpiContactInfo src)
{
  if (this == &src)
    return *this;
	*pDonnees = *(src.pDonnees);
	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiContactInfo::operator==(const NSEpiContactInfo& o)
{
  return (*pDonnees == *(o.pDonnees));
}


// -----------------------------------------------------------------------------
//
// Implémentation des méthodes NSEpiContElmt
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSEpiContElmtData::NSEpiContElmtData()
{
	// Met les champs de données à zéro
	metAZero();
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiContElmtData::NSEpiContElmtData(NSEpiContElmtData& rv)
{
	strcpy(contact, 	rv.contact);
	strcpy(code,  		rv.code);
	strcpy(preoccup,  rv.preoccup);
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiContElmtData::~NSEpiContElmtData()
{
}


// -----------------------------------------------------------------------------
// copy assignment operator
// -----------------------------------------------------------------------------
NSEpiContElmtData&
NSEpiContElmtData::operator=(NSEpiContElmtData src)
{
  if (this == &src)
    return *this;

	strcpy(contact, 	src.contact);
	strcpy(code,  		src.code);
	strcpy(preoccup,  src.preoccup);

	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiContElmtData::operator==(const NSEpiContElmtData& o)
{
	if ((strcmp(contact, o.contact) == 0) &&
      (strcmp(code,    o.code)    == 0))
		return 1;
	else
		return 0;
}

// -----------------------------------------------------------------------------
// Met à zéro les variables de la fiche
// -----------------------------------------------------------------------------
void
NSEpiContElmtData::metAZero()
{
	// Met les champs de données à zéro
	memset(contact, 	0, CONTELMT_CONTACT_LEN + 1);
	memset(code,   		0, CONTELMT_CODE_LEN + 1);
	memset(preoccup, 	0, CONTELMT_PREOCCUP_LEN + 1);
}


// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSEpiContElmt::NSEpiContElmt(NSContexte *pCtx)
  : NSEpiContElmtInfo(),
    NSFiche(pCtx)
{
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiContElmt::NSEpiContElmt(NSEpiContElmt& rv)
  : NSEpiContElmtInfo(rv),
    NSFiche(rv.pContexte)
{
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiContElmt::~NSEpiContElmt()
{
}


// -----------------------------------------------------------------------------
// Transfère le contenu de pRecBuff dans les variables de la fiche
// -----------------------------------------------------------------------------
void
NSEpiContElmt::alimenteFiche()
{
  alimenteChamp(pDonnees->contact,  CONTELMT_CONTACT_FIELD,   CONTELMT_CONTACT_LEN);
  alimenteChamp(pDonnees->code, 	  CONTELMT_CODE_FIELD, 	    CONTELMT_CODE_LEN);
  alimenteChamp(pDonnees->preoccup, CONTELMT_PREOCCUP_FIELD,  CONTELMT_PREOCCUP_LEN);
}


// -----------------------------------------------------------------------------
// Transfère le contenu des valeurs de la fiche dans pRecBuff
// -----------------------------------------------------------------------------
void
NSEpiContElmt::videFiche()
{
	videChamp(pDonnees->contact,  CONTELMT_CONTACT_FIELD,   CONTELMT_CONTACT_LEN);
  videChamp(pDonnees->code, 	  CONTELMT_CODE_FIELD, 	    CONTELMT_CODE_LEN);
  videChamp(pDonnees->preoccup, CONTELMT_PREOCCUP_FIELD,  CONTELMT_PREOCCUP_LEN);
}


// -----------------------------------------------------------------------------
// Ouvre la table primaire et les index secondaires
// -----------------------------------------------------------------------------
DBIResult
NSEpiContElmt::open()
{
	char tableName[] = "CON_ELMT.DB";

	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
  return (lastError);
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool
NSEpiContElmt::Create()
{
	return true;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool
NSEpiContElmt::Modify()
{
	return true;
}


// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiContElmt&
NSEpiContElmt::operator=(NSEpiContElmt src)
{
  if (this == &src)
    return *this;
	*pDonnees = *(src.pDonnees);
	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiContElmt::operator==(const NSEpiContElmt& o)
{
  return (*pDonnees == *(o.pDonnees));
}


// -----------------------------------------------------------------------------
// Constructor par défaut
// -----------------------------------------------------------------------------
NSEpiContElmtInfo::NSEpiContElmtInfo()
{
	// Crée l'objet de données
	pDonnees = new NSEpiContElmtData();
}


// -----------------------------------------------------------------------------
// Constructor à partir d'un NSEpiContElmt
// -----------------------------------------------------------------------------
NSEpiContElmtInfo::NSEpiContElmtInfo(NSEpiContElmt *pContact)
{
	// Crée l'objet de données
	pDonnees = new NSEpiContElmtData();

	// Copie les valeurs du NSEpiContElmt
	*pDonnees = *(pContact->pDonnees);
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiContElmtInfo::NSEpiContElmtInfo(NSEpiContElmtInfo& rv)
{
	// Crée l'objet de données
	pDonnees = new NSEpiContElmtData();

	// Copie les valeurs du NSEpiContElmtInfo d'origine
	*pDonnees = *(rv.pDonnees);
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiContElmtInfo::~NSEpiContElmtInfo()
{
	delete pDonnees;
}


// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiContElmtInfo&
NSEpiContElmtInfo::operator=(NSEpiContElmtInfo src)
{
  if (this == &src)
    return *this;
	*pDonnees = *(src.pDonnees);
	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiContElmtInfo::operator==(const NSEpiContElmtInfo& o)
{
  return (*pDonnees == *(o.pDonnees));
}


// -----------------------------------------------------------------------------
//
// Implémentation des méthodes NSEpiContClass
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSEpiContClasData::NSEpiContClasData()
{
	// Met les champs de données à zéro
	metAZero();
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiContClasData::NSEpiContClasData(NSEpiContClasData& rv)
{
	strcpy(contElement, rv.contElement);
	strcpy(numero,      rv.numero);
	strcpy(position,    rv.position);
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiContClasData::~NSEpiContClasData()
{
}


// -----------------------------------------------------------------------------
// copy assignment operator
// -----------------------------------------------------------------------------
NSEpiContClasData&
NSEpiContClasData::operator=(NSEpiContClasData src)
{
  if (this == &src)
    return *this;

	strcpy(contElement, src.contElement);
	strcpy(numero,      src.numero);
	strcpy(position,    src.position);

	return *this;
}


// -----------------------------------------------------------------------------
//  Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiContClasData::operator==(const NSEpiContClasData& o)
{
	if ((strcmp(contElement, o.contElement) == 0) &&
      (strcmp(numero,      o.numero)      == 0))
		return 1;
	else
		return 0;
}

// -----------------------------------------------------------------------------
// Met à zéro les variables de la fiche
// -----------------------------------------------------------------------------
void
NSEpiContClasData::metAZero()
{
	// Met les champs de données à zéro
	memset(contElement, 0, CONTCLAS_CONTELMT_LEN + 1);
	memset(numero,   	  0, CONTCLAS_NUMERO_LEN + 1);
	memset(position, 	  0, CONTCLAS_POSITION_LEN + 1);
}

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSEpiContClass::NSEpiContClass(NSContexte *pCtx)
  : NSEpiContClasInfo(),
    NSFiche(pCtx)
{
}

// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiContClass::NSEpiContClass(NSEpiContClass& rv)
  : NSEpiContClasInfo(rv),
    NSFiche(rv.pContexte)
{
}

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiContClass::~NSEpiContClass()
{
}

// -----------------------------------------------------------------------------
// Transfère le contenu de pRecBuff dans les variables de la fiche
// -----------------------------------------------------------------------------
void
NSEpiContClass::alimenteFiche()
{
  alimenteChamp(pDonnees->contElement,  CONTCLAS_CONTELMT_FIELD,  CONTCLAS_CONTELMT_LEN);
  alimenteChamp(pDonnees->numero, 		  CONTCLAS_NUMERO_FIELD, 	  CONTCLAS_NUMERO_LEN);
  alimenteChamp(pDonnees->position,     CONTCLAS_POSITION_FIELD,  CONTCLAS_POSITION_LEN);
}

// -----------------------------------------------------------------------------
// Transfère le contenu des valeurs de la fiche dans pRecBuff
// -----------------------------------------------------------------------------
void
NSEpiContClass::videFiche()
{
	videChamp(pDonnees->contElement,  CONTCLAS_CONTELMT_FIELD,  CONTCLAS_CONTELMT_LEN);
  videChamp(pDonnees->numero, 		  CONTCLAS_NUMERO_FIELD,    CONTCLAS_NUMERO_LEN);
  videChamp(pDonnees->position,     CONTCLAS_POSITION_FIELD,  CONTCLAS_POSITION_LEN);
}


// -----------------------------------------------------------------------------
// Ouvre la table primaire et les index secondaires
// -----------------------------------------------------------------------------
DBIResult
NSEpiContClass::open()
{
	char tableName[] = "CON_CLAS.DB";

	// Appelle la fonction open() de la classe de base pour ouvrir l'index primaire
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return (lastError);
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool
NSEpiContClass::Create()
{
  return true;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool
NSEpiContClass::Modify()
{
	return true;
}


// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiContClass&
NSEpiContClass::operator=(NSEpiContClass src)
{
  if (this == &src)
    return *this;
	*pDonnees = *(src.pDonnees);
	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiContClass::operator==(const NSEpiContClass& o)
{
  return (*pDonnees == *(o.pDonnees));
}


// -----------------------------------------------------------------------------
// Constructor par défaut
// -----------------------------------------------------------------------------
NSEpiContClasInfo::NSEpiContClasInfo()
{
	// Crée l'objet de données
	pDonnees = new NSEpiContClasData();
}


// -----------------------------------------------------------------------------
// Constructor à partir d'un NSEpiContClass
// -----------------------------------------------------------------------------
NSEpiContClasInfo::NSEpiContClasInfo(NSEpiContClass *pContact)
{
	// Crée l'objet de données
	pDonnees = new NSEpiContClasData();

	// Copie les valeurs du NSEpiContClass
	*pDonnees = *(pContact->pDonnees);
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiContClasInfo::NSEpiContClasInfo(NSEpiContClasInfo& rv)
{
	// Crée l'objet de données
	pDonnees = new NSEpiContClasData();

	// Copie les valeurs du NSEpiContClasInfo d'origine
	*pDonnees = *(rv.pDonnees);
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiContClasInfo::~NSEpiContClasInfo()
{
	delete pDonnees;
}


// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiContClasInfo&
NSEpiContClasInfo::operator=(NSEpiContClasInfo src)
{
  if (this == &src)
    return *this;
	*pDonnees = *(src.pDonnees);
	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int

NSEpiContClasInfo::operator==(const NSEpiContClasInfo& o)
{
  return (*pDonnees == *(o.pDonnees));
}


// -----------------------------------------------------------------------------
// Implémentation des méthodes NSEpiContCode
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSEpiContCodeData::NSEpiContCodeData()
{
	// Met les champs de données à zéro
	metAZero();
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiContCodeData::NSEpiContCodeData(NSEpiContCodeData& rv)
{
	strcpy(contElement,     rv.contElement);
	strcpy(numero,          rv.numero);
	strcpy(classification,  rv.classification);
	strcpy(code,            rv.code);
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiContCodeData::~NSEpiContCodeData()
{
}


// -----------------------------------------------------------------------------
// copy assignment operator
// -----------------------------------------------------------------------------
NSEpiContCodeData&
NSEpiContCodeData::operator=(NSEpiContCodeData src)
{
  if (this == &src)
    return *this;

	strcpy(contElement,     src.contElement);
	strcpy(numero,          src.numero);
	strcpy(classification,  src.classification);
	strcpy(code,            src.code);

	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiContCodeData::operator==(const NSEpiContCodeData& o)
{
	if ((strcmp(contElement, o.contElement) == 0) &&
      (strcmp(numero,      o.numero)      == 0))
		return 1;
	else
		return 0;
}

// -----------------------------------------------------------------------------
// Met à zéro les variables de la fiche
// -----------------------------------------------------------------------------
void
NSEpiContCodeData::metAZero()
{
	// Met les champs de données à zéro
	memset(contElement, 	  0, CONTCODE_CONTELMT_LEN + 1);
	memset(numero,   		    0, CONTCODE_NUMERO_LEN + 1);
	memset(classification,  0, CONTCODE_CLASSIF_LEN + 1);
	memset(code,  		      0, CONTCODE_CODE_LEN + 1);
}

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSEpiContCode::NSEpiContCode(NSContexte *pCtx)
              : NSEpiContCodeInfo(), NSFiche(pCtx)
{
}

// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiContCode::NSEpiContCode(NSEpiContCode& rv)
              : NSEpiContCodeInfo(rv), NSFiche(rv.pContexte)
{
}

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiContCode::~NSEpiContCode()
{
}

// -----------------------------------------------------------------------------
// Transfère le contenu de pRecBuff dans les variables de la fiche
// -----------------------------------------------------------------------------
void
NSEpiContCode::alimenteFiche()
{
  alimenteChamp(pDonnees->contElement, 	  CONTCODE_CONTELMT_FIELD,  CONTCODE_CONTELMT_LEN);
  alimenteChamp(pDonnees->numero, 		    CONTCODE_NUMERO_FIELD, 	  CONTCODE_NUMERO_LEN);
  alimenteChamp(pDonnees->classification, CONTCODE_CLASSIF_FIELD,   CONTCODE_CLASSIF_LEN);
  alimenteChamp(pDonnees->code,  	        CONTCODE_CODE_FIELD, 	    CONTCODE_CODE_LEN);
}

// -----------------------------------------------------------------------------
// Transfère le contenu des valeurs de la fiche dans pRecBuff
// -----------------------------------------------------------------------------
void
NSEpiContCode::videFiche()
{
	videChamp(pDonnees->contElement, 	  CONTCODE_CONTELMT_FIELD,  CONTCODE_CONTELMT_LEN);
  videChamp(pDonnees->numero, 		    CONTCODE_NUMERO_FIELD,    CONTCODE_NUMERO_LEN);
  videChamp(pDonnees->classification, CONTCODE_CLASSIF_FIELD,   CONTCODE_CLASSIF_LEN);
  videChamp(pDonnees->code,  	        CONTCODE_CODE_FIELD,      CONTCODE_CODE_LEN);
}

// -----------------------------------------------------------------------------
// Ouvre la table primaire et les index secondaires
// -----------------------------------------------------------------------------
DBIResult
NSEpiContCode::open()
{
	char tableName[] = "CON_CODE.DB";

	// Appelle la fonction open() de la classe de base pour ouvrir l'index primaire
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return (lastError);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool
NSEpiContCode::Create()
{
	return true;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool
NSEpiContCode::Modify()
{
	return true;
}

// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiContCode&
NSEpiContCode::operator=(NSEpiContCode src)
{
  if (this == &src)
    return *this;
	*pDonnees = *(src.pDonnees);
	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiContCode::operator==(const NSEpiContCode& o)
{
  return (*pDonnees == *(o.pDonnees));
}


// -----------------------------------------------------------------------------
// Constructor par défaut
// -----------------------------------------------------------------------------
NSEpiContCodeInfo::NSEpiContCodeInfo()
{
	// Crée l'objet de données
	pDonnees = new NSEpiContCodeData();
}


// -----------------------------------------------------------------------------
// Constructor à partir d'un NSEpiContCode
// -----------------------------------------------------------------------------
NSEpiContCodeInfo::NSEpiContCodeInfo(NSEpiContCode *pContact)
{
	// Crée l'objet de données
	pDonnees = new NSEpiContCodeData();

	// Copie les valeurs du NSEpiContCode
	*pDonnees = *(pContact->pDonnees);
}

// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiContCodeInfo::NSEpiContCodeInfo(NSEpiContCodeInfo& rv)
{
	// Crée l'objet de données
	pDonnees = new NSEpiContCodeData();

	// Copie les valeurs du NSEpiContCodeInfo d'origine
	*pDonnees = *(rv.pDonnees);
}

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiContCodeInfo::~NSEpiContCodeInfo()
{
	delete pDonnees;
}

// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiContCodeInfo&
NSEpiContCodeInfo::operator=(NSEpiContCodeInfo src)
{
  if (this == &src)
    return *this;
	*pDonnees = *(src.pDonnees);
	return *this;
}

// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiContCodeInfo::operator==(const NSEpiContCodeInfo& o)
{
  return (*pDonnees == *(o.pDonnees));
}


// -----------------------------------------------------------------------------
//
// Implémentation des méthodes NSEpiPreoccup
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSEpiPreoccupData::NSEpiPreoccupData()
{
	// Met les champs de données à zéro
	metAZero();
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiPreoccupData::NSEpiPreoccupData(NSEpiPreoccupData& rv)
{
	strcpy(patient,     rv.patient);
	strcpy(code,        rv.code);
	strcpy(revision,    rv.revision);
	strcpy(session,     rv.session);
	strcpy(lexique,  	  rv.lexique);
	strcpy(complement,  rv.complement);
	strcpy(dateCree,    rv.dateCree);
	strcpy(dateFerme,  	rv.dateFerme);
	strcpy(actif,  		  rv.actif);
	strcpy(recurrent,   rv.recurrent);
	strcpy(mere,  		  rv.mere);
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiPreoccupData::~NSEpiPreoccupData()
{
}


// -----------------------------------------------------------------------------
// copy assignment operator
// -----------------------------------------------------------------------------
NSEpiPreoccupData&
NSEpiPreoccupData::operator=(NSEpiPreoccupData src)
{
  if (this == &src)
    return *this;

	strcpy(patient,     src.patient);
	strcpy(code,        src.code);
	strcpy(revision,    src.revision);
	strcpy(session,     src.session);
	strcpy(lexique,  	  src.lexique);
	strcpy(complement,  src.complement);
	strcpy(dateCree,    src.dateCree);
	strcpy(dateFerme,  	src.dateFerme);
	strcpy(actif,  		  src.actif);
	strcpy(recurrent,  	src.recurrent);
	strcpy(mere,  		  src.mere);

	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiPreoccupData::operator==(const NSEpiPreoccupData& o)
{
	if ((strcmp(patient,  o.patient)  == 0) &&
      (strcmp(code,     o.code)     == 0) &&
      (strcmp(revision, o.revision) == 0))
		return 1;
	else
		return 0;
}

// -----------------------------------------------------------------------------
// Met à zéro les variables de la fiche
// -----------------------------------------------------------------------------
void
NSEpiPreoccupData::metAZero()
{
	// Met les champs de données à zéro
	memset(patient,     0, PREOCCUP_PATIENT_LEN + 1);
	memset(code,   		  0, PREOCCUP_CODE_LEN + 1);
	memset(revision, 	  0, PREOCCUP_REVISION_LEN + 1);
	memset(session,     0, PREOCCUP_SESSION_LEN + 1);
	memset(lexique,  	  0, PREOCCUP_LEXIQUE_LEN + 1);
	memset(complement,  0, PREOCCUP_COMPLEMENT_LEN + 1);
	memset(dateCree,    0, PREOCCUP_DATE_CREE_LEN + 1);
	memset(dateFerme,  	0, PREOCCUP_DATE_FERME_LEN + 1);
	memset(actif,  		  0, PREOCCUP_ACTIF_LEN + 1);
	memset(recurrent,  	0, PREOCCUP_RECURRENT_LEN + 1);
	memset(mere,  		  0, PREOCCUP_MERE_LEN + 1);
}

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSEpiPreoccup::NSEpiPreoccup(NSContexte *pCtx)
  : NSEpiPreoccupInfo(),
    NSFiche(pCtx)
{
}

// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiPreoccup::NSEpiPreoccup(NSEpiPreoccup& rv)
  : NSEpiPreoccupInfo(rv),
    NSFiche(rv.pContexte)
{
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiPreoccup::~NSEpiPreoccup()
{
}


// -----------------------------------------------------------------------------
// Transfère le contenu de pRecBuff dans les variables de la fiche
// -----------------------------------------------------------------------------
void
NSEpiPreoccup::alimenteFiche()
{
  alimenteChamp(pDonnees->patient,    PREOCCUP_PATIENT_FIELD,     PREOCCUP_PATIENT_LEN);
  alimenteChamp(pDonnees->code, 		  PREOCCUP_CODE_FIELD, 	      PREOCCUP_CODE_LEN);
  alimenteChamp(pDonnees->revision,   PREOCCUP_REVISION_FIELD,    PREOCCUP_REVISION_LEN);
  alimenteChamp(pDonnees->session,    PREOCCUP_SESSION_FIELD,     PREOCCUP_SESSION_LEN);
  alimenteChamp(pDonnees->lexique,  	PREOCCUP_LEXIQUE_FIELD, 	  PREOCCUP_LEXIQUE_LEN);
  alimenteChamp(pDonnees->complement, PREOCCUP_COMPLEMENT_FIELD,  PREOCCUP_COMPLEMENT_LEN);
  alimenteChamp(pDonnees->dateCree,   PREOCCUP_DATE_CREE_FIELD,   PREOCCUP_DATE_CREE_LEN);
  alimenteChamp(pDonnees->dateFerme,  PREOCCUP_DATE_FERME_FIELD,  PREOCCUP_DATE_FERME_LEN);
  alimenteChamp(pDonnees->actif,  	  PREOCCUP_ACTIF_FIELD,       PREOCCUP_ACTIF_LEN);
  alimenteChamp(pDonnees->recurrent,  PREOCCUP_RECURRENT_FIELD,   PREOCCUP_RECURRENT_LEN);
  alimenteChamp(pDonnees->mere,  	    PREOCCUP_MERE_FIELD,        PREOCCUP_MERE_LEN);
}


// -----------------------------------------------------------------------------
// Transfère le contenu des valeurs de la fiche dans pRecBuff
// -----------------------------------------------------------------------------
void
NSEpiPreoccup::videFiche()
{
  videChamp(pDonnees->patient,    PREOCCUP_PATIENT_FIELD,     PREOCCUP_PATIENT_LEN);
  videChamp(pDonnees->code, 		  PREOCCUP_CODE_FIELD, 	      PREOCCUP_CODE_LEN);
  videChamp(pDonnees->revision,   PREOCCUP_REVISION_FIELD,    PREOCCUP_REVISION_LEN);
  videChamp(pDonnees->session,    PREOCCUP_SESSION_FIELD,     PREOCCUP_SESSION_LEN);
  videChamp(pDonnees->lexique,  	PREOCCUP_LEXIQUE_FIELD, 	  PREOCCUP_LEXIQUE_LEN);
  videChamp(pDonnees->complement, PREOCCUP_COMPLEMENT_FIELD,  PREOCCUP_COMPLEMENT_LEN);
  videChamp(pDonnees->dateCree,   PREOCCUP_DATE_CREE_FIELD,   PREOCCUP_DATE_CREE_LEN);
  videChamp(pDonnees->dateFerme,  PREOCCUP_DATE_FERME_FIELD,  PREOCCUP_DATE_FERME_LEN);
  videChamp(pDonnees->actif,  	  PREOCCUP_ACTIF_FIELD,       PREOCCUP_ACTIF_LEN);
  videChamp(pDonnees->recurrent,  PREOCCUP_RECURRENT_FIELD,   PREOCCUP_RECURRENT_LEN);
  videChamp(pDonnees->mere,  	    PREOCCUP_MERE_FIELD,        PREOCCUP_MERE_LEN);
}


// -----------------------------------------------------------------------------
// Ouvre la table primaire et les index secondaires
// -----------------------------------------------------------------------------
DBIResult
NSEpiPreoccup::open()
{
	char tableName[] = "PREOCCUP.DB";

	// Appelle la fonction open() de la classe de base pour ouvrir l'index primaire
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return (lastError);
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool
NSEpiPreoccup::Create()
{
	return true;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool
NSEpiPreoccup::Modify()
{
	return true;
}


// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiPreoccup&
NSEpiPreoccup::operator=(NSEpiPreoccup src)
{
  if (this == &src)
    return *this;
	*pDonnees = *(src.pDonnees);
	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiPreoccup::operator==(const NSEpiPreoccup& o)
{
  return (*pDonnees == *(o.pDonnees));
}


// -----------------------------------------------------------------------------
// Constructor par défaut
// -----------------------------------------------------------------------------
NSEpiPreoccupInfo::NSEpiPreoccupInfo()
{
	// Crée l'objet de données
	pDonnees = new NSEpiPreoccupData();
}


// -----------------------------------------------------------------------------
// Constructor à partir d'un NSEpiPreoccup
// -----------------------------------------------------------------------------
NSEpiPreoccupInfo::NSEpiPreoccupInfo(NSEpiPreoccup *pContact)
{
	// Crée l'objet de données
	pDonnees = new NSEpiPreoccupData();

	// Copie les valeurs du NSEpiPreoccup
	*pDonnees = *(pContact->pDonnees);
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiPreoccupInfo::NSEpiPreoccupInfo(NSEpiPreoccupInfo& rv)
{
	// Crée l'objet de données
	pDonnees = new NSEpiPreoccupData();

	// Copie les valeurs du NSEpiPreoccupInfo d'origine
	*pDonnees = *(rv.pDonnees);
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiPreoccupInfo::~NSEpiPreoccupInfo()
{
	delete pDonnees;
}


void
NSEpiPreoccupInfo::initActif(bool bActif)
{
  if (bActif)
    pDonnees->actif[0] = '1';
  else
    pDonnees->actif[0] = '0';
}


void
NSEpiPreoccupInfo::initRecurrent(bool bRecurrent)
{
  if (bRecurrent)
    pDonnees->recurrent[0] = '1';
  else
    pDonnees->recurrent[0] = '0';
}


// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiPreoccupInfo&
NSEpiPreoccupInfo::operator=(NSEpiPreoccupInfo src)
{
  if (this == &src)
    return *this;
	*pDonnees = *(src.pDonnees);
	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiPreoccupInfo::operator==(const NSEpiPreoccupInfo& o)
{
  return (*pDonnees == *(o.pDonnees));
}


// -----------------------------------------------------------------------------
//
// Implémentation des méthodes NSEpiProbleme
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSEpiProblemeData::NSEpiProblemeData()
{
	// Met les champs de données à zéro
	metAZero();
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiProblemeData::NSEpiProblemeData(NSEpiProblemeData& rv)
{
	strcpy(preoccupation, rv.preoccupation);
	strcpy(code,          rv.code);
	strcpy(revision,      rv.revision);
	strcpy(session,       rv.session);
	strcpy(signifiant,    rv.signifiant);
	strcpy(severite,      rv.severite);
	strcpy(lexique,  		  rv.lexique);
	strcpy(complement,    rv.complement);
	strcpy(certitude,     rv.certitude);
	strcpy(origine,       rv.origine);
	strcpy(fin,           rv.fin);
	strcpy(dateDebut,     rv.dateDebut);
	strcpy(dateCree,      rv.dateCree);
	strcpy(dateFerme,  		rv.dateFerme);
	strcpy(dateObso,  		rv.dateObso);
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiProblemeData::~NSEpiProblemeData()
{
}


// -----------------------------------------------------------------------------
// copy assignment operator
// -----------------------------------------------------------------------------
NSEpiProblemeData&
NSEpiProblemeData::operator=(NSEpiProblemeData src)
{
  if (this == &src)
    return *this;

	strcpy(preoccupation, src.preoccupation);
	strcpy(code,          src.code);
	strcpy(revision,      src.revision);
	strcpy(session,       src.session);
	strcpy(signifiant,    src.signifiant);
	strcpy(severite,      src.severite);
	strcpy(lexique,  		  src.lexique);
	strcpy(complement,    src.complement);
	strcpy(certitude,     src.certitude);
	strcpy(origine,       src.origine);
	strcpy(fin,           src.fin);
	strcpy(dateDebut,     src.dateDebut);
	strcpy(dateCree,      src.dateCree);
	strcpy(dateFerme,  		src.dateFerme);
	strcpy(dateObso,  		src.dateObso);

	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiProblemeData::operator==(const NSEpiProblemeData& o)
{
	if ((strcmp(preoccupation, o.preoccupation) == 0) &&
      (strcmp(code,          o.code)          == 0) &&
      (strcmp(revision,      o.revision)      == 0))
		return 1;
	else
		return 0;
}

// -----------------------------------------------------------------------------
// Met à zéro les variables de la fiche
// -----------------------------------------------------------------------------
void
NSEpiProblemeData::metAZero()
{
	// Met les champs de données à zéro
	memset(preoccupation, 0, PROBLEME_PREOCCUP_LEN + 1);
	memset(code,   		    0, PROBLEME_CODE_LEN + 1);
	memset(revision, 	    0, PROBLEME_REVISION_LEN + 1);
	memset(session,       0, PROBLEME_SESSION_LEN + 1);
	memset(signifiant,  	0, PROBLEME_SIGNIFIANT_LEN + 1);
	memset(severite,      0, PROBLEME_SEVERITE_LEN + 1);
	memset(lexique,  		  0, PROBLEME_LEXIQUE_LEN + 1);
	memset(complement, 	  0, PROBLEME_COMPLEMENT_LEN + 1);
	memset(certitude,   	0, PROBLEME_CERTITUDE_LEN + 1);
	memset(origine, 	    0, PROBLEME_ORIGINE_LEN + 1);
	memset(fin,           0, PROBLEME_FIN_LEN + 1);
	memset(dateDebut,  		0, PROBLEME_DATE_DEBUT_LEN + 1);
	memset(dateCree,      0, PROBLEME_DATE_CREE_LEN + 1);
	memset(dateFerme,  		0, PROBLEME_DATE_FERME_LEN + 1);
	memset(dateObso,  		0, PROBLEME_DATE_OBSO_LEN + 1);
}


// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSEpiProbleme::NSEpiProbleme(NSContexte *pCtx)
  : NSEpiProblemeInfo(),
    NSFiche(pCtx)
{
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiProbleme::NSEpiProbleme(NSEpiProbleme& rv)
  : NSEpiProblemeInfo(rv),
    NSFiche(rv.pContexte)
{
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiProbleme::~NSEpiProbleme()
{
}


// -----------------------------------------------------------------------------
// Transfère le contenu de pRecBuff dans les variables de la fiche
// -----------------------------------------------------------------------------
void
NSEpiProbleme::alimenteFiche()
{
  alimenteChamp(pDonnees->preoccupation,  PROBLEME_PREOCCUP_FIELD,    PROBLEME_PREOCCUP_LEN);
  alimenteChamp(pDonnees->code, 		      PROBLEME_CODE_FIELD, 	      PROBLEME_CODE_LEN);
  alimenteChamp(pDonnees->revision,       PROBLEME_REVISION_FIELD,    PROBLEME_REVISION_LEN);
  alimenteChamp(pDonnees->session,        PROBLEME_SESSION_FIELD,     PROBLEME_SESSION_LEN);
  alimenteChamp(pDonnees->signifiant,     PROBLEME_SIGNIFIANT_FIELD,  PROBLEME_SIGNIFIANT_LEN);
  alimenteChamp(pDonnees->severite,       PROBLEME_SEVERITE_FIELD,    PROBLEME_SEVERITE_LEN);
  alimenteChamp(pDonnees->lexique,  	    PROBLEME_LEXIQUE_FIELD,     PROBLEME_LEXIQUE_LEN);
  alimenteChamp(pDonnees->complement,     PROBLEME_COMPLEMENT_FIELD,  PROBLEME_COMPLEMENT_LEN);
  alimenteChamp(pDonnees->certitude, 	    PROBLEME_CERTITUDE_FIELD,   PROBLEME_CERTITUDE_LEN);
  alimenteChamp(pDonnees->origine,        PROBLEME_ORIGINE_FIELD,     PROBLEME_ORIGINE_LEN);
  alimenteChamp(pDonnees->fin,            PROBLEME_FIN_FIELD,         PROBLEME_FIN_LEN);
  alimenteChamp(pDonnees->dateDebut,      PROBLEME_DATE_DEBUT_FIELD,  PROBLEME_DATE_DEBUT_LEN);
  alimenteChamp(pDonnees->dateCree,       PROBLEME_DATE_CREE_FIELD,   PROBLEME_DATE_CREE_LEN);
  alimenteChamp(pDonnees->dateFerme,      PROBLEME_DATE_FERME_FIELD,  PROBLEME_DATE_FERME_LEN);
  alimenteChamp(pDonnees->dateObso,  	    PROBLEME_DATE_OBSO_FIELD,   PROBLEME_DATE_OBSO_LEN);
}


// -----------------------------------------------------------------------------
// Transfère le contenu des valeurs de la fiche dans pRecBuff
// -----------------------------------------------------------------------------
void
NSEpiProbleme::videFiche()
{
  videChamp(pDonnees->preoccupation,  PROBLEME_PREOCCUP_FIELD,    PROBLEME_PREOCCUP_LEN);
  videChamp(pDonnees->code, 		      PROBLEME_CODE_FIELD, 	      PROBLEME_CODE_LEN);
  videChamp(pDonnees->revision,       PROBLEME_REVISION_FIELD,    PROBLEME_REVISION_LEN);
  videChamp(pDonnees->session,        PROBLEME_SESSION_FIELD,     PROBLEME_SESSION_LEN);
  videChamp(pDonnees->signifiant,     PROBLEME_SIGNIFIANT_FIELD,  PROBLEME_SIGNIFIANT_LEN);
  videChamp(pDonnees->severite,       PROBLEME_SEVERITE_FIELD,    PROBLEME_SEVERITE_LEN);
  videChamp(pDonnees->lexique,  	    PROBLEME_LEXIQUE_FIELD, 	  PROBLEME_LEXIQUE_LEN);
  videChamp(pDonnees->complement,     PROBLEME_COMPLEMENT_FIELD,  PROBLEME_COMPLEMENT_LEN);
  videChamp(pDonnees->certitude, 	    PROBLEME_CERTITUDE_FIELD,   PROBLEME_CERTITUDE_LEN);
  videChamp(pDonnees->origine,        PROBLEME_ORIGINE_FIELD,     PROBLEME_ORIGINE_LEN);
  videChamp(pDonnees->fin,            PROBLEME_FIN_FIELD,         PROBLEME_FIN_LEN);
  videChamp(pDonnees->dateDebut,      PROBLEME_DATE_DEBUT_FIELD,  PROBLEME_DATE_DEBUT_LEN);
  videChamp(pDonnees->dateCree,       PROBLEME_DATE_CREE_FIELD,   PROBLEME_DATE_CREE_LEN);
  videChamp(pDonnees->dateFerme,      PROBLEME_DATE_FERME_FIELD,  PROBLEME_DATE_FERME_LEN);
  videChamp(pDonnees->dateObso,  	    PROBLEME_DATE_OBSO_FIELD,   PROBLEME_DATE_OBSO_LEN);
}


// -----------------------------------------------------------------------------
// Ouvre la table primaire et les index secondaires
// -----------------------------------------------------------------------------
DBIResult
NSEpiProbleme::open()
{
	char tableName[] = "PROBLEME.DB";

	// Appelle la fonction open() de la classe de base pour ouvrir l'index primaire
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return (lastError);
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool
NSEpiProbleme::Create()
{
	return true;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool
NSEpiProbleme::Modify()
{
	return true;
}


// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiProbleme&
NSEpiProbleme::operator=(NSEpiProbleme src)
{
  if (this == &src)
    return *this;
	*pDonnees = *(src.pDonnees);
	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiProbleme::operator==(const NSEpiProbleme& o)
{
  return (*pDonnees == *(o.pDonnees));
}


// -----------------------------------------------------------------------------
// Constructor par défaut
// -----------------------------------------------------------------------------
NSEpiProblemeInfo::NSEpiProblemeInfo()
{
	// Crée l'objet de données
	pDonnees = new NSEpiProblemeData();
}


// -----------------------------------------------------------------------------
// Constructor à partir d'un NSEpiProbleme
// -----------------------------------------------------------------------------
NSEpiProblemeInfo::NSEpiProblemeInfo(NSEpiProbleme *pContact)
{
	// Crée l'objet de données
	pDonnees = new NSEpiProblemeData();

	// Copie les valeurs du NSEpiProbleme
	*pDonnees = *(pContact->pDonnees);
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiProblemeInfo::NSEpiProblemeInfo(NSEpiProblemeInfo& rv)
{
	// Crée l'objet de données
	pDonnees = new NSEpiProblemeData();

	// Copie les valeurs du NSEpiProblemeInfo d'origine
	*pDonnees = *(rv.pDonnees);
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiProblemeInfo::~NSEpiProblemeInfo()
{
	delete pDonnees;
}


// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiProblemeInfo&
NSEpiProblemeInfo::operator=(NSEpiProblemeInfo src)
{
  if (this == &src)
    return *this;
	*pDonnees = *(src.pDonnees);
	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiProblemeInfo::operator==(const NSEpiProblemeInfo& o)
{
  return (*pDonnees == *(o.pDonnees));
}


// -----------------------------------------------------------------------------
//
// Implémentation des méthodes NSEpiProbEven
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSEpiProbEvenData::NSEpiProbEvenData()
{
	// Met les champs de données à zéro
	metAZero();
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiProbEvenData::NSEpiProbEvenData(NSEpiProbEvenData& rv)
{
	strcpy(probleme,    rv.probleme);
	strcpy(code,        rv.code);
	strcpy(revision,    rv.revision);
	strcpy(session,     rv.session);
	strcpy(lexique,  	  rv.lexique);
	strcpy(complement,  rv.complement);
	strcpy(certitude,   rv.certitude);
	strcpy(severite,    rv.severite);
	strcpy(dateHeure,   rv.dateHeure);
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiProbEvenData::~NSEpiProbEvenData()
{
}


// -----------------------------------------------------------------------------
// copy assignment operator
// -----------------------------------------------------------------------------
NSEpiProbEvenData&
NSEpiProbEvenData::operator=(NSEpiProbEvenData src)
{
  if (this == &src)
    return *this;

	strcpy(probleme,    src.probleme);
	strcpy(code,        src.code);
	strcpy(revision,    src.revision);
	strcpy(session,     src.session);
	strcpy(lexique,  	  src.lexique);
	strcpy(complement,  src.complement);
	strcpy(certitude,   src.certitude);
	strcpy(severite,    src.severite);
	strcpy(dateHeure,   src.dateHeure);

	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiProbEvenData::operator == ( const NSEpiProbEvenData& o )
{
	if ((strcmp(probleme, o.probleme) == 0) &&
      (strcmp(code,     o.code)     == 0) &&
      (strcmp(revision, o.revision) == 0))
		return 1;
	else
		return 0;
}

// -----------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
// -----------------------------------------------------------------------------
void
NSEpiProbEvenData::metAZero()
{
	// Met les champs de données à zéro
	memset(probleme,    0, PROBEVEN_PROBLEME_LEN + 1);
	memset(code,   		  0, PROBEVEN_CODE_LEN + 1);
	memset(revision, 	  0, PROBEVEN_REVISION_LEN + 1);
	memset(session,     0, PROBEVEN_SESSION_LEN + 1);
	memset(lexique,  	  0, PROBEVEN_LEXIQUE_LEN + 1);
	memset(complement,  0, PROBEVEN_COMPLEMENT_LEN + 1);
  memset(certitude,   0, PROBEVEN_CERTITUDE_LEN + 1);
	memset(severite, 	  0, PROBEVEN_SEVERITE_LEN + 1);
	memset(dateHeure,   0, PROBEVEN_DATE_HEURE_LEN + 1);
}

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSEpiProbEven::NSEpiProbEven(NSContexte *pCtx)
  : NSEpiProbEvenInfo(),
    NSFiche(pCtx)
{
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiProbEven::NSEpiProbEven(NSEpiProbEven& rv)
  : NSEpiProbEvenInfo(rv),
    NSFiche(rv.pContexte)
{
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiProbEven::~NSEpiProbEven()
{
}


// -----------------------------------------------------------------------------
// Transfère le contenu de pRecBuff dans les variables de la fiche
// -----------------------------------------------------------------------------
void
NSEpiProbEven::alimenteFiche()
{
  alimenteChamp(pDonnees->probleme,   PROBEVEN_PROBLEME_FIELD,    PROBEVEN_PROBLEME_LEN);
  alimenteChamp(pDonnees->code, 		  PROBEVEN_CODE_FIELD, 	      PROBEVEN_CODE_LEN);
  alimenteChamp(pDonnees->revision,   PROBEVEN_REVISION_FIELD,    PROBEVEN_REVISION_LEN);
  alimenteChamp(pDonnees->session,    PROBEVEN_SESSION_FIELD,     PROBEVEN_SESSION_LEN);
  alimenteChamp(pDonnees->lexique,  	PROBEVEN_LEXIQUE_FIELD,     PROBEVEN_LEXIQUE_LEN);
  alimenteChamp(pDonnees->complement, PROBEVEN_COMPLEMENT_FIELD,  PROBEVEN_COMPLEMENT_LEN);
  alimenteChamp(pDonnees->certitude, 	PROBEVEN_CERTITUDE_FIELD,   PROBEVEN_CERTITUDE_LEN);
  alimenteChamp(pDonnees->severite,   PROBEVEN_SEVERITE_FIELD,    PROBEVEN_SEVERITE_LEN);
  alimenteChamp(pDonnees->dateHeure,  PROBEVEN_DATE_HEURE_FIELD,  PROBEVEN_DATE_HEURE_LEN);
}


// -----------------------------------------------------------------------------
// Transfère le contenu des valeurs de la fiche dans pRecBuff
// -----------------------------------------------------------------------------
void
NSEpiProbEven::videFiche()
{
  videChamp(pDonnees->probleme,   PROBEVEN_PROBLEME_FIELD,    PROBEVEN_PROBLEME_LEN);
  videChamp(pDonnees->code, 		  PROBEVEN_CODE_FIELD, 	      PROBEVEN_CODE_LEN);
  videChamp(pDonnees->revision,   PROBEVEN_REVISION_FIELD,    PROBEVEN_REVISION_LEN);
  videChamp(pDonnees->session,    PROBEVEN_SESSION_FIELD,     PROBEVEN_SESSION_LEN);
  videChamp(pDonnees->lexique,  	PROBEVEN_LEXIQUE_FIELD,     PROBEVEN_LEXIQUE_LEN);
  videChamp(pDonnees->complement, PROBEVEN_COMPLEMENT_FIELD,  PROBEVEN_COMPLEMENT_LEN);
  videChamp(pDonnees->certitude, 	PROBEVEN_CERTITUDE_FIELD,   PROBEVEN_CERTITUDE_LEN);
  videChamp(pDonnees->severite,   PROBEVEN_SEVERITE_FIELD,    PROBEVEN_SEVERITE_LEN);
  videChamp(pDonnees->dateHeure,  PROBEVEN_DATE_HEURE_FIELD,  PROBEVEN_DATE_HEURE_LEN);
}


// -----------------------------------------------------------------------------
// Ouvre la table primaire et les index secondaires
// -----------------------------------------------------------------------------
DBIResult
NSEpiProbEven::open()
{
	char tableName[] = "PROBEVENT.DB";

	// Appelle la fonction open() de la classe de base pour ouvrir l'index primaire
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return (lastError);
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool
NSEpiProbEven::Create()
{
	return true;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool
NSEpiProbEven::Modify()
{
	return true;
}


// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiProbEven&
NSEpiProbEven::operator=(NSEpiProbEven src)
{
  if (this == &src)
    return *this;
	*pDonnees = *(src.pDonnees);
	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiProbEven::operator==(const NSEpiProbEven& o)
{
  return (*pDonnees == *(o.pDonnees));
}


// -----------------------------------------------------------------------------
// Constructor par défaut
// -----------------------------------------------------------------------------
NSEpiProbEvenInfo::NSEpiProbEvenInfo()
{
	// Crée l'objet de données
	pDonnees = new NSEpiProbEvenData();
}


// -----------------------------------------------------------------------------
// Constructor à partir d'un NSEpiProbEven
// -----------------------------------------------------------------------------
NSEpiProbEvenInfo::NSEpiProbEvenInfo(NSEpiProbEven *pContact)
{
	// Crée l'objet de données
	pDonnees = new NSEpiProbEvenData();

	// Copie les valeurs du NSEpiProbEven
	*pDonnees = *(pContact->pDonnees);
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiProbEvenInfo::NSEpiProbEvenInfo(NSEpiProbEvenInfo& rv)
{
	// Crée l'objet de données
	pDonnees = new NSEpiProbEvenData();

	// Copie les valeurs du NSEpiProbEvenInfo d'origine
	*pDonnees = *(rv.pDonnees);
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiProbEvenInfo::~NSEpiProbEvenInfo()
{
	delete pDonnees;
}


// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiProbEvenInfo&
NSEpiProbEvenInfo::operator=(NSEpiProbEvenInfo src)
{
  if (this == &src)
    return *this;
	*pDonnees = *(src.pDonnees);
	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int
NSEpiProbEvenInfo::operator==(const NSEpiProbEvenInfo& o)
{
  return (*pDonnees == *(o.pDonnees));
}

*/

// -----------------------------------------------------------------------------
//
// Implementation of NSEpiClasser methods
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSEpiClasserData::NSEpiClasserData()
{
	// Met les champs de données à zéro
	metAZero();
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiClasserData::NSEpiClasserData(const NSEpiClasserData& rv)
{
	_sCode           = rv._sCode;
	_sClassification = rv._sClassification;
	_sCriteria       = rv._sCriteria;
	_sRelation       = rv._sRelation;
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiClasserData::~NSEpiClasserData()
{
}

// -----------------------------------------------------------------------------
// Operator =
// -----------------------------------------------------------------------------
NSEpiClasserData&
NSEpiClasserData::operator=(const NSEpiClasserData& src)
{
  if (this == &src)
    return *this;

	_sCode           = src._sCode;
	_sClassification = src._sClassification;
	_sCriteria       = src._sCriteria;
	_sRelation       = src._sRelation;

	return *this;
}


// -----------------------------------------------------------------------------
// Comparison operator
// -----------------------------------------------------------------------------
int
NSEpiClasserData::operator==(const NSEpiClasserData& o)
{
	if ((_sCode           == o._sCode)           &&
      (_sClassification == o._sClassification) &&
      (_sCriteria       == o._sCriteria)       &&
      (_sRelation       == o._sRelation)       )
		return 1;
	else
		return 0;
}

// -----------------------------------------------------------------------------
// Met à zéro les variables de la fiche
// -----------------------------------------------------------------------------
void NSEpiClasserData::metAZero() {
    _sCode.clear();
    _sClassification.clear();
    _sCriteria.clear();
    _sRelation.clear();
}

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSEpiClasser::NSEpiClasser(NSSuper *pSuper)
             :NSEpiClasserInfo(),
              NSFiche(pSuper)
{
}

// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiClasser::NSEpiClasser(NSEpiClasser& rv)
             :NSEpiClasserInfo(rv),
              NSFiche(rv._pSuper)
{
}

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiClasser::~NSEpiClasser()
{
}

// -----------------------------------------------------------------------------
// Donne l'ensemble des relations pour un code, une relation, un critère
// -----------------------------------------------------------------------------
DBIResult NSEpiClasser::donneRelations(string *pResult,
                                       string sCode,
                                       string sClassification,
                                       string sCritere) {
  if (NULL == pResult)
    return DBIERR_NONE;

  pResult->clear();

  bool bOpenClose = false;
  if (isOpen != true)
    bOpenClose = true;

  // Ouverture de la table - Open the table
  if (bOpenClose)
  {
    lastError = open();
    if (lastError != DBIERR_NONE)
      return lastError;
  }

  // Recherche de la première relation - Looking for the first proper relation
  string cle = sCode + sClassification;

  CURProps  curProps;
  DBIResult lastError   = DbiGetCursorProps(PrendCurseur(), curProps);
  Byte      *pIndexRec  = new Byte[curProps.iRecBufSize];
  memset(pIndexRec, 0, curProps.iRecBufSize);
  DbiPutField(PrendCurseur(), CLASSER_CODE_FIELD,    pIndexRec, (Byte *)sCode.c_str());
  DbiPutField(PrendCurseur(), CLASSER_CLASSIF_FIELD, pIndexRec, (Byte *)sClassification.c_str());

  lastError = chercheClefComposite("CODAGE", NODEFAULTINDEX, keySEARCHGEQ, dbiWRITELOCK, pIndexRec);
  delete[] pIndexRec;

  if ((lastError != DBIERR_NONE) && (lastError != DBIERR_EOF))
	{
    if (bOpenClose)
      close();
		return lastError;
  }

  if (lastError == DBIERR_EOF)
	{
    if (bOpenClose)
      close();
		return DBIERR_NONE;
  }

  lastError = getRecord();
  if (lastError != DBIERR_NONE)
	{
    if (bOpenClose)
      close();
		return lastError;
  }

  // On vérifie que la fiche trouvée appartient bien au document
  if ((_Donnees.getCode()           != sCode)           ||
      (_Donnees.getClassification() != sClassification))
  {
    if (bOpenClose)
      close();
		return DBIERR_NONE;
	}

  // On avance dans le fichier tant que les fiches trouvées sont OK
  while ( (lastError                    != DBIERR_EOF)  &&
          (_Donnees.getCode()           == sCode)       &&
          (_Donnees.getClassification() == sClassification))
  {
    if ((_Donnees.getCriteria() == sCritere) &&
        (!_Donnees.getRelation().empty())
    {
      if (!pResult->empty())
        *pResult += string(";");
      *pResult += _Donnees.getRelation();
    }

    lastError = suivant(dbiWRITELOCK);
    if ((lastError != DBIERR_NONE) && (lastError != DBIERR_EOF))
		{
      if (bOpenClose)
        close();
      return lastError;
		}

    if (lastError != DBIERR_EOF)
    {
      lastError = getRecord();
      if (lastError != DBIERR_NONE)
			{
        if (bOpenClose)
          close();
        return lastError;
			}
    }
  }

  // Fermeture du fichier
  if (bOpenClose)
    return close();

  return DBIERR_NONE;
}

// -----------------------------------------------------------------------------
// Transfère le contenu de pRecBuff dans les variables de la fiche
// -----------------------------------------------------------------------------
void
NSEpiClasser::alimenteFiche()
{
  char code[CLASSER_CODE_LEN + 1];
  char classification[CLASSER_CLASSIF_LEN + 1];  char critere[CLASSER_CRITERE_LEN + 1];  char relation[CLASSER_RELATION_LEN + 1];

  alimenteChamp(code, 		      CLASSER_CODE_FIELD,     CLASSER_CODE_LEN);
  alimenteChamp(classification, CLASSER_CLASSIF_FIELD,  CLASSER_CLASSIF_LEN);
  alimenteChamp(critere,        CLASSER_CRITERE_FIELD,  CLASSER_CRITERE_LEN);
  alimenteChamp(relation,  	    CLASSER_RELATION_FIELD, CLASSER_RELATION_LEN);

  _Donnees.setCode(code);
  _Donnees.setClassification(classification);
  _Donnees.setCriteria(critere);
  _Donnees.setRelation(relation);
}

// -----------------------------------------------------------------------------
// Transfère le contenu des valeurs de la fiche dans pRecBuff
// -----------------------------------------------------------------------------
void NSEpiClasser::videFiche() {
  videChamp((char*)(_Donnees.getCode().c_str()),           CLASSER_CODE_FIELD,     CLASSER_CODE_LEN);
  videChamp((char*)(_Donnees.getClassification().c_str()), CLASSER_CLASSIF_FIELD,  CLASSER_CLASSIF_LEN);
  videChamp((char*)(_Donnees.getCriteria().c_str()),       CLASSER_CRITERE_FIELD,  CLASSER_CRITERE_LEN);
  videChamp((char*)(_Donnees.getRelation().c_str()),       CLASSER_RELATION_FIELD, CLASSER_RELATION_LEN);
}

// -----------------------------------------------------------------------------
// Ouvre la table primaire et les index secondaires
// -----------------------------------------------------------------------------
DBIResult NSEpiClasser::open()
{
	char tableName[] = "CLASSER.DB";

	// Appelle la fonction open() de la classe de base pour ouvrir l'index primaire
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return (lastError);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool NSEpiClasser::Create() {
	return true;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool NSEpiClasser::Modify() {
	return true;
}


// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiClasser& NSEpiClasser::operator=(NSEpiClasser src) {
  if (this == &src)
    return *this;

	_Donnees = src._Donnees;

	return *this;
}

// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int NSEpiClasser::operator==(const NSEpiClasser& o) {
  return (_Donnees == o._Donnees);
}
#endif

// -----------------------------------------------------------------------------
// Constructor par défaut
// -----------------------------------------------------------------------------
NSEpiClasserInfo::NSEpiClasserInfo() {
}

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------------------
// Constructor à partir d'un NSEpiClasser
// -----------------------------------------------------------------------------
NSEpiClasserInfo::NSEpiClasserInfo(NSEpiClasser *pContact) {
	_Donnees = pContact->_Donnees;
}
#endif

// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiClasserInfo::NSEpiClasserInfo(NSEpiClasserInfo& rv)
{
	_Donnees = rv._Donnees;
}

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiClasserInfo::~NSEpiClasserInfo()
{
}

// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiClasserInfo&
NSEpiClasserInfo::operator=(NSEpiClasserInfo src)
{
  if (this == &src)
    return *this;

	_Donnees = src._Donnees;

  return *this;
}

// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int NSEpiClasserInfo::operator==(const NSEpiClasserInfo& o) {
  return (_Donnees == o._Donnees);
}

#ifdef _ENTERPRISE_DLL

void
NSEpiClasserInfo::alimenteFiche(MYSQL_ROW *pRow)
{
  if ((MYSQL_ROW*) NULL == pRow)
    return;

  _Donnees.metAZero();

  if ((*pRow)[CLASSER_CODE_FIELD - 1])
    _Donnees.setCode((*pRow)[CLASSER_CODE_FIELD - 1]);
  if ((*pRow)[CLASSER_CLASSIF_FIELD - 1])
    _Donnees.setClassification((*pRow)[CLASSER_CLASSIF_FIELD - 1]);
  if ((*pRow)[CLASSER_CRITERE_FIELD - 1])
	  _Donnees.setCriteria((*pRow)[CLASSER_CRITERE_FIELD - 1]);
  if ((*pRow)[CLASSER_RELATION_FIELD - 1])
    _Donnees.setRelation((*pRow)[CLASSER_RELATION_FIELD - 1]);
}

// -----------------------------------------------------------------------------
// Get every existing relations for a code, a relation, a criteria
// -----------------------------------------------------------------------------
bool
NSEpiClasserInfo::donneRelations(string *pResult, string sCode, string sClassification, string sCritere, ontologyBaseManager* pOntologyManager)
{
  if ((NULL == pOntologyManager) || (NULL == pResult))
    return false;

  *pResult = string("");

  // Looking for all codes that belong to given classification
  //
  string sQuery = string("SELECT * FROM classer WHERE CODE = \"") + sCode + string("\" AND CLASSIFICATION = \"") + sClassification + string("\"");
  if (string("") != sCritere)
    sQuery += string(" AND CRITERE = \"") + sCritere + string("\"");

  MYSQL_RES *pSqlResult = pOntologyManager->executeQuery(sQuery, false);
  if ((MYSQL_RES *) NULL == pSqlResult)
    return false;

  MYSQL_ROW Row = mysql_fetch_row(pSqlResult);
  while (Row)
  {
    _Donnees.metAZero();
    alimenteFiche(&Row);

    if (string("") != *pResult)
      *pResult += string(";");
    *pResult += _Donnees.getRelation();

    Row = mysql_fetch_row(pSqlResult);
  }

  mysql_free_result(pSqlResult);

  return true;
}
#endif

//******************** THESAURUS ********************

// -----------------------------------------------------------------------------
//
// Implement NSThesaurus methods
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSThesaurusData::NSThesaurusData()
{
	// Met les champs de données à zéro
	metAZero();
}


// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSThesaurusData::NSThesaurusData(NSThesaurusData& rv)
{
  _iId      = rv._iId;
  _sOrigine = rv._sOrigine;
  _ibui     = rv._ibui;
  _labelFr  = rv._labelFr;
  _labelNl  = rv._labelNl;
  _icpc2    = rv._icpc2;
  _icd10    = rv._icd10;
  _icpc2_2  = rv._icpc2_2;
  _icd10_2  = rv._icd10_2;
  _icpc2_1X = rv._icpc2_1X;
  _icd10_1X = rv._icd10_1X;
  _icpc2_2X = rv._icpc2_2X;
  _icd10_2X = rv._icd10_2X;
  _icpc2_1Y = rv._icpc2_1Y;
  _icd10_1Y = rv._icd10_1Y;
  _icpc2_2Y = rv._icpc2_2Y;
  _icd10_2Y = rv._icd10_2Y;
}

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSThesaurusData::~NSThesaurusData()
{
}

// -----------------------------------------------------------------------------
// copy assignment operator
// -----------------------------------------------------------------------------
NSThesaurusData&
NSThesaurusData::operator=(NSThesaurusData src)
{
  if (this == &src)
    return *this;

  _iId      = src._iId;
  _sOrigine = src._sOrigine;
	_ibui     = src._ibui;
  _labelFr  = src._labelFr;
  _labelNl  = src._labelNl;
  _icpc2    = src._icpc2;
  _icd10    = src._icd10;
  _icpc2_2  = src._icpc2_2;
  _icd10_2  = src._icd10_2;
  _icpc2_1X = src._icpc2_1X;
  _icd10_1X = src._icd10_1X;
  _icpc2_2X = src._icpc2_2X;
  _icd10_2X = src._icd10_2X;
  _icpc2_1Y = src._icpc2_1Y;
  _icd10_1Y = src._icd10_1Y;
  _icpc2_2Y = src._icpc2_2Y;
  _icd10_2Y = src._icd10_2Y;

	return *this;
}

// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int NSThesaurusData::operator==(const NSThesaurusData& o) {
	if (_ibui == o._ibui)
		return 1;
	else
		return 0;
}

// -----------------------------------------------------------------------------
// Met à zéro les variables de la fiche
// -----------------------------------------------------------------------------
void NSThesaurusData::metAZero() {
  _iId      = -1;
  _sOrigine.clear();
    _ibui.clear();
  _labelFr.clear();
  _labelNl.clear();
  _icpc2.clear();
  _icd10.clear();
  _icpc2_2.clear();
  _icd10_2.clear();
  _icpc2_1X.clear();
  _icd10_1X.clear();
  _icpc2_2X.clear();
  _icd10_2X.clear();
  _icpc2_1Y.clear();
  _icd10_1Y.clear();
  _icpc2_2Y.clear();
  _icd10_2Y.clear();
}

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSThesaurus::NSThesaurus(NSSuper *pSuper)
            :NSThesaurusInfo(), NSFiche(pSuper)
{
}

// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSThesaurus::NSThesaurus(NSThesaurus& rv)
            :NSThesaurusInfo(rv), NSFiche(rv._pSuper)
{
}

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSThesaurus::~NSThesaurus()
{
}

// -----------------------------------------------------------------------------
// Transfère le contenu de pRecBuff dans les variables de la fiche
// -----------------------------------------------------------------------------
void NSThesaurus::alimenteFiche() {
  char ibui[THESAURUS_IBUI_LEN + 1];
  char labelFr[THESAURUS_LABEL_LEN + 1];
  char labelNl[THESAURUS_LABEL_LEN + 1];
  char icpc2[THESAURUS_ICPC2_LEN + 1];
  char icd10[THESAURUS_ICD10_LEN + 1];
  char icpc2_2[THESAURUS_ICPC2_LEN + 1];
  char icd10_2[THESAURUS_ICD10_LEN + 1];
  char icpc2_1X[THESAURUS_ICPC2_LEN + 1];
  char icd10_1X[THESAURUS_ICD10_LEN + 1];
  char icpc2_2X[THESAURUS_ICPC2_LEN + 1];
  char icd10_2X[THESAURUS_ICD10_LEN + 1];
  char icpc2_1Y[THESAURUS_ICPC2_LEN + 1];
  char icd10_1Y[THESAURUS_ICD10_LEN + 1];
  char icpc2_2Y[THESAURUS_ICPC2_LEN + 1];
  char icd10_2Y[THESAURUS_ICD10_LEN + 1];

  alimenteChamp(ibui,			THESAURUS_IBUI_FIELD,     THESAURUS_IBUI_LEN);
  alimenteChamp(labelFr,	THESAURUS_FR_LABEL_FIELD,	THESAURUS_LABEL_LEN);
  alimenteChamp(labelNl,	THESAURUS_NL_LABEL_FIELD,	THESAURUS_LABEL_LEN);
  alimenteChamp(icpc2,		THESAURUS_ICPC2_FIELD,		THESAURUS_ICPC2_LEN);
  alimenteChamp(icd10,		THESAURUS_ICD10_FIELD,		THESAURUS_ICD10_LEN);
  alimenteChamp(icpc2_2,  THESAURUS_ICPC2_2_FIELD,  THESAURUS_ICPC2_LEN);
  alimenteChamp(icd10_2,  THESAURUS_ICD10_2_FIELD,  THESAURUS_ICD10_LEN);
  alimenteChamp(icpc2_1X, THESAURUS_ICPC2_1X_FIELD, THESAURUS_ICPC2_LEN);
  alimenteChamp(icd10_1X, THESAURUS_ICD10_1X_FIELD, THESAURUS_ICD10_LEN);
  alimenteChamp(icpc2_2X, THESAURUS_ICPC2_2X_FIELD, THESAURUS_ICPC2_LEN);
  alimenteChamp(icd10_2X, THESAURUS_ICD10_2X_FIELD, THESAURUS_ICD10_LEN);
  alimenteChamp(icpc2_1Y, THESAURUS_ICPC2_1Y_FIELD, THESAURUS_ICPC2_LEN);
  alimenteChamp(icd10_1Y, THESAURUS_ICD10_1Y_FIELD, THESAURUS_ICD10_LEN);
  alimenteChamp(icpc2_2Y, THESAURUS_ICPC2_2Y_FIELD, THESAURUS_ICPC2_LEN);
  alimenteChamp(icd10_2Y, THESAURUS_ICD10_2Y_FIELD, THESAURUS_ICD10_LEN);

  _Donnees._ibui     = string(ibui);
  _Donnees._labelFr  = string(labelFr);
  _Donnees._labelNl  = string(labelNl);
  _Donnees._icpc2    = string(icpc2);
  _Donnees._icd10    = string(icd10);
  _Donnees._icpc2_2  = string(icpc2_2);
  _Donnees._icd10_2  = string(icd10_2);
  _Donnees._icpc2_1X = string(icpc2_1X);
  _Donnees._icd10_1X = string(icd10_1X);
  _Donnees._icpc2_2X = string(icpc2_2X);
  _Donnees._icd10_2X = string(icd10_2X);
  _Donnees._icpc2_1Y = string(icpc2_1Y);
  _Donnees._icd10_1Y = string(icd10_1Y);
  _Donnees._icpc2_2Y = string(icpc2_2Y);
  _Donnees._icd10_2Y = string(icd10_2Y);
}

// -----------------------------------------------------------------------------
// Transfère le contenu des valeurs de la fiche dans pRecBuff
// -----------------------------------------------------------------------------
void NSThesaurus::videFiche()
{
  videChamp((char*)(_Donnees._ibui.c_str()),		 THESAURUS_IBUI_FIELD,     THESAURUS_IBUI_LEN);
  videChamp((char*)(_Donnees._labelFr.c_str()),	 THESAURUS_FR_LABEL_FIELD, THESAURUS_LABEL_LEN);
  videChamp((char*)(_Donnees._labelNl.c_str()),	 THESAURUS_NL_LABEL_FIELD, THESAURUS_LABEL_LEN);
  videChamp((char*)(_Donnees._icpc2.c_str()),		 THESAURUS_ICPC2_FIELD,		 THESAURUS_ICPC2_LEN);
  videChamp((char*)(_Donnees._icd10.c_str()),		 THESAURUS_ICD10_FIELD,		 THESAURUS_ICD10_LEN);
  videChamp((char*)(_Donnees._icpc2_2.c_str()),  THESAURUS_ICPC2_2_FIELD,  THESAURUS_ICPC2_LEN);
  videChamp((char*)(_Donnees._icd10_2.c_str()),  THESAURUS_ICD10_2_FIELD,  THESAURUS_ICD10_LEN);
  videChamp((char*)(_Donnees._icpc2_1X.c_str()), THESAURUS_ICPC2_1X_FIELD, THESAURUS_ICPC2_LEN);
  videChamp((char*)(_Donnees._icd10_1X.c_str()), THESAURUS_ICD10_1X_FIELD, THESAURUS_ICD10_LEN);
  videChamp((char*)(_Donnees._icpc2_2X.c_str()), THESAURUS_ICPC2_2X_FIELD, THESAURUS_ICPC2_LEN);
  videChamp((char*)(_Donnees._icd10_2X.c_str()), THESAURUS_ICD10_2X_FIELD, THESAURUS_ICD10_LEN);
  videChamp((char*)(_Donnees._icpc2_1Y.c_str()), THESAURUS_ICPC2_1Y_FIELD, THESAURUS_ICPC2_LEN);
  videChamp((char*)(_Donnees._icd10_1Y.c_str()), THESAURUS_ICD10_1Y_FIELD, THESAURUS_ICD10_LEN);
  videChamp((char*)(_Donnees._icpc2_2Y.c_str()), THESAURUS_ICPC2_2Y_FIELD, THESAURUS_ICPC2_LEN);
  videChamp((char*)(_Donnees._icd10_2Y.c_str()), THESAURUS_ICD10_2Y_FIELD, THESAURUS_ICD10_LEN);
}

// -----------------------------------------------------------------------------
// Ouvre la table primaire et les index secondaires
// -----------------------------------------------------------------------------
DBIResult NSThesaurus::open()
{
	char tableName[] = "Clinical_Labels.DB";

	// Appelle la fonction open() de la classe de base pour ouvrir l'index primaire
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return (lastError);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool NSThesaurus::Create()
{
	return true;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool NSThesaurus::Modify()
{
	return true;
}

// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSThesaurus& NSThesaurus::operator=(NSThesaurus src)
{
  if (this == &src)
    return *this;

	_Donnees = src._Donnees;

	return *this;
}

// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int NSThesaurus::operator==(const NSThesaurus& o)
{
  return (_Donnees == o._Donnees);
}
#endif

// -----------------------------------------------------------------------------
// Constructor par défaut
// -----------------------------------------------------------------------------
NSThesaurusInfo::NSThesaurusInfo()
{
}

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------------------
// Constructor à partir d'un NSEpiClasser
// -----------------------------------------------------------------------------
NSThesaurusInfo::NSThesaurusInfo(NSThesaurus *pContact)
{
	_Donnees = pContact->_Donnees;
}
#endif

// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSThesaurusInfo::NSThesaurusInfo(NSThesaurusInfo& rv)
{
	_Donnees = rv._Donnees;
}

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSThesaurusInfo::~NSThesaurusInfo()
{
}

// -----------------------------------------------------------------------------
// arrange the result "as a" Classer record for a given classification
// -----------------------------------------------------------------------------
string NSThesaurusInfo::getClasserString(string sClassif) {
  if (sClassif.empty())
    sClassif = string("6CISP");

  string sReturn;

  if (strlen(sClassif.c_str()) < BASE_SENS_LEN)
    return sReturn;

  if (string("6CISP") == string(sClassif, 0, BASE_SENS_LEN))
  {
    addALaClasser(&sReturn, string(_Donnees._icpc2));
    addALaClasser(&sReturn, string(_Donnees._icpc2_2));
    addALaClasser(&sReturn, string(_Donnees._icpc2_1X));
    addALaClasser(&sReturn, string(_Donnees._icpc2_2X));
    addALaClasser(&sReturn, string(_Donnees._icpc2_1Y));
    addALaClasser(&sReturn, string(_Donnees._icpc2_2Y));
  }
  else if (string("6CIMA") == string(sClassif, 0, BASE_SENS_LEN))
  {
    addALaClasser(&sReturn, string(_Donnees._icd10));
    addALaClasser(&sReturn, string(_Donnees._icd10_2));
    addALaClasser(&sReturn, string(_Donnees._icd10_1X));
    addALaClasser(&sReturn, string(_Donnees._icd10_2X));
    addALaClasser(&sReturn, string(_Donnees._icd10_1Y));
    addALaClasser(&sReturn, string(_Donnees._icd10_2Y));
  }
  return sReturn;
}

void NSThesaurusInfo::addALaClasser(string* pResult, string sCode)
{
  if ((NULL == pResult) || (sCode.empty()))
    return;

  string sAdaptedCode = sCode;

  // Procedure codes start by '*' and are more than 3 chars long
  //
  if ('*' == sAdaptedCode[0])
  {
    sAdaptedCode[0] = '?';
    sAdaptedCode = string(sAdaptedCode, 0, 3);
  }

  if (!pResult->empty())
    *pResult += string(";");

  *pResult += sAdaptedCode;
}

// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSThesaurusInfo&
NSThesaurusInfo::operator=(NSThesaurusInfo src)
{
  if (this == &src)
    return *this;

	_Donnees = src._Donnees;

  return *this;
}

// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int NSThesaurusInfo::operator==(const NSThesaurusInfo& o) {
  return (_Donnees == o._Donnees);
}

#ifdef _ENTERPRISE_DLL
void
NSThesaurusInfo::alimenteFiche(MYSQL_ROW *pRow)
{
  if ((MYSQL_ROW*) NULL == pRow)
    return;

  _Donnees.metAZero();

  if ((*pRow)[THESAURUS_ID_FIELD - 1])
    _Donnees._iId		   = StringToInt((*pRow)[THESAURUS_ID_FIELD - 1]);
  if ((*pRow)[THESAURUS_ORIGIN_FIELD - 1])
    _Donnees._sOrigine = (*pRow)[THESAURUS_ORIGIN_FIELD - 1];
  if ((*pRow)[THESAURUS_IBUI_FIELD - 1])
    _Donnees._ibui		 = (*pRow)[THESAURUS_IBUI_FIELD - 1];
  if ((*pRow)[THESAURUS_FR_LABEL_FIELD - 1])
    _Donnees._labelFr	 = (*pRow)[THESAURUS_FR_LABEL_FIELD - 1];
  if ((*pRow)[THESAURUS_NL_LABEL_FIELD - 1])
    _Donnees._labelNl	 = (*pRow)[THESAURUS_NL_LABEL_FIELD - 1];
  if ((*pRow)[THESAURUS_ICPC2_FIELD - 1])
    _Donnees._icpc2		 = (*pRow)[THESAURUS_ICPC2_FIELD - 1];
  if ((*pRow)[THESAURUS_ICD10_FIELD - 1])
    _Donnees._icd10		 = (*pRow)[THESAURUS_ICD10_FIELD - 1];
  if ((*pRow)[THESAURUS_ICPC2_2_FIELD - 1])
    _Donnees._icpc2_2  = (*pRow)[THESAURUS_ICPC2_2_FIELD - 1];
  if ((*pRow)[THESAURUS_ICD10_2_FIELD - 1])
    _Donnees._icd10_2  = (*pRow)[THESAURUS_ICD10_2_FIELD - 1];
  if ((*pRow)[THESAURUS_ICPC2_1X_FIELD - 1])
    _Donnees._icpc2_1X = (*pRow)[THESAURUS_ICPC2_1X_FIELD - 1];
  if ((*pRow)[THESAURUS_ICD10_1X_FIELD - 1])
    _Donnees._icd10_1X = (*pRow)[THESAURUS_ICD10_1X_FIELD - 1];
  if ((*pRow)[THESAURUS_ICPC2_2X_FIELD - 1])
    _Donnees._icpc2_2X = (*pRow)[THESAURUS_ICPC2_2X_FIELD - 1];
  if ((*pRow)[THESAURUS_ICD10_2X_FIELD - 1])
    _Donnees._icd10_2X = (*pRow)[THESAURUS_ICD10_2X_FIELD - 1];
  if ((*pRow)[THESAURUS_ICPC2_1Y_FIELD - 1])
    _Donnees._icpc2_1Y = (*pRow)[THESAURUS_ICPC2_1Y_FIELD - 1];
  if ((*pRow)[THESAURUS_ICD10_1Y_FIELD - 1])
    _Donnees._icd10_1Y = (*pRow)[THESAURUS_ICD10_1Y_FIELD - 1];
  if ((*pRow)[THESAURUS_ICPC2_2Y_FIELD - 1])
    _Donnees._icpc2_2Y = (*pRow)[THESAURUS_ICPC2_2Y_FIELD - 1];
  if ((*pRow)[THESAURUS_ICD10_2Y_FIELD - 1])
    _Donnees._icd10_2Y = (*pRow)[THESAURUS_ICD10_2Y_FIELD - 1];
}

void
NSThesaurusInfo::alimenteFicheSentinelle(MYSQL_ROW *pRow)
{
  if ((MYSQL_ROW*) NULL == pRow)
    return;

  _Donnees.metAZero();

  if ((*pRow)[1])
    _Donnees._labelFr = (*pRow)[1];
  if ((*pRow)[2])
    _Donnees._icpc2   = (*pRow)[2];
}

#endif

// -----------------------------------------------------------------------------
//
// Implémentation des méthodes NSEpiClassif
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
NSEpiClassifData::NSEpiClassifData()
{
	// Met les champs de données à zéro
	metAZero();
}

// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiClassifData::NSEpiClassifData(const NSEpiClassifData& rv)
{
  _sClassification = rv._sClassification;
	_sCode           = rv._sCode;
	_sLabel          = rv._sLabel;
	_sChapter        = rv._sChapter;
}

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiClassifData::~NSEpiClassifData()
{
}

// -----------------------------------------------------------------------------
// copy assignment operator
// -----------------------------------------------------------------------------
NSEpiClassifData& NSEpiClassifData::operator=(const NSEpiClassifData& src) {
  if (this == &src)
    return *this;

	_sClassification = src._sClassification;
	_sCode           = src._sCode;
	_sLabel          = src._sLabel;
	_sChapter        = src._sChapter;

	return *this;
}

// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int NSEpiClassifData::operator==(const NSEpiClassifData& o) {
	if ((_sCode           == o._sCode)           &&
      (_sClassification == o._sClassification) &&
      (_sLabel          == o._sLabel)          &&
      (_sChapter        == o._sChapter))
		return 1;
	else
		return 0;
}

// -----------------------------------------------------------------------------
// Met à zéro les variables de la fiche
// -----------------------------------------------------------------------------
void NSEpiClassifData::metAZero() {
    _sClassification.clear();
    _sCode.clear();
    _sLabel.clear();
    _sChapter.clear();
}

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------------------
//  Constructor
// -----------------------------------------------------------------------------
NSEpiClassif::NSEpiClassif(NSSuper *pSuper)
             :NSEpiClassifInfo(), NSFiche(pSuper)
{
}

// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiClassif::NSEpiClassif(NSEpiClassif& rv)
             :NSEpiClassifInfo(rv), NSFiche(rv._pSuper)
{
}

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiClassif::~NSEpiClassif() {
}

#ifdef _ENTERPRISE_DLL

void
NSEpiClassif::alimenteFiche(MYSQL_ROW *pRow)
{
  if ((MYSQL_ROW*) NULL == pRow)
    return;

  _Donnees.metAZero();

  if ((*pRow)[CLASSIF_CLASSIF_FIELD - 1])
    _Donnees.setClassification((*pRow)[CLASSIF_CLASSIF_FIELD - 1]);
  if ((*pRow)[CLASSIF_CODE_FIELD - 1])
    _Donnees.setCode((*pRow)[CLASSIF_CODE_FIELD - 1]);
  if ((*pRow)[CLASSIF_LIBELLE_FIELD - 1])
	  _Donnees.setLabel((*pRow)[CLASSIF_LIBELLE_FIELD - 1]);
  if ((*pRow)[CLASSIF_CHAPITRE_FIELD - 1])
    _Donnees.setChapter((*pRow)[CLASSIF_CHAPITRE_FIELD - 1]);
}

#else

// -----------------------------------------------------------------------------
// Transfère le contenu de pRecBuff dans les variables de la fiche
// -----------------------------------------------------------------------------
void NSEpiClassif::alimenteFiche() {
  char classification[CLASSIF_CLASSIF_LEN + 1];
  char code[CLASSIF_CODE_LEN + 1];  char libelle[CLASSIF_LIBELLE_LEN + 1];  char chapitre[CLASSIF_CHAPITRE_LEN + 1];

  alimenteChamp(classification, CLASSIF_CLASSIF_FIELD,  CLASSIF_CLASSIF_LEN);
  alimenteChamp(code, 		      CLASSIF_CODE_FIELD,     CLASSIF_CODE_LEN);
  alimenteChamp(libelle,        CLASSIF_LIBELLE_FIELD,  CLASSIF_LIBELLE_LEN);
  alimenteChamp(chapitre,  	    CLASSIF_CHAPITRE_FIELD, CLASSIF_CHAPITRE_LEN);

  _Donnees.setClassification(classification);
  _Donnees.setCode(code);
  _Donnees.setLabel(libelle);
  _Donnees.setChapter(chapitre);
}

// -----------------------------------------------------------------------------
// Transfère le contenu des valeurs de la fiche dans pRecBuff
// -----------------------------------------------------------------------------
void NSEpiClassif::videFiche() {
  videChamp((char*)(_Donnees.getClassification().c_str()), CLASSIF_CLASSIF_FIELD,  CLASSIF_CLASSIF_LEN);
  videChamp((char*)(_Donnees.getCode().c_str()),           CLASSIF_CODE_FIELD,     CLASSIF_CODE_LEN);
  videChamp((char*)(_Donnees.getLabel().c_str()),          CLASSIF_LIBELLE_FIELD,  CLASSIF_LIBELLE_LEN);
  videChamp((char*)(_Donnees.getChapter().c_str()),        CLASSIF_CHAPITRE_FIELD, CLASSIF_CHAPITRE_LEN);
}

// -----------------------------------------------------------------------------
// Ouvre la table primaire et les index secondaires
// -----------------------------------------------------------------------------
DBIResult NSEpiClassif::open() {
	char tableName[] = "CLASSIF.DB";

	// Appelle la fonction open() de la classe de base pour ouvrir l'index primaire
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return(lastError);
}

#endif

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool NSEpiClassif::Create() {
	return true;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool NSEpiClassif::Modify() {
	return true;
}

// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiClassif&
NSEpiClassif::operator=(NSEpiClassif src)
{
  if (&src == this)
    return *this;

	_Donnees = src._Donnees;

	return *this;
}


// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int NSEpiClassif::operator==(const NSEpiClassif& o) {
  return (_Donnees == o._Donnees);
}


bool NSEpiClassif::initArrayFromCodeList(string sClassification,
                                         ClasseStringVector* pVectCodes,
                                         NSEpiClassifArray* pResultArray) {
  if ((NULL == pVectCodes) || (NULL == pResultArray))
    return false;

  pResultArray->vider();

  if (pVectCodes->empty())
    return true;

  for (iterClassString itCodes = pVectCodes->begin(); pVectCodes->end() != itCodes; itCodes++)
  {
    bool bSuccess = false;

    if (strlen((*itCodes)->getItem().c_str()) <= CLASSIF_CODE_LEN)
    {
      CURProps curProps;
      /* DBIResult lastError = */ DbiGetCursorProps(PrendCurseur(), curProps);
      Byte* pIndexRec = new Byte[curProps.iRecBufSize];
      memset(pIndexRec, 0, curProps.iRecBufSize);
      DbiPutField(PrendCurseur(), CLASSIF_CLASSIF_FIELD, pIndexRec, (Byte*) sClassification.c_str());
      DbiPutField(PrendCurseur(), CLASSIF_CODE_FIELD,    pIndexRec, (Byte*) (*itCodes)->getItem().c_str());

      lastError = chercheClefComposite("",
                                       0,
                                       keySEARCHEQ,
                                       dbiWRITELOCK,
                                       pIndexRec);
      delete[] pIndexRec;

      if (lastError != DBIERR_NONE)
      {
        string sErrorText = string("Cannot find ") + (*itCodes)->getItem() + string(" in Classif.db");
        _pSuper->trace(&sErrorText, 1, NSSuper::trError);
      }
      else
      {
        lastError = getRecord();
        if (lastError != DBIERR_NONE)
        {
          string sErrorText = string("Problem reading ") + (*itCodes)->getItem() + string(" in Classif.db");
          _pSuper->trace(&sErrorText, 1, NSSuper::trError);
        }
        else
          bSuccess = true;
      }
    }

    if (bSuccess)
      pResultArray->push_back(new NSEpiClassifInfo(this));
    else
      pResultArray->push_back(new NSEpiClassifInfo());
  }

  return true;
}
#endif // #ifndef _ENTERPRISE_DLL

// -----------------------------------------------------------------------------
// Constructor par défaut
// -----------------------------------------------------------------------------
NSEpiClassifInfo::NSEpiClassifInfo()
{
}

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------------------
// Constructor à partir d'un NSEpiClasser
// -----------------------------------------------------------------------------
NSEpiClassifInfo::NSEpiClassifInfo(NSEpiClassif *pContact)
{
	_Donnees = pContact->_Donnees;
}
#endif

// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSEpiClassifInfo::NSEpiClassifInfo(NSEpiClassifInfo& rv)
{
	_Donnees = rv._Donnees;
}

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
NSEpiClassifInfo::~NSEpiClassifInfo()
{
}

// -----------------------------------------------------------------------------
// Get ICPC category
// Based ICPC's design : symptoms < 30, pathologies >= 70, procedures in-between
// -----------------------------------------------------------------------------
ICPC_CATEGORY NSEpiClassifInfo::getIcpcCategory() {
  // Valid ICPC codes must be in the form Char Digit Digit (ex : R05)
  //
  if (strlen(_Donnees.getCode().c_str()) < 3)
    return IcpcUndefined;

  if (!isdigit(_Donnees.getCode()[1]) || !isdigit(_Donnees.getCode()[2]))
    return IcpcUndefined;

  if (_Donnees.getCode()[1] < '3')
    return IcpcSymptom;

  if (_Donnees.getCode()[1] >= '7')
    return IcpcPathology;

  return IcpcProcedure;
}

ICPC_PATHOLOGY NSEpiClassifInfo::getIcpcPathology() {
  return IcpcUndefinedPatho;
}

// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
NSEpiClassifInfo& NSEpiClassifInfo::operator=(const NSEpiClassifInfo& src) {
  if (&src == this)
    return *this;

	_Donnees = src._Donnees;

	return *this;
}

#ifndef _ENTERPRISE_DLL
NSEpiClassifInfo& NSEpiClassifInfo::operator=(const NSEpiClassif& src) {
  _Donnees = src._Donnees;
  return *this;
}
#endif

// -----------------------------------------------------------------------------
// Opérateur de comparaison
// -----------------------------------------------------------------------------
int NSEpiClassifInfo::operator==(const NSEpiClassifInfo& o) {
  return (_Donnees == o._Donnees);
}

#ifdef _ENTERPRISE_DLL
void
NSEpiClassifInfo::alimenteFiche(MYSQL_ROW *pRow)
{
  if ((MYSQL_ROW*) NULL == pRow)
    return;

  _Donnees.metAZero();

  if ((*pRow)[CLASSIF_CLASSIF_FIELD - 1])
    _Donnees.setClassification((*pRow)[CLASSIF_CLASSIF_FIELD - 1]);
  if ((*pRow)[CLASSIF_CODE_FIELD - 1])
    _Donnees.setCode((*pRow)[CLASSIF_CODE_FIELD - 1]);
  if ((*pRow)[CLASSIF_LIBELLE_FIELD - 1])
	  _Donnees.setLabel((*pRow)[CLASSIF_LIBELLE_FIELD - 1]);
  if ((*pRow)[CLASSIF_CHAPITRE_FIELD - 1])
    _Donnees.setChapter((*pRow)[CLASSIF_CHAPITRE_FIELD - 1]);
}
#endif

NSEpiClassifInfoArray::~NSEpiClassifInfoArray()
{
  vider();
}

bool NSEpiClassifInfoArray::ajouteElement(NSEpiClassifInfo *pElement) {
  // Si l'élément existe déjà, on ne fait rien
  if (false == empty())
    for (NSEpiClassifInfoIter i = begin(); end() != i; i++)
      if (*(*i) == *pElement)
        return false;

  push_back(pElement);

  return true;
}

void NSEpiClassifInfoArray::vider() {
  if (empty())
    return;

  for (NSEpiClassifInfoIter i = begin(); end() != i; )
  {
    delete (*i);
    erase(i);
  }
}

// -----------------------------------------------------------------------------
//
// Implement NSEpiClassifArray methods
//
// -----------------------------------------------------------------------------
NSEpiClassifArray::NSEpiClassifArray()
                  :NSEpiClassifInfoVector()
{
  _sFileName.clear();
}

NSEpiClassifArray::NSEpiClassifArray(string sFile)
  : NSEpiClassifInfoVector()
{
  _sFileName = sFile;
}

NSEpiClassifArray::~NSEpiClassifArray()
{
  vider();
}

bool NSEpiClassifArray::fillArray() {
  ifstream  inFile;
  string    line;

  inFile.open(_sFileName.c_str());
  if (!inFile)
    return false;

  while (!inFile.eof())
  {
    getline(inFile, line);
    if (line != "")
    {
      // on ajoute dans l'EpiClassifArray ce qu'on vient de lire dans le fichier
      string  sClassif;
      string  sCode;
      string  sLibelle;
      string  sChapitre;

      // recherche de la classif
      // le premier paramètre est l'ID dans la table, c'est pour cela qu'on
      // commence à partir de la première tabulation
      size_t  posBeg = line.find("\t");
      size_t  posEnd = line.find("\t", posBeg + 1);
      if ((posEnd != string::npos) && (posBeg != string::npos) && (posBeg < posEnd))
        sClassif = string(line, posBeg + 1, posEnd - (posBeg + 1));
      else
        return false;

      // recherche du code
      posBeg  = posEnd;
      posEnd  = line.find("\t", posBeg + 1);
      if ((posEnd != string::npos) && (posBeg != string::npos) && (posBeg < posEnd))
        sCode = string(line, posBeg + 1, posEnd - (posBeg + 1));
      else
        return false;

      // recherche du libelle
      posBeg  = posEnd;
      posEnd  = line.find("\t", posBeg + 1);
      if ((posEnd != string::npos) && (posBeg != string::npos) && (posBeg < posEnd))
        sLibelle = string(line, posBeg + 1, posEnd - (posBeg + 1));
      else
        return false;

      // recherche du chapitre
      posBeg  = posEnd;
      posEnd  = strlen(line.c_str());
      if ((posEnd != string::npos) && (posBeg != string::npos) && (posBeg < posEnd))
        sChapitre = string(line, posBeg + 1, posEnd - (posBeg + 1));
      else
        return false;

      if ((sClassif != "") && (sCode != "") && (sLibelle != ""))
      {
        NSEpiClassifInfo *pEpiClassifInfoTemp = new NSEpiClassifInfo();

        pEpiClassifInfoTemp->setClassification(sClassif);
        pEpiClassifInfoTemp->setCode(sCode);
        pEpiClassifInfoTemp->setLabel(sLibelle);
        pEpiClassifInfoTemp->setChapter(sChapitre);

        push_back(pEpiClassifInfoTemp);
      }
      else
        return false;
    }
  }
  inFile.close();
  return true;
}

bool NSEpiClassifArray::searchKey(string sClassif, string sCode) {
  for (NSEpiClassifInfoIter iter = begin(); end() != iter; iter++)
  {
    if (((*iter)->getClassification() == sClassif)  &&
        ((*iter)->getCode()           == sCode))
    {
      _Donnees = (*iter)->_Donnees;
      return true;
    }
  }
  return false;
}

bool NSEpiClassifArray::vider() {
  if (empty())
    return true;

  for (NSEpiClassifInfoIter iter = begin(); end() != iter; )
  {
    delete (*iter);
    erase(iter);
  }

  return true;
}


// -----------------------------------------------------------------------------
//
// Implement NSCodPrestArray method
//
// -----------------------------------------------------------------------------


/*
// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
NSCodPrestArray::NSCodPrestArray(NSCodPrestArray& rv)
  : NSFicheCodPrestArray()
{
  for (NSCodPrestIter i = rv.begin(); i != rv.end(); i++)
    push_back(new NSCodPrestInfo(*(*i)));
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
void
NSCodPrestArray::vider()
{
  for (NSCodPrestIter i = begin(); i != end(); )
  {
    delete (*i);
    erase(i);
  }
}


NSCodPrestArray::~NSCodPrestArray()
{
	vider();
}
*/
