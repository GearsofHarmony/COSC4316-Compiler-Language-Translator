#include<iostream>
#include<fstream>

/**
 * ASCII Character encoding for ASCII look-up table
 */
enum ASCII
{
	Letter,
	Digit,
	WS,
	eof,
	mop1,
	mop2,
	addop1,
	addop2,
	relop1,
	relop2,
	relop3,
	assign,
	comma,
	semi,
	LP,
	RP,
	LB,
	RB,
	Other
};
/**
 * ASCII Look-up table
 */
const unsigned int ASCII[16][8] =
{
	{	ASCII::eof,		ASCII::Other,	ASCII::WS,		ASCII::Digit,	ASCII::Other,	ASCII::Letter,	ASCII::Other,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::relop3,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::LP,		ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::WS,		ASCII::Other,	ASCII::RP,		ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::WS,		ASCII::Other,	ASCII::mop1,	ASCII::Other,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::WS,		ASCII::Other,	ASCII::addop1,	ASCII::semi,	ASCII::Letter,	ASCII::Other,	ASCII::Letter,	ASCII::LB	},
	{	ASCII::WS,		ASCII::Other,	ASCII::comma,	ASCII::relop1,	ASCII::Letter,	ASCII::Other,	ASCII::Letter,	ASCII::Other	},
	{	ASCII::WS,		ASCII::Other,	ASCII::addop2,	ASCII::assign,	ASCII::Letter,	ASCII::Other,	ASCII::Letter,	ASCII::RB	},
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::relop2,	ASCII::Letter,	ASCII::Other,	ASCII::Letter,	ASCII::Other	},
	{	ASCII::Other,	ASCII::Other,	ASCII::mop2,	ASCII::Other,	ASCII::Letter,	ASCII::Other,	ASCII::Letter,	ASCII::Other	}
};
/**
 * Reserved word list
 */
const unsigned char reservedWords[][11] =
{
	"CLASS$",
	"PROCEDURE$",
	"CONST$",
	"VAR$",
	"CALL$",
	"DO$",
	"IF$",
	"ODD$",
	"THEN$",
	"WHILE$"
};
/**
 * Data functions as a dynamic reallocation stack OR queue
 * @param 'size' is initial and current memory allocated
 * @param 'top' is current top of stack where -1 is empty
 * @param 'data' a pointer to memory location
 */
class Data
{
private:
	int size = 16;
	int top = -1;
	unsigned char* data = 0;
	/**
	 * Reallocates memory
	 */
	void realloc()
	{
		unsigned char* temp = new unsigned char[size];
		for (int ii = 0; ii <= top; ii++)
			temp[ii] = data[ii];
		delete[] data; data = temp;
	}
public:
	Data() { data = new unsigned char[size]; };
	~Data() { delete[] data; data = 0; };
	Data& operator=(const Data& inVal)
	{
		if (inVal.size > this->size)
		{
			this->size = inVal.size;
			this->realloc();
		}
		this->top = inVal.top;
		for (int zz = 0; zz <= this->top; zz++)
			this->data[zz] = inVal.data[zz];
		return *this;
	}
	/**
	 * Get size of stack
	 * @return current top of stack
	 */
	int Size() { return top; };
	/**
	 * Empty the stack
	 */
	void empty() { top = -1; };
	/**
	 * Push new entry onto stack
	 * @param[in] 'input' new value to enter stack
	 * @throw basic when top exceeds max size the memory is reallocated by 8
	 */
	void push(unsigned char input)
	{
		if (top + 1 >= size)
		{
			size += 8;
			realloc();
		}
		data[++top] = input;
	}
	/**
	 * Pop entry from stack
	 * @return top entry from stack before decrementing 'top'
	 * @throw basic if 'data' is empty null character is returned
	 */
	unsigned char pop()
	{
		if (top >= 0)
			return data[top--];
		else
			return '\0';
	}
	/**
	 * Read from 'dat' bottom-to-top without modifying memory
	 * @param[in] 'index' is index for entry
	 * @return entry at 'index' of 'dat'
	 * @throw basic if 'index' is NOT greater than or equal to 0 And less than or equal to top 
	 * then null character is returned
	 */
	unsigned char read(int index)
	{
		if (index >= 0 && index <= top)
			return data[index];
		else
			return '\0';
	}
};
/**
 * Simple class to write to file
 * @param 'fout' ofstream for writing to file
 * @note anticipating some tailoring for future parts
 */
class FileOut
{
private:
	std::ofstream fout;
public:
	FileOut() {};
	FileOut(const char* outFile) { openFile(outFile); };
	~FileOut() { fout.close(); }
	/**
	 * Open file for first time or close previous file then open new one
	 * @param[in] 'outFile' file name to open
	 * @throw basic silent failure if file doesn't open outputs message to terminal
	 */
	void openFile(const char* outFile)
	{
		fout.close(); fout.open(outFile, std::ios::out | std::ios::binary);
		if (fout.is_open() == false)
			std::cout << "Can't open file: " << outFile << std::endl;
	};
	void write(unsigned char output) { fout << output; }
	void write(const char* output) { fout << output; }
	void write(Data* data)
	{
		for (int zz = 0; zz <= data->Size(); zz++)
			write(data->read(zz));
	}
	void writeInt(const int output)
	{
		Data dat;
		int temp = output;
		do
		{
			int out = (temp % 10); temp = (temp - out) / 10;
			dat.push(unsigned char(out + 0x30));
		} while (temp);
		for (int zz = 0; zz <= dat.Size();)
			fout << dat.pop();
	}
};
/**
 * Simple read file class with buffer for input
 * @param 'fin' input file stream
 * @param 'buffer' is the input buffer window of 4 bytes
 * @param 'numbits' number of bits in the buffer
 */
class FileIn
{
private:
	std::ifstream fin;
	unsigned int buffer; int numbits;
	/**
	 * fill the buffer to full
	 * @throw basic stop filling the buffer when 'numbits' equal to 32 or when EOF is reached
	 */
	void fillBuffer()
	{
		unsigned char temp;
		do
		{
			if (buffer < (1U << numbits))
			{
				fin >> std::noskipws >> temp;
				if (!isEOF())
					buffer |= temp << numbits;
				numbits += 8;
			}
		} while (numbits <= 24 && !isEOF());
	}
	/**
	 * Read number of bits from buffer
	 * @param[in] 'num' number of bits to read
	 */
	unsigned char receive(const int num)
	{
		unsigned int temp = 0;
		if (numbits < num)
			fillBuffer();
		temp = buffer & ((1 << num) - 1);
		buffer = buffer >> num;
		numbits -= num;
		return temp;
	}
public:
	FileIn() { buffer = 0; numbits = 0; };
	FileIn(const char* inFile) { buffer = 0; numbits = 0; openFile(inFile); };
	~FileIn() { fin.close(); }
	/**
	 * Open file for first time or close previous file then open new one
	 * @param[in] 'inFile' file name to open
	 * @throw basic silent failure if file doesn't open outputs message to terminal
	 */
	void openFile(const char* inFile)
	{
		fin.close(); fin.open(inFile, std::ios::in | std::ios::binary);
		if (fin.is_open() == false)
			std::cout << "Can't find file: " << inFile << std::endl;
	};
	/**
	 * Is file at end
	 * @return true if EOF else false
	 */
	bool isEOF() { return fin.eof(); }
	/**
	 * Read 1 byte from buffer
	 */
	unsigned char read() { return receive(8); }
	/**
	 * Return byte to buffer
	 * @param[in] 'input' is value to return to buffer
	 */
	void restore(unsigned char input) { buffer = (buffer << 8) | (unsigned char)input; numbits += 8; }
	/**
	 * Encode input to indexed value with ASCII look-up table
	 * @param[in] 'input' encode input into indexed value for table
	 */
	unsigned int encode(unsigned char input) { return ASCII[0x0F & (unsigned int)input][(0xF0 & (unsigned int)input) >> 4]; }
};
/**
 * Decision Table class for all parsers to utilize
 * @param 'row' row index of array
 * @param 'col' column index of array
 * @param 'decTable' decision table for parser
 */
class DecisionTable
{
private:
	int row, col;
	int* decTable;
	/**
	 * Initializes 'decTable' with a copy of input 'table'
	 * @param[in] 'table' pointer to table for copying
	 * @param[in] 'nRow' number of rows for 'table'
	 * @param[in] 'nCol' number of columns for 'table'
	 */
	void initialize(const int* table, int nRow, int nCol)
	{
		row = nRow; col = nCol;
		decTable = new int[row * col];
		for (int xx = 0; xx < row; xx++)
			for (int yy = 0; yy < col; yy++)
				decTable[(xx * col) + yy] = table[(xx * col) + yy];
	};
public:
	DecisionTable() { row = col = 0; decTable = 0; };
	~DecisionTable() { delete[] decTable; decTable = 0; }
	/**
	 * Public method for user to rebuild table
	 * @param[in] 'table' pointer to table for copying
	 * @param[in] 'nRow' number of rows for 'table'
	 * @param[in] 'nCol' number of columns for 'table'
	 */
	void buildTable(const int* table, int nRow, int nCol) { initialize(table, nRow, nCol); };
	/**
	 * Indexer for 2D array parameters for 1D array
	 * @param[in] 'nRow' row index for 'decTable'
	 * @param[in] 'nCol' column index for 'decTable'*/
	int index(int nRow, int nCol) { return decTable[(nRow * col) + nCol]; }
};
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
		decTable.buildTable(table, nRow, nCol);
	}
	int parse(Data* data)
	{
		unsigned char input;
		unsigned int nextState = 0;
		bool finished = false;
		bool resWordCheck[10] = { true, true, true, true, true, true, true, true, true, true };
		while (!finished)
		{
			switch (nextState)
			{
			case 0: data->empty(); nextState = decTable.index(nextState, fin.encode(input = fin.read())); break;
			case 1: data->push(input); nextState = decTable.index(nextState, fin.encode(input = fin.read())); break;
			case 2: data->push(input); nextState = decTable.index(nextState, fin.encode(input = fin.read()));
				for (int ii = 0; ii < 10; ii++)
					if (resWordCheck[ii]) { 
						if (reservedWords[ii][data->Size()] != data->read(data->Size())) { resWordCheck[ii] = false; } }
				break;
			case 3: data->push(input); nextState = decTable.index(nextState, fin.encode(input = fin.read())); break;
			case 4: nextState = decTable.index(nextState, fin.encode(input = fin.read())); break;
			case 5: nextState = decTable.index(nextState, fin.encode(input = fin.read())); break;
			case 6: data->push(input); nextState = decTable.index(nextState, fin.encode(input = fin.read())); break;
			case 7: data->push(input); nextState = decTable.index(nextState, fin.encode(input = fin.read())); break;
			case 8: data->push(input); nextState = decTable.index(nextState, fin.encode(input = fin.read())); break;
			case 9: data->push(input); nextState = decTable.index(nextState, fin.encode(input = fin.read())); break;
			case 10: finished = true; fin.restore(input);
				for (int ii = 0; ii <= 10; ii++)
					if (ii < 10) 
						if (resWordCheck[ii] && reservedWords[ii][data->Size() + 1] == '$') 
						{ nextState = ii;	break; }
				break;
			case 11: finished = true; fin.restore(input); break;
			case 12: finished = true; data->push(input); break;
			case 13: finished = true; data->push(input); break;
			case 14: finished = true; data->push(input); break;
			case 15: finished = true; fin.restore(input); break;
			case 16: finished = true; fin.restore(input); break;
			case 17: finished = true; data->push(input); break;
			case 18: finished = true; fin.restore(input); break;
			case 19: finished = true; data->push(input); break;
			case 20: finished = true; fin.restore(input); break;
			case 21: finished = true; data->push(input); break;
			case 22: finished = true; data->push(input); break;
			case 23: finished = true; data->push(input); break;
			case 24: finished = true; data->push(input); break;
			case 25: finished = true; data->push(input); break;
			case 26: finished = true; data->push(input); break;
			case 27: finished = true; data->push(input); break;
			case 28: finished = true; data->push(input); break;
			case 29: finished = true; data->push(input); break;
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
	TokenTable(const char* outFile, const int* table, int nRow, int nCol)
	{
		fout.openFile(outFile);
		decTable.buildTable(table, nRow, nCol);
	};
	bool entry(int input, Data* data)
	{
		bool finished = false;
		int nextState = decTable.index(0, input);
		if (nextState < 13)
			fout.write(data);
		switch (nextState)
		{
		case 0: 
			type = input; fout.write(" $");
			for (int xx = 0; xx <= data->Size(); xx++)
				fout.write(reservedWords[type][xx]);
			fout.write('\n');
			break;
		case 1:
			switch (type)
			{
			case 0: fout.write(" $ProgramName\n"); break;
			case 1: fout.write(" $ProcName\n"); break;
			case 2: fout.write(" $ConstVar\n"); break;
			case 3: fout.write(" $VarName\n"); break;
			case 4: fout.write(" $ProcName\n"); break;
			default: type = 3; break;
			}
			break;
		case 2: fout.write(" $NumLit\n"); break;
		case 3: fout.write(" $addop\n"); break;
		case 4: fout.write(" $mop\n"); break;
		case 5: fout.write(" $relop\n"); break;
		case 6: fout.write(" $=\n"); break;
		case 7: fout.write(" $,\n"); break;
		case 8: fout.write(" $;\n"); break;
		case 9: fout.write(" $(\n"); break;
		case 10: fout.write(" $)\n"); break;
		case 11: fout.write(" ${\n"); break;
		case 12: fout.write(" $}\n"); break;
		case 13: finished = true; break;
		default: finished = true; std::cout << std::endl << "Bad input! " << std::endl; break;
		}
		return finished;
	}
};
/**
 * Parses and writes to symbol table file
 * @param 'type' identifies if variable name is 
 *		ConstVar, ProgramName, VarName, or ProcName
 * @param 'DS' Data Segment location
 * @param 'CS' Code Segment location
 * @param 'fout' output file
 * @param 'decTable' decision table for parse
 */
class SymbolTable
{
private:
	int type = 0;
	int DS = 0, CS = 0;
	FileOut fout;
	DecisionTable decTable;
public:
	SymbolTable(const char* outFile, const int* table, int nRow, int nCol)
	{
		fout.openFile(outFile);
		decTable.buildTable(table, nRow, nCol);
	};
	void entry(int input, Data* data)
	{
		int nextState = decTable.index(0, input);
		switch (nextState)
		{
		case 0:
			type = input; break;
		case 1:
			if (type < 4)
				fout.write(data);
			switch (type)
			{
			case 0: 
				fout.write(" $ProgramName ");
				fout.write('?'); fout.write(" CS"); fout.writeInt(CS++);
				fout.write('\n'); DS = 0; break;
			case 1: 
				fout.write(" $ProcName ");
				fout.write('?'); fout.write(" CS"); fout.writeInt(CS++);
				fout.write('\n'); DS = 0; break;
			case 2: fout.write(" $ConstVar "); break;
			case 3: 
				fout.write(" $VarName ");
				fout.write('?'); fout.write(" DS"); fout.writeInt(DS++);
				fout.write('\n'); break;
			default: break;
			}
			break;
		case 2:
			if (type != 2)
			{
				fout.write("lit"); fout.write(data); fout.write(" $NumLit ");
			}
			fout.write(data); fout.write(" DS"); fout.writeInt(DS++); fout.write('\n');
			break;
		case 3: type = 4; break;
		default: break;
		}
	}
};
/**
 * main program
 */
int main()
{
	Data data;
	int table1[] =
	{
		/**
		 *	0 - 8: Letter, Digit, WS,  eof, mop1, mop2, addop1, addop2, relop1,
		 *	9 - 17: relop2, relop3, assign, comma, semi, LP, RP, LB, RB,
		 *	18: Other
		 */
		2,	1,	0,	29,	14,	3,	12,	13,	6,	7,	9,	8,	23,	24,	25,	26,	27,	28,	-1,	// State 0
		11,	1,	11,	11,	11,	11,	11,	11,	11,	11,	11,	11,	11,	11,	11,	11,	11,	11,	-1,	// State 1
		2,	2,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	10,	-1,	// State 2
		15,	15,	15,	15,	4,	15,	15,	15,	15,	15,	15,	15,	15,	15,	15,	15,	15,	15,	-1,	// State 3
		4,	4,	4,	4,	5,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	// State 4
		4,	4,	4,	4,	4,	0,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	// State 5
		16,	16,	16,	16,	16,	16,	16,	16,	16,	16,	16,	17,	16,	16,	16,	16,	16,	16,	-1,	// State 6
		18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	18,	19,	18,	18,	18,	18,	18,	18,	-1,	// State 7
		20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	21,	20,	20,	20,	20,	20,	20,	-1,	// State 8
		-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	22,	-1,	-1,	-1,	-1,	-1,	-1,	-1	// State 9
	};
	int table2[30] =
	{ 0,0,0,0,0,0,0,0,0,0,1,2,3,3,4,4,5,5,5,5,6,5,5,7,8,9,10,11,12,13 };
	int table3[30] =
	{ 0,0,0,0,0,0,0,0,0,0,1,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,3,-1,-1,-1,-1,-1 };
	Scanner scan("Java0.txt", table1, 10, ASCII::Other + 1); 
	TokenTable token("TokenTable.txt", table2, 1, 30);
	SymbolTable symbol("SymbolTable.txt", table3, 1, 30);
	int input;
	do
	{
		input = scan.parse(&data);
		symbol.entry(input, &data);
	} while (!token.entry(input, &data));
	return 0;
}