#ifndef _BINARY_HEAP_H
#define _BINARY_HEAP_H
#include <algorithm>
#include <functional>


template <typename V, 
		 		 typename Comp = std::less<V> >
class binary_heap
{

	typedef std::vector<V> Cont; 
	typedef V			value_type;
	typedef Cont		container_type;
	typedef Comp		comparer_type;
	typedef typename	container_type::iterator			iterator;
	typedef typename	container_type::const_iterator		const_iterator;
	typedef				binary_heap<V, Comp>				_Myt;
	typedef	typename	Cont::size_type						size_type;
public:
	binary_heap();
	~binary_heap();
	
	void push(const value_type& v);	//!< insert element
	void pop();						//!< remove top element 
	void adjust();						//!< recalc binary_heap
	value_type top();					//!< heap top element, must be check heap is not empty !

	// {{{ unused interfaces
#if 0
	iterator begin() {
		return _values.begin();
	}

	iterator end() { 
		return _values.end();
	}

	const_iterator begin() const {
		return _values.begin();
	}

	const_iterator end() const {
		return _values.end();
	}
#endif //  }}}
	size_type find(const value_type& v);
	size_type find(const _Myt::size_type i, const _Myt::value_type& v);
	static const size_type npos = 0xFFFFFFFF; // for failed find operation

	bool empty() const { return _values.empty(); }
	size_type size() const { return _values.size(); }

	void dump();
private:
	binary_heap(const _Myt& o);
	_Myt& operator = (const _Myt& o);	
protected:
	void swim(size_type i); //!< operation applied return true, otherwise return false
	void sink(size_type i);	//!< operation applied return true, otherwise return false
	bool has_left(size_type i) { return left_index(i) < size();}
	bool has_right(size_type i) { return right_index(i) < size();}
	value_type& left(size_type i);
	value_type& right(size_type i);
	size_type	left_index(size_type i) { return (i << 1) + 1; }
	size_type	right_index(size_type i) { return (i << 1) + 2; }
	size_type	parent_index(size_type i) { assert(i > 0); return (i-1) >> 1; }
private:
	container_type _values;	
};

template <typename V, typename Pred>
binary_heap<V,Pred>::binary_heap() {

}

template <typename V, typename Pred>
binary_heap<V,Pred>::~binary_heap() {

}

template <typename V, typename Pred>
void binary_heap<V,Pred>::push(const _Myt::value_type& o) {
	_values.push_back(o);
	swim(_values.size() - 1);
}

template <typename V, typename Pred>
void binary_heap<V,Pred>::pop() {
	assert( !empty() );
	if (_values.size() > 1) {
		iterator i = _values.begin();
		iterator j = _values.begin();
		std::advance(j, _values.size() - 1);
		std::iter_swap(i, j);
	}
	_values.pop_back();
	
	sink(0);
}

template <typename V, typename Pred>
void binary_heap<V,Pred>::adjust() {
	Pred pred;	
	std::sort(_values.begin(), _values.end(), pred);  
}

template <typename V, typename Pred>
typename binary_heap<V, Pred>::value_type
binary_heap<V, Pred>::top() {
	assert(!_values.empty());
	return _values.at(0);
}

template <typename V, typename Pred>
void binary_heap<V,Pred>::swim(_Myt::size_type i) {

	if (i == 0 || i >= size()) {
		return;
	}
	const size_type pindex = parent_index(i);
	const _Myt::value_type& myV = _values.at(i);
	const _Myt::value_type& myP = _values.at(pindex);
	Pred pred;
	if ( pred(myV, myP) ) {
		iterator iter = _values.begin();
		std::advance(iter, i);

		iterator piter = _values.begin();
		std::advance(piter, pindex);

		std::iter_swap(iter, piter);
		swim(pindex);
	}	

}

template <typename V, typename Pred>
void binary_heap<V,Pred>::sink(_Myt::size_type i) {

//	assert( i < _values.size() );
	if (i >= _values.size()) {
		return;
	}

	if (!has_left(i)) { // no chidren 
		return;
	}

	value_type& v = _values.at(i);
	Pred pred;
	bool left_is_mini_side = true;

	if (has_right(i)) {
		if (!pred(left(i), right(i))) {
			left_is_mini_side = false;
		}
	}
	
	if (left_is_mini_side) {
		if ( pred(left(i), v) ) {
			iterator itr	= _values.begin();
			std::advance(itr, i);

			iterator liter  = _values.begin();
			std::advance(liter, left_index(i));
			
			std::iter_swap( itr, liter );
			sink(left_index(i));
		}
	} else {
		if ( pred(right(i), v) ) {
			iterator itr	= _values.begin();
			std::advance(itr, i);

			iterator riter  = _values.begin();
			std::advance(riter, right_index(i));
			
			std::iter_swap( itr, riter );
			sink(right_index(i));
		}
	}
}

template <typename V, typename Pred>
typename binary_heap<V,Pred>::value_type& 
binary_heap<V,Pred>::left(_Myt::size_type i) {
	assert(i < _values.size());
	return _values.at( left_index(i) );
}

template <typename V, typename Pred>
typename binary_heap<V,Pred>::value_type& 
binary_heap<V, Pred>::right(_Myt::size_type i) {
	assert(i < _values.size());
	return _values.at( right_index(i) );
}

template <typename V, typename Pred>
typename binary_heap<V,Pred>::size_type
binary_heap<V, Pred>::find(const _Myt::value_type& v) {
	return find(0, v); 
}

template <typename V, typename Pred>
typename binary_heap<V,Pred>::size_type 
binary_heap<V, Pred>::find(const _Myt::size_type i, const _Myt::value_type& v) {

	if (_values.empty()) {
		return _Myt::npos; 
	}
	
	if (i >= _values.size()) {
		return _Myt::npos;
	}
		
	const _Myt::value_type& myV = _values.at(i);
	Pred pred;
	if ( myV == v ) { 
		return i;
	} else if ( pred(v,myV) ) {
		return find( left_index(i), v ); 
	} 
	return find( right_index(i), v );
}

template <typename V, typename Pred>
void binary_heap<V, Pred>::dump() {
	typename _Myt::container_type::iterator i = _values.begin();
	int index = 0; 
	while (i != _values.end()) {
		std::cout << *i << " ";
		++index;
		++i;
		if (index % 16 == 0) {
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}

#endif // _BINARY_HEAP_H

