/**
 * Author: Joshua Renfro
 * 
 * Version 1.6
 * This library was tailored to build the map for our 
 * Romania Problem with detailed instructions to implement the class
 * 
 * Version 1.7
 * This library was tailored to store information of grammar file
 * for precedence matrix calculation and analysis for connectedness
 * and grounding
 */
#ifndef NODES_HPP
#define NODES_HPP

template<class T> void FREE(T*& ptr) { if (ptr) { delete(ptr); (ptr) = NULL; } };

/**
 * Circular Linked list data storage and data retrieval
 * which can be expanded as an array of circular linked lists.
 *
 * @tparam NodeDat is the data type to be stored in Nodeptr struct
 * @note The user MUST implement a data structure (example below) to work properly.
 *     Example:
 *         struct foo
 *		   {
 *             int varA;
 *             explicit foo(short inNum) :varA(inNum) {};
 *             friend std::ostream& operator<<(std::ostream& cout, const foo& data) { cout << data.varA << ';'; return cout; }
 *         }
 * @param data is the data stored of NodeDat.
 * @param head points to the next entry of the list.
 */
template<typename NodeDat>
struct Nodeptr
{
	NodeDat data;
	Nodeptr* next;
	Nodeptr(NodeDat inVal) : data(inVal) { next = NULL; };
	friend std::ostream& operator<<(std::ostream& cout, const Nodeptr& data) { return cout << data.data; }
};

/**
 * Circular Linked list data storage and data retrieval 
 * which can be expanded as an array of circular linked lists.
 * 
 * @tparam NodeDat is the data type to be stored in Nodeptr struct
 * @param size is the list size where if 0 then the list is empty.
 * @param head is a pointer to the beginning of the list.
 */
template<typename NodeDat>
class Node
{
private:
	int size;
	Nodeptr<NodeDat>* head;
public:
	/**
	* @param size, head will be initialized to 0
	*/
	Node() { size = 0; head = NULL; };
	/**
	* Copy constructor
	*/
	Node(const Node& inVal);
	/**
	* Upon deletion check list if empty, if false then delete each entry of list
	*/
	~Node()
	{
		if (this->size != 0)
			Node<NodeDat>::deleteList(this);
	};

	Node& operator=(const Node& inVal)
	{
		if (isEmpty(this) == false)
			Node<NodeDat>::deleteList(this);
		Nodeptr<NodeDat>* ptr;
		ptr = inVal.head;
		for (int ii = 0; ii < inVal.size; ii++)
		{
			Node<NodeDat>::insertRight(this, ptr->data);
			ptr = ptr->next;
		}
		return *this;
	}
	/**
	* @param[in] 'list' is the referenced list used.
	*     'list' is made constant to prevent modification of values.
	* @returns 'list->size' is the returning value of type 'int'
	* @throw no throw exceptions are made
	*/
	static int getSize(const Node* list) { return list->size; }
	/*
	* @param[in] 'list' is the referenced list used.
	* @returns true if list head is null otherwise false.
	*/
	static bool isEmpty(const Node* list) { if (list->head == NULL) return true; else return false; }
	/**
	* Find data node from list with index value
	* @param[in] 'list' is the referenced list.
	* @param[in] 'index' is the indexed entry to search.
	* @returns 'ptr' with data type NodeDat is returned
	* @throw basic an exception is thrown when 'index' > 'list->size' is true
	*     The function will continue regardless due to the list being circular
	*/
	static NodeDat getData(const Node* list, int index);
	/**
	* Insert new entry to the left or beginning of the list.
	* @param[in out] 'list' is the referenced list
	* @param[in] 'item' is the new entry to be added
	* @throw none no exceptions are made 
	* @warning Must be done correctly when calling this method see note below for proper call
	* @note the best way to create a new entry is to:
	*     Node<Data>::insertLeft(&List[index], new Data(param1, param2));
	*     This is to assume that the user has created an explicit constructor for the new data entry.
	*/
	//static void insertLeft(Node* list, Nodeptr<NodeDat>* item);
	static void insertLeft(Node* list, NodeDat item);
	/**
	* Insert new entry to the right or end of the list.
	* @param[in out] 'list' is the referenced list
	* @param[in] 'item' is the new entry to be added
	* @throw none no exceptions are made 
	* @warning Must be done correctly when calling this method see note below for proper call
	* @note the best way to create a new entry is to:
	*     Node<Data>::insertRight(&List[index], new Data(param1, param2));
	*     This is to assume that the user has created an explicit constructor for the new data entry.
	*/
	//static void insertRight(Node* list, Nodeptr<NodeDat>* item);
	static void insertRight(Node* list, NodeDat item);
	/**
	* Delete entry from beginning of list
	* @param[in out] 'list' is the referenced list
	* @throw basic when the 'list->head' is null then an underflow is detected and the function ceases
	*/
	static void deleteLeft(Node* list);
	/**
	* Delete entry from end of list
	* @param[in out] 'list' is the referenced list
	* @throw basic when the 'list->head' is null then an underflow is detected and the function ceases
	*/
	static void deleteRight(Node* list);
	/**
	* Delete random node in list
	* @param[in out] 'list' is the referenced list.
	* @param[in] 'index' is the indexed entry to search.
	* @returns 'ptr' is returned once found check @tparam
	* @throw basic an exception is thrown when 'index' > 'list->size' is true
	*     The function will continue regardless due to the list being circular
	*/
	static void deleteNode(Node* list, int index);
	/**
	* Delete entire list
	* @param[in out] 'list' is the referenced list
	* @throw basic when the 'list->head' is null then an underflow is detected and the function ceases
	*/
	static void deleteList(Node* list);
	/**
	* Print contents of list
	* @param[in out] 'list' is the referenced list
	* @throw none
	* @note It is strongly recommended that the user creates their own
	*     instance of cout through ostream& operator<<(ostream& cout, Data& data) 
	*     where 'Data' is the data type used for @tparam 'Nodeptr'
	*/
	static void printList(const Node* list);
};
template<typename NodeDat>
Node<NodeDat>::Node(const Node& inVal)
{
	this->size = 0; this->head = NULL;
	//Nodeptr<NodeDat>* ptr;
	//ptr = inVal.head;
	//for (int ii = 0; ii < inVal.size; ii++)
	//{
	//	Node<NodeDat>::insertRight(this, new Nodeptr<NodeDat>(ptr->data));
	//	ptr = ptr->next;
	//}
	*this = inVal;
}
template<typename NodeDat>
NodeDat Node<NodeDat>::getData(const Node* list, int index)
{
	Nodeptr<NodeDat>* ptr;
	ptr = list->head;
	/*if (index > list->size)
		std::cout << "\nIndex exceeds list size!\n";*/

	for (int ii = 0; ii < index; ii++)
	{
		ptr = ptr->next;
	}
	return ptr->data;
}
template<typename NodeDat>
void Node<NodeDat>::insertLeft(Node* list, NodeDat item)
{
	Nodeptr<NodeDat>* ptr, * nptr;
	nptr = new Nodeptr<NodeDat>(item);

	if (list->head == NULL)
	{
		nptr->next = nptr;
		list->head = nptr;
	}
	else
	{
		ptr = list->head;
		while (ptr->next != list->head)
		{
			ptr = ptr->next;
		}
		nptr->next = ptr->next;
		ptr->next = nptr;
		list->head = nptr;
	}
	list->size += 1;
};
template<typename NodeDat>
void Node<NodeDat>::insertRight(Node* list, NodeDat item)
{
	Nodeptr<NodeDat>* ptr;
	insertLeft(list, item);
	ptr = list->head;
	list->head = ptr->next;
};
template<typename NodeDat>
void Node<NodeDat>::deleteLeft(Node* list)
{
	Nodeptr<NodeDat>* ptr, * dptr;
	
	if (list->head == NULL)
	{
		std::cout << "\nUnderflow detected!\n";
	}
	else
	{
		dptr = ptr = list->head;
		while (ptr->next != list->head)
		{
			ptr = ptr->next;
		}
		list->head = ptr->next = dptr->next;
		FREE(dptr);
		if (list->size == 1)
		{
			list->head = NULL;
		}
		list->size -= 1;
	}
};
template<typename NodeDat>
void Node<NodeDat>::deleteRight(Node* list)
{
	Nodeptr<NodeDat>* ptr, * dptr;

	if (list->head == NULL)
	{
		std::cout << "\nUnderflow detected!\n";
	}
	else
	{
		ptr = dptr = list->head;
		while (dptr->next != list->head)
		{
			ptr = dptr;
			dptr = dptr->next;
		}
		ptr->next = list->head;
		FREE(dptr);
		if (list->size == 1)
		{
			list->head = NULL;
		}
		list->size -= 1;
	}
};
template<typename NodeDat>
void Node<NodeDat>::deleteNode(Node* list, int index)
{
	Nodeptr<NodeDat>* ptr, * dptr;
	
	if (list->head == NULL)
	{
		std::cout << "\nList is Empty\n!";
	}
	else
	{
		if (index >= list->size)
		{
			std::cout << "\nIndex exceeds/is equal to list size!\n"; index %= list->size;
		}
		if (index == 0)
			deleteLeft(list);
		else if (index == list->size - 1)
			deleteRight(list);
		else
		{
			ptr = dptr = list->head;
			for (int ii = 0; ii < index; ii++)
			{
				ptr = dptr;
				dptr = dptr->next;
			}
			ptr->next = dptr->next;
			FREE(dptr);
			list->size -= 1;
		}
	}
}
template<typename NodeDat>
void Node<NodeDat>::deleteList(Node* list)
{
	Nodeptr<NodeDat>* ptr;

	if (list->head == NULL)
	{
		std::cout << "\nUnderflow detected!\n";
	}
	else
	{
		//std::cout << "\nDeleting List!\n";
		ptr = list->head;
		for (int ii = 0; ii < list->size; ii++)
		{
			list->head = ptr->next;
			FREE(ptr);
			ptr = list->head;
		}
		list->size = 0;
		list->head = NULL;
	}
};
template<typename NodeDat>
void Node<NodeDat>::printList(const Node* list)
{
	Nodeptr<NodeDat>* ptr;
	ptr = list->head;
	for (int ii = 0; ii < list->size; ii++)
	{
		std::cout << *ptr;
		ptr = ptr->next;
	}
};
#endif