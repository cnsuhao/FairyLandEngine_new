//============================================================================
// Script.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Script
//----------------------------------------------------------------------------

class Script
{
public:
	typedef _void	(*OnCommand)( StringPtr param1, StringPtr param2, StringPtr param3, StringPtr param4 );
	typedef _long	(*OnGetNumber)( _dword param1, _dword param2 );
	typedef String	(*OnGetString)( _dword param1, _dword param2 );

private:
	struct ScriptVariable
	{
		String		mName;

		ScriptVariable( StringPtr name )
			: mName( name ) { }

		virtual ~ScriptVariable( )
			{ }

		virtual StringPtr GetName( ) const
			{ return mName; }

		virtual String GetString( ) const
			{ return String( L"" ); }

		virtual _bool SetString( StringPtr string )
			{ return _false; }

		virtual _long GetNumber( ) const
			{ return 0; }

		virtual _bool SetNumber( _long number )
			{ return _false; }
	};

	struct ScriptNumber : public ScriptVariable
	{
		_long		mNumber;

		ScriptNumber( StringPtr name, _long number = 0 )
			: ScriptVariable( name ), mNumber( number ) { }

		virtual String GetString( ) const
			{ return Long( GetNumber( ) ).ToString( ); }

		virtual _long GetNumber( ) const
			{ return mNumber; }

		virtual _bool SetNumber( _long number )
			{ mNumber = number; return _true; }
	};

	struct ScriptRandomNumber : public ScriptVariable
	{
		Array< _long >	mNumberArray;

		ScriptRandomNumber( StringPtr name )
			: ScriptVariable( name ) { }

		virtual String GetString( ) const
			{ return Long( GetNumber( ) ).ToString( ); }

		virtual _long GetNumber( ) const
			{ return mNumberArray.Number( ) == 0 ? 0 : mNumberArray[ Math::Random( ) % mNumberArray.Number( ) ]; }
	};

	struct ScriptRegisteredNumber : public ScriptVariable
	{
		OnGetNumber	mCallback;
		_dword		mParam1;
		_dword		mParam2;

		ScriptRegisteredNumber( StringPtr name, OnGetNumber funcpointer, _dword param1 = 0, _dword param2 = 0 )
			: ScriptVariable( name ), mCallback( funcpointer ), mParam1( param1 ), mParam2( param2 ) { }

		virtual String GetString( ) const
			{ return Long( GetNumber( ) ).ToString( ); }

		virtual _long GetNumber( ) const
			{ return mCallback == _null ? 0 : (*mCallback)( mParam1, mParam2 ); }
	};

	struct ScriptString : public ScriptVariable
	{
		String		mString;

		ScriptString( StringPtr name, StringPtr string = L"" )
			: ScriptVariable( name ), mString( string ) { }

		virtual String GetString( ) const
			{ return mString; }

		virtual _long GetNumber( ) const
			{ return Long( GetString( ) ); }

		virtual _bool SetString( StringPtr string )
			{ mString = string; return _true; }
	};

	struct ScriptRandomString : public ScriptVariable
	{
		Array< String >	mStringArray;

		ScriptRandomString( StringPtr name )
			: ScriptVariable( name ) { }

		virtual String GetString( ) const
			{ return mStringArray.Number( ) == 0 ? L"" : mStringArray[ Math::Random( ) % mStringArray.Number( ) ]; }

		virtual _long GetNumber( ) const
			{ return Long( GetString( ) ); }
	};

	struct ScriptRegisteredString : public ScriptVariable
	{
		OnGetString	mCallback;
		_dword		mParam1;
		_dword		mParam2;

		ScriptRegisteredString( StringPtr name, OnGetString funcpointer, _dword param1 = 0, _dword param2 = 0 )
			: ScriptVariable( name ), mCallback( funcpointer ), mParam1( param1 ), mParam2( param2 ) { }

		virtual String GetString( ) const
			{ return mCallback == _null ? L"" : (*mCallback)( mParam1, mParam2 ); }

		virtual _long GetNumber( ) const
			{ return Long( GetString( ) ); }
	};

	typedef Array< ScriptVariable* > VariableArray;

	struct ScriptCommand
	{
		String		mName;
		String		mParam1;
		String		mParam2;
		String		mParam3;
		String		mParam4;

		ScriptCommand( ) { }

		ScriptCommand( StringPtr name, StringPtr param1, StringPtr param2, StringPtr param3, StringPtr param4 )
			: mName( name ), mParam1( param1 ), mParam2( param2 ), mParam3( param3 ), mParam4( param4 ) { }
	};

	typedef Array< ScriptCommand > CommandArray;

	struct ScriptFunction
	{
		String			mName;
		CommandArray	mCommandArray;

		ScriptFunction( ) { }

		ScriptFunction( StringPtr name )
			: mName( name ) { }
	};

	typedef Array< ScriptFunction > FunctionArray;

	struct ScriptCallback
	{
		String		mName;
		OnCommand	mCallback;

		ScriptCallback( ) { }

		ScriptCallback( StringPtr name, OnCommand funcpointer )
			: mName( name ), mCallback( funcpointer ) { }
	};

	typedef Array< ScriptCallback > CallbackArray;

	struct ParseContext
	{
		ScriptNumber*		mNumber;
		ScriptRandomNumber*	mRandomNumber;
		ScriptString*		mString;
		ScriptRandomString*	mRandomString;
		ScriptFunction*		mFunction;
		_dword				mLastIfCommand;

		ParseContext( )
		{
			mNumber			= _null;
			mRandomNumber	= _null;
			mString			= _null;
			mRandomString	= _null;
			mFunction		= _null;
			mLastIfCommand	= -1;
		}
	};

	VariableArray	mVariableArray;
	FunctionArray	mFunctionArray;
	CallbackArray	mCallbackArray;

	ScriptFunction*	mCurrentFunction;
	_dword			mNextCommandIndex;
	_dword			mDelayTickcount;
	_bool			mPaused;

private:
	ScriptVariable* GetVariable( StringPtr name );
	ScriptFunction* GetFunction( StringPtr name );
	ScriptCallback* GetCallback( StringPtr name );

	_bool ExcuteCommand( ScriptCommand& command );
	_bool ExcuteFunction( ScriptFunction& function, _dword startcommand );

	_void FinishContext( ParseContext& parsecontext );

public:
	Script( );
	~Script( );

	_bool LoadScript( StringPtr filename );
	_bool LoadScript( TxtFile& sptfile );
	_void FreeScript( );

	_bool CreateNumber( StringPtr name, _long number );
	_bool SetNumberValue( StringPtr name, _long number );
	_bool CreateRandomNumber( StringPtr name, _long* numberlist, _dword count );
	_bool CreateRegisteredNumber( StringPtr name, OnGetNumber funcpointer, _dword param1 = 0, _dword param2 = 0 );

	_bool CreateString( StringPtr name, StringPtr string );
	_bool SetStringValue( StringPtr name, StringPtr string );
	_bool CreateRandomString( StringPtr name, StringPtr* stringlist, _dword count );
	_bool CreateRegisteredString( StringPtr name, OnGetString funcpointer, _dword param1 = 0, _dword param2 = 0 );

	_bool CreateCallbackFunction( StringPtr name, OnCommand funcpointer );

	_bool TriggerFunction( StringPtr name );
	_bool ParseString( String& string );

	_void Pause( );
	_void Resume( );
	_void Stop( );
	_void Update( _dword elapse );
};

};