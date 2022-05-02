#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP
#include "Constants.h"
#include "Pass1.hpp"
#include "SyntaxPass.hpp"

class AssemblyGenerator
{
	void generateData(FileOut& fout)
	{
		SymbolTableReader Table("tSTR$");
		Data<unsigned char> name, value;
		int input = 0;
		int index = 1;
		fout.write("section\t.data\n");
		fout.write("\tuserMsg\tdb\t\'Enter an integer(less than 32,765): \'\n\tlenUserMsg\tequ\t$-userMsg\n");
		fout.write("\tdisplayMsg\tdb\t\'You entered: \'\n\tlenDisplayMsg\tequ\t$-displayMsg\n");
		fout.write("\tprintTempchar\tdb\t\'Tempchar=: \'\n\tlenprintTempchar\tequ\t$-printTempchar\n");
		fout.write("\tResult\tdb\t\'Ans=\'\n\tResultValue\tdb\t\'aaaaa\'\n\tdb\t0xA\n\tResultEnd\tequ\t$-Result\n");
		fout.write("\tnum\ttimes\t6\tdb\t\'ABCDEF\'\n\tnumEnd\tequ\t$-num\n");
		do
		{
			input = Table.getIDENT(&name, &value, TableID(index++, 0));
			if (input == 26 || input == 23)
			{
				fout.write('\t'); fout.writeData(&name); fout.write("\tdw\t"); fout.writeData(&value); fout.write('\n');
			}
		} while (input != IDeof);
	}
	void generateBSS(FileOut& fout)
	{
		SymbolTableReader Table("tSTR$");
		Data<unsigned char> name, value;
		int input = 0; int index = 1;
		fout.write("section\t.bss\n");
		fout.write("\tTempChar\tRESB\t1\n\ttestchar\tRESB\t1\n\tReadInt\tRESW\t1\n\ttempint\tRESW\t1\n\tnegflag\tRESB\t1\n");
		do
		{
			input = Table.getIDENT(&name, &value, TableID(index++, 0));
			if (input == 27)
			{
				fout.write('\t'); fout.writeData(&name); fout.write("\tresw\t1\n");
			}
		} while (input != IDeof);
	}
public:
	AssemblyGenerator() {}
	void generateASM()
	{
		Scanner scan(QuadFile,"SSCN$");
		FileOut fout(ASMFile);
		Data<unsigned char> data, data2, label;
		int input = 0;
		fout.write("sys_exit\tequ\t1\nsys_read\tequ\t3\nsys_write\tequ\t4\n");
		fout.write("stdin\tequ\t0\nstdout\tequ\t1\nstderr\tequ\t3\n");
		generateData(fout);
		generateBSS(fout);
		do
		{
			input = scan.parse(&data);
			switch (input)
			{
			case IDClass:
				scan.parse(&data); input = scan.parse(&data);
				if (input == IDName) { fout.write("\tglobal\t_start\nsection\t.text\n_start:\n"); }
				else if (input == IDWut) { fout.write("\tmov\teax,sys_exit\n\txor ebx,ebx\n\tint\t0x80\n"); }
				break;
			case IDProc:
				scan.parse(&data); input = scan.parse(&data);
				if (input == IDName) { fout.writeData(&data); fout.write(":\n"); }
				else if (input == IDWut) { fout.write("\tret\n"); }
				break;
			case IDCall:
				fout.write('\t'); fout.writeData(&data); fout.write('\t');
				scan.parse(&data); input = scan.parse(&data);
				fout.writeData(&data); fout.write('\n');
				break;
			case IDWhile:
				scan.parse(&data); input = scan.parse(&label);
				fout.write('w'); fout.writeData(&label); fout.write(":NOP\n");
				break;
			case IDDo:
				fout.write("\tjmp\tw");
				scan.parse(&data); input = scan.parse(&data); fout.writeData(&data);
				fout.write('\n'); fout.writeData(&data); fout.write(":NOP\n");
				break;
			case IDIf:
				scan.parse(&data); input = scan.parse(&label);
				break;
			case IDThen:
				scan.parse(&data); input = scan.parse(&data);
				fout.writeData(&data); fout.write(":NOP\n");
				break;
			case IDOdd:
				fout.write("\tmov\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n\tand\tax,0x0001\n\tcmp\tax,0x0001\n\tjne\t");
				fout.writeData(&label); fout.write('\n');
				break;
			case IDGet:
				fout.write("\tCALL\tGetAnInteger\n");
				fout.write("\tmov\tax,[ReadInt]\n");
				fout.write("\tmov\t[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("],ax\n");
				break;
			case IDPrint:
				fout.write("\tmov\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tCALL\tPrint\n");
				fout.write("\tmov\teax,sys_write\n");
				fout.write("\tmov\tebx,stdout\n");
				fout.write("\tmov\tecx,Result\n");
				fout.write("\tmov\tedx,ResultEnd\n");
				fout.write("\tint\t0x80\n");
				break;
			case IDaddop:
				fout.write("\tmov\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tadd\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tmov\t[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("],ax\n");
				break;
			case IDsubop:
				fout.write("\tmov\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tsub\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tmov\t[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("],ax\n");
				break;
			case IDmop:
				fout.write("\tmov\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tmov\tbx,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tmul\tbx\n");
				fout.write("\tmov\t[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("],ax\n");
				break;
			case IDdivop:
				fout.write("\tmov\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tmov\tbx,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tdiv\tbx\n");
				fout.write("\tmov\t[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("],ax\n");
				break;
			case IDLRop:
				fout.write("\tmov\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tcmp\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tjnl\t");  fout.writeData(&label); fout.write("\n");
				break;
			case IDLERop:
				fout.write("\tmov\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tcmp\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tjnle\t");  fout.writeData(&label); fout.write("\n");
				break;
			case IDGRop:
				fout.write("\tmov\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tcmp\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tjng\t");  fout.writeData(&label); fout.write("\n");
				break;
			case IDGERop:
				fout.write("\tmov\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tcmp\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tjnge\t");  fout.writeData(&label); fout.write("\n");
				break;
			case IDassign:
				fout.write("\tmov\teax,");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("\n");
				fout.write("\tmov\tbx,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tmov\t[eax],bx\n");
				break;
			case IDEQRop:
				fout.write("\tmov\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tcmp\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tjne\t");  fout.writeData(&label); fout.write("\n");
				break;
			case IDNEQRop:
				fout.write("\tmov\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tcmp\tax,[");
				scan.parse(&data); scan.parse(&data); fout.writeData(&data);
				fout.write("]\n");
				fout.write("\tje\t");  fout.writeData(&label); fout.write("\n");
				break;
			default:
				break;
			}
		} while (input != IDeof);
		fout.write("GetAnInteger:\n");
		fout.write("\tmov\teax,sys_write\n");
		fout.write("\tmov\tebx,stdout\n");
		fout.write("\tmov\tecx,userMsg\n");
		fout.write("\tmov\tedx,lenUserMsg\n");
		fout.write("\tint\t0x80\n");
		fout.write("\tmov\teax,sys_read\n");
		fout.write("\tmov\tebx,stdin\n");
		fout.write("\tmov\tecx,num\n");
		fout.write("\tmov\tedx,6\n");
		fout.write("\tint\t0x80\n");
		fout.write("CSTI:NOP\n");
		fout.write("\tmov\tax,0\n");
		fout.write("\tmov\t[ReadInt],ax\n");
		fout.write("\tmov\tecx, num\n");
		fout.write("\tmov\tbx,0\n");
		fout.write("\tmov\tbl,byte[ecx]\n");
		fout.write("Next:NOP\n");
		fout.write("\tsub\tbl,\'0\'\n");
		fout.write("\tmov\tax,[ReadInt]\n");
		fout.write("\tmov\tdx,10\n");
		fout.write("\tmul\tdx\n");
		fout.write("\tadd\tax,bx\n");
		fout.write("\tmov\t[ReadInt], ax\n");
		fout.write("\tmov\tbx,0\n");
		fout.write("\tadd\tecx,1\n");
		fout.write("\tmov\tbl,byte[ecx]\n");
		fout.write("\tcmp\tbl,0xA\n");
		fout.write("\tjne\tNext\n");
		fout.write("\tret\n");

		fout.write("Print:NOP\n");
		fout.write("\tmov\tebx,ResultValue+4\n");
		fout.write("ConvertLoop:NOP\n");
		fout.write("\tsub\tdx,dx\n");
		fout.write("\tmov\tcx,10\n");
		fout.write("\tdiv\tcx\n");
		fout.write("\tadd\tdl,\'0\'\n");
		fout.write("\tmov\t[ebx],dl\n");
		fout.write("\tdec\tebx\n");
		fout.write("\tcmp\tebx,ResultValue\n");
		fout.write("\tjge\tConvertLoop\n");
		fout.write("\tret\n");
	}
};
#endif