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
	int size = 16;
	int top = -1;
	Type* data = 0;
	/**
	 * Reallocates memory
	 */
	void reAlloc()
	{
		Type* temp = new Type[size+=16];
		for (int ii = 0; ii <= top; ii++)
			temp[ii] = data[ii];
		delete[] data; data = temp;
	}
public:
	Data() { data = new Type[size]; };
	~Data() { if (data != NULL) { delete[] data; data = 0; } };
	Data& operator=(const Data& inVal)
	{
		this->empty();
		for (int zz = 0; zz <= inVal.top; zz++)
			this->push(inVal.data[zz]);
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
#endif
