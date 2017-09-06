#ifndef __NSPATHOR_H
#define __NSPATHOR_H

class NSDico ;
class NSEditDicoGlobal ;
class NSPathologInfo ;

#include <vector>

#ifndef __linux__
#include <owl/dialog.h>
#include <owl/listbox.h>
#include <owl/static.h>
#include <owl/radiobut.h>
#endif

#include "nssavoir/nspatho.h"

#ifndef __linux__
#if defined(_DANSLEXIDLL)
    #define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif
#endif

using namespace std;


typedef vector<NSPathologInfo*> NSPathoInfoArray;
typedef NSPathoInfoArray::iterator iter;

#ifndef __linux__
class _CLASSELEXI NSPathoArray : public NSPathoInfoArray
#else
class NSPathoArray : public NSPathoInfoArray
#endif
{
  public :

    // Constructeurs
    NSPathoArray() : NSPathoInfoArray() {}
    NSPathoArray(NSPathoArray& rv) ;
    // Destructeur
    virtual ~NSPathoArray() ;
    NSPathoArray& operator=(NSPathoArray src) ;
    //
    void vider() ;
};

class NSListPathoInfo : public NSPathologInfo
{
  public :

    bool ammorce ;

		NSListPathoInfo() ;
		NSListPathoInfo(NSPatholog*) ;
		NSListPathoInfo(NSListPathoInfo& rv) ;
    ~NSListPathoInfo() ;

		NSListPathoInfo& operator=(NSListPathoInfo src) ;
		int operator == ( const NSListPathoInfo& o ) ;
};

//
// Definition de NSPatPathoArray (Array de NSListPathoInfo(s))
//
//typedef TArrayAsVector<NSListPathoInfo> NSListPathoInfoArray;


//typedef vector<NSListPathoInfo*> NSListPathoInfoArray;

typedef vector<NSListPathoInfo*> NSListPathoInfoArray ;
typedef NSListPathoInfoArray::iterator iterNSListPathoInfoArray ;

class NSListPathoArray : public NSListPathoInfoArray
{
  public :

    // Constructeurs
    NSListPathoArray() : NSListPathoInfoArray() {}
    NSListPathoArray(NSListPathoArray& rv) ;
    // Destructeur
    virtual ~NSListPathoArray() ;
    NSListPathoArray& operator=(NSListPathoArray src) ;
    void vider() ;
};

#ifndef __linux__

class ChoixPathoDialog;
//*******************************************************************
//						classe List   ListeBoxLexique
//*******************************************************************
class ListeBoxLexique : public OWL::TListBox
{
  public:

    ChoixPathoDialog* pChoixPathoDialog ;

    ListeBoxLexique(TWindow* parent, int ressource, ChoixPathoDialog* pChoixPatho, TModule* module) ;
    ~ListeBoxLexique() ;

    void EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void CmPathoDblClk() ;
    void LexiqueMedicament() ;
    void LexiqueGeneral() ;

  DECLARE_RESPONSE_TABLE(ListeBoxLexique) ;
};

//
// Objet "Boite de dialogue" utilise pour selectionner une pathologie
// au sein du lexique
//
#ifdef _INCLUS
class _CLASSELEXI ChoixPathoDialog : public NSDialogWrapper
#else
class _CLASSELEXI ChoixPathoDialog : public TDialog
#endif // _INCLUS
{
  public:

    ListeBoxLexique*   _pListeBox ;
		NSListPathoArray*  _pListeArray ;
    OWL::TRadioButton* _pLexiqueGeneral ;     //choix du lexique general
    OWL::TRadioButton* _pLexiqueMedicament ;  //choix de lexique medicament
		int 			         _iElementChoisi ;
		string			       _sAmmorce ;
    NSDico* 		       _pDico ;
    NSEditDicoGlobal*  _pEdit ;
    string 			       _sTypeLexique ;        //lexique ou medical
    bool 			         _bSetup ;

    ///////
    TStatic*  			   _CurSelOfListBox ;
    //////

		ChoixPathoDialog(TWindow*, NSDico* , NSEditDicoGlobal* pEdite,
      							string sTypeLexique = "", TModule* module = 0);
		~ChoixPathoDialog();

		void SetupArray(bool bFirstPass = true);
		void SetupListe();
		void SetupWindow();
		int  DonneAmmorce(string* pAmmorce);

		void CmSelectPatho(WPARAM Cmd);

		void CmPathoDblClk(WPARAM Cmd);

    void GetCodeLexique(char* code);

    void montrerDialogue();
		void cacherDialogue();
    void InsererElementLexique(); //inserer un element du lexique suite a la touche Inserer
    void ElementPrecedent(); //selection l'element precedent
    void ElementSuivant();   //selection l'element suivant
    void LexiqueMedicament();
    void LexiqueGeneral();
		void OuvreDocument(int NumDoc);
    void SelectCodeLexique(string sCodeLexique);
    void GetLabelChoisi(string* pChaineRecuperee);
    void EvKeyDown(uint key, uint repeatCount, uint flags);
    void ConvertitMajuscule(char* chaine);
    bool EstOuvert() { return _bOuvert; }
    void setEdit(NSEditDicoGlobal* pE, string sTypeLexiq = "") { _pEdit = pE ; _sTypeLexique = sTypeLexiq ; }
    void EvClose();
    void EvDestroy();
    void CmOk();
    void CmCancel();
    bool doesBelong(THandle hWndFocus) ;

    string getAmmorce()           { return _sAmmorce ; }
    void   setAmmorce(string sAm) { _sAmmorce = sAm ; }

    NSEditDicoGlobal* getEdit()                      { return _pEdit ; }

  protected :

    TWindow* _pFenetreAppel ;  // HANDLE SUR LA BOITE DIALOGUE
    bool 	   _bOuvert ;   	    // Boite de dialogue fermee ou ouverte ?

	DECLARE_RESPONSE_TABLE(ChoixPathoDialog) ;
};

#endif // #ifndef __linux__

#endif

