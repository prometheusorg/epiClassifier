//----------------------------------------------------------------------------
//   Programme protege par une licence Open Source GPL
//   Philippe Ameline
//----------------------------------------------------------------------------
#ifndef __NSLDVUTI_H
#define __NSLDVUTI_H

#include "classlib/time.h"

#define DATETIMESTRLEN 14
#define DATESTRLEN      8

/**
* Classe de gestion du temps dans tout le logiciel
* Time management class (to be used everywhere)
*/
class NVLdVTemps

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

class TimeArray : public TimeVector

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

class NVLdVPoint

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

class NVLdVRect

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

#endif

#endif // __NSLDVUTI_H

