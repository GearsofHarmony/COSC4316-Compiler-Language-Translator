#ifndef DECISIONTABLES_HPP
#define DECISIONTABLES_HPP
#include "Constants.h"
#include "FileIO.hpp"

/**
 * Decision Table class for all parsers to utilize
 * @param 'page' page index of array
 * @param 'row' row index of array
 * @param 'col' column index of array
 * @param 'decTable' decision table for parser
 */
class DecisionTable
{
private:
	FileIn fin;
	int page, row, col;
	int* decTable;
	/**
	 * Initializes 'decTable' with a copy of input 'table'
	 * @param[in] 'table' pointer to table for copying
	 * @param[in] 'nCol' number of columns for 'table'
	 * @param[in] 'nRow' number of rows for 'table'
	 * @param[in] 'nPage' number of pages for 'table'
	 */
	void initialize(int nPage, int nRow, int nCol)
	{
		row = nRow; col = nCol; page = nPage;
		int size = page * row * col;
		if (decTable != NULL)
		{
			delete[] decTable; decTable = 0;
		}
		decTable = new int[page * row * col];
		for (int zz = 0; zz < size; zz++)
		{
			decTable[zz] = fin.readInt();
			/*for (int xx = 0; xx < row; xx++)
			{
				for (int yy = 0; yy < col; yy++)
				{
					int loc = (zz * row * col) + (xx * col) + yy;
					decTable[loc] = fin.readInt();
				}
			}*/
		}
	};
	/**
	 * Public method for user to rebuild table
	 * @param[in] 'table' pointer to table for copying
	 * @param[in] 'nCol' number of columns for 'table'
	 */
	void buildTable1D(int nCol) { initialize(1, 1, nCol); };
	/**
	 * Public method for user to rebuild table
	 * @param[in] 'table' pointer to table for copying
	 * @param[in] 'nRow' number of rows for 'table'
	 * @param[in] 'nCol' number of columns for 'table'
	 */
	void buildTable2D(int nRow, int nCol) { initialize(1, nRow, nCol); };
	/**
	 * Public method for user to rebuild table
	 * @param[in] 'table' pointer to table for copying
	 * @param[in] 'nPage' number of pages for 'table'
	 * @param[in] 'nRow' number of rows for 'table'
	 * @param[in] 'nCol' number of columns for 'table'
	 */
	void buildTable3D(int nPage, int nRow, int nCol) { initialize(nPage, nRow, nCol); };
public:
	DecisionTable() { page = row = col = 0; decTable = 0; };
	~DecisionTable() { delete[] decTable; decTable = 0; }
	void buildTable(const char* table)
	{
		fin.openFile("DecisionTablesList.txt");
		Data<unsigned char> name;
		int input = 0;
		bool finished = false;
		while (!fin.isEOF() && !finished)
		{
			name.empty();
			for (int ii = 0; ii < 4; ii++) { name.push(fin.read()); }
			int size, page = fin.readInt(), row = fin.readInt(), col = fin.readInt();
			for (int ii = 0; ii < 6; ii++)
			{
				if (table[ii] != name.read(ii))
				{
					if (table[ii] == '$' && ii > name.Size())
					{
						if (page * row * col)
						{
							buildTable3D(page, row, col);
						}
						else if (row * col)
						{
							buildTable2D(row, col);
						}
						else if (col)
						{
							buildTable1D(col);
						}
						finished = true;
					}
					else
					{
						if (size = page * row * col)
						{
							for (int xx = 0; xx < size; xx++) { fin.readInt(); }
						}
						else if (size = row * col)
						{
							for (int xx = 0; xx < size; xx++) { fin.readInt(); }
						}
						else if (col)
						{
							for (int xx = 0; xx < col; xx++) { fin.readInt(); }
						}
					}
					break;
				}
			}
		}
	}
	/**
	 * Indexer for 1D array parameters for 1D array
	 * @param[in] 'nCol' column index for 'decTable'
	 */
	int index1D(int nCol) { return decTable[nCol]; }
	/**
	 * Indexer for 2D array parameters for 1D array
	 * @param[in] 'nCol' column index for 'decTable'
	 * @param[in] 'nRow' row index for 'decTable'
	 */
	int index2D(int nRow, int nCol) { return decTable[(nRow * col) + nCol]; }
	/**
	 * Indexer for 3D array parameters for 1D array
	 * @param[in] 'nCol' column index for 'decTable'
	 * @param[in] 'nRow' row index for 'decTable'
	 * @param[in] 'nPage' page index for 'decTable'
	 */
	int index3D(int nPage, int nRow, int nCol) { return decTable[(nPage * row * col) + (nRow * col) + nCol]; }
};
#endif