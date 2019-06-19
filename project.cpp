/*
	C++ Program to perform lexical analysis of conditional statement in C++.
	- By Ashish Soni, Jyoti Dalal and Yamini Goel (Group 8)

	This program also scans the program for identifier rules and syntax.
*/

#include "iostream"
#include "fstream"
#include "keywords.h"
#include "singlyll.h"
#include "cstring"

using namespace std;

/*
	This function checks if a given word is a data type or not (eg. int).
*/

int checkDataType( char ele[] )
{
	for( int i = 0; i < 6; ++i )
	{
		if( !strcmp( ele, dataTypes[ i ] ) )
		{
			return i;
		}
	}
	return -1;
}

int checkOperator( char ele[] )
{
	for( int i = 0; i < 12; ++i )
	{
		if( !strcmp( ele, operators[ i ] ) )
		{
			return i;
		}
	}
	return -1;
}

/*
	This function checks if a given identifier is valid or not. An identifier must only consist of alphabets, digits and underscore and also must not be a keyword.
*/

int checkLiteral( char ele[] )
{
	int
		i = 0;
	
	while( ele[ i ] != '\0' )
	{
		if( !isdigit( ele[ i ] ) && ( ele[ i ] != '.' ) && ( ele[ i ] != '-' ) )
		{
			return 0;
		}
		++i;
	}
	return 1;
}

int checkvalididentifier( char ele[] )
{
	int
		i = 0;

	if(  isdigit( ele[ 0 ] )  )
	{
		return 0;
	}

	while( ele[ i ] != '\0' )
	{
		if(   !(  isalnum( ele[ i ] ) || ( ele[ i ] == '_' )  )   )
		{
			return 0;
		}
		++i;
	}
	return 1;
}

int main()
{
	fstream 
		file;

	char
		fname[ 50 ];

	cout << "\n\n\nPlease enter file name to be lexically scanned: ";
	cin >> fname;

	file.open( fname, ios::in );

	if( !file.is_open() )
	{
		cout << "Error: File " << fname << " failed to open!\n";
		return 0;	
	}
	
	
	char 
		ch = 0,
		prev,
		word[ 128 ];

	int 
		wordNo = 0,		// Tracks word number in a line
		index = -1,
		line = 1,
		flag = 1,
		prevdt = -1,
		expvar = 0,
		iflin = 0,
		bracecount = 0,		// Braces count
		i = 0;
	
	SinglyLL 
		obj;
	
	while ( !file.eof() )
	{
		if( ch != ' ' )
		{
			prev = ch;
		}
		ch = file.get();
		
		if( ch == '\n' )
		{
			if( prev != ';' && !iflin && (wordNo != 0)) // If no semicolon
			{
				// Error
				flag = 0;
				cout << "Error: No semicolon at line" << line;
				break;
			}
			if ( expvar )
			{
				// Error
				flag = 0;
				cout << "Error: Expecting an identifier at line " << line;
				break;
			}
			++line;
			wordNo = 0;
			prevdt = -1;
			expvar = 0;
			//iflin = 0;
			index = -1;
		}
		else if( ch == ' ' )
		{
			if( prev != '\n' && prev != ';' ) 
			{
				++wordNo;
			}
			i = 0;
		}
		else if( ch == ';' )
		{
			wordNo = 0;
		}
		else if( ch == '{' )
		{
			++bracecount;
		}
		else if( ch == '}' )
		{
			--bracecount;
		}
		else if( ch == ',' )
		{
			if( prevdt == -1 ) // If comma is encountered and no datatype was declared
			{
				// Error
				flag = 0;
				cout << "Error: Expecting a datatype at line " << line;
				break;
			}
			if ( expvar )
			{
				// Error
				flag = 0;
				cout << "Error: Expecting an identifier before comma at line " << line;
				break;
			}
			i = 0;
			expvar = 1; // Comma encountered, now expecting an identifier.
		}
		else
		{

			word[ i ] = ch;
			++i;
			
			char
				tmp = file.peek();

			if( tmp == ' ' || tmp == ';' || tmp == ',' || tmp == '(' || tmp == '\n' )
			{
				word[ i ] = '\0';
				i = 0;
				index = checkDataType( word );
				//cout << word << "\n";

				if( index != -1 ) // if word is a datatype
				{
					if( !wordNo ) // if it is the first word in line
					{
						/*if( expvar ) // If a datatype was already defined but no identifer was written(eg. int;). 
						{
							// Error
							cout << "Expecting an identifier at line " << line;
							flag = 0;
							break;
						}*/
						expvar = 1;
						prevdt = index;
						//obj.addToHead( word, 0 );
					}
					else // word is a datatype but not in first line.
					{
						// Error
						cout << "Error: Invalid syntax at line " << line;
						flag = 0;
						break;
					}
				}
				else if(  !strcmp( word, "if" )  ) // if statement encounterd
				{
ifblock:
					char
						tmp,
						ifline[ 64 ]; // Extracting whole if line

					SinglyLL
						literals,
						operators,
						operands;
			
					i = 0;
					iflin = 1;

					while( ( !file.eof() ) && ( ( tmp = file.get() ) != '\n' ) )
					{
						if( tmp == EOF )
						{
							break;
						}
						ifline[ i ] = tmp;
						++i;
					}
					ifline[ i ] = '\n';
					++i;
					ifline[ i ] = '\0';
					i = 0;

					int
						j = 0,
						tokenflagz = 0, // 0 = expecting operand, 1 = expecting operator.
						pcount = 0, // parenthesis count
						opercount = 0,
						idencount = 0,
						litcount = 0,
						opcount = 0, // opening parenthesis count
						cpcount = 0; // closing parenthesis count
 		
					while( ifline [ j ] != '\n' )
					{
						if( ifline[ j ] == ' ' )
						{

						}
						else if( ifline[ j ] == '(' )
						{
							++pcount;
							++opcount;
						}
						else if( ifline[ j ] == ')' )
						{
							--pcount;
							++cpcount;
						}
						else
						{
							word[ i ] = ifline[ j ];
							++i;
							
							tmp = ifline[ j+1 ];

							if( tmp == ' ' || tmp == '(' || tmp == ')' )
							{
								word[ i ] = '\0';
								i = 0;

								if( checkvalididentifier( word ) )
								{
									if ( !obj.isInList( word ) )
									{
										// Error
										flag = 0;
										cout << "Error: Undefined identifier " << word << " used in line " << line;
										goto here;
									}
									if( tokenflagz )
									{
										// Error
										flag = 0;
										cout << "Error: Expecting operand at line " << line << "\n";
										goto here;
									}
									tokenflagz = 1;
									operands.addToHead( word, -1 );
									++idencount;
								}
								else
								{
									if( checkOperator( word ) != -1 )
									{
										if( !tokenflagz )
										{
											// Error
											flag = 0;
											cout << "Error: Expecting operator at line " << line << "\n";
											goto here;
										}
										tokenflagz = 0;
										operators.addToHead( word, -1 );
										++opercount;
									}
									else if( checkLiteral( word ) )
									{
										if( tokenflagz )
										{
											// Error
											flag = 0;
											cout << "Error: Expecting operator at line " << line << "\n";
											goto here;
										}
										tokenflagz = 1;
										literals.addToHead( word, -1 );
										++litcount;
									}
									else
									{
										// Error
										flag = 0;
										cout << "Error: Invalid symbol " << word << " in line " << line << "\n";
										goto here;
									}
								}
							}
						}
						++j;
					}
					if( pcount )
					{
						// Error
						flag = 0;
						cout << "Error: Missing parenthesis at line " << line << "\n";
						break;
					}

					if( !tokenflagz )
					{
						// Error
						flag = 0;
						cout << "Error: Missing operand at line " << line << "\n";
						break;
					}

					cout << "\nLexical analysis of if statement: " << ifline << "\n\n";
					cout << "TOKEN\t\t\t\tCOUNT\n";
					cout << "Opening parenthesis:\t\t" << opcount << "\n";
					cout << "Closing parenthesis:\t\t" << cpcount << "\n";
					cout << "Identifier:\t\t\t" << idencount << " (";
					operands.display();
					cout << ")\n";
					cout << "Operators:\t\t\t" << opercount << " (";
					operators.display();
					cout << ")\n";
					cout << "Literals:\t\t\t" << litcount << " (";
					literals.display();
					cout << ")\n";

					++line;
				}
				else if(  !strcmp( word, "else" )  ) // else statement encounterd
				{
					if( !iflin )
					{
						// Error
						flag = 0;
						cout << "Error: No if statement before else at line " << line << "\n";
						break;
					}
					char
						tmp2,
						tempword[ 64 ]; // Extracting whole if line
		
					int 
						z = 0;

					while( ( !file.eof() ) && ( ( tmp2 = file.get() ) != '\n' ) )
					{
						if( tmp2 != ' ' )
						{
							tempword[ z ] = tmp2;
							++z;
							char ch3 = file.peek();
							if( ch3 == '(' || ch3 == ' '  )
							{
								break;
							}
						}
					}
					tempword[ z ] = '\0';

					if( !z ) // only else encountered
					{
						iflin = 0;
						// Nothing to do here.
					}
					else if( !strcmp( tempword, "if" ) )
					{
						goto ifblock;
					}
					else
					{
						// Error
						flag = 0;
						cout << "Error: Invalid else syntax at line " << line << "\n";
						break;
					}
				}
				else // if its not a datatype
				{
					if( prevdt == -1 ) // If word is not a datatype and no datatype was defined before it (eg. var;).
					{
						// Error
						cout << "Error: Expecting a datatype before identifier at line " << line;
						flag = 0;
						break;
					}
					if( wordNo != 1 && expvar != 1 ) // No comma was provided
					{
						// Error
						cout << "Error: Expecting a comma at line " << line;
						flag = 0;
						break;
					}
					if( obj.isInList( word ) ) // if variable already exists
					{
						// Error
						cout << "Error: Variable already defined at line " << line;
						flag = 0;
						break;
					}
					else
					{
						if( checkvalididentifier( word ) )
						{
							obj.addToHead( word, prevdt );
							expvar = 0;
						}
						else // invalid identifier
						{
							// Error
							cout << "Error: Invalid identifier at line " << line;
							flag = 0;
							break;
						}
					}
				}
			}
		}
	}
here:
	file.close();

	if( bracecount != 0 )
	{
		( bracecount > 0 ) ? cout << "\nError: missing closing brace '}'\n" : cout << "\nError: missing opening brace '{'\n";
		flag = 0;
	}
			
	if( flag )
	{
		cout << "\n\nVariables used in program...\n";
		obj.display();
		cout << "\n\n\t --> Lexical analysis successful. No errors found.\n\n";
	}
	else
	{
		cout << "\n\n\t --> Lexical analysis stopped.\n\n";
	}
	return 0;
}
