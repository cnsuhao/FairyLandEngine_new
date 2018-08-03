//============================================================================
// Time.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Time Implementation
//----------------------------------------------------------------------------

Time::Time( )
{
	mYear			= 0;
	mMonth			= 0;
	mDayOfWeek		= 0;
	mDay			= 0;
	mHour			= 0;
	mMinute			= 0;
	mSecond			= 0;
	mMilliseconds	= 0;
}

Time::Time( StringPtr string )
{
	mYear			= 0;
	mMonth			= 0;
	mDayOfWeek		= 0;
	mDay			= 0;
	mHour			= 0;
	mMinute			= 0;
	mSecond			= 0;
	mMilliseconds	= 0;

	// Year.
	mYear = Long( string );

	_dword index = -1;

	// Month.
	if ( ( index = string.SearchL2R( '-' ) ) == -1 )
		return;

	string = StringPtr( (const _char*) string + index + 1 );
	mMonth = Long( string );

	// Day.
	if ( ( index = string.SearchL2R( '-' ) ) == -1 )
		return;

	string = StringPtr( (const _char*) string + index + 1 );
	mDay   = Long( string );

	// Hour.
	if ( ( index = string.SearchL2R( ' ' ) ) == -1 )
		return;

	string = StringPtr( (const _char*) string + index + 1 );
	mHour  = Long( string );

	// Minute.
	if ( ( index = string.SearchL2R( ':' ) ) == -1 )
		return;

	string  = StringPtr( (const _char*) string + index + 1 );
	mMinute = Long( string );

	// Second.
	if ( ( index = string.SearchL2R( ':' ) ) == -1 )
		return;

	string  = StringPtr( (const _char*) string + index + 1 );
	mSecond = Long( string );
}

_bool Time::operator == ( const Time& time ) const
{
	return mYear == time.mYear && mMonth == time.mMonth && mDay == time.mDay && mHour == time.mHour &&
		mMinute == time.mMinute && mSecond == time.mSecond && mMilliseconds == time.mMilliseconds;
}

_bool Time::operator != ( const Time& time ) const
{
	return mYear != time.mYear || mMonth != time.mMonth || mDay != time.mDay || mHour != time.mHour ||
		mMinute != time.mMinute || mSecond != time.mSecond || mMilliseconds != time.mMilliseconds;
}

_bool Time::operator > ( const Time& time ) const
{
	// If the year members of the two times are different, then can decide
	// the compare result, otherwise continue compare with other members.
	if ( mYear > time.mYear )
		return _true;
	if ( mYear < time.mYear )
		return _false;

	// If the month members of the two times are different, then can decide
	// the compare result, otherwise continue compare with other members.
	if ( mMonth > time.mMonth )
		return _true;
	if ( mMonth < time.mMonth )
		return _false;

	// If the day members of the two times are different, then can decide
	// the compare result, otherwise continue compare with other members.
	if ( mDay > time.mDay )
		return _true;
	if ( mDay < time.mDay )
		return _false;

	// If the hour members of the two times are different, then can decide
	// the compare result, otherwise continue compare with other members.
	if ( mHour > time.mHour )
		return _true;
	if ( mHour < time.mHour )
		return _false;

	// If the minute members of the two times are different, then can decide
	// the compare result, otherwise continue compare with other members.
	if ( mMinute > time.mMinute )
		return _true;
	if ( mMinute < time.mMinute )
		return _false;

	// If the second members of the two times are different, then can decide
	// the compare result, otherwise continue compare with other members.
	if ( mSecond > time.mSecond )
		return _true;
	if ( mSecond < time.mSecond )
		return _false;

	// If the milliseconds members of the two times are different, then can decide
	// the compare result, otherwise continue compare with other members.
	if ( mMilliseconds > time.mMilliseconds )
		return _true;
	if ( mMilliseconds < time.mMilliseconds )
		return _false;

	return _false;
}

_bool Time::operator < ( const Time& time ) const
{
	// If the year members of the two times are different, then can decide
	// the compare result, otherwise continue compare with other members.
	if ( mYear < time.mYear )
		return _true;
	if ( mYear > time.mYear )
		return _false;

	// If the month members of the two times are different, then can decide
	// the compare result, otherwise continue compare with other members.
	if ( mMonth < time.mMonth )
		return _true;
	if ( mMonth > time.mMonth )
		return _false;

	// If the day members of the two times are different, then can decide
	// the compare result, otherwise continue compare with other members.
	if ( mDay < time.mDay )
		return _true;
	if ( mDay > time.mDay )
		return _false;

	// If the hour members of the two times are different, then can decide
	// the compare result, otherwise continue compare with other members.
	if ( mHour < time.mHour )
		return _true;
	if ( mHour > time.mHour )
		return _false;

	// If the minute members of the two times are different, then can decide
	// the compare result, otherwise continue compare with other members.
	if ( mMinute < time.mMinute )
		return _true;
	if ( mMinute > time.mMinute )
		return _false;

	// If the second members of the two times are different, then can decide
	// the compare result, otherwise continue compare with other members.
	if ( mSecond < time.mSecond )
		return _true;
	if ( mSecond > time.mSecond )
		return _false;

	// If the milliseconds members of the two times are different, then can decide
	// the compare result, otherwise continue compare with other members.
	if ( mMilliseconds < time.mMilliseconds )
		return _true;
	if ( mMilliseconds > time.mMilliseconds )
		return _false;

	return _false;
}

_bool Time::operator >= ( const Time& time ) const
{
	return mYear >= time.mYear && mMonth >= time.mMonth && mDay >= time.mDay && mHour >= time.mHour &&
		mMinute >= time.mMinute && mSecond >= time.mSecond && mMilliseconds >= time.mMilliseconds;
}

_bool Time::operator <= ( const Time& time ) const
{
	return mYear <= time.mYear && mMonth <= time.mMonth && mDay <= time.mDay && mHour <= time.mHour &&
		mMinute <= time.mMinute && mSecond <= time.mSecond && mMilliseconds <= time.mMilliseconds;
}

String Time::ToString( ) const
{
	// Format time to string as "YYYY-MM-DD HH:MM:SS(MMMM)".
	return StringFormatter::FormatString( L"%.4d-%.2d-%.2d %.2d-%.2d-%.2d(%.4d)",
		mYear, mMonth, mDay, mHour, mMinute, mSecond, mMilliseconds );
}