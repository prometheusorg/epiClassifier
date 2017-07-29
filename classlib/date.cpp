//----------------------------------------------------------------------------
// Borland Class Library
// Copyright (c) 1993, 1998 by Borland International, All Rights Reserved
//
//$Revision:   1.5  $
//
// TDate class implementation
//----------------------------------------------------------------------------
// #include <classlib/pch.h>
#include "../include/classlib/date.h"
// #include <services/cstring.h>
// #include <services/checks.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
// #include <tchar.h>

using namespace ClassLib ;

/****************************************************************
 *                                                              *
 *                      static constants                        *
 *                                                              *
 ****************************************************************/

static const _TUCHAR DaysInMonth[12] =
    { 31,28,31,30,31,30,31,31,30,31,30,31 };
static const DayTy FirstDayOfEachMonth[12] =
    { 1,32,60,91,121,152,182,213,244,274,305,335 };
static const _TCHAR *MonthNames[12] =
    { "January","February","March","April","May","June",
      "July","August","September","October","November","December" };
static const _TCHAR *UCMonthNames[12] =
    { "JANUARY","FEBRUARY","MARCH","APRIL","MAY","JUNE",
      "JULY","AUGUST","SEPTEMBER","OCTOBER","NOVEMBER","DECEMBER" };
static const _TCHAR *WeekDayNames[7] =
    { "Monday","Tuesday","Wednesday",
      "Thursday","Friday","Saturday","Sunday" };
static const _TCHAR *UCWeekDayNames[7] =
    { "MONDAY","TUESDAY","WEDNESDAY",
      "THURSDAY","FRIDAY","SATURDAY","SUNDAY" };

static int FindMatch( const _TCHAR *str, const _TCHAR**candidates, int icand );

/***************************************************************************/

//      constructors

/***************************************************************************/

//
// Construct a TDate for today's date.
//
TDate::TDate()
{
    long clk = time(0);
    struct tm *now = localtime(&clk);
    Julnum = Jday(now->tm_mon+1, now->tm_mday, now->tm_year+1900);
}

//
// Construct a TDate with a given day of the year and a given year.  The
// base date for this computation is Dec. 31 of the previous year.  If
// year == 0, Construct a TDate with Jan. 1, 1901 as the "day zero".
// i.e., TDate(-1) = Dec. 31, 1900 and TDate(1) = Jan. 2, 1901.
//
TDate::TDate(DayTy day, YearTy year)
{
    if( year )
        Julnum = Jday( 12, 31, year-1 ) + (JulTy)day;
    else
        Julnum = jul1901                + (JulTy)day;
}

//
//   Construct a TDate for the given day, monthName, and year.
//
TDate::TDate( DayTy day, const _TCHAR *monthName, YearTy year )
{
    Julnum = Jday( IndexOfMonth(monthName), day, year );
}

//
//   Construct a TDate for the given day, month, and year.
//
TDate::TDate( DayTy day, MonthTy month, YearTy year )
{
    Julnum = Jday( month, day, year );
}

/***************************************************************************/

//                      static member functions

/***************************************************************************/

//
// Returns a string name for the weekday number.
// Monday == 1, ... , Sunday == 7
// Return 0 for weekday number out of range
//
const _TCHAR *TDate::DayName( DayTy weekDayNumber )
{
    return AssertWeekDayNumber(weekDayNumber) ? WeekDayNames[weekDayNumber-1] : 0;
}

//
// Return the number, 1-7, of the day of the week named nameOfDay.
// Return 0 if name doesn't match.
//
DayTy TDate::DayOfWeek( const _TCHAR *nameOfDay )
{
    return (DayTy)(FindMatch( nameOfDay, UCWeekDayNames, 7 )+1);
}

//
// Is a day (1-31) within a given month?
//
int TDate::DayWithinMonth( MonthTy month, DayTy day, YearTy year )
{
    if( day <= 0 || !AssertIndexOfMonth(month) )
        return 0;
    unsigned d = DaysInMonth[month-1];
    if( LeapYear(year) && month == 2 )
        d++;
    return day <= d;
}

//
// How many days are in the given YearTy year?
//
DayTy TDate::DaysInYear( YearTy year )
{
    return LeapYear(year) ? 366 : 365;
}

//
// Returns the number, 1-12, of the month named nameOfMonth.
// Return 0 for no match.
//
MonthTy TDate::IndexOfMonth( const _TCHAR *nameOfMonth )
{
    return (MonthTy)(FindMatch( nameOfMonth, UCMonthNames, 12 )+1);
}


YearTy &TDate::BaseYear()
{
  static YearTy base = 1900;
  return base;
}

YearTy &TDate::MaxShortYear()
{
  static YearTy m = 999;
  return m;
}

//
// Convert Gregorian calendar date to the corresponding Julian day
// number j.  Algorithm 199 from Communications of the ACM, Volume 6, No.
// 8, (Aug. 1963), p. 444.  Gregorian calendar started on Sep. 14, 1752.
// This function not valid before that.
// Returns 0 if the date is invalid.
//

JulTy TDate::Jday( MonthTy m, DayTy d, YearTy y )
{
	const unsigned long num_days_in_four_jul_years = 1461;
	const unsigned long num_jul_days_to_zero = 1721119L;
	const unsigned long num_jul_days_in_century = 146097L;

	unsigned long c, ya;
	if( y <= MaxShortYear() )
		y += BaseYear();
	if( !DayWithinMonth(m, d, y) )
		return (JulTy)0;

	if( m > 2 )
		m -= 3;
	else
		{
		m += 9;
		y--;
		}

	c = y / 100;
	ya = y - 100*c;
	return ((num_jul_days_in_century*c)>>2) + ((num_days_in_four_jul_years*ya)>>2) + (153*m + 2)/5 + d + num_jul_days_to_zero;
//	return ((146097L*c)>>2) + ((1461*ya)>>2) + (153*m + 2)/5 + d + 1721119L;
}

//
// Algorithm from K & R, "The C Programming Language", 1st ed.
//
int TDate::LeapYear( YearTy year )
{
    return (year&3) == 0 && year%100 != 0 || year % 400 == 0;
}

//
// Returns a string name for the month number.
// Return 0 if invalid month number.
//
const _TCHAR *TDate::MonthName( MonthTy monthNumber )
{
    return AssertIndexOfMonth(monthNumber) ? MonthNames[monthNumber-1] : 0;
}

//
// Return index of case-insensitive match; -1 if no match.
//
static int FindMatch( const _TCHAR *str, const _TCHAR**candidates, int icand )
{
    // unsigned len = _tcslen(str);
  unsigned len = strlen(str) ;

  while(icand--)
  {
    // if( _tcsnicmp(str, candidates[icand], len) == 0)
    if( strncasecmp(str, candidates[icand], len) == 0)
      break;
  }
  return icand;
}

/****************************************************************
 *                                                              *
 *                      Member functions                        *
 *                                                              *
 ****************************************************************/

//
// Compare function:
//
int TDate::CompareTo( const TDate &d ) const
{
    if( Julnum < d.Julnum )
        return -1;
    else if( Julnum > d.Julnum )
        return 1;
    else
        return 0;
}

//
//
//
DayTy TDate::Day() const
{
    return DayTy(Julnum - Jday( 12, 31, Year()-1 ));
}

//
// Returns the day of the month of this TDate.
//
DayTy TDate::DayOfMonth() const
{
    MonthTy m; DayTy d; YearTy y;
    Mdy( m, d, y );
    return d;
}

//
// Return the number of the first day of a given month
// Return 0 if "month" is outside of the range 1 through 12, inclusive.
//
DayTy TDate::FirstDayOfMonth( MonthTy month ) const
{
    if ( !AssertIndexOfMonth(month) )
        return 0;
    unsigned firstDay = FirstDayOfEachMonth[month-1];
    if (month > 2 && Leap())
        firstDay++;
    return firstDay;
}

unsigned TDate::Hash() const
{
    return (unsigned)Julnum;
}

//
// Convert a Julian day number to its corresponding Gregorian calendar
// date.  Algorithm 199 from Communications of the ACM, Volume 6, No. 8,
// (Aug. 1963), p. 444.  Gregorian calendar started on Sep. 14, 1752.
// This function not valid before that.
//
void TDate::Mdy( MonthTy & m, DayTy & D, YearTy & y ) const
{
    unsigned long d;
    JulTy j = Julnum - 1721119L;
    y = (YearTy) (((j<<2) - 1) / 146097L);
    j = (j<<2) - 1 - 146097L*y;
    d = (j>>2);
    j = ((d<<2) + 3) / 1461;
    d = (d<<2) + 3 - 1461*j;
    d = (d + 4)>>2;
    m = (MonthTy)(5*d - 3)/153;
    d = 5*d - 3 - 153*m;
    D = (DayTy)((d + 5)/5);
    y = (YearTy)(100*y + j);

    if( m < 10 )
        m += 3;
    else
        {
        m -= 9;
        y++;
        }
}

//
//
//
TDate TDate::Max( const TDate & dt ) const
{
    return dt.Julnum > Julnum ? dt : *this;
}

//
//
//
TDate TDate::Min( const TDate & dt ) const
{
    return dt.Julnum < Julnum ? dt : *this;
}

//
// Returns the month of this TDate.
//
MonthTy TDate::Month() const
{
    MonthTy m; DayTy d; YearTy y;
    Mdy(m, d, y);
    return m;
}

//
//
//
TDate TDate::Previous( const _TCHAR *dayName) const
{
    return Previous( DayOfWeek(dayName) );
}

//
//
//
TDate TDate::Previous( DayTy desiredDayOfWeek ) const
{
    //    Renumber the desired and current day of week to start at 0 (Monday)
    //    and end at 6 (Sunday).

    desiredDayOfWeek--;
    DayTy thisDayOfWeek = WeekDay() - 1;
    JulTy j = Julnum;

    //    Have to determine how many days difference from current day back to
    //    desired, if any.  Special calculation under the 'if' statement to
    //    effect the wraparound counting from Monday (0) back to Sunday (6).

    if( desiredDayOfWeek > thisDayOfWeek )
        thisDayOfWeek += 7 - desiredDayOfWeek;
    else
        thisDayOfWeek -= desiredDayOfWeek;
    j -= thisDayOfWeek; // Adjust j to set it at the desired day of week.
    return  TDate(j);
}

DayTy TDate::WeekDay() const
{
    return DayTy(((((Julnum+1)%7)+6)%7)+1);
}

//
// Returns the year of this TDate.
//
YearTy TDate::Year() const
{
    MonthTy m; DayTy d; YearTy y;
    Mdy(m, d, y);
    return y;
}
