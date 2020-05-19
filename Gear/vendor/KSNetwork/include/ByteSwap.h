#pragma once
#include "MemoryStream.h"

#ifndef RoboCat_ByteSwap_h
#define RoboCat_ByteSwap_h

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

#endif
