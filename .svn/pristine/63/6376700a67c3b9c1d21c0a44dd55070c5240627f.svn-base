//============================================================================
// Expression.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Section Number String Table
//----------------------------------------------------------------------------

StringPtr gSectionNumberStringTable[] = 
{
	L"#0#",  L"#1#",  L"#2#",  L"#3#",  L"#4#",  L"#5#",  L"#6#",  L"#7#",  L"#8#",  L"#9#",  L"#10#", L"#11#", L"#12#", L"#13#", L"#14#", L"#15#", 
	L"#16#", L"#17#", L"#18#", L"#19#", L"#20#", L"#21#", L"#22#", L"#23#", L"#24#", L"#25#", L"#26#", L"#27#", L"#28#", L"#29#", L"#30#", L"#31#",
	L"#32#", L"#33#", L"#34#", L"#35#", L"#36#", L"#37#", L"#38#", L"#39#", L"#40#", L"#41#", L"#42#", L"#43#", L"#44#", L"#45#", L"#46#", L"#47#",
	L"#48#", L"#49#", L"#50#", L"#51#", L"#52#", L"#53#", L"#54#", L"#55#", L"#56#", L"#57#", L"#58#", L"#59#", L"#60#", L"#61#", L"#62#", L"#63#",
};

//----------------------------------------------------------------------------
// Operator Priority Table
//----------------------------------------------------------------------------

_chara Expression::sOperatorPriorityTable[ Expression::_OPERATOR_NUMBER ][ Expression::_OPERATOR_NUMBER ] = 
{
	//		   ?,   +,   -,   *,   /,  >,   >=,  !=,  ==,  <,   <=,  &&,  ||
	/*    */ { '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?' }, // 0
	/* +  */ { '?', '=', '=', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>' }, // 1
	/* -  */ { '?', '=', '=', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>' }, // 2
	/* *  */ { '?', '>', '>', '=', '=', '>', '>', '>', '>', '>', '>', '>', '>' }, // 3
	/* /  */ { '?', '>', '>', '=', '=', '>', '>', '>', '>', '>', '>', '>', '>' }, // 4
	/* >  */ { '?', '<', '<', '<', '<', '=', '=', '=', '=', '=', '=', '>', '>' }, // 5
	/* >= */ { '?', '<', '<', '<', '<', '=', '=', '=', '=', '=', '=', '>', '>' }, // 6
	/* != */ { '?', '<', '<', '<', '<', '=', '=', '=', '=', '=', '=', '>', '>' }, // 7
	/* == */ { '?', '<', '<', '<', '<', '=', '=', '=', '=', '=', '=', '>', '>' }, // 8
	/* <  */ { '?', '<', '<', '<', '<', '=', '=', '=', '=', '=', '=', '>', '>' }, // 9
	/* <= */ { '?', '<', '<', '<', '<', '=', '=', '=', '=', '=', '=', '>', '>' }, // 10
	/* && */ { '?', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '=' }, // 11
	/* || */ { '?', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '=' }, // 12
};

//----------------------------------------------------------------------------
// Expression Implementation
//----------------------------------------------------------------------------

Expression::Expression( )
{
	mIsCheckMode		= _false;

	mMaxLevel			= 0;

	mUserData1			= 0;
	mUserData2			= 0;
	mUserData3			= 0;
	mUserData4			= 0;

	mCalledMethodNumber	= 0;
	mSectionNumber		= 0;

	mErrorFunc			= _null;
	mMethodFunc			= _null;
	mVariableFunc		= _null;
	mCheckMethodFunc	= _null;
	mCheckVariableFunc	= _null;
	mCheckTypeFunc		= _null;
}

Expression::~Expression( )
{
}

_bool Expression::Load( StringPtr expression, _void* parameter1, _void* parameter2 )
{
	Unload( );

	// Format expression ( drop space and table )
	_char  	expressionbackup[ 512 ];
	_char*	expressionbackupaddress = expressionbackup;
	_bool	stringsection			= _false;

	// Insert left bracket
	*expressionbackupaddress ++ = '(';

	// Expression string address
	const _char* address = ( const _char* ) expression;

	// Start to format expression
	while ( *address != 0 )
	{
		_char character		= *address ++;
		_char nextcharacter = *address;

		// Is "String" section ?
		if ( character == '"' )
		{
			stringsection ^= _true;
		}

		// Drop space and table symbol when isn't "String" section
		if ( stringsection == _false )
		{
			// Skip space and table symbols
			if ( character == ' ' || character == '\t' )
				continue;

			// Convert some symbol
			if ( character == '>' )
			{
				if ( nextcharacter == '=' )
				{
					// >=
					character = _OPERATOR_BIG_EQUAL;

					// jump character
					address ++;
				}
				else
				{
					// >
					character = _OPERATOR_BIG;
				}
			}
			else if ( character == '<' )
			{
				if ( nextcharacter == '=' )
				{
					// <=
					character = _OPERATOR_LESS_EQUAL;

					// Jump character
					address ++;
				}
				else
				{
					// <
					character = _OPERATOR_LESS;
				}
			}
			else if ( character == '!' && nextcharacter == '=' ) 
			{
				// != 
				character = _OPERATOR_NOT_EQUAL;

				// Jump character
				address ++;
			}
			else if ( character == '=' && nextcharacter == '=' ) 
			{
				// == 
				character = _OPERATOR_EQUAL;

				// Jump character
				address ++;
			}
			else if ( character == '&' && nextcharacter == '&' ) 
			{
				// &&
				character = _OPERATOR_AND;

				// Jump character
				address ++;
			}
			else if ( character == '|' && nextcharacter == '|' ) 
			{
				// ||
				character = _OPERATOR_OR;

				// Jump character
				address ++;
			}
		}

		*expressionbackupaddress ++ = character;
	}

	// Insert right bracket
	*expressionbackupaddress ++ = ')';

	// Set 'null' end tag
	*expressionbackupaddress = 0;

	// Check string section what if is end by the '"' symbol
	if ( stringsection == _true )
	{
		if ( mErrorFunc != _null )
			(*mErrorFunc)( this, StringFormatter::FormatString( L"'%s' expression miss \" symbol", expression ), parameter1, parameter2 );

		return _false;
	}

	// Clone expression
	mExpression = expressionbackup;

	// Build section
	_dword index = 0;
	if ( BuildSection( index, 1, &mSectionArray[ mSectionNumber ++ ] ) == _false )
		return _false;

	// Connect the last string to first section
	if ( index < mExpression.Length( ) )
		mSectionArray[ 0 ].mExpression += StringPtr( mExpression ).SubString( index );

	return _true;
}

_void Expression::Unload( )
{
	for ( _dword i = 0; i < _MAX_SECTION_NUMBER; i ++ )
	{
		Section& section = mSectionArray[i];

		section.mIsMethod	= _false;
		section.mLevel 		= 0;
		section.mArgIndex	= -1;
		section.mValue 		= 0;
		section.mExpression	= L"";
	}

	mIsCheckMode	= _false;

	mMaxLevel		= 0;
	mSectionNumber	= 0;
}

_void Expression::Calculate( TinyVariable& result, _void* parameter1, _void* parameter2 )
{
	// Initialize data
	mCalledMethodNumber = 0;

	// Common expression without any equal symbols
	Calculate( 0, mSectionNumber, result, parameter1, parameter2 );
}

_void Expression::Check( _void* parameter1, _void* parameter2 )
{
	// Initialize data
	mCalledMethodNumber = 0;

	// Enable check mode
	mIsCheckMode = _true;
	{
		Calculate( 0, mSectionNumber, TinyVariable( ), parameter1, parameter2 );
	}
	// Disable check mode
	mIsCheckMode = _false;
}

_bool Expression::BuildSection( _dword& index, _dword level, Section* section, _bool splited, _bool wholesection )
{
	// Operator table and data
	_dword	previousoperatorflag	= _OPERATOR_UNKNOWN;
	_dword	previousoperatorindex	= 0;

	// Flags
	_bool	stringflag				= _false;

	// Update max level
	mMaxLevel = Math::Max( mMaxLevel, level );

	// Set section level
	section->mLevel = level;

	// Start to build sections
	while ( mExpression[ index ] != 0 )
	{
		// Get character
		_char character		= mExpression[ index ];
		_char nextcharacter = mExpression[ index + 1 ];
		_char prevcharacter = index > 0 ? mExpression[ index - 1 ] : 0;

		// Check string flag
		if ( character == '"' )
		{
			// Change string flag
			stringflag ^= _true;

			// Update expression
			section->mExpression += character;

			// Next character
			index ++;

			continue;
		}

		// String operation
		if ( stringflag == _true )
		{
			// Update expression
			section->mExpression += character;

			// Next character
			index ++;

			continue;
		}

		// Is operator character ?
		_dword operatorflag = GetOperatorFlag( character );
		if ( operatorflag != _OPERATOR_UNKNOWN )
		{
			// Some operator is end terminal symbol
			if ( level > 1 && operatorflag >= _OPERATOR_BIG && operatorflag <= _OPERATOR_OR )
			{
				return _true;
			}

			// Does need to increase level by priority ?
			_long compareresult = CompareOperator( operatorflag, previousoperatorflag );
			if ( compareresult > 0 )
			{
				// Get the first operator flag of current expression
				_dword currentsectionoperatorflag = GetOperatorFlag( section->mExpression[0] );

				// Can't remove negative expression
				if ( currentsectionoperatorflag != _OPERATOR_SUB || currentsectionoperatorflag != previousoperatorflag ) 
				{
					// Check section number
					if ( mSectionNumber >= _MAX_SECTION_NUMBER )
						return _false;

					// Search for the operator flag
					_dword opindex = section->mExpression.SearchR2L( GetOperatorCode( previousoperatorflag ) );
					if ( opindex == -1 )
						return _false;

					// Backup removed string
					_char removedstring[ 64 ];
					StringFormatter::CopyString( removedstring, StringPtr( section->mExpression ).SubString( opindex + 1 ), 63 );

					// Remove the expression for connecting
					section->mExpression.Remove( opindex + 1, section->mExpression.Length( ) - opindex - 1 );

					// Set connection tag
					section->mExpression += gSectionNumberStringTable[ mSectionNumber ];

					// Set next connective section expression
					Section& nextsection = mSectionArray[ mSectionNumber ];
					nextsection.mExpression = removedstring;

					// Does need to increase connective section level ?
					if ( removedstring[0] == '#' )
					{
						// Get section number ( jump first '#' symbol )
						_dword sectionnumber = GetSectionNumber( removedstring + 1 );
						if ( sectionnumber == -1 )
							return _false;

						// Increase section level
						mSectionArray[ sectionnumber ].mLevel ++;

						// Update max level
						mMaxLevel = Math::Max( mMaxLevel, mSectionArray[ sectionnumber ].mLevel );
					}

					// Continue to build sections
					if ( BuildSection( index, level + 1, &mSectionArray[ mSectionNumber ++ ], _true, previousoperatorflag == _OPERATOR_SUB ? _true : _false ) == _false )
						return _false;

					// Locate section
					for ( _dword i = 0; i < mSectionNumber; i ++ )
					{
						if ( level - 1 == mSectionArray[ i ].mLevel )
						{
							section = &mSectionArray[ i ];

							break;
						}
					}

					// Clean previous operator flag
					previousoperatorflag	= 0;
					previousoperatorindex	= -1;

					continue;
				}
			}
			// Negative operator section must be the only one section and change positive operator to negative, or else wrong
			else if ( wholesection == _true && compareresult < 0 )
			{
				// Check section number
				if ( mSectionNumber >= _MAX_SECTION_NUMBER )
					return _false;

				// Set connection tag
				section->mExpression += character == '+' ? '-' : character;
				section->mExpression += gSectionNumberStringTable[ mSectionNumber ];

				// Next character
				index ++;

				// Continue to build sections
				return BuildSection( index, level + 1, &mSectionArray[ mSectionNumber ++ ] );
			}

			// Update previous operator flag
			previousoperatorflag	= operatorflag;
			previousoperatorindex	= index;
		}

		// Push level ?
		if ( character == '(' )
		{
			// Check section number
			if ( mSectionNumber >= _MAX_SECTION_NUMBER )
				return _false;

			_dword argindex	= -1;

			// Check if it is a method connective section 
			if ( IsMethodOrVariableName( section->mExpression ) )
			{
				_dword removelength = 0;

				// Test if only contains method name, then continue to create this section
				if ( IsEnglishCharacter( section->mExpression[0] ) || section->mExpression[0] == ':' )
				{
					if ( section->mIsMethod == _false )
					{
						// Method
						section->mIsMethod = _true;

						// Update expression
						section->mExpression += character;

						// Next character 
						index ++;

						continue;
					}

					// Search for split position
					_dword indextemp = section->mExpression.SearchR2L( ',' );
					if ( indextemp != -1 )
					{
						// Multi-arguments, now clean the argument index
						argindex = 0;

						// Set argument index
						StringPtr	string	= section->mExpression;
						_dword		index_t	= 0;
						while ( ( index_t = string.SearchL2R( ',' ) ) != -1 )
						{
							string = string.SubString( index_t + 1 );

							argindex ++;
						}

						removelength = section->mExpression.Length( ) - indextemp - 1;
					}
					else
					{
						// Only one argument
						argindex		= 0;
						removelength	= section->mExpression.Length( ) - section->mExpression.SearchL2R( '(' ) - 1;
					}
				}

				// Get method name length
				if ( removelength == 0 )
					removelength = index - previousoperatorindex - 1;

				// Remove method name
				section->mExpression.Remove( section->mExpression.Length( ) - removelength, removelength );

				// Rollback index
				index -= ( removelength + 1 );

			} // End of ( IsMethodOrVariableName( section->mExpression ) )

			// Set connection tag
			section->mExpression += gSectionNumberStringTable[ mSectionNumber ];

			// Jump left bracket
			index ++;

			// Set argument index
			if ( argindex != -1 )
				mSectionArray[ mSectionNumber ].mArgIndex = argindex;

			// Continue to build sections
			if ( BuildSection( index, level + 1, &mSectionArray[ mSectionNumber ++ ] ) == _false )
				return _false;
		}
		// Pop level
		else if ( character == ')' )
		{
			// Check level
			if ( level == 0 )
				return _false;

			// Update expression
			if ( section->mIsMethod == _true )
				section->mExpression += character; // only method can include right bracket

			// Next character
			if ( splited == _false )
				index ++;

			// Complete this section building
			return _true;
		}
		// Write character
		else
		{
			// Update expression
			section->mExpression += character;

			// Next character
			index ++;
		}
	}

	return _true;
}

_void Expression::CalculateSection( Section& section, TinyVariable& result, _void* parameter1, _void* parameter2 )
{
	if ( section.mIsMethod == _true )
	{
		// Method calling
		CalculateExpression( section, section.mExpression, 0, L"", result, parameter1, parameter2 );
	}
	else
	{
		// Common string
		_dword	operatorflag	= 0;
		_dword	index			= 0;

		_char 	buffer1[ 128 ];
		_char 	buffer2[ 128 ];
		_char*	address = buffer1;

		buffer1[0] = 0;
		buffer2[0] = 0;

		while ( section.mExpression[ index ] != 0 )
		{
			_char character = section.mExpression[ index ++ ];

			// Is a operator ?
			if ( GetOperatorFlag( character ) != _OPERATOR_UNKNOWN )
			{
				if ( operatorflag != 0 && buffer2[0] != 0 )
				{
					// Set 'null' end tag
					*address = 0;

					// Calculate the expression
					CalculateExpression( section, buffer1, operatorflag, buffer2, result, parameter1, parameter2 );

					// Set result to bufer1 as an operator string
					if ( result.GetType( ) == TinyVariable::_TYPE_ID_STRING || result.GetType( ) == TinyVariable::_TYPE_ID_STRING_PTR )
					{
						// String
						StringFormatter::FormatBuffer( buffer1, 128, L"\"%s\"", StringPtr( result ) );
					}
					else
					{
						// Number
						if ( _float( result ) != 0.0f )
						{
							StringFormatter::CopyString( buffer1, Double( _double( result ) ).ToString( ), 127 );
						}
						else
						{
							buffer1[0] = '0';
							buffer1[1] = 0;
						}
					}

					// Clean data
					operatorflag	= 0;
					address			= buffer2;
					*address		= 0;

					// Rollback for the operator
					index --;

					continue;
				}

				// Copy operator flag
				operatorflag = character;

				// Is the first character of buffer ?
				if ( buffer1[0] == 0 )
					*address ++ = character;

				// Set 'null' end tag
				*address = 0;

				// Change to buffer2
				address = buffer2;

				continue;
			}

			*address ++ = character;
		}

		// Set 'null' end tag
		*address = 0;

		if ( buffer1[0] != 0 && buffer2[0] == 0 )
		{
			// Only one operator string
			CalculateExpression( section, buffer1, 0, L"", result, parameter1, parameter2 );
		}
		else if ( buffer1[0] != 0 && buffer2[0] != 0 && operatorflag != 0 )
		{
			// Two operator string
			CalculateExpression( section, buffer1, operatorflag, buffer2, result, parameter1, parameter2 );
		}
		else
		{
			// Only numeric string
			result = Float( buffer1 );
		}
	}

	// Store value of this section
	section.mValue = result;
}

_void Expression::CalculateExpression( const Section& section, StringPtr op1string, _dword operatorflag, StringPtr op2string, TinyVariable& result, _void* parameter1, _void* parameter2 )
{
	// Check negative operator
	if ( op1string == L"-" )
	{
		GetValue( section, op2string, result, parameter1, parameter2 );

		result *= -1;
	}
	else if ( op1string == L"*" || op1string == L"/" )
	{
		GetValue( section, op2string, result, parameter1, parameter2 );
	}
	else if ( op2string == L"" && operatorflag == 0 )
	{
		GetValue( section, op1string, result, parameter1, parameter2 );
	}
	else
	{
		TinyVariable variable;
		GetValue( section, op1string, result, parameter1, parameter2 );
		GetValue( section, op2string, variable, parameter1, parameter2 );

		// Run in check mode
		if ( mIsCheckMode == _true )
		{
			// Op string must be not empty
			if ( op1string != L"" && op2string != L"" )
			{
				// Compare type
				if ( result.GetType( ) != TinyVariable::_TYPE_ID_UNKNOWN && variable.GetType( ) != TinyVariable::_TYPE_ID_UNKNOWN && result.GetType( ) != variable.GetType( ) )
				{
					// check type 
					if ( mCheckTypeFunc != _null )
						(*mCheckTypeFunc)( this, op1string, _char( operatorflag ), op2string, parameter1, parameter2 );
				}
			}
		}
		else
		{
			// +
			if ( operatorflag == '+' )
			{
				result += variable;
			}
			// -
			else if ( operatorflag == '-' )
			{
				result -= variable;
			}
			// *
			else if ( operatorflag == '*' )
			{
				result *= variable;
			}
			// /
			else if ( operatorflag == '/' )
			{
				result /= variable;
			}
			// >
			else if ( operatorflag == _OPERATOR_BIG )
			{
				result = result > variable;
			}
			// >=
			else if ( operatorflag == _OPERATOR_BIG_EQUAL )
			{
				result = result >= variable;
			}
			// !=
			else if ( operatorflag == _OPERATOR_NOT_EQUAL )
			{
				result = result != variable;
			}
			// ==
			else if ( operatorflag == _OPERATOR_EQUAL )
			{
				result = result == variable;
			}
			// <
			else if ( operatorflag == _OPERATOR_LESS )
			{
				result = result < variable;
			}
			// <=
			else if ( operatorflag == _OPERATOR_LESS_EQUAL )
			{
				result = result <= variable;
			}
			// &&
			else if ( operatorflag == _OPERATOR_AND )
			{
				result &= variable;
			}
			// ||
			else if ( operatorflag == _OPERATOR_OR )
			{
				result |= variable;
			}
		}
	}
}

_void Expression::Calculate( _dword startindex, _dword number, TinyVariable& result,_void* parameter1, _void* parameter2 )
{
	// Calculate by level
	for ( _dword level = mMaxLevel; level > 0; level -- )
	{
		// Calculate by sections
		for ( _dword i = startindex; i < number; i ++ )
		{
			Section& section = mSectionArray[i];
			if ( section.mLevel != level )
				continue;

			CalculateSection( section, result, parameter1, parameter2 );
		}
	}
}

_void Expression::GetValue( const Section& section, StringPtr expression, TinyVariable& result,_void* parameter1, _void* parameter2 )
{
	// Clear the result
	result.Clear( );

	// Get value 
	if ( expression[0] == '#' )
	{
		// Get section number ( jump first '#' symbol )
		_dword sectionnumber = GetSectionNumber( expression.SubString( 1 ) );
		if ( sectionnumber == -1 )
			return;

		// Get value
		result = mSectionArray[ sectionnumber ].mValue;
	}
	else
	{
		// Check if method call
		if ( IsMethodOrVariableName( expression ) )
		{
			// May be it's a method or variable
			_dword leftbracketindex = expression.SearchL2R( '(' );
			if ( leftbracketindex != -1 )
			{
				// Copy expression string
				String expressionbackup = expression;
				
				// Trim right bracket symbol
				expressionbackup.TrimRight( ')' );

				// Get method name
				_char methodname[ 64 ];
				StringFormatter::CopyString( methodname, expressionbackup.SubString( 0, leftbracketindex ), 63 );

				// Get parameter string info and drop right bracket
				_char parameterstring[ 1024 ];
				StringFormatter::CopyString( parameterstring, StringPtr( expressionbackup ).SubString( leftbracketindex + 1 ), 1023 );

				// Split parameter string
				_char*	parametertable[ 32 ];
				_dword	parameternumber = 0;
				_char*	address			= parameterstring;
				_char*	addressbackup	= parameterstring;
				_bool	stringflag		= _false;
				while ( _true )
				{
					// Check string flag
					if ( *address ++ == '"' )
						stringflag ^= _true;

					// Skip string
					if ( stringflag == _true )
						continue;
					
					// Is split symbol ?
					if ( *address == ',' || *address == 0 )
					{
						// Set 'Null' end tag
						*address ++ = 0;

						// Set parameter table and Increase paramter number
						parametertable[ parameternumber ++ ] = addressbackup;

						// Split completed ?
						if ( *address == 0 )
							break;

						// Update address backup
						addressbackup = address;
					}
				}
				
				// Initialize variable array
				for ( _dword i = 0; i < parameternumber; i ++ )
				{
					StringPtr string = parametertable[i];
					if ( string == L"" )
						continue;

					_dword length = string.Length( );

					if ( length > 2 && string[0] == '#' && string[ length - 1 ] == '#' )
					{
						GetValue( section, parametertable[i], mTempTinyVariableArray[i], parameter1, parameter2 );
					}
					else if ( string.SearchL2R( '#' ) == -1 )
					{
						mTempTinyVariableArray[i].Clear( );
					}
					else
					{
						static Section section;
						section.mExpression = string;

						CalculateSection( section, mTempTinyVariableArray[i], parameter1, parameter2 );
					}
				}

				// Method
				if ( mIsCheckMode == _true )
				{
					// Check mode
					if ( mCheckMethodFunc != _null )
						(*mCheckMethodFunc)( this, methodname, parameternumber, parametertable, parameter1, parameter2 );
				}
				else
				{
					// Calculation mode
					if ( mMethodFunc != _null )
						(*mMethodFunc)( this, section.mLevel, section.mArgIndex, methodname, parameternumber, parametertable, mTempTinyVariableArray, result, parameter1, parameter2 );
				}

				// Increase called method number
				mCalledMethodNumber ++;
			}
			else
			{
				// Variable
				if ( mIsCheckMode == _true )
				{
					// Check mode
					if ( mCheckVariableFunc != _null )
						(*mCheckVariableFunc)( this, expression, parameter1, parameter2 );
				}
				else
				{
					// Calculation mode
					if ( mVariableFunc != _null )
						(*mVariableFunc)( this, expression, result, parameter1, parameter2 );
				}
			}
		}
		else
		{
			if ( expression[0] == '"' )
			{
				// Only string expression
				result = ( const _char* ) String( expression ).TrimBoth( '"' );
			}
			else
			{
				// Only numeric expression
				result = _float( Float( expression ) );
			}
		}
	}
}

_dword Expression::GetSectionNumber( StringPtr expression ) const
{
	// Copy expression
	_char expressionbuffer[ 1024 ];
	StringFormatter::CopyString( expressionbuffer, expression, 1023 );

	// Clear end '#' symbol
	expressionbuffer[ expression.Length( ) - 1 ] = 0;

	// Get section number
	_dword sectionnumber = Long( expressionbuffer );
	if ( sectionnumber >= mSectionNumber )
		return -1; // wrong section number

	return sectionnumber;
}
