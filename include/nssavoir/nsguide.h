//---------------------------------------------------------------------------
//     NSBBFICH.H
//     PA   juillet 95
//  Contient les definitions des objets Big Brother
//---------------------------------------------------------------------------
#ifndef __NSGUIDE_H
#define __NSGUIDE_H

#include <cstring>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
  #include "ns_sgbd\nsfiche.h"
#endif

#include "nssavoir/nsvarray.h"

#define	GUIDE	 string("GUIDE")  //GUIDE.DB
#define	DECODE	 string("DECODE") //DECODE.DB

#ifndef __linux__
#if defined(_DANSLEXIDLL)
	#define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif
#endif

//
// Taille des champs communs aux fichiers de Chemins
//
#define CH_CODE_LEN       6
#define CH_AUTEUR_LEN 	  3
#define CH_CHEMIN_LEN 	255
#define CH_SCENARIO_LEN   6
//
// Taille des champs du fichier GUIDES
//
#define BB_COMMENTAIRE_LEN 	     20
#define BB_FICHIER_DIALOGUE_LEN  20
#define BB_NOM_DIALOGUE_LEN      32
#define BB_OUVRE_DIALOGUE_LEN 	  1
#define BB_FICHIER_FONCTION_LEN  20
#define BB_NOM_FONCTION_LEN 	   10
#define BB_DECALAGE_NIVEAU_LEN 	  6
#define BB_FILS_LEN 			      255
#define BB_EXCLUSION_LEN 		    255
#define BB_AUTOMATIQUE_LEN   	    1
#define BB_ACTIF_VIDE_LEN 	   	  1
#define BB_UNICITE_LESION_LEN     1
#define BB_IMPOSE_LEN 	          1
#define BB_TRI_LEN 	   		       20
//
// Ordre des champs du fichier GUIDES
//
#define BB_AUTEUR_FIELD 		       1
#define BB_CODE_FIELD 			       2
#define BB_CHEMIN_FIELD 		       3
#define BB_COMMENTAIRE_FIELD       4
#define BB_FICHIER_DIALOGUE_FIELD  5
#define BB_NOM_DIALOGUE_FIELD 	   6
#define BB_OUVRE_DIALOGUE_FIELD    7
#define BB_FICHIER_FONCTION_FIELD  8
#define BB_NOM_FONCTION_FIELD 	   9
#define BB_DECALAGE_NIVEAU_FIELD  10
#define BB_FILS_FIELD 			      11
#define BB_EXCLUSION_FIELD 		    12
#define BB_AUTOMATIQUE_FIELD 	    13
#define BB_ACTIF_VIDE_FIELD 	    14
#define BB_UNICITE_LESION_FIELD   15
#define BB_IMPOSE_FIELD 	        16
#define BB_TRI_FIELD 	   		      17
#define BB_SCENARIO_FIELD 	   	  18

//
// Taille des champs du fichier DECODE
//
#define DKD_COMMENTAIRE_LEN 	  20
#define DKD_FICHIER_LEN 		    20
#define DKD_NOM_FONCTION_LEN 	  10
//
// Ordre des champs du fichier DECODE
//
#define DKD_AUTEUR_FIELD       1
#define DKD_CODE_FIELD         2
#define DKD_CHEMIN_FIELD       3
#define DKD_COMMENTAIRE_FIELD  4
#define DKD_FICHIER_FIELD      5
#define DKD_NOM_FONCTION_FIELD 6
#define DKD_SCENARIO_FIELD     7

// tailles et indices des champs de GROUPGD.DB
///////////////////////////////////////////////

#define GRPGD_GROUPE_LEN         3
#define GRPGD_LIBELLE_LEN       70
#define GRPGD_DATE_LEN           8
#define GRPGD_UTIL_LEN           3

#define GRPGD_GROUPE_FIELD      1
#define GRPGD_LIBELLE_FIELD     2
#define GRPGD_DATE_FIELD        3
#define GRPGD_UTIL_FIELD        4

//
//
//
#ifndef __linux__
class _CLASSELEXI BBChemData
#else
class BBChemData
#endif
{
	public :

    //
    // Variables de stockage des valeurs
    //
    char code[CH_CODE_LEN + 1] ;
    char auteur[CH_AUTEUR_LEN + 1] ;
    char chemin[CH_CHEMIN_LEN + 1] ;
    char scenario[CH_SCENARIO_LEN + 1] ;

    void metAZero() ;

    BBChemData() { metAZero() ; }
    BBChemData(BBChemData& rv) ;
    ~BBChemData() ;
    
    BBChemData& operator=(BBChemData src) ;
    int         operator==(const BBChemData& o) ;

		string getID()           { return string(code) ;     }
    string getGroupID()      { return string(auteur) ;   }
    string getSemanticPath() { return string(chemin) ;   }
    string getValidityCase() { return string(scenario) ; }

    void setID(string sID)              { strcpy(code,     sID.c_str()) ;    }
    void setGroupID(string sGroup)      { strcpy(auteur,   sGroup.c_str()) ; }
    void setSemanticPath(string sPath)  { strcpy(chemin,   sPath.c_str()) ;  }
    void setValidityCase(string sValid) { strcpy(scenario, sValid.c_str()) ; }
};

#ifndef _ENTERPRISE_DLL
class _CLASSELEXI BBChem : public NSFiche
#else
#ifndef __linux__
class _CLASSELEXI BBChem : public NSSuperRoot
#else
class BBChem : public NSSuperRoot
#endif
#endif
{
	public :

    BBChemData*	pDonnees ;	  // Donnees de la fiche PARADOX

    BBChem(NSSuper* pSuper) ;
    ~BBChem() ;

#ifndef _ENTERPRISE_DLL
    DBIResult open(string tableName) ;
    DBIResult open() ;
#endif
    void alimenteFiche() ;
    void videFiche() ;
};

//---------------------------------------------------------------------------
//  Classe BBItemData
//      ANCETRE : BBChemData
//---------------------------------------------------------------------------
//
// Objet donnees des Objets BBItem et BBFichItem
//
#ifndef __linux__
class _CLASSELEXI BBItemData : public BBChemData
#else
class BBItemData : public BBChemData
#endif
{
	public :

		//
		// Variables de stockage des valeurs
		//
    char commentaire[BB_COMMENTAIRE_LEN + 1] ;
    char fichierDialogue[BB_FICHIER_DIALOGUE_LEN + 1] ;
    char nomDialogue[BB_NOM_DIALOGUE_LEN + 1] ;
    char ouvreDialogue[BB_OUVRE_DIALOGUE_LEN + 1] ;
    char fichierFonction[BB_FICHIER_FONCTION_LEN + 1] ;
    char nomFonction[BB_NOM_FONCTION_LEN + 1] ;
    char decalageNiveau[BB_DECALAGE_NIVEAU_LEN + 1] ;
    char fils[BB_FILS_LEN + 1] ;
    char exclusion[BB_EXCLUSION_LEN + 1] ;
    char automatique[BB_AUTOMATIQUE_LEN + 1] ;
    char actif_vide[BB_ACTIF_VIDE_LEN + 1] ;
    char unicite_lesion[BB_UNICITE_LESION_LEN + 1] ;
    char impose[BB_IMPOSE_LEN + 1] ;
    char tri[BB_TRI_LEN + 1] ;

    bool estAutomatique() { return ((automatique[0] == 'O') || (automatique[0] == 'o')) ; }
    bool actifVide()      { return ((actif_vide[0] != 'N') && (actif_vide[0] != 'n')) ; }
    bool uniciteLesion()  { return ((unicite_lesion[0] != 'N') && (unicite_lesion[0] != 'n')) ; }
    bool ouvreDlg()       { return ((ouvreDialogue[0] == 'O') || (ouvreDialogue[0] == 'o')) ; }
    bool ouvreArchetype() { return ((ouvreDialogue[0] == 'A') || (ouvreDialogue[0] == 'a')) ; }
    bool estImpose()      { return ((impose[0] == 'O') || (impose[0] == 'o')) ; }

    void metAZero() ;

    BBItemData() : BBChemData() { metAZero() ; }
    BBItemData(BBItemData& rv) ;
    ~BBItemData() ;

    BBItemData& operator=(BBItemData src) ;
    int         operator==(const BBItemData& o) ;

		string getLabel()      { return string(commentaire); }
    string getDialogFile() { return string(fichierDialogue) ; }
    string getDialogName() { return string(nomDialogue) ; }
    string getFctFile()    { return string(fichierFonction) ; }
    string getFctName()    { return string(nomFonction) ; }
    string getLevelShift() { return string(decalageNiveau) ; }
    string getSonsList()   { return string(fils) ; }
    string getSonsRules()  { return string(exclusion) ; }
    string getSortRules()  { return string(tri) ; }

    void setLabel(string sLabel)      { strcpy(commentaire,     sLabel.c_str()) ; }
    void setDialogFile(string sDlgF)  { strcpy(fichierDialogue, sDlgF.c_str()) ;  }
    void setDialogName(string sDlgN)  { strcpy(nomDialogue,     sDlgN.c_str()) ;  }
    void setFctFile(string sFctF)     { strcpy(fichierFonction, sFctF.c_str()) ;  }
    void setFctName(string sFctN)     { strcpy(nomFonction,     sFctN.c_str()) ;  }
    void setLevelShift(string sLevel) { strcpy(decalageNiveau,  sLevel.c_str()) ; }
    void setSonsList(string sSons)    { strcpy(fils,            sSons.c_str()) ;  }
    void setSonsRules(string sRules)  { strcpy(exclusion,       sRules.c_str()) ; }
    void setSortRules(string sSort)   { strcpy(tri,             sSort.c_str()) ;  }
    void setAutomatic(bool bAuto)     { (true == bAuto) ? strcpy(automatique, "O")     : strcpy(automatique, "N") ; }
    void setEmptyActivation(bool bEA) { (true == bEA)   ? strcpy(actif_vide, "O")      : strcpy(actif_vide, "N") ; }
    void setUnicity(bool bUniq)       { (true == bUniq) ? strcpy(unicite_lesion, "O")  : strcpy(unicite_lesion, "N") ; }
    void setOpenDialog(bool bOpen)    { (true == bOpen) ? strcpy(ouvreDialogue, "O")   : strcpy(ouvreDialogue, "N") ; }
    void setOpenArchetype(bool bOpen) { (true == bOpen) ? strcpy(ouvreDialogue, "A")   : strcpy(ouvreDialogue, "N") ; }
    void setNeeded(bool bNeed)        { (true == bNeed) ? strcpy(impose, "O")          : strcpy(impose, "N") ; }
};

////////////////////////////////////////////////////////////////////////////////////////
//
// Classe BBFiche
//
#ifndef __linux__
class _CLASSELEXI BBFiche : public BBChem
#else
class BBFiche : public BBChem
#endif
{
	public :

    BBFiche(NSSuper* pSuper) ;
    ~BBFiche() ;

#ifndef _ENTERPRISE_DLL
    DBIResult open(string tableName) ;
    DBIResult open() ;
#endif
    void alimenteFiche() ;
    void videFiche() ;
};

//
// Objet contenant uniquement un pointeur sur les donnees
//
#ifndef __linux__
class _CLASSELEXI BBItemInfo
#else
class BBItemInfo
#endif
{
	public :
  	//
    // Objet qui contient les donnees
    //
    BBItemData* pDonnees ;

    BBItemInfo() ;
		BBItemInfo(BBFiche*) ;
		BBItemInfo(BBItemInfo& rv) ;
    ~BBItemInfo() ;

    BBItemInfo& operator=(BBItemInfo src) ;
		int         operator == (BBItemInfo& o) ;

    // Getters and Setters
    //
    bool estAutomatique() { return pDonnees->estAutomatique() ; }
    bool actifVide()      { return pDonnees->actifVide() ;      }
    bool uniciteLesion()  { return pDonnees->uniciteLesion() ;  }
    bool ouvreDlg()       { return pDonnees->ouvreDlg() ;       }
    bool ouvreArchetype() { return pDonnees->ouvreArchetype() ; }
    bool estImpose()      { return pDonnees->estImpose() ;      }

    void metAZero()       { pDonnees->metAZero() ; }

    string getID()           { return pDonnees->getID() ;           }
    string getGroupID()      { return pDonnees->getGroupID() ;      }
    string getSemanticPath() { return pDonnees->getSemanticPath() ; }
    string getValidityCase() { return pDonnees->getValidityCase() ; }

    void setID(string sID)              { pDonnees->setID(sID) ;              }
    void setGroupID(string sGroup)      { pDonnees->setGroupID(sGroup) ;      }
    void setSemanticPath(string sPath)  { pDonnees->setSemanticPath(sPath) ;  }
    void setValidityCase(string sValid) { pDonnees->setValidityCase(sValid) ; }

		string getLabel()      { return pDonnees->getLabel() ;      }
    string getDialogFile() { return pDonnees->getDialogFile() ; }
    string getDialogName() { return pDonnees->getDialogName() ; }
    string getFctFile()    { return pDonnees->getFctFile() ;    }
    string getFctName()    { return pDonnees->getFctName() ;    }
    string getLevelShift() { return pDonnees->getLevelShift() ; }
    string getSonsList()   { return pDonnees->getSonsList() ;   }
    string getSonsRules()  { return pDonnees->getSonsRules() ;  }
    string getSortRules()  { return pDonnees->getSortRules() ;  }

    void setLabel(string sLabel)      { pDonnees->setLabel(sLabel) ;        }
    void setDialogFile(string sDlgF)  { pDonnees->setDialogFile(sDlgF) ;    }
    void setDialogName(string sDlgN)  { pDonnees->setDialogName(sDlgN) ;    }
    void setFctFile(string sFctF)     { pDonnees->setFctFile(sFctF) ;       }
    void setFctName(string sFctN)     { pDonnees->setFctName(sFctN) ;       }
    void setLevelShift(string sLevel) { pDonnees->setLevelShift(sLevel) ;   }
    void setSonsList(string sSons)    { pDonnees->setSonsList(sSons) ;      }
    void setSonsRules(string sRules)  { pDonnees->setSonsRules(sRules) ;    }
    void setSortRules(string sSort)   { pDonnees->setSortRules(sSort) ;     }
    void setAutomatic(bool bAuto)     { pDonnees->setAutomatic(bAuto) ;     }
    void setEmptyActivation(bool bEA) { pDonnees->setEmptyActivation(bEA) ; }
    void setUnicity(bool bUniq)       { pDonnees->setUnicity(bUniq) ;       }
    void setOpenDialog(bool bOpen)    { pDonnees->setOpenDialog(bOpen) ;    }
    void setNeeded(bool bNeed)        { pDonnees->setNeeded(bNeed) ;        }
};

//
// Definition de BBItemArray (Array de BBItemInfo)
//
typedef vector<BBItemInfo*> BBItemVector ;
typedef BBItemVector::iterator BBItemIter ;

#ifndef __linux__
class _CLASSELEXI BBItemArray : public BBItemVector
#else
class BBItemArray : public BBItemVector
#endif
{
	public :

		// Constructeurs
    BBItemArray() : BBItemVector() {}
    BBItemArray(BBItemArray& rv) ;
    // Destructeur
    virtual ~BBItemArray() ;

    BBItemArray& operator=(BBItemArray src) ;
    void vider() ;
};

//---------------------------------------------------------------------------
//  Classe BBDecode
//      ANCETRE : NSFiche
//---------------------------------------------------------------------------
//
// Objet donnees des Objets BBItem et BBFichItem
//
#ifndef __linux__
class _CLASSELEXI BBDecodeData : public BBChemData
#else
class BBDecodeData : public BBChemData
#endif
{
	public :

    //
    // Variables de stockage des valeurs
    //
    char commentaire[DKD_COMMENTAIRE_LEN + 1] ;
    char fichier[DKD_FICHIER_LEN + 1] ;
    char nomFonction[DKD_NOM_FONCTION_LEN + 1] ;

    void metAZero() ;

    BBDecodeData() : BBChemData() { metAZero() ; }
    BBDecodeData(BBDecodeData& rv) ;
    ~BBDecodeData() ;

    BBDecodeData& operator=(BBDecodeData src) ;
    int           operator==(const BBDecodeData& o) ;

    string getLabel()        { return string(commentaire) ; }
    string getFileName()     { return string(fichier) ;     }
    string getFunctionName() { return string(nomFonction) ; }

    void setLabel(string sLabel)       { strcpy(commentaire, sLabel.c_str()) ; }
    void setFileName(string sDlgF)     { strcpy(fichier,     sDlgF.c_str()) ;  }
    void setFunctionName(string sDlgN) { strcpy(nomFonction, sDlgN.c_str()) ;  }
};

#ifndef __linux__
class _CLASSELEXI BBDecode : public BBChem
#else
class BBDecode : public BBChem
#endif
{
	public :

    BBDecode(NSSuper* pSuper) ;
    ~BBDecode() ;

#ifndef _ENTERPRISE_DLL
    DBIResult open(string tableName) ;
    DBIResult open() ;
#endif
    void alimenteFiche() ;
    void videFiche() ;
};

//////////////////////////////////////////////////////////////////////////////
// Base GROUPGD.DB (Fichier groupe des fils guide)
//////////////////////////////////////////////////////////////////////////////

#ifndef __linux__
class _CLASSELEXI NSGroupGdData
#else
class NSGroupGdData
#endif
{
	public :
  	//
    // Variables de stockage des valeurs
    //
    char groupe[GRPGD_GROUPE_LEN + 1] ;
    char libelle[GRPGD_LIBELLE_LEN + 1] ;
    char date[GRPGD_DATE_LEN + 1] ;
    char util[GRPGD_UTIL_LEN + 1] ;

    NSGroupGdData() ;
    NSGroupGdData(NSGroupGdData& rv) ;

    NSGroupGdData& operator=(NSGroupGdData src) ;
    int            operator==(const NSGroupGdData& o) ;

    string getGroup() { return string(groupe) ;  }
    string getLabel() { return string(libelle) ; }
    string getDate()  { return string(date) ;    }
    string getUser()  { return string(util) ;    }

    void setGroup(string sGrpe)  { strcpy(groupe,  sGrpe.c_str()) ;  }
    void setLabel(string sLabel) { strcpy(libelle, sLabel.c_str()) ; }
    void setDate(string sDate)   { strcpy(date,    sDate.c_str()) ;  }
    void setUser(string sUser)   { strcpy(util,    sUser.c_str()) ;  }
};

//
// Objet derive de NSFiche utilise pour les operations de base de donnees
//
#ifndef _ENTERPRISE_DLL
class _CLASSELEXI NSGroupGd : public NSFiche
#else
#ifndef __linux__
class _CLASSELEXI NSGroupGd : public NSSuperRoot
#else
class NSGroupGd : public NSSuperRoot
#endif
#endif
{
	public :

	  //
	  // Variables de stockage des valeurs
	  //
	  NSGroupGdData* pDonnees ;

	  NSGroupGd(NSSuper* pSuper) ;
	  ~NSGroupGd() ;

#ifndef _ENTERPRISE_DLL
	  DBIResult open() ;
#endif
	  void alimenteFiche() ;
	  void videFiche() ;

    string getGroup() { return pDonnees->getGroup() ; }
    string getLabel() { return pDonnees->getLabel() ; }
    string getDate()  { return pDonnees->getDate() ;  }
    string getUser()  { return pDonnees->getUser() ;  }

    void setGroup(string sGrpe)  { pDonnees->setGroup(sGrpe) ;  }
    void setLabel(string sLabel) { pDonnees->setLabel(sLabel) ; }
    void setDate(string sDate)   { pDonnees->setDate(sDate) ;   }
    void setUser(string sUser)   { pDonnees->setUser(sUser) ;   }
};

//
// Objet contenant uniquement un pointeur sur les donnees
//
#ifndef __linux__
class _CLASSELEXI NSGroupGdInfo
#else
class NSGroupGdInfo
#endif
{
	public :
  	//
    // Objet qui contient les donnees
    //
    NSGroupGdData* pDonnees ;

    NSGroupGdInfo() ;
		NSGroupGdInfo(NSGroupGd*) ;
		NSGroupGdInfo(NSGroupGdInfo& rv) ;
    ~NSGroupGdInfo() ;

    NSGroupGdInfo& operator=(NSGroupGdInfo src) ;
		int            operator == ( const NSGroupGdInfo& o ) ;

    string getGroup() { return pDonnees->getGroup() ; }
    string getLabel() { return pDonnees->getLabel() ; }
    string getDate()  { return pDonnees->getDate() ;  }
    string getUser()  { return pDonnees->getUser() ;  }

    void setGroup(string sGrpe)  { pDonnees->setGroup(sGrpe) ;  }
    void setLabel(string sLabel) { pDonnees->setLabel(sLabel) ; }
    void setDate(string sDate)   { pDonnees->setDate(sDate) ;   }
    void setUser(string sUser)   { pDonnees->setUser(sUser) ;   }
};

//
// Definition de NSGroupGdArray (Array de NSGroupGd)
//
typedef vector<NSGroupGdInfo*> NSGroupGdVector ;
typedef NSGroupGdVector::iterator NSGroupGdIter ;

#ifndef __linux__
class _CLASSELEXI NSGroupGdArray : public NSGroupGdVector
#else
class NSGroupGdArray : public NSGroupGdVector
#endif
{
	public :
  	// Constructeurs
    NSGroupGdArray() : NSGroupGdVector() {}
    NSGroupGdArray(NSGroupGdArray& rv) ;
    // Destructeur
    virtual ~NSGroupGdArray() ;

    NSGroupGdArray& operator=(NSGroupGdArray src) ;
    void vider() ;
};

#endif    // __NSGUIDE_H

