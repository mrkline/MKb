#pragma once

#include <string>
#include <stdint.h>

/// An IPv4 address
class IP
{
public:

	/// Byte order for binary addresses
	enum ByteOrder {
		BO_HOST, ///< Host byte order (depends on system)
		BO_NET ///< Network byte order (big endian)
	};


	/// Default constructor. Sets the ip to 0.0.0.0
	IP();

	/**
	\brief Constructs the IP from a string
	\param ipStr The string representation of the IP address
	*/
	IP(const std::string& ipStr);

	/**
	\brief Constructs the IP from individual octets
	\param oct0 The first octet
	\param oct1 The second octet
	\param oct2 The third octet
	\param oct3 The fourth octet
	*/
	IP(unsigned char oct0, unsigned char oct1, unsigned char oct2, unsigned char oct3);

	/**
	\brief Constructs the IP from its binary representation
	\param bin The binary representation of the IP address
	\param order The byte order of bin
	*/
	IP(uint32_t bin, ByteOrder order = BO_HOST);

	/**
	\brief Sets the IP from individual octets
	void set(unsigned char oct0, unsigned char oct1,
	         unsigned char oct2, unsigned char oct3);
	\param oct0 The first octet
	\param oct1 The second octet
	\param oct2 The third octet
	\param oct3 The fourth octet
	*/
	void set(unsigned char oct0, unsigned char oct1, unsigned char oct2, unsigned char oct3);

	/**
	\brief Gets a given octet
	\param octetNum The index of the octet to return
	\returns The specified octet
	\throws ArgumentOutOfRangeException if octetNum is not within the range [0,4)
	*/
	unsigned char getOctet(unsigned char octetNum) const;

	/**
	\brief Sets an octet to a new value
	\param octetNum The index of the octet to set
	\param val The new value of the octet
	\throws ArgumentOutOfRangeException if octetNum is not within the range [0,4)
	*/
	void setOctet(unsigned char octetNum, unsigned char val);


	/// Gets the IP as a string
	const std::string& getAsString() const { return strRep; }

	/// Gets the binary represetnation of the string
	/// \param order The byte order
	uint32_t getAsBinary(ByteOrder order = BO_HOST) const;

	IP& operator=(const std::string& str);

	/// Assigns the IP from a binary representation, assumed to be in host order
	IP& operator=(uint32_t bin);

	bool operator==(const IP& other) const;
	bool operator!=(const IP& other) const { return !operator==(other); }

private:
	unsigned char octets[4];
	std::string strRep;

	/// Updates the internal string representation of the IP based on the octets
	void updateString();

	/**
	Sets the IP octets from a string
	\throws ArgumentException if an IP could not be constructed from the given
	        string
	*/
	void octetsFromString(const std::string& ipStr);

	/**
	\brief Sets the IP octets from a binary representation
	\param bin The binary representation of the IP address
	\param order The byte order of bin
	*/
	void octetsFromBinary(uint32_t bin, ByteOrder order = BO_HOST);
};

/// An IP end point consisting of an address and a port
struct IPEndPoint
{
	IP Address;
	int Port;

	IPEndPoint(const IP& address, int port)
			: Address(address), Port(port)
	{ }
};
