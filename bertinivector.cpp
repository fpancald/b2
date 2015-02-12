/*
 * bertinivector.cpp
 *
 *  Created on: 28/nov/2014
 *      Author: Francesco
 */
#include <iostream>
using namespace std;
#include "bertinivector.h"
#include <cmath> /*for l norms*/

template <class T>
void bertinivector<T>::alloc_new(){
	maxsize=vsize*2;
	T* tmp=new T[maxsize];
	for(int i=0;i<vsize;i++)
		tmp[i]=array[i];
	delete[] array;
	array=tmp;
}

template <class T>
bertinivector<T>::bertinivector() {
	maxsize=20;
	array=new T[maxsize];
	vsize=0;
}

template <class T>
bertinivector<T>::bertinivector(int i) {
	maxsize=i;
	array=new T[maxsize];
	vsize=0;
}

template <class T>
bertinivector<T>::bertinivector(const bertinivector& v) {
	maxsize=v.maxsize;
	vsize=v.vsize;
	array= new T[maxsize];
	for(int i=0;i<v.vsize;i++)
		array[i]=v.array[i];
}

template <class T>
void bertinivector<T>::push_back(T i){
	if(vsize+1>maxsize)
		alloc_new();
	array[vsize]=i;
	vsize++;
}

template <class T>
int bertinivector<T>::size() const{
	return vsize;
}

template <class T>
T bertinivector<T>::operator[](int i){
	return array[i];
}

template <class T>
T bertinivector<T>::at(int i) const{
	if(i<vsize)
		return array[i];
	throw 10;//exception
}

template <class T>
bertinivector<T> bertinivector<T>::operator+=(const T& i)
{
	this->push_back(i);
	return *this;
}

template <class T>
bertinivector<T> bertinivector<T>::operator+=(const bertinivector& v)
{
	for(int j=0;j<v.vsize;j++)
		this->push_back(v.array[j]);
	return *this;
}

template <class T>
bertinivector<T> bertinivector<T>::operator+(const bertinivector<T>& v)
{
	bertinivector<T> tmp;
	if(vsize!=v.vsize)
		throw 10;
	else
		tmp.vsize=vsize;
	tmp.maxsize=2*tmp.vsize;
	for (int i=0; i<tmp.vsize; i++)
			tmp.array[i] = array[i] + v.array[i];

    return tmp;
}

template <class T>
bertinivector<T> bertinivector<T>::operator-(const bertinivector<T>& v)
{
	bertinivector<T> tmp;
	if(vsize!=v.vsize)
		throw 10;
	else
		tmp.vsize=vsize;
	tmp.maxsize=2*tmp.vsize;
	for (int i=0; i<tmp.vsize; i++)
			tmp.array[i] = array[i] - v.array[i];

    return tmp;
}

template <class T>
bertinivector<T> bertinivector<T>::dot_product(const bertinivector<T>& v) const
{
	bertinivector<T> tmp;
	if(vsize!=v.vsize)
		throw 10;
	else
		tmp.vsize=vsize;
	tmp.maxsize=2*tmp.vsize;
	for (int i=0; i<tmp.vsize; i++)
			tmp.array[i] = array[i] * v.array[i];

    return tmp;
}

template <class T>
T bertinivector<T>::inner_product(const bertinivector<T>& v) const
{
	T sum=0;
	int N;
	if(vsize!=v.vsize)
		throw 10;
	else
		N=vsize;
	for (int i=0; i<N; i++)
			sum+= array[i] * v.array[i];

    return sum;
}

template <class Y>
bertinivector<bertinivector<Y> > outer_product(const bertinivector<Y>& lhs,const bertinivector<Y>& rhs)
{
	bertinivector<bertinivector<Y> > prod;
	int N;
	if(rhs.vsize==lhs.vsize){
		prod.vsize=lhs.vsize;
		prod.maxsize=2*prod.vsize;
		for (int i=0; i<prod.vsize; i++){
			prod[i].vsize=prod.vsize;
			prod[i].maxsize=2*prod[i].vsize;
			for(int j=0; prod[i].vsize; j++)
				prod[i]= lhs.array[i]*rhs.array[j];
		}

    return prod;}
	throw 10;//exception
}

template <class Y>
bertinivector<Y> operator*(const Y& scalar, const bertinivector<Y>& rhs)
{
	bertinivector<Y> tmp;
	tmp=rhs*scalar;
    return tmp;
}

template <class Y>
bertinivector<Y> operator*(const bertinivector<Y>& lhs, const Y& scalar)
{
	bertinivector<Y> tmp;
	tmp.vsize=lhs.vsize;
	tmp.maxsize=2*tmp.vsize;
	for (int i=0; i<tmp.vsize; i++)
			tmp.array[i] = scalar*lhs.array[i];
		
    return tmp;
}

template <class Y>
Y operator*(const bertinivector<Y>& lhs, const bertinivector<Y>& rhs)
{
	Y tmp;
	tmp=lhs.inner_product(rhs);
    return tmp;
}

template <class T>
T bertinivector<T>::sumv()
{
	T sum=0;
	for (int i=0; i<vsize; i++)
		sum+= array[i];
			
    return sum;
}

template <class T>
T bertinivector<T>::l1norm()
{
	T sum=0;
	for (int i=0; i<vsize; i++)
		sum+= abs(array[i]);
			
    return sum;
}

template <class T>
T bertinivector<T>::l2sum()
{
	T sum=0;
	for (int i=0; i<vsize; i++)
		sum+= array[i] * array[i];
			
    return sum;
}

template <class T>
T bertinivector<T>::l2norm()
{
	T sum=0;
	for (int i=0; i<vsize; i++)
		sum+= array[i] * array[i];
		
	sum=sqrt(sum);
    return sum;
}

template <class T>
T bertinivector<T>::lpsum(T p)
{
	T sum=0;
	if(p<1)
		throw 10;
	for (int i=0; i<vsize; i++)
			sum+= pow(abs(array[i]),p);	
    return sum;
}

template <class T>
T bertinivector<T>::lpnorm(T p)
{
	T sum=0;	
	if(p<1)
		throw 10;
	for (int i=0; i<vsize; i++)
		sum+= pow(abs(array[i]),p);
		
	sum=pow(sum,1/p);
    return sum;
}

template <class T>
T bertinivector<T>::linfnorm()
{
	bertinivector<T> temp;
	T m;	
	temp.vsize=vsize;
	temp.maxsize=maxsize;
	for (int i=0; i<vsize; i++)
		temp.array[i]= abs(array[i]);
		
	m=temp.maxv();
    return m;
}

template <class T>
T bertinivector<T>::maxv()
{
	T m=array[0];
	if(vsize>1)	
		for (int i=1; i<vsize; i++)
			if(m<array[i])
				m=array[i];
    return m;
}

template <class T>
T bertinivector<T>::minv()
{
	T m=array[0];	
	if(vsize>1)
		for (int i=1; i<vsize; i++)
			if(m>array[i])
				m=array[i];
    return m;
}

template <class T>
T bertinivector<T>::dist(const bertinivector<T>& v, T p) const
{
	T d;
	bertinivector<T> tmp;
	if(vsize!=v.vsize)
		throw 10;
	tmp.vsize=vsize;
	tmp.maxsize=maxsize;
	for (int i=0;i<vsize;i++)
		tmp.array[i]=v.array[i]-array[i];
	d=tmp.lpnorm(p);	
    return d;
}

template <class T>
T bertinivector<T>::dist(const bertinivector<T>& v) const
{
	T d;
	bertinivector<T> tmp;
	if(vsize!=v.vsize)
		throw 10;
	tmp.vsize=vsize;
	tmp.maxsize=maxsize;
	for (int i=0;i<vsize;i++)
		tmp.array[i]=v.array[i]-array[i];
	d=tmp.l2norm();	
    return d;
}

template <class T>
bertinivector<T> bertinivector<T>::operator=(const bertinivector& v)
{
	if(this!=&v)//vec=vec
	{
		maxsize=v.maxsize;
		vsize=v.vsize;
		delete[] array; //delete array=deleted v.array
		array=new T[maxsize];
		for(int i=0;i<v.vsize;i++)
			array[i]=v.array[i];
			
	}
	return *this;
}

template <class T>
bertinivector<T> bertinivector<T>::operator=(const T& scalar)
{
	for(int i=0;i<vsize;i++)
		array[i]=scalar;
			
	return *this;
}

template <class T>
bertinivector<T> bertinivector<T>::operator*=(const T& scalar)
{
	bertinivector<T> tmp;
		tmp.maxsize=maxsize;
		tmp.vsize=vsize;
		tmp.array=new T[maxsize];
		for(int i=0;i<vsize;i++)
			tmp.array[i]=scalar*array[i];
		delete[] array; //delete array=deleted v.array
		array=new T[maxsize];
		for(int i=0;i<vsize;i++)
			array[i]=tmp.array[i];
			
	return *this;
}

template < class T >
friend ostream& operator << (ostream& os, const bertinivector<T>& v) 
{
    os << "[";
    for (int i=0;i<v.vsize;i++)
    {
        os << " " << v[i];
    }
    os << " ]";
    return os;
}

template <class T>
bool bertinivector<T>::operator==(const bertinivector& v)
{
	//bool ans=1;
	if(this==&v)//vec=vec
		return 1;
	if(vsize!=v.vsize)
		return 0;
	for (int i=0;i<vsize;i++)
		if(v.array[i]!=array[i])
			return 0;
	return 1;
}

template <class T>
bool bertinivector<T>::operator!=(const bertinivector& v)
{
	//bool ans=1;
	if(this==&v)//vec=vec
		return 0;
	if(vsize!=v.vsize)
		return 1;
	for (int i=0;i<vsize;i++)
		if(v.array[i]!=array[i])
			return 1;
	return 0;
}

template <class T>
bool bertinivector<T>::isnear(const bertinivector& v, T toll,T p)
{
	T d;
	bertinivector<T> tmp;
	if(this==&v)//vec=vec
		return 0;
	if(vsize!=v.vsize)
		return 1;
	tmp.vsize=vsize;
	tmp.maxsize=maxsize;
	for (int i=0;i<vsize;i++)
		tmp.array[i]=v.array[i]-array[i];
	d=tmp.lpnorm(p);
	if(d<=toll)
		return 1;
	return 0;
}

template <class T>
bool bertinivector<T>::isnear(const bertinivector& v, T toll)
{
	T d;
	bertinivector<T> tmp;
	if(this==&v)//vec=vec
		return 0;
	if(vsize!=v.vsize)
		return 1;
	tmp.vsize=vsize;
	tmp.maxsize=maxsize;
	for (int i=0;i<vsize;i++)
		tmp.array[i]=v.array[i]-array[i];
	d=tmp.l2norm();
	if(d<=toll)
		return 1;
	return 0;
}

template <class T>
bertinivector<T>::~bertinivector() {
	delete[] array;
}

