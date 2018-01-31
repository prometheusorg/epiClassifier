//----------------------------------------------------------------------------
// Borland Class Library
// Copyright (c) 1993, 1998 by Borland International, All Rights Reserved
//
//$Revision:   1.4  $
//
//----------------------------------------------------------------------------
#if !defined(CLASSLIB_TIME_H)
#define CLASSLIB_TIME_H

#if !defined(CLASSLIB_DATE_H)
# include <classlib/date.h>
#endif
#if !defined(__TIME_H)
# include <time.h>
#endif

#include <string>
#include <iosfwd>
using std::istream;
using std::ostream;

namespace ClassLib {

// class _BIDSCLASS ipstream;
// class _BIDSCLASS opstream;

typedef unsigned HourTy;
typedef unsigned MinuteTy;
typedef unsigned SecondTy;
typedef unsigned long ClockTy;

static const unsigned long secFrom_Jan_1_1901_to_Jan_1_1970 = 2177452800L;

class TTime
{

public:

    static int  _daylight;
    static long _timezone;

    friend TDate::TDate( const TTime & );

    TTime();                  // Current time
    TTime( ClockTy s );       // Seconds since Jan 1, 1901.
    TTime( HourTy h, MinuteTy m, SecondTy s = 0 );
                                // Specified time and today's date
    TTime( const TDate &, HourTy h=0, MinuteTy m=0, SecondTy s=0 );
                                // Given date and time

    string AsString() const;
    int CompareTo( const TTime & ) const;
    unsigned Hash() const;
    HourTy Hour() const;        // hour: local time
    HourTy HourGMT() const;     // hour: GMT
    int IsDST() const;
    int IsValid() const;
    TTime Max( const TTime & t ) const;
    TTime Min( const TTime & t ) const;
    MinuteTy Minute() const;    // minute: local time
    MinuteTy MinuteGMT() const; // minute: GMT
    SecondTy Second() const;    // second: local time or GMT
    ClockTy Seconds() const;

    // Write times:
    // friend ostream & _BIDSFUNC operator << ( ostream _BIDSFAR &, const TTime _BIDSFAR & );

    // Read or write times on persistent streams
    // friend opstream _BIDSFAR & _BIDSFUNC operator << ( opstream _BIDSFAR & s, const TTime _BIDSFAR & d );
    // friend ipstream _BIDSFAR & _BIDSFUNC operator >> ( ipstream _BIDSFAR & s, TTime _BIDSFAR & d );

    // Boolean operators.
    int operator <  ( const TTime & t ) const;
    int operator <= ( const TTime & t ) const;
    int operator >  ( const TTime & t ) const;
    int operator >= ( const TTime & t ) const;
    int operator == ( const TTime & t ) const;
    int operator != ( const TTime & t ) const;
    int Between( const TTime & a, const TTime & b ) const;

    // Add or subtract seconds.
    friend TTime operator + ( const TTime & t, long s );
    friend TTime operator + ( long s, const TTime & t );
    friend TTime operator - ( const TTime & t, long s );
    friend TTime operator - ( long s, const TTime & t );
    void operator++();
    void operator--();
    void operator+=(long s);
    void operator-=(long s);

    // Static member functions:
    static TTime BeginDST( unsigned year ); // Start of DST for given year.
    static TTime EndDST( unsigned year );   // End of DST for given year.
    static int PrintDate( int );    // Whether to include date when printing time

protected:

    static int AssertDate( const TDate & );
    static const TDate RefDate;
    static const TDate MaxDate;

private:

    ClockTy Sec;        // Seconds since 1/1/1901.
    static int PrintDateFlag;  // True to print date along with time.

    ClockTy LocalSecs() const;
    static TTime BuildLocal( const TDate &, HourTy );

};

inline TTime::TTime( ClockTy s )
{
    Sec = s;
}

inline int TTime::IsValid() const
{
    return Sec > 0;
}

inline ClockTy TTime::Seconds() const
{
    return Sec;
}

inline int TTime::operator <  ( const TTime& t ) const
{
    return Sec < t.Sec;
}

inline int TTime::operator <= ( const TTime& t ) const
{
    return Sec <= t.Sec;
}

inline int TTime::operator >  ( const TTime& t ) const
{
    return Sec > t.Sec;
}

inline int TTime::operator >= ( const TTime& t ) const
{
    return Sec >= t.Sec;
}

inline int TTime::operator == ( const TTime& t ) const
{
    return Sec == t.Sec;
}

inline int TTime::operator != ( const TTime& t ) const
{
    return Sec != t.Sec;
}

inline int TTime::Between( const TTime& a, const TTime& b ) const
{
    return *this >= a && *this <= b;
}

inline TTime operator + ( const TTime& t, long s )
{
    return TTime(t.Sec+s);
}

inline TTime operator + ( long s, const TTime& t )
{
    return TTime(t.Sec+s);
}

inline TTime operator - ( const TTime& t, long s )
{
    return TTime(t.Sec-s);
}

inline TTime operator - ( long s, const TTime& t )
{
    return TTime(t.Sec-s);
}

inline void TTime::operator++()
{
    Sec += 1;
}

inline void TTime::operator--()
{
    Sec -= 1;
}

inline void TTime::operator+=(long s)
{
    Sec += s;
}

inline void TTime::operator-=(long s)
{
    Sec -= s;
}

inline unsigned HashValue( TTime & t )
{
    return t.Hash();
}

}     // namespace ClassLib

#endif  // CLASSLIB_TIME_H
