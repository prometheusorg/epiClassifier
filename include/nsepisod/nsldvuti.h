//----------------------------------------------------------------------------
//   Programme protege par une licence Open Source GPL
//   Philippe Ameline
//----------------------------------------------------------------------------
#ifndef __NSLDVUTI_H
#define __NSLDVUTI_H

#ifndef __linux__
#if defined(_EPISODUSDLL)
	#define _EPISODUS __export
#else
	#define _EPISODUS __import
#endif
#endif

#ifndef __linux__
// #include <classlib/time.h>
#include <owl/window.h>
#include <WinSys/geometry.h>
#include <WinSys/color.h>
#include <classlib/time.h>
#else
#include "classlib/time.h"
#endif

#define DATETIMESTRLEN 14
#define DATESTRLEN      8

/**
* Classe de gestion du temps dans tout le logiciel
* Time management class (to be used everywhere)
*/
#ifndef __linux__
class _EPISODUS NVLdVTemps
#else
class NVLdVTemps
#endif
{
	protected:

  	// AAAAMMJJhhmmss
    string  sTemps ;

	public:

  	NVLdVTemps() ;
    NVLdVTemps(int iA, int iM, int iJ, int ih, int im, int is) ;
    NVLdVTemps(NVLdVTemps& rv) ;
    ~NVLdVTemps() ;

    void    init()       { sTemps = string(DATETIMESTRLEN, '0') ; }
    void    setNoLimit() { sTemps = string("9999") + string(DATETIMESTRLEN - 4, '0') ; }
    void    takeTime() ;

    bool    initFromDate(string sDate) ;
    bool    initFromDateHeure(string sDateHeure) ;
#ifndef __linux__
    bool    initFromSystemTime(LPSYSTEMTIME lpSystemTime) ;
#endif
    void		normalize() ;

    bool    estVide()    { return ((string(DATETIMESTRLEN, '0') == sTemps) ||
                                                    (string("") == sTemps)) ; }
    bool    estNoLimit() { return ( string("9999") + string(DATETIMESTRLEN - 4, '0') == sTemps) ; }

    int     donneAns() ;
    int     donneMois() ;
    int     donneJours() ;
    int     donneHeures() ;
    int     donneMinutes() ;
    int     donneSecondes() ;

    string  donneDateHeure() { return sTemps ; }
    string  donneDate()      { return string(sTemps, 0, 8) ; }
    string  donneHeure()     { return string(sTemps, 8, 6) ; }
    string  donneFormattedDateHeure(NSContexte* pContexte, string sLang) ;
    string  donneFormattedDate(NSContexte* pContexte, string sLang) ;
    string  donneFormattedHeure(NSContexte* pContexte, string sLang) ;

    void    placeAns(int iA) ;
    void    placeMois(int iM) ;
    void    placeJours(int iJ) ;
    void    placeHeures(int ih) ;
    void    placeMinutes(int im) ;
    void    placeSecondes(int is) ;

    // Fonctions de gestion du temps
    void    ajouteAns(int iDeltaT, bool bAdjust = true) ;
    void    ajouteMois(int iDeltaT, bool bAdjust = true) ;
    void    ajouteJours(int iDeltaT) ;
    void    ajouteHeures(long iDeltaT) ;
    void    ajouteMinutes(long iDeltaT) ;
    void    ajouteSecondes(long iDeltaT) ;

    void    ajouteTemps(int iDeltaT, string sUnite, NSContexte *pCtx) ;
    unsigned long yearsBetween(NVLdVTemps otherDate) ;
    unsigned long monthsBetween(NVLdVTemps otherDate) ;
    unsigned long daysBetween(NVLdVTemps otherDate) ;
    unsigned long hoursBetween(NVLdVTemps otherDate) ;
    unsigned long minutesBetween(NVLdVTemps otherDate) ;
    unsigned long secondsBetween(NVLdVTemps otherDate) ;

    struct tm   *convert2tm() ;
    void    convert(struct tm *tm2Convert) ;
    void    convert(ClassLib::TTime tt2Convert) ;
    void    convert(ClassLib::TDate *tt2Convert) ;

    string  donneMonthName() ;

    NVLdVTemps& operator=(NVLdVTemps src) ;

    bool operator==(NVLdVTemps src) { return sTemps == src.sTemps ; }
    bool operator>(NVLdVTemps src)  { return sTemps >  src.sTemps ; }
    bool operator>=(NVLdVTemps src) { return sTemps >= src.sTemps ; }
    bool operator<(NVLdVTemps src)  { return sTemps <  src.sTemps ; }
    bool operator<=(NVLdVTemps src) { return sTemps <= src.sTemps ; }
};

// definition d'un array de pixels

typedef vector<NVLdVTemps*> TimeVector ;
typedef TimeVector::iterator TimeIter ;

#ifndef __linux__
class _EPISODUS TimeArray : public TimeVector
#else
class TimeArray : public TimeVector
#endif
{
	public:

  	// Constructeurs - Destructeur
    TimeArray() ;
    TimeArray(TimeArray& rv) ;
    ~TimeArray() ;

    void sortDates(bool bRecentOnTop) ;

    void vider() ;

    TimeArray& operator=(TimeArray src) ;
};

#ifndef _ENTERPRISE_DLL

class NSLdvView ;
class NSLdvViewArea ;

#ifndef __linux__
class _EPISODUS NVLdVPoint
#else
class NVLdVPoint
#endif
{
	protected:

  	NVLdVTemps  tX ;
    long        dY ;

    NSLdvView*  pView ;

	public:

  	NVLdVPoint(NSLdvView* pTheView) ;
    NVLdVPoint(NVLdVPoint& rv) ;
    ~NVLdVPoint();

    void initialise(NS_CLASSLIB::TPoint* pOrigine, NS_CLASSLIB::TPoint* pPt, long lWorkingAreaPosInfDroit) ;

    NVLdVPoint& operator=(NVLdVPoint src) ;                                                          

    NVLdVTemps  getX()  { return tX ; }
    long        getY()  { return dY ; }

    void        setX(NVLdVTemps tTemps) ;
    void        setY(long lHaut)        { dY = lHaut ; }
};

#ifndef __linux__
class _EPISODUS NVLdVRect
#else
class NVLdVRect
#endif
{
	protected:

  	NVLdVTemps  tLeft ;
    long        lTop ;
    NVLdVTemps  tRight ;
    long        lBottom ;

    NSLdvView*  pView ;

	public:

  	NVLdVRect(NSLdvView* pTheView) ;
    NVLdVRect(NSLdvView* pTheView, NVLdVTemps tDeb, NVLdVTemps tFin,
                                            long lPlancher, long lPlafond) ;
    NVLdVRect(NSLdvView* pTheView, NVLdVPoint* upLeft, NVLdVPoint* loRight) ;
    NVLdVRect(NVLdVRect& rv) ;
    ~NVLdVRect() ;

    void initialise(NS_CLASSLIB::TPoint* pOrigine, NS_CLASSLIB::TRect* pRect, long lWorkingAreaPosInfDroit) ;
    void initialise(NVLdVTemps tDeb, NVLdVTemps tFin,
                                            long lPlancher, long lPlafond) ;

    NVLdVRect& operator=(NVLdVRect src) ;

    void setLeft(NVLdVTemps tps)    { tLeft   = tps ; }
    void setRight(NVLdVTemps tps)   { tRight  = tps ; }
    void setTop(long lv)            { lTop    = lv ; }
    void setBottom(long lv)         { lBottom = lv ; }

    NVLdVTemps getLeft()    { return tLeft ;    }
    NVLdVTemps getRight()   { return tRight ;   }
    long       getTop()     { return lTop ;     }
    long       getBottom()  { return lBottom ;  }

    NVLdVPoint BottomLeft() ;
    NVLdVPoint BottomRight() ;
    NVLdVPoint TopLeft() ;
    NVLdVPoint TopRight() ;

    unsigned long Width() ;     // Width in pixels

    long       Height()     { return lBottom - lTop ; }

    bool Contains(NVLdVPoint point) ;  // Contient other
    bool Contains(NVLdVRect other) ;
    bool Touches(NVLdVRect other) ;    // A une intersection avec other
};

#ifndef _EXT_CAPTURE

#ifndef __linux__
class _EPISODUS NSPixel
{
	protected:

  	NS_CLASSLIB::TPoint point ;
    NS_CLASSLIB::TColor color ;

	public:

  	NSPixel(NS_CLASSLIB::TPoint Point, NS_CLASSLIB::TColor Color) ;
    NSPixel(int X, int Y, NS_CLASSLIB::TColor Color) ;
    NSPixel(NSPixel& rv) ;
    ~NSPixel() ;

    NSPixel& operator=(NSPixel src) ;

    NS_CLASSLIB::TPoint getPoint()  { return point ; }
    int                 X()         { return point.X() ; }
    int                 Y()         { return point.Y() ; }
    NS_CLASSLIB::TColor getColor()  { return color ; }

    char*               getTuple() ;
};

// definition d'un array de pixels

typedef vector<NSPixel*> PixArray ;
typedef PixArray::iterator pixelIter ;

class _EPISODUS PixelArray : public PixArray
{
	public:

  	// Constructeurs - Destructeur
    PixelArray() ;
    PixelArray(PixelArray& rv) ;
    ~PixelArray() ;

    NS_CLASSLIB::TRect ArrayRect ;

    bool ScreenCapture(TWindowDC* pWinDC, ClassLib::TRect* pSrcRect) ;

    bool Affiche(TDC* pDestDC, NS_CLASSLIB::TRect* pDstRect, NS_CLASSLIB::TPoint* pSrcPt) ;

    void ajouter(NSPixel* pNewPix) ;

    NS_CLASSLIB::TColor getColor(NS_CLASSLIB::TPoint pt, bool* pExist) ;

    int getBottom() { return ArrayRect.Bottom() ; }
    int getTop()    { return ArrayRect.Top() ;    }
    int getLeft()   { return ArrayRect.Left() ;   }
    int getRight()  { return ArrayRect.Right() ;  }

    void vider() ;
		PixelArray& operator=(PixelArray src) ;
};

class NSOCRbloc;

class _EPISODUS NSBitmap
{
	public:

  	// Constructeurs - Destructeur
    NSBitmap() ;
    NSBitmap(NSBitmap& rv) ;
    ~NSBitmap() ;

    bool ScreenCapture(TWindowDC* pWinDC, ClassLib::TRect* pSrcRect) ;
    bool Affiche(TDC* pDestDC, NS_CLASSLIB::TRect* pDstRect, NS_CLASSLIB::TPoint* pSrcPt, int iZoomFactor = 1) ;

    bool trouveBloc(NSOCRbloc* pBloc, NS_CLASSLIB::TPoint* pPoint) ;

    bool enableCapture(ClassLib::TRect* pRect, NS_CLASSLIB::TColor* pBgColor) ;
    void nearBlank(ClassLib::TRect* pRect, NS_CLASSLIB::TColor* pBgColor, bool *pLeft, bool *pTop, bool *pRight, bool *pBottom) ;
    void existEdges(ClassLib::TRect* pRect, NS_CLASSLIB::TColor* pBgColor, bool *pLeft, bool *pTop, bool *pRight, bool *pBottom) ;

    NS_CLASSLIB::TColor getColor(int X, int Y, bool* pExist) ;
    NS_CLASSLIB::TColor getColor(NS_CLASSLIB::TPoint pt, bool* pExist) ;

    int getBottom() { return pBitmap->Height() ; }
    int getTop()    { return 0 ; }
    int getLeft()   { return 0 ; }
    int getRight()  { return pBitmap->Width() ; }

    TDib* getDib()  { return pBitmap ; }

    NSBitmap& operator=(NSBitmap src) ;

	protected:

  	TDib* pBitmap ;
};

class _EPISODUS NSOCRbloc
{
	public:

  	NS_CLASSLIB::TRect  BlocRect ;
    NS_CLASSLIB::TRect  BlocOCR ;
    string              sTexte ;
    NS_CLASSLIB::TColor bgColor ;
    NS_CLASSLIB::TColor fgColor ;

    NSOCRbloc(NS_CLASSLIB::TRect Bloc) ;
    NSOCRbloc(NS_CLASSLIB::TRect Bloc,  NS_CLASSLIB::TColor bColor,
            NS_CLASSLIB::TColor fColor = NS_CLASSLIB::TColor::Black) ;
    NSOCRbloc(NSOCRbloc& rv) ;
    ~NSOCRbloc() ;

    NSOCRbloc& operator=(NSOCRbloc src) ;
};

// definition d'un array de modeles
typedef vector<NSOCRbloc*> OCRArray;
typedef OCRArray::iterator blocIter;

class _EPISODUS OCRBlocsArray : public OCRArray
{
	public:

  	// Constructeurs - Destructeur
    OCRBlocsArray() : OCRArray() {}
    OCRBlocsArray(OCRBlocsArray& rv) ;
    ~OCRBlocsArray() ;

    void    vider() ;
    OCRBlocsArray& operator=(OCRBlocsArray src) ;
};

#endif

#endif

#endif

#endif // __NSLDVUTI_H

