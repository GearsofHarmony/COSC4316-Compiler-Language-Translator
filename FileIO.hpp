#ifndef FILEIO_HPP
#define FILEIO_HPP
#include<iostream>
#include<fstream>
#include"Constants.h"
#include"Data.hpp"

/**
 * Simple read file class with buffer for input
 * @param 'fin' input file stream
 * @param 'buffer' is the input buffer window of 4 bytes
 * @param 'numbits' number of bits in the buffer
 */
class FileIn
{
private:
	std::ifstream fin;
	unsigned int buffer; int numbits;
	/**
	 * fill the buffer to full
	 * @throw basic stop filling the buffer when 'numbits' equal to 32 or when EOF is reached
	 */
	void fillBuffer()
	{
		unsigned char temp;
		do
		{
			if (buffer < (1U << numbits))
			{
				fin >> std::noskipws >> temp;
				if (!isEOF()) buffer |= temp << numbits;
				numbits += 8;
			}
		} while (numbits <= 24 && !isEOF());
	}
	/**
	 * Read number of bits from buffer
	 * @param[in] 'num' number of bits to read
	 */
	unsigned char receive(const int num)
	{
		unsigned int temp = 0;
		if (numbits < num) { fillBuffer(); }
		temp = buffer & ((1 << num) - 1);
		buffer = buffer >> num;
		numbits -= num;
		return temp;
	}
public:
	FileIn() { buffer = 0; numbits = 0; };
	FileIn(const char* inFile) { buffer = 0; numbits = 0; openFile(inFile); };
	~FileIn() { fin.close(); }
	/**
	 * Open file for first time or close previous file then open new one
	 * @param[in] 'inFile' file name to open
	 * @throw basic silent failure if file doesn't open outputs message to terminal
	 */
	void openFile(const char* inFile)
	{
		fin.close(); fin.open(inFile, std::ios::in | std::ios::binary);
		if (fin.is_open() == false)
			std::cout << "Can't find file: " << inFile << std::endl;
	};
	/**
	 * Is file at end
	 * @return true if EOF else false
	 */
	bool isEOF() { return fin.eof(); }
	/**
	 * Read 1 byte from buffer
	 */
	unsigned char read() { return receive(8); }
	/**
	 * Read Integer from file
	 * @return integer value
	 */
	int readInt() { int output = 0; for (int ii = 0; ii < 4; ii++) { output = (output << 8) | read(); } return output; }
	/**
	 * Return byte to buffer
	 * @param[in] 'input' is value to return to buffer
	 */
	void restore(unsigned char input) { buffer = (buffer << 8) | input; numbits += 8; }
	/**
	 * Encode input to indexed value with ASCII look-up table
	 * @param[in] 'input' encode input into indexed value for table
	 */
	unsigned int encode(unsigned char input) { return ASCII[0x0F & input][(0xF0 & input) >> 4]; }
};
/**
 * Simple class to write to file
 * @param 'fout' ofstream for writing to file
 * @note anticipating some tailoring for future parts
 */
class FileOut
{
private:
	std::ofstream fout;
public:
	FileOut() {};
	FileOut(const char* outFile) { openFile(outFile); };
	~FileOut() { fout.close(); }
	/**
	 * Open file for first time or close previous file then open new one
	 * @param[in] 'outFile' file name to open
	 * @throw basic silent failure if file doesn't open outputs message to terminal
	 */
	void openFile(const char* outFile)
	{
		fout.close(); fout.open(outFile, std::ios::out | std::ios::binary);
		if (fout.is_open() == false)
			std::cout << "Can't open file: " << outFile << std::endl;
	};
	//void write(unsigned char output) { fout << output; }
	//void write(const char* output) { fout << output; }
	/**
	 * Write to file
	 * @tparam 'Type' is the data type used
	 * @param[in] 'data' is the data to write to file
	 * @note This function should only be used for primitive data types
	 *		and some structs with overridden write operations
	 */
	template<typename Type> void write(Type data) { fout << data; }
	/**
	 * Write to file
	 * @tparam 'Type' is the data type used
	 * @param[in] 'data' is the data to write to file
	 * @note This function should only be used for Data class data types
	 *		and some structs with overridden write operations
	 */
	template<typename Type>
	void writeData(Data<Type>* data)
	{
		for (int zz = 0; zz <= data->Size(); zz++)
			fout << data->read(zz);
	}
	/**
	 * Write integer to file as unsigned character
	 * @param[in] 'data' is the data to write to file
	 * @note This function should be used to write integers as strings
	 */
	void writeInt(int output)
	{
		Data<unsigned char> dat;
		do
		{
			dat.push(unsigned char((output % 10) + 0x30));
			output = output / 10;
		} while (output);
		for (int zz = 0; zz <= dat.Size();)
			fout << dat.pop();
	}
	/**
	 * Write integer to file as unsigned character for binary formatting
	 * @param[in] 'data' is the data to write to file
	 * @note This function should be used to write integers as binary representation
	 */
	void writeByte(int output)
	{
		Data<unsigned char> dat;
		do
		{
			dat.push(unsigned char(output % 256));
			output = output / 256;
		} while (output);
		while(dat.Size() < 3)
		{
			dat.push('\0');
		}
		for (int zz = 0; zz <= dat.Size();)
		{
			fout << dat.pop();
		}
	}
};
#endif
