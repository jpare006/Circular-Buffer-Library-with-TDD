#include "CppUTest/TestHarness.h"
#include "CircularBuffer.h"
#include <cstdint>

static const size_t size = 5;
static uint8_t data[] = {1,2,3,4,5};
static uint8_t read_data;

//************* Helper functions *************//
void fill_buffer(circular_buffer<uint8_t>& test_cbuf)
{
	for(size_t i = 0; i < size; i++)
	{
		test_cbuf.put(data[i]);
	}
}

void fill_buffer_num_elements(circular_buffer<uint8_t>& test_cbuf, uint8_t num)
{
	for(uint8_t i = 0; i < num; i++)
	{
		test_cbuf.put(i);
	}
}

//************* end helper *************//


TEST_GROUP(CircularBuffer)
{
};


//************* begin tests *************//
TEST(CircularBuffer, InitiCbufClassWithBufferAndSize)
{
	circular_buffer<uint8_t> test_cbuf(size);

	CHECK_EQUAL(size, test_cbuf.capacity());
}

TEST(CircularBuffer, CbufStateAfterInitIsEmpty)
{
	circular_buffer<uint8_t> test_cbuf(size);

	CHECK_FALSE(test_cbuf.full());
	CHECK(test_cbuf.empty());
}

TEST(CircularBuffer, AddFirstDataElementToCbufAndReadIt)
{
	circular_buffer<uint8_t> test_cbuf(size);

	test_cbuf.put(data[0]);
	read_data = test_cbuf.get();

	LONGS_EQUAL(data[0], read_data);
}

TEST(CircularBuffer, AddandReadMultipleDataElements)
{
	circular_buffer<uint8_t> test_cbuf(size);
	
	test_cbuf.put(data[0]);
	test_cbuf.put(data[1]);
	test_cbuf.put(data[2]);

	LONGS_EQUAL(data[0], test_cbuf.get());
	LONGS_EQUAL(data[1], test_cbuf.get());
	LONGS_EQUAL(data[2], test_cbuf.get());
}

TEST(CircularBuffer, FullFlagSetWhenCbufFull)
{
	circular_buffer<uint8_t> test_cbuf(size);

	fill_buffer(test_cbuf);

	CHECK(test_cbuf.full());
}

TEST(CircularBuffer, OverwriteOldestElementsWhenPutCalledAfterCbufFull)
{
	circular_buffer<uint8_t> test_cbuf(size);

	fill_buffer(test_cbuf);
	test_cbuf.put(6);
	test_cbuf.put(7);

	uint8_t read_data_arr[size];
	for(size_t i = 0; i < size; i++)
	{
		read_data_arr[i] = test_cbuf.get();
	}

	uint8_t expected_data[size] = {3,4,5,6,7};
	for(size_t i = 0; i < size; i++)
	{
		LONGS_EQUAL(expected_data[i], read_data_arr[i]);
	}

}

TEST(CircularBuffer, HeadPointerRollsOverAfterMaxSizeAmountElementsOverwritten)
{
	circular_buffer<uint8_t> test_cbuf(size);

	fill_buffer_num_elements(test_cbuf, (size * 2) + 1);

	uint8_t read_data_arr[size];
	for(size_t i = 0; i < size; i++)
	{
		read_data_arr[i] = test_cbuf.get();
	}

	uint8_t expected_data[] = {6,7,8,9,10};
	for(size_t i = 0; i < size; i++)
	{
		LONGS_EQUAL(expected_data[i], read_data_arr[i]);
	}
}

//************* end tests *************//