#ifndef PASS1_HPP
#define PASS1_HPP
#include"Constants.h"
#include"FileIO.hpp"
#include"Data.hpp"

/**
 * Pass 1 for compiler
 * @note this parse can be tailored for other passes
 */
class Scanner
{
private:
	FileIn fin;
	DecisionTable decTable;
public:
	Scanner(const char* inFile, const int* table, int nRow, int nCol)
	{
		fin.openFile(inFile);
		decTable.buildTable2D(table, nRow, nCol);
	}
	int parse(Data<unsigned char>* data)
	{
		unsigned char input;
		unsigned int nextState = 0;
		bool finished = false;
		while (!finished)
		{
			switch (nextState)
			{
			case 0: data->empty(); nextState = decTable.index2D(nextState, fin.encode(input = fin.read())); break;
			case 1: data->push(input); nextState = decTable.index2D(nextState, fin.encode(input = fin.read())); break;
			case 2: data->push(input); nextState = decTable.index2D(nextState, fin.encode(input = fin.read())); break;
			case 3: data->push(input); nextState = decTable.index2D(nextState, fin.encode(input = fin.read())); break;
			case 4: data->push(input); nextState = decTable.index2D(nextState, fin.encode(input = fin.read())); break;
			case 5: data->push(input); nextState = decTable.index2D(nextState, fin.encode(input = fin.read())); break;
			case 6: data->push(input); nextState = decTable.index2D(nextState, fin.encode(input = fin.read())); break;
			case 7: data->push(input); nextState = decTable.index2D(nextState, fin.encode(input = fin.read())); break;
			case 8: data->push(input); nextState = decTable.index2D(nextState, fin.encode(input = fin.read())); break;
			case 9: data->push(input); nextState = decTable.index2D(nextState, fin.encode(input = fin.read())); break;
			case IDName: finished = true; fin.restore(input); nextState = checkReservedWord(data); break;
			case IDNumLit: finished = true; fin.restore(input); break;
			case IDaddop: finished = true; data->push(input); break;
			case IDsubop: finished = true; data->push(input); break;
			case IDmop: finished = true; data->push(input); break;
			case IDdivop: finished = true; fin.restore(input); break;
			case IDLRop: finished = true; fin.restore(input); break;
			case IDLERop: finished = true; data->push(input); break;
			case IDGRop: finished = true; fin.restore(input); break;
			case IDGERop: finished = true; data->push(input); break;
			case IDassign: finished = true; fin.restore(input); break;
			case IDEQRop: finished = true; data->push(input); break;
			case IDNEQRop: finished = true; data->push(input); break;
			case IDcomma: finished = true; data->push(input); break;
			case IDsemi: finished = true; data->push(input); break;
			case IDLP: finished = true; data->push(input); break;
			case IDRP: finished = true; data->push(input); break;
			case IDLB: finished = true; data->push(input); break;
			case IDRB: finished = true; data->push(input); break;
			case IDeof: finished = true; data->push(input); break;
			default: finished = true; std::cout << std::endl << "Bad input! " << input << std::endl; break;
			}
		}
		return nextState;
	}
};
/**
 * Parses and writes to token table file
 * @param 'type' identifies if variable name is
 *		ConstVar, ProgramName, VarName, or ProcName
 * @param 'fout' output file
 * @param 'decTable' decision table for parse
 */
class TokenTable
{
private:
	int type = 0;
	FileOut fout;
	DecisionTable decTable;
public:
	TokenTable(const char* outFile, const int* table, int nCol)
	{
		fout.openFile(outFile);
		decTable.buildTable1D(table, nCol);
	};
	void entry(int input, Data<unsigned char>* data)
	{
		int nextState = decTable.index1D(input);
		switch (nextState)
		{
		case 0:
			type = input;
			fout.write(data); fout.write(" $");
			for (int xx = 0; xx <= data->Size(); xx++)
				fout.write(reservedWords[type][xx]);
			fout.write('\n');
			break;
		case 1:
			fout.write(data);
			switch (type)
			{
			case 0: fout.write(" $ProgramName\n"); break;
			case 1: fout.write(" $ProcName\n"); break;
			case 2: fout.write(" $ConstVar\n"); break;
			case 3: fout.write(" $VarName\n"); break;
			case 4: fout.write(" $ProcName\n"); break;
			default: fout.write(" $VarName\n"); break;
			}
			break;
		case 2: fout.write(data); fout.write(" $NumLit\n"); break;
		case 3: fout.write(data); fout.write(" $addop\n"); break;
		case 4: fout.write(data); fout.write(" $mop\n"); break;
		case 5: fout.write(data); fout.write(" $relop\n"); break;
		case 6: fout.write(data); fout.write(" $=\n"); break;
		case 7: fout.write(data); fout.write(" $,\n"); break;
		case 8: fout.write(data); fout.write(" $;\n"); break;
		case 9: fout.write(data); fout.write(" $(\n"); break;
		case 10: fout.write(data); fout.write(" $)\n"); break;
		case 11: fout.write(data); fout.write(" ${\n"); break;
		case 12: fout.write(data); fout.write(" $}\n"); break;
		default: break;
		}
	}
};
/**
 * Parses and writes to symbol table file
 * @param 'memDS' memory size in bytes for 'DS'
 * @param 'memCS' memory size in bytes for 'CS'
 * @param 'type' identifies if variable name is
 *		ConstVar, ProgramName, VarName, or ProcName
 * @param 'DS' Data Segment location
 * @param 'CS' Code Segment location
 * @param 'numTempVar' number of temporary variables
 * @param 'tempVar' counter for possible number of temporary variables
 * @param 'fout' output file
 * @param 'decTable' decision table for parse
 */
class SymbolTable
{
private:
	const int memDS = 2, memCS = 4;
	int type = 0,
		DS = 0, CS = 0,
		numTempVar = 0, tempVar = 0;
	FileOut fout;
	DecisionTable decTable;
public:
	SymbolTable(const char* outFile, const int* table, int nCol)
	{
		fout.openFile(outFile);
		decTable.buildTable1D(table, nCol);
	};
	void entry(int input, Data<unsigned char>* data)
	{
		int nextState = decTable.index1D(input);
		switch (nextState)
		{
		case 0:
			type = input; break;
		case 1:
			switch (type)
			{
			case 0:
				fout.write(data); fout.write(" $ProgramName ");
				fout.write('?'); fout.write(" CS "); fout.writeInt(CS);
				fout.write('\n'); CS += 4; break;
			case 1:
				fout.write(data); fout.write(" $ProcName ");
				fout.write('?'); fout.write(" CS "); fout.writeInt(CS);
				fout.write('\n'); CS += 4; break;
			case 2: fout.write(data); fout.write(" $ConstVar "); break;
			case 3:
				fout.write(data); fout.write(" $VarName ");
				fout.write('?'); fout.write(" DS "); fout.writeInt(DS);
				fout.write('\n'); DS += 2; break;
			default: break;
			}
			break;
		case 2:
			if (type != 2)
			{
				fout.write("lit"); fout.write(data); fout.write(" $NumLit ");
			}
			fout.write(data); fout.write(" DS "); fout.writeInt(DS);
			fout.write('\n'); DS += 2;
			break;
		case 3: tempVar++; break;
		case 4:
			if (numTempVar < tempVar) { numTempVar = tempVar; }
			type = 4; tempVar = 0;
			break;
		case 5:
			for (int ii = 1; ii <= numTempVar; ii++)
			{
				fout.write('T'); fout.writeInt(ii); fout.write(" $TempVar ");
				fout.write('?'); fout.write(" DS "); fout.writeInt(DS);
				fout.write('\n'); DS += 2;
			}
			break;
		default: break;
		}
	}
};
/**
 * First Pass of translator
 */
void pass1()
{
	/**
	 *	0 - 3 ::= Letter, Digit, WS, eof
	 *	4 - 7 ::= +, -, *, /
	 *	8 - 10 ::= <, >, !
	 *	11 - 13 ::= =, ',', ';'
	 *	14 - 17 ::= '(', ')', '{', '}'
	 *	18 - 19 ::= ':', Other
	 */
	int scanTable[] =
	{
		// State 0
		2,1,0,IDeof,
		IDaddop,IDsubop,IDmop,3,
		4,5,7,
		6,IDcomma,IDsemi,
		IDLP,IDRP,IDLB,IDRB,
		-1,-1,
		// State 1
		IDNumLit,1,IDNumLit,IDNumLit,
		IDNumLit,IDNumLit,IDNumLit,IDNumLit,
		IDNumLit,IDNumLit,IDNumLit,
		IDNumLit,IDNumLit,IDNumLit,
		IDNumLit,IDNumLit,IDNumLit,IDNumLit,
		-1,-1,
		// State 2
		2,2,IDName,IDName,
		IDName,IDName,IDName,IDName,
		IDName,IDName,IDName,
		IDName,IDName,IDName,
		IDName,IDName,IDName,IDName,
		-1,-1,
		// State 3
		IDdivop,IDdivop,IDdivop,IDdivop,
		IDdivop,IDdivop,8,IDdivop,
		IDdivop,IDdivop,IDdivop,
		IDdivop,IDdivop,IDdivop,
		IDdivop,IDdivop,IDdivop,IDdivop,
		-1,-1,
		// State 4
		IDLRop,IDLRop,IDLRop,IDLRop,
		IDLRop,IDLRop,IDLRop,IDLRop,
		IDLRop,IDLRop,IDLRop,
		IDLERop,IDLRop,IDLRop,
		IDLRop,IDLRop,IDLRop,IDLRop,
		-1,-1,
		// State 5
		IDGRop,IDGRop,IDGRop,IDGRop,
		IDGRop,IDGRop,IDGRop,IDGRop,
		IDGRop,IDGRop,IDGRop,
		IDGERop,IDGRop,IDGRop,
		IDGRop,IDGRop,IDGRop,IDGRop,
		-1,-1,
		// State 6
		IDassign,IDassign,IDassign,IDassign,
		IDassign,IDassign,IDassign,IDassign,
		IDassign,IDassign,IDassign,
		IDEQRop,IDassign,IDassign,
		IDassign,IDassign,IDassign,IDassign,
		-1,-1,
		// State 7
		-1,-1,-1,-1,
		-1,-1,-1,-1,
		-1,-1,-1,
		IDNEQRop,-1,-1,
		-1,-1,-1,-1,
		-1,-1,
		// State 8
		8,8,8,-1,
		8,8,9,8,
		8,8,8,
		8,8,8,
		8,8,8,8,
		8,8,
		// State 9
		8,8,8,-1,
		8,8,8,0,
		8,8,8,
		8,8,8,
		8,8,8,8,
		8,8
	};
	/**
	 *	0 - 9 ::= CLASS, PROCEDURE, CONST, VAR, CALL, DO, WHILE, IF, THEN, ODD
	 *	10 - 11 ::= Name, NumLit
	 *	12 - 15 ::= +, -, *, /
	 *	16 - 21 ::= <, <=, >, >=, ==, !=
	 *	22 ::= =
	 *	23 - 24 ::= ',', ';'
	 *	25 - 28 ::= '(', ')', '{', '}'
	 *	29 - 30 ::= ::=, EOF
	 */
	int Pass1Table[] =
	{
		// TokenTable
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 1, 2,
		 3, 3, 4, 4,
		 5, 5, 5, 5, 5, 5,
		 6,
		 7, 8,
		 9,10,11,12,
		-1,13,
		// SymbolTable
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 1, 2,
		 3, 3, 3, 3,
		-1,-1,-1,-1,-1,-1,
		-1,
		-1, 4,
		-1,-1,-1,-1,
		-1, 5
	};
	Data<unsigned char> data;
	Scanner scan("Java0.txt", scanTable, 10, ASCII::Other + 1);
	TokenTable token("TokenTable.txt", Pass1Table, IDeof + 1);
	SymbolTable symbol("SymbolTable.txt", Pass1Table + IDeof + 1, IDeof + 1);
	int input = 0;
	do
	{
		input = scan.parse(&data);
		token.entry(input, &data);
		symbol.entry(input, &data);
	} while (input != IDeof);
}
#endif