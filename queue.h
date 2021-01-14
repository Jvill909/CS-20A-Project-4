#ifndef QUEUE_H_
#define QUEUE_H_

#include"list.h"

// Queue ADT using linked list as primary data structure.
// 
template<typename Type>
class Queue {
public:

	Queue();

	// Basic queue operations
	void	push(Type item);
	void	pop();
	Type	front() const;

	// Check to see if there is anything on the queue
	bool	empty() const;

	// Use to see what is in the queue for debugging
	void	print() const;

#ifndef MAKE_MEMBERS_PUBLIC
private:
#endif
	List<Type> m_list;
};


// Queue Implementation
//

// Queue() Default constrcutor:
//		Call the defualt constructor for its List.
//		Already implemented.
template<typename Type>
Queue<Type>::Queue() :m_list() {}

// push():
template<typename Type>
void Queue<Type>::push(Type item) {
	m_list.push_rear(item);
	/* TODO */

}

// pop():
template<typename Type>
void Queue<Type>::pop() {
	m_list.pop_front();
	/* TODO */

}

// empty():
template<typename Type>
bool Queue<Type>::empty() const {
	return m_list.empty();
	/* TODO */

	return false;
}

// front():
template<typename Type>
Type Queue<Type>::front() const {
	return m_list.front();
	/* TODO */

	//Type RETURN_VALUE_REQUIRED_TO_COMPILE; 

	//return RETURN_VALUE_REQUIRED_TO_COMPILE;
}

// print()
template<typename Type>
void Queue<Type>::print() const {
	m_list.print();
	/* TODO */

}




























#endif //QUEUE_H_X_XMMXX 