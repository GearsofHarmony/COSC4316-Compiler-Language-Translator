#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP
#include"Constants.h"
#include"FileIO.hpp"
#include"Data.hpp"
#include"Nodes.hpp"
#include"Stack.hpp"

namespace Grammar
{
	struct ID
	{
	public:
		bool isTerm = 0;
		int type = 0;
		friend std::ostream& operator<<(std::ostream& cout, const ID& data)
		{
			if (data.isTerm == 0)
			{
				if (data.type == IDEval)
				{
					cout << "|";
				}
				else
				{
					for (int ii = 0; ii < 20; ii++)
					{
						if (nonTermList[data.type][ii] == '$')
						{
							break;
						}
						cout << nonTermList[data.type][ii];
					}
				}
			}
			else
			{
				if (data.type < 10)
				{
					for (int ii = 0; ii < 11; ii++)
					{
						if (reservedWords[data.type][ii] == '$')
						{
							break;
						}
						cout << reservedWords[data.type][ii];
					}
				}
				else
				{
					switch (data.type)
					{
					case IDaddop: cout << "+"; break;
					case IDsubop: cout << "-"; break;
					case IDmop: cout << "*"; break;
					case IDdivop: cout << "/"; break;
					case IDLRop: cout << "<"; break;
					case IDLERop: cout << "<="; break;
					case IDGRop: cout << ">"; break;
					case IDGERop: cout << ">="; break;
					case IDassign: cout << "="; break;
					case IDEQRop: cout << "=="; break;
					case IDNEQRop: cout << "!="; break;
					case IDcomma: cout << ","; break;
					case IDsemi: cout << ";"; break;
					case IDLP: cout << "("; break;
					case IDRP: cout << ")"; break;
					case IDLB: cout << "{"; break;
					case IDRB: cout << "}"; break;
					case IDeof: cout << "EOF"; break;
					default: break;
					}
				}
			}
			cout << ' ';
			return cout;
		}
	};
	struct IDNode
	{
		ID data;
		explicit IDNode(ID inData) :data(inData) {};
		friend std::ostream& operator<<(std::ostream& cout, const IDNode& data)
		{
			cout << data.data;
			return cout;
		}
	};
	/**
	 * Grammar Scanner
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
				case IDName: finished = true; fin.restore(input); nextState = checkReservedWord(data); break;
				case IDNumLit: finished = true; fin.restore(input); break;
				case IDaddop: finished = true; data->push(input); break;
				case IDsubop: finished = true; data->push(input); break;
				case IDmop: finished = true; data->push(input); break;
				case IDdivop: finished = true; break;
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
				case IDEval: finished = true; data->push(input); break;
				case IDeof: finished = true; data->push(input); break;
				default: finished = true; std::cout << std::endl << "Bad input! " << input << std::endl; break;
				}
			}
			return nextState;
		}
	};

	class Identifier
	{
	private:
		DecisionTable decTable;
	public:
		Identifier(const int* table, int nPage, int nRow, int nCol)
		{
			decTable.buildTable3D(table, nPage, nRow, nCol);
		};
		void entry(Grammar::Scanner* scan, Stack<Grammar::ID>* stack)
		{
			int input;
			Grammar::ID temp;
			Data<unsigned char> data;
			bool finished = false;
			int nextState = 0;
			while (!finished)
			{
				switch (nextState)
				{
				case 0:
					scan->parse(&data);
					temp.type = checkNonTerm(&data);
					temp.isTerm = 0;
					nextState = decTable.index3D(nextState, temp.isTerm, temp.type);
					break;
				case 1:
					stack->push(temp, 1);
					temp.type = scan->parse(&data);
					nextState = decTable.index3D(nextState, 1, temp.type);
					break;
				case 2:
					stack->push(temp);
					input = scan->parse(&data);
					if ((temp.type = checkNonTerm(&data)) < numNonTerm)
					{
						temp.isTerm = 0;
					}
					else
					{
						temp.type = input;
						temp.isTerm = 1;
					}
					nextState = decTable.index3D(nextState, temp.isTerm, temp.type);
					break;
				case 3:
					stack->push(stack->pop(), 1);
					temp.isTerm = 0;
					nextState = 2;
					break;
				case 4: finished = true; break;
				default: finished = true; break;
				}
			}
		};
	};

	class GrammarListBuilder
	{
	private:
		DecisionTable decTable;
	public:
		GrammarListBuilder(const int* table, int nPage, int nRow, int nCol) 
		{
			decTable.buildTable3D(table, nPage, nRow, nCol);
		};
		void entry(Node<Grammar::IDNode>* List, Stack<Grammar::ID>* stack)
		{
			do
			{
				int nextState = 0;
				bool finished = false;
				Grammar::ID GID, tempID, cmpID;
				while (!finished)
				{
					switch (nextState)
					{
					case 0:
						GID = stack->pop(1); nextState = decTable.index3D(nextState, GID.isTerm, GID.type);
						break;
					case 1:
						tempID = stack->pop(); nextState = decTable.index3D(nextState, tempID.isTerm, tempID.type);
						break;
					case 2:
						Node<Grammar::IDNode>::insertLeft(&List[GID.type], Grammar::IDNode(tempID));
						nextState = decTable.index3D(nextState, tempID.isTerm, tempID.type);
						break;
					case 3: finished = true;
						if(!stack->isempty(1))
						{
							cmpID = stack->pop(1);
							if (GID.type == cmpID.type && GID.isTerm == cmpID.isTerm)
								Node<Grammar::IDNode>::insertLeft(&List[GID.type], Grammar::IDNode(tempID));
							stack->push(cmpID, 1);
						}
						break;
					default: finished = true; break;
					}
				}
			} while (!stack->isempty(1));
		};
	};
};
/**
 * Parse Grammar File for evaluation
 */
void GramParse(Node<Grammar::IDNode>* List)
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
		IDaddop,IDsubop,IDmop,IDdivop,
		3,4,6,
		5,IDcomma,IDsemi,
		IDLP,IDRP,IDLB,IDRB,
		7,-1,
		// State 1
		IDNumLit,1,IDNumLit,IDNumLit,
		IDNumLit,IDNumLit,IDNumLit,IDNumLit,
		IDNumLit,IDNumLit,IDNumLit,
		IDNumLit,IDNumLit,IDNumLit,
		IDNumLit,IDNumLit,IDNumLit,IDNumLit,
		IDNumLit,IDNumLit,
		// State 2
		2,2,IDName,IDName,
		IDName,IDName,IDName,IDName,
		IDName,IDName,IDName,
		IDName,IDName,IDName,
		IDName,IDName,IDName,IDName,
		IDName,IDName,
		// State 3
		IDLRop,IDLRop,IDLRop,IDLRop,
		IDLRop,IDLRop,IDLRop,IDLRop,
		IDLRop,IDLRop,IDLRop,
		IDLERop,IDLRop,IDLRop,
		IDLRop,IDLRop,IDLRop,IDLRop,
		IDLRop,IDLRop,
		// State 4
		IDGRop,IDGRop,IDGRop,IDGRop,
		IDGRop,IDGRop,IDGRop,IDGRop,
		IDGRop,IDGRop,IDGRop,
		IDGERop,IDGRop,IDGRop,
		IDGRop,IDGRop,IDGRop,IDGRop,
		IDGRop,IDGRop,
		// State 5
		IDassign,IDassign,IDassign,IDassign,
		IDassign,IDassign,IDassign,IDassign,
		IDassign, IDassign,IDassign,
		IDEQRop,IDassign,IDassign,
		IDassign,IDassign,IDassign,IDassign,
		IDassign,IDassign,
		// State 6
		-1,-1,-1,-1,
		-1,-1,-1,-1,
		-1,-1,-1,
		IDNEQRop,-1,-1,
		-1,-1,-1,-1,
		-1,-1,
		// State 7
		-1,-1,-1,-1,
		-1,-1,-1,-1,
		-1,-1,-1,
		-1,-1,-1,
		-1,-1,-1,-1,
		8,-1,
		// State 8
		-1,-1,-1,-1,
		-1,-1,-1,-1,
		-1,-1,-1,
		IDEval,-1,-1,
		-1,-1,-1,-1,
		-1,-1
	};
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
	int parseTable[] =
	{
		// State 0
		// False
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		-1,-1,-1,-1,-1,
		// True
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,
		-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,
		-1,
		-1,-1,
		-1,-1,-1,-1,
		-1,-1,
		// State 1
		// False
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,
		// True
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,
		-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,
		-1,
		-1,-1,
		-1,-1,-1,-1,
		 2,-1,
		// State 2
		// False
		 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		-1,-1,-1,-1,-1,
		// True
		 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		 2, 2,
		 2, 2, 2, 2,
		 2, 2, 2, 2, 2, 2,
		 2,
		 2, 2,
		 2, 2, 2, 2,
		 3, 4
	};
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
	int gramParseTable[] =
	{
		// State 0
		// False
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1,-1,
		-1,-1,-1,-1,-1,
		// True
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,
		-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,
		-1,
		-1,-1,
		-1,-1,-1,-1,
		-1,-1,
		// State 1
		// False
		 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		-1,-1,-1, 3,-1,
		// True
		 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		 2, 2,
		 2, 2, 2, 2,
		 2, 2, 2, 2, 2, 2,
		 2,
		 2, 2,
		 2, 2, 2, 2,
		-1,-1,
		// State 2
		// False
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		-1,-1,-1,-1,-1,
		// True
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		 1, 1,
		 1, 1, 1, 1,
		 1, 1, 1, 1, 1, 1,
		 1,
		 1, 1,
		 1, 1, 1, 1,
		-1,-1,
	};
	Stack<Grammar::ID> GIDStack(255, 2);
	Grammar::Scanner scan("Grammer.txt", scanTable, 9, ASCII::Other + 1);
	Grammar::Identifier idScan(parseTable, 3, 2, IDeof + 1);
	Grammar::GrammarListBuilder buildList(gramParseTable, 3, 2, IDeof + 1);
	idScan.entry(&scan, &GIDStack);
	if (DBG) GIDStack.print();
	buildList.entry(List, &GIDStack);
	if (DBG) GIDStack.print();
}
#endif