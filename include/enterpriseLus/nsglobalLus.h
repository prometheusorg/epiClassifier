//---------------------------------------------------------------------------
//   NSGLOBAL.H
//   KRZISCH Ph.   janvier 92
//  Contient toutes les définitions et variables globales de NAUTILUS.
//---------------------------------------------------------------------------
#ifndef __NSGLOBALLUS_H
#define __NSGLOBALLUS_H

//*************************************************************************
//   Fichier des déclaration Globales
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

// Graph type
enum GRAPHTYPE { graphPerson = 0, graphObject };

// Type de pids
enum PIDSTYPE { pidsPatient = 0, pidsCorresp, pidsUtilisat, pidsPatientGroup, pidsAll };

// Type de personne (pour le mode N_TIERS)
enum PERSONTYPE { asPatient = 0, asCorresp, asUser };

const char nodeSeparationMARK      = '|' ;const char cheminSeparationMARK    = '/' ;
const char intranodeSeparationMARK = '.' ;

enum GENRE { genreNull = 0, genreMS = 1, genreFS, genreNS, genreMP, genreFP, genreNP, genreIT } ;

//---------------------------------------------------------------------------
//  Variables globales
//---------------------------------------------------------------------------

// pour les string
#define NPOS string::npos// For files

#ifndef FOR_LINUX
	#define FILESLASH "\\"
	#define NEWLINE	"\r\n"
#else
	#define FILESLASH	"/"
	#define NEWLINE	"\n\n"
#endif
//---------------------------------------------------------------------------

#endif   // __NSGLOBALLUS_H

