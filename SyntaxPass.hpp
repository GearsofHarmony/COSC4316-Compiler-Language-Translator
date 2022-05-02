#ifndef SYNTAXPASS_HPP
#define SYNTAXPASS_HPP
#include "Constants.h"
#include "Data.hpp"
#include "Stack.hpp"
#include "Pass1.hpp"
#include "Grammar.hpp"
#include "PrecedenceGenerator.hpp"

namespace SyntaxDBG { const bool DBG = 0; };
struct TableID
{
	int index = 0;
	int type = 0;
	TableID() {};
	explicit TableID(int inIndex, int inSize = 0) :index(inIndex), type(inSize) {};
	friend std::ostream& operator<<(std::ostream& cout, const TableID& data)
	{
		return cout << data.index << ' ' << data.type << ' ';
	}
};
struct ObjID
{
	TableID name;
	Grammar::ID id;
	ObjID() {}
	explicit ObjID(TableID inName, Grammar::ID inID)
	{
		this->name = inName;
		this->id = inID;
	}
	ObjID& operator=(const ObjID& inVal)
	{
		this->name = inVal.name;
		this->id = inVal.id;
		return *this;
	}
	friend std::ostream& operator<<(std::ostream& cout, const ObjID& data)
	{
		return cout << data.id << ' ' << data.name;
	}
};

Grammar::ID getID(int input, Data<unsigned char>* data)
{
	Grammar::ID temp;
	const char termList[][8] =
	{
		"relop$ ",
		"addop$ ",
		"mop$   ",
		"assign$",
		"comma$ ",
		"semi$  ",
		"LP$    ",
		"RP$    ",
		"LB$    ",
		"RB$    "
	};
	for (int ii = 0; ii < 7; ii++)
	{
		for (int xx = 0; xx < 8; xx++)
		{
			if (termList[ii][xx] != data->read(xx))
			{
				if (termList[ii][xx] == '$' && xx > data->Size())
				{
					temp.type = input; temp.isTerm = 1;
					return temp;
				}
				break;
			}
		}
	}
	if ((temp.type = checkNonTerm(data)) < NumNonTerm)
	{
		temp.isTerm = 0;
	}
	else
	{
		temp.isTerm = 1;
		temp.type = input;
	}
	return temp;
}

struct Entry
{
	Data<unsigned char> name;
	Grammar::ID type;
	Data<unsigned char> value;
	bool isDS;
	int memLoc;
};

class SymbolTableReader
{
private:
	DecisionTable decTable;
	int scanSymTable(Scanner& scan, Entry& data)
	{
		const char* DS = "DS$";
		const char* CS = "CS$";
		Data<unsigned char> temp;
		int nextState = 0;
		int input = 0;
		bool finished = false;
		while (!finished)
		{
			switch (nextState)
			{
			case 0: nextState = decTable.index2D(nextState, input = scan.parse(&data.name)); break;
			case 1: nextState = decTable.index2D(nextState, scan.parse(&temp)); break;
			case 2: nextState = decTable.index2D(nextState, scan.parse(&temp)); break;
			case 3: data.type = getID(input, &temp); nextState = decTable.index2D(nextState, scan.parse(&temp)); break;
			case 4: data.value = temp; nextState = decTable.index2D(nextState, scan.parse(&temp)); break;
			case 5:
				for (int xx = 0; xx < 4; xx++)
				{
					if (DS[xx] != temp.read(xx))
					{
						if (DS[xx] == '$' && xx > temp.Size())
						{
							data.isDS = 1;
						}
						break;
					}
				}
				for (int xx = 0; xx < 4; xx++)
				{
					if (CS[xx] != temp.read(xx))
					{
						if (CS[xx] == '$' && xx > temp.Size())
						{
							data.isDS = 0;
						}
						break;
					}
				}
				nextState = decTable.index2D(nextState, scan.parse(&temp));
				break;
			case 6: finished = true;
				data.memLoc = (temp.read(0) - 0x30);
				for (int ii = 1; ii <= temp.Size(); ii++)
				{
					data.memLoc = 10 * (temp.read(ii) - 0x30);
				}
			default: finished = true; break;
			};
		}
		return input;
	}
public:
	SymbolTableReader(const char* table)
	{
		decTable.buildTable(table);
	}
	TableID find(Data<unsigned char>* inVal)
	{
		Scanner scan(SymbolFile, "SSCN$");
		int tableIndex = 0;
		int input = 0;
		inVal->push('$');
		do
		{
			Entry data;
			input = scanSymTable(scan, data);
			for (int ii = 0; ii <= inVal->Size(); ii++)
			{
				if (inVal->read(ii) != data.name.read(ii))
				{
					if (inVal->read(ii) == '$' && ii > data.name.Size())
					{
						inVal = &data.name;
						input = IDeof;
						switch (data.type.type)
						{
						case 23: return TableID(tableIndex, 1);
						case 24: return TableID(tableIndex, 2);
						case 25: return TableID(tableIndex, 3);
						case 26: return TableID(tableIndex, 4);
						case 27: return TableID(tableIndex, 5);
						default: return TableID(tableIndex, 0);
						}
					}
					break;
				}
			}
			for (int ii = 0; ii <= inVal->Size(); ii++)
			{
				if (inVal->read(ii) != data.value.read(ii))
				{
					if (inVal->read(ii) == '$' && ii > data.value.Size())
					{
						inVal = &data.value;
						input = IDeof;
						switch (data.type.type)
						{
						case 23: return TableID(tableIndex, 1);
						case 24: return TableID(tableIndex, 2);
						case 25: return TableID(tableIndex, 3);
						case 26: return TableID(tableIndex, 4);
						case 27: return TableID(tableIndex, 5);
						default: return TableID(tableIndex, 0);
						}
					}
					break;
				}
			}
			tableIndex++;
		} while (input != IDeof);
		return TableID(0, 0);
	}
	void check(Data<unsigned char>* inVal, TableID seek)
	{
		Scanner scan(SymbolFile, "SSCN$");
		int input;
		int tableIndex = 0;
		do
		{
			Entry data;
			input = scanSymTable(scan, data);
			if (seek.index == tableIndex)
			{
				*inVal = data.name;
				input = IDeof;
			}
			tableIndex++;
		} while (input != IDeof);
	}
	int getIDENT(Data<unsigned char>* name, Data<unsigned char>* value, TableID seek)
	{
		Scanner scan(SymbolFile, "SSCN$");
		int input;
		int tableIndex = 0;
		do
		{
			Entry data;
			input = scanSymTable(scan, data);
			if (seek.index == tableIndex)
			{
				*name = data.name;
				*value = data.value;
				return data.type.type;
			}
			tableIndex++;
		} while (input != IDeof);
		return input;
	}
	void fillStack(Stack<ObjID>& stack)
	{
		Scanner scan(TokenFile, "SSCN$");
		int input;
		do
		{
			Data<unsigned char> temp;
			Data<unsigned char> data;
			int nextState = 0;
			input = 0;
			bool finished = false;
			while (!finished)
			{
				switch (nextState)
				{
				case 0: nextState = decTable.index2D(nextState, input = scan.parse(&data)); break;
				case 1: nextState = decTable.index2D(nextState, scan.parse(&temp)); break;
				case 2: nextState = decTable.index2D(nextState, scan.parse(&temp)); break;
				case 3: finished = true; stack.push(ObjID(find(&data), getID(input, &temp))); break;
				default: finished = true; break;
				};
			}
		} while (input != IDeof);
	}
};

struct Quad
{
	ObjID op;
	ObjID opa1;
	ObjID opa2;
	ObjID opa3;
	Quad() {};
	explicit Quad(ObjID inVal1, ObjID inVal2, ObjID inVal3, ObjID inVal4)
	{
		op = inVal1;
		opa1 = inVal2;
		opa2 = inVal3;
		opa3 = inVal4;
	}
	Quad& operator=(const Quad& inVal)
	{
		this->op = inVal.op;
		this->opa1 = inVal.opa1;
		this->opa2 = inVal.opa2;
		this->opa3 = inVal.opa3;
		return *this;
	}
	friend std::ostream& operator<<(std::ostream& cout, const Quad& data)
	{
		SymbolTableReader symbolTable("tSTR$");
		Data<unsigned char> temp;
		switch (data.op.id.type)
		{
		case IDClass:
		case IDProc:
			cout << data.op.id << ',';
			if (Grammar::ID(1, IDWut).cmp(data.opa1.id))
			{
				cout << data.opa1.id;
			}
			else
			{
				cout << ' ';
				symbolTable.check(&temp, data.opa1.name);
				for (int ii = 0; ii <= temp.Size(); ii++)
				{
					cout << temp.read(ii);
				}
			}
			cout << ',' << data.opa2.id << ',' << data.opa3.id;
			break;
		case IDCall:
			cout << data.op.id << ", ";
			symbolTable.check(&temp, data.opa1.name);
			for (int ii = 0; ii <= temp.Size(); ii++)
			{
				cout << temp.read(ii);
			}
			cout << ',' << data.opa2.id << ',' << data.opa3.id;
			break;
		case IDWhile:
		case IDDo:
		case IDIf:
		case IDThen:
			cout << data.op.id << ',';
			if (Grammar::ID(1, IDWut).cmp(data.opa1.id))
			{
				cout << data.opa1.id;
			}
			else
			{
				cout << data.opa1.id << data.opa1.name.index;
			}
			cout << ',' << data.opa2.id << ',' << data.opa3.id;
			break;
		case IDOdd:
			cout << data.op.id << ", ";
			symbolTable.check(&temp, data.opa1.name);
			for (int ii = 0; ii <= temp.Size(); ii++)
			{
				cout << temp.read(ii);
			}
			cout << ',' << data.opa2.id << ',' << data.opa3.id;
			break;
		case IDGet:
		case IDPrint:
			cout << data.op.id << ", ";
			symbolTable.check(&temp, data.opa1.name);
			for (int ii = 0; ii <= temp.Size(); ii++)
			{
				cout << temp.read(ii);
			}
			cout << data.opa2.id << ',' << data.opa3.id; 
			break;
		case IDaddop:
		case IDsubop:
		case IDmop:
		case IDdivop:
			cout << data.op.id << ", ";
			symbolTable.check(&temp, data.opa1.name);
			for (int ii = 0; ii <= temp.Size(); ii++)
			{
				cout << temp.read(ii);
			}cout << ", ";
			symbolTable.check(&temp, data.opa2.name);
			for (int ii = 0; ii <= temp.Size(); ii++)
			{
				cout << temp.read(ii);
			}cout << ", ";
			symbolTable.check(&temp, data.opa3.name);
			for (int ii = 0; ii <= temp.Size(); ii++)
			{
				cout << temp.read(ii);
			}
			break;
		case IDLRop:
		case IDLERop:
		case IDGRop:
		case IDGERop:
		case IDassign:
		case IDEQRop:
		case IDNEQRop:
			cout << data.op.id << ", ";
			symbolTable.check(&temp, data.opa1.name);
			for (int ii = 0; ii <= temp.Size(); ii++)
			{
				cout << temp.read(ii);
			}cout << ", ";
			symbolTable.check(&temp, data.opa2.name);
			for (int ii = 0; ii <= temp.Size(); ii++)
			{
				cout << temp.read(ii);
			}
			cout << ',' << data.opa3.id;
			break;
		default:
			cout << ' ';
			symbolTable.check(&temp, data.op.name);
			for (int ii = 0; ii <= temp.Size(); ii++)
			{
				cout << temp.read(ii);
			}
			cout << ',' << data.opa1.id << data.opa1.name.index << ',' << data.opa2.id << ',' << data.opa3.id;
			break;
		}
		return cout << '\n';
	}
};

class SyntaxPass
{
private:
	FileOut fout;
	DecisionTable decTable;
	SymbolTableReader* symbolTable = 0;
	Data<Quad> idStack, qStack, stmtStack;
	Data<ObjID> BE;
	bool terminated = false;
	int TVar = 0;
	int be = 1;
	int Loop = 1;
	bool checkHandle(Stack<ObjID>& stack, PrecTableGen& precTable, Node<Grammar::IDNode>* List)
	{
		MatN tempMat;
		ObjID YeildTo; YeildTo.id.type = IDEval; YeildTo.id.isTerm = 1;
		Data<ObjID> data;
		do
		{
			data.push(stack.pop());
		} while (!data.read(data.Size()).id.cmp(YeildTo.id));
		stack.push(data.pop());
		if (SyntaxDBG::DBG)
		{
			for (int ii = 0; ii <= data.Size(); ii++)
			{
				std::cout << data.read(ii) << ' ';
			}std::cout << '\n';
		}
		ObjID check, current = data.read(data.Size());
		tempMat = precTable.generateFirst(List);
		for (int ii = 0; ii <= 1; ii++)
		{
			if (data.read(data.Size() - ii).id.isTerm)
			{
				current = data.read(data.Size() - ii);
				tempMat = precTable.generateFirstTerm(List);
				break;
			}
		}
		for (int index = ListNonTerm - 1; index >= 0; index--)
		{
			if (tempMat.mat[index][decTable.index2D(current.id.isTerm, current.id.type)])
			{
				Data<ObjID> temp;
				int size = Node<Grammar::IDNode>::getSize(&List[index]);
				for (int xx = 0; xx < size; xx++)
				{
					bool test = true;
					check.id = Node<Grammar::IDNode>::getData(&List[index], xx).data;
					if (check.id.cmp(Grammar::ID(0, IDEval)))
					{
						for (int ii = 0; ii <= temp.Size(); ii++)
						{
							check = data.read(data.Size() - ii);
							if (data.Size() != temp.Size())
							{
								test = false;
								temp.empty();
							}
							else if (!check.id.cmp(temp.read(ii).id))
							{
								if (data.Size() > 0)
								{
									if (!warshall(precTable.generateFirst(List)).mat[decTable.index2D(temp.read(ii).id.isTerm, temp.read(ii).id.type)][decTable.index2D(check.id.isTerm, check.id.type)])
									{
										test = false;
										temp.empty();
									}
								}
								else
								{
									test = false;
									temp.empty();
								}
							}
						}
						if (test)
						{
							stack.pop();
							check.id.isTerm = 0; check.id.type = index;
							if (data.Size() > 0)
							{
								for (int ii = 0; ii <= data.Size(); ii++)
								{
									Data<unsigned char> temp;
									if (data.read(ii).id.isTerm)
									{
										switch (data.read(ii).id.type)
										{
										case IDClass:
											while (qStack.Size() >= 0 && !qStack.read(qStack.Size()).op.id.cmp(Grammar::ID(1, IDProc)))
											{
												stmtStack.push(qStack.pop());
											}
											while (qStack.Size() >= 0)
											{
												idStack.push(qStack.pop());
											}
											qStack.push(Quad(data.read(ii), data.read(ii - 1), ObjID(TableID(0, 0), Grammar::ID(1, IDWut)), ObjID(TableID(0, 0), Grammar::ID(1, IDWut))));
											while (stmtStack.Size() >= 0)
											{
												qStack.push(stmtStack.pop());
											}
											qStack.push(Quad(data.read(ii), ObjID(TableID(0, 0), Grammar::ID(1, IDWut)), ObjID(TableID(0, 0), Grammar::ID(1, IDWut)), ObjID(TableID(0, 0), Grammar::ID(1, IDWut))));
											while (idStack.Size() >= 0)
											{
												qStack.push(idStack.pop());
											}
											break;
										case IDProc:
											while (qStack.Size() >= 0 && !qStack.read(qStack.Size()).op.id.cmp(Grammar::ID(1, IDProc)))
											{
												idStack.push(qStack.pop());
											}
											qStack.push(Quad(data.read(ii), data.read(ii - 1), ObjID(TableID(0, 0), Grammar::ID(1, IDWut)), ObjID(TableID(0, 0), Grammar::ID(1, IDWut))));
											while (idStack.Size() >= 0)
											{
												qStack.push(idStack.pop());
											}
											qStack.push(Quad(data.read(ii), ObjID(TableID(0, 0), Grammar::ID(1, IDWut)), ObjID(TableID(0, 0), Grammar::ID(1, IDWut)), ObjID(TableID(0, 0), Grammar::ID(1, IDWut))));
											break;
										case IDCall:
											qStack.push(Quad(data.read(ii), data.read(ii - 1), ObjID(TableID(0, 0), Grammar::ID(1, IDWut)), ObjID(TableID(0, 0), Grammar::ID(1, IDWut))));
											break;
										case IDWhile:
										case IDIf:
											while(qStack.Size() >= 0)
											{
												if (qStack.read(qStack.Size()).op.id.cmp(BE.read(BE.Size()).id))
												{
													if (qStack.read(qStack.Size() - 1).op.id.cmp(Grammar::ID(1, IDIf)) || qStack.read(qStack.Size() - 1).op.id.cmp(Grammar::ID(1, IDWhile)))
													{
														idStack.push(qStack.pop());
													}
													else
													{
														idStack.push(qStack.pop());
														break;
													}
												}
												idStack.push(qStack.pop());
											}
											qStack.push(Quad(data.read(ii), data.read(ii - 1), ObjID(TableID(0, 0), Grammar::ID(1, IDWut)), ObjID(TableID(0, 0), Grammar::ID(1, IDWut))));
											if (data.read(ii - 4).id.cmp(Grammar::ID(1, IDsemi)))
											{
												while (!qStack.read(qStack.Size()).op.id.cmp(Grammar::ID(1, IDassign)))
												{
													qStack.push(idStack.pop());
												}
												qStack.push(Quad(data.read(ii - 2), data.read(ii - 1), ObjID(TableID(0, 0), Grammar::ID(1, IDWut)), ObjID(TableID(0, 0), Grammar::ID(1, IDWut))));
												while (idStack.Size() >= 0)
												{
													qStack.push(idStack.pop());
												}
											}
											else
											{
												while (idStack.Size() >= 0)
												{
													qStack.push(idStack.pop());
												}
												qStack.push(Quad(data.read(ii - 2), data.read(ii - 1), ObjID(TableID(0, 0), Grammar::ID(1, IDWut)), ObjID(TableID(0, 0), Grammar::ID(1, IDWut))));
											}
											BE.pop(); break;
										case IDOdd:
											qStack.push(Quad(data.read(ii), data.read(ii - 1), ObjID(TableID(0, 0), Grammar::ID(1, IDWut)), ObjID(TableID(0, 0), Grammar::ID(1, IDWut))));
											BE.push(data.read(ii));
											check.name.index = be++;
											data.empty(); break;
										case IDGet:
										case IDPrint:
											qStack.push(Quad(data.read(ii), data.read(ii - 1), ObjID(TableID(0, 0), Grammar::ID(1, IDWut)), ObjID(TableID(0, 0), Grammar::ID(1, IDWut))));
											data.empty(); break;
										case IDaddop:
										case IDsubop:
										case IDmop:
										case IDdivop:
											temp.empty(); temp.push('T'); temp.push((0x30 + ++TVar));
											check.name = symbolTable->find(&temp);
											qStack.push(Quad(data.read(ii), data.read(ii + 1), data.read(ii - 1), check));
											data.empty(); break;
										case IDLRop:
										case IDLERop:
										case IDGRop:
										case IDGERop:
											qStack.push(Quad(data.read(ii), data.read(ii + 1), data.read(ii - 1), ObjID(TableID(0, 0), Grammar::ID(1, IDWut))));
											BE.push(data.read(ii));
											check.name.index = be++;
											data.empty(); break;
										case IDassign:
											temp.empty();
											symbolTable->check(&temp, data.read(ii + 1).name);
											check.name = symbolTable->find(&temp);
											if (check.name.type == 4 && (!check.id.cmp(Grammar::ID(0, 2)) && !check.id.cmp(Grammar::ID(0, 3))))
											{
												std::cout << "Cannot redefine Constant Variable!\n";
											}
											else if (check.name.type != 4)
											{
												qStack.push(Quad(data.read(ii), data.read(ii + 1), data.read(ii - 1), ObjID(TableID(0, 0), Grammar::ID(1, IDWut))));
											}
											TVar = 0; data.empty(); break;
										case IDEQRop:
										case IDNEQRop:
											qStack.push(Quad(data.read(ii), data.read(ii + 1), data.read(ii - 1), ObjID(TableID(0, 0), Grammar::ID(1, IDWut))));
											BE.push(data.read(ii));
											check.name.index = be++;
											data.empty(); break;
										};
									}
								}
							}
							else
							{
								check.name = current.name;
							}
							stack.push(check, 1);
							if (SyntaxDBG::DBG) { std::cout << check << '\n'; };
							if (SyntaxDBG::DBG) { for (int ii = 0; ii <= qStack.Size(); ii++) { std::cout << qStack.read(ii); } std::cout << '\n'; }
							return true;
						}
					}
					else
					{
						temp.push(check);
					}
				}
			}
		}
		while (data.Size() >= 0)
		{
			stack.push(data.pop());
		}
		return false;
	}
	void correctHandle(Stack<ObjID>& stack, PrecTableGen& precTable, Node<Grammar::IDNode>* List)
	{
		MatN matPG = precTable.PGTable(List);
		ObjID YeildTo = ObjID(TableID(0, 0), Grammar::ID(1, IDEval));
		ObjID EMPTY = ObjID(TableID(0, 0), Grammar::ID(0, 11));
		ObjID temp;
		Data<ObjID> data, result;
		do
		{
			data.push(stack.pop());
		} while (!data.read(data.Size()).id.cmp(YeildTo.id));
		stack.push(data.pop());
		if (SyntaxDBG::DBG)
		{
			std::cout << "ReEval!\n";
			for (int ii = 0; ii <= data.Size(); ii++)
			{
				std::cout << data.read(ii) << ' ';
			}std::cout << '\n';
		}
		ObjID current = data.pop();
		stack.push(current);
		do
		{
			ObjID next = data.pop();
			int nextState = matPG.mat[decTable.index2D(current.id.isTerm, current.id.type)][decTable.index2D(next.id.isTerm, next.id.type)];
			if (SyntaxDBG::DBG)
			{
				std::cout << current;
				switch (nextState)
				{
				case 1: std::cout << "(>) "; break;
				case 2: std::cout << "(<) "; break;
				case 4: std::cout << "(=) "; break;
				default: std::cout << ' ' << nextState << ' '; break;
				}std::cout << next << '\n';
			}
			switch (nextState)
			{
			case 0:
				if (precTable.testTable(decTable.index2D(EMPTY.id.isTerm, EMPTY.id.type), List).mat[decTable.index2D(current.id.isTerm, current.id.type)][decTable.index2D(EMPTY.id.isTerm, EMPTY.id.type)])
				{
					for (int row = temp.id.isTerm; row < 2; row++)
					{
						for (int col = temp.id.type; col < NumNonTerm; col++)
						{
							if (precTable.testTable(decTable.index2D(EMPTY.id.isTerm, EMPTY.id.type), List).mat[decTable.index2D(temp.id.isTerm = row, temp.id.type = col)][decTable.index2D(EMPTY.id.isTerm, EMPTY.id.type)])
							{
								if (matPG.mat[decTable.index2D(temp.id.isTerm, temp.id.type)][decTable.index2D(next.id.isTerm, next.id.type)])
								{
									data.push(next);
									data.push(temp);
									next = current;
									row = 2;
									break;
								}
							}
						}
					}
				}
				else
				{
					std::cout << "Error!\n";
					terminated = true;
				}
				break;
			case 1:
				data.push(next);
				for (int ii = 0; ii <= data.Size(); ii++)
				{
					stack.push(data.read(ii), 1);
				}
				data.empty();
				if (!checkHandle(stack, precTable, List))
				{
					std::cout << "Error!\n";
					terminated = true;
				}
				break;
			case 2: stack.push(YeildTo); stack.push(next); break;
			case 3:
				if (!checkHandle(stack, precTable, List))
				{
					std::cout << "Error!\n";
					terminated = true;
				}
				else
				{
					data.push(next);
					next = stack.pop(1);
					stack.push(YeildTo);
					stack.push(next);
				}
				break;
			case 4: stack.push(next); break;
			case 5:
				data.push(next);
				while (data.Size() >= 0)
				{
					stack.push(data.pop());
				}
				while (!checkHandle(stack, precTable, List))
				{
					stack.push(next = stack.pop(), 1);
				}
				if (!next.id.cmp(YeildTo.id))
				{
					stack.push(stack.pop(1));
				}
				if (SyntaxDBG::DBG) stack.print();
				break;
			case 6: stack.push(next); break;
			}
			current = next;
		} while (data.Size() >= 0);
		if (SyntaxDBG::DBG) stack.print();
	}
public:
	SyntaxPass(const char* outFile, const char* table)
	{
		fout.openFile(outFile);
		decTable.buildTable(table);
	}
	~SyntaxPass()
	{
		delete symbolTable; symbolTable = 0;
	}
	void Pass2(PrecTableGen& precTable)
	{
		symbolTable = new SymbolTableReader("tSTR$");
		Stack<ObjID> stack(255, 2);
		symbolTable->fillStack(stack);
		ObjID YeildTo = ObjID(TableID(0, 0), Grammar::ID(1, IDEval));
		ObjID EMPTY = ObjID(TableID(0, 0), Grammar::ID(0, 11));
		stack.push(EMPTY);
		while (!stack.isempty())
		{
			stack.push(stack.pop(), 1);
		}
		stack.push(YeildTo);
		stack.push(stack.pop(1));
		if (SyntaxDBG::DBG) { stack.print(); };
		typedef Node<Grammar::IDNode> Node;
		Node List[ListNonTerm];
		GramParse(List);
		if (SyntaxDBG::DBG)
		{
			for (int ii = 0; ii < ListNonTerm; ii++)
			{
				for (int xx = 0; xx < 20; xx++)
				{
					if (NonTermList[ii][xx] == '$')
					{
						break;
					}
					std::cout << NonTermList[ii][xx];
				}
				std::cout << " ::= ";
				Node::printList(&List[ii]);
				std::cout << '\n';
			}
			std::cout << std::endl;
		}
		for (int ii = 0; ii < ListNonTerm; ii++)
		{
			Node::insertRight(&List[ii], Grammar::IDNode(Grammar::ID(0, IDEval)));
		}
		if (SyntaxDBG::DBG)
		{
			printMat(precTable.OPG(List), "OPG\n");
			printMat(precTable.PGTable(List), "PG\n");
			printMat(precTable.generateWithin(List), "Within\n");
			printMat(precTable.testTable(decTable.index2D(EMPTY.id.isTerm, EMPTY.id.type), List), "Empty\n");
		}
		int counter = 0;
		do
		{
			Data<ObjID> data;
			bool finished = false;
			int nextState = 0;
			int start = 0;
			MatN tempMat;
			ObjID next, current = stack.pop(); stack.push(current);
			while (!finished)
			{
				switch (nextState)
				{
				case 0:
					if (SyntaxDBG::DBG) std::cout << "Test Precedence!\n";
					next = stack.pop(1);
					while (!next.id.isTerm || next.id.cmp(YeildTo.id))
					{
						if (next.id.cmp(EMPTY.id))
						{
							break;
						}
						stack.push(next);
						next = stack.pop(1);
					}
					nextState = precTable.OPG(List).mat[decTable.index2D(current.id.isTerm, current.id.type)][decTable.index2D(next.id.isTerm, next.id.type)];
					if (nextState == 0)
					{
						if (SyntaxDBG::DBG)
						{
							std::cout << current;
							switch (nextState)
							{
							case 1: std::cout << "(>) "; break;
							case 2: std::cout << "(<) "; break;
							case 4: std::cout << "(=) "; break;
							default: std::cout << ' ' << nextState << ' '; break;
							}std::cout << next << '\n';
						}
						stack.push(next, 1);
						finished = true;
						counter++;
						if (counter == 5)
						{
							counter = 0;
							stack.push(stack.pop(1));
							correctHandle(stack, precTable, List);
						}
						break;
					}
					counter = 0;
					if (nextState == 1 || nextState == 3)
					{
						stack.push(next, 1);
					}
					if (SyntaxDBG::DBG)
					{
						std::cout << current;
						switch (nextState)
						{
						case 1: std::cout << "(>) "; break;
						case 2: std::cout << "(<) "; break;
						case 4: std::cout << "(=) "; break;
						default: std::cout << ' ' << nextState << ' '; break;
						}std::cout << next << '\n'; stack.print();
					}
					break;
				case 1: finished = true;
					if (!checkHandle(stack, precTable, List))
					{
						correctHandle(stack, precTable, List);
					}
					break;
				case 2: finished = true;
					do
					{
						data.push(stack.pop());
					} while (!data.read(data.Size()).id.cmp(current.id));
					stack.push(data.pop());
					stack.push(YeildTo);
					while (data.Size() >= 0)
					{
						stack.push(data.pop());
					}
					stack.push(next); break;
					data.empty();
				case 3: finished = true;
					do
					{
						data.push(stack.pop());
					} while (!data.read(data.Size()).id.cmp(current.id));
					stack.push(data.pop());
					for (int ii = 0; ii <= data.Size(); ii++)
					{
						stack.push(data.read(ii), 1);
					}
					data.empty();
					stack.push(YeildTo, 1); 
					if (!checkHandle(stack, precTable, List))
					{
						correctHandle(stack, precTable, List);
					}
					break;
				case 4: finished = true; stack.push(next); break;
				case 5: finished = true;
					if (!current.id.isTerm)
					{
						stack.push(next, 1);
					}
					else if (current.id.isTerm && next.id.isTerm)
					{
						stack.push(next);
					}
					if (!checkHandle(stack, precTable, List))
					{
						correctHandle(stack, precTable, List);
					}
					break;
				case 6: nextState = 2; break;
				default: finished = true; break;
				};
			}
		} while (!stack.isempty() && !terminated);
		if (SyntaxDBG::DBG) stack.print();
		fout.writeData(&qStack);
	}
};

void Pass2()
{
	PrecTableGen precTable("tPRC$");
	SyntaxPass syntax(QuadFile, "tPRC$");
	syntax.Pass2(precTable);
}
#endif