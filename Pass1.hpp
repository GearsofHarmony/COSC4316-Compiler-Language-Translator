#ifndef PASS1_HPP
#define PASS1_HPP
#include"Constants.h"
#include"Data.hpp"
#include"DecisionTables.hpp"
#include"FileIO.hpp"
#include"Scanner.hpp"

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