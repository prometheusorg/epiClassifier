#if !defined(NSCONVER_H)
#define NSCONVER_H

#include <vector>
#include "partage/nsvector.h"

//
// Taille des champs du fichier CONVERT
//
#define CV_RESULTAT_LEN         5
#define CV_OPERATION_LEN        2
#define CV_APARTIRDE_LEN        5
#define CV_LIEALAQUANTITE_LEN   5
#define CV_FORMULE_LEN        100
#define CV_METHODE_LEN          5
#define CV_UNITE_LEN            5

//
// Ordre des champs du fichier
//
#define CV_RESULTAT_FIELD        1
#define CV_OPERATION_FIELD       2
#define CV_APARTIRDE_FIELD       3
#define CV_LIEALAQUANTITE_FIELD  4
#define CV_FORMULE_FIELD         5
#define CV_METHODE_FIELD         6
#define CV_UNITE_FIELD           7

class NSPatPathoArray;

//---------------------------------------------------------------------------
// Classe NSCVData
//---------------------------------------------------------------------------

class NSCVData
{
	public :

    //
    // Variables de stockage des valeurs
    //
#ifndef _ENTERPRISE_DLL

    char resultat[CV_RESULTAT_LEN + 1];
    char operation[CV_OPERATION_LEN + 1];
    char apartir[CV_APARTIRDE_LEN + 1];
    char quantite[CV_LIEALAQUANTITE_LEN + 1];
    char formule[CV_FORMULE_LEN + 1];
    char methode[CV_METHODE_LEN + 1];
    char unite[CV_UNITE_LEN + 1];

    string getResult()   { return string(resultat); }
    string getOperator() { return string(operation); }
    string getFrom()     { return string(apartir); }
    string getConcept()  { return string(quantite); }
    string getFormula()  { return string(formule); }
    string getMethod()   { return string(methode); }
    string getUnit()     { return string(unite); }

#else

    string _sResult;
    string _sOperator;
    string _sFrom;
    string _sForConcept;
    string _sFormula;
    string _sMethod;
    string _sUnit;

    string getResult()   { return _sResult; }
    string getOperator() { return _sOperator; }
    string getFrom()     { return _sFrom; }
    string getConcept()  { return _sForConcept; }
    string getFormula()  { return _sFormula; }
    string getMethod()   { return _sMethod; }
    string getUnit()     { return _sUnit; }

    void setResult(string sV)   { _sResult = sV; }
    void setOperator(string sV) { _sOperator = sV; }
    void setFrom(string sV)     { _sFrom = sV; }
    void setConcept(string sV)  { _sForConcept = sV; }
    void setFormula(string sV)  { _sFormula = sV; }
    void setMethod(string sV)   { _sMethod = sV; }
    void setUnit(string sV)     { _sUnit = sV; }

#endif

    NSCVData(NSCVData& rv);
    NSCVData() { metAZero(); }
    ~NSCVData();

    NSCVData& operator=(NSCVData src);
    int       operator==(const NSCVData& o);

    //methodes
    void metAZero();
};

//---------------------------------------------------------------------------
// Classe NSCV
//---------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
class _CLASSELEXI NSCV : public NSFiche
#else
#ifndef __linux__
class _CLASSELEXI NSCV : public NSSuperRoot
#else
class NSCV : public NSSuperRoot
#endif
#endif
{
  public :

    NSCVData* pDonnees;   // Objet qui contient les donnees

    NSCV(NSSuper* pSuper);
    ~NSCV();

#ifndef _ENTERPRISE_DLL
    DBIResult open();
    void   alimenteFiche();
    void   videFiche();
#endif

    void   AnalyseurSyntaxique(string sFormule, double* pValeur);
    bool   ConvertirUnite(double* pValeur, string sResultat, string sUnite, string sLieA = "");
    double Operation(double dOperande, double dOperateur, string sOperation);
    bool   isOperator(char cChar);
};

//---------------------------------------------------------------------------
// Classe NSCVInfo
//---------------------------------------------------------------------------
class NSCVInfo
{
	public :
		//
    // Objet qui contient les donnees
		//
		NSCVData* pDonnees;

		NSCVInfo();
		NSCVInfo(NSCV* pNSCV);
		NSCVInfo(NSCVInfo& rv);
		~NSCVInfo();
		NSCVInfo& operator=(NSCVInfo src);
		int operator == ( const NSCVInfo& o );
};

//********************************************************************
// code lexique et valeur chiffree : Exemple (VPOIDS, 70)
//********************************************************************
class CodeStructure
{
  public:

    double dValeur;
    string sCode;
    string sUnite;

    CodeStructure(string sChaine, double dVal = -11111111, string sUnite = "");
    CodeStructure(const CodeStructure& src);
    CodeStructure& operator=(CodeStructure src);
};

typedef vector<CodeStructure*> vectClass;
typedef vectClass::iterator iterCode;

class VectorCodeStructure : public vectClass
{
  public:

    VectorCodeStructure();
    ~VectorCodeStructure();
    VectorCodeStructure(VectorCodeStructure& src);
    // Surcharge de l'operateur d'affectation
    VectorCodeStructure& operator=(VectorCodeStructure src);
    void vider();
    bool Appartient(string sItem, CodeStructure* pResultat);
    double donneValeur(string sItem);
};

#endif

