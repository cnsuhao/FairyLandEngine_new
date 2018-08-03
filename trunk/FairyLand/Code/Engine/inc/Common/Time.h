//============================================================================
// Time.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Time
//----------------------------------------------------------------------------

//! This class represents a date and time using individual members.
class Time
{
public:
	//!	The current year.
	_word	mYear;
	//!	The current month, January is 1.
	_word	mMonth;
	//!	The current day of the week, Sunday is 0, Monday is 1, and so on.
	_word	mDayOfWeek;
	//!	The current day of the month.
	_word	mDay;
	//! The current hour.
	_word	mHour;
	//! The current minute.
	_word	mMinute;
	//! The current second.
	_word	mSecond;
	//! The current millisecond.
	_word	mMilliseconds;

public:
	//! Constructor, create time with all fields are zero.
	//! @param		none.
	Time( );
	//! Constructor, create time from a string.
	//! @param		string		The string contains the time, the format is "YYYY-MM-DD HH:MM:SS".
	Time( StringPtr string );

	//! Compare two times that whether they are equal.
	//! @param		time	The second time to be compare with.
	//! @return		True if they are equal, false otherwise.
	_bool operator == ( const Time& time ) const;
	//! Compare two times that whether they are not equal.
	//! @param		time	The second time to be compare with.
	//! @return		True if they are not equal, false otherwise.
	_bool operator != ( const Time& time ) const;
	//! Compare two times that whether the first one is after the second one.
	//! @param		time	The second time to be compare with.
	//! @return		True if the first one is after the second one, false otherwise.
	_bool operator >  ( const Time& time ) const;
	//! Compare two times that whether the first one is before the second one.
	//! @param		time	The second time to be compare with.
	//! @return		True if the first one is before the second one, false otherwise.
	_bool operator <  ( const Time& time ) const;
	//! Compare two times that whether the first one is equal or after the second one.
	//! @param		time	The second time to be compare with.
	//! @return		True if the first one is equal or after the second one, false otherwise.
	_bool operator >= ( const Time& time ) const;
	//! Compare two times that whether the first one is equal or before the second one.
	//! @param		time	The second time to be compare with.
	//! @return		True if the first one is equal or before the second one, false otherwise.
	_bool operator <= ( const Time& time ) const;

	//! Convert time to string, the format is "YYYY-MM-DD HH-MM-SS-MMMM".
	//! @param		none
	//! @return		The formated string.
	String ToString( ) const;
};

};