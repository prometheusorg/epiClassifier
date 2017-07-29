//---------------------------------------------------------------------------
//     NSPERSON.H
//
//  Contient les definitions des objets PATIENT et UTILISATEUR de NAUTILUS
//
//  1ere version : KRZISCH Ph. janvier 92   Derniere modif : 12/08/93
//---------------------------------------------------------------------------
#ifndef __NS_FIC_H
#define __NS_FIC_H

#include <cstring>
#include "nssavoir/nspatbas.h"
#ifndef __linux__
#include "ns_sgbd/nsfiche.h"
#endif

#ifndef __linux__
#if defined(_DANSLEXIDLL)
	#define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif
#endif

//
// Taille des champs du fichier FICHES
// les autres champs sont dans nspatbas.h
//
#define FICHE_SENS_LEN   		    5

//
// Indice des champs du fichier FICHES
//
#define FICHE_SENS_FIELD   		    1
#define FICHE_LOCALISATION_FIELD  2
#define FICHE_TYPE_FIELD  		 	  3
#define FICHE_LEXIQUE_FIELD       4
#define FICHE_COMPLEMENT_FIELD    5
#define FICHE_UNIT_FIELD          6
#define FICHE_CERTITUDE_FIELD     7
#define FICHE_INTERET_FIELD  	 	  8
#define FICHE_PLURIEL_FIELD       9

//---------------------------------------------------------------------------
//  Classe NSSavFiche
//---------------------------------------------------------------------------

//
// Objet contenant les donnees
//
class NSSavFicheData : public NSPathoBaseData
{
  protected:

    //
    // Variables de stockage des valeurs
    //
    // char sens[FICHE_SENS_LEN + 1] ;

    string _sSens ;

	public :

    NSSavFicheData() ;
    NSSavFicheData(NSSavFicheData& rv) ;
    ~NSSavFicheData() ;

    void metAZero() ;

    string getSens()            { return _sSens ; }
    void   setSens(string sVal) { setFixedSizeValue(&_sSens, FICHE_SENS_LEN, sVal) ; }

    NSSavFicheData& operator=(NSSavFicheData src) ;
    int             operator==(const NSSavFicheData& o) ;
};

//
// Objet derive de NSFiche servant aux acces de base de donnees
//
#ifndef __linux__
class _CLASSELEXI NSSavFiche : public NSFiche
{
	public :

		//
    // Objet qui contient les donnees
    //
    NSSavFicheData* pDonnees ;

    NSSavFiche(NSSuper* pSuper) ;
    NSSavFiche(NSSavFiche& rv) ;
    ~NSSavFiche() ;

    void alimenteFiche() ;
    void videFiche() ;
    DBIResult open() ;

    DBIResult getPatRecord() ;

    string getSens()            { return pDonnees->getSens() ; }
    void   setSens(string sVal) { pDonnees->setSens(sVal) ; }

    string getLexique()      { return pDonnees->getLexique() ; }
    string getComplement()   { return pDonnees->getComplement() ; }
    string getUnit()         { return pDonnees->getUnit() ; }
    string getCertitude()    { return pDonnees->getCertitude() ; }
    string getInteret()      { return pDonnees->getInteret() ; }
    string getPluriel()      { return pDonnees->getPluriel() ; }
    string getVisible()      { return pDonnees->getVisible() ; }
    string getType()         { return pDonnees->getTypeString() ; }
    string getLocalisation() { return pDonnees->getLocalisation() ; }

    virtual bool Create() ;
    virtual bool Modify() ;

    NSSavFiche& operator=(NSSavFiche src) ;
    int operator == ( const NSSavFiche& o ) ;
} ;
#endif // #ifndef __linux__

//---------------------------------------------------------------------------
//  Classe NSPatInfo  (destinee a etre stockee dans une Array)
//---------------------------------------------------------------------------
#ifndef __linux__
class _CLASSELEXI NSSavFicheInfo
#else
class NSSavFicheInfo
#endif
{
  protected:

    //
    // Objet qui contient les donnees
		//
		NSSavFicheData* pDonnees ;

	public:

		NSSavFicheInfo() ;
#ifndef __linux__
		NSSavFicheInfo(NSSavFiche*) ;
#endif
		NSSavFicheInfo(NSSavFicheInfo& rv) ;
    ~NSSavFicheInfo() ;

    string getSens()         { return pDonnees->getSens() ; }
    string getLexique()      { return pDonnees->getLexique() ; }
    string getComplement()   { return pDonnees->getComplement() ; }
    string getUnit()         { return pDonnees->getUnit() ; }
    string getCertitude()    { return pDonnees->getCertitude() ; }
    string getInteret()      { return pDonnees->getInteret() ; }
    string getPluriel()      { return pDonnees->getPluriel() ; }
    string getVisible()      { return pDonnees->getVisible() ; }
    string getType()         { return pDonnees->getTypeString() ; }
    string getLocalisation() { return pDonnees->getLocalisation() ; }

    void   setSens(string sSens)           { pDonnees->setSens(sSens) ; }
    void   setLexique(string sLexique)     { pDonnees->setLexique(sLexique) ; }
    void   setComplement(string sCplmnt)   { pDonnees->setComplement(sCplmnt) ; }
    void   setUnit(string sUnit)           { pDonnees->setUnit(sUnit) ; }
    void   setCertitude(string sCertitude) { pDonnees->setCertitude(sCertitude) ; }
    void   setInteret(string sInteret)     { pDonnees->setInteret(sInteret) ; }
    void   setPluriel(string sPluriel)     { pDonnees->setPluriel(sPluriel) ; }
    void   setVisible(string sVisible)     { pDonnees->setVisible(sVisible) ; }
    void   setType(string sType)           { pDonnees->setTypeString(sType) ; }
    void   setLocalisation(string sLocalisation) { pDonnees->setLocalisation(sLocalisation) ; }

		NSSavFicheInfo& operator=(NSSavFicheInfo src) ;
		int operator == ( const NSSavFicheInfo& o ) ;
} ;

class NSPatPathoArray ;

//
// Objet de gestion des fiches
//
#ifndef __linux__
class _CLASSELEXI NSFicheManager : public NSSuperRoot
{
	public :

		NSSavFiche* pCurseur ;

		NSFicheManager(NSSuper* pSuper) ;
		NSFicheManager(NSFicheManager& rv) ;
		~NSFicheManager() ;

		bool getFiche(string sSens, NSPatPathoArray* pPPTarray, bool bVerbose = true) ;
		bool setFiche(string sSens, NSPatPathoArray* pPPTarray) ;
		void updateFiche(NSPatPathoArray* pFiche, string sChapter, string sSubChapter, NSPatPathoArray* pPPTvalues) ;
} ;
#endif // #ifndef __linux__

//---------------------------------------------------------------------------
#endif    // __NS_FIC_H
