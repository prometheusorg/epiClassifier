//---------------------------------------------------------------------------
//     NSPERSON.H
//
//  Contient les definitions des objets PATIENT et UTILISATEUR de NAUTILUS
//
//  1ere version : KRZISCH Ph. janvier 92   Derniere modif : 12/08/93
//---------------------------------------------------------------------------
#ifndef __NSTRANSA_H
#define __NSTRANSA_H

#include <cstring>
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
// Taille des champs du fichier TRANSAC
//

#define TRAN_PATIENT_LEN    PIDS_NSS_LEN
#define TRAN_CODE_LEN   	  6
#define TRAN_OPERATEUR_LEN  PIDS_NSS_LEN
#define TRAN_DATE_DEB_LEN  12
#define TRAN_DATE_FIN_LEN  12

//
// Indice des champs du fichier TRANSAC
//
#define TRAN_PATIENT_FIELD   1
#define TRAN_CODE_FIELD  	   2
#define TRAN_OPERATEUR_FIELD 3
#define TRAN_DATE_DEB_FIELD  4
#define TRAN_DATE_FIN_FIELD  5

//---------------------------------------------------------------------------
//  Classe NSTransaction
//---------------------------------------------------------------------------

//
// Objet contenant les données
//
class NSTransacData
{
  public :

    //
	  // Variables de stockage des valeurs
	  //
    char patient[TRAN_PATIENT_LEN + 1] ;
	  char code[TRAN_CODE_LEN + 1] ;
	  char operateur[TRAN_OPERATEUR_LEN + 1] ;
	  char dateDeb[TRAN_DATE_DEB_LEN + 1] ;
    char dateFin[TRAN_DATE_FIN_LEN + 1] ;

	  NSTransacData() ;
	  NSTransacData(NSTransacData& rv) ;
    ~NSTransacData() ;

	  NSTransacData& operator=(NSTransacData src) ;
	  int            operator==(const NSTransacData& o) ;

    void metAZero() ;
};

//
// Objet dérivé de NSFiche servant aux accès de base de données
//
/*
class _CLASSELEXI NSTransaction : public NSFiche
{
    public :
        //
	    // Objet qui contient les données
	    //
	    NSTransacData* pDonnees;

	    NSTransaction(NSContexte* pCtx);
        NSTransaction(NSTransaction& rv);
	    ~NSTransaction();

	    void alimenteFiche();
	    void videFiche();
	    DBIResult open();
        DBIResult getPatRecord();

        bool fermeTransaction();
        bool nouvelleTransaction(bool bTranSys = false);
        bool DonneNouveauCode(char* code, bool bTransSys = false);

	    virtual bool Create();
	    virtual bool Modify();

        NSTransaction& operator=(NSTransaction src);
        int operator == ( const NSTransaction& o );
};
*/

//---------------------------------------------------------------------------
//  Classe NSTransacInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
#ifndef __linux__
class _CLASSELEXI NSTransacInfo
#else
class NSTransacInfo
#endif
{
	public :
		//
		// Objet qui contient les données
		//
		NSTransacData* pDonnees;

		NSTransacInfo();
		// NSTransacInfo(NSTransaction*);
		NSTransacInfo(NSTransacInfo& rv);
    ~NSTransacInfo();

		NSTransacInfo& operator=(NSTransacInfo src);
		int operator == ( const NSTransacInfo& o );
};

//---------------------------------------------------------------------------
#endif    // __NSTRANSA_H
