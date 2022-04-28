#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <time.h>
#include "Pass1.hpp"
#include "SyntaxPass.hpp"

void TABLE()
{
	FileOut fout("DecisionTablesList.txt");
	/**
	 *	0 - 3 ::= Letter, Digit, WS, eof
	 *	4 - 7 ::= +, -, *, /
	 *	8 - 10 ::= <, >, !
	 *	11 - 13 ::= =, ',', ';'
	 *	14 - 17 ::= '(', ')', '{', '}'
	 *	18 - 21 ::= ':', $, ?, Other
	 */
	int scanTable[] =
	{
		// State 0
		2,1,0,32,
		12,13,14,3,
		4,5,7,
		6,23,24,
		25,26,27,28,
		127,29,127,127,
		// State 1
		11,1,11,11,
		11,11,11,11,
		11,11,11,
		11,11,11,
		11,11,11,11,
		127,127,127,127,
		// State 2
		2,2,10,10,
		10,10,10,10,
		10,10,10,
		10,10,10,
		10,10,10,10,
		127,127,127,127,
		// State 3
		15,15,15,15,
		15,15,8,15,
		15,15,15,
		15,15,15,
		15,15,15,15,
		127,127,127,127,
		// State 4
		16,16,16,16,
		16,16,16,16,
		16,16,16,
		17,16,16,
		16,16,16,16,
		127,127,127,127,
		// State 5
		18,18,18,18,
		18,18,18,18,
		18,18,18,
		19,18,18,
		18,18,18,18,
		127,127,127,127,
		// State 6
		22,22,22,22,
		22,22,22,22,
		22,22,22,
		20,22,22,
		22,22,22,22,
		127,127,127,127,
		// State 7
		127,127,127,127,
		127,127,127,127,
		127,127,127,
		21,127,127,
		127,127,127,127,
		127,127,127,127,
		// State 8
		8,8,8,127,
		8,8,9,8,
		8,8,8,
		8,8,8,
		8,8,8,8,
		8,8,8,8,
		// State 9
		8,8,8,127,
		8,8,8,0,
		8,8,8,
		8,8,8,
		8,8,8,8,
		8,8,8,8
	};
	int scanTable1[] =
	{
		// State 0
		2,1,0,32,
		12,13,14,33,
		4,5,7,
		6,23,24,
		25,26,27,28,
		8,29,127,127,
		// State 1
		11,1,11,11,
		11,11,11,11,
		11,11,11,
		11,11,11,
		11,11,11,11,
		127,127,127,127,
		// State 2
		2,2,10,10,
		10,10,10,10,
		10,10,10,
		10,10,10,
		10,10,10,10,
		127,127,127,127,
		// State 3
		15,15,15,15,
		15,15,8,15,
		15,15,15,
		15,15,15,
		15,15,15,15,
		127,127,127,127,
		// State 4
		16,16,16,16,
		16,16,16,16,
		16,16,16,
		17,16,16,
		16,16,16,16,
		127,127,127,127,
		// State 5
		18,18,18,18,
		18,18,18,18,
		18,18,18,
		19,18,18,
		18,18,18,18,
		127,127,127,127,
		// State 6
		22,22,22,22,
		22,22,22,22,
		22,22,22,
		20,22,22,
		22,22,22,22,
		127,127,127,127,
		// State 7
		127,127,127,127,
		127,127,127,127,
		127,127,127,
		21,127,127,
		127,127,127,127,
		127,127,127,127,
		// State 8
		127,127,127,127,
		127,127,127,127,
		127,127,127,
		127,127,127,
		127,127,127,127,
		9,127,127,127,
		// State 9
		127,127,127,127,
		127,127,127,127,
		127,127,127,
		30,127,127,
		127,127,127,127,
		127,127,127,127
	};
	int scanTable2[] =
	{
		// State 0
		2,1,0,32,
		12,13,14,3,
		4,5,7,
		6,23,24,
		25,26,27,28,
		127,29,31,127,
		// State 1
		11,1,11,11,
		11,11,11,11,
		11,11,11,
		11,11,11,
		11,11,11,11,
		127,127,127,127,
		// State 2
		2,2,10,10,
		10,10,10,10,
		10,10,10,
		10,10,10,
		10,10,10,10,
		127,127,127,127,
		// State 3
		15,15,15,15,
		15,15,8,15,
		15,15,15,
		15,15,15,
		15,15,15,15,
		127,127,127,127,
		// State 4
		16,16,16,16,
		16,16,16,16,
		16,16,16,
		17,16,16,
		16,16,16,16,
		127,127,127,127,
		// State 5
		18,18,18,18,
		18,18,18,18,
		18,18,18,
		19,18,18,
		18,18,18,18,
		127,127,127,127,
		// State 6
		22,22,22,22,
		22,22,22,22,
		22,22,22,
		20,22,22,
		22,22,22,22,
		127,127,127,127,
		// State 7
		127,127,127,127,
		127,127,127,127,
		127,127,127,
		21,127,127,
		127,127,127,127,
		127,127,127,127,
		// State 8
		8,8,8,127,
		8,8,9,8,
		8,8,8,
		8,8,8,
		8,8,8,8,
		8,8,8,127,
		// State 9
		8,8,8,127,
		8,8,8,0,
		8,8,8,
		8,8,8,
		8,8,8,8,
		8,8,8,127,
	};
	/**
	 *	0 - 11 ::= CLASS, PROCEDURE, CONST, VAR, CALL, DO, WHILE, IF, THEN, ODD, GET, PRINT
	 *	12 - 13 ::= Name, NumLit
	 *	14 - 17 ::= +, -, *, /
	 *	18 - 23 ::= <, <=, >, >=, ==, !=
	 *	24 ::= =
	 *	25 - 26 ::= ',', ';'
	 *	27 - 30 ::= '(', ')', '{', '}'
	 *	31 - 34 ::= $, ::=, ?, EOF
	 */
	int Pass1Table[] =
	{
		// TokenTable
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 1, 2,
		 3, 3, 4, 4,
		 5, 5, 5, 5, 5, 5,
		 6,
		 7, 8,
		 9,10,11,12,
		127,127,127,13,
		// SymbolTable
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 1, 2,
		 3, 3, 3, 3,
		127,127,127,127,127,127,
		127,
		127, 4,
		127,127,127, 4,
		127,127,127, 5
	};
	int SyntaxTable[] =
	{
		// State 0
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		 1, 1,
		 1, 1, 1, 1,
		 1, 1, 1, 1, 1, 1,
		 1,
		 1, 1,
		 1, 1, 1, 1,
		127,127,127,127,
		// State 1
		127,127,127,127,127,127,127,127,127,127,127,127,
		127,127,
		127,127,127,127,
		127,127,127,127,127,127,
		127,
		127,127,
		127,127,127,127,
		 2,127,127,127,
		 // State 2
		 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		 3,127,
		127,127,127,127,
		127,127,127,127,127,127,
		127,
		127,127,
		127,127,127,127,
		127,127,127,127,
		// state 3
		127,127,127,127,127,127,127,127,127,127,127,127,
		127, 4,
		127,127,127,127,
		127,127,127,127,127,127,
		127,
		127,127,
		127,127,127,127,
		127,127, 4,127,
		// state 4
		127,127,127,127,127,127,127,127,127,127,127,127,
		 5,127,
		127,127,127,127,
		127,127,127,127,127,127,
		127,
		127,127,
		127,127,127,127,
		127,127,127,127,
		// state 5
		127,127,127,127,127,127,127,127,127,127,127,127,
		127, 6,
		127,127,127,127,
		127,127,127,127,127,127,
		127,
		127,127,
		127,127,127,127,
		127,127,127,127,
	};
	/**
	 *	False: Non-Terminal
	 *	0 - 29 ::= Identities
	 *	30 - 34 ::= Empty, Empty, Empty, Empty, '::=', Empty, Empty
	 *	True: Terminal
	 *	0 - 11 ::= CLASS, PROCEDURE, CONST, VAR, CALL, DO, WHILE, IF, THEN, ODD, GET, PRINT
	 *	12 - 13 ::= Name, NumLit
	 *	14 - 17 ::= +, -, *, /
	 *	18 - 23 ::= <, <=, >, >=, ==, !=
	 *	24 ::= =
	 *	25 - 26 ::= ',', ';'
	 *	27 - 30 ::= '(', ')', '{', '}'
	 *	31 - 34 ::= $, ::=, ?, EOF
	 */
	int parseTable[] =
	{
		// State 0
		// False
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		127,127,127,127,127,127,127,
		// True
		127,127,127,127,127,127,127,127,127,127,127,127,
		127,127,
		127,127,127,127,
		127,127,127,127,127,127,
		127,
		127,127,
		127,127,127,127,
		127,127,127,127,
		// State 1
		// False
		127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
		127,127,127,127,127,127,127,
		// True
		127,127,127,127,127,127,127,127,127,127,127,127,
		127,127,
		127,127,127,127,
		127,127,127,127,127,127,
		127,
		127,127,
		127,127,127,127,
		127, 2,127,127,
		// State 2
		// False
		 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		127,127,127,127,127,127,127,
		// True
		 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		 2, 2,
		 2, 2, 2, 2,
		 2, 2, 2, 2, 2, 2,
		 2,
		 2, 2,
		 2, 2, 2, 2,
		127, 3,127, 4,
		// Grammar Parse Table 2
		// State 0
		// False
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		127,127,127,127,127,127,127,
		// True
		127,127,127,127,127,127,127,127,127,127,127,127,
		127,127,
		127,127,127,127,
		127,127,127,127,127,127,
		127,
		127,127,
		127,127,127,127,
		127,127,127,127,
		// State 1
		// False
		 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		127,127,127,127, 3,127,127,
		// True
		 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		 2, 2,
		 2, 2, 2, 2,
		 2, 2, 2, 2, 2, 2,
		 2,
		 2, 2,
		 2, 2, 2, 2,
		127,127,127,127,
		// State 2
		// False
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		127,127,127,127,127,127,127,
		// True
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		 1, 1,
		 1, 1, 1, 1,
		 1, 1, 1, 1, 1, 1,
		 1,
		 1, 1,
		 1, 1, 1, 1,
		127,127,127,127,
	};
	int convertTable[] =
	{
		// False
		 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,
		127,127,127,127,53,127,127,
		// True
		28,29,30,31,32,33,34,35,36,37,38,39,
		22,23,
		40,40,41,41,
		42,42,42,42,42,42,
		43,
		44,45,
		46,47,48,49,
		127,66,127,127,
	};
	int size1 = 10 * TableSize, size2 = 2 * IDTableSize, size3 = 3 * size2, size4 = 2 * size3;
	fout.write("PSCN"); fout.writeByte(0); fout.writeByte(10); fout.writeByte(TableSize); for (int ii = 0; ii < size1; ii++) { fout.writeByte(scanTable[ii]); };
	fout.write("GSCN"); fout.writeByte(0); fout.writeByte(10); fout.writeByte(TableSize); for (int ii = 0; ii < size1; ii++) { fout.writeByte(scanTable1[ii]); };
	fout.write("SSCN"); fout.writeByte(0); fout.writeByte(10); fout.writeByte(TableSize); for (int ii = 0; ii < size1; ii++) { fout.writeByte(scanTable2[ii]); };
	fout.write("tTKN"); fout.writeByte(0); fout.writeByte(1); fout.writeByte(IDTableSize); for (int ii = 0; ii < IDTableSize; ii++) { fout.writeByte(Pass1Table[ii]); };
	fout.write("tSYM"); fout.writeByte(0); fout.writeByte(1); fout.writeByte(IDTableSize); for (int ii = IDTableSize; ii < size2; ii++) { fout.writeByte(Pass1Table[ii]); };
	fout.write("tSTR"); fout.writeByte(0); fout.writeByte(6); fout.writeByte(IDTableSize); for (int ii = 0; ii < size3; ii++) { fout.writeByte(SyntaxTable[ii]); };
	fout.write("tGID"); fout.writeByte(3); fout.writeByte(2); fout.writeByte(IDTableSize); for (int ii = 0; ii < size3; ii++) { fout.writeByte(parseTable[ii]); };
	fout.write("tGLB"); fout.writeByte(3); fout.writeByte(2); fout.writeByte(IDTableSize); for (int ii = size3; ii < size4; ii++) { fout.writeByte(parseTable[ii]); };
	fout.write("tPRC"); fout.writeByte(0); fout.writeByte(2); fout.writeByte(IDTableSize); for (int ii = 0; ii < size2; ii++) { fout.writeByte(convertTable[ii]); };
}
/**
 * main program
 */
int main()
{
	bool memDBG = 1;
	if (memDBG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	clock_t start;
	start = clock();

	//TABLE();
	pass1();
	syntaxPass();

	std::cout << ((clock() - (double)start) / CLOCKS_PER_SEC);
	if (memDBG)
		_CrtDumpMemoryLeaks();
	return 0;
}

//T2Q3::Test();
//T2Q5::Test();
//#include "FileIO.hpp"
//
//FileOut fout;
//class Mat10
//{
//public:
//	static const int MatNSize = 10;
//	int mat[MatNSize][MatNSize];
//	static const Mat10 Identity;
//	Mat10()
//	{
//		*this = Mat10::Identity;
//	};
//	explicit Mat10(int inMat[MatNSize][MatNSize])
//	{
//		memcpy(mat, inMat, 100 * sizeof(int));
//	};
//	friend Mat10 operator*(const int left, const Mat10& right)
//	{
//		Mat10 retVal;
//
//		for (int row = 0; row < MatNSize; row++)
//		{
//			for (int col = 0; col < MatNSize; col++)
//			{
//				retVal.mat[row][col] = (left * right.mat[row][col]);
//			}
//		}
//		return retVal;
//	};
//	friend Mat10 operator|(const Mat10& left, const Mat10& right)
//	{
//		Mat10 retVal;
//
//		for (int row = 0; row < MatNSize; row++)
//			for (int col = 0; col < MatNSize; col++)
//				retVal.mat[row][col] = (left.mat[row][col] | right.mat[row][col]);
//		return retVal;
//	};
//	friend Mat10 operator&(const Mat10& left, const Mat10& right)
//	{
//		Mat10 retVal;
//
//		for (int row = 0; row < MatNSize; row++)
//		{
//			for (int col = 0; col < MatNSize; col++)
//			{
//				retVal.mat[row][col] = 0;
//				for (int ii = 0; ii < MatNSize; ii++)
//					retVal.mat[row][col] = retVal.mat[row][col] | (left.mat[row][ii] & right.mat[ii][col]);
//			}
//		}
//		return retVal;
//	}; 
//	friend Mat10 operator^(const Mat10& left, const Mat10& right)
//	{
//		Mat10 retVal;
//
//		for (int row = 0; row < MatNSize; row++)
//			for (int col = 0; col < MatNSize; col++)
//				retVal.mat[row][col] = (left.mat[row][col] ^ right.mat[row][col]);
//		return retVal;
//	};
//	static Mat10 transpose(const Mat10& mat)
//	{
//		int temp[MatNSize][MatNSize];
//		for (int row = 0; row < MatNSize; row++)
//		{
//			for (int col = 0; col < MatNSize; col++)
//			{
//				temp[row][col] = mat.mat[col][row];
//			}
//		}
//		return Mat10(temp);
//	}
//};
//static int m10Zero[Mat10::MatNSize][Mat10::MatNSize] =
//{
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
//};
//static int m10Ident[Mat10::MatNSize][Mat10::MatNSize] =
//{
//	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }
//};
//const Mat10 Mat10::Identity(m10Ident);
//
//class Mat9
//{
//public:
//	static const int MatNSize = 9;
//	int mat[MatNSize][MatNSize];
//	static const Mat9 Identity;
//	Mat9()
//	{
//		*this = Mat9::Identity;
//	};
//	explicit Mat9(int inMat[MatNSize][MatNSize])
//	{
//		memcpy(mat, inMat, MatNSize * MatNSize * sizeof(int));
//	};
//	friend Mat9 operator*(const int left, const Mat9& right)
//	{
//		Mat9 retVal;
//
//		for (int row = 0; row < MatNSize; row++)
//		{
//			for (int col = 0; col < MatNSize; col++)
//			{
//				retVal.mat[row][col] = (left * right.mat[row][col]);
//			}
//		}
//		return retVal;
//	};
//	friend Mat9 operator|(const Mat9& left, const Mat9& right)
//	{
//		Mat9 retVal;
//
//		for (int row = 0; row < MatNSize; row++)
//			for (int col = 0; col < MatNSize; col++)
//				retVal.mat[row][col] = (left.mat[row][col] | right.mat[row][col]);
//		return retVal;
//	};
//	friend Mat9 operator&(const Mat9& left, const Mat9& right)
//	{
//		Mat9 retVal;
//
//		for (int row = 0; row < MatNSize; row++)
//		{
//			for (int col = 0; col < MatNSize; col++)
//			{
//				retVal.mat[row][col] = 0;
//				for (int ii = 0; ii < MatNSize; ii++)
//					retVal.mat[row][col] = retVal.mat[row][col] | (left.mat[row][ii] & right.mat[ii][col]);
//			}
//		}
//		return retVal;
//	};
//	friend Mat9 operator^(const Mat9& left, const Mat9& right)
//	{
//		Mat9 retVal;
//
//		for (int row = 0; row < MatNSize; row++)
//			for (int col = 0; col < MatNSize; col++)
//				retVal.mat[row][col] = (left.mat[row][col] ^ right.mat[row][col]);
//		return retVal;
//	};
//	static Mat9 transpose(const Mat9& mat)
//	{
//		int temp[MatNSize][MatNSize];
//		for (int row = 0; row < MatNSize; row++)
//		{
//			for (int col = 0; col < MatNSize; col++)
//			{
//				temp[row][col] = mat.mat[col][row];
//			}
//		}
//		return Mat9(temp);
//	}
//};
//static int m9Zero[Mat9::MatNSize][Mat9::MatNSize] =
//{
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 }
//};
//static int m9Ident[Mat9::MatNSize][Mat9::MatNSize] =
//{
//	{ 1, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 1, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 1, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 1, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 1, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 1, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 1, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 1, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 1 }
//};
//const Mat9 Mat9::Identity(m9Ident);
//
//template<typename MatN = class Mat10>
//MatN warshall(const MatN& table)
//{
//	MatN temp = table;
//	for (int ii = 0; ii < MatN::MatNSize; ii++)
//	{
//		for (int jj = 0; jj < MatN::MatNSize; jj++)
//		{
//			if (temp.mat[jj][ii])
//			{
//				for (int kk = 0; kk < MatN::MatNSize; kk++)
//				{
//					temp.mat[jj][kk] |= temp.mat[ii][kk];
//				}
//			}
//		}
//	}
//	return temp;
//}
//template<typename MatN = class Mat10>
//void printMat(const MatN& table, void printLabel(const int))
//{
//	for (int row = 0; row < MatN::MatNSize; row++)
//	{
//		printLabel(row);
//		for (int col = 0; col < MatN::MatNSize; col++)
//		{
//			fout.writeInt(table.mat[row][col]);
//		}
//		fout.write('\n');
//	}
//}
//namespace T2Q3
//{
//	const char label[11][5] =
//	{
//		" E $",
//		" a $",
//		":= $",
//		" A $",
//		" % $",
//		" B $",
//		" # $",
//		" ( $",
//		" ) $",
//		"** $"
//	};
//	void printLabel(const int index)
//	{
//		int ii = 0;
//		do
//		{
//			fout.write(label[index][ii++]);
//		} while (label[index][ii] != '$');
//	}
//	void Test()
//	{
//		fout.openFile("Test2Q3.txt");
//		int firstTable[10][10] =
//		{
//			{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 1, 0, 0, 0, 0, 0, 1, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
//		};
//		int lastTable[10][10] =
//		{
//			{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 1, 0, 0, 0, 1, 0, 0, 1, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
//		};
//		int withinTable[10][10] =
//		{
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 },
//			{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 1, 0, 1, 0, 1, 0 },
//			{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },
//			{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }
//		};
//		Mat10 first(firstTable), last(lastTable), within(withinTable);
//		Mat10 pFirst = warshall(first), pLast = warshall(last);
//		Mat10 sFirst = Mat10::Identity | pFirst;
//		Mat10 yPrec = within & pFirst,
//			tPrec = Mat10::transpose(pLast) & within & sFirst,
//			Result = yPrec | (2 * within) | (4 * tPrec);
//
//		fout.write("First:\n   E, a, :=, A, %, B, #, (, ), **\n");
//		printMat(first, printLabel);
//		fout.write("Last:\n   E, a, :=, A, %, B, #, (, ), **\n");
//		printMat(last, printLabel);
//		fout.write("Within:\n   E, a, :=, A, %, B, #, (, ), **\n");
//		printMat(within, printLabel);
//		fout.write("First+ ::= warshall(first)\n   E, a, :=, A, %, B, #, (, ), **\n");
//		printMat(pFirst, printLabel);
//		fout.write("Last+ ::= warshall(last)\n   E, a, :=, A, %, B, #, (, ), **\n");
//		printMat(pLast, printLabel);
//		fout.write("First* ::= Identity | pFirst\n   E, a, :=, A, %, B, #, (, ), **\n");
//		printMat(sFirst, printLabel);
//		fout.write("Yield Precedence ::= within & pFirst\n   E, a, :=, A, %, B, #, (, ), **\n");
//		printMat(yPrec, printLabel);
//		fout.write("Take Precedence ::= transpose(pLast) & within & sFirst\n   E, a, :=, A, %, B, #, (, ), **\n");
//		printMat(tPrec, printLabel);
//		fout.write("Result ::= yPrec | (2 * within) | (4 * tPrec)\n   E, a, :=, A, %, B, #, (, ), **\n");
//		printMat(Result, printLabel);
//	}
//}
//namespace T2Q5
//{
//	const char label[10][5] =
//	{
//		" E $",
//		" a $",
//		":= $",
//		"B1 $",
//		" B $",
//		" A $",
//		" * $",
//		" + $",
//		" ( $",
//		" ) $",
//	};
//	void printLabel(const int index)
//	{
//		int ii = 0;
//		do
//		{
//			fout.write(label[index][ii++]);
//		} while (label[index][ii] != '$');
//	}
//	void Test()
//	{
//		fout.openFile("Test2Q5.txt");
//		int firstTable[10][10] =
//		{
//			{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
//			{ 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },
//			{ 0, 1, 0, 0, 0, 0, 0, 0, 1, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
//		};
//		int lastTable[10][10] =
//		{
//			{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
//			{ 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 },
//			{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
//		};
//		int withinTable[10][10] =
//		{
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 1, 1, 0, 0 },
//			{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
//			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
//		};
//		Mat10 first(firstTable), last(lastTable), within(withinTable);
//		Mat10 pFirst = warshall(first), pLast = warshall(last);
//		Mat10 sFirst = Mat10::Identity | pFirst;
//		Mat10 yPrec = within & pFirst,
//			tPrec = Mat10::transpose(pLast) & within & sFirst,
//			Result = yPrec | (2 * within) | (4 * tPrec);
//
//		fout.write("First:\n   E, a, :=, B1, B, A, *, +, (, )\n");
//		printMat(first, printLabel);
//		fout.write("Last:\n   E, a, :=, B1, B, A, *, +, (, )\n");
//		printMat(last, printLabel);
//		fout.write("Within:\n   E, a, :=, B1, B, A, *, +, (, )\n");
//		printMat(within, printLabel);
//		fout.write("First+ ::= warshall(first)\n   E, a, :=, B1, B, A, *, +, (, )\n");
//		printMat(pFirst, printLabel);
//		fout.write("Last+ ::= warshall(last)\n   E, a, :=, B1, B, A, *, +, (, )\n");
//		printMat(pLast, printLabel);
//		fout.write("First* ::= Identity | pFirst\n   E, a, :=, B1, B, A, *, +, (, )\n");
//		printMat(sFirst, printLabel);
//		fout.write("Yield Precedence ::= within & pFirst\n   E, a, :=, B1, B, A, *, +, (, )\n");
//		printMat(yPrec, printLabel);
//		fout.write("Take Precedence ::= transpose(pLast) & within & sFirst\n   E, a, :=, B1, B, A, *, +, (, )\n");
//		printMat(tPrec, printLabel);
//		fout.write("Result ::= yPrec | (2 * within) | (4 * tPrec)\n   E, a, :=, B1, B, A, *, +, (, )\n");
//		printMat(Result, printLabel);
//	}
//}
