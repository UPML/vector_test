#include <ctime>
#include <iostream>
#include <random>
#include "vector.h"
#include "gtest/gtest.h"

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
		//test default ctor
		Vector<T> data;
		ASSERT_FALSE(data.size() != 0 || (!data.empty()));
		//test push_back
		data.push_back(T(1));
		data.push_back(std::move(T(2)));
		T tmp = T(3);
		data.push_back(tmp);
		ASSERT_EQ(data.size(), 3);
		//test copy ctor
		Vector<T> copy(data);
		ASSERT_EQ(copy.size(), 3);
		copy.push_back(T(5));
		ASSERT_EQ(data.size(), 3);
		//test move ctor
		Vector<T> temporary;
		temporary.push_back(T(4));
		Vector<T> moved(std::move(temporary));
		ASSERT_EQ(moved.size(), 1);
		//test copy operator =
		copy = copy = moved;
		ASSERT_EQ(copy.size(), 1);
		//test move operator =
		Vector<T> moved2 = std::move(moved);
		ASSERT_EQ(moved2.size(), 1);
		//test swap
		data.swap(moved2);
		ASSERT_EQ(data.size(), 1);
		ASSERT_EQ(moved2.size(), 3);
		std::swap(data, moved2);
		ASSERT_EQ(data.size(), 3);
		ASSERT_EQ(moved2.size(), 1);
		//test empty
		ASSERT_FALSE(data.empty());
		ASSERT_FALSE(moved2.empty());
		moved2.pop_back();
		ASSERT_TRUE(moved2.empty());
		moved2.push_back(T(1));
		//size() has been tested already
		//test max size = " << data.max_size() << std::endl;
		data.push_back(T(7));
		//test pop_back
		for (size_t i = 0; i < 10; ++i)
			data.push_back(T(i + 10));
		ASSERT_EQ(data.size(), 14);
		for (size_t i = 0; i < 10; ++i)
			data.pop_back();
		ASSERT_EQ(data.size(), 4);
		//test capacity
		ASSERT_FALSE(data.capacity() <= 4);
		//test reserve
		data.reserve(100);
		ASSERT_EQ(data.capacity(), 100);
		//test shrink_to_fit
		data.shrink_to_fit();
		ASSERT_EQ(data.capacity(), data.size());
		//test shrink_to_fit
		data.clear();
		ASSERT_EQ(data.size(), 0);
		//test []
		data.push_back(T(3));
		data[0] = T(7);
		const Vector<T>& lnkVect = data;
		const T& lnkT = lnkVect[0];
		//push_back and pop_back have been already tested
		//test emplace_back
#ifdef VECTOR_HAS_EMPLACE_BACK
		data.emplace_back(777);
		data.emplace_back(std::move(123));
		ASSERT_EQ(data.size(), 3);
#endif // VECTOR_HAS_EMPLACE_BACK
	}
	void TestWithoutCopy()
	{
		//test default ctor
		Vector<T> data;
		ASSERT_EQ(data.size(), 0);
		ASSERT_TRUE(data.empty());
		//test push_back
		data.push_back(T(1));
		data.push_back(std::move(T(2)));
		ASSERT_EQ(data.size(), 2);
		//test move ctor
		Vector<T> temporary;
		temporary.push_back(T(4));
		Vector<T> moved(std::move(temporary));
		ASSERT_EQ(moved.size(), 1);
		//test move operator =
		Vector<T> moved2 = std::move(moved);
		ASSERT_EQ(moved2.size(), 1);
		//test swap
		data.swap(moved2);
		ASSERT_EQ(data.size(), 1);
		ASSERT_EQ(moved2.size(), 2);
		std::swap(data, moved2);
		ASSERT_EQ(data.size(), 2);
		ASSERT_EQ(moved2.size(), 1);
		//test empty
		ASSERT_FALSE(data.empty());
		ASSERT_FALSE(moved2.empty());
		moved2.pop_back();
		ASSERT_TRUE(moved2.empty());
		moved2.push_back(T(1));
		//size() has been tested already
		ASSERT_EQ(std::numeric_limits<ptrdiff_t>::max() / sizeof(T), data.max_size());
		data.push_back(T(7));
		data.push_back(T(7));
		//test pop_back
		for (size_t i = 0; i < 10; ++i)
			data.push_back(T(i + 10));
		ASSERT_EQ(data.size(), 14);
		for (size_t i = 0; i < 10; ++i)
			data.pop_back();
		ASSERT_EQ(data.size(), 4);
		//test capacity
		ASSERT_TRUE(data.capacity() > 4);
		//test reserve
		data.reserve(100);
		ASSERT_EQ(data.capacity(), 100);
		//test shrink_to_fit
		data.shrink_to_fit();
		ASSERT_EQ(data.capacity(), data.size());
		//test shrink_to_fit
		data.clear();
		ASSERT_EQ(data.size(), 0);
		//test []
		data.push_back(T(3));
		data[0] = T(7);
		const Vector<T>& lnkVect = data;
		const T& lnkT = lnkVect[0];
		//push_back and pop_back have been already tested
		//test emplace_back
#ifdef VECTOR_HAS_EMPLACE_BACK
		data.emplace_back(777);
		data.emplace_back(std::move(123));
		ASSERT_EQ(data.size(), 3);
#endif // VECTOR_HAS_EMPLACE_BACK
	}
};

class ClassForTestDctor
{
private:
	int value_;
public:
	static ptrdiff_t count;
	ClassForTestDctor()
	{
		++count;
		value_ = 0;
	}
	//I didn't write explicit, because I use that Ctor as type conversion
	ClassForTestDctor(const int& val) :
		value_(val)
	{
		++count;
	}
	ClassForTestDctor(const ClassForTestDctor& other) :
		ClassForTestDctor(other.value_)
	{ }
	ClassForTestDctor& operator = (const ClassForTestDctor& other)
	{
		value_ = other.value_;
		return *this;
	}
	ClassForTestDctor(ClassForTestDctor&& other) :
		ClassForTestDctor(other.value_)
	{ }
	ClassForTestDctor& operator = (ClassForTestDctor&& other)
	{
		value_ = other.value_;
		return *this;
	}
	~ClassForTestDctor()
	{
		--count;
	}
	operator int()
	{
		return value_;
	}
};
ptrdiff_t ClassForTestDctor::count = 0;


void TestDctor()
{
	Vector<ClassForTestDctor> data;
	size_t length = 10000;
	data.reserve(length);
	for (size_t i = 0; i < length; ++i)
	{
		data.push_back(i);
	}
}


TEST(VectorTest, MemoryCheck)
{
	Vector<ClassForTestDctor> data;
	size_t length = 20000;
	data.reserve(length);
	ASSERT_EQ(ClassForTestDctor::count, 0);
	for (size_t i = 0; i < length; ++i)
	{
		data.push_back(i);
	}
	ASSERT_EQ(ClassForTestDctor::count, length);
	data = Vector<ClassForTestDctor>();
	ASSERT_EQ(ClassForTestDctor::count, 0);
	data.reserve(length);
	ASSERT_EQ(ClassForTestDctor::count, 0);
	for (size_t i = 0; i < length; ++i)
	{
		data.push_back(i);
	}
	ASSERT_EQ(ClassForTestDctor::count, length);
	data.clear();
	ASSERT_EQ(ClassForTestDctor::count, 0);
	data.reserve(length);
	ASSERT_EQ(ClassForTestDctor::count, 0);
	for (size_t i = 0; i < length; ++i)
	{
		data.push_back(i);
	}
	ASSERT_EQ(ClassForTestDctor::count, length);
	for (size_t i = 0; i < length; ++i)
	{
		data.pop_back();
	}
	ASSERT_EQ(ClassForTestDctor::count, 0);
	data.clear();
	ASSERT_EQ(ClassForTestDctor::count, 0);
	for (size_t i = 0; i < length; ++i)
	{
		data.push_back(i);
	}
	ASSERT_EQ(ClassForTestDctor::count, length);
	for (size_t i = 0; i < length; ++i)
	{
		data.pop_back();
	}
	ASSERT_EQ(ClassForTestDctor::count, 0);
	TestDctor();
	ASSERT_EQ(ClassForTestDctor::count, 0);
	data.clear();
	ASSERT_EQ(ClassForTestDctor::count, 0);
	for (size_t i = 0; i < length; ++i)
	{
		data.push_back(i);
	}
	ASSERT_EQ(ClassForTestDctor::count, length);
	Vector<ClassForTestDctor> copy = data;
	ASSERT_EQ(ClassForTestDctor::count, 2 * length);
	Vector<ClassForTestDctor> copy2(data);
	ASSERT_EQ(ClassForTestDctor::count, 3 * length);
	copy.clear();
	ASSERT_EQ(ClassForTestDctor::count, 2 * length);
	Vector<ClassForTestDctor> moved = std::move(copy2);
	Vector<ClassForTestDctor> moved2(std::move(data));
	ASSERT_EQ(ClassForTestDctor::count, 2 * length);
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
	ASSERT_EQ(moved.size(), 20);
	ASSERT_EQ(moved.capacity(), cap);
	for (size_t i = 0; i < 20; ++i)
	{
		ASSERT_EQ(moved[i].Get(), i);
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
			ASSERT_EQ(copy[i].Get(), static_cast<int>(i));
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
	//std::cout << "first tests" << std::endl;
	Vector<int> data;
	for (int i = 0; i < 5; ++i)
	{
		data.push_back(i);
	}
	ASSERT_EQ(data.size(), 5);
	Vector<int> copy(data);
	for (size_t i = 0; i < 5; ++i)
	{
		ASSERT_EQ(copy[i], data[i]);
	}
	for (int i = 0; i < 5; ++i)
		data[i] += 5;
	for (size_t i = 0; i < 5; ++i)
	{
		ASSERT_NE(copy[i], data[i]);
	}
	Vector<int> moved(std::move(copy));
	for (size_t i = 0; i < 5; ++i)
	{
		ASSERT_EQ(moved[i], i);
	}
	Vector<int> moved2 = std::move(moved);
	for (size_t i = 0; i < 5; ++i)
	{
		ASSERT_EQ(moved2[i], i);
	}
	Vector<int> copy2 = data;
	for (size_t i = 0; i < 5; ++i)
	{
		ASSERT_EQ(copy2[i], data[i]);
	}
	for (int i = 0; i < 5; ++i)
		data[i] += 5;
	for (size_t i = 0; i < 5; ++i)
	{
		ASSERT_NE(copy2[i], data[i]);
	}
	std::swap(data, copy2);
	for (size_t i = 0; i < 5; ++i)
	{
		ASSERT_EQ(data[i], i + 5);
		ASSERT_EQ(copy2[i], i + 10);
	}
	data.push_back(155);
	ASSERT_EQ(data.size(), 6);
	ASSERT_EQ(data[5], 155);
	data[0] = 123;
	ASSERT_EQ(data[0], 123);
#ifdef VECTOR_HAS_EMPLACE_BACK
	Vector<std::vector<TestClWithCopy>> emplback;
	emplback.emplace_back(5, TestClWithCopy(5));//std::vector ctor (size, value)
#endif // VECTOR_HAS_EMPLACE_BACK


}


TEST(VectorTest, intTest)
{
	TestVectror<int>().TestWithCopy();
}
TEST(VectorTest, doubleTest)
{
	TestVectror<double>().TestWithCopy();
}
TEST(VectorTest, MyClassWithoutCopy)
{
	TestVectror<TestClNonCopy>().TestWithoutCopy();
}
TEST(VectorTest, MyClassWithData)
{
	TestVectror<TestClWithData>().TestWithoutCopy();
}
TEST(VectorTest, MyClassWithCopy)
{
	TestVectror<TestClWithCopy>().TestWithCopy();
}
TEST(VectorTest, MyClassWithCountOfInstance)
{
	ASSERT_EQ(ClassForTestDctor::count, 0);
	TestVectror<TestClWithCopy>().TestWithCopy();
	ASSERT_EQ(ClassForTestDctor::count, 0);
}
TEST(VectorTest, ExceptionTest)
{
	for (size_t i = 0; i < 5; ++i)
	{
		TestException();
	}
}

void HugeTest()
{
	Vector<int> data;
	size_t lastSize = 0;
	std::cout << "Program can use all memory of computer. It can make problems with your PC!!!" << std::endl;
	std::cout << "Type \"y\" to continue" << std::endl;
	if (std::cin.get() != static_cast<int>('y'))
		return;
	size_t max = data.max_size();
	if (sizeof(size_t) == 8)
		max = 1000000000;
	try
	{
		for (size_t i = 0; i < max; ++i)
		{
			lastSize = data.size();
			data.push_back(static_cast<int>(i));
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

TEST(VectorTest, TestVector)
{
	TestVector();
}

TEST(VectorTest, TestSwapCapacity)
{
	Vector<int> data;
	data.reserve(20);
	for (size_t i = 0; i < 10; i++)
	{
		data.push_back(i);
	}
	Vector<int>(data).swap(data);
	ASSERT_EQ(data.capacity(), 10);
	ASSERT_EQ(data.capacity(), data.size());
}


int main(int argc, char **argv) {
	setlocale(LC_ALL, "ru-RU");
	std::cout << "Checking of iterators: ";
#ifdef CHECK_ITERATOR
	std::cout << "Enabled";
#else
	std::cout << "Diasbled";
#endif // CHECK_ITERATOR
	std::cout << std::endl << "Checking memory: ";
#ifdef TEST_MEM_I
	std::cout << "Enabled";
#else
	std::cout << "Diasbled";
#endif // TEST_MEM_I
	std::cout << std::endl;

	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();

	return result;
}
