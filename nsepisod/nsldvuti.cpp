//----------------------------------------------------------------------------
//                  Ligne de vie - Vue du modèle Document/Vue
//----------------------------------------------------------------------------
#include <cstdio>
#include <cstring>
#include <algorithm>
#ifndef _ENTERPRISE_DLL
  #include <owl\pch.h>
#endif

/*
#include "classlib/date.h"
#include "classlib/time.h"
*/

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
#else
  #include "nautilus\nssuper.h"
  #include "partage\nsdivfct.h"
  #include "nsdn\nsdochis.h"
  #include "nautilus\nsldvvue.h"
#endif

#include "nsepisod/nsldvuti.h"
#include "nssavoir/nsfilgd.h"

#include "prtime.h"

#ifndef _ENTERPRISE_DLL
// --------------------------------------------------------------------------
// ------------------------- METHODES DE NVLdVPoint -------------------------
// --------------------------------------------------------------------------

NVLdVPoint::NVLdVPoint(NSLdvView* pTheView)
{
	tX.init() ;
	dY = 0 ;

	pView = pTheView ;
}

NVLdVPoint::NVLdVPoint(NVLdVPoint& rv)
{
	tX    = rv.tX ;
	dY    = rv.dY ;

	pView = rv.pView ;
}

NVLdVPoint::~NVLdVPoint()
{
}

void
NVLdVPoint::initialise(NS_CLASSLIB::TPoint* pOrigine, NS_CLASSLIB::TPoint* pPt, long lWorkingAreaPosInfDroit)
{
	tX = pView->getDateHeureInfDroit() ;
	//
	// Calcul de la différence en pixels sur l'axe des X (temps)
	//
	int iDelta = pView->getTimeUnitFromPhysicalWidth(pPt->x - pOrigine->x) ;
	//
	// Conversion en AAAAMMJJhhmmss
	//
	switch (pView->getXunit())
	{
		case pixAns :
			tX.ajouteAns(iDelta) ;
			break ;
		case pixMois :
			tX.ajouteMois(iDelta) ;
			break ;
		case pixSemaine :
			tX.ajouteJours(iDelta * 7) ;
			break ;
		case pixJour :
			tX.ajouteJours(iDelta) ;
			break ;
		case pixHeure :
			tX.ajouteHeures(iDelta) ;
			break ;
		case pixMinute :
			tX.ajouteMinutes(iDelta) ;
			break ;
		case pixSeconde :
			tX.ajouteSecondes(iDelta) ;
			break ;
	}
	//
	// Calcul de la différence en pixels sur l'axe des Y
	//
	dY = lWorkingAreaPosInfDroit + long(pOrigine->y - pPt->y) ;
}

void
NVLdVPoint::setX(NVLdVTemps tTemps)
{
	tX = tTemps ;
	tX.normalize() ;
}

NVLdVPoint&
NVLdVPoint::operator=(NVLdVPoint src)
{
	if (this == &src)
		return *this ;

  tX    = src.tX ;
  dY    = src.dY ;

  pView = src.pView ;

  return *this ;
}

// --------------------------------------------------------------------------
// ------------------------- METHODES DE NVLdVRect --------------------------
// --------------------------------------------------------------------------

NVLdVRect::NVLdVRect(NSLdvView* pTheView)
{
  lTop    = 0 ;
  lBottom = 0 ;

  pView   = pTheView ;
}

NVLdVRect::NVLdVRect(NSLdvView* pTheView, NVLdVTemps tDeb, NVLdVTemps tFin,
                                                long lPlancher, long lPlafond)
{
  pView = pTheView ;

  initialise(tDeb, tFin, lPlancher, lPlafond) ;
}

NVLdVRect::NVLdVRect(NSLdvView* pTheView, NVLdVPoint* upLeft, NVLdVPoint* loRight)
{
  pView = pTheView ;

  initialise(upLeft->getX(), loRight->getX(), loRight->getY(), upLeft->getY()) ;
}

NVLdVRect::NVLdVRect(NVLdVRect& rv)
{
  tLeft   = rv.tLeft ;
  tRight  = rv.tRight ;
  lTop    = rv.lTop ;
  lBottom = rv.lBottom ;

  pView   = rv.pView ;
}

NVLdVRect::~NVLdVRect()
{
}

void
NVLdVRect::initialise(NS_CLASSLIB::TPoint* pOrigine, NS_CLASSLIB::TRect* pRect, long lWorkingAreaPosInfDroit)
{
  tLeft   = pView->getDateHeureInfDroit() ;
  tRight  = pView->getDateHeureInfDroit() ;

  lTop    = lWorkingAreaPosInfDroit ;
  lBottom = lWorkingAreaPosInfDroit ;

  // Calcul de la différence en pixels sur l'axe des X (temps)
  //
  int iDeltaLeft  = pView->getTimeUnitFromPhysicalWidth(pRect->left  - pOrigine->x) ;
  int iDeltaRight = pView->getTimeUnitFromPhysicalWidth(pRect->right - pOrigine->x) ;

  // Conversion en AAAAMMJJhhmmss
  //
  switch (pView->getXunit())
  {
    case pixAns :
        tLeft.ajouteAns(iDeltaLeft) ;
        tRight.ajouteAns(iDeltaRight) ;
        break ;
    case pixMois :
        tLeft.ajouteMois(iDeltaLeft) ;
        tRight.ajouteMois(iDeltaRight) ;
        break ;
    case pixSemaine :
        tLeft.ajouteJours(iDeltaLeft * 7) ;
        tRight.ajouteJours(iDeltaRight * 7) ;
        break ;
    case pixJour :
        tLeft.ajouteJours(iDeltaLeft) ;
        tRight.ajouteJours(iDeltaRight) ;
        break ;
    case pixHeure :
        tLeft.ajouteHeures(iDeltaLeft) ;
        tRight.ajouteHeures(iDeltaRight) ;
        break ;
    case pixMinute :
        tLeft.ajouteMinutes(iDeltaLeft) ;
        tRight.ajouteMinutes(iDeltaRight) ;
        break ;
    case pixSeconde :
        tLeft.ajouteSecondes(iDeltaLeft) ;
        tRight.ajouteSecondes(iDeltaRight) ;
        break ;
  }

  // Calcul de la différence en pixels sur l'axe des Y
  //
  int iDeltaTop    = pOrigine->y - pRect->top ;
  int iDeltaBottom = pOrigine->y - pRect->bottom ;

  lTop    += long(iDeltaTop) ;
  lBottom += long(iDeltaBottom) ;
}

void
NVLdVRect::initialise(NVLdVTemps tDeb, NVLdVTemps tFin,
                                                long lPlancher, long lPlafond)
{
	tLeft   = tDeb ;
  tLeft.normalize() ;
	tRight  = tFin ;
	tRight.normalize() ;
	lTop    = lPlafond ;
	lBottom = lPlancher ;
}

NVLdVPoint
NVLdVRect::BottomLeft()
{
	NVLdVPoint point(pView) ;
	point.setX(tLeft) ;
	point.setY(lBottom) ;
	return point ;
}

NVLdVPoint
NVLdVRect::BottomRight()
{
	NVLdVPoint point(pView) ;
	point.setX(tRight) ;
	point.setY(lBottom) ;
	return point ;
}

NVLdVPoint
NVLdVRect::TopLeft()
{
  NVLdVPoint point(pView) ;
  point.setX(tLeft) ;
  point.setY(lTop) ;
  return point ;
}

NVLdVPoint
NVLdVRect::TopRight()
{
  NVLdVPoint point(pView) ;
  point.setX(tRight) ;
  point.setY(lTop) ;
  return point ;
}

//
// Width (pixels)
//
unsigned long
NVLdVRect::Width()
{
	//
	// Differences pour chaque colonne.
	// Differences for each column.
	//
	int iDeltaAns  = tRight.donneAns()  - tLeft.donneAns() ;
	int iDeltaMois = tRight.donneMois() - tLeft.donneMois() ;

	if (pView->getXunit() == pixAns)
	{
  	int iDelta = iDeltaAns ;
    if      (iDeltaMois > 6)
    	iDelta++ ;
    else if (iDeltaMois < -6)
    	iDelta-- ;
    return long(iDelta) ;
	}

	int iDeltaJours = tRight.donneJours() - tLeft.donneJours() ;

	if (pView->getXunit() == pixMois)
	{
  	int iDelta = iDeltaAns * 12 + iDeltaMois ;
    if      (iDeltaJours > 15)
    	iDelta++ ;
    else if (iDeltaJours < -15)
    	iDelta-- ;
    return long(iDelta) ;
	}

	//
	// Précision au moins en jours
	//
	unsigned long lDeltaJ =
        TDate::Jday(tRight.donneMois(), tRight.donneJours(), tRight.donneAns())
        - TDate::Jday(tLeft.donneMois(), tLeft.donneJours(), tLeft.donneAns()) ;

	if (pView->getXunit() == pixSemaine)
	{
  	double dDelta = double(lDeltaJ) / double(7) ;
    double dFloor = floor(dDelta) ;
    if ((dDelta - dFloor) > double(0.5))
    	dFloor++ ;
    return long(dFloor) ;
	}

	int iDeltaHeures = tRight.donneHeures() - tLeft.donneHeures();

	if (pView->getXunit() == pixJour)
	{
  	if (iDeltaHeures > 12)
    	return(lDeltaJ++) ;
    if (iDeltaJours < -12)
    	return(lDeltaJ--) ;
    return(lDeltaJ) ;
	}

	int iDeltaMinutes = tRight.donneMinutes() - tLeft.donneMinutes() ;

	unsigned long lDeltaH = (lDeltaJ * 24) + iDeltaHeures ;

	if (pView->getXunit() == pixHeure)
	{
  	if (iDeltaMinutes > 30)
    	return(lDeltaH++) ;
    if (iDeltaJours < -30)
    	return(lDeltaH--) ;
    return(lDeltaH) ;
	}

	int iDeltaSecondes = tRight.donneSecondes() - tLeft.donneSecondes() ;

	unsigned long lDeltaM = (lDeltaH * 60) + iDeltaMinutes ;

	if (pView->getXunit() == pixMinute)
	{
  	if (iDeltaSecondes > 30)
    	return(lDeltaM++) ;
    if (iDeltaSecondes < -30)
    	return(lDeltaM--) ;
    return(lDeltaM) ;
	}

	unsigned long lDeltaS = (lDeltaM * 60) + iDeltaSecondes ;

	if (pView->getXunit() == pixSeconde)
  	return(lDeltaS) ;

	return 0 ;
}

//
// Return true if the rectangle contains the point.
// Renvoie true si le rectangle contient le point.
//
bool
NVLdVRect::Contains(NVLdVPoint point)
{
	return ((point.getX() >= tLeft) && (point.getX() < tRight) &&
            (point.getY() <= lTop) && (point.getY() > lBottom)) ;
}

//
// Return true if the rectangle complete surrounds the other.
// Renvoie true si le rectangle englobe l'autre.
//
bool
NVLdVRect::Contains(NVLdVRect other)
{
	return ((other.tLeft >= tLeft) && (other.tRight <= tRight) &&
            (other.lTop <= lTop)   && (other.lBottom >= lBottom)) ;
}

//
// Return true if the intersection of the two rectangles is not null.
// Renvoir true si l'intersection des deux rectangles n'est pas nulle.
//
bool
NVLdVRect::Touches(NVLdVRect other)
{
	return ((other.tRight >= tLeft) && (other.tLeft <= tRight) &&
            (other.lBottom <= lTop) && (other.lTop >= lBottom)) ;
}

NVLdVRect&
NVLdVRect::operator=(NVLdVRect src)
{
	if (this == &src)
		return *this ;

  tLeft   = src.tLeft ;
  tRight  = src.tRight ;
  lTop    = src.lTop ;
  lBottom = src.lBottom ;

  pView   = src.pView ;

  return *this ;
}
#endif

// --------------------------------------------------------------------------
// ------------------------- METHODES DE NVLdVTemps -------------------------
// --------------------------------------------------------------------------

NVLdVTemps::NVLdVTemps()
{
	sTemps = "00000000000000" ;
}

NVLdVTemps::NVLdVTemps(int iA, int iM, int iJ, int ih, int im, int is)
{
	sTemps = "00000000000000" ;
	placeAns(iA) ;
	placeMois(iM) ;
	placeJours(iJ) ;
	placeHeures(ih) ;
	placeMinutes(im) ;
	placeSecondes(is) ;
}

NVLdVTemps::NVLdVTemps(NVLdVTemps& rv)
{
    sTemps = rv.sTemps;
}

NVLdVTemps::~NVLdVTemps()
{
}

/**
* Init from a string of the kind : AAAAMMJJ
*/
bool
NVLdVTemps::initFromDate(string sDate)
{
	sTemps = "00000000000000" ;

	// Good size ?
	size_t iDateLen = strlen(sDate.c_str()) ;
	if (iDateLen < 8)
		return false ;
	if (iDateLen > 8)
		return initFromDateHeure(sDate) ;

	// Only digits ?
	for (size_t i = 0; i < iDateLen; i++)
		if (!(isdigit(sDate[i])))
			return false ;

	sTemps = sDate + string("000000") ;

	return true ;
}

/**
* Init from a string of the kind : AAAAMMJJhhmmss
*/
bool
NVLdVTemps::initFromDateHeure(string sDateHeure)
{
	sTemps = "00000000000000" ;

	// Good size ?
	size_t iDateLen = strlen(sDateHeure.c_str()) ;
	if (iDateLen != 14)
		return false ;

	// Only digits ?
	for (size_t i = 0; i < iDateLen; i++)
		if (!(isdigit(sDateHeure[i])))
			return false ;

	sTemps = sDateHeure ;

	return true ;
}

#ifndef __linux__
bool
NVLdVTemps::initFromSystemTime(LPSYSTEMTIME pST)
{
	if (NULL == pST)
		return false ;

	char buf[40] ;
  sprintf(buf,"%04d%02d%02d%02d%02d%02d",
                pST->wYear, pST->wMonth, pST->wDay,
                pST->wHour, pST->wMinute, pST->wSecond) ;

	sTemps = string(buf) ;

	return true ;
}
#endif

/**
* replace day and/or month by 01 if they are not specified
* replace year by 1900 if unspecified
* verify that day is good for the month (ex february the 30th -> 28)
*/
void
NVLdVTemps::normalize()
{
	// Year
	if (donneAns() <= 0)
  	placeAns(1900) ;

	// Month
	if (donneMois() <= 0)
		placeMois(1) ;
	if (donneMois() > 12)
		placeMois(12) ;

	// Day
	if (donneJours() <= 0)
	{
		placeJours(1) ;
    return ;
	}

	int iJours = donneJours() ;
  if (iJours < 29)
		return ;

  int iMois = donneMois() ;
  if (iMois == 2)
	{
		if (bissextile(donneAns()))
			placeJours(29) ;
		else
			placeJours(28) ;
    return ;
	}

	if ((iMois == 4) || (iMois == 6) || (iMois == 9) || (iMois == 11))
  {
  	if (iJours > 30)
    	placeJours(30) ;
    return ;
  }

	if (iJours > 31)
    	placeJours(31) ;
}

string
NVLdVTemps::donneFormattedDateHeure(NSContexte* pContexte, string sLang)
{
	string sTimeFormat = pContexte->getSuperviseur()->getText("0localInformation", "timeFormat") ;
  string formatDateHeure = getFormatedTime(sTemps, sLang, sTimeFormat, "") ;
	return formatDateHeure ;
}

string
NVLdVTemps::donneFormattedDate(NSContexte* pContexte, string sLang)
{
	string sDateFormat = pContexte->getSuperviseur()->getText("0localInformation", "dateFormat") ;
  string formatDate = getFormatedDate(string(sTemps, 0, 8), sLang, sDateFormat) ;
	return formatDate ;
}

string
NVLdVTemps::donneFormattedHeure(NSContexte* pContexte, string sLang)
{
	string sHourFormat = pContexte->getSuperviseur()->getText("0localInformation", "hourFormat") ;
  string formatHeure = getFormatedHour(string(sTemps, 8, 6), sLang, sHourFormat) ;
	return formatHeure ;
}

void
NVLdVTemps::takeTime()
{
  /*
  long clk = time(0) ;
  struct tm _FAR *now = localtime(&clk) ;

  placeAns(now->tm_year+1900) ;
  placeMois(now->tm_mon+1) ;
  placeJours(now->tm_mday) ;
  placeHeures(now->tm_hour) ;
  placeMinutes(now->tm_min) ;
  placeSecondes(now->tm_sec) ;
  */

  PRTime now = PR_Now() ;

  PRExplodedTime et_tmp ;
  PR_ExplodeTime(now, PR_LocalTimeParameters, &et_tmp) ;

  placeAns(et_tmp.tm_year) ;
  placeMois(et_tmp.tm_month + 1) ;
  placeJours(et_tmp.tm_mday) ;
  placeHeures(et_tmp.tm_hour) ;
  placeMinutes(et_tmp.tm_min) ;
  placeSecondes(et_tmp.tm_sec) ;
}

int
NVLdVTemps::donneAns()
{
	return donneDigit(sTemps[0]) * 1000 +
         donneDigit(sTemps[1]) * 100  +
         donneDigit(sTemps[2]) * 10   +
         donneDigit(sTemps[3]) ;
}

int
NVLdVTemps::donneMois()
{
	return donneDigit(sTemps[4]) * 10 + donneDigit(sTemps[5]) ;
}

int
NVLdVTemps::donneJours()
{
	return donneDigit(sTemps[6]) * 10 + donneDigit(sTemps[7]) ;
}

int
NVLdVTemps::donneHeures()
{
	return donneDigit(sTemps[8]) * 10 + donneDigit(sTemps[9]) ;
}

int
NVLdVTemps::donneMinutes()
{
	return donneDigit(sTemps[10]) * 10 + donneDigit(sTemps[11]) ;
}

int
NVLdVTemps::donneSecondes()
{
	return donneDigit(sTemps[12]) * 10 + donneDigit(sTemps[13]) ;
}

void
NVLdVTemps::placeAns(int iA)
{
	char sBuff[5] ;
	numacar(sBuff, iA, 4) ;
	for (int i = 0; i < 4; i++)
  	sTemps[i] = sBuff[i] ;
}

void
NVLdVTemps::placeMois(int iM)
{
	char sBuff[3] ;
	numacar(sBuff, iM, 2) ;
	for (int i = 0; i < 2; i++)
  	sTemps[i + 4] = sBuff[i] ;
}

void
NVLdVTemps::placeJours(int iJ)
{
	char sBuff[3];
	numacar(sBuff, iJ, 2);
	for (int i = 0; i < 2; i++)
		sTemps[i + 6] = sBuff[i] ;
}

void
NVLdVTemps::placeHeures(int ih)
{
	char sBuff[3];
	numacar(sBuff, ih, 2);
	for (int i = 0; i < 2; i++)
		sTemps[i + 8] = sBuff[i] ;
}

void
NVLdVTemps::placeMinutes(int im)
{
	char sBuff[3] ;
	numacar(sBuff, im, 2) ;
	for (int i = 0; i < 2; i++)
		sTemps[i + 10] = sBuff[i] ;
}

void
NVLdVTemps::placeSecondes(int is)
{
	char sBuff[3] ;
	numacar(sBuff, is, 2) ;
	for (int i = 0; i < 2; i++)
		sTemps[i + 12] = sBuff[i] ;
}

/**
*
* Add 12 months
*
*/
void
NVLdVTemps::ajouteAns(int iDeltaT, bool bAdjust)
{
	// placeAns(donneAns() + iDeltaT);

	ajouteMois(12 * iDeltaT, bAdjust) ;
}

/**
*
* Ajoute un nombre de mois
* La variable bAjust permet de savoir s'il faut adapter la date pour qu'elle
* corresponde à une vraie date (ex 31 février ramené au 28)
* Mettre bAjust à faux permet de garder des dates "fictives", comme "chaque fin
* de mois" qui correspond au 31 ; ces dates seront corrigées en temps réel par
* les méthodes donneXXX
*
* Add a certain amount of months
* bAjust is true when we want to adjust the date in order to get a genuine one
* for example february the 31th becomes february the 28th
* When bAjust is set to false, it allows to get "fake" dates such as "every end
* of month" when the day is set to the 31th ; these fake dates will be adjusted
* by the donneXXX methodes
*
*/
void
NVLdVTemps::ajouteMois(int iDeltaT, bool bAdjust)
{
	int iYear = donneAns() ;
	int iMonth = (iYear - 1900) * 12 + donneMois() + iDeltaT ;
	if ((iMonth % 12) == 0)
	{
		placeMois(12) ;
		placeAns((iMonth / 12) - 1 + 1900) ;
	}
	else
	{
		placeMois(iMonth % 12) ;
		placeAns((iMonth / 12) + 1900) ;
	}

  if (bAdjust)
  	normalize() ;

/*
    double dAnnees = double(iDeltaT) / double(12);
    int iAnnees = floor(dAnnees);
    int iMois   = iDeltaT - (12 * iAnnees);
    placeMois(donneMois() + iMois);
    ajouteAns(iAnnees);
*/
}

void
NVLdVTemps::ajouteJours(int iDeltaT)
{
  ClassLib::TDate date(donneJours(), donneMois(), donneAns()) ;
	date += iDeltaT ;
	placeAns(date.Year()) ;
	placeMois(date.Month()) ;
	placeJours(date.DayOfMonth()) ;
}

void
NVLdVTemps::ajouteHeures(long iDeltaT)
{
  ClassLib::TDate tdDate = ClassLib::TDate(donneJours(), donneMois(), donneAns()) ;
  ClassLib::TTime ttTime = ClassLib::TTime(tdDate, donneHeures(), donneMinutes(), donneSecondes()) ;
  ttTime += iDeltaT * 3600 ;
  this->convert(ttTime) ;

/*
    double dJours = double(iDeltaT) / double(24);
    int iJours  = floor(dJours);
    int iHeures = iDeltaT - (24 * iJours);
    placeHeures(donneHeures() + iHeures);
    ajouteJours(iJours);
*/
}

void
NVLdVTemps::ajouteMinutes(long iDeltaT)
{
  ClassLib::TDate tdDate = ClassLib::TDate(donneJours(), donneMois(), donneAns()) ;
  ClassLib::TTime ttTime = ClassLib::TTime(tdDate, donneHeures(), donneMinutes(), donneSecondes()) ;
	ttTime += iDeltaT * 60 ;
	this->convert(ttTime) ;

/*
    double dHeures = double(iDeltaT) / double(60);
    int iHeures  = floor(dHeures);
    int iMinutes = iDeltaT - (60 * iHeures);
    placeMinutes(donneMinutes() + iMinutes);
    ajouteHeures(iHeures);
*/
}

struct tm   *NVLdVTemps::convert2tm()
{
	struct tm *tmDate = 0 ;
try
{
	tmDate = new struct tm ;
  tmDate->tm_sec = donneSecondes() ;
  tmDate->tm_min = donneMinutes() ;
  tmDate->tm_hour = donneHeures() ;
  tmDate->tm_mday = donneJours() ;
  tmDate->tm_mon = donneMois() ;
  tmDate->tm_year = donneAns() ;

  return tmDate ;
}
catch (...)
{
	erreur("Exception NVLdVTemps::convert2tm.", standardError) ;
  return tmDate ;
}
}

void    NVLdVTemps::convert(ClassLib::TDate *td2Convert)
{
    this->placeJours(td2Convert->DayOfMonth()) ;
    this->placeMois(td2Convert->Month()) ;
    this->placeAns(td2Convert->Year()) ;
}

void    NVLdVTemps::convert(ClassLib::TTime tt2Convert)
{
	this->placeSecondes(tt2Convert.Second()) ;
  this->placeMinutes(tt2Convert.Minute()) ;
  this->placeHeures(tt2Convert.Hour()) ;

  ClassLib::TDate tdDate = ClassLib::TDate(tt2Convert) ;

  this->placeJours(tdDate.DayOfMonth()) ;
  this->placeMois(tdDate.Month()) ;
  this->placeAns(tdDate.Year()) ;
}

string  NVLdVTemps::donneMonthName()
{
try
{
  ClassLib::TDate tdDate(donneJours(), donneMois(), donneAns()) ;
  char  cMonthName[50] ;
  sprintf(cMonthName, tdDate.NameOfMonth(), sizeof(tdDate.NameOfMonth())) ;

  string  sResult = string(cMonthName) ;

  return sResult ;
}
catch (...)
{
	erreur("Exception NVLdVTemps::donneMonthName.", standardError) ;
  return "" ;
}
}

void
NVLdVTemps::convert(struct tm *tm2Convert)
{
	this->placeSecondes(tm2Convert->tm_sec) ;
  this->placeMinutes(tm2Convert->tm_min) ;
  this->placeHeures(tm2Convert->tm_hour) ;
  this->placeJours(tm2Convert->tm_mday) ;
  this->placeMois(tm2Convert->tm_mon) ;
  this->placeAns(tm2Convert->tm_year) ;
}

void
NVLdVTemps::ajouteSecondes(long iDeltaT)
{
  ClassLib::TDate tdDate = ClassLib::TDate(donneJours(), donneMois(), donneAns()) ;
  ClassLib::TTime ttTime = ClassLib::TTime(tdDate, donneHeures(), donneMinutes(), donneSecondes()) ;
  ttTime += iDeltaT ;
  this->convert(ttTime) ;

/*
    double dMinutes = double(iDeltaT) / double(60);
    int iMinutes  = floor(dMinutes);
    int iSecondes = iDeltaT - (60 * iMinutes);
    placeSecondes(donneSecondes() + iSecondes);
    ajouteMinutes(iMinutes);
*/
}

void
NVLdVTemps::ajouteTemps(int iDeltaT, string sUnite, NSContexte *pCtx)
{
	if (iDeltaT == 0)
  	return ;

	string sUniteSens ;
	pCtx->getDico()->donneCodeSens(&sUnite, &sUniteSens) ;

	if      (sUniteSens == "2DAT3")
  	ajouteAns(iDeltaT) ;
  else if (sUniteSens == "2DAT2")
  	ajouteMois(iDeltaT) ;
  else if (sUniteSens == "2DAT1")
  	ajouteJours(7 * iDeltaT) ;
  else if (sUniteSens == "2DAT0")
  	ajouteJours(iDeltaT) ;
  else if (sUniteSens == "2HE00")
  	ajouteHeures(iDeltaT) ;
  else if (sUniteSens == "2MINU")
  	ajouteMinutes(iDeltaT) ;
  else if (sUniteSens == "2SEC0")
  	ajouteSecondes(iDeltaT) ;
}

unsigned long
NVLdVTemps::yearsBetween(NVLdVTemps otherDate)
{
  unsigned long ulMonths = monthsBetween(otherDate) ;

  return ulMonths / 12 ;
}

unsigned long
NVLdVTemps::monthsBetween(NVLdVTemps otherDate)
{
  // Par sécurité, on prend d'abord une copie normalisée
  //
	NVLdVTemps NormThis(*this) ;
  NormThis.normalize() ;
  NVLdVTemps NormOther(otherDate) ;
  NormOther.normalize() ;

  unsigned long ulThisMonths  = NormThis.donneMois() + 12 * NormThis.donneAns() ;
  unsigned long ulOtherMonths = NormOther.donneMois() + 12 * NormOther.donneAns() ;

  return ulThisMonths - ulOtherMonths ;
}

unsigned long
NVLdVTemps::daysBetween(NVLdVTemps otherDate)
{
	// Par sécurité, on prend d'abord une copie normalisée
  //
	NVLdVTemps NormThis(*this) ;
  NormThis.normalize() ;
  NVLdVTemps NormOther(otherDate) ;
  NormOther.normalize() ;

  ClassLib::TDate TThis(NormThis.donneJours(), NormThis.donneMois(), NormThis.donneAns()) ;
  ClassLib::TDate TOther(NormOther.donneJours(), NormOther.donneMois(), NormOther.donneAns()) ;

  return TThis - TOther ;

/*
    unsigned long lDeltaJ =
        TDate::Jday(donneMois(), donneJours(), donneAns())
        - TDate::Jday(otherDate.donneMois(), otherDate.donneJours(), otherDate.donneAns());

    return lDeltaJ ;
*/
}

unsigned long
NVLdVTemps::hoursBetween(NVLdVTemps otherDate)
{
}

unsigned long
NVLdVTemps::minutesBetween(NVLdVTemps otherDate)
{
}

unsigned long
NVLdVTemps::secondsBetween(NVLdVTemps otherDate)
{
}

NVLdVTemps&
NVLdVTemps::operator=(NVLdVTemps src)
{
	if (this == &src)
		return *this ;

	sTemps = src.sTemps ;

	return *this ;
}

// --------------------------------------------------------------------------
// --------------------------- METHODES DE TimeArray ------------------------
// --------------------------------------------------------------------------

TimeArray::TimeArray()
          :TimeVector()
{
}

TimeArray::TimeArray(TimeArray& rv)
          :TimeVector()
{
try
{
  if (false == rv.empty())
    for (TimeIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new NVLdVTemps(*(*i))) ;
}
catch (...)
{
    erreur("Exception TimeArray copy ctor.", standardError) ;
}
}

bool rcSortByDateInf(NVLdVTemps* p1, NVLdVTemps* p2)
{
	return (*p1 < *p2) ;
}

bool rcSortByDateSup(NVLdVTemps* p1, NVLdVTemps* p2)
{
	return (*p1 > *p2) ;
}

void
TimeArray::sortDates(bool bRecentOnTop)
{
    if (bRecentOnTop)
        sort(begin(), end(), rcSortByDateSup) ;
    else
        sort(begin(), end(), rcSortByDateInf) ;
}

void
TimeArray::vider()
{
  if (empty())
    return ;

  for (TimeIter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

TimeArray::~TimeArray()
{
	vider();
}

TimeArray&
TimeArray::operator=(TimeArray src)
{
try
{
  if (this == &src)
		return *this ;

	vider() ;

  if (false == src.empty())
    for (TimeIter i = src.begin() ; src.end() != i ; i++)
    	push_back(new NVLdVTemps(*(*i))) ;

	return *this ;
}
catch (...)
{
  erreur("Exception TimeArray = operator.", standardError) ;
  return *this ;
}
}

#ifndef _ENTERPRISE_DLL
#ifndef _EXT_CAPTURE

// --------------------------------------------------------------------------
// --------------------------- METHODES DE NSPixel --------------------------
// --------------------------------------------------------------------------

NSPixel::NSPixel(NS_CLASSLIB::TPoint Point, NS_CLASSLIB::TColor Color)
{
    point = Point;
    color = Color;
}

NSPixel::NSPixel(int X, int Y, NS_CLASSLIB::TColor Color)
{
    point = NS_CLASSLIB::TPoint(X, Y);
    color = Color;
}

NSPixel::NSPixel(NSPixel& rv)
{
    point = rv.point;
    color = rv.color;
}

NSPixel::~NSPixel()
{
}

char*
NSPixel::getTuple()
{
	char tuple[3] ;

	tuple[0] = color.Red() ;
	tuple[1] = color.Green() ;
	tuple[2] = color.Blue() ;

	return tuple ;
}

NSPixel&
NSPixel::operator=(NSPixel src)
{
	if (this == &src)
		return *this ;

	point = src.point ;
	color = src.color ;

	return *this ;
}

// --------------------------------------------------------------------------
// -------------------------- METHODES DE PixelArray ------------------------
// --------------------------------------------------------------------------

PixelArray::PixelArray()
           : PixArray()
{
    ArrayRect.Set(0, 0, 0, 0);
}

PixelArray::PixelArray(PixelArray& rv)
           :PixArray()
{
try
{
    ArrayRect = rv.ArrayRect ;

    if (!(rv.empty()))
        for (pixelIter i = rv.begin(); i != rv.end(); i++)
            push_back(new NSPixel(*(*i))) ;
}
catch (...)
{
    erreur("Exception PixelArray copy ctor.", standardError, 0) ;
}
}

bool
PixelArray::ScreenCapture(TWindowDC* pWinDC, ClassLib::TRect* pSrcRect)
{
try
{
    for (int i = pSrcRect->Left(); i < pSrcRect->Right(); i++)
        for (int j = pSrcRect->Top(); j < pSrcRect->Bottom(); j++)
            ajouter(new NSPixel(i, j, pWinDC->GetPixel(i, j))) ;

    return true ;
}
catch (...)
{
    erreur("Exception PixelArray::ScreenCapture.", standardError, 0) ;
    return false ;
}
}

bool
PixelArray::Affiche(TDC* pDestDC, NS_CLASSLIB::TRect* pDstRect, NS_CLASSLIB::TPoint* pSrcPt)
{
    if (empty())
        return true;

    int iDeltaX = pSrcPt->X();
    int iDeltaY = pSrcPt->Y();

    int iXMin = pDstRect->Left()   - iDeltaX;
    int iXMax = pDstRect->Right()  - iDeltaX;
    int iYMin = pDstRect->Top()    - iDeltaY;
    int iYMax = pDstRect->Bottom() - iDeltaY;

    pixelIter it = begin();
    for ( ; it != end(); it++)
    {
        int x = (*it)->getPoint().X();
        if ((x >= iXMin) && (x <= iXMax))
        {
            int y = (*it)->getPoint().Y();
            if ((y >= iYMin) && (y <= iYMax))
                pDestDC->SetPixel(x-iDeltaX, y-iDeltaY, (*it)->getColor());
        }
    }
    return true ;
}

void
PixelArray::ajouter(NSPixel* pNewPix)
{
    if (!(ArrayRect.Contains(pNewPix->getPoint())))
    {
        int pixX    = pNewPix->getPoint().X();
        int pixY    = pNewPix->getPoint().Y();

        int left    = ArrayRect.Left();
        int right   = ArrayRect.Right();
        int top     = ArrayRect.Top();
        int bottom  = ArrayRect.Bottom();

        if      (pixX < left)
            left = pixX;
        else if (pixX > right)
            right = pixX;

        if      (pixY < top)
            top = pixY;
        else if (pixY > bottom)
            bottom = pixY;

        ArrayRect.Set(left, top, right, bottom);
    }

    push_back(pNewPix);
}

NS_CLASSLIB::TColor
PixelArray::getColor(NS_CLASSLIB::TPoint pt, bool* pExist)
{
    if (empty())
    {
        *pExist = false;
        return NS_CLASSLIB::TColor::White;
    }

    pixelIter i = begin();
    for (; (i != end()) && ((*i)->getPoint() != pt); i++);

    if (i == end())
    {
        *pExist = false;
        return NS_CLASSLIB::TColor::White;
    }

    *pExist = true;
    return (*i)->getColor();
}

void
PixelArray::vider()
{
    if (empty())
        return;

    for (pixelIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

PixelArray::~PixelArray()
{
	vider();
}

PixelArray&
PixelArray::operator=(PixelArray src)
{
try
{
  if (this == &src)
		return *this ;

	vider() ;

	ArrayRect = src.ArrayRect ;

	if (!(src.empty()))
  	for (pixelIter i = src.begin(); i != src.end(); i++)
    	push_back(new NSPixel(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception PixelArray = operator.", standardError, 0) ;
	return *this ;
}
}

// --------------------------------------------------------------------------
// -------------------------- METHODES DE NSBitmap --------------------------
// --------------------------------------------------------------------------

NSBitmap::NSBitmap()
{
    pBitmap = 0;
}

NSBitmap::NSBitmap(NSBitmap& rv)
{
try
{
    if (rv.pBitmap)
        pBitmap = new TDib(*(rv.pBitmap)) ;
}
catch (...)
{
    erreur("Exception NSBitmap copy ctor.", standardError, 0) ;
}
}

NSBitmap::~NSBitmap()
{
    if (pBitmap)
        delete pBitmap;
}

bool
NSBitmap::ScreenCapture(TWindowDC* pWinDC, ClassLib::TRect* pSrcRect)
{
try
{
    if (pBitmap)
    {
        delete pBitmap;
        pBitmap = 0;
    }

    // TScreenDC dc;
    // ::GetSystemPaletteEntries(dc, 0, nColors, (LPPALETTEENTRY)Colors);

    // Construction d'un Bitmap dépendant du DC
    //
    OWL::TBitmap* pBMap = new OWL::TBitmap(*pWinDC, pSrcRect->Width(), pSrcRect->Height());
    // OWL::TBitmap* pBMap = new OWL::TBitmap(dc, pSrcRect->Width(), pSrcRect->Height());
    //
    // Création d'un Memory DC compatible avec le Window DCavec ce bitmap
    //
    TMemoryDC* pMemDC = new TMemoryDC(*pWinDC);
    // TMemoryDC* pMemDC = new TMemoryDC(dc);
    //
    // Sélection du Bitmap pour le Memory DC
    //
    pMemDC->SelectObject(*pBMap);
    //
    // Copie du Windows DC vers le Memory DC
    //
    if (pMemDC->BitBlt(0, 0, pSrcRect->Width(), pSrcRect->Height(),
                                *pWinDC, 0, 0))
    {
        bool hasPalette = ToBool(pWinDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE);
        //
        // Cas où le dc possède une palette
        //
        if (hasPalette)
        {
            HANDLE hPal = pMemDC->GetCurrentObject(OBJ_PAL);
            if (hPal)
            {
                TPalette* pPal = new TPalette(HPALETTE(hPal));

                uint16  nColors;
                pPal->GetObject(nColors);
                if (nColors)
                {
                    LOGPALETTE* logPal = (LOGPALETTE*) new
                        uint8[sizeof(LOGPALETTE)+(nColors-1)*sizeof(PALETTEENTRY)];

                    logPal->palVersion = 0x300;
                    logPal->palNumEntries = nColors;
                    int iNb = pPal->GetPaletteEntries(0, nColors, logPal->palPalEntry);

                    if (iNb > 0)
                    {
                        TPalette copyPal(logPal);
                        pBitmap = new TDib(*pBMap, &copyPal);
                    }
                    // else
                    //    DWORD err = GetLastError();

                    delete[] logPal;
                }
                delete pPal;
            }
        }
        //
        // Cas où le dc ne possède pas de palette
        //
        else
            pBitmap = new TDib(*pBMap);
    }
    delete pMemDC;
    delete pBMap;

    return true;
}
catch (...)
{
    erreur("Exception ScreenCapture.", standardError, 0) ;
    return false ;
}
}

//
// Affichage du rectangle pSrcRect du bitmap sur le DC pDestDC
// avec pDestPt comme point en haut et à gauche du rectangle à afficher
//
bool
NSBitmap::Affiche(TDC* pDestDC, NS_CLASSLIB::TRect* pDstRect, NS_CLASSLIB::TPoint* pSrcPt, int iZoomFactor)
{
  if (!pBitmap)
    return true;
try
{
    bool bBottomUp;

    // There are two varieties of DIB: "bottom-up" DIBs, in which the origin
    // lies at the lower left corner, and "top-down" DIBs, in which the origin
    // lies at the upper left corner. If the height of a DIB, as indicated by
    // the biHeight member of the BITMAPINFOHEADER structure, is a positive
    // value, it is a bottom-up DIB; if the height is a negative value, it is
    // a top-down DIB. Top-down DIBs cannot be compressed.

    if (pBitmap->Height() > 0)
        bBottomUp = true;
    else
        bBottomUp = false;

    //
    // Faut-il se préoccuper de Palette (affichage < 24 bits)
    //
    bool hasPalette = ToBool(pDestDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE);
    if (hasPalette)
    {
        TPointer<TPalette> palette = new TPalette(*pBitmap);
        pDestDC->SelectObject(*palette);
        pDestDC->RealizePalette();
    }

    //
    // Adaptation entre le rectangle à repeindre et le rectangle du DIB
    //
    NS_CLASSLIB::TRect dibRect;
    if (bBottomUp)
    {
        dibRect = NS_CLASSLIB::TRect(pDstRect->Left() + pSrcPt->X(),
                                     pBitmap->Height() - pDstRect->Bottom() - pSrcPt->Y(),
                                     pDstRect->Right() + pSrcPt->X(),
                                     pBitmap->Height() - pDstRect->Top() - pSrcPt->Y());
    }
    else
        dibRect = *pDstRect ;

    if (iZoomFactor != 1)
    {
        pDestDC->SaveDC() ;
        pDestDC->SetMapMode(MM_ISOTROPIC) ;
        SIZE destSize ;
        destSize.cx = pDstRect->Width() ;
        destSize.cy = pDstRect->Height() ;

        ::SetWindowExtEx(*pDestDC, pDstRect->Width() / iZoomFactor,
                                pDstRect->Height() / iZoomFactor, &destSize) ;
        ::SetViewportExtEx(*pDestDC, pDstRect->Width(),
                                pDstRect->Height(), &destSize) ;
    }

    bool bResult = pDestDC->StretchDIBits(*pDstRect, dibRect, *pBitmap) ;

    if (iZoomFactor != 1)
        pDestDC->RestoreDC() ;

    return bResult ;
}
catch (...)
{
    erreur("Exception Affiche bitmap.", standardError, 0) ;
    return false ;
}
}

bool
NSBitmap::trouveBloc(NSOCRbloc* pBloc, NS_CLASSLIB::TPoint* pPoint)
{
    bool                bExist;

    NS_CLASSLIB::TColor cFond = getColor(*pPoint, &bExist);

    int     iTop            = pPoint->Y();
    int     iBottom         = pPoint->Y();
    int     iLeft           = pPoint->X();
    int     iRight          = pPoint->X();

    bool    bGrowToTop      = true;
    bool    bGrowToBottom   = true;
    bool    bGrowToLeft     = true;
    bool    bGrowToRight    = true;

    // Algo :   On fait grandir un rectangle à partir du point de départ
    //
    //          Lorsque, dans une direction l'ensemble des points ajoutés est
    //          différent de la couleur du fond, on suppose qu'on a atteint
    //          le bord dans cette direction.
    //
    //          On vérifie, lors de la croissance des autres directions, que ce
    //          bord reste bien exempt de pixels de la couleur du fond ; si ce
    //          n'est pas le cas, on redémarre la progression dans cette
    //          direction

    bool bTourner = true;

    while (bTourner)
    {
        //
        // On fait grandir le rectangle d'un cran
        // Rectangle growing one step
        //
        if ((bGrowToTop) && (iTop > 0))
            iTop--;

        if ((bGrowToBottom) && (iBottom < getBottom() - 1))
            iBottom++;

        if ((bGrowToLeft) && (iLeft > 0))
            iLeft--;

        if ((bGrowToRight) && (iRight < getRight() - 1))
            iRight++;

        if (((!bGrowToTop)      || (iTop == 0))                 &&
            ((!bGrowToBottom)   || (iBottom == getBottom()-1))  &&
            ((!bGrowToLeft)     || (iLeft == 0))                &&
            ((!bGrowToRight)    || (iRight == getRight()-1)))
            break;

        //
        // Inspection en haut - watching top line
        //
        // Attention : >=0 et non >0 car sinon la ligne 0 est incluse d'office
        // Be careful : >=0 and not >0 or 0 line should be automatically included
        //
        if (iTop >= 0)
        {
            // On regarde s'il existe au moins un point de la couleur du fond
            // sur la ligne du haut
            // We are scanning top line in order to see if it contains a pixel
            // of background color
            //
            NS_CLASSLIB::TPoint nextPoint = NS_CLASSLIB::TPoint(iLeft, iTop);
            int i;
            for (i = iLeft; i <= iRight; i++)
            {
                if ((getColor(nextPoint, &bExist) == cFond) && (bExist))
                    break;
                nextPoint = NS_CLASSLIB::TPoint(i, iTop);
            }
            //
            // Aucun pixel de la couleur du fond
            // No background colored pixel found
            //
            if (i > iRight)
                bGrowToTop = false;
            else
                bGrowToTop = true;
        }

        //
        // Inspection en bas - watching bottom line
        //
        if (iBottom < getBottom())
        {
            NS_CLASSLIB::TPoint nextPoint = NS_CLASSLIB::TPoint(iLeft, iBottom);
            int i;
            for (i = iLeft; i <= iRight; i++)
            {
                if ((getColor(nextPoint, &bExist) == cFond) && (bExist))
                    break;
                nextPoint = NS_CLASSLIB::TPoint(i, iBottom);
            }
            if (i > iRight)
                bGrowToBottom = false;
            else
                bGrowToBottom = true;
        }

        //
        // Inspection à gauche - watching left line
        //
        if (iLeft >= 0)
        {
            NS_CLASSLIB::TPoint nextPoint = NS_CLASSLIB::TPoint(iLeft, iTop);
            int i;
            for (i = iTop; i <= iBottom; i++)
            {
                if ((getColor(nextPoint, &bExist) == cFond) && (bExist))
                    break;
                nextPoint = NS_CLASSLIB::TPoint(iLeft, i);
            }
            if (i > iBottom)
                bGrowToLeft = false;
            else
                bGrowToLeft = true;
        }

        //
        // Inspection à droite - watching right line
        //
        if (iRight < getRight())
        {
            NS_CLASSLIB::TPoint nextPoint = NS_CLASSLIB::TPoint(iRight, iTop);
            int i;
            for (i = iTop; i <= iBottom; i++)
            {
                if ((getColor(nextPoint, &bExist) == cFond) && (bExist))
                    break;
                nextPoint = NS_CLASSLIB::TPoint(iRight, i);
            }
            if (i > iBottom)
                bGrowToRight = false;
            else
                bGrowToRight = true;
        }

        if (((!bGrowToTop)      || (iTop == 0))                 &&
            ((!bGrowToBottom)   || (iBottom == getBottom()-1))  &&
            ((!bGrowToLeft)     || (iLeft == 0))                &&
            ((!bGrowToRight)    || (iRight == getRight()-1)))
            bTourner = false;
    }

    // haut non inclus
    if (!bGrowToTop)
        iTop++;
    // bas non inclus
    if (!bGrowToBottom)
        iBottom--;
    // haut non inclus
    if (!bGrowToLeft)
        iLeft++;
    // bas non inclus
    if (!bGrowToRight)
        iRight--;

    pBloc->BlocRect = NS_CLASSLIB::TRect(iLeft, iTop, iRight, iBottom);
    pBloc->bgColor  = cFond;

    /*
    NS_CLASSLIB::TPoint nextPoint = *pPoint;

    // Recherche du bord gauche
    //
    if (iLeft > 0)
    {
        // Recherche vers le haut à gauche
        //
        bool bProgression = true;
        while (bProgression)
        {
            nextPoint = NS_CLASSLIB::TPoint(iLeft - 1, iTop);

            // Recherche vers la gauche
            while ((iLeft > 0) &&
                   (getColor(nextPoint, &bExist) == cFond) &&
                   (bExist))
            {
                iLeft--;
                if (iLeft > 0)
                    nextPoint = NS_CLASSLIB::TPoint(iLeft - 1, iTop);
            }

            if (iTop > 0)
            {
                nextPoint = NS_CLASSLIB::TPoint(iLeft, iTop - 1);
                if ((getColor(nextPoint, &bExist) == cFond) &&
                    (bExist))
                    iTop--;
                else
                    bProgression = false;
            }
            else
                bProgression = false;
        }
        //
        // Recherche vers le bas à gauche
        //
        if (iBottom < getBottom())
            bProgression = true;
        else
            bProgression = false;

        while (bProgression)
        {
            nextPoint = NS_CLASSLIB::TPoint(iLeft - 1, iBottom);

            // Recherche vers la gauche
            while ((iLeft > 0) &&
                   (getColor(nextPoint, &bExist) == cFond) &&
                   (bExist))
            {
                iLeft--;
                if (iLeft > 0)
                    nextPoint = NS_CLASSLIB::TPoint(iLeft - 1, iBottom);
            }

            if (iBottom < getBottom())
            {
                nextPoint = NS_CLASSLIB::TPoint(iLeft, iBottom + 1);
                if ((getColor(nextPoint, &bExist) == cFond) &&
                    (bExist))
                    iBottom++;
                else
                    bProgression = false;
            }
            else
                bProgression = false;
        }
    }

    // Recherche du bord droit
    //
    if (iRight < getRight())
    {
        // Recherche vers le haut à droite
        //
        bool bProgression = true;
        while (bProgression)
        {
            nextPoint = NS_CLASSLIB::TPoint(iRight + 1, iTop);

            // Recherche vers la droite
            while ((iRight < getRight()) &&
                   (getColor(nextPoint, &bExist) == cFond) &&
                   (bExist))
            {
                iRight++;
                if (iRight < getRight())
                    nextPoint = NS_CLASSLIB::TPoint(iRight + 1, iTop);
            }

            if (iTop > 0)
            {
                nextPoint = NS_CLASSLIB::TPoint(iRight, iTop - 1);
                if ((getColor(nextPoint, &bExist) == cFond) &&
                    (bExist))
                    iTop--;
                else
                    bProgression = false;
            }
            else
                bProgression = false;
        }
        //
        // Recherche vers le bas à droite
        //
        if (iBottom < getBottom())
            bProgression = true;
        else
            bProgression = false;

        while (bProgression)
        {
            nextPoint = NS_CLASSLIB::TPoint(iRight + 1, iBottom);

            // Recherche vers la gauche
            while ((iRight < getRight()) &&
                   (getColor(nextPoint, &bExist) == cFond) &&
                   (bExist))
            {
                iRight++;
                if (iRight > 0)
                    nextPoint = NS_CLASSLIB::TPoint(iRight + 1, iBottom);
            }

            if (iBottom < getBottom())
            {
                nextPoint = NS_CLASSLIB::TPoint(iRight, iBottom + 1);
                if ((getColor(nextPoint, &bExist) == cFond) &&
                    (bExist))
                    iBottom++;
                else
                    bProgression = false;
            }
            else
                bProgression = false;
        }
    }
    pBloc->BlocRect = NS_CLASSLIB::TRect(iLeft, iTop, iRight, iBottom);
    pBloc->bgColor  = cFond;

    */

    return true;
}

bool
NSBitmap::enableCapture(ClassLib::TRect* pRect, NS_CLASSLIB::TColor* pBgColor)
{
    if ((!pRect) || (!pBgColor))
        return false ;

    bool bExist;

    // Couleur du point supérieur gauche
    ClassLib::TPoint PointTest = pRect->TopLeft();

    *pBgColor = getColor(PointTest, &bExist);
    if (!bExist)
        return false;

    bool bLeft, bTop, bRight, bBottom;

    bLeft      = false;
    bTop       = false;
    bRight     = false;
    bBottom    = false;

    nearBlank(pRect, pBgColor, &bLeft, &bTop, &bRight, &bBottom);

    if (bLeft && bTop && bRight && bBottom)
        return true;

    existEdges(pRect, pBgColor, &bLeft, &bTop, &bRight, &bBottom);

    if (bLeft && bTop && bRight && bBottom)
        return true;

    return false;
}

void
NSBitmap::nearBlank(ClassLib::TRect* pRect, NS_CLASSLIB::TColor* pBgColor,
                    bool *pLeft, bool *pTop, bool *pRight, bool *pBottom)
{
    if (!pRect || !pBgColor || !pLeft || !pTop || !pRight || !pBottom)
        return ;

    bool bExist;
    //
    // On vérifie que le rectangle correspond bien à une zone de saisie
    //
    // Le test consiste à vérifier que le tour du rectangle donné a bien une
    // couleur uniforme, qui est alors considérée comme la couleur du fond
    //

    ClassLib::TPoint PointTest = pRect->TopLeft();

    int iLargeur = pRect->Width();
    int iHauteur = pRect->Height();

    int iNbPts;
    //
    // Couleur des points qui constituent le tour
    //
    // Progression jusqu'au point supérieur droit
    //
    if (!(*pTop))
    {
        iNbPts = 0;
        while (PointTest != pRect->TopRight())
        {
            PointTest = ClassLib::TPoint(PointTest.X() + 1, PointTest.Y());
            if (getColor(PointTest, &bExist) != *pBgColor)
                iNbPts++;
        }
        if (iNbPts <= iLargeur / 10)
            *pTop = true;
    }
    else
        PointTest = pRect->TopRight();

    //
    // Progression jusqu'au point inférieur droit
    //
    if (!(*pRight))
    {
        iNbPts = 0;
        while (PointTest != pRect->BottomRight())
        {
            PointTest = ClassLib::TPoint(PointTest.X(), PointTest.Y() + 1);
            if (getColor(PointTest, &bExist) != *pBgColor)
                iNbPts++;
        }
        if (iNbPts <= iHauteur / 10)
            *pRight = true;
    }
    else
        PointTest = pRect->BottomRight();

    //
    // Progression jusqu'au point inférieur gauche
    //
    if (!(*pBottom))
    {
        iNbPts = 0;
        while (PointTest != pRect->BottomLeft())
        {
            PointTest = ClassLib::TPoint(PointTest.X() - 1, PointTest.Y());
            if (getColor(PointTest, &bExist) != *pBgColor)
                iNbPts++;
        }
        if (iNbPts <= iLargeur / 10)
            *pBottom = true;
    }
    else
        PointTest = pRect->BottomLeft();

    //
    // Progression jusqu'au point supérieur gauche
    //
    if (!(*pLeft))
    {
        iNbPts = 0;
        while (PointTest != pRect->TopLeft())
        {
            PointTest = ClassLib::TPoint(PointTest.X(), PointTest.Y() - 1);
            if (getColor(PointTest, &bExist) != *pBgColor)
                iNbPts++;
        }
        if (iNbPts <= iHauteur / 10)
            *pLeft = true;
    }

    return;
}

void
NSBitmap::existEdges(ClassLib::TRect* pRect, NS_CLASSLIB::TColor* pBgColor,
                        bool *pLeft, bool *pTop, bool *pRight, bool *pBottom)
{
    if (!pRect || !pBgColor || !pLeft || !pTop || !pRight || !pBottom)
        return ;

    bool bExist;
    //
    // On vérifie que le rectangle correspond bien à une zone de saisie
    //
    // Le test consiste à vérifier que le bord extérieur du rectangle donné
    // a bien une couleur uniforme
    //
    NS_CLASSLIB::TColor edgeColor;
    ClassLib::TPoint PointTest = pRect->TopLeft();

    //
    // Couleur des points qui constituent le tour
    //
    // Progression jusqu'au point supérieur droit
    //
    // Si on point du bord n'appartient pas à la couleur du fond, alors,
    // soit il n'est pas prolongé (ses voisins extérieurs ne sont pas de la
    // même couleur), soit cette couleur est celle de tout le cadre
    //
    if (!(*pTop))
    {
        if (PointTest.Y() == getTop())
            *pTop = true;
        else
        {
            bool bRun = true;
            while ((PointTest != pRect->TopRight()) && bRun)
            {
                PointTest = ClassLib::TPoint(PointTest.X() + 1, PointTest.Y());
                if (getColor(PointTest, &bExist) != *pBgColor)
                {
                    edgeColor = getColor(PointTest, &bExist);
                    if ((getColor(PointTest.X()-1, PointTest.Y()-1, &bExist) == edgeColor) ||
                        (getColor(PointTest.X(),   PointTest.Y()-1, &bExist) == edgeColor) ||
                        (getColor(PointTest.X()+1, PointTest.Y()-1, &bExist) == edgeColor))
                    {
                        for (int j = pRect->Left(); (j <= pRect->Right()) && bRun; j++)
                            if (getColor(j, PointTest.Y()-1, &bExist) != edgeColor)
                                bRun = false;
                    }
                }
            }
            if (bRun)
                *pTop = true;
        }
    }
    PointTest = pRect->TopRight();

    //
    // Progression jusqu'au point inférieur droit
    //
    if (!(*pRight))
    {
        if (PointTest.X() == getRight())
            *pRight = true;
        else
        {
            bool bRun = true;
            while ((PointTest != pRect->BottomRight()) && bRun)
            {
                PointTest = ClassLib::TPoint(PointTest.X(), PointTest.Y() + 1);
                if (getColor(PointTest, &bExist) != *pBgColor)
                {
                    edgeColor = getColor(PointTest, &bExist);
                    if ((getColor(PointTest.X()+1, PointTest.Y()-1, &bExist) == edgeColor) ||
                        (getColor(PointTest.X()+1, PointTest.Y(),   &bExist) == edgeColor) ||
                        (getColor(PointTest.X()+1, PointTest.Y()+1, &bExist) == edgeColor))
                    {
                        for (int j = pRect->Top(); (j <= pRect->Bottom()) && bRun; j++)
                            if (getColor(PointTest.X()+1, j, &bExist) != edgeColor)
                                bRun = false;
                    }
                }
            }
            if (bRun)
                *pRight = true;
        }
    }
    PointTest = pRect->BottomRight();

    //
    // Progression jusqu'au point inférieur gauche
    //
    if (!(*pBottom))
    {
        if (PointTest.Y() == getTop())
            *pBottom = true;
        else
        {
            bool bRun = true;
            while ((PointTest != pRect->BottomLeft()) && bRun)
            {
                PointTest = ClassLib::TPoint(PointTest.X() - 1, PointTest.Y());
                if (getColor(PointTest, &bExist) != *pBgColor)
                {
                    edgeColor = getColor(PointTest, &bExist);
                    if ((getColor(PointTest.X()-1, PointTest.Y()+1, &bExist) == edgeColor) ||
                        (getColor(PointTest.X(),   PointTest.Y()+1, &bExist) == edgeColor) ||
                        (getColor(PointTest.X()+1, PointTest.Y()+1, &bExist) == edgeColor))
                    {
                        for (int j = pRect->Left(); (j <= pRect->Right()) && bRun; j++)
                            if (getColor(j, PointTest.Y()+1, &bExist) != edgeColor)
                                bRun = false;
                    }
                }
            }
            if (bRun)
                *pBottom = true;
        }
    }
    PointTest = pRect->BottomLeft();

    //
    // Progression jusqu'au point supérieur gauche
    //
    if (!(*pLeft))
    {
        if (PointTest.X() == getLeft())
            *pLeft = true;
        else
        {
            bool bRun = true;
            while ((PointTest != pRect->TopLeft()) && bRun)
            {
                PointTest = ClassLib::TPoint(PointTest.X(), PointTest.Y() - 1);
                if (getColor(PointTest, &bExist) != *pBgColor)
                {
                    edgeColor = getColor(PointTest, &bExist);
                    if ((getColor(PointTest.X()-1, PointTest.Y()-1, &bExist) == edgeColor) ||
                        (getColor(PointTest.X()-1, PointTest.Y(),   &bExist) == edgeColor) ||
                        (getColor(PointTest.X()-1, PointTest.Y()+1, &bExist) == edgeColor))
                    {
                        for (int j = pRect->Top(); (j <= pRect->Bottom()) && bRun; j++)
                            if (getColor(PointTest.X()-1, j, &bExist) != edgeColor)
                                bRun = false;
                    }
                }
            }
            if (bRun)
                *pLeft = true;
        }
    }

    return;
}

NS_CLASSLIB::TColor
NSBitmap::getColor(NS_CLASSLIB::TPoint pt, bool* pExist)
{
    return getColor(pt.X(), pt.Y(), pExist);
}

NS_CLASSLIB::TColor
NSBitmap::getColor(int X, int Y, bool* pExist)
{
    int iX = X;
    int iY = Y;

    // DIB: "bottom-up" ou "top-down"

    if (pBitmap->Height() > 0)
        iY = pBitmap->Height() - 1 - Y;

    // 1-Bit format (monochrome)
    //
    // Monochrome bitmaps are essentially two colors (usually, but not always,
    // black and white) and will return biBitCount == 1. They store on disk to
    // such small sizes because each byte of the bitmapped data block can
    // represent eight pixels. The color palette for the image must contain two
    // entries, but they don't have to be black and white. (However, be aware
    // that most applications which see a biBitCount == 1 when evaluating the
    // header will skip the palette completely and assume that the image is
    // black and white.)
    // Each bit in the bitmap array represents a pixel. If the bit is clear, the
    // pixel is displayed with the color of the first entry in the bmciColors
    // table. If the bit is set, the pixel has the color of the second entry in
    // the table.

    // 4 bit format (VGA or 16 color)
    //
    // In this case each byte of image data represents two pixels
    // (biBitCount == 4). Bits represent the color of the pixel in descending
    // order; i.e. the first pixel is stored in bits 7-4 of the byte, the
    // second pixel in bits 3-0. The four bits specify the number of the color
    // palette entry (color 0 through color 15).

    // 8 bit format (256 colors)
    //
    // This format offers a maximum of 256 colors, and the bmciColors member
    // contains 256 entries. In this case, each byte in the image's bitmap data
    // array represents a single pixel. The value of that byte is the number of
    // the color palette entry to be used (from 0 to 255).

    // 24 bit format (TrueColor)
    //
    // These bitmaps can have a maximum of 2^24 colors (biBitCount == 24).
    // The color palette array in a TrueColor image is actually empty. Each
    // three-byte sequence in the bitmap data array represents the relative
    // intensities of the three primary hues of a pixel. The hues are described
    // as values ranging from 0 to 255 and are stored in the three bytes in the
    // order Blue, Green and Red. This is an important distinction, because most
    // references to colors in Windows use the opposite order: Red/Green/Blue,
    // so think "BGR" when working with TrueColor images instead of "RGB".
    // A color palette can be specified to accelerate the drawing process for
    // Windows, in which case biClrUsed will not be 0. But as you can see, it's
    // not needed, since the pixel data itself contains the color information.

    // 32 bit format
    //
    // Like TrueColor, 32 bit images have a maximum of 2^24 colors
    // (biBitCount == 24). Once again, the color palette array is empty. And
    // once again, each three-byte sequence in the bitmap array represents the
    // relative intensities of the blue, green and red hues, and there is one
    // unused byte. A color palette can be specified in addition in order to
    // accelerate the character process for Windows, biClrUsed is not 0 in this
    // case.

    uint8 HUGE* pPixel = (uint8 HUGE*) pBitmap->PixelPtr(iX, iY);

    if (!pPixel)
    {
        *pExist = false;
        return NS_CLASSLIB::TColor::White;
    }
    *pExist = true;

    int iBitsPix = pBitmap->BitsPixel();

    int iBlue   = 0;
    int iGreen  = 0;
    int iRed    = 0;

    int iLeftByte;
    int iRightByte;

    switch (iBitsPix)
    {
        case  2 : break;
        case  4 : break;

        case  8 : return pBitmap->GetColor(*pPixel);

        case 16 :
            iLeftByte = *(pPixel++);
            iRightByte = *pPixel;
            iBlue  = iLeftByte & 0x0F;
            iGreen = (iLeftByte & 0xF0) / 16;
            iRed   = iRightByte & 0x0F;
            return TColor(iRed, iGreen, iBlue);

        case 24 :
        case 32 :
            iBlue  = *(pPixel++);
            iGreen = *(pPixel++);
            iRed   = *pPixel;
            return TColor(iRed, iGreen, iBlue);
    }

    return NS_CLASSLIB::TColor::White;
}

NSBitmap&
NSBitmap::operator=(NSBitmap src)
{
try
{
	if (this == &src)
		return *this ;

	if (pBitmap)
		delete pBitmap ;

	if (src.pBitmap)
		pBitmap = new TDib(*(src.pBitmap)) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSBitmap = operator.", standardError, 0) ;
	return *this ;
}
}

// --------------------------------------------------------------------------
// -------------------------- METHODES DE NSOCRbloc -------------------------
// --------------------------------------------------------------------------

NSOCRbloc::NSOCRbloc(NS_CLASSLIB::TRect Bloc)
{
    BlocRect = Bloc;
    sTexte   = "";
    bgColor  = NS_CLASSLIB::TColor::White;
    fgColor  = NS_CLASSLIB::TColor::Black;
}

NSOCRbloc::NSOCRbloc(NS_CLASSLIB::TRect Bloc,   NS_CLASSLIB::TColor bColor,
                                                NS_CLASSLIB::TColor fColor)
{
    BlocRect = Bloc;
    sTexte   = "";
    bgColor  = bColor;
    fgColor  = fColor;
}

NSOCRbloc::NSOCRbloc(NSOCRbloc& rv)
{
    BlocRect = rv.BlocRect;
    sTexte   = rv.sTexte;
    bgColor  = rv.bgColor;
    fgColor  = rv.fgColor;
}

NSOCRbloc::~NSOCRbloc()
{
}

NSOCRbloc&
NSOCRbloc::operator=(NSOCRbloc src)
{
  if (this == &src)
		return *this ;

	BlocRect = src.BlocRect;
	sTexte   = src.sTexte;
	bgColor  = src.bgColor;
	fgColor  = src.fgColor;

	return *this ;
}

// --------------------------------------------------------------------------
// ------------------------ METHODES DE OCRBlocsArray -----------------------
// --------------------------------------------------------------------------

OCRBlocsArray::OCRBlocsArray(OCRBlocsArray& rv)
              :OCRArray()
{
try
{
    if (!(rv.empty()))
        for (blocIter i = rv.begin(); i != rv.end(); i++)
            push_back(new NSOCRbloc(*(*i))) ;
}
catch (...)
{
    erreur("Exception OCRBlocsArray copy ctor.", standardError, 0) ;
}
}

void
OCRBlocsArray::vider()
{
    if (empty())
        return;

    for (blocIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

OCRBlocsArray::~OCRBlocsArray()
{
	vider();
}

OCRBlocsArray&
OCRBlocsArray::operator=(OCRBlocsArray src)
{
try
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
  	for (blocIter i = src.begin(); i != src.end(); i++)
    	push_back(new NSOCRbloc(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception OCRBlocsArray = operator.", standardError, 0) ;
	return *this ;
}
}

#endif // #ifndef _ENTERPRISE_DLL

#endif

