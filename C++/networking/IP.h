#pragma once

#include <string>

//! An IPv4 address
class IP
{
public:
	/*!
	\brief Constructs the IP from a string
	\param ipStr The string representation of the IP address
	*/
	IP(const std::string& ipStr);

	/*!
	\brief Constructs the IP from individual octets
	\param oct0 The first octet
	\param oct1 The second octet
	\param oct2 The third octet
	\param oct3 The fourth octet
	*/
	IP(unsigned char oct0, unsigned char oct1, unsigned char oct2, unsigned char oct3);

	/*!
	\brief Constructs the IP from its binary representation
	\param bin The binary representation of the IP address
	*/
	IP(unsigned int bin);

	/*!
	\brief Gets a given octet
	\param octetNum The index of the octet to return
	\returns The specified octet
	\throws ArgumentOutOfRangeException if octetNum is not within the range [0,4)
	*/
	unsigned char GetOctet(unsigned char octetNum) const;

	/*!
	\brief Sets an octet to a new value
	\param octetNum The index of the octet to set
	\param val The new value of the octet
	\throws ArgumentOutOfRangeException if octetNum is not within the range [0,4)
	*/
	void SetOctet(unsigned char octetNum, unsigned char val);

	//! Gets the IP as a string
	const std::string& GetAsString() const { return strRep; }

	//! Gets the binary represetnation of the string
	unsigned int GetAsBinary() const;

	IP& operator=(const std::string& str);
	IP& operator=(unsigned int bin);

	bool operator==(const IP& other) const;
	bool operator!=(const IP& other) const { return !operator==(other); }

private:
	unsigned char octets[4];
	std::string strRep;

	//! Updates the internal string representation of the IP based on the octets
	void UpdateString();

	/*!
	Sets the IP octets from a string
	\throws ArgumentException if an IP could not be constructed from the given string
	*/
	void OctetsFromString(const std::string& ipStr);

	/*!
	\brief Sets the IP octets from a binary representation
	\param bin The binary representation of the IP address
	*/
	void OctetsFromBinary(unsigned int bin);
};

//! An IP end point consisting of an address and a port
struct IPEndPoint
{
	IP Address;
	int Port;

	IPEndPoint(const IP& address, int port)
		: Address(address), Port(port)
	{ }
};
