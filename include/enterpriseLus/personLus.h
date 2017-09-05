// -----------------------------------------------------------------------------
// personLus.h
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2009/07/23 17:08:43 $
// -----------------------------------------------------------------------------
// Person object
// -----------------------------------------------------------------------------
// PK  - 01/1992
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Philippe Ameline, 2011
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant a [rappeler les
// caractéristiques techniques de votre logiciel].
//
// Ce logiciel est regi par la licence CeCILL soumise au droit francais et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffusee par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilite au code source et des droits de copie, de
// modification et de redistribution accordes par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limitee. Pour les memes raisons, seule une
// responsabilite restreinte pese sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les concedants successifs.
//
// A cet egard  l'attention de l'utilisateur est attiree sur les risques
// associes au chargement, a l'utilisation, a la modification et/ou au
// developpement et a la reproduction du logiciel par l'utilisateur etant donne
// sa specificite de logiciel libre, qui peut le rendre complexe a manipuler et
// qui le reserve donc a des developpeurs et des professionnels avertis
// possedant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invites a charger et tester l'adequation du logiciel a leurs besoins
// dans des conditions permettant d'assurer la securite de leurs systemes et ou
// de leurs donnees et, plus generalement, a l'utiliser et l'exploiter dans les
// memes conditions de securite.
//
// Le fait que vous puissiez acceder a cet en-tete signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accepte les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to [describe
// functionalities and technical features of your software].
//
// This software is governed by the CeCILL  license under French law and abiding
// by the rules of distribution of free software. You can use, modify and/ or
// redistribute the software under the terms of the CeCILL license as circulated
// by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy, modify
// and redistribute granted by the license, users are provided only with a
// limited warranty and the software's author, the holder of the economic
// rights, and the successive licensors have only limited liability.
//
// In this respect, the user's attention is drawn to the risks associated with
// loading, using, modifying and/or developing or reproducing the software by
// the user in light of its specific status of free software, that may mean that
// it is complicated to manipulate, and that also therefore means that it is
// reserved for developers and experienced professionals having in-depth
// computer knowledge. Users are therefore encouraged to load and test the
// software's suitability as regards their requirements in conditions enabling
// the security of their systems and/or data to be ensured and, more generally,
// to use and operate it in the same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.
// -----------------------------------------------------------------------------

#ifndef __PERSONLUS_H
#define __PERSONLUS_H

class NSPersonGraphManager ;
class NSPatPathoArray ;
class NSBasicAttributeArray ;
class NSHealthTeam ;
class NSHealthTeamDocument ;
class NSSearchStruct ;
class NSContexte ;
class NSHISTODocument ;
class NSDocumentHisto ;
class NSFrameDocumentsArray ;
// class NSChemiseArray ;

// # include <cstring.h>

# include "../partage/nsvector.h"
# include "../nautilus/nsanxary.h"
# include "../nsbb/nspatpat.h"

const char LocalPatient  = '-' ;
const char INMEMORY_CHAR = '#' ;

# define PAT_NSS_LEN         PIDS_NSS_LEN

// -----------------------------------------------------------------------------
//
//                                Person
//
// -----------------------------------------------------------------------------
// A person...
// -----------------------------------------------------------------------------

class Person
{
  protected:

	  // Information storage strings
    //
    string _sID ;
    string _sPseudo ;
    string _sFamilyName ;
    string _sFirstName ;
    string _sCode ;
    string _sMaidenName ;
    string _sGender ;
    string _sAddress ;
    string _sBirthDate ;
    string _sMobilePhoneNumber ;
    string _sOfficePhoneNumber ;
    string _sFamilyStatus ;
    string _sLang ;
    string _sCivility ;
    string _sFullName ;

    string _sProJob ;
    string _sProSpeciality ;
    string _sProMajor ;
		string _sProActivity ;
    string _sProTitle ;
    string _sProCivility ;
    string _sProRoles ;
    string _sProAddress ;
    string _sProCity ;

    NSContexte*           _pContexte ;

    string                _sContribution ;

  public:

	  Person(NSContexte* pCtx) ;
	  Person(Person& rv) ;

    ~Person() ;

    void    init() ;

	  Person& operator=(Person src) ;
	  int     operator==(Person& o) ;

    void    buildFullName(string sLang = "") ;
    string  buildCivility(bool bShort, string sLang = "") ;
    int     donneNaissance(char *dateNaiss) ;

    // Indique si le patient est local
    inline bool IsLocal()    { return (LocalPatient  == _sID[0]) ; }
    // Indique si le patient est en memoire
    inline bool IsInMemory() { return (INMEMORY_CHAR == _sID[0]) ; }
    // Indique si le patient est collectif
    inline bool IsGlobal()   { return (!IsInMemory() && !IsLocal()) ; }

    bool    isMale()    { return (string("HMASC1") == _sGender) ; }
    bool 		isFemale()  { return (string("HFEMI1") == _sGender) ; }
    void 		setMale()   { _sGender = string("HMASC1") ; }
    void 		setFemale() { _sGender = string("HFEMI1") ; }

    // donnees "traduites" en français
    string  donneSitFam(string sLang = "") ;
    string 	getTranslatedBirthDate(string sLang = "") ;
    string 	getTitle(string sLang = "") ;

    bool    initGraphs() ;

    void    debloquer() ;

    bool    getReadOnly() ;
    bool    getADebloquer() ;
    void    setReadOnly(bool bRO) ;
    void    setADebloquer(bool bAD) ;

    void    GetDocPatho(NSPatPathoArray *pPatho, string sRootSens) ;
    void    DonnePathoSynthese(NSPatPathoArray *pPathoSynthese) { GetDocPatho(pPathoSynthese, string("ZSYNT")) ; } /* Fills a patpaho with synthese */

    // bool    DonneArray(string sNoeud, NSPatPathoArray *pPPT) ;
    // bool    DonneSousArray(string sNoeud, NSPatPathoArray *pPPT) ;

    bool    initCorrespArray(NSPatPathoArray* pPatPathoArray = 0) ;

    bool    getFoldersPatho(NSPatPathoArray* pPatPathoArray) ;
    // bool    getFoldersArray(NSChemiseArray* pChemisesArray) ;

    string 	fabriqueAgeLabel(string sLang = "") ;
    string 	fabriqueAgeLabelYears(string sLang = "") ;
    string 	fabriqueAgeLabelMonths(string sLang = "") ;
    string 	fabriqueAgeLabelDays(string sLang = "") ;
    string 	fabriqueAgeLabelHours(string sLang = "") ;

    NSContexte*           getContext()     { return _pContexte ; }

    string  donneLang()      	 { return _sLang ; }
    string  getPseudo()        { return _sPseudo ; }
    string  getId()            { return _sID ; }
    string  getFamilyName()    { return _sFamilyName ; }
    string  getFirstName()     { return _sFirstName ; }
    string  getCode()          { return _sCode ; }
    string  getGender()        { return _sGender ; }
    string  getAddress()       { return _sAddress ; }
    string  getBirthDate()     { return _sBirthDate ; }
    string  getMobilePhone()   { return _sMobilePhoneNumber ; }
    string  getOfficePhone()   { return _sOfficePhoneNumber ; }
    string  getFamilyStatus()  { return _sFamilyStatus ; }
    string	getCivility()			 { return _sCivility ; }
    string  getFullName()      { return _sFullName ; }
    string	getMaidenName()    { return _sMaidenName ; }

    string  getProJob()        { return _sProJob ; }
    string  getProSpeciality() { return _sProSpeciality ; }
    string  getProMajor()      { return _sProMajor ; }
		string  getProActivity()   { return _sProActivity ; }
    string  getProTitle()      { return _sProTitle ; }
    string  getProCivility()   { return _sProCivility ; }
    string  getProRoles()      { return _sProRoles ; }
    string  getProAddress()    { return _sProAddress ; }
    string  getProCity()       { return _sProCity ; }

    void setId(string sVal)           { _sID                = sVal ; }
    void setPseudo(string sVal)       { _sPseudo            = sVal ; }
    void setFamilyName(string sVal)   { _sFamilyName        = sVal ; }
    void setFirstName(string sVal)    { _sFirstName         = sVal ; }
    void setCode(string sVal)         { _sCode              = sVal ; }
    void setMaidenName(string sVal)   { _sMaidenName        = sVal ; }
    void setGender(string sVal)       { _sGender            = sVal ; }
    void setAddress(string sVal)      { _sAddress           = sVal ; }
    void setBirthDate(string sVal)    { _sBirthDate         = sVal ; }
    void setMobilePhone(string sVal)  { _sMobilePhoneNumber = sVal ; }
    void setOfficePhone(string sVal)  { _sOfficePhoneNumber = sVal ; }
    void setFamilyStatus(string sVal) { _sFamilyStatus      = sVal ; }
    void setLang(string sVal)         { _sLang              = sVal ; }
    void setCivility(string sVal)     { _sCivility          = sVal ; }
    void setFullName(string sVal)     { _sFullName          = sVal ; }

    bool LookForPath(string sChemin, NSSearchStruct *pSearchStruct, bool reinit_structure = false) ;

    NSDocumentHisto* GetDocument(string sRootSens) ;

    bool             getArray(string sNode, NSPatPathoArray *pPPT) ;
    bool             getSubArray(string sNode, NSPatPathoArray *pPPT) ;
    bool             getFoldersArray(NSChemiseArray* pChemisesArray) ;

    bool             CreeContribution(bool bInitiale) ;
    bool             FermeContribution() ;
    string           getContribution() { return _sContribution ; }
} ;

#endif // !__PERSONLUS_H

