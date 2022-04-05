#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "Data.hpp"

const bool DBG = 0;
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
	Other
};
/**
 * Identifiers for distinguished states for scanner
 */
enum IDENT
{
	IDName = 10,
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
	IDEval,
	IDeof
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
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::Other,	ASCII::Other,	ASCII::LP,		ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::WS,		ASCII::Other,	ASCII::RP,		ASCII::Digit,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::WS,		ASCII::Other,	ASCII::mop,		ASCII::colon,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter,	ASCII::Letter	},
	{	ASCII::WS,		ASCII::Other,	ASCII::addop,	ASCII::semi,	ASCII::Letter,	ASCII::Other,	ASCII::Letter,	ASCII::LB	},
	{	ASCII::WS,		ASCII::Other,	ASCII::comma,	ASCII::LRop,	ASCII::Letter,	ASCII::Other,	ASCII::Letter,	ASCII::Other	},
	{	ASCII::WS,		ASCII::Other,	ASCII::subop,	ASCII::assign,	ASCII::Letter,	ASCII::Other,	ASCII::Letter,	ASCII::RB	},
	{	ASCII::Other,	ASCII::Other,	ASCII::Other,	ASCII::GRop,	ASCII::Letter,	ASCII::Other,	ASCII::Letter,	ASCII::Other	},
	{	ASCII::Other,	ASCII::Other,	ASCII::divop,	ASCII::Other,	ASCII::Letter,	ASCII::Other,	ASCII::Letter,	ASCII::Other	}
};
/**
 * Reserved word list
 */
const unsigned char reservedWords[][11] =
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
	"ODD$      "
};
const int listNonTerm = 24;
const int numNonTerm = 26;
/**
 * Non-Terminals list
 */
const char nonTermList[][18] =
{
	"program$         ",
	"block$           ",
	"constdefpart$    ",
	"constlist$       ",
	"variabledefpart$ ",
	"varlist$         ",
	"proceduredefpart$",
	"stmt$            ",
	"empty$           ",
	"simplestmt$      ",
	"callstmt$        ",
	"parmlist$        ",
	"identlist$       ",
	"compoundstmt$    ",
	"stmtlist$        ",
	"ifstmt$          ",
	"whilestmt$       ",
	"be$              ",
	"relop$           ",
	"exp$             ",
	"addop$           ",
	"term$            ",
	"mop$             ",
	"fac$             ",
	"ident$           ",
	"integer$         "
};

int checkReservedWord(Data<unsigned char>* data)
{
	for (int ii = 0; ii < 10; ii++)
	{
		for (int xx = 0; xx < 11; xx++)
		{
			if (reservedWords[ii][xx] != data->read(xx))
			{
				if (reservedWords[ii][xx] == '$' && xx > data->Size())
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
	for (int ii = 0; ii < numNonTerm; ii++)
	{
		for (int xx = 0; xx < 18; xx++)
		{
			if (nonTermList[ii][xx] != data->read(xx))
			{
				if (nonTermList[ii][xx] == '$' && xx > data->Size())
				{
					return ii;
				}
				break;
			}
		}
	}
	return numNonTerm;
}
#endif