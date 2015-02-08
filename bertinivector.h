/*
 * bertinivector.h
 *
 *  Created on: 28/nov/2014
 *      Author: Francesco
 */

#ifndef BERTINIVECTOR_H_
#define BERTINIVECTOR_H_
template <class T>
class bertinivector {
	int vsize, maxsize;
	T* array;
	void alloc_new();

public:
	bertinivector();
	bertinivector(int i);
	bertinivector(const bertinivector&);//copy constructor
	void push_back(T);
	int size() const;
	T operator[](int);//access i-th element
	T at(int i) const;//return value at index i
	bertinivector operator+=(const T&);//append element use push_back
	bertinivector operator+=(const bertinivector&);//append vector use for loop push_back
	bertinivector operator+(const bertinivector&);//add vectors if different size fill with zeros the short one
	bertinivector operator-(const bertinivector&);//sub vectors if different size fill with zeros the short one
	bertinivector dot_product(const bertinivector&) const;//multiplication index-by-index vectors if different size fill with zeros the short one
	T inner_product(const bertinivector&) const;//scalar product of vector with argument vector if different size fill with zeros the short one then sum up
	//template <class Y> friend bertinivector<bertinivector<Y> > outer_product(const bertinivector<Y>& lhs,const bertinivector<Y>& rhs);
	template <class Y> friend Y operator*(const bertinivector<Y>& lhs,const bertinivector<Y>& rhs);
	template <class Y> friend bertinivector<Y> operator*(const bertinivector<Y>& lhs,const Y& scalar);//multiplication by scalar number
	template <class Y> friend bertinivector<Y> operator*(const Y& scalar,const bertinivector<Y>& rhs);
	T l2norm();//l2 norm (squared)
	T l2norm_sqrt();//l2 norm 
	bertinivector operator=(const bertinivector&);//substitute vector
	bertinivector operator=(const T&);//set each element in vector =constant
	bertinivector operator*=(const T&);//substitute vector with vector multiplied by constant
	//friend ostream& operator<<(ostream& s,const bertinivectorector<T>& v);
	//istream
	virtual ~bertinivector();
	

};

#endif /* BERTINIVECTOR_H_ */
