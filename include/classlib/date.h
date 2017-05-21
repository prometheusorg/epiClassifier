//----------------------------------------------------------------------------
// Borland Class Library
// Copyright (c) 1993, 1998 by Borland International, All Rights Reserved
//
//$Revision:   1.8  $
//
//----------------------------------------------------------------------------
#if !defined(CLASSLIB_DATE_H)
#define CLASSLIB_DATE_H

#include <iosfwd>
#include <string>
using std::ostream;
using std::istream;
using std::string;

namespace ClassLib {

class TTime ;
// class _BIDSCLASS ipstream;
// class _BIDSCLASS opstream;

typedef unsigned DayTy;
typedef unsigned MonthTy;
typedef unsigned YearTy;
typedef unsigned long JulTy;

typedef char     _TCHAR ;
typedef unsigned _TUCHAR ;

static const JulTy jul1901 = 2415386L;  // Julian day for 1/1/1901

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  class TDate                                                           */
/*                                                                        */
/*------------------------------------------------------------------------*/

class TDate
{

public:

    enum HowToPrint
        {
        Normal,
        Terse,
        Numbers,
        EuropeanNumbers,
        European
        };

    // Construct a TDate with the current date

    TDate();

    // Construct a TDate with a given day of the year and a given year.
    // The base date for this computation is Dec. 31 of the previous year.
    // If year == 0, Construct a TDate with Jan. 1, 1901 as the "day zero".
    // i.e., TDate(-1,0) = Dec. 31, 1900 and TDate(1,0) = Jan. 2, 1901.

    TDate( DayTy day, YearTy year );

    //  Construct a TDate for the given day, month, and year.
    TDate( DayTy, const _TCHAR * month, YearTy );
    TDate( DayTy, MonthTy, YearTy );
    // TDate( istream & s );      // Read date from stream.
    TDate( const TTime & );    // Construct a TDate from a TTime

    string AsString() const;
    int Between( const TDate & d1, const TDate & d2 ) const;
    int CompareTo( const TDate & ) const;
    DayTy Day() const;            // 1-365
    DayTy DayOfMonth() const;     // 1-31
    DayTy FirstDayOfMonth() const;
    DayTy FirstDayOfMonth( MonthTy ) const;
    unsigned Hash() const;
    int IsValid() const;
    int Leap()  const;                             // leap year?
    TDate Max( const TDate & dt ) const;
    TDate Min( const TDate & dt ) const;
    MonthTy Month() const;
    const _TCHAR *NameOfDay() const;
    const _TCHAR *NameOfMonth() const;
    TDate Previous( const _TCHAR *dayName ) const;  // Return date of previous dayName
    TDate Previous( DayTy ) const;                // Same as above, but use day of week
    DayTy WeekDay() const;
    YearTy Year() const;

    // Date comparisons:
    int operator <  ( const TDate & date ) const;
    int operator <= ( const TDate & date ) const;
    int operator >  ( const TDate & date ) const;
    int operator >= ( const TDate & date ) const;
    int operator == ( const TDate & date ) const;
    int operator != ( const TDate & date ) const;

    // Arithmetic operators:
    JulTy operator - ( const TDate & dt ) const;
    friend TDate operator + ( const TDate & dt, int dd );
    friend TDate operator + ( int dd, const TDate & dt );
    friend TDate operator - ( const TDate & dt, int dd );
    void operator ++ ();
    void operator -- ();
    void operator += ( int dd );
    void operator -= ( int dd );

    // Read or write dates:
    // friend ostream _BIDSFAR & _BIDSFUNC operator << ( ostream _BIDSFAR & s, const TDate _BIDSFAR & d );
    // friend istream _BIDSFAR & _BIDSFUNC operator >> ( istream _BIDSFAR & s, TDate _BIDSFAR & d );

    // Read or write dates on persistent streams
    // friend opstream _BIDSFAR & _BIDSFUNC operator << ( opstream _BIDSFAR & s, const TDate _BIDSFAR & d );
    // friend ipstream _BIDSFAR & _BIDSFUNC operator >> ( ipstream _BIDSFAR & s, TDate _BIDSFAR & d );

    // Static member functions:
    static const _TCHAR *DayName( DayTy weekDayNumber );
    static DayTy DayOfWeek( const _TCHAR * dayName );
    static int DayWithinMonth( MonthTy, DayTy, YearTy );
    static DayTy DaysInYear( YearTy );
    static MonthTy IndexOfMonth( const _TCHAR * monthName );
    static JulTy Jday( MonthTy, DayTy, YearTy );
    static int LeapYear( YearTy year );
    static const _TCHAR *MonthName( MonthTy monthNumber );
    static HowToPrint SetPrintOption( HowToPrint h );
    
    // A YearTy value in the range [0..MaxShortYear) will have BaseYear
    // added to it when converting a year into a Julian day number (the
    // internal representation of TDate).  This affects the TDate
    // constructors and TDate::Jday.
    static YearTy &BaseYear();           // default: 1900
    static YearTy &MaxShortYear();       // default: 999

protected:

    static int AssertWeekDayNumber( DayTy d );
    static int AssertIndexOfMonth( MonthTy m );

private:

    JulTy Julnum;                   // Julian Day Number (Not same as Julian date.)
    static HowToPrint PrintOption;  // Printing with different formats

    // void ParseFrom( istream _BIDSFAR & );     // Reading dates
    void Mdy( MonthTy &, DayTy &, YearTy & ) const;
    TDate( JulTy j );

};

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  TDate inline functions                                                */
/*                                                                        */
/*------------------------------------------------------------------------*/

/*
inline TDate::TDate( istream& s )
{
    ParseFrom(s);
}
*/

inline int TDate::Between( const TDate& d1, const TDate& d2 ) const
{
    return Julnum >= d1.Julnum && Julnum <= d2.Julnum;
}

inline DayTy TDate::FirstDayOfMonth() const
{
    return FirstDayOfMonth(Month());
}

inline int TDate::IsValid() const
{
    return Julnum>0;
}

inline int TDate::Leap() const
{
    return LeapYear(Year());
}

inline const _TCHAR *TDate::NameOfDay() const
{
    return DayName(WeekDay());
}

inline const _TCHAR *TDate::NameOfMonth() const
{
    return MonthName(Month());
}

inline int TDate::operator < ( const TDate& date ) const
{
    return Julnum < date.Julnum;
}

inline int TDate::operator <= ( const TDate& date ) const
{
    return Julnum <= date.Julnum;
}

inline int TDate::operator > ( const TDate& date ) const
{
    return Julnum > date.Julnum;
}

inline int TDate::operator >= ( const TDate& date ) const
{
    return Julnum >= date.Julnum;
}

inline int TDate::operator == ( const TDate& date ) const
{
    return Julnum == date.Julnum;
}

inline int TDate::operator != ( const TDate& date ) const
{
    return Julnum != date.Julnum;
}

inline JulTy TDate::operator - ( const TDate& dt ) const
{
    return Julnum - dt.Julnum;
}

inline TDate operator + ( const TDate& dt, int dd )
{
    return TDate(dt.Julnum + dd);
}

inline TDate operator + ( int dd, const TDate& dt )
{
    return TDate(dt.Julnum + dd);
}

inline TDate operator - ( const TDate& dt, int dd )
{
    return TDate(dt.Julnum - dd);
}

inline void TDate::operator ++ ()
{
    Julnum += 1;
}

inline void TDate::operator -- ()
{
    Julnum -= 1;
}

inline void TDate::operator += ( int dd )
{
    Julnum += dd;
}

inline void TDate::operator -= ( int dd )
{
    Julnum -= dd;
}

/*
inline istream& _BIDSFUNC operator >> ( istream& s, TDate& d )
{
    d.ParseFrom(s);
    return s;
}
*/

inline int TDate::AssertWeekDayNumber( DayTy d )
{
    return d>=1 && d<=7;
}

inline int TDate::AssertIndexOfMonth( MonthTy m )
{
    return m>=1 && m<=12;
}

inline TDate::TDate( JulTy j )
{
    Julnum = j;
}

inline unsigned HashValue( TDate & d )
{
    return d.Hash();
}

}   // namespace ClassLib

#endif  // CLASSLIB_DATE_H
