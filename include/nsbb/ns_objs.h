//---------------------------------------------------------------------------
//    NS_OBJS.H
//    NAUTILUS Septembre 2003
//
//    Gestion des "choses"
//---------------------------------------------------------------------------
#ifndef __NS_OBJS_H
#define __NS_OBJS_H

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "partage/nsvector.h"
#include "nsbb/nsexport.h"
#include "nssavoir/nspatbas.h"
#include "nssavoir/ns_fic.h"
#include "nssavoir/nstransa.h"
#include "nsdn/nsdocum.h"
#include "nsbb/nstlibre.h"
#include "nsbb/nspatpat.h"

// #define OBJECT_ID_LEN 	  	 13
// #define OBJECT_ID_LEN  PIDS_NSS_LEN + DOC_CODE_DOCUM_LEN

//
// Indice des champs du fichier NSMDATOBJ
//
#define OBD_OBJECT_FIELD  	  1
#define OBD_NOEUD_FIELD       2
#define OBD_TYPE_FIELD   	  3
#define OBD_LEXIQUE_FIELD     4
#define OBD_COMPLEMENT_FIELD  5
#define OBD_CERTITUDE_FIELD   6
#define OBD_PLURIEL_FIELD     7
#define OBD_TRAN_NEW_FIELD    8
#define OBD_TRAN_DEL_FIELD    9
#define OBD_UNIT_FIELD       10

//---------------------------------------------------------------------------
//  Classe NSObjectData
//---------------------------------------------------------------------------
class NSObjectData
{
    public :

        char objectID[OBJECT_ID_LEN + 1];
        char noeud[PPD_NOEUD_LEN + 1];
        char type[BASE_TYPE_LEN + 1];
   	    char lexique[BASE_LEXIQUE_LEN + 1];
     	char complement[BASE_COMPLEMENT_LEN + 1];
	  	char certitude[BASE_CERTITUDE_LEN + 1];
        char pluriel[BASE_PLURIEL_LEN + 1];
        char tran_new[TRAN_CODE_LEN + 1];
        char tran_del[TRAN_CODE_LEN + 1];
        char unit[BASE_UNIT_LEN + 1];

		NSObjectData() { metAZero(); }
		NSObjectData(NSObjectData& rv);

		NSObjectData& operator=(NSObjectData src);
		int 		  operator==(const NSObjectData& o);

		void metAZero();

        int  getType();
        void setType(int newType);

        string getID()   { return string(objectID); }
        string getNode() { return string(objectID) + string(noeud); }
};

/*
class _NSBBCLASSE NSObjectDat : public NSFiche
{
    public :

        NSObjectData* pDonnees;   // Objet qui contient les donnees

	    NSObjectDat(NSContexte* pCtx);
	    ~NSObjectDat();

	    DBIResult open();
	    void alimenteFiche();
	    void videFiche();

	    short prendStatut() { return statut; }
	    void  metStatut(short nouvStatut) { statut = nouvStatut; }

	    virtual bool Create();
	    virtual bool Modify();
};
*/

class NSObjectDatInfo
{
  public :

    //
    // Objet qui contient les donnees
		//
		NSObjectData* pDonnees;

		NSObjectDatInfo();
		// NSObjectDatInfo(NSObjectDat* pObjDat);
		NSObjectDatInfo(NSObjectDatInfo& rv);

    string getID()   { return pDonnees->getID(); }
    string getNode() { return pDonnees->getNode(); }

		NSObjectDatInfo& operator=(NSObjectDatInfo src);
		int operator == (const NSObjectDatInfo& o);
    int operator == (const NSPatPathoInfo& o);
    ~NSObjectDatInfo();
};

typedef vector<NSObjectDatInfo*> NSObjectDatInfoArray;
typedef NSObjectDatInfoArray::iterator ObjectDatIter;

class NSObjectDatArray : public NSObjectDatInfoArray, public NSRoot
{
  public :

    //
    // Constructeurs
    //
    NSObjectDatArray(NSContexte* pCtx = 0);
    NSObjectDatArray(NSObjectDatArray& rv);
    //
    // Destructeur
    //
    ~NSObjectDatArray();
    void vider();
    //
    // Operateurs
    //
    NSObjectDatArray& operator=(NSObjectDatArray src);
    //
    // Fonctions de gestion du vecteur
    //
    bool ajouteElement(NSObjectDatInfo* pObjDat);
};

//
// Indice des champs du fichier NSMLOCOBJ
//
#define OBL_OBJECT_FIELD  1
#define OBL_TRAN_FIELD    2
#define OBL_NOEUD_FIELD   3
#define OBL_LOC_FIELD     4
#define OBL_VISIBLE_FIELD 5
#define OBL_INTERET_FIELD 6

//---------------------------------------------------------------------------
//  Classe NSObjectLocData
//---------------------------------------------------------------------------
class NSObjectLocData
{
    public :

        char objectID[OBJECT_ID_LEN + 1];
        char transaction[TRAN_CODE_LEN + 1];
        char noeud[PPD_NOEUD_LEN + 1];
        char codeLocalisation[BASE_LOCALISATION_LEN + 1];
        char visible[BASE_VISIBLE_LEN + 1];
        char interet[BASE_INTERET_LEN + 1];

		NSObjectLocData() { metAZero(); }
		NSObjectLocData(NSObjectLocData& rv);

		NSObjectLocData&    operator=(NSObjectLocData src);
		int 			    operator==(const NSObjectLocData& o);

        string getID()   { return string(objectID); }

		void metAZero();
};

class NSObjectLocInfo
{
    public :
        //
		// Objet qui contient les données
		//
		NSObjectLocData* pDonnees;

		NSObjectLocInfo();
		NSObjectLocInfo(NSObjectLocInfo& rv);

		NSObjectLocInfo&    operator=(NSObjectLocInfo src);
		int                 operator==(const NSObjectLocInfo& o);
        ~NSObjectLocInfo();

        string getID()   { return pDonnees->getID(); }
};

typedef vector<NSObjectLocInfo*> NSObjectLocInfoArray;
typedef NSObjectLocInfoArray::iterator ObjectLocIter;

class NSObjectLocArray : public NSObjectLocInfoArray, public NSRoot
{
  public :

    //
    // Constructeurs
    //
    NSObjectLocArray(NSContexte* pCtx = 0);
    NSObjectLocArray(NSObjectLocArray& rv);
    //
    // Destructeur
    //
    ~NSObjectLocArray();
    void vider();
    //
    // Operateurs
    //
    NSObjectLocArray& operator=(NSObjectLocArray src);
    //
    // Fonctions de gestion du vecteur
    //
    bool ajouteElement(NSObjectLocInfo* pPatPaLoc);
};

//
// indices des champs pour le fichier NSMTLIBOBJ
//
#define OBJTLIBRE_OBJECT_FIELD      1
#define OBJTLIBRE_CODE_FIELD	    2
#define OBJTLIBRE_TEXTE_FIELD	    3
#define OBJTLIBRE_SUIVANT_FIELD     4

//---------------------------------------------------------------------------
// Classe NSObjectTLibreData
//---------------------------------------------------------------------------

class NSObjectTLibreData
{
  public :

    //
    // Variables de stockage des valeurs
    //

        char object[OBJECT_ID_LEN + 1];
	    char code[TLIBRE_CODE_LEN + 1];
	    char texte[TLIBRE_TEXTE_LEN + 1];
        char suivant[TLIBRE_SUIVANT_LEN + 1];

        NSObjectTLibreData(NSObjectTLibreData& rv);
	    NSObjectTLibreData() { metAZero(); }

        ~NSObjectTLibreData();

        NSObjectTLibreData& operator=(NSObjectTLibreData src);
        int 			    operator==(const NSObjectTLibreData& o);

        //méthodes
	    void metABlanc();
	    void metAZero();
};

//---------------------------------------------------------------------------
// Classe NSTlibreInfo
//---------------------------------------------------------------------------

class NSObjectTLibreInfo
{
	public :

		//
		// Objet qui contient les données
		//

		NSObjectTLibreData* pDonnees;

		NSObjectTLibreInfo();
		NSObjectTLibreInfo(NSObjectTLibreInfo& rv);
        ~NSObjectTLibreInfo();

		NSObjectTLibreInfo& operator=(NSObjectTLibreInfo src);
		int operator == ( const NSObjectTLibreInfo& o );
};

//---------------------------------------------------------------------------
// Classe NSObjectTLibreBase
//---------------------------------------------------------------------------
class NSObjectTLibreBase : public NSRoot
{
  public :

    //
    // Variables de stockage des valeurs
    //

    NSObjectTLibreInfo* pNSTexteLibre;
    NSObjectTLibreBase(NSContexte* pCtx);
    NSObjectTLibreBase(NSObjectTLibreBase& rv);
    ~NSObjectTLibreBase();

    NSObjectTLibreBase& operator=(NSObjectTLibreBase src);
    int 	            operator==(const NSObjectTLibreBase& o);

    //methodes
    bool creerCurseur();
    bool IncrementeCode(string* pTexte);
    bool InsereTexte(string sCodeObj, string *pCode, string Stexte);    //	retourner le code de la premiere fiche
                                                                            //	qui va contenir contenant le texte sTexte
    bool RecupereTexte(string sCodeObj, string sCode, string* pTexte); 	//	mettre le contenu de la fiche de code sCode
     																	 	// dans pTexte
};

//---------------------------------------------------------------------------
#endif    // __NS_OBJS_H
