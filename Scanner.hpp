#ifndef SCANNER_HPP
#define SCANNER_HPP
#include "Constants.h"
#include "Data.hpp"
#include "DecisionTables.hpp"
#include "FileIO.hpp"

/**
 * Scanner for compiler
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
#endif // !SCANNER_HPP
