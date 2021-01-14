#ifndef CS20A_LIST_H
#define CS20A_LIST_H

#include<iostream>
#include<assert.h>
// Linked List object that maintains both m_head and m_tail pointers
// and the m_size of the list.  Note that you have to keep the m_head,
// m_tail and m_size consistent with the intended state of the List 
// otherwise very bad things happen. 
template<typename Item>
class List {
public:

	List(); // Done - nothing else to add?

	List(const List<Item>& other); // done

	List<Item>& operator=(const List<Item>& other); // done

	~List(); // Done - nothing else to add?

	void	print() const; // done - nothing else to add?
	bool	empty() const; // done - nothing else to add?

	void	push_front(const Item &item); // double - done -- tail added
	void	push_rear(const Item &item); // double - done -- tail added
	void	insert(int index, const Item &item); // double - done -- tail added -- change index to 0

	// Note that the user must first ensure the list is not empty
	// prior to calling these functions.
	Item	front() const; // done - nothing else to add?
	Item	rear() const;  // done - nothing else to add?
	Item	get(int index) const; // done - nothing else to add? -- changed index to 0

	int		size() const;  // done - nothing else to add?
	int		find(const Item &item) const; // done - 

	bool	pop_front(); // double - done -- tail does not need adding
	bool	pop_rear(); // double - done
	bool	remove(int index); // double - done -- changed index to 0

#ifndef MAKE_MEMBERS_PUBLIC
private:
#endif
	// Forward declare the nodes for our List.
	// Will be implemented along with list's
	// member functions
	struct Node;

	// We'll have both m_head and m_tail points for 
	// Fast insertion/deletion from both ends.
	Node*	m_head;
	Node*	m_tail;

	// Keep track of number of nodes in the list
	int		m_size;
};


/* List Implementation
//
//  Since List is a template class (which is not an actual 
//  class yet, not until we actually instantiate the list) 
//  we need to keep the implementation together with 
//  the definition.  There are ways to simulate having 
//  separate "implementation/definition" with templates, 
//  but they aren't necessary and can be confusing.
*/

/* Node definition
//		Already implemented, nothing to do here but to use it.
*/
template<typename Item>
struct List<Item>::Node {
	Node() :next(nullptr), prev(nullptr) {}
	Node(Item it, Node* p, Node* n) : item(it), next(n), prev(p) {}

	Item  item;
	Node* next;
	Node* prev;
};



/* List default constructor
//		Set m_head and m_tail pointer to point to nothing, m_size is zero
//		Already implemented, nothing to do.
*/
template<typename Item>
List<Item>::List() :m_head(nullptr), m_tail(nullptr), m_size(0) {
	m_head = nullptr;
	m_tail = nullptr;
	m_size = 0;
}


/* Copy constructor  
*/
template<typename Item>
List<Item>::List(const List<Item>& other) {
	std::cout << "this is a copy constructor" << std::endl;

	if (other.m_head == nullptr)
	{
		m_head = nullptr;
		m_tail = nullptr;
		m_size = 0;
	}
	else if (other.m_size == 1)
	{
		m_size = other.m_size;
		Node *n = new Node;
		m_head = n;
		m_tail = n;
		n->item = other.m_head->item;
		n->prev = nullptr;
		n->next = nullptr;
	}
	else
	{
		Node *p = other.m_head;
		int indexSpot = 1;
		while (indexSpot <= other.m_size)
		{
			insert(indexSpot, p->item);

			indexSpot++;
			p = p->next;
		}
	}


}
/* Overloaded assignment operator ???????????  do any template
                                               typenames need to be added
*/
template<typename Item>
List<Item>& List<Item>::operator=(const List<Item>& other) { 
	std::cout << "this is a operator overload" << std::endl;

	while (m_size != 0)
	{
		pop_front();
	}

	Node *p = other.m_head;
	int indexSpot = 1;
	while (indexSpot <= other.m_size)
	{
		insert(indexSpot, p->item);

		indexSpot++;
		p = p->next;
	}
	return *this;
}


/* List destructor
*/
template<typename Item>
List<Item>::~List() {
	Node *p;
	p = m_head;
	while (p != nullptr)
	{
		Node *n = p->next;
		delete p;
		p = n;
	}
}

/* List print
*/		
template<typename Item>
void List<Item>::print() const {
	Node *p;
	p = m_head;
	while (p != nullptr)
	{
		if (p->next == nullptr)
		{
			std::cout << p->item << std::endl;
			p = p->next;
		}
		else
		{
			std::cout << p->item << " ";
			p = p->next;
		}
	}
}

/* List empty
*/
template<typename Item>
bool List<Item>::empty() const {
	if (m_head == nullptr)
		return true;
	else
		return false;
}


/* List push_front
*/
template<typename Item>
void List<Item>::push_front(const Item &item) {
	Node *n;
	n = new Node;
	n->item = item;
	n->next = m_head;
	n->prev = nullptr;

	if (m_size == 1)
		m_head->prev = n;
	m_head = n;

	if (m_tail == nullptr)
		m_tail = n;

	m_size++;
}

/* List push_rear
*/
template<typename Item>
void List<Item>::push_rear(const Item &item) {
	if (m_head == nullptr)
		push_front(item);
	else
	{
		Node *n = new Node;
		n->item = item;
		n->prev = m_tail;
		n->next = nullptr;
		m_tail->next = n;
		m_tail = n;

		m_size++;
	}
}

/* List insert
*/
template<typename Item>
void List<Item>::insert(int index, const Item &item) {
	if (m_size <= 0)
		push_front(item);
	else if (index >= m_size)
		push_rear(item);
	else
	{
		Node *p;
		p = m_head;
		int indexPlace = 0;
		while (indexPlace != (index - 1))
		{
			p = p->next;
			indexPlace++;
		}
		Node *n = new Node;
		n->item = item;
		n->next = p->next;
		n->prev = p;
		p->next->prev = n;
		p->next = n;

		m_size++;
	}
}

/*  List front
*/
template<typename Item>
Item List<Item>::front() const {

	return m_head->item;
}

/* List rear
*/
template<typename Item>
Item List<Item>::rear() const {

	return m_tail->item;
}

/* List get
//		returns the item at index
*/
template<typename Item>
Item List<Item>::get(int index) const {

	if (index == 0)
		return front();  // Changed from project 3, Added 'return'
	else if (index > m_size)
		return rear();  // Changed from project 3, Added 'return'
	else
	{
		Node *p;
		p = m_head;
		int indexPlace = 0;
		while (indexPlace != index)
		{
			p = p->next;
			indexPlace++;
		}
		return p->item;
	}
}

/* List size
*/
template<typename Item>
int List<Item>::size() const {
	return m_size;
}

/* List find
*/
template<typename Item>
int List<Item>::find(const Item &item) const {

	Node *p;
	p = m_head;
	int indexPlace = 0;
	while (p != nullptr)
	{
		if (p->item == item)
			return indexPlace;
		else
		{
			p = p->next;
			indexPlace++;
		}

	}
	return -1;
}

/* List pop_front
*/		
template<typename Item>
bool List<Item>::pop_front() {

	if (m_head == nullptr)
		return false;
	else
	{
		if (m_head == m_tail)
		{
			m_tail = nullptr;
			delete m_head;
			m_head = nullptr;
		}
		else
		{
			Node *d;
			d = m_head;
			m_head = d->next;
			m_head->prev = nullptr;
			delete d;
		}
		m_size--;
		return true;
	}
}

/* List pop_rear
*/	
template<typename Item>
bool List<Item>::pop_rear() {
	if (m_tail == nullptr)
		return false;
	else
	{
		if (m_head == m_tail)
		{
			m_tail = nullptr;
			delete m_head;
			m_head = nullptr;
		}
		else
		{
			Node *d;
			d = m_tail;
			m_tail = d->prev;
			m_tail->next = nullptr;
			delete d;
		}
		m_size--;
		return true;
	}
}

/* List remove
*/
template<typename Item>
bool List<Item>::remove(int index) {

	if (index > m_size)
		return false;
	else if (index == 0)
		return pop_front();
	else if (index > m_size)
		return pop_rear();
	else
	{
		Node *p;
		p = m_head;
		int indexPlace = 0;
		while (indexPlace != (index - 1))
		{
			p = p->next;
			indexPlace++;
		}
		Node *deleteIndex = p->next;
		p->next = deleteIndex->next;
		p->next->prev = p;
		delete deleteIndex;
		m_size--;
		return true;
	}

}







































































#endif // _X_XMMXX