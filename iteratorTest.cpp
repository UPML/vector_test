#include <ctime>
#include <iostream>
#include <list>
#include <random>
#include "vector.h"
#include "gtest/gtest.h"

using std::swap;

#ifdef TEST_MEM_I
ptrdiff_t CounterMem::countLnk = 0;
#endif // TEST_MEM_I

class NonConstProxy
{
private:
	Vector<int> data;
public:
	Vector<int>::iterator begin()
	{
		return data.begin();
	}
	Vector<int>::iterator end()
	{
		return data.end();
	}
	NonConstProxy(const Vector<int>& value)
		:data(value)
	{ }
};

class ConstProxy
{
private:
	Vector<int> data;
public:
	Vector<int>::const_iterator begin()
	{
		return data.cbegin();
	}
	Vector<int>::const_iterator end()
	{
		return data.cend();
	}
	ConstProxy(const Vector<int>& value)
		:data(value)
	{ }
};

class NonConstProxyReversed
{
private:
	Vector<int> data;
public:
	Vector<int>::reverse_iterator begin()
	{
		return data.rbegin();
	}
	Vector<int>::reverse_iterator end()
	{
		return data.rend();
	}
	NonConstProxyReversed(const Vector<int>& value)
		:data(value)
	{ }
};

class ConstProxyReversed
{
private:
	Vector<int> data;
public:
	Vector<int>::const_reverse_iterator begin()
	{
		return data.crbegin();
	}
	Vector<int>::const_reverse_iterator end()
	{
		return data.crend();
	}
	ConstProxyReversed(const Vector<int>& value)
		:data(value)
	{ }
};


template<typename proxy, typename IterType>
void TestOperators(proxy data)
{

	ASSERT_EQ(*(data.begin() + 3), 3);
	ASSERT_EQ(*(data.end() - 7), 3);
	ASSERT_EQ(*(++data.begin()), 1);
	IterType it = data.begin();
	it++;
	ASSERT_EQ(*it, 1);
	ASSERT_EQ(*(--data.end()), 9);
	IterType last = data.end();
	last--;
	ASSERT_EQ(*last, 9);
	ASSERT_EQ(data.end() - data.begin(), 10);
	ASSERT_EQ(data.begin() - data.end(), -10);
	it += 5;
	ASSERT_EQ(*it, 6);
	it -= 2;
	ASSERT_EQ(*it, 4);
	ASSERT_EQ(it[3], 7);
	ASSERT_EQ(*(3 + it), 7);
	for (auto i = data.begin(); i != data.end(); ++i)
		ASSERT_EQ(*i, i - data.begin());
	for (size_t i = 0; i < 10; ++i)
		ASSERT_EQ(data.begin()[i], i);
	ASSERT_TRUE(data.begin() < data.end());
	ASSERT_TRUE(data.begin() <= data.end());
	ASSERT_FALSE(data.begin() > data.end());
	ASSERT_FALSE(data.begin() >= data.end());
	ASSERT_TRUE(data.end() > data.begin());
	ASSERT_TRUE(data.end() >= data.begin());
	ASSERT_FALSE(data.end() < data.begin());
	ASSERT_FALSE(data.end() <= data.begin());
	ASSERT_TRUE(data.begin() == data.begin());
	ASSERT_TRUE(data.begin() != data.end());
	ASSERT_FALSE(data.begin() != data.begin());
	ASSERT_FALSE(data.begin() == data.end());
	ASSERT_TRUE(data.begin() >= data.begin());
	ASSERT_TRUE(data.begin() <= data.begin());
	IterType second = it + 3;
	ASSERT_EQ(*it, 4);
	ASSERT_EQ(*second, 7);
	swap(it, second);
	ASSERT_EQ(*it, 7);
	ASSERT_EQ(*second, 4);
	swap(it, second);
	IterType copy(it);
	ASSERT_EQ(*it, *copy);
	IterType moved(std::move(it));
	ASSERT_EQ(*moved, *copy);
	it = copy;
	ASSERT_EQ(*it, *copy);
	moved = (std::move(it));
	ASSERT_EQ(*moved, *copy);

}

template<typename Iterator>
void TestSwap(Iterator it)
{
	Iterator second = it + 3;
	ASSERT_EQ(*it, 4);
	ASSERT_EQ(*second, 7);
#ifdef CHECK_ITERATOR
	it.swap(second);
	ASSERT_EQ(*it, 7);
	ASSERT_EQ(*second, 4);
	second.swap(it);
	ASSERT_EQ(*it, 4);
	ASSERT_EQ(*second, 7);
#endif // CHECK_ITERATOR
	swap(it, second);
	ASSERT_EQ(*it, 7);
	ASSERT_EQ(*second, 4);
}
TEST(IteratorTest, TestOperators)
{
	Vector<int> data;
	for (int i = 0; i < 10; ++i)
	{
		data.push_back(i);
	}
	ASSERT_EQ(*data.begin(), 0);
	ASSERT_EQ(*(data.end() - 1), 9);
	*data.begin() = 2;
	ASSERT_EQ(*data.begin(), 2);
	data.begin()[0] = 0;
	for (auto i = data.begin(); i != data.end(); ++i)
		ASSERT_EQ(*i, i - data.begin());
	for (Vector<int>::iterator i = data.begin(); i < data.end(); ++i)
		(*i) = (i - data.begin()) + 1;
	for (Vector<int>::iterator i = data.begin(); i < data.end(); ++i)
		ASSERT_EQ(*i, (i - data.begin()) + 1);
	for (size_t i = 0; i < 10; ++i)
		ASSERT_EQ(data[i], i + 1);
	for (Vector<int>::iterator i = data.begin(); i < data.end(); ++i)
		--(*i);
	TestOperators<NonConstProxy, Vector<int>::iterator>(NonConstProxy(data));
	TestOperators<ConstProxy, Vector<int>::const_iterator>(ConstProxy(data));
	TestSwap(data.begin() + 4);
	TestSwap(data.cbegin() + 4);
	for (size_t i = 0; 2 * i < data.size(); ++i)
	{
		std::swap(data[i], data[data.size() - 1 - i]);
	}
	TestOperators<NonConstProxyReversed, Vector<int>::reverse_iterator>(NonConstProxyReversed(data));
	TestOperators<ConstProxyReversed, Vector<int>::const_reverse_iterator>(ConstProxyReversed(data));
	Vector<int>::iterator it = data.begin();
	Vector<int>::iterator copy(it);
	++it;
	ASSERT_TRUE(it != copy);
	Vector<int>::iterator moved(std::move(it));
	ASSERT_TRUE(moved != copy);
	it = moved;
	ASSERT_EQ(moved, it);
	moved = std::move(copy);
	ASSERT_NE(moved, it);
	Vector<Vector<int> > matrix;
	matrix.push_back(Vector<int>());
	matrix.begin()->reserve(1);
	Vector<int> strangeCopy;
	for (int& i : data)
	{
		strangeCopy.push_back(i);
	}
	ASSERT_EQ(strangeCopy.size(), 10);
	for (size_t i = 0; i < 10; ++i)
	{
		ASSERT_EQ(strangeCopy[i], 9 - i);
	}
	ASSERT_TRUE(data.cbegin() < it);
	ASSERT_TRUE(it > data.cbegin());
	ASSERT_TRUE(data.cbegin() != it);
	ASSERT_TRUE(it != data.cbegin());
	ASSERT_TRUE(data.cend() > it);
	ASSERT_TRUE(it < data.cend());
	ASSERT_TRUE(data.cbegin() <= it);
	ASSERT_TRUE(it >= data.cbegin());
	ASSERT_TRUE(data.cend() >= it);
	ASSERT_TRUE(it <= data.cend());
	Vector<int>::const_iterator constIter = it;
	ASSERT_EQ(*constIter, *it);
	ASSERT_TRUE(constIter == it);
	ASSERT_TRUE(it == constIter);
	Vector<int>::const_iterator constIter2 = static_cast<Vector<int>::const_iterator>(it);
	ASSERT_EQ(*constIter2, *it);
	ASSERT_TRUE(constIter2 == it);
	ASSERT_TRUE(it == constIter2);
	Vector<int>::const_iterator constIter3(data.begin());
	ASSERT_EQ(*constIter3, *data.begin());
	ASSERT_TRUE(constIter3 == data.begin());
	Vector<int>::const_reverse_iterator constrIter = data.rbegin();
	ASSERT_TRUE(constrIter == data.rbegin());
	Vector<int>::const_reverse_iterator constrIter2 = static_cast<Vector<int>::const_reverse_iterator>(data.rbegin());
	ASSERT_TRUE(constrIter2 == data.rbegin());
	Vector<int>::const_reverse_iterator constrIter3(data.rbegin());
	ASSERT_TRUE(constrIter3 == data.rbegin());
	ASSERT_EQ(it - data.cbegin(), 1);
	ASSERT_EQ(data.cbegin() - it, -1);
}

TEST(IteratorTest, VectorVsIterator)
{
	Vector<int> v1;
	Vector<int> v2;
	for (size_t i = 0; i < 10; ++i)
	{
		v1.push_back(i);
		v2.push_back(2 * i);
	}
	Vector<Vector<int>::iterator> iterators;
	for (Vector<int>::iterator i = v1.begin(); i < v1.end(); ++i)
	{
		iterators.push_back(i);
	}
	std::swap(v1, v2);
	Vector<int> moved = std::move(v1);
	Vector<int> moved2(std::move(v2));
	Vector<int> copy(moved);
	Vector<int> copy2 = moved2;
	Vector<double> doubleVect;
	for (size_t i = 0; i < 5; ++i)
		doubleVect.push_back(i + 0.25);
	Vector<Vector<double>::iterator> doubleIterators;
	for (auto i = doubleVect.begin(); i < doubleVect.end(); ++i)
		doubleIterators.push_back(i);
	for (size_t i = 0; i < 1000; ++i)
		doubleVect.push_back(i*1.1);
}

TEST(VectorTest, TestCtor2Operator)
{
	std::vector<int> realvect;
	for (size_t i = 0; i < 5; ++i)
	{
		realvect.push_back(i);
	}
	Vector<int> data(realvect.begin(), realvect.end());
	ASSERT_EQ(data.size(), realvect.size());
	for (size_t i = 0; i < data.size(); ++i)
	{
		ASSERT_EQ(data[i], realvect[i]);
	}
	std::list<double> lst;
	for (size_t i = 0; i < 10; ++i)
		lst.push_back(i + 0.5);
	Vector<double> data2(lst.begin(), lst.end());
	ASSERT_EQ(data2.size(), lst.size());
	auto iterData = data2.begin();
	auto iterLst = lst.begin();
	while (iterLst != lst.end())
	{
		ASSERT_EQ(*iterLst, *iterData);
		++iterData;
		++iterLst;
	}
}

#ifdef CHECK_ITERATOR
TEST(IteratorTest, TestExceptions)
{
	Vector<int> data;
	for (size_t i = 0; i < 10; ++i)
		data.push_back(static_cast<int>(i));
	try
	{
		data[12];
		FAIL();
	}
	catch (VectorExceptions::IndexOutOfRangeInVector)
	{
	}
	try
	{
		//strange test: -1 for size_t...
		data[-1];
		FAIL();
	}
	catch (VectorExceptions::IndexOutOfRangeInVector)
	{
	}
	try
	{
		data.begin()--;
		FAIL();
	}
	catch (VectorExceptions::IteratorOutOfRangeVectorException)
	{
	}
	try
	{
		--data.begin();
		FAIL();
	}
	catch (VectorExceptions::IteratorOutOfRangeVectorException)
	{
	}
	try
	{
		++data.end();
		FAIL();
	}
	catch (VectorExceptions::IteratorOutOfRangeVectorException)
	{
	}
	try
	{
		data.end()++;
		FAIL();
	}
	catch (VectorExceptions::IteratorOutOfRangeVectorException)
	{
	}
	try
	{
		data.begin() + 20;
		FAIL();
	}
	catch (VectorExceptions::IteratorOutOfRangeVectorException)
	{
	}
	try
	{
		data.begin() - 1;
		FAIL();
	}
	catch (VectorExceptions::IteratorOutOfRangeVectorException)
	{
	}
	try
	{
		21 + data.begin();
		FAIL();
	}
	catch (VectorExceptions::IteratorOutOfRangeVectorException)
	{
	}
	try
	{
		data.end() + 2;
		FAIL();
	}
	catch (VectorExceptions::IteratorOutOfRangeVectorException)
	{
	}
	try
	{
		data.end() - 11;
		FAIL();
	}
	catch (VectorExceptions::IteratorOutOfRangeVectorException)
	{
	}
	try
	{
		2 + data.end();
		FAIL();
	}
	catch (VectorExceptions::IteratorOutOfRangeVectorException)
	{
	}
	try
	{
		int t = *data.end();
		FAIL();
	}
	catch (VectorExceptions::IteratorOutOfRangeVectorException)
	{
	}
	catch (...)
	{
		FAIL();
	}
	Vector<int> second;
	for (int i = 0; i < 5; ++i)
		second.push_back(i);
	try
	{
		if (data.begin() < second.end())
			FAIL();
		FAIL();
	}
	catch (VectorExceptions::IteratorFromDifferentVectorsException)
	{
	}
	try
	{
		if (data.begin() > second.end())
			FAIL();
		FAIL();
	}
	catch (VectorExceptions::IteratorFromDifferentVectorsException)
	{
	}
	try
	{
		if (data.begin() == second.end())
			FAIL();
		FAIL();
	}
	catch (VectorExceptions::IteratorFromDifferentVectorsException)
	{
	}
	try
	{
		if (data.begin() != second.end())
			FAIL();
		FAIL();
	}
	catch (VectorExceptions::IteratorFromDifferentVectorsException)
	{
	}
	try
	{
		if (data.begin() <= second.end())
			FAIL();
		FAIL();
	}
	catch (VectorExceptions::IteratorFromDifferentVectorsException)
	{
	}
	try
	{
		if (data.begin() >= second.end())
			FAIL();
		FAIL();
	}
	catch (VectorExceptions::IteratorFromDifferentVectorsException)
	{
	}
	try
	{
		(data.begin() - second.end());
		FAIL();
	}
	catch (VectorExceptions::IteratorFromDifferentVectorsException)
	{
	}
	try
	{
		(data.begin()[-1]);
		FAIL();
	}
	catch (VectorExceptions::IteratorOutOfRangeVectorException)
	{
	}
	try
	{
		(data.begin()[12]);
		FAIL();
	}
	catch (VectorExceptions::IteratorOutOfRangeVectorException)
	{
	}
	Vector<Vector<int>> matrix;
	try
	{
		matrix.begin()->reserve(1);
		FAIL();
	}
	catch (VectorExceptions::IteratorOutOfRangeVectorException)
	{
	}
	auto oldBegin = data.begin();
	data.reserve(1000);
	for (size_t i = 0; i < 1000; ++i)
	{
		try
		{
			int t = oldBegin[i];
			FAIL();
		}
		catch (...)
		{
		}
	}
	while (data.size() != 30)
		data.push_back(rand());
	auto remembered = data.end() - 2;
	ASSERT_EQ(remembered, data.begin() + 28);
	ASSERT_EQ(*remembered, data.begin()[28]);
	for (size_t i = 0; i < 10; ++i)
		data.pop_back();
	try
	{
		auto tmp = *remembered;
		FAIL();
	}
	catch (VectorExceptions::IteratorOutOfRangeVectorException)
	{
	}
	catch (...)
	{
		throw;
	}
	Vector<Vector<int>::iterator> iterators;
	for (auto i = data.begin(); i != data.end(); ++i)
		iterators.push_back(i);
	while (data.size() != data.capacity())
		data.push_back(rand());
	for (auto i : iterators)
	{
		int t = *i;
		ASSERT_EQ(t, data.begin()[i - data.begin()]);
	}
}
#endif // CHECK_ITERATOR

TEST(IteratorTest, TestIteratorTraits)
{
	Vector<int> data;
	for (int i = 0; i < 5; ++i)
		data.push_back(i);
	std::iterator_traits<Vector<int>::iterator>::value_type t = *data.begin();
	ASSERT_EQ(typeid(int), typeid(std::iterator_traits<Vector<int>::iterator>::value_type));
}

#ifdef TEST_MEM_I
TEST(IteratorTest, MemoryCheck)
{
	ASSERT_EQ(CounterMem::countLnk, 0);
}
#endif // TEST_MEM_I

void CheckIteratorsValid(const Vector<int>& data, const Vector<Vector<int>::iterator>& iterators)
{
	for (size_t i = 0; i < iterators.size(); ++i)
	{
		ASSERT_EQ(*iterators[i], data[i]);
	}
}

TEST(IteratorTest, InvalidationOfConst)
{
	Vector<int> data;
	Vector<int> copy;
	data.reserve(100);
	for (int i = 0; i < 30; ++i)
		data.push_back(i);
	Vector<Vector<int>::iterator> iterators;
	for (auto i = data.begin(); i != data.end(); ++i)
		iterators.push_back(i);
	CheckIteratorsValid(data, iterators);
	for (int i = 0; i < 20; ++i)
		data.push_back(2 * i);
	CheckIteratorsValid(data, iterators);
	for (int i = 0; i < 10; ++i)
		data.pop_back();
	CheckIteratorsValid(data, iterators);
	//const methods
	data.empty();
	CheckIteratorsValid(data, iterators);
	data.size();
	CheckIteratorsValid(data, iterators);
	data.capacity();
	CheckIteratorsValid(data, iterators);
	std::swap(data, copy);
	CheckIteratorsValid(copy, iterators);
}

TEST(IteratorTest, stdBegin)
{
	Vector<int> data;
	data.push_back(1);
	data.push_back(2);
	ASSERT_EQ(*std::begin(data), 1);
	ASSERT_EQ(*(std::end(data) - 1), 2);
}

TEST(IteratorTest, EmptyVector)
{
	Vector<int> data;
	ASSERT_TRUE(data.begin() == data.end());
	ASSERT_TRUE(data.rbegin() == data.rend());
	ASSERT_TRUE(data.cbegin() == data.cend());
	ASSERT_TRUE(data.crbegin() == data.crend());
}