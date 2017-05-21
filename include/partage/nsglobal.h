//---------------------------------------------------------------------------
//   NSGLOBAL.H
//   KRZISCH Ph.   janvier 92
//  Contient toutes les definitions et variables globales de NAUTILUS.
//---------------------------------------------------------------------------
#ifndef __NSGLOBAL_H
#define __NSGLOBAL_H

#ifdef __WINDOWS__
# include <windows.h>
#endif

// using namespace OWL;

//
// Exige par la version 5.01 du compilateur
//
// Dans BC5/INCLUDE/OCF/AUTODEFS.H
// #if !defined(_OLECTL_H_)
//		# if defined(__BIVBX_H) && !defined(NO_VBX_PICTURES)
//			#   error BIVBX.H's LPPICTURE is incompatible with OLECTL.H - define NO_VBX_PICTURES
//

#define  NO_VBX_PICTURES

//*************************************************************************
//   Fichier des declaration Globales
//*************************************************************************

#define PIDS_NSS_LEN       7
#define DOC_CODE_DOCUM_LEN 6
#define OBJECT_ID_LEN      PIDS_NSS_LEN + DOC_CODE_DOCUM_LEN

//---------------------------------------------------------------------------
//  Definition des messages WINDOWS a usage interne
//---------------------------------------------------------------------------
#define WM_CONTINUE WM_USER+0       // Message envoye a une fenetre pour
            // passer a la suivante
#define WM_CORRECT  WM_USER+1       // Message envoye par un controle EDIT
            // a sa fenetre mere
//#define WM_DICOCORRECT  WM_USER+2       // Message envoye par un controle EDIT
            // a sa fenetre mere

class NSSuper ;

class NSSuperRoot
{
   public:

		NSSuper* _pSuper ;
		NSSuperRoot(NSSuper* pSuper) : _pSuper(pSuper) {}
};

class NSContexte ;

class NSRoot
{
   public:

		NSContexte* pContexte ;
		NSRoot(NSContexte* pCtx) : pContexte(pCtx) {}
};

// Type de graphe
enum GRAPHTYPE { graphPerson = 0, graphObject };

// Type de pids
enum PIDSTYPE { pidsPatient = 0, pidsCorresp, pidsUtilisat, pidsPatientGroup, pidsAll };

// Type de personne (pour le mode N_TIERS)
enum PERSONTYPE { asPatient = 0, asCorresp, asUser };

const int MOT_PASSE     = 1;
const int UTILISATEUR   = 2;
const int PATIENT_ID    = 3;
const int FICHE_PATIENT = 4;

const char nodeSeparationMARK      = '|' ;
const char cheminSeparationMARK    = '/' ;
const char intranodeSeparationMARK = '.' ;

enum GENRE { genreNull = 0, genreMS = 1, genreFS, genreNS, genreMP, genreFP, genreNP, genreIT } ;

//---------------------------------------------------------------------------
//  Variables globales
//---------------------------------------------------------------------------

//int CompteurId = 0 ;		// ID des noeuds fixes par le BBItem
#define NS_LEXIQUE_TAILLE 6
#define NS_LEXIQUE_SIGNIF 5

#ifdef __MAIN

#ifdef __WINDOWS__
  WNDPROC lpOldProc;                // Adresse de la WndProc des contrôles EDIT
  HINSTANCE hInst;                  // Instance courante
#endif

  char	sDate [2], sTime [2], sAMPM [2][5] ;
  int	iDate, iTime ;

#else

#ifdef __WINDOWS__
  extern WNDPROC lpOldProc;
  extern HINSTANCE hInst;
#endif

  extern char	sDate [2], sTime [2], sAMPM [2][5] ;
  extern int	iDate, iTime ;

#endif

#ifdef __WINDOWS__
const ID_TIMER = 1;                 // Numero de l'horloge
#endif

//---------------------------------------------------------------------------
#define YEAR	(datetime->tm_year % 100)
#define MONTH   (datetime->tm_mon + 1)
#define MDAY	(datetime->tm_mday)
#define WDAY	(datetime->tm_wday)
#define HOUR	(datetime->tm_hour)
#define MIN	(datetime->tm_min)
#define SEC	(datetime->tm_sec)

// pour les string

#define NPOS string::npos

// For files

#define NewLineWindows "\r\n"
#define NewLineUnix    "\n\n"

#ifndef __linux
	#define FILESLASH "\\"
	#define NEWLINE	"\r\n"
#else
	#define FILESLASH	"/"
	#define NEWLINE	"\n\n"
#endif

//---------------------------------------------------------------------------

#endif   // __NSGLOBAL_H
