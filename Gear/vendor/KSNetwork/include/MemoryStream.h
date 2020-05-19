#pragma once
#include <cstdlib>
#include <cstdint>
#include <type_traits>
#include <iostream>
#include <algorithm>
#include <vector>

#define STREAM_ENDIANNESS 0
#define PLATFORM_ENDIANNESS 0

inline uint16_t ByteSwap2(uint16_t inData)
{
	return (inData >> 8) | (inData << 8);
}

inline uint64_t ByteSwap4(uint64_t inData)
{
	return  ((inData >> 24) & 0x000000ff) |
		((inData >> 8) & 0x0000ff00) |
		((inData << 8) & 0x00ff0000) |
		((inData << 24) & 0xff000000);
}

inline uint64_t ByteSwap8(uint64_t inData)
{
	return  ((inData >> 56) & 0x00000000000000ff) |
		((inData >> 40) & 0x000000000000ff00) |
		((inData >> 24) & 0x0000000000ff0000) |
		((inData >> 8) & 0x00000000ff000000) |
		((inData << 8) & 0x000000ff00000000) |
		((inData << 24) & 0x0000ff0000000000) |
		((inData << 40) & 0x00ff000000000000) |
		((inData << 56) & 0xff00000000000000);
}


template < typename tFrom, typename tTo >
class TypeAliaser
{
public:
	TypeAliaser(tFrom inFromValue) :
		mAsFromType(inFromValue) {}
	tTo& Get() { return mAsToType; }

	union
	{
		tFrom 	mAsFromType;
		tTo		mAsToType;
	};
};


template <typename T, size_t tSize > class ByteSwapper;

//specialize for 1...
template <typename T>
class ByteSwapper< T, 1 >
{
public:
	T Swap(T inData) const
	{
		return inData;
	}
};



template <typename T>
class ByteSwapper< T, 2 >
{
public:
	T Swap(T inData) const
	{
		uint16_t result =
			ByteSwap2(TypeAliaser< T, uint16_t >(inData).Get());
		return TypeAliaser< uint16_t, T >(result).Get();
	}
};

template <typename T>
class ByteSwapper< T, 4 >
{
public:
	T Swap(T inData) const
	{
		uint64_t result =
			ByteSwap4(TypeAliaser< T, uint64_t >(inData).Get());
		return TypeAliaser< uint64_t, T >(result).Get();
	}
};

template <typename T>
class ByteSwapper< T, 8 >
{
public:
	T Swap(T inData) const
	{
		uint64_t result =
			ByteSwap8(TypeAliaser< T, uint64_t >(inData).Get());
		return TypeAliaser< uint64_t, T >(result).Get();
	}
};

template < typename T >
T ByteSwap(T inData)
{
	return ByteSwapper< T, sizeof(T) >().Swap(inData);
}


class OutputMemoryStream
{
public:
	OutputMemoryStream() :
		mBuffer(nullptr), mHead(0), mCapacity(0)
	{
		ReallocBuffer(32);
	}

	~OutputMemoryStream() { std::free(mBuffer); }

	//get a pointer to the data in the stream
	const 	char*		GetBufferPtr()	const { return mBuffer; }
	uint64_t	GetLength()		const { return mHead; }

	void		Write(const void* inData,
		size_t inByteCount);

	template< typename T > void Write(T inData)
	{
		static_assert(std::is_arithmetic< T >::value ||
			std::is_enum< T >::value,
			"Generic Write only supports primitive data types");

		if (STREAM_ENDIANNESS == PLATFORM_ENDIANNESS)
		{
			Write(&inData, sizeof(inData));
		}
		else
		{
			T swappedData = ByteSwap(inData);
			Write(&swappedData, sizeof(swappedData));
		}

	}

	void Write(const std::vector< int >& inIntVector)
	{
		size_t elementCount = inIntVector.size();
		Write(elementCount);
		Write(inIntVector.data(), elementCount * sizeof(int));
	}

	template< typename T >
	void Write(const std::vector< T >& inVector)
	{
		uint64_t elementCount = inVector.size();
		Write(elementCount);
		for (const T& element : inVector)
		{
			Write(element);
		}
	}

	void Write(const std::string& inString)
	{
		size_t elementCount = inString.size();
		Write(elementCount);
		Write(inString.data(), elementCount * sizeof(char));
	}

private:
	void		ReallocBuffer(uint64_t inNewLength);

	char*		mBuffer;
	uint64_t	mHead;
	uint64_t	mCapacity;

};

class InputMemoryStream
{
public:
	InputMemoryStream(char* inBuffer, uint64_t inByteCount) :
		mBuffer(inBuffer), mCapacity(inByteCount), mHead(0) {}

	~InputMemoryStream() { std::free(mBuffer); }

	uint64_t		GetRemainingDataSize() const
	{
		return mCapacity - mHead;
	}

	void		Read(void* outData, uint64_t inByteCount);


	template< typename T > void Read(T& outData)
	{
		static_assert(std::is_arithmetic< T >::value ||
			std::is_enum< T >::value,
			"Generic Read only supports primitive data types");
		Read((void*)(&outData), sizeof(outData));
	}

	template< typename T >
	void Read(std::vector< T >& outVector)
	{
		size_t elementCount;
		Read(elementCount);
		outVector.resize(elementCount);
		for (const T& element : outVector)
		{
			Read(element);
		}
	}

	void Read(std::string& outString)
	{
		size_t elementCount = outString.size();
		Read(elementCount);
		outString.resize(elementCount);
		for (const auto& element : outString)
		{
			Read(element);
		}
	}

private:
	char*		mBuffer;
	uint64_t	mHead;
	uint64_t	mCapacity;
};

