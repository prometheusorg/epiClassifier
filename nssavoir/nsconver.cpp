//    NSCV.CPP
//    PA aout 97
//    Conversion d'une unite dans une autre et calcul de formules
//---------------------------------------------------------------------------

#ifndef __linux__
# include <windows.h>
# include <mem.h>
# include <io.h>
#else
# include <math.h>
#endif

#include <stdio.h>
#include <fcntl.h>

#include <cstring>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include <bde.hpp>
  #include "nautilus\nssuper.h"
  #include "partage\nsdivfct.h"
#endif

#include "nssavoir/nsconver.h"
#include "nssavoir/nsfilgd.h"
#include "nsbb/nspatpat.h"
#ifndef __linux__
#include "nautilus/nshistdo.h"
#endif

//************************************************************************
// classe CodeStructure
//************************************************************************
CodeStructure::CodeStructure(const CodeStructure& src)
{
	dValeur = src.dValeur;
	sCode 	= src.sCode;
	sUnite 	= src.sUnite;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
CodeStructure& CodeStructure::operator= (CodeStructure src)
{
  if (this == &src)
    return *this;

	dValeur = src.dValeur;
	sCode 	= src.sCode;
	sUnite 	= src.sUnite;

	return *this;
}

CodeStructure::CodeStructure(string sChaine , double dVal, string sUnit)
{
	dValeur = dVal;
	sCode  	= sChaine;
	sUnite 	= sUnit;
}

//**************************************************************************
//								structure contenant un code lexique et une valeur
//												chiffrée
//									  Exemple (VPOIDS, 70)
//**************************************************************************

VectorCodeStructure::~VectorCodeStructure()
{
	vider();
}

VectorCodeStructure::VectorCodeStructure()
                   :vectClass()
{}

void
VectorCodeStructure::vider()
{
	if (empty())
		return;

	iterCode i = begin();
	while (end() != i)
	{
		delete *i;
		erase(i);
	}
}

VectorCodeStructure&
VectorCodeStructure::operator=(VectorCodeStructure src)
{
  if (this == &src)
    return *this;

	vider();
	if (false == src.empty())
		for (iterCode i = src.begin(); src.end() != i; i++)
			push_back(new CodeStructure(*(*i)));

	return *this;
}

VectorCodeStructure::VectorCodeStructure(VectorCodeStructure& src)
{
	if (false == src.empty())
		for (iterCode i = src.begin(); src.end() != i; i++)
			push_back(new CodeStructure(*(*i)));
}

//-----------------------------------------------------------------
//savoir s'il existe une structure dont le code est sItem, retourner
//dValeur sinon -11111111
//-----------------------------------------------------------------
double
VectorCodeStructure::donneValeur(string sItem)
{
	if (false == empty())
		for (iterCode i = begin(); end() != i; i++)
			if (sItem == (*i)->sCode)
				return (*i)->dValeur;

	return -11111111;
}

//-----------------------------------------------------------------
//savoir s'il existe une structure dont le code est sItem
//-----------------------------------------------------------------
bool
VectorCodeStructure::Appartient(string sItem, CodeStructure* pResultat)
{
	if (empty())
    return false;

  for (iterCode i = begin(); end() != i; i++)
  {
    if (sItem == (*i)->sCode)
    {
      *pResultat = *(*i);
      return true;
    }
  }

	return false;
}

//***************************************************************************
// 				  Implémentation des mèthodes NSCVData
//***************************************************************************

//---------------------------------------------------------------------------
//  Fonction:		NSPatientData::NSPatientData(NSPatientData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCVData::NSCVData(NSCVData& rv)
{
#ifndef _ENTERPRISE_DLL

	strcpy(resultat,  rv.resultat);
	strcpy(operation, rv.operation);
	strcpy(apartir,   rv.apartir);
	strcpy(quantite,  rv.quantite);

	strcpy(formule,   rv.formule);
	strcpy(methode,   rv.methode);
	strcpy(unite,	 		rv.unite);

#else

  _sResult     = rv._sResult;
  _sOperator   = rv._sOperator;
  _sFrom       = rv._sFrom;
  _sForConcept = rv._sForConcept;
  _sFormula    = rv._sFormula;
  _sMethod     = rv._sMethod;
  _sUnit       = rv._sUnit;

#endif
}

NSCVData::~NSCVData()
{}

//---------------------------------------------------------------------------
//  Fonction :		NSCVData::operator=(NSCVData src)
//  Description :	Opérateur =
//  Retour :		Référence sur l'objet cible
//---------------------------------------------------------------------------
NSCVData&
NSCVData::operator=(NSCVData rv)
{
	if (this == &rv)
		return *this;

#ifndef _ENTERPRISE_DLL

	strcpy(resultat,  rv.resultat);
	strcpy(operation, rv.operation);
	strcpy(apartir,   rv.apartir);
	strcpy(quantite,  rv.quantite);

	strcpy(formule,   rv.formule);
	strcpy(methode,   rv.methode);
	strcpy(unite,	    rv.unite);

#else

  _sResult     = rv._sResult;
  _sOperator   = rv._sOperator;
  _sFrom       = rv._sFrom;
  _sForConcept = rv._sForConcept;
  _sFormula    = rv._sFormula;
  _sMethod     = rv._sMethod;
  _sUnit       = rv._sUnit;

#endif

	return *this;
}

//---------------------------------------------------------------------------
//  Fonction :		NSCVData::operator==(NSCVData& o)
//  Description :	Opérateur de comparaison
//  Retour :		0 ou 1
//---------------------------------------------------------------------------
int
NSCVData::operator == ( const NSCVData& rv )
{
#ifndef _ENTERPRISE_DLL
	if((strcmp(resultat,  rv.resultat)  == 0) &&
     (strcmp(operation, rv.operation) == 0) &&
     (strcmp(apartir,   rv.apartir)   == 0) &&
     (strcmp(quantite,  rv.quantite)  == 0) &&
     (strcmp(formule,   rv.formule)   == 0) &&
     (strcmp(unite, 	  rv.unite)     == 0)
    )
#else
  if ((_sResult     == rv._sResult)     &&
      (_sOperator   == rv._sOperator)   &&
      (_sFrom       == rv._sFrom)       &&
      (_sForConcept == rv._sForConcept) &&
      (_sFormula    == rv._sFormula)    &&
      (_sMethod     == rv._sMethod)     &&
      (_sUnit       == rv._sUnit))
#endif

		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Function:    NSCVData::metAZero()
//
//  Description: Met à 0 les variables.
//---------------------------------------------------------------------------
void
NSCVData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
#ifndef _ENTERPRISE_DLL

	memset( resultat,   0, CV_RESULTAT_LEN );
	memset( operation,  0, CV_OPERATION_LEN );
	memset( apartir,    0, CV_APARTIRDE_LEN );
	memset( quantite,   0, CV_LIEALAQUANTITE_LEN );
	memset( formule,    0, CV_FORMULE_LEN );
	memset( methode,    0, CV_METHODE_LEN  );
	memset( unite,      0, CV_UNITE_LEN  );

#else

  _sResult     = string("");
  _sOperator   = string("");
  _sFrom       = string("");
  _sForConcept = string("");
  _sFormula    = string("");
  _sMethod     = string("");
  _sUnit       = string("");

#endif
}

//***************************************************************************
// 				Implémentation des méthodes NSCVInfo
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSCVInfo::NSCVInfo()
{
	// Crée l'objet de données
	pDonnees = new NSCVData();
}

//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSPatPatho
//---------------------------------------------------------------------------
NSCVInfo::NSCVInfo(NSCV* pCVcursor)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSCVData(*(pCVcursor->pDonnees));
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCVInfo::NSCVInfo(NSCVInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSCVData();
	//
	// Copie les valeurs du NSCVInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//destructeur
NSCVInfo::~NSCVInfo()
{
	if (pDonnees)
		delete pDonnees;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSCVInfo&
NSCVInfo::operator=(NSCVInfo src)
{
	if (this == &src)
		return *this;

	*pDonnees = *(src.pDonnees);

	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int NSCVInfo::operator == ( const NSCVInfo& o )
{
	return (*pDonnees == *(o.pDonnees));
}

//***************************************************************************
//							 Implémentation des mèthodes NSCV
//***************************************************************************
//---------------------------------------------------------------------------
//  Function:    NSCV::NSCV(NSSuper* pSuper)
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
NSCV::NSCV(NSSuper* pSuper) : NSFiche(pSuper)
#else
NSCV::NSCV(NSSuper* pSuper) : NSSuperRoot(pSuper)
#endif
{
	pDonnees = new NSCVData();
}

//---------------------------------------------------------------------------
//  Function:    NSCV::~NSCV()
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
NSCV::~NSCV()
{
	if (pDonnees)
		delete pDonnees;
}

#ifndef _ENTERPRISE_DLL
//---------------------------------------------------------------------------
//  Function :  	NSCV::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSCV::alimenteFiche()
{
	alimenteChamp(pDonnees->resultat,   CV_RESULTAT_FIELD,CV_RESULTAT_LEN );
	alimenteChamp(pDonnees->operation,  CV_OPERATION_FIELD,CV_OPERATION_LEN );
	alimenteChamp(pDonnees->apartir,    CV_APARTIRDE_FIELD,CV_APARTIRDE_LEN );
	alimenteChamp(pDonnees->quantite,   CV_LIEALAQUANTITE_FIELD,CV_LIEALAQUANTITE_LEN );
	alimenteChamp(pDonnees->formule,    CV_FORMULE_FIELD,CV_FORMULE_LEN);
	alimenteChamp(pDonnees->methode,    CV_METHODE_FIELD,CV_METHODE_LEN );
	alimenteChamp(pDonnees->unite,      CV_UNITE_FIELD, CV_UNITE_LEN );
}

//---------------------------------------------------------------------------
//  Fonction :  	NSCV::videFiche()
//
//  Description :	Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSCV::videFiche()
{
	videChamp(pDonnees->resultat,   CV_RESULTAT_FIELD,CV_RESULTAT_LEN );
	videChamp(pDonnees->operation,  CV_OPERATION_FIELD,CV_OPERATION_LEN );
	videChamp(pDonnees->apartir,    CV_APARTIRDE_FIELD,CV_APARTIRDE_LEN );
	videChamp(pDonnees->quantite,   CV_LIEALAQUANTITE_FIELD,CV_LIEALAQUANTITE_LEN );
	videChamp(pDonnees->formule,    CV_FORMULE_FIELD,CV_FORMULE_LEN);
	videChamp(pDonnees->methode,    CV_METHODE_FIELD,CV_METHODE_LEN );
	videChamp(pDonnees->unite,      CV_UNITE_FIELD, CV_UNITE_LEN );
}

//---------------------------------------------------------------------------
//  Function :  	NSCV::open()
//
//  Description : Ouvre la table primaire
//
//  Retour :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSCV::open()
{
	char tableName[] = "CONVERT.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_GUIDES);
	return(lastError);
}
#endif

//-------------------------------------------------------------------------
//analyser le formule
//-------------------------------------------------------------------------
void
NSCV::AnalyseurSyntaxique(string sFormule, double* pValeur)
{
}

//-------------------------------------------------------------------------
//convertir sResultat (à partir de son unite sUnite) mettre à jour pValeur
//exemple  pValeur = 25 cm -> pValeur = 0.25 m
//
// Peut se faire spécifiquement en fonction d'une donnée (par exemple
// la covertion de mmol/l en mg/l dépend de la molécule concernée
//-------------------------------------------------------------------------
bool
NSCV::ConvertirUnite(double* pValeur, string sResultat, string sUnite, string sLieA)
{
	if (NULL == pValeur)
		return false;

	if (sResultat == sUnite)
		return true;

	string sCode;

	string sCodeSens;
	NSDico::donneCodeSens(&sResultat, &sCodeSens);
	string sUnitSens;
	NSDico::donneCodeSens(&sUnite, &sUnitSens);

	if (sCodeSens == sUnitSens)
		return true;

	string sLieASens;
	NSDico::donneCodeSens(&sLieA, &sLieASens);

	bool bTrouve = false;

#ifndef _ENTERPRISE_DLL
	//
	// On regarde si c'est lié à une donnée particulière
	//
	if (sLieA != "")
	{
		sCode = sCodeSens + "CV" + sUnitSens + sLieASens;
		lastError = chercheClef(&sCode,
                            "",
                            0,
                            keySEARCHEQ,
                            dbiWRITELOCK
                           		);
		if (lastError == DBIERR_NONE)
    	bTrouve = true;
	}
	if (!bTrouve)
	{
		sCode = sCodeSens + "CV" + sUnitSens;
		lastError = chercheClef(&sCode,
                            "",
                            0,
                            keySEARCHGEQ,
                            dbiWRITELOCK
                            	);
    if (lastError == DBIERR_NONE)
    	bTrouve = true;
	}
#endif

	if (!bTrouve)
		return false;

#ifndef _ENTERPRISE_DLL
	lastError = getRecord();
	if (lastError != DBIERR_NONE)
	{
		erreur("Le fichier de conversion semble endommagé.", standardError, lastError);
    return false;
	}
#endif

	// Good record ?
  if ((pDonnees->getResult()   != sCodeSens) ||
  		(pDonnees->getOperator() != "CV") ||
      (pDonnees->getFrom()     != sUnitSens) ||
			((pDonnees->getConcept() != sLieASens) && (pDonnees->getConcept() != "")))
		return false;


	// 100 niveaux de piles
	double val[100];
	string ope[100];
	int	   nbNiv = 100;
	for (int j = 0; j < nbNiv; j++)
	{
  	val[j] = 0;
    ope[j] = "";
	}

	string sFormule = pDonnees->getFormula();
	size_t  i = 0;
	int     pileur = 0;
	bool tourner = true;
	while (tourner)
	{
		// Parenthèse ouvrante : on empile
    //
    if (sFormule[i] == '(')
    {
    	for (int j = nbNiv - 1; j > pileur; j++)
      {
      	val[j] = val[j-1];
				ope[j] = ope[j-1];
      }
      val[pileur] = 0;
      ope[pileur] = "";
      i++;
    }
    //
    // Opérateurs + - * / ^x, etc
    //
    else if (isOperator(sFormule[i]))
    {
    	ope[pileur] = string(1, sFormule[i]);
      i++;
    }
    //
    // Parenthèse fermante : on dépile
    //
    else if (sFormule[i] == ')')
    {
    	while (ope[pileur+1] != "")
      {
      	val[pileur] = Operation(val[pileur+1], val[pileur], ope[pileur+1]);
        for (int j = pileur+1; j < nbNiv-1; j++)
        {
        	val[j] = val[j+1];
          ope[j] = ope[j+1];
        }
        val[nbNiv-1] = 0;
        ope[nbNiv-1] = "";
      }
      i++;
    }
    //
    // Crochet : valeur à convertir
    //
    else if (sFormule[i] == '[')
    {
    	i++;
      string sCode = "";
      while ((i < strlen(sFormule.c_str())) && (sFormule[i] != ']'))
      {
      	sCode += string(1, sFormule[i]);
        i++;
      }
      if (sFormule[i] == ']')
      	i++;
      else
      	return false;

      if (sCode != sUnitSens)
      	return false;

      if (ope[pileur] != "")
      	val[pileur] = Operation(val[pileur], *pValeur, ope[pileur]);
      else
      	val[pileur] = *pValeur;
    }
    else if ((sFormule[i] >= '0') && (sFormule[i] <= '9'))
    {
    	string sNbre = string(1, sFormule[i]);
      i++;
      while ((i < strlen(sFormule.c_str())) &&
            	   (((sFormule[i] >= '0') && (sFormule[i] <= '9')) ||
                   	(sFormule[i] == '.')))
      {
      	sNbre += string(1, sFormule[i]);
        i++;
      }
      double dNbre = StringToDouble(sNbre);
      if (ope[pileur] != "")
      	val[pileur] = Operation(val[pileur], dNbre, ope[pileur]);
      else
      	val[pileur] = dNbre;
    }
    else
    	i++;

    if (i >= strlen(sFormule.c_str()))
    	tourner = false;
  }
  
  *pValeur = val[pileur];
	return true;
}

double
NSCV::Operation(double dOperande, double dOperateur, string sOperation)
{
	if 		(sOperation == string("+"))
		return dOperande + dOperateur;
	if (sOperation == string("-"))
		return dOperande - dOperateur;
	if (sOperation == string("*"))
		return dOperande * dOperateur;
	if (sOperation == string("/"))
		return dOperande / dOperateur;
	if (sOperation == string("^"))
		return pow(dOperande, dOperateur);
  if (sOperation == string("£"))
  {
    if (double(10) == dOperande)
      return log10(dOperateur);
  }
	return 0;
}

bool
NSCV::isOperator(char cChar)
{
  switch (cChar)
  {
    case '+' :
    case '-' :
    case '*' :
    case '/' :
    case '^' :
    case '£' :
      return true;
  }
  return false;
}
