//---------------------------------------------------------------------------
//    NSPERSON.CPP
//    KRZISCH PH.   janvier 92
//  Implementation de objets NAUTILUS
//---------------------------------------------------------------------------
#include <string>
#include <cstring>

//using namespace std;

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
#else
  #include "partage\nsdivfct.h"
#endif
#include "nssavoir/nspatbas.h"

//***************************************************************************
// Implémentation des méthodes NSPathoBaseData
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSPathoBaseData::NSPathoBaseData()
{
	metAZero() ;
}

//---------------------------------------------------------------------------
//  Fonction : NSPathoBaseData::metAZero()
//  Met les champs de données à zéro
//---------------------------------------------------------------------------
void
NSPathoBaseData::metAZero()
{
  // memset(codeLocalisation, 0, BASE_LOCALISATION_LEN + 1) ;
  // memset(type, 		    0, BASE_TYPE_LEN + 1) ;
  // memset(lexique, 		    0, BASE_LEXIQUE_LEN + 1) ;
  // memset(complement, 	    0, BASE_COMPLEMENT_LEN + 1) ;
  // memset(unit,             0, BASE_UNIT_LEN + 1) ;
  // memset(certitude,        0, BASE_CERTITUDE_LEN + 1) ;
  // memset(interet,          0, BASE_INTERET_LEN + 1) ;
  // memset(pluriel,          0, BASE_PLURIEL_LEN + 1) ;
  // memset(visible,          0, BASE_VISIBLE_LEN + 1) ;

  _sCodeLocalisation = string("") ;
  _sType             = string("") ;
  _sLexique          = string("") ;
  _sComplement       = string("") ;
  _sUnit             = string("") ;
  _sCertitude        = string("") ;
  _sInteret          = string("") ;
  _sPluriel          = string("") ;
  _sVisible          = string("") ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPathoBaseData::NSPathoBaseData(const NSPathoBaseData& rv)
{
  _sCodeLocalisation = rv._sCodeLocalisation ;
  _sType             = rv._sType ;
  _sLexique          = rv._sLexique ;
  _sComplement       = rv._sComplement ;
  _sUnit             = rv._sUnit ;
  _sCertitude        = rv._sCertitude ;
  _sInteret          = rv._sInteret ;
  _sPluriel          = rv._sPluriel ;
  _sVisible          = rv._sVisible ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSPathoBaseData::~NSPathoBaseData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSPathoBaseData&
NSPathoBaseData::operator=(const NSPathoBaseData& src)
{
  if (&src == this)
		return *this ;

  _sCodeLocalisation = src._sCodeLocalisation ;
  _sType             = src._sType ;
  _sLexique          = src._sLexique ;
  _sComplement       = src._sComplement ;
  _sUnit             = src._sUnit ;
  _sCertitude        = src._sCertitude ;
  _sInteret          = src._sInteret ;
  _sPluriel          = src._sPluriel ;
  _sVisible          = src._sVisible ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSPathoBaseData::operator == (const NSPathoBaseData& o)
{
  for (int i = 0 ; i < 4 ; i++)
    if (getIndice(i) != o.getIndice(i))
      return 0 ;

	//
	// Le champ "visible", qui n'est pas à proprement parler un champ
	// de données est exclus de la comparaison
	//
	if (// (_sCodeLocalisation == o._sCodeLocalisation) &&
      (_sType             == o._sType) &&
      (_sLexique          == o._sLexique) &&
      (_sComplement       == o._sComplement) &&
      (_sUnit             == o._sUnit) &&
      (_sCertitude        == o._sCertitude) &&
      (_sInteret          == o._sInteret) &&
      (_sPluriel          == o._sPluriel) &&
      (_sVisible          == o._sVisible)
       )
		return 1 ;
	else
		return 0 ;
}

bool
NSPathoBaseData::sameContent(const NSPathoBaseData& o)
{
	//
  // Le champ "codeLocalisation" est exclu de la comparaison
  //
	// Le champ "visible", qui n'est pas à proprement parler un champ
	// de données est exclu de la comparaison
	//
	if ((_sType             == o._sType) &&
      (_sLexique          == o._sLexique) &&
      (_sComplement       == o._sComplement) &&
      (_sUnit             == o._sUnit) &&
      (_sCertitude        == o._sCertitude) &&
      (_sInteret          == o._sInteret) &&
      (_sPluriel          == o._sPluriel) // &&
      // (_sVisible          == o._sVisible)
       )
		return true ;
	else
		return false ;
}

void
NSPathoBaseData::setFixedSizeValue(string *pValue, size_t iSize, string sNewValue)
{
  if ((NULL == pValue) || (0 == iSize))
    return ;

  if (strlen(sNewValue.c_str()) <= iSize)
    *pValue = sNewValue ;
  else
  	*pValue = string(sNewValue, 0, iSize) ;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int
NSPathoBaseData::getIndice(const int indice) const
{
  if (string("") == _sCodeLocalisation)
    return 0 ;

  string sStringToConvert = string("") ;

  // Starting by '+': format is "+LLLLCCC"
  //
  if ('+' == _sCodeLocalisation[0])
  {
    if      (0 == indice)
      sStringToConvert = string(_sCodeLocalisation, 1, 4) ;
    else if (1 == indice)
      sStringToConvert = string(_sCodeLocalisation, 5, 3) ;
  }
  // Not starting by '+': format is "LLCCzztt"
  //
  else
  {
    if ((indice >= 0) && (indice < 4))
      sStringToConvert = string(_sCodeLocalisation, 2 * indice, 2) ;
  }

  if (string("") == sStringToConvert)
    return 0 ;

  int iValeur = 0 ;

  // base 62 = 26 + 26 + 10
  //
  for (size_t i = 0 ; i < strlen(sStringToConvert.c_str()) ; i++)
  {
    iValeur = 62 * iValeur ;
    char caract = sStringToConvert[i] ;

    if      ((caract >= '0') && (caract <= '9'))
      iValeur += caract - '0' ;
    else if ((caract >= 'A') && (caract <= 'Z'))
      iValeur += caract + 10 - 'A' ;
    else if ((caract >= 'a') && (caract <= 'z'))
      iValeur += caract + 36 - 'a' ;
  }

  return iValeur ;
}

//---------------------------------------------------------------------------
// Since the 5.06 version, the format is "+LLLLCCC"
//---------------------------------------------------------------------------
void
NSPathoBaseData::setIndice(int indice, int valeur)
{
  if ((indice < 0) || (indice > 3))
    return ;

  // format is "+LLLLCCC"
  //
  if (indice < 2)
  {
    if (string("") == _sCodeLocalisation)
      _sCodeLocalisation = string(BASE_LOCALISATION_LEN, '0') ;
    //
    // Old format - need to adapt
    //
    else if ('+' != _sCodeLocalisation[0])
    {
      int iOtherValue = 0 ;
      (0 == indice) ? iOtherValue = getColonne() : iOtherValue = getLigne() ;

      _sCodeLocalisation = string(BASE_LOCALISATION_LEN, '0') ;
      _sCodeLocalisation[0] = '+' ;

      (0 == indice) ? setColonne(iOtherValue) : setLigne(iOtherValue) ;
    }

    unsigned int iSize = 4 ;
    if (1 == indice)
      iSize = 3 ;

    // base 62 = 26 + 26 + 10
    //
    string sValue = string("") ;
    numacarbase(&sValue, (unsigned int) valeur, iSize, 62) ;

    if (string("") == sValue)
      return ;

    for (unsigned int i = 0 ; i < iSize ; i++)
      _sCodeLocalisation[(4*indice)+1+i] = sValue[i] ;

    return ;
  }

  // format is "LLCCzztt"
  //

  if (string("") == _sCodeLocalisation)
    _sCodeLocalisation = string(BASE_LOCALISATION_LEN, '0') ;

  // base 62 = 26 + 26 + 10
  //
  int iValeur[2] ;

	iValeur[0] = valeur / 62 ;

  if (iValeur[0] > 0)
    iValeur[1] = valeur - iValeur[0] * 62 ;
  else
    iValeur[1] = valeur ;

  for (int i = 1; i >= 0; i--)
  {
    if      ((iValeur[i] >= 0) && (iValeur[i] <= 9))
      _sCodeLocalisation[(2 * indice) + i] = char(iValeur[i] + '0') ;
    else if ((iValeur[i] >= 10) && (iValeur[i] <= 35))
      _sCodeLocalisation[(2 * indice) + i] = char(iValeur[i] - 10 + 'A') ;
    else if ((iValeur[i] >= 36) && (iValeur[i] <= 61))
      _sCodeLocalisation[(2 * indice) + i] = char(iValeur[i] - 36 + 'a') ;
  }
}

