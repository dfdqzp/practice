#ifndef PRIORITYQUEUE_HEADER
#define PRIORITYQUEUE_HEADER

template<class T, int Comp(const T&, const T&)>
class PriorityQueue {
    public:
        // initialize function
        PriorityQueue(int compacity)
		{
			_compacity = compacity;
			_heap = new T[_compacity + 1];
			// how to optimize? Not all object has no-arguments constructor
			_heap[0] = T(); 
			_size = 0;
		}

        // get attribute function
        int compacity() { return _compacity; }
        int size() { return _size; }
        T* heap() { return _heap; }
        T pop()
		{
			if (_size < 1)
				return T();

			T root = _heap[1];
			T temp_root = _heap[_size]; // use tail node as temporary root node.

			int curPos = 1;
			int child = curPos * 2;

			while(child <= _size)
			{
				Comp(_heap[child] , _heap[child + 1]) > 0 ? child ++ : 0;

				if (Comp(temp_root, _heap[child]) > 0)
				{
					_heap[curPos] = _heap[child];
					curPos = child;
					child = curPos * 2;
				}
				else
				{
					break;
				}
			}

			_heap[curPos] = temp_root;
			_size--;

			return root;
		}

        bool insert(const T value)
		{
			if (_size == _compacity)  return false;

			int curPos = ++_size;
			for (; Comp(_heap[curPos / 2], value) > 0; curPos /= 2)
			{
				_heap[curPos] = _heap[curPos / 2];
			}

			return _heap[curPos] = value, true;
		}


    private:
        int _compacity;
        int _size;
        T* _heap;

    private:
        PriorityQueue();
        PriorityQueue& operator = (const PriorityQueue &);
        PriorityQueue (const PriorityQueue &);
} ;
#endif // PRIORITYQUEUE_HEADER