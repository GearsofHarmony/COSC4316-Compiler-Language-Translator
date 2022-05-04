#ifndef PRECEDENCEGENERATOR_HPP
#define PRECEDENCEGENERATOR_HPP
#include <iostream>
#include "Constants.h"
#include "Grammar.hpp"

namespace PrecGenDBG { const bool DBG = 0; };

/// Precedence table for syntax
class MatN
{
public:
	static const int MatNSize = 50;
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
/// Find transitive closure for matrix
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
MatN matStar(const MatN& table)
{
	return (MatN::Identity() | warshall(table));
}
/**
 * Display contents of matrix in terminal
 * @param[in] 'Result' contents of matrix to display
 * @param[in] 'msg' custom message to display before matrix. Default => "Table\n"
 */
void printMat(const MatN& Result, const char* msg = "Table\n")
{
	std::cout << msg;
	for (int row = 0; row < MatN::MatNSize; row++)
	{
		if (row == NumNonTerm)
			std::cout << '\n';
		if (row <= 9)
			std::cout << ' ';
		std::cout << row << ':';
		for (int col = 0; col < MatN::MatNSize; col++)
		{
			int temp = 0x7 & Result.mat[row][col];
			if (col == NumNonTerm)
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
/**
 * Display contents of matrix in terminal
 * @param[in] 'Result' contents of matrix to display
 * @param[in] 'rows' pointer to list of rows & columns to only display
 * @param[in] 'size' size of 'rows'
 */
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
			if (col == NumNonTerm)
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
	DecisionTable decTable;
public:
	PrecTableGen(const char* table)
	{
		decTable.buildTable(table);
	}
	/**
	 * A FIRST B if and only if there is a rule A ::= B***
	 * @param[in] 'List' Grammar list to construct matrix
	 * @returns matrix
	 */
	MatN generateFirst(Node<Grammar::IDNode>* List)
	{
		MatN table = MatN::ZeroMat();
		if (PrecGenDBG::DBG) std::cout << "First:\n";
		for (int index = 0; index < ListNonTerm; index++)
		{
			if (!Node<Grammar::IDNode>::isEmpty(&List[index]))
			{
				Data<Grammar::ID> temp;
				Grammar::ID check;
				int size = Node<Grammar::IDNode>::getSize(&List[index]);
				for (int ii = 0; ii < size; ii++)
				{
					check = Node<Grammar::IDNode>::getData(&List[index], ii).data;
					if (check.cmp(Grammar::ID(0, IDEval)))
					{
						if (temp.Size() >= 0)
						{
							check = temp.read(0);
							table.mat[index][decTable.index2D(check.isTerm, check.type)] = 1;
							temp.empty();
							if (PrecGenDBG::DBG) std::cout << check << '\n';
						}
					}
					else
					{
						temp.push(check);
					}
				}
			}
		}
		if (PrecGenDBG::DBG) std::cout << '\n';
		return table;
	}
	/**
	 * A WITHIN B if and only if there is a rule U ::= ***AB***
	 * @param[in] 'List' Grammar list to construct matrix
	 * @returns matrix
	 */
	MatN generateWithin(Node<Grammar::IDNode>* List)
	{
		MatN table = MatN::ZeroMat();
		if (PrecGenDBG::DBG) std::cout << "Within:\n";
		for (int index = 0; index < ListNonTerm; index++)
		{
			if (!Node<Grammar::IDNode>::isEmpty(&List[index]))
			{
				Data<Grammar::ID> temp;
				Grammar::ID check;
				int size = Node<Grammar::IDNode>::getSize(&List[index]);
				for (int ii = 0; ii < size; ii++)
				{
					check = Node<Grammar::IDNode>::getData(&List[index], ii).data;
					if (check.cmp(Grammar::ID(0, IDEval)))
					{
						check = temp.pop();
						while (temp.Size() >= 0)
						{
							Grammar::ID row = temp.pop();
							table.mat[decTable.index2D(row.isTerm, row.type)][decTable.index2D(check.isTerm, check.type)] = 1;
							if (PrecGenDBG::DBG) std::cout << row << ' ' << check << '\n';
							check = row;
						}
					}
					else
					{
						temp.push(check);
					}
				}
			}
		}
		if (PrecGenDBG::DBG) std::cout << '\n';
		return table;
	}
	/**
	 * A LAST B if and only if there is a rule A ::= ***B
	 * @param[in] 'List' Grammar list to construct matrix
	 * @returns matrix
	 */
	MatN generateLast(Node<Grammar::IDNode>* List)
	{
		MatN table = MatN::ZeroMat();
		if (PrecGenDBG::DBG) std::cout << "Last:\n";
		for (int index = 0; index < ListNonTerm; index++)
		{
			if (!Node<Grammar::IDNode>::isEmpty(&List[index]))
			{
				Data<Grammar::ID> temp;
				Grammar::ID check;
				int size = Node<Grammar::IDNode>::getSize(&List[index]);
				for (int ii = 0; ii < size; ii++)
				{
					check = Node<Grammar::IDNode>::getData(&List[index], ii).data;
					if (check.cmp(Grammar::ID(0, IDEval)))
					{
						if (temp.Size() >= 0)
						{
							check = temp.read(temp.Size());
							table.mat[index][decTable.index2D(check.isTerm, check.type)] = 1;
							temp.empty();
							if (PrecGenDBG::DBG) std::cout << check << '\n';
						}
					}
					else
					{
						temp.push(check);
					}
				}
			}
		}
		if (PrecGenDBG::DBG) std::cout << '\n';
		return table;
	}
	/**
	 * Let S be a terminal and U, V be non-Terminals
	 * U FIRSTTERM S if and only if there is a rule U ::= S*** OR U ::= VS***
	 * @param[in] 'List' Grammar list to construct matrix
	 * @returns matrix
	 */
	MatN generateFirstTerm(Node<Grammar::IDNode>* List)
	{
		MatN table = MatN::ZeroMat();
		if (PrecGenDBG::DBG) std::cout << "First Term:\n";
		for (int index = 0; index < ListNonTerm; index++)
		{
			if (!Node<Grammar::IDNode>::isEmpty(&List[index]))
			{
				Data<Grammar::ID> temp;
				Grammar::ID check;
				int size = Node<Grammar::IDNode>::getSize(&List[index]);
				for (int ii = 0; ii < size; ii++)
				{
					check = Node<Grammar::IDNode>::getData(&List[index], ii).data;
					if (check.cmp(Grammar::ID(0, IDEval)))
					{
						if (temp.Size() >= 0)
						{
							if (temp.read(0).isTerm)
							{
								check = temp.read(0);
							}
							else if (temp.read(1).isTerm)
							{
								check = temp.read(1);
							}
							if (check.isTerm)
							{
								table.mat[index][decTable.index2D(check.isTerm, check.type)] = 1;
								if (PrecGenDBG::DBG) std::cout << check << '\n';
							}
							temp.empty();
						}
					}
					else
					{
						temp.push(check);
					}
				}
			}
		}
		if (PrecGenDBG::DBG) std::cout << '\n';
		return table;
	}
	/**
	 * Let R, S be a terminal and U, V be non-Terminals
	 * R WITHINTERM S if and only if there is a rule U ::= ***RS*** OR U ::= ***RVS***
	 * @param[in] 'List' Grammar list to construct matrix
	 * @returns matrix
	 */
	MatN generateWithinTerm(Node<Grammar::IDNode>* List)
	{
		MatN table = MatN::ZeroMat();
		if (PrecGenDBG::DBG) std::cout << "Within Term:\n";
		for (int index = 0; index < ListNonTerm; index++)
		{
			if (!Node<Grammar::IDNode>::isEmpty(&List[index]))
			{
				Data<Grammar::ID> temp;
				Grammar::ID check;
				int size = Node<Grammar::IDNode>::getSize(&List[index]);
				for (int ii = 0; ii < size; ii++)
				{
					check = Node<Grammar::IDNode>::getData(&List[index], ii).data;
					if (check.cmp(Grammar::ID(0, IDEval)))
					{
						check = temp.pop();
						while (temp.Size() >= 0)
						{
							Grammar::ID row = temp.pop();
							table.mat[decTable.index2D(row.isTerm, row.type)][decTable.index2D(check.isTerm, check.type)] = 1;
							if (PrecGenDBG::DBG) std::cout << row << ' ' << check << '\n';
							check = row;
						}
					}
					else if (check.isTerm)
					{
						temp.push(check);
					}
				}
			}
		}
		if (PrecGenDBG::DBG) std::cout << '\n';
		return table;
	}
	/**
	 * Let S be a terminal and U, V be non-Terminals
	 * U LASTTERM S if and only if there is a rule U ::= ***S OR U ::= ***SV
	 * @param[in] 'List' Grammar list to construct matrix
	 * @returns matrix
	 */
	MatN generateLastTerm(Node<Grammar::IDNode>* List)
	{
		MatN table = MatN::ZeroMat();
		if (PrecGenDBG::DBG) std::cout << "Last Term:\n";
		for (int index = 0; index < ListNonTerm; index++)
		{
			if (!Node<Grammar::IDNode>::isEmpty(&List[index]))
			{
				Data<Grammar::ID> temp;
				Grammar::ID check;
				int size = Node<Grammar::IDNode>::getSize(&List[index]);
				for (int ii = 0; ii < size; ii++)
				{
					check = Node<Grammar::IDNode>::getData(&List[index], ii).data;
					if (check.cmp(Grammar::ID(0, IDEval)))
					{
						if (temp.Size() >= 0)
						{
							if (temp.read(temp.Size()).isTerm)
							{
								check = temp.read(0);
							}
							else if (temp.read(temp.Size() - 1).isTerm)
							{
								check = temp.read(1);
							}
							if(check.isTerm)
							{
								table.mat[index][decTable.index2D(check.isTerm, check.type)] = 1;
								if (PrecGenDBG::DBG) std::cout << check << '\n';
							}
							temp.empty();
						}
					}
					else
					{
						temp.push(check);
					}
				}
			}
		}
		if (PrecGenDBG::DBG) std::cout << '\n';
		return table;
	}
	MatN OPG(Node<Grammar::IDNode>* List)
	{
		MatN Equal = generateWithin(List) | generateWithinTerm(List);
		MatN Yield = (Equal & matStar(generateFirst(List))) & generateFirstTerm(List);
		MatN Take = MatN::transpose(matStar(generateLast(List)) & generateLastTerm(List)) & Equal;
		return ((2 * Yield) | (4 * Equal) | (1 * Take));
	}
	MatN PGTable(Node<Grammar::IDNode>* List)
	{
		MatN Equal = generateWithin(List);
		MatN Yield = Equal & warshall(generateFirst(List));
		MatN Take = (MatN::transpose(warshall(generateLast(List))) & Equal) & matStar(generateFirst(List));
		return ((2 * Yield) | (4 * Equal) | (1 * Take));
	}
	MatN testTable(int entry, Node<Grammar::IDNode>* List)
	{
		//MatN Equal = warshall(generateWithin(List));
		MatN Equal = generateWithin(List);
		MatN First = warshall(generateFirst(List));
		MatN Test = MatN::ZeroMat(); Test.mat[entry][entry] = 1;
		return Equal & First & Test;
	}
};
#endif // !PRECEDENCEGENERATOR_HPP
