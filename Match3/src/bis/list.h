#pragma once

namespace bis {

	template<typename T>
	struct Node
	{
		Node(const T& d) : data(d), next(0), prev(0) {}
		T data;
		Node<T>* prev;
		Node<T>* next;
	};

	template<typename T>
	class list
	{
	public:
		list() :m_Head(0), m_Tail(0), m_Size(0) {}
		~list();
		inline size_t size() { return m_Size; }

		Node<T>* insert(const T& data, Node<T>* position);
		void push_back(const T& data);
		void push_front(const T& data);
		void remove(Node<T>* position);
		void pop_back();
		void pop_front();
		Node<T>& back();
		Node<T>& front();

	private:
		Node<T>* m_Head;
		Node<T>* m_Tail;
		size_t m_Size;
	};

	template<typename T>
	inline list<T>::~list()
	{
		Node<T>* headNode = m_Head;

		while (headNode)
		{
			Node<T>* nextNode = headNode->next;
			delete headNode;
			headNode = nextNode;
		}
	}

	template<typename T>
	inline Node<T>* list<T>::insert(const T& data, Node<T>* position)
	{
		Node<T>* newNode = new Node<T>(data);
		newNode->next = position;

		if (position)
		{
			newNode->prev = position->prev;
			position->prev = newNode;
		}
		else
		{
			newNode->prev = m_Tail;
			m_Tail = newNode;
		}

		if (newNode->prev)
		{
			newNode->prev->next = newNode;
		}
		else
		{
			m_Head = newNode;
		}
		m_Size++;

		return newNode;
	}

	template<typename T>
	inline void list<T>::push_back(const T& data)
	{
		list<T>::insert(data, nullptr); //Cannot sent tail here, because insertion happens before tail making it not a push back. 
	}

	template<typename T>
	inline void list<T>::push_front(const T & data)
	{
		list<T>::insert(data, m_Head);
	}

	template<typename T>
	inline void list<T>::remove(Node<T>* position)
	{

		if (position->next)
		{
			if (position->next)
			{
				position->next->prev = position->prev;
			}
			if (position->prev)
			{
				position->prev->next = position->next;
			}

			if (position == m_Head)
			{
				m_Head = position->next;
			}
			if (position == m_Tail)
			{
				m_Tail = position->prev;
			}
			m_Size--;
			delete position;
		}
	}

	template<typename T>
	inline void list<T>::pop_back()
	{
		list<T>::remove(m_Tail);
	}

	template<typename T>
	inline void list<T>::pop_front()
	{
		list<T>::remove(m_Head);
	}

	template<typename T>
	inline Node<T>& list<T>::back()
	{
		return m_Tail->data;
	}

	template<typename T>
	inline Node<T>& list<T>::front()
	{
		return m_Head->data;
	}
}
