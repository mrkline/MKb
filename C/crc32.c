#include "crc32.h"

//! Lookup table for the crc32 algorithm
static uint32_t table[256];

//! Polynomial used by IEEE for 32-bit CRC
static const uint32_t IEEEPoly = 0x04C11DB7;

//! Used by CRC32Init to flip the bits of an integer.
static inline uint32_t Reflect(uint32_t ref, char ch)
{
	uint32_t value = 0;

	// Swap bit 0 for bit 7, bit 1 for bit 6, etc.
	for (int pos = 1; pos < ch + 1; ++pos)
	{
		if (ref & 1)
			value |= 1 << (ch - pos);

		ref >>= 1;
	}

	return value;
}

void CRC32Init()
{
	// 256 values representing ASCII character codes. 
    for (int c = 0; c <= 0xFF; ++c) 
    { 
		table[c] = Reflect(c, 8) << 24; 
        for (int p = 0; p < 8; ++p) 
		{
			table[c] = (table[c] << 1)
				^ ((table[c] & (1 << 31)) ? IEEEPoly : 0); 
		}
        table[c] = Reflect(table[c], 32); 
	} 
}

uint32_t CRC32Generate(void* data, size_t dataLength)
{
	uint32_t crc = 0xFFFFFFFF;

    for (uint8_t* cData = (uint8_t*)data; dataLength; --dataLength, ++cData)
    {
		crc = table[(crc & 0xFF) ^ *cData] ^ (crc >> 8);
    }

	return ~crc;
}
