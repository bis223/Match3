#pragma once

namespace bis {

	const size_t DEFAULT_SIZE = 4;

	template<typename T>
	class vector_reverse_iterator
	{
	public:
		vector_reverse_iterator() :current(0) {}
		vector_reverse_iterator(T* pos) :current(pos) {}

		inline T& operator*()
		{
			return *current;
		}

		inline T* operator->()
		{
			return current;
		}

		inline vector_reverse_iterator operator++(int)
		{
			current--;
			return *this;
		}

		inline vector_reverse_iterator operator--(int)
		{
			current++;
			return *this;
		}

	private:
		T* current;
	};

	template<typename T>
	class vector
	{
	public:
		using reverse_iterator = typename vector_reverse_iterator<T>;

		vector()
			:	m_Array(new T[DEFAULT_SIZE]),
				m_ReservedSize(DEFAULT_SIZE),
				m_Size(0)
		{}

		vector(size_t size)
			: m_Array(new T[size]),
			m_ReservedSize(size),
			m_Size(0)
		{}

		~vector() 
		{ 
			delete[] m_Array;
		}

		typedef T* iterator;

		void push_back(const T & t);
		void resize(const size_t new_size);
		inline size_t size() { return m_Size; }
		inline iterator begin()	{return m_Array;}
		inline iterator end()	{return m_Array + m_Size;}

		inline reverse_iterator rbegin()	{return reverse_iterator(m_Array + m_Size);}
		inline reverse_iterator rend()	{return reverse_iterator(m_Array);}

	private:
		T* m_Array;
		size_t m_ReservedSize;
		size_t m_Size;
	};

	template <typename T>
	void vector<T>::push_back(const T& t)
	{
		if (m_Size == m_ReservedSize)
		{
			resize(m_Size + DEFAULT_SIZE);
		}

		m_Array[m_Size] = t;
		m_Size++;
	}

	template <typename T>
	void vector<T>::resize(const size_t new_size)
	{
		if (new_size > m_Size)
		{
			T* new_array = new T[new_size];
			for (size_t i = 0; i < m_Size; i++)
			{
				new_array[i] = m_Array[i];
			}
			delete[] m_Array;

			m_Array = new_array;

			m_ReservedSize = new_size;
		}
	}

	
}
