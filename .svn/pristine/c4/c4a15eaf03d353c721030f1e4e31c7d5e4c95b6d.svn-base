//============================================================================
// Script.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Script
//----------------------------------------------------------------------------

Script::Script( )
{
	Stop( );
}

Script::~Script( )
{
	for ( _dword i = 0; i < mVariableArray.Number( ); i ++ )
		delete mVariableArray[i];
}

Script::ScriptVariable* Script::GetVariable( StringPtr name )
{
	for ( _dword i = 0; i < mVariableArray.Number( ); i ++ )
	{
		if ( mVariableArray[i]->GetName( ) == name )
			return mVariableArray[i];
	}

	return _null;
}

Script::ScriptFunction* Script::GetFunction( StringPtr name )
{
	for ( _dword i = 0; i < mFunctionArray.Number( ); i ++ )
	{
		if ( mFunctionArray[i].mName == name )
			return &mFunctionArray[i];
	}

	return _null;
}

Script::ScriptCallback* Script::GetCallback( StringPtr name )
{
	for ( _dword i = 0; i < mCallbackArray.Number( ); i ++ )
	{
		if ( mCallbackArray[i].mName == name )
			return &mCallbackArray[i];
	}

	return _null;
}

_bool Script::ExcuteCommand( ScriptCommand& command )
{
	if ( command.mName.CompareLowercase( L"wait" ) )
	{
		mDelayTickcount = Long( command.mParam1 );

		return _false;
	}
	else if ( command.mName.CompareLowercase( L"pause" ) )
	{
		mPaused = _true;

		return _false;
	}
	else if ( command.mName.CompareLowercase( L"resume" ) )
	{
		mPaused = _false;

		return _true;
	}
	else if ( command.mName.CompareLowercase( L"return" ) )
	{
		mCurrentFunction	= _null;
		mNextCommandIndex	= -1;

		return _false;
	}
	else if ( command.mName.CompareLowercase( L"call" ) )
	{
		if ( TriggerFunction( command.mParam1 ) == _false )
			return _false;

		return _true;
	}
	else
	{
		ScriptCallback* callback = GetCallback( command.mName );

		if ( callback == _null )
			return _false;

		if ( callback->mCallback != _null )
		{
			String parameter1 = command.mParam1, parameter2 = command.mParam2, parameter3 = command.mParam3, parameter4 = command.mParam4;

			ScriptVariable* variable1 = GetVariable( command.mParam1 );
			if ( variable1 != _null )
				parameter1 = variable1->GetString( );

			ScriptVariable* variable2 = GetVariable( command.mParam2 );
			if ( variable2 != _null )
				parameter2 = variable2->GetString( );

			ScriptVariable* variable3 = GetVariable( command.mParam3 );
			if ( variable3 != _null )
				parameter3 = variable3->GetString( );

			ScriptVariable* variable4 = GetVariable( command.mParam4 );
			if ( variable4 != _null )
				parameter4 = variable4->GetString( );

			ParseString( parameter1 );
			ParseString( parameter2 );
			ParseString( parameter3 );
			ParseString( parameter4 );

			(*callback->mCallback)( parameter1, parameter2, parameter3, parameter4 );
		}
	}

	return _true;
}

_bool Script::ExcuteFunction( ScriptFunction& function, _dword startcommand )
{
	mCurrentFunction	= &function;
	mNextCommandIndex	= startcommand;

	for ( _dword i = startcommand; i < function.mCommandArray.Number( ); i ++ )
	{
		ScriptCommand& command = function.mCommandArray[i];

		mCurrentFunction	= &function;
		mNextCommandIndex	= i + 1;

		// Process condition command.
		if ( command.mName.CompareLowercase( L"if" ) )
		{
			StringPtr compare = command.mParam2;

			_long parameter1 = Long( command.mParam1 ), parameter2 = Long( command.mParam3 );

			ScriptVariable* variable1 = GetVariable( command.mParam1 );
			if ( variable1 != _null )
				parameter1 = variable1->GetNumber( );

			ScriptVariable* variable2 = GetVariable( command.mParam3 );
			if ( variable2 != _null )
				parameter2 = variable2->GetNumber( );

			_dword nextcommand = Long( command.mParam4 );

			if ( nextcommand > 0 )
				nextcommand --;

			if ( compare == L">" )
			{
				if ( parameter1 <= parameter2 )
					mNextCommandIndex = i = nextcommand;
			}
			// Process condition command.
			else if ( compare == L"<" )
			{
				if ( parameter1 >= parameter2 )
					mNextCommandIndex = i = nextcommand;
			}
			// Process condition command.
			else if ( compare == L">=" )
			{
				if ( parameter1 < parameter2 )
					mNextCommandIndex = i = nextcommand;
			}
			// Process condition command.
			else if ( compare == L"<=" )
			{
				if ( parameter1 > parameter2 )
					mNextCommandIndex = i = nextcommand;
			}
			// Process condition command.
			else if ( compare == L"==" )
			{
				// Convert to string to compare.
				String temp1 = command.mParam1;
				String temp2 = command.mParam3;

				if ( variable1 != _null )
					temp1 = variable1->GetString( );

				if ( variable2 != _null )
					temp2 = variable2->GetString( );

				if ( temp1.CompareLowercase( temp2 ) == _false )
					mNextCommandIndex = i = nextcommand;
			}
			// Process condition command.
			else if ( compare == L"!=" )
			{
				// Convert to string to compare.
				String temp1 = command.mParam1;
				String temp2 = command.mParam3;

				if ( variable1 != _null )
					temp1 = variable1->GetString( );

				if ( variable2 != _null )
					temp2 = variable2->GetString( );

				if ( temp1.CompareLowercase( temp2 ) == _true )
					mNextCommandIndex = i = nextcommand;
			}
		}
		else
		{
			if ( ExcuteCommand( command ) == _false )
				return _false;
		}
	}

	mCurrentFunction	= _null;
	mNextCommandIndex	= -1;

	return _true;
}

_void Script::FinishContext( ParseContext& parsecontext )
{
	if ( parsecontext.mNumber != _null )
	{
		mVariableArray.Append( parsecontext.mNumber );
	}
	else if ( parsecontext.mRandomNumber != _null )
	{
		mVariableArray.Append( parsecontext.mRandomNumber );
	}
	else if ( parsecontext.mString != _null )
	{
		mVariableArray.Append( parsecontext.mString );
	}
	else if ( parsecontext.mRandomString != _null )
	{
		mVariableArray.Append( parsecontext.mRandomString );
	}
	else if ( parsecontext.mFunction != _null )
	{
		_dword index  = parsecontext.mLastIfCommand;
		_dword number = parsecontext.mFunction->mCommandArray.Number( );

		if ( index < number )
			parsecontext.mFunction->mCommandArray[ index ].mParam4 = Long( number ).ToString( );

		mFunctionArray.Append( *parsecontext.mFunction );

		delete parsecontext.mFunction;
	}

	parsecontext = ParseContext( );
}

_bool Script::LoadScript( StringPtr filename )
{
	TxtFile sptfile;

	if ( sptfile.Open( filename ) == _false )
		return _false;

	return LoadScript( sptfile );
}

_bool Script::LoadScript( TxtFile& sptfile )
{
	sptfile.SeekToBegin( );

	String keystring, value1, value2, value3, value4, line;

	ParseContext parsecontext;

	while ( sptfile.ReachEnd( ) == _false )
	{
		sptfile.SavePosition( );

		sptfile.ReadWord( keystring );
		sptfile.ReadWordInLine( value1 );
		sptfile.ReadWordInLine( value2 );
		sptfile.ReadWordInLine( value3 );
		sptfile.ReadWordInLine( value4 );

		sptfile.LoadPosition( );

		sptfile.ReadLine( line );

		if ( keystring.SearchL2R( L"//" ) == 0 )
			continue;

		if ( keystring.CompareLowercase( L"number" ) )
		{
			FinishContext( parsecontext );

			parsecontext.mNumber = new ScriptNumber( value1 );
		}
		else if ( keystring.CompareLowercase( L"randomnumber" ) )
		{
			FinishContext( parsecontext );

			parsecontext.mRandomNumber = new ScriptRandomNumber( value1 );
		}
		else if ( keystring.CompareLowercase( L"string" ) )
		{
			FinishContext( parsecontext );

			parsecontext.mString = new ScriptString( value1 );
		}
		else if ( keystring.CompareLowercase( L"randomstring" ) )
		{
			FinishContext( parsecontext );

			parsecontext.mRandomString = new ScriptRandomString( value1 );
		}
		else if ( keystring.CompareLowercase( L"function" ) )
		{
			FinishContext( parsecontext );

			parsecontext.mFunction = new ScriptFunction( value1 );
		}
		else
		{
			if ( parsecontext.mNumber != _null )
			{
				parsecontext.mNumber->mNumber = Long( line );
			}
			else if ( parsecontext.mRandomNumber != _null )
			{
				parsecontext.mRandomNumber->mNumberArray.Append( Long( line ) );
			}
			else if ( parsecontext.mString != _null )
			{
				while ( line.Replace( L"<p>", L"\r\n" ) );

				if ( parsecontext.mString->mString == L"" )
					parsecontext.mString->mString = line;
				else
					parsecontext.mString->mString += String( L"\r\n" ) + line;
			}
			else if ( parsecontext.mRandomString != _null )
			{
				while ( line.Replace( L"<p>", L"\r\n" ) );

				parsecontext.mRandomString->mStringArray.Append( line );
			}
			else if ( parsecontext.mFunction != _null )
			{
				if ( keystring == L"if" || keystring == L"endif" )
				{
					_dword index  = parsecontext.mLastIfCommand;
					_dword number = parsecontext.mFunction->mCommandArray.Number( );

					if ( index < number )
						parsecontext.mFunction->mCommandArray[ index ].mParam4 = Long( number ).ToString( );

					parsecontext.mLastIfCommand = -1;
				}

				if ( keystring == L"if" )
					parsecontext.mLastIfCommand = parsecontext.mFunction->mCommandArray.Number( );

				if ( keystring == L"endif" )
					continue;

				parsecontext.mFunction->mCommandArray.Append( ScriptCommand( keystring, value1, value2, value3, value4 ) );
			}
		}
	}

	FinishContext( parsecontext );

	return _true;
}

_void Script::FreeScript( )
{
	for ( _dword i = 0; i < mVariableArray.Number( ); i ++ )
		delete mVariableArray[i];

	mVariableArray.Clear( _true );
	mFunctionArray.Clear( _true );
	mCallbackArray.Clear( _true );

	Stop( );
}

_bool Script::CreateNumber( StringPtr name, _long number )
{
	if ( GetVariable( name ) != _null )
		return _false;

	ScriptNumber* variable = new ScriptNumber( name, number );

	mVariableArray.Append( variable );

	return _true;
}

_bool Script::SetNumberValue( StringPtr name, _long number )
{
	ScriptVariable* variable = GetVariable( name );

	if ( variable == _null )
		return _false;

	return variable->SetNumber( number );
}

_bool Script::CreateRandomNumber( StringPtr name, _long* numberlist, _dword count )
{
	if ( GetVariable( name ) != _null )
		return _false;

	ScriptRandomNumber* variable = new ScriptRandomNumber( name );

	for ( _dword i = 0; i < count; i ++ )
		variable->mNumberArray.Append( numberlist[i] );

	mVariableArray.Append( variable );

	return _true;
}

_bool Script::CreateRegisteredNumber( StringPtr name, OnGetNumber funcpointer, _dword param1, _dword param2 )
{
	if ( GetVariable( name ) != _null )
		return _false;

	ScriptRegisteredNumber* variable = new ScriptRegisteredNumber( name, funcpointer, param1, param2 );

	mVariableArray.Append( variable );

	return _true;
}

_bool Script::CreateString( StringPtr name, StringPtr string )
{
	if ( GetVariable( name ) != _null )
		return _false;

	ScriptString* variable = new ScriptString( name, string );

	mVariableArray.Append( variable );

	return _true;
}

_bool Script::SetStringValue( StringPtr name, StringPtr string )
{
	ScriptVariable* variable = GetVariable( name );

	if ( variable == _null )
		return _false;

	return variable->SetString( string );
}

_bool Script::CreateRandomString( StringPtr name, StringPtr* stringlist, _dword count )
{
	if ( GetVariable( name ) != _null )
		return _false;

	ScriptRandomString* variable = new ScriptRandomString( name );

	for ( _dword i = 0; i < count; i ++ )
		variable->mStringArray.Append( String( stringlist[i] ) );

	mVariableArray.Append( variable );

	return _true;
}

_bool Script::CreateRegisteredString( StringPtr name, OnGetString funcpointer, _dword param1, _dword param2 )
{
	if ( GetVariable( name ) != _null )
		return _false;

	ScriptRegisteredString* variable = new ScriptRegisteredString( name, funcpointer, param1, param2 );

	mVariableArray.Append( variable );

	return _true;
}

_bool Script::CreateCallbackFunction( StringPtr name, OnCommand funcpointer )
{
	if ( GetCallback( name ) != _null )
		return _false;

	mCallbackArray.Append( ScriptCallback( name, funcpointer ) );

	return _true;
}

_bool Script::TriggerFunction( StringPtr name )
{
	if ( mCurrentFunction != _null )
		return _false;

	ScriptFunction* function = GetFunction( name );

	if ( function == _null )
		return _false;

	return ExcuteFunction( *function, 0 );
}

_bool Script::ParseString( String& string )
{
	_bool retval = _false;

	StringPtr tempstring = string;

	while ( _true )
	{
		_dword index1 = tempstring.SearchL2R( L"$(" );

		if ( index1 == -1 )
			break;

		index1 += 2;

		_dword index2 = StringPtr( (const _char*) tempstring + index1 ).SearchL2R( ')' );

		if ( index2 == -1 )
			break;

		String name( (const _char*) tempstring + index1, index2 );

		tempstring = StringPtr( (const _char*) tempstring + index1 );

		ScriptVariable* variable = GetVariable( name );

		if ( variable != _null )
		{
			index1 = ( (_dword) ( (const _char*) tempstring ) - (_dword) ( (const _char*) string ) ) / sizeof( _char ) - 2;

			String value = variable->GetString( );

			string.Remove( index1, index2 + 3 );
			string.Insert( index1, value );

			tempstring = StringPtr( (const _char*) string + index1 + value.Length( ) );

			retval = _true;
		}
	}

	return retval;
}

_void Script::Pause( )
{
	mPaused = _true;
}

_void Script::Resume( )
{
	mPaused = _false;
}

_void Script::Stop( )
{
	mCurrentFunction	= _null;
	mNextCommandIndex	= -1;
	mDelayTickcount		= 0;
	mPaused				= _false;
}

_void Script::Update( _dword elapse )
{
	if ( mPaused )
		return;

	if ( mDelayTickcount > elapse )
	{
		mDelayTickcount -= elapse;

		return;
	}

	mDelayTickcount = 0;

	if ( mCurrentFunction != _null )
		ExcuteFunction( *mCurrentFunction, mNextCommandIndex );
}