#include "IP.h"

#include <sstream>
#include <vector>

#include "Exceptions.h"

using namespace std;

IP::IP(const string& ipStr)
{
	OctetsFromString(ipStr);
	UpdateString();
}

IP::IP(unsigned char oct0, unsigned char oct1, unsigned char oct2, unsigned char oct3)
{
	octets[0] = oct0;
	octets[1] = oct1;
	octets[2] = oct2;
	octets[3] = oct3;

	UpdateString();
}

IP::IP(unsigned int bin)
{
	OctetsFromBinary(bin);
	UpdateString();
}

unsigned char IP::GetOctet( unsigned char octetNum ) const
{
	if(octetNum > 3)
		throw ArgumentOutOfRangeException("IPs have octets [0,4)", __FUNCTION__);

	return octets[octetNum];
}

void IP::SetOctet( unsigned char octetNum, unsigned char val )
{
	if(octetNum > 3)
		throw ArgumentOutOfRangeException("IPs have octets [0,4)", __FUNCTION__);

	octets[octetNum] = val;
	UpdateString();
}

unsigned int IP::GetAsBinary() const
{
	typedef unsigned int ui; // to save some typing
	return ((ui)octets[0] << 24) + ((ui)octets[1] << 16) + ((ui)octets[2] << 8) + (ui)octets[3];
}

IP& IP::operator=( const std::string& str )
{
	OctetsFromString(str);
	UpdateString();
	return *this;
}

IP& IP::operator=(unsigned int bin )
{
	OctetsFromBinary(bin);
	UpdateString();
	return *this;
}

bool IP::operator==(const IP& other) const
{
	for(int c = 0; c < 4; ++c)
	{
		if(octets[c] != other.octets[c])
			return false;
	}
	return true;
}

void IP::UpdateString()
{
	stringstream ss;
	ss << (int)octets[0] << "." << (int)octets[1] << "." << (int)octets[2] << "." << (int)octets[3];
	strRep = ss.str();
}

void IP::OctetsFromString( const string& ipStr ) 
{
	// Split the string using '.' as the delimetor
	vector<string> octetStrs;
	stringstream ss(ipStr);
	string octet;

	while(getline(ss, octet, '.'))
	{
		octetStrs.push_back(octet);
	}

	if(octetStrs.size() != 4)
		throw ArgumentException("An IP address could not be constructed from the given string", __FUNCTION__);

	octets[0] = atoi(octetStrs[0].c_str());
	octets[1] = atoi(octetStrs[1].c_str());
	octets[2] = atoi(octetStrs[2].c_str());
	octets[3] = atoi(octetStrs[3].c_str());
}

void IP::OctetsFromBinary(unsigned int bin )
{
	octets[0] = (0xFF000000 & bin) >> 24;
	octets[1] = (0x00FF0000 & bin) >> 16;
	octets[2] = (0x0000FF00 & bin) >> 8;
	octets[3] = (0x000000FF & bin);
}
