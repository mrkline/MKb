#pragma once

#include <cstddef> // For size_t
#include <stdint.h> // For uint32_t

class CRC32Generator
{
public:
	/// Polynomial used by IEEE for 32-bit CRC
	static const uint32_t IEEEPolynomial = 0x04C11DB7;

	/// Initializes the CRC generator and builds the needed lookup table
	explicit CRC32Generator(uint32_t polynomial = IEEEPolynomial);

	/**
	\brief Generates a 32-bit CRC checksum for a given amount of data
	\param data A pointer to the data from which a checksum should be generated
	\param dataLength The length of the data from which a checksum should be
			generated, in bytes
	\returns The 32-bit CRC checksum of the given data
	*/
	uint32_t CRC32Generate(void* data, size_t dataLength);

private:
	/// Used by CRC32Init to flip the bits of an integer.
	uint32_t Reflect(uint32_t ref, char ch);

	/// Lookup table for the crc32 algorithm
	uint32_t table[256];
};
