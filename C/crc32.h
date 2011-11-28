#pragma once

#include <stddef.h> // For size_t
#include <stdint.h> // For uint32_t

//! Initializes the CRC generator and builds the needed lookup table
void CRC32Init();

/*!
\brief Generates a 32-bit CRC checksum for a given amount of data
\param data A pointer to the data from which a checksum should be generated
\param dataLength The length of the data from which a checksum should be
		generated, in bytes
\returns The 32-bit CRC checksum of data
*/
uint32_t CRC32Generate(void* data, size_t dataLength);
