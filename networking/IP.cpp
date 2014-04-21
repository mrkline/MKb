#include "IP.hpp"

#include <sstream>
#include <vector>
#include <cstring>

#include "Exceptions.hpp"
#include "Portability.hpp"

using namespace std;
using namespace Exceptions;

IP::IP()
{
	memset(octets, 0, sizeof(octets));
	updateString();
}

IP::IP(const string& ipStr)
{
	octetsFromString(ipStr);
	updateString();
}

IP::IP(unsigned char oct0, unsigned char oct1, unsigned char oct2, unsigned char oct3)
{
	octets[0] = oct0;
	octets[1] = oct1;
	octets[2] = oct2;
	octets[3] = oct3;

	updateString();
}

IP::IP(uint32_t bin, ByteOrder order /* = BO_HOST */)
{
	octetsFromBinary(bin, order);
	updateString();
}

void IP::set(unsigned char oct0, unsigned char oct1, unsigned char oct2, unsigned char oct3)
{
	octets[0] = oct0;
	octets[1] = oct1;
	octets[2] = oct2;
	octets[3] = oct3;

	updateString();
}

unsigned char IP::getOctet(unsigned char octetNum) const
{
	if (octetNum > 3)
		throw ArgumentOutOfRangeException("IPs have octets [0,4)", __FUNCTION__);

	return octets[octetNum];
}

void IP::setOctet(unsigned char octetNum, unsigned char val)
{
	if (octetNum > 3)
		throw ArgumentOutOfRangeException("IPs have octets [0,4)", __FUNCTION__);

	octets[octetNum] = val;
	updateString();
}

uint32_t IP::getAsBinary(ByteOrder order /* = host */) const
{
	typedef uint32_t ui; // to save some typing
	ui ret = ((ui)octets[0] << 24) | ((ui)octets[1] << 16) | ((ui)octets[2] << 8) | ((ui)octets[3]);
	return order == BO_HOST ? ret : htonl(ret);
}

IP& IP::operator=(const std::string& str)
{
	octetsFromString(str);
	updateString();
	return *this;
}

IP& IP::operator=(uint32_t bin)
{
	octetsFromBinary(bin, BO_HOST);
	updateString();
	return *this;
}

bool IP::operator==(const IP& other) const
{
	for (int c = 0; c < 4; ++c) {
		if (octets[c] != other.octets[c])
			return false;
	}
	return true;
}

void IP::updateString()
{
	stringstream ss;
	ss << (int)octets[0] << "." << (int)octets[1] << "." << (int)octets[2] << "." << (int)octets[3];
	strRep = ss.str();
}

void IP::octetsFromString(const string& ipStr)
{
	// Split the string using '.' as the delimetor
	vector<string> octetStrs;
	stringstream ss(ipStr);
	string octet;

	while (getline(ss, octet, '.')) {
		octetStrs.push_back(octet);
	}

	if (octetStrs.size() != 4)
		throw ArgumentException( "An IP address could not be constructed from the given string", __FUNCTION__);

	octets[0] = atoi(octetStrs[0].c_str());
	octets[1] = atoi(octetStrs[1].c_str());
	octets[2] = atoi(octetStrs[2].c_str());
	octets[3] = atoi(octetStrs[3].c_str());
}

void IP::octetsFromBinary(uint32_t bin, ByteOrder order /* = BO_HOST */)
{
	if (order == BO_NET)
		bin = ntohl(bin);
	octets[0] = (0xFF000000 & bin) >> 24;
	octets[1] = (0x00FF0000 & bin) >> 16;
	octets[2] = (0x0000FF00 & bin) >> 8;
	octets[3] = (0x000000FF & bin);
}
