//---------------------------------------------------------------------------
//     NSPERSON.H
//
//  Contient les definitions des objets PATIENT et UTILISATEUR de NAUTILUS
//
//  1ere version : KRZISCH Ph. janvier 92   Derniere modif : 12/08/93
//---------------------------------------------------------------------------
#ifndef __NSPATBASE_H
#define __NSPATBASE_H

#include <cstring>

#ifndef __linux__
#if defined(_DANSLEXIDLL)
  #define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif
#endif

#define BASE_LEXI_LEN        6
#define BASE_SENS_LEN        5
#define BASE_FORMAT_SENS_LEN 3

//
// Taille des champs
//
#define BASE_LOCALISATION_LEN 8
#define BASE_TYPE_LEN  		    1
#define BASE_LEXIQUE_LEN      BASE_LEXI_LEN
#define BASE_UNIT_LEN         BASE_LEXI_LEN
#define BASE_COMPLEMENT_LEN   15
#define BASE_CERTITUDE_LEN    BASE_LEXI_LEN
#define BASE_INTERET_LEN      1
#define BASE_PLURIEL_LEN      BASE_LEXI_LEN
#define BASE_VISIBLE_LEN      1

//
// Taille des champs du fichier PATPADAT
//
#define PPD_NOEUD_LEN         5

//
// Classe NSPathoBaseData, mère de toutes les classes Data
// stockables dans des patpathoArray
//
#ifndef __linux__
class _CLASSELEXI NSPathoBaseData
#else
class NSPathoBaseData
#endif
{
  protected:

    string _sCodeLocalisation ;
		string _sType ;
		string _sLexique ;
		string _sComplement ;
		string _sUnit ;
		string _sCertitude ;
		string _sInteret ;
		string _sPluriel ;
		string _sVisible ;

	public :
		//
		// Variables de stockage des valeurs
		//
		// char codeLocalisation[BASE_LOCALISATION_LEN + 1] ;
		// char type[BASE_TYPE_LEN + 1] ;
		// char lexique[BASE_LEXIQUE_LEN + 1] ;
		// char complement[BASE_COMPLEMENT_LEN + 1] ;
		// char unit[BASE_UNIT_LEN + 1] ;
		// char certitude[BASE_CERTITUDE_LEN + 1] ;
		// char interet[BASE_INTERET_LEN + 1] ;
		// char pluriel[BASE_PLURIEL_LEN + 1] ;
		// char visible[BASE_VISIBLE_LEN + 1] ;

		NSPathoBaseData() ;
		NSPathoBaseData(const NSPathoBaseData& rv) ;
		~NSPathoBaseData() ;

		void 	 metAZero() ;

		string asString() ;
		void	 fromString(string sNode) ;

		bool   sameContent(const NSPathoBaseData& o) ;

    int    getIndice(const int indice) const ;
    int    getLigne()   const { return getIndice(0) ; }
    int    getColonne() const { return getIndice(1) ; }
    int    getZ()       const { return getIndice(2) ; }
    int    getT()       const { return getIndice(3) ; }

    void   setIndice(int indice, int valeur) ;
    void   setLigne(int valeur)   { setIndice(0, valeur) ; }
    void   setColonne(int valeur) { setIndice(1, valeur) ; }
    void   setZ(int valeur)       { setIndice(2, valeur) ; }
    void   setT(int valeur)       { setIndice(3, valeur) ; }

    string getLocalisation() const { return _sCodeLocalisation ; }
		string getTypeString()   const { return _sType ; }
		string getLexique()      const { return _sLexique ; }
		string getComplement()   const { return _sComplement ; }
		string getUnit()         const { return _sUnit ; }
		string getCertitude()    const { return _sCertitude ; }
		string getInteret()      const { return _sInteret ; }
		string getPluriel()      const { return _sPluriel ; }
		string getVisible()      const { return _sVisible ; }

    void setLocalisation(string sVar) { setFixedSizeValue(&_sCodeLocalisation, BASE_LOCALISATION_LEN, sVar) ; }
		void setTypeString(string sVar)   { setFixedSizeValue(&_sType, BASE_TYPE_LEN, sVar) ; }
		void setLexique(string sVar)      { setFixedSizeValue(&_sLexique, BASE_LEXIQUE_LEN, sVar) ; }
		void setComplement(string sVar)   { setFixedSizeValue(&_sComplement, BASE_COMPLEMENT_LEN, sVar) ; }
		void setUnit(string sVar)         { setFixedSizeValue(&_sUnit, BASE_UNIT_LEN, sVar) ; }
		void setCertitude(string sVar)    { setFixedSizeValue(&_sCertitude, BASE_CERTITUDE_LEN, sVar) ; }
		void setInteret(string sVar)      { setFixedSizeValue(&_sInteret, BASE_INTERET_LEN, sVar) ; }
		void setPluriel(string sVar)      { setFixedSizeValue(&_sPluriel, BASE_PLURIEL_LEN, sVar) ; }
		void setVisible(string sVar)      { setFixedSizeValue(&_sVisible, BASE_VISIBLE_LEN, sVar) ; }

    void setFixedSizeValue(string *pValue, size_t iSize, string sNewValue) ;

		NSPathoBaseData& operator=(const NSPathoBaseData& src) ;
    int 			       operator==(const NSPathoBaseData& o) ;
} ;

//---------------------------------------------------------------------------
#endif    // __NSPATBASE_H
