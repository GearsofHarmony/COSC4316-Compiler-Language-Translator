#ifndef PRECEDENCEGENERATOR_HPP
#define PRECEDENCEGENERATOR_HPP
#include <iostream>
#include "Constants.h"
#include "Grammar.hpp"

class MatN
{
public:
	static const int MatNSize = 48;
	short mat[MatNSize][MatNSize];
	MatN()
	{
		*this = MatN::Identity();
	};
	explicit MatN(short inMat[MatNSize][MatNSize])
	{
		memcpy(mat, inMat, MatNSize * MatNSize * sizeof(short));
	};
	friend MatN operator*(const short left, const MatN& right)
	{
		MatN retVal;
		for (int row = 0; row < MatNSize; row++)
		{
			for (int col = 0; col < MatNSize; col++)
			{
				retVal.mat[row][col] = (left * right.mat[row][col]);
			}
		}
		return retVal;
	};
	friend MatN operator|(const MatN& left, const MatN& right)
	{
		MatN retVal;
		for (int row = 0; row < MatNSize; row++)
			for (int col = 0; col < MatNSize; col++)
				retVal.mat[row][col] = (left.mat[row][col] | right.mat[row][col]);
		return retVal;
	};
	friend MatN operator&(const MatN& left, const MatN& right)
	{
		MatN retVal;
		for (int row = 0; row < MatNSize; row++)
		{
			for (int col = 0; col < MatNSize; col++)
			{
				retVal.mat[row][col] = 0;
				for (int ii = 0; ii < MatNSize; ii++)
					retVal.mat[row][col] = retVal.mat[row][col] | (left.mat[row][ii] & right.mat[ii][col]);
			}
		}
		return retVal;
	};
	static MatN Identity()
	{
		short temp[MatNSize][MatNSize];
		memset(temp, 0, MatNSize * MatNSize * sizeof(short));
		for (int ii = 0; ii < MatNSize; ii++)
			temp[ii][ii] = 1;
		return MatN(temp);
	}
	static MatN ZeroMat()
	{
		short temp[MatNSize][MatNSize];
		memset(temp, 0, MatNSize * MatNSize * sizeof(short));
		return MatN(temp);
	}
	static MatN transpose(const MatN& mat)
	{
		short temp[MatNSize][MatNSize];
		for (int row = 0; row < MatNSize; row++)
		{
			for (int col = 0; col < MatNSize; col++)
			{
				temp[row][col] = mat.mat[col][row];
			}
		}
		return MatN(temp);
	}
};
MatN warshall(const MatN& table)
{
	MatN temp = table;
	for (int ii = 0; ii < MatN::MatNSize; ii++)
	{
		for (int jj = 0; jj < MatN::MatNSize; jj++)
		{
			if (temp.mat[jj][ii])
			{
				for (int kk = 0; kk < MatN::MatNSize; kk++)
				{
					temp.mat[jj][kk] |= temp.mat[ii][kk];
				}
			}
		}
	}
	return temp;
}
void printMat(const MatN& Result, const char* msg = "Result\n")
{
	std::cout << msg;
	for (int row = 0; row < MatN::MatNSize; row++)
	{
		if (row == numNonTerm)
			std::cout << '\n';
		if (row <= 9)
			std::cout << ' ';
		std::cout << row << ':';
		for (int col = 0; col < MatN::MatNSize; col++)
		{
			int temp = 0x7 & Result.mat[row][col];
			if (col == numNonTerm)
				std::cout << ' ';
			if (temp <= 9)
				std::cout << ' ';
			if (temp <= 99)
				std::cout << ' ';
			std::cout << temp;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
void printMat(const MatN& Result, const int* rows, const int size)
{
	for (int row = 0; row < size; row++)
	{
		if (rows[row] <= 9)
			std::cout << ' ';
		std::cout << rows[row] << ':';
		for (int col = 0; col < size; col++)
		{
			int temp = 0x7 & Result.mat[rows[row]][rows[col]];
			if (col == numNonTerm)
				std::cout << ' ';
			if (temp <= 9)
				std::cout << ' ';
			if (temp <= 99)
				std::cout << ' ';
			std::cout << temp;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
class PrecTableGen
{
private:
	typedef Node<Grammar::IDNode> Node;
	DecisionTable decTable;
	Node List[listNonTerm];
public:
	PrecTableGen(const int* table, int nRow, int nCol)
	{
		decTable.buildTable2D(table, nRow, nCol);
		GramParse(List);
		if (DBG)
		{
			for (int ii = 0; ii < listNonTerm; ii++)
			{
				for (int xx = 0; xx < 20; xx++)
				{
					if (nonTermList[ii][xx] == '$')
					{
						break;
					}
					std::cout << nonTermList[ii][xx];
				}
				std::cout << " ::= ";
				Node::printList(&List[ii]);
				std::cout << '\n';
			}
			std::cout << std::endl;
		}
	}
	MatN generateFirst()
	{
		MatN table = MatN::ZeroMat();
		if (DBG) std::cout << "First:\n";
		for (int index = 0; index < listNonTerm; index++)
		{
			if (!Node::isEmpty(&List[index]))
			{
				int size = Node::getSize(&List[index]);
				Grammar::ID idCol = Node::getData(&List[index], 0).data;
				int col = decTable.index2D(idCol.isTerm, idCol.type);
				table.mat[index][col] = 1;
				if (DBG) std::cout << idCol << '\n';
				for (int ii = 1; ii < size; ii++)
				{
					idCol = Node::getData(&List[index], ii).data;
					col = decTable.index2D(idCol.isTerm, idCol.type);
					if (col == 53)
					{
						idCol = Node::getData(&List[index], ++ii).data;
						col = decTable.index2D(idCol.isTerm, idCol.type);
						table.mat[index][col] = 1;
						if (DBG) std::cout << idCol << '\n';
					}
				}
			}
		}
		if (DBG) std::cout << '\n';
		return table;
	}
	MatN generateWithin()
	{
		MatN table = MatN::ZeroMat();
		if (DBG) std::cout << "Within:\n";
		for (int index = 0; index < listNonTerm; index++)
		{
			int size = Node::getSize(&List[index]);
			for (int ii = 0; ii < size;)
			{
				Grammar::ID idRow = Node::getData(&List[index], ii).data;
				int row = decTable.index2D(idRow.isTerm, idRow.type);
				Grammar::ID idCol = Node::getData(&List[index], ++ii).data;
				int col = decTable.index2D(idCol.isTerm, idCol.type);
				if ((row != 53 && col != 53) && ii < size)
				{
					table.mat[row][col] = 1;
					if (DBG) std::cout << idRow << ' ' << idCol << '\n';
				}
			}
		}
		if (DBG) std::cout << '\n';
		return table;
	}
	MatN generateLast()
	{
		MatN table = MatN::ZeroMat();
		if (DBG) std::cout << "Last:\n";
		for (int index = 0; index < listNonTerm; index++)
		{
			if (!Node::isEmpty(&List[index]))
			{
				int size = Node::getSize(&List[index]);
				for (int ii = 0; ii <= size; ii++)
				{
					Grammar::ID idCol = Node::getData(&List[index], ii).data;
					int col = decTable.index2D(idCol.isTerm, idCol.type);
					if (col == 53 || ii >= size)
					{
						idCol = Node::getData(&List[index], ii - 1).data;
						col = decTable.index2D(idCol.isTerm, idCol.type);
						table.mat[index][col] = 1;
						if (DBG) std::cout << idCol << '\n';
					}
				}
			}
		}
		if (DBG) std::cout << '\n';
		return table;
	}
	MatN generateFirstTerm()
	{
		MatN table = MatN::ZeroMat();
		if (DBG) std::cout << "First Term:\n";
		for (int index = 0; index < listNonTerm; index++)
		{
			bool first = false;
			if (!Node::isEmpty(&List[index]))
			{
				int size = Node::getSize(&List[index]);
				for (int ii = 0; ii < size; ii++)
				{
					Grammar::ID idCol = Node::getData(&List[index], ii).data;
					int col = decTable.index2D(idCol.isTerm, idCol.type);
					if (col >= numNonTerm && col != 53 && !first)
					{
						for (int xx = 0; xx < 2; xx++)
						{
							first = true;
							table.mat[index][col] = 1;
							if (DBG) std::cout << idCol << '\n';
							break;
						}
					}
					else if (col == 53)
					{
						first = false;
					}
				}
			}
		}
		if (DBG) std::cout << '\n';
		return table;
	}
	MatN generateWithinTerm()
	{
		MatN table = MatN::ZeroMat();
		if (DBG) std::cout << "Within Term:\n";
		for (int index = 0; index < listNonTerm; index++)
		{
			if (!Node::isEmpty(&List[index]))
			{
				int size = Node::getSize(&List[index]);
				for (int ii = 0; ii < size;)
				{
					Grammar::ID idRow = Node::getData(&List[index], ii++).data;
					int row = decTable.index2D(idRow.isTerm, idRow.type);
					if (row >= numNonTerm && row != 53)
					{
						for (int xx = 0; xx < 2; xx++)
						{
							Grammar::ID idCol = Node::getData(&List[index], ii++).data;
							int col = decTable.index2D(idCol.isTerm, idCol.type);
							if (col >= numNonTerm && col != 53 && ii <= size)
							{
								table.mat[row][col] = 1;
								ii--;
								if (DBG) std::cout << idRow << ' ' << idCol << '\n';
								break;
							}
							else if (col == 53)
							{
								break;
							}
						}
					}
				}
			}
		}
		if (DBG) std::cout << '\n';
		return table;
	}
	MatN generateLastTerm()
	{
		MatN table = MatN::ZeroMat();
		if (DBG) std::cout << "Last Term:\n";
		for (int index = 0; index < listNonTerm; index++)
		{
			if (!Node::isEmpty(&List[index]))
			{
				int size = Node::getSize(&List[index]);
				for (int ii = 0; ii < size;)
				{
					Grammar::ID idCol = Node::getData(&List[index], ii++).data;
					int col = decTable.index2D(idCol.isTerm, idCol.type);
					int offset = 0;
					if (ii == size || col == 53)
					{
						if (col == 53)
							offset = 1;
						for (int xx = 1 + offset; xx <= (2 + offset); xx++)
						{
							if ((ii - xx) >= 0)
							{
								idCol = Node::getData(&List[index], ii - xx).data;
								col = decTable.index2D(idCol.isTerm, idCol.type);
								if (col >= numNonTerm && col != 53)
								{
									col = decTable.index2D(idCol.isTerm, idCol.type);
									table.mat[index][col] = 1;
									if (DBG) std::cout << idCol << '\n';
									break;
								}
							}

						}
					}
				}
			}
		}
		if (DBG) std::cout << '\n';
		return table;
	}
};

MatN OPG()
{
	/**
	 *	False: Non-Terminal
	 *	0 - 25 ::= Identities
	 *	26 - 30 ::= 29 -> '::='
	 *	True: Terminal
	 *	0 - 9 ::= CLASS, PROCEDURE, CONST, VAR, CALL, DO, WHILE, IF, THEN, ODD
	 *	10 - 11 ::= Name, NumLit
	 *	12 - 15 ::= +, -, *, /
	 *	16 - 21 ::= <, <=, >, >=, ==, !=
	 *	22 ::= =
	 *	23 - 24 ::= ',', ';'
	 *	25 - 28 ::= '(', ')', '{', '}'
	 *	29 - 30 ::= ::=, EOF
	 */
	int convertTable[] =
	{
		// False
		 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
		-1,-1,-1,53,-1,
		// True
		26,27,28,29,30,31,32,33,34,35,
		24,25,
		38,38,39,39,
		40,40,40,40,40,40,
		41,
		42,43,
		44,45,46,47,
		-1,-1,
	};
	PrecTableGen precTable(convertTable, 2, IDeof + 1);
	MatN Equal = precTable.generateWithin() | precTable.generateWithinTerm();
	MatN Yield = (Equal & (MatN::Identity() | warshall(precTable.generateFirst()))) & precTable.generateFirstTerm();
	MatN Take = MatN::transpose((MatN::Identity() | warshall(precTable.generateLast())) & precTable.generateLastTerm()) & Equal;
	return ((2 * Yield) | (4 * Equal) | (1 * Take));
}
#endif // !PRECEDENCEGENERATOR_HPP
