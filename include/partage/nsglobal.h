//---------------------------------------------------------------------------
//   NSGLOBAL.H
//   KRZISCH Ph.   janvier 92
//  Contient toutes les definitions et variables globales de NAUTILUS.
//---------------------------------------------------------------------------
#ifndef __NSGLOBAL_H
#define __NSGLOBAL_H

//*************************************************************************
//   Fichier des declaration Globales
//*************************************************************************

#define PIDS_NSS_LEN       7
#define DOC_CODE_DOCUM_LEN 6
#define OBJECT_ID_LEN      PIDS_NSS_LEN + DOC_CODE_DOCUM_LEN

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

#define NS_LEXIQUE_TAILLE 6
#define NS_LEXIQUE_SIGNIF 5

extern char	sDate [2], sTime [2], sAMPM [2][5] ;
extern int	iDate, iTime ;

// pour les string

#define NPOS string::npos

// For files

#ifndef __linux
#define FILESLASH "\\"
#define NEWLINE	"\r\n"
#else
#define FILESLASH	"/"
#define NEWLINE	"\n\n"
#endif

//---------------------------------------------------------------------------

#endif   // __NSGLOBAL_H
