#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <time.h>
#include "Pass1.hpp"
#include "PrecedenceGenerator.hpp"
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
	pass1();
	printMat(OPG());

	//T2Q3::Test();
	//T2Q5::Test();
	std::cout << ((clock() - (double)start) / CLOCKS_PER_SEC);
	if (memDBG)
		_CrtDumpMemoryLeaks();
	return 0;
}