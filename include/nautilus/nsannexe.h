//---------------------------------------------------------------------------
//     NSANNEXE.H
//
//  Contient les definitions des objets annexes de NAUTILUS
//
//  1ere version : PA juillet 94   Derniere modif : 18/07/94
//---------------------------------------------------------------------------
#ifndef __NSANNEXE_H
#define __NSANNEXE_H

class NSTypeMimeData ;

//
// Taille des champs du fichier TYPEMIME
//
#define MIM_EXTENSION_LEN		10
#define MIM_LIBELLE_LEN			30
#define MIM_MIME_LEN			30
#define MIM_TYPE_LEN			5

//
// Indice des champs du fichier TYPEMIME
//
#define MIM_EXTENSION_FIELD		1
#define MIM_LIBELLE_FIELD		2
#define MIM_MIME_FIELD			3
#define MIM_TYPE_FIELD			4

//
// Taille des champs du fichier ADRESSES
//
#define ADR_CODE_LEN      	5
#define ADR_ADRESSE1_LEN   35
#define ADR_ADRESSE2_LEN   35
#define ADR_ADRESSE3_LEN   35
#define ADR_CODE_POST_LEN   5
#define ADR_VILLE_LEN      35
#define ADR_TELEPHONE_LEN  15
#define ADR_FAX_LEN			   15
#define ADR_MESSAGERIE_LEN 35

//---------------------------------------------------------------------------
//  Classe NSAdresses
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  Classe NSTypeMime
//---------------------------------------------------------------------------

//
// Objet données
//
class NSTypeMimeData
{
  protected:

    //
    // Variables de stockage des valeurs
    //
    // char extension[MIM_EXTENSION_LEN + 1];
    // char libelle[MIM_LIBELLE_LEN + 1];
    // char mime[MIM_MIME_LEN + 1];
    // char type[MIM_TYPE_LEN + 1];

    string _sExtension ;
    string _sLibelle ;
    string _sMime ;
    string _sType ;

  public :

    NSTypeMimeData() ;
    NSTypeMimeData(const NSTypeMimeData& rv) ;

    string getExtension() { return _sExtension ; }
    string getLibelle()   { return _sLibelle ; }
    string getMime()      { return _sMime ; }
    string getType()      { return _sType ; }

    // char*  getExtensionPtr() { return extension ; }
    // char*  getLibellePtr()   { return libelle ; }
    // char*  getMimePtr()      { return mime ; }
    // char*  getTypePtr()      { return type ; }

    void   setExtension(string sVar) { _sExtension = sVar ; }
    void   setLibelle(string sVar)   { _sLibelle   = sVar ; }
    void   setMime(string sVar)      { _sMime      = sVar ; }
    void   setType(string sVar)      { _sType      = sVar ; }

    NSTypeMimeData& operator=(const NSTypeMimeData& src) ;
    int 			      operator==(const NSTypeMimeData& o) const ;

    void metAZero() ;
};

class NSTypeMime ;

class NSTypeMimeInfo
{
  protected:

    //
    // Objet qui contient les données
    //
    NSTypeMimeData _Donnees ;

  public :

    string getExtension() { return _Donnees.getExtension() ; }
    string getLibelle()   { return _Donnees.getLibelle() ; }
    string getMime()      { return _Donnees.getMime() ; }
    string getType()      { return _Donnees.getType() ; }

    void   setExtension(string sVar) { _Donnees.setExtension(sVar) ; }
    void   setLibelle(string sVar)   { _Donnees.setLibelle(sVar) ; }
    void   setMime(string sVar)      { _Donnees.setMime(sVar) ; }
    void   setType(string sVar)      { _Donnees.setType(sVar) ; }

    NSTypeMimeInfo() ;
		NSTypeMimeInfo(NSTypeMime*) ;    // OBSOLETE
		NSTypeMimeInfo(const NSTypeMimeInfo& rv) ;
    ~NSTypeMimeInfo() ;

    NSTypeMimeInfo& operator=(const NSTypeMimeInfo& src) ;
		int             operator==(const NSTypeMimeInfo& o) const ;
};

//---------------------------------------------------------------------------

#endif    // __NSANNEXE_H

