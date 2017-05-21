//----------------------------------------------------------------------------
// nscr_amx.h : Phrases au sein de l'editeur de comptes rendus
//              Sentences inside the report generator editor
//----------------------------------------------------------------------------
#ifndef __NSCR_AMX_H
#define __NSCR_AMX_H

#include "nautilus/nsannexe.h"
// #include "nautilus/nstxstyl.h"
#include "partage/NTArray.h"

/*class NSCRPhraLes
{
	public:

		string sDocument;
		UINT   iNumeroLesion;
		char	 cType, cTypeIndex;

		NSCRPhraLes();
		NSCRPhraLes(NSCRPhraLes& rv);
		~NSCRPhraLes();
		NSCRPhraLes& operator=(NSCRPhraLes src);
		int operator == (const NSCRPhraLes& o);
		int operator != (const NSCRPhraLes& o) {return !(*this==o);}
}; */

//
// Définition de NSCRPhraLesArray (Array de NSCRPhraLes)
//
//typedef TArrayAsVector<NSCRPhraLes> NSCRPhrLsArray;
typedef vector</*NSCRPhraLes**/string*> NSCRPhrLsArray;
typedef NSCRPhrLsArray::iterator NSCRPhrLsArrayIter;
typedef NTArray<string> NSCRPhraLesArray;

/*
class NSCRPhraLesArray : public NSCRPhrLsArray
{
    public :

	    // Constructeurs
	    NSCRPhraLesArray() : NSCRPhrLsArray() {}
	NSCRPhraLesArray(NSCRPhraLesArray& rv);
	// Destructeur
	virtual ~NSCRPhraLesArray();
	NSCRPhraLesArray& operator=(NSCRPhraLesArray src);
   void vider();
}; */

//
// Phrases brutes issues du décodeur et liaison avec une ou plusieurs NSLesion
//
class NSCRPhrase
{
	public:

    unsigned int iNumero ;
    unsigned int iIndentation ;
    string       sTexte ;

    NSCRPhraLesArray aLesions ;
    NSCRPhraLesArray aChemins ;

    NSCRPhrase() ;
    NSCRPhrase(NSCRPhrase& rv) ;
    ~NSCRPhrase() ;
    NSCRPhrase& operator=(NSCRPhrase src) ;
    int operator == (const NSCRPhrase& o) ;
    int operator != (const NSCRPhrase& o) { return !(*this==o) ; }
};

#endif // __NSCR_AMX_H
