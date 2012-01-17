#include "CRC32Generator.hpp"

uint32_t CRC32Generator::Reflect(uint32_t ref, char ch)
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

CRC32Generator::CRC32Generator(uint32_t polynomial)
{
	// 256 values representing ASCII character codes.
	for (int c = 0; c <= 0xFF; ++c)
	{
		table[c] = Reflect(c, 8) << 24;
		for (int p = 0; p < 8; ++p)
		{
			table[c] = (table[c] << 1)
			           ^ ((table[c] & (1 << 31)) ? polynomial : 0);
		}
		table[c] = Reflect(table[c], 32);
	}
}

uint32_t CRC32Generator::CRC32Generate(void* data, size_t dataLength)
{
	uint32_t crc = ~0;

	for (uint8_t* cData = (uint8_t*)data; dataLength; --dataLength, ++cData)
		crc = table[(crc & 0xFF) ^ *cData] ^ (crc >> 8);

	return ~crc;
}
