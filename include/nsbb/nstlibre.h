// traitement des textes libres dans les treeview
#ifndef __NSTLIBRE_H
#define __NSTLIBRE_H

#include <vector>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "partage/nsvector.h"

#ifndef __linux__
#include "nsbb/nsexport.h"
#endif

//tailles des champs pour le fichier Tlibre.db
#define TLIBRE_PAT_LEN     PIDS_NSS_LEN
#define TLIBRE_CODE_LEN    5
#define TLIBRE_TEXTE_LEN   40
#define TLIBRE_SUIVANT_LEN 5

//indices des champs pour le fichier Tlibre.db

#define TLIBRE_PAT_FIELD     1
#define TLIBRE_CODE_FIELD    2
#define TLIBRE_TEXTE_FIELD	 3
#define TLIBRE_SUIVANT_FIELD 4

// **************************************************************************
//
//                            Classes utilitaires
//
// **************************************************************************

//---------------------------------------------------------------------------
// Classe NSTexteLibreData
//---------------------------------------------------------------------------
class NSTexteLibreData
{
  public :

    //
    // Variables de stockage des valeurs
    //
    char patient[TLIBRE_PAT_LEN + 1] ;
    char code[TLIBRE_CODE_LEN + 1] ;
    char texte[TLIBRE_TEXTE_LEN + 1] ;
    char suivant[TLIBRE_SUIVANT_LEN + 1] ;

    NSTexteLibreData(NSTexteLibreData& rv) ;
    NSTexteLibreData() { metAZero() ; }
    ~NSTexteLibreData() ;

    NSTexteLibreData& operator=(NSTexteLibreData src) ;
    int               operator==(const NSTexteLibreData& o) ;

    //methodes
    void metAZero() ;
};

//---------------------------------------------------------------------------
// Classe NSTextelibreBase
//---------------------------------------------------------------------------
/*
class  NSTextelibreBase : public NSFiche
{
  public :

    NSTexteLibreData* pDonnees ;   // Objet qui contient les donnees

    NSTextelibreBase(NSContexte* pCtx);
    ~NSTextelibreBase();

    virtual DBIResult open() = 0;
    void metABlanc() { pDonnees->metABlanc(); }
    void alimenteFiche() ;
    void videFiche() ;
};
*/

//---------------------------------------------------------------------------
// Classe NSTlibreInfo
//---------------------------------------------------------------------------
class NSTlibreInfo
{
	public :
		//
    // Objet qui contient les donnees
		//
		NSTexteLibreData* pDonnees ;

		NSTlibreInfo() ;
		// NSTlibreInfo(NSTextelibreBase* pNSTextelibre) ;
		NSTlibreInfo(NSTlibreInfo& rv) ;
    ~NSTlibreInfo() ;

		NSTlibreInfo& operator=(NSTlibreInfo src) ;
		int operator == ( const NSTlibreInfo& o ) ;

    string getPatient() { return string(pDonnees->patient) ; }
    string getCode()    { return string(pDonnees->code) ; }
    string getTexte()   { return string(pDonnees->texte) ; }
    string getSuivant() { return string(pDonnees->suivant) ; }
};

// **************************************************************************
//
//                            Classes de base
//
// **************************************************************************

/*
//---------------------------------------------------------------------------
// Classe NSTlibreBase
//---------------------------------------------------------------------------
class _NSBBCLASSE NSTlibreBase : public NSRoot
{
    public :
        //
	    // Variables de stockage des valeurs
	    //
        NSTextelibreBase* pNSTexteLibre;

        NSTlibreBase(NSContexte* pCtx);
        NSTlibreBase(NSTlibreBase& rv);
        ~NSTlibreBase();

        NSTlibreBase& operator=(NSTlibreBase src);
        int 	      operator==(const NSTlibreBase& o);

        //méthodes
        virtual bool creerCurseur();

        bool IncrementeCode(string* pTexte);
	    bool InsereTexte(string sCodePat, string *pCode, string Stexte);  //	retourner le code de la première fiche
                                                         //	qui va contenir contenant le texte sTexte
	    bool RecupereTexte(string sCodePat, string sCode, string* pTexte); 	//	mettre le contenu de la fiche de code sCode
     																	 	// dans pTexte
        bool MettreFicheAJour(string sCode, string sNouveauTexte);
     																		// mettre à jour le texte de la fiche (sCode)
     																		// avec le texte sTexte

        bool DetruireFiche(NSTextelibreBase* pNSTexteLibre, string sCode);
        bool CreerNouvelleFiche(NSTextelibreBase*  pNSTexteLibre);
        bool ChangerFiche(NSTextelibreBase* pNSTexteLibre, string sCode, string* pNewTexte);
};

// **************************************************************************
//
//                 Classes des textes libres dans les documents
//
// **************************************************************************

//---------------------------------------------------------------------------
// Classe NSTextelibre
//---------------------------------------------------------------------------
class  NSTextelibre : public NSTextelibreBase
{
    public :

	    NSTextelibre(NSContexte* pCtx);
	    ~NSTextelibre();

        DBIResult open();
};

//---------------------------------------------------------------------------
// Classe NSTlibre
//---------------------------------------------------------------------------
class _NSBBCLASSE NSTlibre : public NSTlibreBase
{
    public :

        NSTlibre(NSContexte* pCtx);
        NSTlibre(NSTlibre& rv);
        ~NSTlibre();

        bool creerCurseur();

        NSTlibre& operator=(NSTlibre src);
        int 	  operator==(const NSTlibre& o);
};

// **************************************************************************
//
//                 Classes des textes libres de noms de fichiers
//
// **************************************************************************

//---------------------------------------------------------------------------
// Classe NSTextelibreFileName
//---------------------------------------------------------------------------
class  NSTextelibreFileName : public NSTextelibreBase
{
    public :

	    NSTextelibreFileName(NSContexte* pCtx);
	    ~NSTextelibreFileName();

        DBIResult open();
};

//---------------------------------------------------------------------------
// Classe NSTlibreFileName
//---------------------------------------------------------------------------
class _NSBBCLASSE NSTlibreFileName : public NSTlibreBase
{
    public :

        NSTlibreFileName(NSContexte* pCtx);
        NSTlibreFileName(NSTlibreFileName& rv);
        ~NSTlibreFileName();

        bool creerCurseur();

        NSTlibreFileName& operator=(NSTlibreFileName src);
        int 	  operator==(const NSTlibreFileName& o);
};
*/

#endif

