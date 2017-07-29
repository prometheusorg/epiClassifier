//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------

#ifndef __NSCR_AMA_H
#define __NSCR_AMA_H

#include <vector>
#include "nautilus/nscr_amx.h"

//
// Définition de NSCRPhraseArray (Array de NSCRPhrase(s))
//

typedef vector<NSCRPhrase*> NSCRPhrArray;
typedef NSCRPhrArray::iterator NSCRPhrArrayIter;

class  NSCRPhraseArray : public NSCRPhrArray
{
public :

	// Constructeurs

	NSCRPhraseArray() : NSCRPhrArray() {}

	NSCRPhraseArray(NSCRPhraseArray& rv);

	// Destructeur

	virtual ~NSCRPhraseArray();

	// Opérateur =

	NSCRPhraseArray& operator=(NSCRPhraseArray src);

   void vider();
};

#endif // __NSCR_ANA_H
