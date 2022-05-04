#ifndef PASS1_HPP
#define PASS1_HPP
#include"Constants.h"
#include"FileIO.hpp"
#include"Data.hpp"
#include "DecisionTables.hpp"

/**
 * Pass 1 for compiler
 * @param 'fin' is the FileIO to read
 * @param 'decTable' is the decision table
 * @note this parse can be tailored for other passes
 */
class Scanner
{
private:
	FileIn fin;
	DecisionTable decTable;
public:
	Scanner() {}
	/**
	 * Constructor for Scanner
	 * @param[in] 'inFile' is the file to access
	 * @param[in] 'table' is the header name to access table
	 * @warning 'table' must have '$' character at end of string to function properly!
	 */
	Scanner(const char* inFile, const char* table)
	{
		fin.openFile(inFile);
		decTable.buildTable(table);
	}
	/**
	 * Initialize Scanner if created with default constructor or for reuse
	 * @param[in] 'inFile' is the file to access
	 * @param[in] 'table' is the header name to access table
	 * @warning 'table' must have '$' character at end of string to function properly!
	 */
	void initScanner(const char* inFile, const char* table)
	{
		fin.openFile(inFile);
		decTable.buildTable(table);
	}
	/**
	 * Parse function to identify unique strings and characters in file
	 * @param[out] 'data' pointer to storage that stores unique string or character
	 * @returns integer of ID for string or character
	 * @warning 'data' contents are emptied each time this function is called
	 */
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
			case 10: finished = true; fin.restore(input); nextState = checkReservedWord(data); break;
			case 11: finished = true; fin.restore(input); nextState = IDNumLit; break;
			case 12: finished = true; data->push(input); nextState = IDaddop; break;
			case 13: finished = true; data->push(input); nextState = IDsubop; break;
			case 14: finished = true; data->push(input); nextState = IDmop; break;
			case 15: finished = true; fin.restore(input); nextState = IDdivop; break;
			case 16: finished = true; fin.restore(input); nextState = IDLRop; break;
			case 17: finished = true; data->push(input); nextState = IDLERop; break;
			case 18: finished = true; fin.restore(input); nextState = IDGRop; break;
			case 19: finished = true; data->push(input); nextState = IDGERop; break;
			case 20: finished = true; data->push(input); nextState = IDEQRop; break;
			case 21: finished = true; data->push(input); nextState = IDNEQRop; break;
			case 22: finished = true; fin.restore(input); nextState = IDassign; break;
			case 23: finished = true; data->push(input); nextState = IDcomma; break;
			case 24: finished = true; data->push(input); nextState = IDsemi; break;
			case 25: finished = true; data->push(input); nextState = IDLP; break;
			case 26: finished = true; data->push(input); nextState = IDRP; break;
			case 27: finished = true; data->push(input); nextState = IDLB; break;
			case 28: finished = true; data->push(input); nextState = IDRB; break;
			case 29: finished = true; data->push(input); nextState = IDSymb; break;
			case 30: finished = true; data->push(input); nextState = IDEval; break;
			case 31: finished = true; data->push(input); nextState = IDWut; break;
			case 32: finished = true; data->push(input); nextState = IDeof; break;
			case 33: finished = true; nextState = IDdivop; break;
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
	/**
	 * Constructor for Scanner
	 * @param[in] 'inFile' is the file to access
	 * @param[in] 'table' is the header name to access table
	 * @warning 'table' must have '$' character at end of string to function properly!
	 */
	TokenTable(const char* outFile, const char* table)
	{
		fout.openFile(outFile);
		decTable.buildTable(table);
	};
	/**
	 * Write string to file for Token Table
	 * @param[in] 'input' string ID generated from scanner
	 * @param[in] 'data' pointer to string for writing to file
	 */
	void entry(int input, Data<unsigned char>* data)
	{
		int nextState = decTable.index1D(input);
		switch (nextState)
		{
		case 0:
			type = input;
			fout.writeData(data); fout.write(" $");
			for (int xx = 0; xx <= data->Size(); xx++)
				fout.write(ReservedWords[type][xx]);
			fout.write('\n');
			break;
		case 1:
			fout.writeData(data);
			switch (type)
			{
			case IDClass: fout.write(" $Pgmident\n"); break;
			case IDProc: fout.write(" $Procident\n"); break;
			case IDConst: fout.write(" $Constident\n"); break;
			case IDVar: fout.write(" $Varident\n"); break;
			case IDCall: fout.write(" $Procident\n"); break;
			default: fout.write(" $ident\n"); break;
			}
			break;
		case 2: fout.writeData(data); fout.write(" $integer\n"); break;
		case 3: fout.writeData(data); fout.write(" $addop\n"); break;
		case 4: fout.writeData(data); fout.write(" $mop\n"); break;
		case 5: fout.writeData(data); fout.write(" $relop\n"); break;
		case 6: fout.writeData(data); fout.write(" $assign\n"); break;
		case 7: fout.writeData(data); fout.write(" $comma\n"); break;
		case 8: fout.writeData(data); fout.write(" $semi\n"); type = 5; break;
		case 9: fout.writeData(data); fout.write(" $LP\n"); break;
		case 10: fout.writeData(data); fout.write(" $RP\n"); break;
		case 11: fout.writeData(data); fout.write(" $LB\n"); break;
		case 12: fout.writeData(data); fout.write(" $RB\n"); break;
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
 * @param 'NumLit' Storage for unique numeric literals found
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
	Data<int> NumLit;
	FileOut fout;
	DecisionTable decTable;
public:
	/**
	 * Constructor for Scanner
	 * @param[in] 'inFile' is the file to access
	 * @param[in] 'table' is the header name to access table
	 * @warning 'table' must have '$' character at end of string to function properly!
	 */
	SymbolTable(const char* outFile, const char* table)
	{
		fout.openFile(outFile);
		decTable.buildTable(table);
	};
	/**
	 * Write string to file for Symbol Table
	 * @param[in] 'input' string ID generated from scanner
	 * @param[in] 'data' pointer to string for writing to file
	 */
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
			case IDClass:
				fout.writeData(data); fout.write(" $Pgmident ");
				fout.write('?'); fout.write(" CS "); fout.writeInt(CS);
				fout.write('\n'); CS += 4; break;
			case IDProc:
				fout.writeData(data); fout.write(" $Procident ");
				fout.write('?'); fout.write(" CS "); fout.writeInt(CS);
				fout.write('\n'); CS += 4; break;
			case IDConst: fout.writeData(data); fout.write(" $Constident "); break;
			case IDVar:
				fout.writeData(data); fout.write(" $Varident ");
				fout.write('?'); fout.write(" DS "); fout.writeInt(DS);
				fout.write('\n'); DS += 2; break;
			default: break;
			}
			break;
		case 2:
			if (type != IDConst)
			{
				input = data->read(0) - 0x30;
				for (int ii = 1; ii <= data->Size(); ii++)
				{
					input = (input * 10) + (data->read(ii) - 0x30);
				}
				for (int ii = 0; ii <= NumLit.Size(); ii++)
				{
					if (NumLit.read(ii) == input)
					{
						return;
					}
				}
				NumLit.push(input);
				fout.write("lit"); fout.writeData(data); fout.write(" $integer ");
			}
			fout.writeData(data); fout.write(" DS "); fout.writeInt(DS);
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
				fout.write('T'); fout.writeInt(ii); fout.write(" $Varident ");
				fout.write('?'); fout.write(" DS "); fout.writeInt(DS);
				fout.write('\n'); DS += 2;
			}
			break;
		default: break;
		}
	}
};
/// First Pass of translator
void Pass1()
{
	Data<unsigned char> data;
	Scanner scan(Java0File, "PSCN$");
	TokenTable token(TokenFile, "tTKN$");
	SymbolTable symbol(SymbolFile, "tSYM$");
	int input = 0;
	do
	{
		input = scan.parse(&data);
		token.entry(input, &data);
		symbol.entry(input, &data);
	} while (input != IDeof);
}
#endif