#ifndef STACK_HPP
#define STACK_HPP
#include <iostream>

int truncate(float inVal)
{
	int temp = (int)inVal;
	if ((float)temp <= inVal)
		return temp;
	else
		return temp - 1;
}
template <class DataType>
class Stack
{
private:
	int numStacks;
	DataType* stack;
	int* top;		// for 0 <= j < numStacks
	int* base;		// for 0 <= j <= numStacks when j = numStacks is top of useable memory in stack
	int* oneArray;	// for 0 <= j <= numStacks; OldTop & NewBase @ j for 0 <= j < numStacks; Growth @ j for 0 < j <= numStacks

	// initialize memory and stackspace
	void initialize(int size, int maxLen, int memLoc)
	{
		stack = new DataType[size];
		top = new int[numStacks];
		base = new int[numStacks + 1];
		oneArray = new int[numStacks + 1];
		for (int ii = 0; ii < numStacks; ii++)
		{
			oneArray[ii] = base[ii] = top[ii] = truncate(float(ii) / float(numStacks) * float(maxLen)) + memLoc;
		}
		base[numStacks] = maxLen;
	}
	void resize()
	{
		DataType* temp = new DataType[base[numStacks] + 64 + 1];
		memcpy(temp, stack, base[numStacks] * sizeof(DataType));
		base[numStacks] += 64;
		delete[] stack;
		stack = temp;
	}
	/// reallocate stack in memory
	bool reallocate()
	{
		//ReA1
		int AvailSpace = base[numStacks] - base[0];
		int TotalInc = 0;
		for (int ii = numStacks - 1; ii >= 0; ii--)
		{
			AvailSpace -= (top[ii] - base[ii]);
			if (top[ii] > oneArray[ii])
			{
				oneArray[ii + 1] = top[ii] - oneArray[ii];
				TotalInc += oneArray[ii + 1];
			}
			else
				oneArray[ii + 1] = 0;
		}
		
		//ReA2
		float equalAlloc = 0.2f;
		float growthAlloc = 1.0f - equalAlloc;
		float minSpace = (float)(base[numStacks] - base[0]) * 0.01f;
		if ((float)AvailSpace < minSpace - 1.0f)
		{
			//terminate program
			std::cout << "Reallocation failed! No memory left!";
			resize();
			AvailSpace = base[numStacks] - base[0];
			for (int ii = numStacks - 1; ii >= 0; ii--)
			{
				AvailSpace -= (top[ii] - base[ii]);
			}
			//return true;
		}

		//ReA3
		float Alpha = equalAlloc * AvailSpace / numStacks;

		//ReA4
		float Beta = growthAlloc * AvailSpace / TotalInc;

		//ReA5
		oneArray[0] = base[0];
		float Sigma = 0.0f;
		for (int ii = 1; ii < numStacks; ii++)
		{
			float Tau = Sigma + Alpha + oneArray[ii] * Beta;
			oneArray[ii] = oneArray[ii - 1] + (top[ii - 1] - base[ii - 1]) + truncate(Tau) - truncate(Sigma);
			Sigma = Tau;
		}
		return false;
	}

	/// move stack in memory
	void moveStack()
	{
		//MoA1
		for (int ii = 1; ii < numStacks; ii++)
		{
			if (oneArray[ii] < base[ii])
			{
				int Delta = base[ii] - oneArray[ii];
				for (int ll = base[ii] + 1; ll <= top[ii]; ll++)
				{
					stack[ll - Delta] = stack[ll];
				}
				base[ii] = oneArray[ii];
				top[ii] = top[ii] - Delta;
			}
		}
		//MoA2
		for (int ii = numStacks - 1; ii > 0; ii--)
		{
			if (oneArray[ii] > base[ii])
			{
				int Delta = oneArray[ii] - base[ii];
				for (int ll = top[ii]; ll > base[ii]; ll--)
				{
					stack[ll + Delta] = stack[ll];
				}
				base[ii] = oneArray[ii];
				top[ii] = top[ii] + Delta;
			}
		}
	}
public:
	/**
	 * Initialize the main memory and the number of stacks.
	 * @param 'maxLen' is the top of main memory. (Writable)
	 * @param 'inStacks' is the number of stacks to use. The default is 1.
	 * @param 'memLoc' is the starting point of memory for stackspace. The default is -1. (Not writable)
	 */
	Stack(int maxLen, int inStacks = 1, int memLoc = -1) :numStacks(inStacks)
	{
		initialize(maxLen + 1, maxLen, memLoc);
	}
	~Stack()
	{
		delete[] stack;
		delete[] base;
		delete[] top;
		delete[] oneArray;
	}
	/// process terminated if no more memory is available
	bool terminated() { return terminate; };
	/**
	 * return subsript location of a stack
	 * @param 'index' must be greater than or equal to 0. Default 0
	 * @return index of 'top'
	 */
	int topof(int index = 0) { return top[index]; }
	/**
	 * Get size of requested stack
	 * @param 'index' must be greater than or equal to 0. Default 0
	 * @return size of stack
	 */
	int getSize(int index = 0) { return top[index] - base[index]; }
	/**
	 * return if stack is empty
	 * @param 'index' must be greater than or equal to 0. Default 0
	 */
	bool isempty(int index = 0) 
	{ 
		if (base[index] == top[index])
			return true; 
		else 
			return false; 
	}
	/**
	 * Print Stack
	 * @param 'index' must be greater than or equal to 0. Default 0
	 */
	void printStack(int index = 0)
	{
		std::cout << "Stack " << index << " @ ";
		for (int ll = base[index] + 1; ll <= top[index]; ll++)
		{
			if (ll <= base[index + 1])
			{
				std::cout << ll << ") " << stack[ll];
				if (ll < top[index])
					std::cout << ", ";
			}
		}
	}
	/**
	 * print contents of all stacks in used memory
	 * @param 'index' must be greater than or equal to 0. Default 0
	 */
	void print()
	{
		for (int ii = 0; ii <= numStacks; ii++)
		{
			std::cout << 'B' << ii << '=' << base[ii];
			if (ii < numStacks)
				std::cout << ", ";
		}
		std::cout << std::endl;
		for (int ii = 0; ii < numStacks; ii++)
		{
			std::cout << 'T' << ii << '=' << top[ii];
			if (ii < numStacks - 1)
				std::cout << ", ";
		}
		std::cout << std::endl;
		for (int ii = 0; ii < numStacks; ii++)
		{
			std::cout << "OldT" << ii << '=' << oneArray[ii];
			if (ii < numStacks - 1)
				std::cout << ", ";
		}
		std::cout << std::endl;
		for (int ii = 0; ii < numStacks; ii++)
		{
			printStack(ii);
			std::cout << std::endl;
		}
	}
	/** 
	 * Push data onto top of stack 
	 * @param 'inVal'
	 * @param 'index' must be greater than or equal to 0. Default 0
	 */
	void push(DataType inVal, int index = 0)
	{
		if (++top[index] <= base[index + 1])
		{
			stack[top[index]] = inVal;
		}
		else
		{
			/*std::cout << "Error Not Enough Space!\nTransaction Causing Overflow:> " << inVal << "\nContent of Memory at Overflow\n";
			print();
			std::cout << "Reallocating...\n";*/
			if (reallocate())
			{
				resize();//return;
			}
			--top[index];
			//std::cout << "Repacking...\n";
			moveStack();
			stack[++top[index]] = inVal;
			//std::cout << "Content of Memory After Repack:\n";
			for (int ii = 0; ii < numStacks; ii++)
			{
				oneArray[ii] = top[ii];
			}
			//print();
		}
	}
	/** 
	 * pop data from top of stack
	 * @param 'index' must be greater than or equal to 0. Default 0
	 * @return entry of @tparam 'DataType'
	 */
	DataType pop(int index = 0)
	{
		if (top[index] > base[index])
			return stack[top[index]--];
		else
			return stack[base[index] + 1];
	}
};
#endif