//----------------------------------------------------------------------------
// Objets de localisation
//
// PA Janvier 2004
//----------------------------------------------------------------------------

#ifndef __NSLOCALISE_H
#define __NSLOCALISE_H

class NSSuper ;

#include <vector>
#include <map>
#include <string>

using std::string;
using std::ifstream;

#ifdef _ENTERPRISE_DLL
  # if defined(_DANSPERSONDLL)
  #  define _CLASSELEXI __export
  # else
  #  define _CLASSELEXI __import
  # endif
#else
  # if defined(_DANSLEXIDLL)
  #  define _CLASSELEXI __export
  # else
  #  define _CLASSELEXI __import
  # endif
#endif

typedef  std::map<std::string, std::string>::iterator  NSLocalisedChapterIter;

//
// Class décrivant et stockant un chapitre
// Un chapitre est une description de variable de texte
///
#ifndef __linux__
class _CLASSELEXI NSLocalisedChapter
#else
class NSLocalisedChapter
#endif
{
  public :
  	// Constructeur par défault
		NSLocalisedChapter() {}

    // Constructeur par copie
    NSLocalisedChapter(NSLocalisedChapter& rv) ;

    // Opérateur affectation
    NSLocalisedChapter& operator=(NSLocalisedChapter& src) ;

    //destructeur
	   ~NSLocalisedChapter() ;

     // Ajoute une variable instancié au chapitre
     void   			addText(std::string sCod, std::string sTxt) ;

     // retourne la valeur d'une variable
     std::string  getLocalText(std::string sCod) ;

     bool empty () {return aLocalTexts.empty() ; }

     std::map<std::string, std::string>* getLocalTexts() { return &aLocalTexts ; }

  private:
  	std::map<std::string, std::string> aLocalTexts ;
};

typedef std::map<std::string, NSLocalisedChapter* >::iterator NSLocalChapterArrayIter ;

#ifndef __linux__
class _CLASSELEXI NSLocalChapterArray
#else
class NSLocalChapterArray
#endif
{
  public :

    // Constructeurs
    NSLocalChapterArray() ;
    NSLocalChapterArray(NSLocalChapterArray& rv) ;
    // Opérateur =
    NSLocalChapterArray& operator=(NSLocalChapterArray& src) ;
    // Destructeur
    ~NSLocalChapterArray() ;

    void   addText(string sChap, string sCod, string sTxt) ;
    string getLocalText(string sChap, string sCod, bool bSendTextBackIfNotFound = false) ;

    NSLocalisedChapter* getChapter(string sChapterTitle) ;
    NSLocalisedChapter* getChapterWhereText(string sText, string sValue) ;

    void   init(ifstream* pInFile, string *psErrorMsg) ;
    void   init(string sFileName, string *psErrorMsg) ;
    bool   writeToFile(string sFileName, NSSuper *pSuper) ;

    bool   empty () {return _chapters.empty() ; }

    void   vider() ;

    NSLocalChapterArrayIter begin() { return _chapters.begin() ; }
    NSLocalChapterArrayIter end()   { return _chapters.end() ; }

  private:

    std::map<std::string, NSLocalisedChapter* > _chapters ;
};

#endif // __NSLOCALISE_H

