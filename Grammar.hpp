#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP
#include"Constants.h"
#include"FileIO.hpp"
#include"Data.hpp"
#include"Nodes.hpp"
#include"Stack.hpp"

namespace GrammarDBG { const bool DBG = 0; };

namespace Grammar
{
	/// Struct for Grammar ID
	struct ID
	{
		bool isTerm = 0;
		int type = 0;
		ID() {};
		explicit ID(bool inTerm, int inType) { isTerm = inTerm; type = inType; };
		bool cmp(const Grammar::ID Right)
		{
			if (this->isTerm == Right.isTerm && this->type == Right.type)
				return true;
			else
				return false;
		}
		friend std::ostream& operator<<(std::ostream& cout, const ID& data)
		{
			cout << ' ';
			if (data.isTerm == 0)
			{
				if (data.type == IDEval)
				{
					cout << '|';
				}
				else
				{
					for (int ii = 0; ii < 20; ii++)
					{
						if (NonTermList[data.type][ii] == '$')
						{
							break;
						}
						cout << NonTermList[data.type][ii];
					}
				}
			}
			else
			{
				if (data.type < NumResWords)
				{
					for (int ii = 0; ii < 11; ii++)
					{
						if (ReservedWords[data.type][ii] == '$')
						{
							break;
						}
						cout << ReservedWords[data.type][ii];
					}
				}
				else
				{
					switch (data.type)
					{
					case IDaddop: cout << '+'; break;
					case IDsubop: cout << '-'; break;
					case IDmop: cout << '*'; break;
					case IDdivop: cout << '/'; break;
					case IDLRop: cout << '<'; break;
					case IDLERop: cout << "<="; break;
					case IDGRop: cout << '>'; break;
					case IDGERop: cout << ">="; break;
					case IDassign: cout << "="; break;
					case IDEQRop: cout << "=="; break;
					case IDNEQRop: cout << "!="; break;
					case IDcomma: cout << "','"; break;
					case IDsemi: cout << "';'"; break;
					case IDLP: cout << "'('"; break;
					case IDRP: cout << "')'"; break;
					case IDLB: cout << "'{'"; break;
					case IDRB: cout << "'}'"; break;
					case IDEval: cout << "(<)"; break;
					case IDWut: cout << '?'; break;
					case IDeof: cout << "EOF"; break;
					default: break;
					}
				}
			}
			return cout;
		}
	};
	/// Struct for Node list
	struct IDNode
	{
		ID data;
		explicit IDNode(ID inData) :data(inData) {};
		friend std::ostream& operator<<(std::ostream& cout, const IDNode& data)
		{
			return cout << data.data;
		}
	};
	/**
	 * Identifies terminals and non-terminals in grammar file
	 * @param 'decTable' decision table
	 * @warning 'table' must have '$' character at end of string to function properly!
	 */
	class Identifier
	{
	private:
		DecisionTable decTable;
	public:
		/**
		 * Constructor for Identifier
		 * @param[in] 'table' is the header name to access table
		 * @warning 'table' must have '$' character at end of string to function properly!
		 */
		Identifier(const char* table)
		{
			//decTable.buildTable3D(table, nPage, nRow, nCol);
			decTable.buildTable(table);
		};
		/**
		 * Parse function to identify and label unique strings and characters to construct Grammar table
		 * @param[in] 'scan' scanner used to identify strings
		 * @param[out] 'stack' storage for Non-terminals and their produced values
		 * @warning 'stack' must have two stacks initialized before use otherwise failure will occur
		 */
		void entry(Scanner* scan, Stack<Grammar::ID>* stack)
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
					if ((temp.type = checkNonTerm(&data)) < NumNonTerm)
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
	/**
	 * Builds Grammar list for use later
	 * @param 'decTable' decision table
	 */
	class GrammarListBuilder
	{
	private:
		DecisionTable decTable;
	public:
		/**
		 * Constructor
		 * @param[in] 'table' is the header name to access table
		 * @warning 'table' must have '$' character at end of string to function properly!
		 */
		GrammarListBuilder(const char* table) 
		{
			//decTable.buildTable3D(table, nPage, nRow, nCol);
			decTable.buildTable(table);
		};
		/**
		 * Build list from stack
		 * @param[out] 'List' grammar list
		 * @param[in] 'stack' information to build list
		 * @warning must call Identifier to build contents in stack before construction of list
		 */
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
/// Parse Grammar File for evaluation
void GramParse(Node<Grammar::IDNode>* List)
{
	Stack<Grammar::ID> GIDStack(255, 2);
	Scanner scan(GrammarFile, "GSCN$");
	Grammar::Identifier idScan("tGID$");
	Grammar::GrammarListBuilder buildList("tGLB$");
	idScan.entry(&scan, &GIDStack);
	if (GrammarDBG::DBG) GIDStack.print();
	buildList.entry(List, &GIDStack);
	if (GrammarDBG::DBG) GIDStack.print();
}
#endif