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

#endif

