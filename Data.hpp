#ifndef DATA_HPP
#define DATA_HPP

/**
 * Data functions as a dynamic reallocation stack OR queue
 * @tparam 'Type' data type for data
 * @param 'size' is initial and current memory allocated
 * @param 'top' is current top of stack where -1 is empty
 * @param 'data' a pointer to memory location
 */
template<typename Type>
class Data
{
private:
	int size = 32;
	int top = -1;
	Type* data = 0;
	/**
	 * Reallocates memory
	 */
	void reAlloc()
	{
		Type* temp = new Type[size+=32];
		for (int ii = 0; ii <= top; ii++)
			temp[ii] = data[ii];
		delete[] data; data = temp;
	}
public:
	Data() { data = new Type[size]; };
	~Data() { delete[] data; data = 0; };
	Data& operator=(const Data& inVal)
	{
		if (inVal.size > this->size)
		{
			this->size = inVal.size;
			this->reAlloc();
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
	void push(Type input)
	{
		if (top + 1 >= size)
		{
			reAlloc();
		}
		data[++top] = input;
	}
	/**
	 * Pop entry from stack
	 * @return top entry from stack before decrementing 'top'
	 * @throw basic if 'data' is empty the value at index 0 is returned
	 */
	Type pop()
	{
		if (top >= 0)
			return data[top--];
		else
			return data[0];
	}
	/**
	 * Read from 'data' bottom-to-top without modifying memory
	 * @param[in] 'index' is index for entry. Default = 'top'
	 * @return entry at 'index' of 'data'
	 * @throw basic if 'index' is NOT greater than or equal to 0 And less than or equal to top
	 * then the value at index 0 is returned
	 */
	Type read(int index = top)
	{
		if (index >= 0 && index <= top)
			return data[index];
		else
			return data[0];
	}
};
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
	int page, row, col;
	int* decTable;
	/**
	 * Initializes 'decTable' with a copy of input 'table'
	 * @param[in] 'table' pointer to table for copying
	 * @param[in] 'nCol' number of columns for 'table'
	 * @param[in] 'nRow' number of rows for 'table'
	 * @param[in] 'nPage' number of pages for 'table'
	 */
	void initialize(const int* table, int nPage, int nRow, int nCol)
	{
		row = nRow; col = nCol; page = nPage;
		decTable = new int[page * row * col];
		for (int zz = 0; zz < page; zz++)
		{
			for (int xx = 0; xx < row; xx++)
			{
				for (int yy = 0; yy < col; yy++)
				{
					int loc = (zz * row * col) + (xx * col) + yy;
					decTable[loc] = table[loc];
				}
			}
		}
	};
public:
	DecisionTable() { page = row = col = 0; decTable = 0; };
	~DecisionTable() { delete[] decTable; decTable = 0; }
	/**
	 * Public method for user to rebuild table
	 * @param[in] 'table' pointer to table for copying
	 * @param[in] 'nCol' number of columns for 'table'
	 */
	void buildTable1D(const int* table, int nCol) { initialize(table, 1, 1, nCol); };
	/**
	 * Public method for user to rebuild table
	 * @param[in] 'table' pointer to table for copying
	 * @param[in] 'nRow' number of rows for 'table'
	 * @param[in] 'nCol' number of columns for 'table'
	 */
	void buildTable2D(const int* table, int nRow, int nCol) { initialize(table, 1, nRow, nCol); };
	/**
	 * Public method for user to rebuild table
	 * @param[in] 'table' pointer to table for copying
	 * @param[in] 'nPage' number of pages for 'table'
	 * @param[in] 'nRow' number of rows for 'table'
	 * @param[in] 'nCol' number of columns for 'table'
	 */
	void buildTable3D(const int* table, int nPage, int nRow, int nCol) { initialize(table, nPage, nRow, nCol); };
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
