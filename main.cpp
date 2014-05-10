/*
!!!!

This is old code. It doesn't google test. Please, do not use it.  May be we should delete this file?
Up to date version of test is in file vector.cpp





*/
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <random>
#include "vector.h"
#include <vector>

//#define MEMORY_CHECK

class TestClNonCopy
{
public:
	explicit TestClNonCopy(size_t n)
	{ }//only for testing, Vector don't use this ctor

	TestClNonCopy(TestClNonCopy && value)
	{ }

	TestClNonCopy& operator = (TestClNonCopy &&value)
	{
		return *this;
	}

};



class TestClWithCopy
{
public:
	explicit TestClWithCopy(size_t n)
	{ }//only for testing, Vector don't use this ctor

	TestClWithCopy(TestClWithCopy && value)
	{ }

	TestClWithCopy& operator = (TestClWithCopy &&value)
	{
		return *this;
	}
	TestClWithCopy(const TestClWithCopy& value)
	{ }

	TestClWithCopy& operator = (const TestClWithCopy &value)
	{
		return *this;
	}
};

class TestClWithData
{
private:
	size_t v;
public:
	explicit TestClWithData(size_t n) :v(n)
	{ }//only for testing, Vector don't use this ctor

	TestClWithData(TestClWithData && value)
	{
		v = value.v;
	}

	TestClWithData& operator = (TestClWithData &&value)
	{
		v = value.v;
		return *this;
	}
};

class ExceptCl
{
private:
	size_t value;
public:
	ExceptCl(size_t n) : value(n)
	{ }
	ExceptCl(const ExceptCl& other) :
		value(other.value)
	{
		if (rand() % 10 == 0)
			throw std::logic_error("copy ctor ex");
	}
	ExceptCl(ExceptCl&& other) :
		value(other.value)
	{ }
	size_t Get()
	{
		return value;
	}
};

template<typename T>
class TestVectror
{
public:
	void TestWithCopy()
	{
		std::cout << "default ctor" << std::endl;
		Vector<T> data;
		if (data.size() != 0 || (!data.empty()))
			throw std::exception();
		std::cout << "push_back" << std::endl;
		data.push_back(T(1));
		data.push_back(std::move(T(2)));
		T tmp = T(3);
		data.push_back(tmp);
		if (data.size() != 3)
			throw std::exception();
		std::cout << "copy ctor" << std::endl;
		Vector<T> copy(data);
		if (copy.size() != 3)
			throw std::exception();
		copy.push_back(T(5));
		if (data.size() != 3)
			throw std::exception();
		std::cout << "move ctor" << std::endl;
		Vector<T> temporary;
		temporary.push_back(T(4));
		Vector<T> moved(std::move(temporary));
		if (moved.size() != 1)
			throw std::exception();
		std::cout << "copy operator =" << std::endl;
		copy = copy = moved;
		if (copy.size() != 1)
			throw std::exception();
		std::cout << "move operator =" << std::endl;
		Vector<T> moved2 = std::move(moved);
		if (moved2.size() != 1)
			throw std::exception();
		std::cout << "swap" << std::endl;
		data.swap(moved2);
		if (data.size() != 1 || moved2.size() != 3)
			throw std::exception();
		std::swap(data, moved2);
		if (data.size() != 3 || moved2.size() != 1)
			throw std::exception();
		std::cout << "empty" << std::endl;
		if (data.empty())
			throw std::exception();
		//size() has been tested already
		std::cout << "max size = " << data.max_size() << std::endl;
		data.push_back(T(7));
		std::cout << "pop_back" << std::endl;
		for (size_t i = 0; i < 10; ++i)
			data.push_back(T(i + 10));
		if (data.size() != 14)
			throw std::exception();
		for (size_t i = 0; i < 10; ++i)
			data.pop_back();
		if (data.size() != 4)
			throw std::exception();
		std::cout << "capacity" << std::endl;
		if (data.capacity() <= 4)
			throw std::exception();
		std::cout << "reserve" << std::endl;
		data.reserve(100);
		if (data.capacity() != 100)
			throw std::exception();
		std::cout << "shrink_to_fit" << std::endl;
		data.shrink_to_fit();
		if (data.capacity() != data.size())
			throw std::exception();
		std::cout << "shrink_to_fit" << std::endl;
		data.clear();
		if (data.size() != 0)
			throw std::exception();
		std::cout << "[]" << std::endl;
		data.push_back(T(3));
		data[0] = T(7);
		const Vector<T>& lnkVect = data;
		const T& lnkT = lnkVect[0];
		//push_back and pop_back have been already tested
		std::cout << "emplace_back" << std::endl;
/*		data.emplace_back(777);
		data.emplace_back(std::move(123));
		if (data.size() != 3)
			throw std::exception();
*/	}
	void TestWithoutCopy()
	{
		std::cout << "default ctor" << std::endl;
		Vector<T> data;
		if (data.size() != 0 || (!data.empty()))
			throw std::exception();
		std::cout << "push_back" << std::endl;
		data.push_back(T(1));
		data.push_back(std::move(T(2)));
		if (data.size() != 2)
			throw std::exception();
		std::cout << "move ctor" << std::endl;
		Vector<T> temporary;
		temporary.push_back(T(4));
		Vector<T> moved(std::move(temporary));
		if (moved.size() != 1)
			throw std::exception();
		std::cout << "move operator =" << std::endl;
		Vector<T> moved2 = std::move(moved);
		if (moved2.size() != 1)
			throw std::exception();
		std::cout << "swap" << std::endl;
		data.swap(moved2);
		if (data.size() != 1 || moved2.size() != 2)
			throw std::exception();
		std::swap(data, moved2);
		if (data.size() != 2 || moved2.size() != 1)
			throw std::exception();
		std::cout << "empty" << std::endl;
		if (data.empty())
			throw std::exception();
		//size() has been tested already
		std::cout << "max size = " << data.max_size() << std::endl;
		data.push_back(T(7));
		data.push_back(T(7));
		std::cout << "pop_back" << std::endl;
		for (size_t i = 0; i < 10; ++i)
			data.push_back(T(i + 10));
		if (data.size() != 14)
			throw std::exception();
		for (size_t i = 0; i < 10; ++i)
			data.pop_back();
		if (data.size() != 4)
			throw std::exception();
		std::cout << "capacity" << std::endl;
		if (data.capacity() <= 4)
			throw std::exception();
		std::cout << "reserve" << std::endl;
		data.reserve(100);
		if (data.capacity() != 100)
			throw std::exception();
		std::cout << "shrink_to_fit" << std::endl;
		data.shrink_to_fit();
		if (data.capacity() != data.size())
			throw std::exception();
		std::cout << "shrink_to_fit" << std::endl;
		data.clear();
		if (data.size() != 0)
			throw std::exception();
		std::cout << "[]" << std::endl;
		data.push_back(T(3));
		data[0] = T(7);
		const Vector<T>& lnkVect = data;
		const T& lnkT = lnkVect[0];
		//push_back and pop_back have been already tested
		std::cout << "emplace_back" << std::endl;
/*		data.emplace_back(777);		data.emplace_back(std::move(123));
		if (data.size() != 3)
			throw std::exception();
*/	}
};

void TestDctor()
{
	Vector<double> data;
	size_t length = 100000000;
	data.reserve(length);
	for (size_t i = 0; i < length; ++i)
	{
		data.push_back(i + 0.0);
	}
	std::cin.get();
}

void TestException()
{
	Vector<ExceptCl> data;
	for (size_t i = 0; i < 20; ++i)
	{
		data.push_back(ExceptCl(i));
	}
	Vector<ExceptCl> data2;
	for (size_t i = 0; i < 20; ++i)
	{
		data2.push_back(ExceptCl(i));
	}
	size_t cap = data2.capacity();
	Vector<ExceptCl> moved(std::move(data2));
	if (moved.size() != 20)
		throw std::exception();
	if (moved.capacity() != cap)
		throw std::exception();
	for (size_t i = 0; i < 20; ++i)
	{
		if (moved[i].Get() != i)
			throw std::exception();
	}
	for (size_t j = 0; j < 18; ++j)
	{
		data.pop_back();
		Vector<ExceptCl> copy;
		try
		{
			copy = data;
		}
		catch (...)
		{
		}
		for (size_t i = 0; i < copy.size(); ++i)
		{
			if (copy[i].Get() != (int)i)
				throw std::exception();
		}
	}
	std::swap(data, moved);
	moved.swap(data);
	Vector<ExceptCl> moved2 = std::move(moved);
	data2 = Vector<ExceptCl>();
	for (size_t i = 0; i < 20; ++i)
	{
		data2.push_back(ExceptCl(i));
	}
	for (size_t i = 0; i < 10; ++i)
	{
		data2.pop_back();
	}
	data2.shrink_to_fit();
	for (size_t i = 0; i < 20; ++i)
	{
		data2.push_back(ExceptCl(i));
	}
	data2.clear();
}

void TestVector()
{
	srand((unsigned)time(NULL));
	std::cout << "first tests" << std::endl;
	Vector<int> data;
	for (int i = 0; i < 5; ++i)
	{
		data.push_back(i);
	}
	if (data.size() != 5)
		throw std::logic_error("wrong size");
	Vector<int> copy(data);
	for (size_t i = 0; i < 5; ++i)
	{
		if (copy[i] != data[i])
			throw std::exception();
	}
	for (int i = 0; i < 5; ++i)
		data[i] += 5;
	for (size_t i = 0; i < 5; ++i)
	{
		if (copy[i] == data[i])
			throw std::exception();
	}
	Vector<int> moved(std::move(copy));
	for (size_t i = 0; i < 5; ++i)
	{
		if (moved[i] != i)
			throw std::exception();
	}
	Vector<int> moved2 = std::move(moved);
	for (size_t i = 0; i < 5; ++i)
	{
		if (moved2[i] != i)
			throw std::exception();
	}
	Vector<int> copy2 = data;
	for (size_t i = 0; i < 5; ++i)
	{
		if (copy2[i] != data[i])
			throw std::exception();
	}
	for (int i = 0; i < 5; ++i)
		data[i] += 5;
	for (size_t i = 0; i < 5; ++i)
	{
		if (copy2[i] == data[i])
			throw std::exception();
	}
	std::swap(data, copy2);
	for (size_t i = 0; i < 5; ++i)
	{
		if (data[i] != i + 5)
			throw std::exception();
		if (copy2[i] != i + 10)
			throw std::exception();
	}
	data.push_back(155);
	if (data.size() != 6)
		throw std::logic_error("wrong size (wait 6)");
	if (data[5] != 155)
		throw std::exception();
	data[0] = 123;
	if (data[0] != 123)
		throw std::exception();
	Vector<std::vector<TestClWithCopy>> emplback;
/*	emplback.emplace_back(5, TestClWithCopy(5));//std::vector ctor (size, value)
*/

#ifdef MEMORY_CHECK
	//While this part of code is running, I have checked memory in task manager
	std::cout << "Test memory" << std::endl;
	data.clear();
	size_t length = 200000000;
	data.reserve(length);
	for (size_t i = 0; i < length; ++i)
	{
		data.push_back(i);
	}
	std::cout << "Press ENTER";
	std::cin.get();
	data = Vector<int>();
	std::cout << "Press ENTER";
	std::cin.get();
	data.reserve(length);
	for (size_t i = 0; i < length; ++i)
	{
		data.push_back(i);
	}
	std::cout << "Press ENTER";
	std::cin.get();
	data.clear();
	std::cout << "Press ENTER";
	std::cin.get();
	data.reserve(length);
	for (size_t i = 0; i < length; ++i)
	{
		data.push_back(i);
	}
	std::cout << "Press ENTER";
	std::cin.get();
	for (size_t i = 0; i < length; ++i)
	{
		data.pop_back();
	}
	data.shrink_to_fit();
	std::cout << "Press ENTER";
	std::cin.get();

	std::cout << "Destructor Test" << std::endl;
	TestDctor();
	std::cout << "Press ENTER";
	std::cin.get();
#endif

	std::cout << "int test" << std::endl;
	TestVectror<int>().TestWithCopy();
	std::cout << "double test" << std::endl;
	TestVectror<double>().TestWithCopy();
	std::cout << "My class without copy" << std::endl;
	TestVectror<TestClNonCopy>().TestWithoutCopy();
	std::cout << "My class with data" << std::endl;
	TestVectror<TestClWithData>().TestWithoutCopy();
	std::cout << "My class with copy" << std::endl;
	TestVectror<TestClWithCopy>().TestWithCopy();

	for (size_t i = 0; i < 5; ++i)
	{
		std::cout << "Exception test" << std::endl;
		TestException();
	}

	std::cout << "final test" << std::endl;
	data.clear();
	size_t lastSize = 0;
	std::cout << "Program can use all memory of computer. It can make problems with your PC!!!" << std::endl;
	std::cout << "Type \"y\" to continue" << std::endl;
	if (std::cin.get() != (int)'y')
		return;
	size_t max = data.max_size();
	if (sizeof(size_t) == 8)
		max = 1000000000;
	try
	{
		for (size_t i = 0; i < max; ++i)
		{
			lastSize = data.size();
			data.push_back((int)i);
			if (i % 100000 == 0)
				std::cout << i << " " << data.size() << " " << data.capacity() <<
				" " << data.capacity()*sizeof(int) << std::endl;
		}
	}
	catch (...)
	{
		std::cout << "Error " << lastSize << " " << data.size();
	}
}
int main()
{
	TestVector();
	return 0;
}
/*
!!!!

This is old code. It doesn't google test. Please, do not use it.  May be we should delete this file?
Up to date version of test is in file vector.cpp





*/
