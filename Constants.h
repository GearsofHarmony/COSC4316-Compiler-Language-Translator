#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "Data.hpp"

const char* Java0File = "mainJava0.txt";
const char* TokenFile = "TokenTable.txt";
const char* SymbolFile = "SymbolTable.txt";
const char* GrammarFile = "Grammar.txt";
const char* QuadFile = "Quads.txt";
const char* ASMFile = "mainJava0.asm";

const int NumResWords = 12;
/**
 * Reserved word list
 */
const unsigned char ReservedWords[NumResWords][11] =
{
	"CLASS$    ",
	"PROCEDURE$",
	"CONST$    ",
	"VAR$      ",
	"CALL$     ",
	"DO$       ",
	"WHILE$    ",
	"IF$       ",
	"THEN$     ",
	"ODD$      ",
	"GET$      ",
	"PRINT$    "
};
const int ListNonTerm = 22;
const int NumNonTerm = 28;
/**
 * Non-Terminals list
 */
const char NonTermList[NumNonTerm][18] =
{
	"program$         ",
	"block$           ",
	"constdefpart$    ",
	"constlist$       ",
	"variabledefpart$ ",
	"varlist$         ",
	"proceduredefpart$",
	"procedurelist$   ",
	"parmlist$        ",
	"identlist$       ",
	"stmt$            ",
	"empty$           ",
	"simplestmt$      ",
	"callstmt$        ",
	"compoundstmt$    ",
	"stmtlist$        ",
	"ifstmt$          ",
	"whilestmt$       ",
	"be$              ",
	"exp$             ",
	"term$            ",
	"fac$             ",
	"ident$           ",
	"integer$         ",
	"Pgmident$        ",
	"Procident$       ",
	"Constident$      ",
	"Varident$        "
};
/**
 * ASCII Character encoding for ASCII look-up table
 */
enum ASCII
{
	Letter = 0,
	Digit,
	WS,
	eof,
	addop,
	subop,
	mop,
	divop,
	LRop,
	GRop,
	exclam,
	assign,
	comma,
	semi,
	LP,
	RP,
	LB,
	RB,
	colon,
	dollar,
	wut,
	Other,
	TableSize
};
/**
 * Identifiers for distinguished states for scanner
 */
enum IDENT
{
	IDClass,
	IDProc,
	IDConst,
	IDVar,
	IDCall,
	IDDo,
	IDWhile,
	IDIf,
	IDThen,
	IDOdd,
	IDGet,
	IDPrint,
	IDName,
	IDNumLit,
	IDaddop,
	IDsubop,
	IDmop,
	IDdivop,
	IDLRop,
	IDLERop,
	IDGRop,
	IDGERop,
	IDEQRop,
	IDNEQRop,
	IDassign,
	IDcomma,
	IDsemi,
	IDLP,
	IDRP,
	IDLB,
	IDRB,
	IDSymb,
	IDEval,
	IDWut,
	IDeof,
	IDTableSize
};
/**
 * ASCII Look-up table
 */
const unsigned int ASCII[16][8] =
{
	{	ASCII::eof,		ASCII::Other,	ASCII::WS,		ASCII::Digit,	ASCII::Other,	ASCII::Letter,	ASCII::Other,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::exclam,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::dollar,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::LP,		ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::WS,		ASCII::Other,	ASCII::RP,		ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::WS,		ASCII::Other,	ASCII::mop,		ASCII::colon,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::WS,		ASCII::Other,	ASCII::addop,	ASCII::semi,	ASCII::Letter,	ASCII::Other,	ASCII::Letter,	ASCII::LB		},
	{	ASCII::WS,		ASCII::Other,	ASCII::comma,	ASCII::LRop,	ASCII::Letter,	ASCII::Other,	ASCII::Letter,	ASCII::Other	},
	{	ASCII::WS,		ASCII::Other,	ASCII::subop,	ASCII::assign,	ASCII::Letter,	ASCII::Other,	ASCII::Letter,	ASCII::RB		},
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::GRop,	ASCII::Letter,	ASCII::Other,	ASCII::Letter,	ASCII::Other	},
	{	ASCII::Other,	ASCII::Other,	ASCII::divop,	ASCII::wut,		ASCII::Letter,	ASCII::Other,	ASCII::Letter,	ASCII::Other	}
};

int checkReservedWord(Data<unsigned char>* data)
{
	for (int ii = 0; ii < NumResWords; ii++)
	{
		for (int xx = 0; xx < 11; xx++)
		{
			if (ReservedWords[ii][xx] != data->read(xx))
			{
				if (ReservedWords[ii][xx] == '$' && xx > data->Size())
				{
					return ii;
				}
				break;
			}
		}
	}
	return IDName;
}

int checkNonTerm(Data<unsigned char>* data)
{
	for (int ii = 0; ii < NumNonTerm; ii++)
	{
		for (int xx = 0; xx < 18; xx++)
		{
			if (NonTermList[ii][xx] != data->read(xx))
			{
				if (NonTermList[ii][xx] == '$' && xx > data->Size())
				{
					return ii;
				}
				break;
			}
		}
	}
	return NumNonTerm;
}
#endif