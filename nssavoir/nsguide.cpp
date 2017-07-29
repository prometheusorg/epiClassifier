//---------------------------------------------------------------------------
//    NSFICHE.CPP
//
//  Implementation des objets PARADOX de NAUTILUS
//
//  1ere version : PA juillet 93       Derniere modif : 17/06/94
//---------------------------------------------------------------------------
#include <string>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
#else
  #include "partage/nsdivfct.h"
  #include "nautilus/nssuper.h"
  #include "ns_sgbd/nsfiche.h"
#endif

//#include "ns_vector.h"
#include "nssavoir/nsguide.h"

//***************************************************************************
// Implémentation des mèthodes BBChemData
//***************************************************************************
void
BBChemData::metAZero()
{
	// Met les champs de données à zéro
	memset(code,	   0, CH_CODE_LEN + 1) ;
	memset(auteur,   0, CH_AUTEUR_LEN + 1) ;
	memset(chemin,   0, CH_CHEMIN_LEN + 1) ;
	memset(scenario, 0, CH_SCENARIO_LEN + 1) ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
BBChemData::BBChemData(BBChemData& rv)
{
	strcpy(code,	   rv.code) ;
	strcpy(auteur,   rv.auteur) ;
	strcpy(chemin,   rv.chemin) ;
	strcpy(scenario, rv.scenario) ;
}

//---------------------------------------------------------------------------
//   Opérateur =
//---------------------------------------------------------------------------
BBChemData&
BBChemData::operator=(BBChemData rv)
{
	if (this == &rv)
		return *this ;

 	strcpy(code,	   rv.code) ;
	strcpy(auteur,   rv.auteur) ;
	strcpy(chemin,   rv.chemin) ;
	strcpy(scenario, rv.scenario) ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
BBChemData::operator == (const BBChemData& o)
{
	if  ((strcmp(code, o.code) == 0) &&
	     (strcmp(auteur, o.auteur) == 0) &&
       (strcmp(chemin, o.chemin) == 0) &&
       (strcmp(scenario, o.scenario) == 0))
		return 1 ;
	else
		return 0 ;
}

BBChemData::~BBChemData()
{
}

BBChem::BBChem(NSSuper* pSuper)
#ifndef _ENTERPRISE_DLL
       :NSFiche(pSuper)
#else
       :NSSuperRoot(pSuper)
#endif
{
	// Ne pas Créer l'objet de données
	// c'est aux classes dérivées de la faire
}

BBChem::~BBChem()
{
	if (pDonnees)
		delete pDonnees ;

	pDonnees = 0 ;
}

//---------------------------------------------------------------------------
//  alimenteFiche()
//
//  ATTENTION : les champs communs n'ont pas le même emplacement
//  				 (FIELD) selon les fichiers. Il faut donc laisser
//					 les objets dérivés faire le travail
//---------------------------------------------------------------------------
void BBChem::alimenteFiche()
{
}

//---------------------------------------------------------------------------
//  videFiche()
//
//  ATTENTION : les champs communs n'ont pas le même emplacement
//  				 (FIELD) selon les fichiers. Il faut donc laisser
//					 les objets dérivés faire le travail
//---------------------------------------------------------------------------
void BBChem::videFiche()
{
}

//
// Les méthodes open() ne font rien, ce sont les classes
// dérivées qui ouvrent les tables
//
#ifndef _ENTERPRISE_DLL
DBIResult
BBChem::open()
{
	return(DBIERR_NONE) ;
}

DBIResult
BBChem::open(string tableName)
{
	return(DBIERR_NONE) ;
}
#endif

//***************************************************************************
// Implémentation des mèthodes BBItemData
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:		BBItemData::metAZero()
//  Description:	Initialise les champs de données à zéro.
//  Retour:			Aucun
//---------------------------------------------------------------------------
void
BBItemData::metAZero()
{
	// Met les champs de données à zéro
	BBChemData::metAZero() ;

	memset(commentaire, 	  0, BB_COMMENTAIRE_LEN + 1) ;
  memset(fichierDialogue, 0, BB_FICHIER_DIALOGUE_LEN + 1) ;
  memset(nomDialogue, 	  0, BB_NOM_DIALOGUE_LEN + 1) ;
	memset(ouvreDialogue, 	0, BB_OUVRE_DIALOGUE_LEN + 1) ;
	memset(fichierFonction, 0, BB_FICHIER_FONCTION_LEN + 1) ;
	memset(nomFonction, 	  0, BB_NOM_FONCTION_LEN + 1) ;
	memset(decalageNiveau, 	0, BB_DECALAGE_NIVEAU_LEN + 1) ;
	memset(fils, 			      0, BB_FILS_LEN + 1) ;
	memset(exclusion, 		  0, BB_EXCLUSION_LEN + 1) ;
  memset(automatique, 	  0, BB_AUTOMATIQUE_LEN + 1) ;
  memset(actif_vide, 		  0, BB_ACTIF_VIDE_LEN + 1) ;
  memset(unicite_lesion, 	0, BB_UNICITE_LESION_LEN + 1) ;
  memset(impose, 		      0, BB_IMPOSE_LEN + 1) ;
  memset(tri, 			      0, BB_TRI_LEN + 1) ;
}

//---------------------------------------------------------------------------
//  Fonction:		BBItemData::BBItemData(BBItemData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
BBItemData::BBItemData(BBItemData& rv)
{
	strcpy(code,		  	    rv.code) ;
  strcpy(auteur, 			    rv.auteur) ;
  strcpy(chemin, 			    rv.chemin) ;
  strcpy(commentaire,		  rv.commentaire) ;
  strcpy(fichierDialogue, rv.fichierDialogue) ;
  strcpy(nomDialogue, 	  rv.nomDialogue) ;
	strcpy(ouvreDialogue, 	rv.ouvreDialogue) ;
	strcpy(fichierFonction, rv.fichierFonction) ;
	strcpy(nomFonction, 	  rv.nomFonction) ;
	strcpy(decalageNiveau, 	rv.decalageNiveau) ;
	strcpy(fils,		  	    rv.fils) ;
	strcpy(exclusion,		    rv.exclusion) ;
  strcpy(automatique,		  rv.automatique) ;
	strcpy(actif_vide, 		  rv.actif_vide) ;
	strcpy(unicite_lesion, 	rv.unicite_lesion) ;
	strcpy(impose, 	        rv.impose) ;
	strcpy(tri, 			      rv.tri) ;
  strcpy(scenario, 		    rv.scenario) ;
}

//---------------------------------------------------------------------------
//  Fonction:		BBItemData::operator=(BBItemData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
BBItemData& BBItemData::operator=(BBItemData rv)
{
	if (this == &rv)
		return *this ;

	metAZero() ;

	strcpy(code,		  		  rv.code) ;
  strcpy(auteur, 			    rv.auteur) ;
  strcpy(chemin, 			    rv.chemin) ;
  strcpy(commentaire,		  rv.commentaire) ;
  strcpy(fichierDialogue, rv.fichierDialogue) ;
  strcpy(nomDialogue, 	  rv.nomDialogue) ;
	strcpy(ouvreDialogue, 	rv.ouvreDialogue) ;
	strcpy(fichierFonction, rv.fichierFonction) ;
	strcpy(nomFonction, 	  rv.nomFonction) ;
	strcpy(decalageNiveau, 	rv.decalageNiveau) ;
	strcpy(fils,		  	    rv.fils) ;
	strcpy(exclusion,		    rv.exclusion) ;
  strcpy(automatique,		  rv.automatique) ;
	strcpy(actif_vide, 		  rv.actif_vide) ;
	strcpy(unicite_lesion, 	rv.unicite_lesion) ;
	strcpy(impose, 	        rv.impose) ;
	strcpy(tri, 			      rv.tri) ;
  strcpy(scenario, 		    rv.scenario) ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		BBItemData::operator==(const BBItemData& o)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int BBItemData::operator == ( const BBItemData& o )
{
	if ((strcmp(code,            o.code)             == 0) &&
      (strcmp(auteur, 		     o.auteur)           == 0) &&
      (strcmp(chemin, 		     o.chemin)           == 0) &&
      (strcmp(commentaire,	   o.commentaire)      == 0) &&
      (strcmp(fichierDialogue, o.fichierDialogue)  == 0) &&
      (strcmp(nomDialogue, 	   o.nomDialogue)      == 0) &&
	    (strcmp(ouvreDialogue, 	 o.ouvreDialogue)    == 0) &&
	    (strcmp(fichierFonction, o.fichierFonction)  == 0) &&
	    (strcmp(nomFonction, 	   o.nomFonction)      == 0) &&
	    (strcmp(decalageNiveau,  o.decalageNiveau)   == 0) &&
	    (strcmp(fils,		  	     o.fils)             == 0) &&
	    (strcmp(exclusion,		   o.exclusion)        == 0) &&
      (strcmp(automatique,	   o.automatique)      == 0) &&
	    (strcmp(actif_vide, 	   o.actif_vide)       == 0) &&
	    (strcmp(unicite_lesion,  o.unicite_lesion)   == 0) &&
	    (strcmp(impose, 	       o.impose)           == 0) &&
	    (strcmp(tri, 			       o.tri)              == 0) &&
      (strcmp(scenario, 		   o.scenario)         == 0))
		return 1 ;
	else
		return 0 ;
}

BBItemData::~BBItemData()
{
}

//***************************************************************************
// Implémentation des méthodes BBItemInfo
//***************************************************************************

//---------------------------------------------------------------------------
//  Fonction:		BBItemInfo::BBItemInfo()
//  Description:	Constructeur par défaut
//  Retour:			Rien
//---------------------------------------------------------------------------
BBItemInfo::BBItemInfo()
{
	// Crée l'objet de données
	pDonnees = new BBItemData() ;
}

//---------------------------------------------------------------------------
//  Fonction:		BBItemInfo::BBItemInfo(BBFiche*)
//  Description:	Constructeur à partir d'un BBFiche
//  Retour:			Rien
//---------------------------------------------------------------------------
BBItemInfo::BBItemInfo(BBFiche* pBBFiche)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new BBItemData() ;
	//
	// Copie les valeurs du NSCorrespondant
	//
	*pDonnees = *(static_cast<BBItemData*>(pBBFiche->pDonnees)) ;
}

//---------------------------------------------------------------------------
//  Fonction:		BBItemInfo::~BBItemInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
BBItemInfo::~BBItemInfo()
{
	delete pDonnees ;
}

//---------------------------------------------------------------------------
//  Fonction:		BBItemInfo::BBItemInfo(const BBItemInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
BBItemInfo::BBItemInfo(BBItemInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new BBItemData() ;
	//
	// Copie les valeurs du BBItemInfo d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------
//  Fonction:		BBItemInfo::operator=(BBItemInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
BBItemInfo&
BBItemInfo::operator=(BBItemInfo src)
{
	if (this == &src)
		return *this ;

	*pDonnees = *(src.pDonnees) ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		BBItemInfo::operator==(BBItemInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int BBItemInfo::operator == (BBItemInfo& o)
{
	return (*pDonnees == *(o.pDonnees)) ;
}

//-------------------------------------------------------------------------
//----------------------- METHODES DE BBItemArray ---------------------
//-------------------------------------------------------------------------

// constructeur copie

BBItemArray::BBItemArray(BBItemArray& rv)
					  : BBItemVector()
{
	if (!(rv.empty()))
	   for (BBItemIter i = rv.begin(); i != rv.end(); i++)
			 	push_back(new BBItemInfo(*(*i))) ;
}

// fonction vider()
void
BBItemArray::vider()
{
	if (empty())
		return ;

	for (BBItemIter i = begin(); i != end(); )
	{
  	delete *i ;
    erase(i) ;
	}
}

// destructeur
BBItemArray::~BBItemArray()
{
	vider() ;
}

//---------------------------------------------------------------------------
//  Fonction	 : BBItemArray::operator=(BBItemArray src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
BBItemArray&
BBItemArray::operator=(BBItemArray src)
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (BBItemIter i = src.begin(); i != src.end(); i++)
			push_back(new BBItemInfo(*(*i))) ;

	return *this ;
}

//***************************************************************************
// Implémentation des méthodes BBFiche
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:  BBFiche::BBFiche(NSSuper* pSuper)
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
BBFiche::BBFiche(NSSuper* pSuper)
		    :BBChem(pSuper)
{
	// Crée l'objet de données
	pDonnees = new BBItemData() ;
}

BBFiche::~BBFiche()
{
	if (pDonnees)
		delete pDonnees ;
	pDonnees = 0 ;
}

//---------------------------------------------------------------------------
//  Function:  BBFiche::alimenteFiche()
//
//  Arguments:   Aucun
//  Description: Transfère le contenu de pRecBuff dans les variables de
//               la fiche
//  Returns:     Rien
//---------------------------------------------------------------------------
void BBFiche::alimenteFiche()
{
	pDonnees->metAZero() ;
#ifndef _ENTERPRISE_DLL
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->code, 			     BB_CODE_FIELD, 	 		      CH_CODE_LEN) ;
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->auteur, 		     BB_AUTEUR_FIELD, 	 		    CH_AUTEUR_LEN) ;
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->chemin, 		     BB_CHEMIN_FIELD, 	 		    CH_CHEMIN_LEN) ;
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->commentaire, 	   BB_COMMENTAIRE_FIELD, 	  	BB_COMMENTAIRE_LEN) ;
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->fichierDialogue, BB_FICHIER_DIALOGUE_FIELD, BB_FICHIER_DIALOGUE_LEN) ;
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->nomDialogue, 	   BB_NOM_DIALOGUE_FIELD, 	  BB_NOM_DIALOGUE_LEN) ;
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->ouvreDialogue,   BB_OUVRE_DIALOGUE_FIELD,   BB_OUVRE_DIALOGUE_LEN) ;
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->fichierFonction, BB_FICHIER_FONCTION_FIELD, BB_FICHIER_FONCTION_LEN) ;
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->nomFonction, 	   BB_NOM_FONCTION_FIELD, 	  BB_NOM_FONCTION_LEN) ;
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->decalageNiveau,  BB_DECALAGE_NIVEAU_FIELD,  BB_DECALAGE_NIVEAU_LEN) ;
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->fils,  			     BB_FILS_FIELD,  			      BB_FILS_LEN) ;
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->exclusion,  	   BB_EXCLUSION_FIELD,  		  BB_EXCLUSION_LEN) ;
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->automatique,  	 BB_AUTOMATIQUE_FIELD,  	  BB_AUTOMATIQUE_LEN) ;
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->actif_vide,  	   BB_ACTIF_VIDE_FIELD,  	  	BB_ACTIF_VIDE_LEN) ;
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->unicite_lesion,  BB_UNICITE_LESION_FIELD,   BB_UNICITE_LESION_LEN) ;
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->impose,  	       BB_IMPOSE_FIELD,   	      BB_IMPOSE_LEN) ;
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->tri,  	  		   BB_TRI_FIELD,   	  		    BB_TRI_LEN) ;
  alimenteChamp((static_cast<BBItemData*>(pDonnees))->scenario, 		   BB_SCENARIO_FIELD, 		    CH_SCENARIO_LEN) ;
#endif
}

//---------------------------------------------------------------------------
//  Function:    BBFiche::videFiche()
//
//  Arguments:   Aucun
//  Description: Transfère le contenu des valeurs de la fiche dans pRecBuff
//  Returns:     Rien
//---------------------------------------------------------------------------
void BBFiche::videFiche()
{
#ifndef _ENTERPRISE_DLL
	videChamp((static_cast<BBItemData*>(pDonnees))->code, 			     BB_CODE_FIELD, 	 		      CH_CODE_LEN) ;
  videChamp((static_cast<BBItemData*>(pDonnees))->auteur, 		     BB_AUTEUR_FIELD, 	 		    CH_AUTEUR_LEN) ;
  videChamp((static_cast<BBItemData*>(pDonnees))->chemin, 		     BB_CHEMIN_FIELD, 	 		    CH_CHEMIN_LEN) ;
  videChamp((static_cast<BBItemData*>(pDonnees))->commentaire, 	   BB_COMMENTAIRE_FIELD, 	    BB_COMMENTAIRE_LEN) ;
  videChamp((static_cast<BBItemData*>(pDonnees))->fichierDialogue, BB_FICHIER_DIALOGUE_FIELD, BB_FICHIER_DIALOGUE_LEN) ;
  videChamp((static_cast<BBItemData*>(pDonnees))->nomDialogue, 	   BB_NOM_DIALOGUE_FIELD, 	  BB_NOM_DIALOGUE_LEN) ;
  videChamp((static_cast<BBItemData*>(pDonnees))->ouvreDialogue,   BB_OUVRE_DIALOGUE_FIELD,   BB_OUVRE_DIALOGUE_LEN) ;
  videChamp((static_cast<BBItemData*>(pDonnees))->fichierFonction, BB_FICHIER_FONCTION_FIELD, BB_FICHIER_FONCTION_LEN) ;
  videChamp((static_cast<BBItemData*>(pDonnees))->nomFonction, 	   BB_NOM_FONCTION_FIELD, 	  BB_NOM_FONCTION_LEN) ;
  videChamp((static_cast<BBItemData*>(pDonnees))->decalageNiveau,  BB_DECALAGE_NIVEAU_FIELD,  BB_DECALAGE_NIVEAU_LEN) ;
  videChamp((static_cast<BBItemData*>(pDonnees))->fils,  			     BB_FILS_FIELD,  		        BB_FILS_LEN) ;
  videChamp((static_cast<BBItemData*>(pDonnees))->exclusion,  	   BB_EXCLUSION_FIELD,  		  BB_EXCLUSION_LEN) ;
  videChamp((static_cast<BBItemData*>(pDonnees))->automatique,  	 BB_AUTOMATIQUE_FIELD,  	  BB_AUTOMATIQUE_LEN) ;
  videChamp((static_cast<BBItemData*>(pDonnees))->actif_vide,  	   BB_ACTIF_VIDE_FIELD,  	    BB_ACTIF_VIDE_LEN) ;
  videChamp((static_cast<BBItemData*>(pDonnees))->unicite_lesion,  BB_UNICITE_LESION_FIELD,   BB_UNICITE_LESION_LEN) ;
  videChamp((static_cast<BBItemData*>(pDonnees))->impose,  	       BB_IMPOSE_FIELD,   	      BB_IMPOSE_LEN) ;
  videChamp((static_cast<BBItemData*>(pDonnees))->tri,  	  		   BB_TRI_FIELD,   	  		    BB_TRI_LEN) ;
  videChamp((static_cast<BBItemData*>(pDonnees))->scenario, 		   BB_SCENARIO_FIELD, 		    CH_SCENARIO_LEN) ;
#endif
}

//---------------------------------------------------------------------------
//  Function:  BBFiche::open(void)
//
//  Arguments:
//
//  Description:
//             Ouvre la table primaire et l'index secondaire
//  Returns:
//             PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
DBIResult BBFiche::open()
{
	//
  // Appelle la fonction open() de la classe de base pour ouvrir
  // l'index primaire
  //
  lastError = NSFiche::open("GUIDES", NSF_GUIDES) ;

  return(lastError) ;
}
#endif

//---------------------------------------------------------------------------
//  Function:  BBFiche::open(unsigned char *tableName)
//
//  Arguments:
//
//  Description:
//             Ouvre la table primaire et l'index secondaire
//  Returns:
//             PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
DBIResult BBFiche::open(string tableName)
{
	//
  // Appelle la fonction open() de la classe de base pour ouvrir
  // l'index primaire
  //
  lastError = NSFiche::open(tableName, NSF_GUIDES) ;

  return(lastError) ;
}
#endif

//***************************************************************************
// Implémentation des mèthodes BBDecode
//***************************************************************************
//---------------------------------------------------------------------------
//  metAZero()
//---------------------------------------------------------------------------
void
BBDecodeData::metAZero()
{
	// Met les champs de données à zéro
  BBChemData::metAZero() ;

	memset(commentaire, 0, DKD_COMMENTAIRE_LEN + 1) ;
	memset(fichier, 	  0, DKD_FICHIER_LEN + 1) ;
	memset(nomFonction, 0, DKD_NOM_FONCTION_LEN + 1) ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
BBDecodeData::BBDecodeData(BBDecodeData& rv)
{
	strcpy(code,		    rv.code) ;
  strcpy(auteur, 		  rv.auteur) ;
  strcpy(chemin, 		  rv.chemin) ;
  strcpy(commentaire,	rv.commentaire) ;
  strcpy(fichier, 	  rv.fichier) ;
	strcpy(nomFonction, rv.nomFonction) ;
  strcpy(scenario, 	  rv.scenario) ;
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
BBDecodeData&
BBDecodeData::operator=(BBDecodeData rv)
{
	if (this == &rv)
  	return *this ;

	metAZero() ;

	strcpy(code,		    rv.code) ;
	strcpy(auteur, 	    rv.auteur) ;
	strcpy(chemin, 	    rv.chemin) ;
	strcpy(commentaire, rv.commentaire) ;
	strcpy(fichier, 	  rv.fichier) ;
	strcpy(nomFonction, rv.nomFonction) ;
	strcpy(scenario, 	  rv.scenario) ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
BBDecodeData::operator == (const BBDecodeData& o)
{
	if (strcmp(code, o.code) == 0)
		return 1;
	else
		return 0;
}

BBDecodeData::~BBDecodeData()
{
}

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
BBDecode::BBDecode(NSSuper* pSuper)
         :BBChem(pSuper)
{
	// Crée l'objet de données
	pDonnees = new BBDecodeData() ;
}

BBDecode::~BBDecode()
{
	if (pDonnees)
		delete pDonnees ;
	pDonnees = 0 ;
}

//---------------------------------------------------------------------------
//  BBDecode::alimenteFiche()
//
//  Transfère le contenu de pRecBuff dans les variables de
//  la fiche
//---------------------------------------------------------------------------
void
BBDecode::alimenteFiche()
{
	pDonnees->metAZero() ;
#ifndef _ENTERPRISE_DLL
	alimenteChamp((static_cast<BBDecodeData*>(pDonnees))->code, 		   DKD_CODE_FIELD, 	 		   CH_CODE_LEN) ;
  alimenteChamp((static_cast<BBDecodeData*>(pDonnees))->auteur, 		 DKD_AUTEUR_FIELD, 	 	   CH_AUTEUR_LEN) ;
  alimenteChamp((static_cast<BBDecodeData*>(pDonnees))->chemin, 		 DKD_CHEMIN_FIELD, 	 	   CH_CHEMIN_LEN) ;
  alimenteChamp((static_cast<BBDecodeData*>(pDonnees))->commentaire, DKD_COMMENTAIRE_FIELD,  DKD_COMMENTAIRE_LEN) ;
  alimenteChamp((static_cast<BBDecodeData*>(pDonnees))->fichier, 	   DKD_FICHIER_FIELD, 		 DKD_FICHIER_LEN) ;
  alimenteChamp((static_cast<BBDecodeData*>(pDonnees))->nomFonction, DKD_NOM_FONCTION_FIELD, DKD_NOM_FONCTION_LEN) ;
  alimenteChamp((static_cast<BBDecodeData*>(pDonnees))->scenario, 	 DKD_SCENARIO_FIELD, 	   CH_SCENARIO_LEN) ;
#endif
}

//---------------------------------------------------------------------------
//  BBDecode::videFiche()
//
//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
BBDecode::videFiche()
{
#ifndef _ENTERPRISE_DLL
	videChamp((static_cast<BBDecodeData*>(pDonnees))->code, 			 DKD_CODE_FIELD, 	 	  	 CH_CODE_LEN) ;
  videChamp((static_cast<BBDecodeData*>(pDonnees))->auteur, 		 DKD_AUTEUR_FIELD, 	 	   CH_AUTEUR_LEN) ;
  videChamp((static_cast<BBDecodeData*>(pDonnees))->chemin, 		 DKD_CHEMIN_FIELD, 	 	   CH_CHEMIN_LEN) ;
  videChamp((static_cast<BBDecodeData*>(pDonnees))->commentaire, DKD_COMMENTAIRE_FIELD,  DKD_COMMENTAIRE_LEN) ;
  videChamp((static_cast<BBDecodeData*>(pDonnees))->fichier, 		 DKD_FICHIER_FIELD, 	   DKD_FICHIER_LEN) ;
  videChamp((static_cast<BBDecodeData*>(pDonnees))->nomFonction, DKD_NOM_FONCTION_FIELD, DKD_NOM_FONCTION_LEN) ;
  videChamp((static_cast<BBDecodeData*>(pDonnees))->scenario, 	 DKD_SCENARIO_FIELD, 	   CH_SCENARIO_LEN) ;
#endif
}

//---------------------------------------------------------------------------
//  BBDecode::open(void)
//
//  Ouvre la table primaire et l'index secondaire
//---------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
DBIResult
BBDecode::open()
{
	//
  // Appelle la fonction open() de la classe de base pour ouvrir
  // l'index primaire
  //
  lastError = NSFiche::open("DECODE", NSF_GUIDES) ;

  return(lastError) ;
}
#endif

//---------------------------------------------------------------------------
//  BBDecode::open(unsigned char *tableName)
//
//  Ouvre la table primaire et l'index secondaire
//---------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
DBIResult
BBDecode::open(string tableName)
{
	//
  // Appelle la fonction open() de la classe de base pour ouvrir
  // l'index primaire
  //
  lastError = NSFiche::open(tableName, NSF_GUIDES) ;

  return(lastError) ;
}
#endif

//===========================================================================
// CLASSE NSGROUPGD
//---------------------------------------------------------------------------
//  Classe NSGroupGdData
//---------------------------------------------------------------------------

NSGroupGdData::NSGroupGdData()
{
	// met les champs de donnees a zero
	memset(groupe,	0, GRPGD_GROUPE_LEN + 1) ;
  memset(libelle,	0, GRPGD_LIBELLE_LEN + 1) ;
  memset(date,		0, GRPGD_DATE_LEN + 1) ;
  memset(util,   	0, GRPGD_UTIL_LEN + 1) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSGroupGdData::NSGroupGdData(NSGroupGdData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSGroupGdData::NSGroupGdData(NSGroupGdData& rv)
{
	strcpy(groupe,  rv.groupe) ;
	strcpy(libelle, rv.libelle) ;
	strcpy(date, 		rv.date) ;
	strcpy(util, 		rv.util) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSGroupGdData::operator=(NSGroupGdData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSGroupGdData&
NSGroupGdData::operator=(NSGroupGdData src)
{
	if (this == &src)
		return *this ;

	strcpy(groupe,  src.groupe) ;
	strcpy(libelle, src.libelle) ;
	strcpy(date, 	  src.date) ;
	strcpy(util,		src.util) ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSGroupGdData::operator==(const NSGroupGdData& o)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSGroupGdData::operator == ( const NSGroupGdData& o )
{
	if ((strcmp(groupe,  o.groupe)  == 0) &&
		  (strcmp(libelle, o.libelle) == 0) &&
		  (strcmp(date, 	 o.date) 		== 0) &&
      (strcmp(util,		 o.util)	  == 0))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------
//  Classe NSGroupGd
//---------------------------------------------------------------------------

NSGroupGd::NSGroupGd(NSSuper* pSuper)
#ifndef _ENTERPRISE_DLL
       :NSFiche(pSuper)
#else
       :NSSuperRoot(pSuper)
#endif
{
	// cree un objet de donnees
	pDonnees = new NSGroupGdData() ;
}

NSGroupGd::~NSGroupGd()
{
	delete pDonnees ;
	pDonnees = 0 ;
}

// Ouverture et fermeture de la base des templates
//////////////////////////////////////////////////

#ifndef _ENTERPRISE_DLL
DBIResult
NSGroupGd::open()
{
	char tableName[] = "GROUPGD.DB" ;
  //
  // Appelle la fonction open() de la classe de base pour ouvrir
  // l'index primaire
  //
  lastError = NSFiche::open(tableName, NSF_GUIDES) ;
  return(lastError) ;
}
#endif

// alimenteFiche
/////////////////////////////////////////////////////////////////////////

void
NSGroupGd::alimenteFiche()
{
#ifndef _ENTERPRISE_DLL
	alimenteChamp(pDonnees->groupe,	 GRPGD_GROUPE_FIELD, 	GRPGD_GROUPE_LEN) ;
	alimenteChamp(pDonnees->libelle, GRPGD_LIBELLE_FIELD, GRPGD_LIBELLE_LEN) ;
	alimenteChamp(pDonnees->date,		 GRPGD_DATE_FIELD, 		GRPGD_DATE_LEN) ;
	alimenteChamp(pDonnees->util,		 GRPGD_UTIL_FIELD, 		GRPGD_UTIL_LEN) ;
#endif
}

// videFiche
/////////////////////////////////////////////////////////////////////////

void
NSGroupGd::videFiche()
{
#ifndef _ENTERPRISE_DLL
	videChamp(pDonnees->groupe,	 GRPGD_GROUPE_FIELD, 	GRPGD_GROUPE_LEN) ;
	videChamp(pDonnees->libelle, GRPGD_LIBELLE_FIELD, GRPGD_LIBELLE_LEN) ;
	videChamp(pDonnees->date,		 GRPGD_DATE_FIELD, 		GRPGD_DATE_LEN) ;
	videChamp(pDonnees->util,		 GRPGD_UTIL_FIELD, 		GRPGD_UTIL_LEN) ;
#endif
}

//---------------------------------------------------------------------------
//  Classe NSGroupGdInfo
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Fonction:		NSGroupGdInfo::NSGroupGdInfo()
//  Description:	Constructeur par défaut
//  Retour:			Rien
//---------------------------------------------------------------------------
NSGroupGdInfo::NSGroupGdInfo()
{
	// Crée l'objet de données
	pDonnees = new NSGroupGdData() ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSGroupGdInfo::NSGroupGdInfo(NSGroupGd*)
//  Description:	Constructeur à partir d'un NSGroupGd
//  Retour:			Rien
//---------------------------------------------------------------------------
NSGroupGdInfo::NSGroupGdInfo(NSGroupGd* pGroupGd)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSGroupGdData() ;
	//
	// Copie les valeurs du NSCorrespondant
	//
	*pDonnees = *(pGroupGd->pDonnees) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSGroupGdInfo::~NSGroupGdInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
NSGroupGdInfo::~NSGroupGdInfo()
{
	delete pDonnees ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSGroupGdInfo::NSGroupGdInfo(const NSGroupGdInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSGroupGdInfo::NSGroupGdInfo(NSGroupGdInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSGroupGdData() ;
	//
	// Copie les valeurs du NSGroupGdInfo d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSGroupGdInfo::operator=(NSGroupGdInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSGroupGdInfo&
NSGroupGdInfo::operator=(NSGroupGdInfo src)
{
	if (this == &src)
		return *this ;

	*pDonnees = *(src.pDonnees) ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSGroupGdInfo::operator==(NSGroupGdInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int NSGroupGdInfo::operator == ( const NSGroupGdInfo& o )
{
	 return (*pDonnees == *(o.pDonnees)) ;
}

//-------------------------------------------------------------------------
//----------------------- METHODES DE NSGroupGdArray ---------------------
//-------------------------------------------------------------------------

// constructeur copie

NSGroupGdArray::NSGroupGdArray(NSGroupGdArray& rv)
               :NSGroupGdVector()
{
	if (false == rv.empty())
	   for (NSGroupGdIter i = rv.begin() ; rv.end() != i ; i++)
			 	push_back(new NSGroupGdInfo(*(*i))) ;
}

// fonction vider()
void
NSGroupGdArray::vider()
{
	if (empty())
		return ;

	for (NSGroupGdIter i = begin() ; end() != i ; )
	{
		delete *i ;
    erase(i) ;
	}
}

// destructeur
NSGroupGdArray::~NSGroupGdArray()
{
	vider() ;
}

//---------------------------------------------------------------------------
//  Fonction	 : NSGroupGdArray::operator=(NSGroupGdArray src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSGroupGdArray&
NSGroupGdArray::operator=(NSGroupGdArray src)
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (NSGroupGdIter i = src.begin(); i != src.end(); i++)
			push_back(new NSGroupGdInfo(*(*i))) ;

	return *this ;
}

