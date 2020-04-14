#include <iostream>
#include <vector>
#include <initializer_list>
#include <string>
#include<bitset>

using namespace std;


template <typename T> class vectorbool;

template<>
class vectorbool<bool>
{
	unsigned long* bits;
	size_t size;
public:
	vectorbool(const initializer_list<bool> &il)
		:size(il.size()), bits(nullptr)
	{
		if (size < sizeof(unsigned long) * CHAR_BIT)
			bits = new unsigned long();
		else
			throw "the size of the initializer list is too big";

		size_t position = 0;
		unsigned long mask = 0;
		for (auto i : il)
		{
			mask = i ? 1ul : 0ul;
			mask <<= position++;
			*bits |= mask;
		}		
	}
	~vectorbool()
	{
		delete bits;
	}

	size_t  getsize() const { return size; }
	unsigned long getbits() const { return *bits; }

	class referencevb
	{
		bool _data;
		size_t index;
		unsigned long* bits;
		friend class vectorbool;
		bool getBoolFromPosition(size_t index)
		{
			unsigned long mask = 1ul << index;
			return static_cast<bool>(*bits & mask);
		}
		void setBoolToPosition(const bool data, const size_t position)
		{
			unsigned long mask = 1ul<<position;

			if (data)
			{
				*bits |= mask;
			}
			else
			{
				*bits &= ~mask;
			}			
		}
	public:
		referencevb() {};
		bool data() const { return _data; }
		referencevb(const referencevb& right)
			:_data(right._data), index(right.index), bits(right.bits)
		{
		}

		referencevb(size_t position, unsigned long* bits)
			:index(position), bits(bits)
		{
			_data = getBoolFromPosition(index);
		}

		referencevb& operator=(const referencevb& right)
		{
			_data = right.data();
			setBoolToPosition(_data, index);
			return *this;
		}

		referencevb& operator=(const bool& right)
		{
			_data = right;
			setBoolToPosition(_data, index);
			return *this;
		}
		operator bool() const
		{
			return _data;
		}
		
	};

	vectorbool<bool>::referencevb operator[](unsigned int index)
	{
		if (index > getsize())
		{
			throw "attempt to go beyond the array";
		}
		return vectorbool<bool>::referencevb(index, bits);
	};
	
};


ostream & operator<<(ostream & os, const vectorbool<bool>::referencevb& vbr)
{
	os << vbr.data();
	return os;
}


//main
void main() try
{

	vectorbool<bool> vb = { true, false, false, true, true };

	for (int i = 0; i < vb.getsize(); ++i)
	{
		cout << " " << vb[i];
	}
	cout << endl;
	cout << std::bitset<sizeof(vb.getbits())*CHAR_BIT>(vb.getbits()) << endl << endl;

	vb[0] = false;
	cout << " vb[0]   = false " << vb[0] << endl;
	cout << std::bitset<sizeof(vb.getbits())*CHAR_BIT>(vb.getbits()) << endl << endl;

	vb[0] = true;
	cout << " !(vb[0] = true) " << !vb[0] << endl;
	cout << std::bitset<sizeof(vb.getbits())*CHAR_BIT>(vb.getbits()) << endl << endl;


	// Invoke vector<bool>::reference::operator=()
	vectorbool<bool>::referencevb refelem0 = vb[0];
	vectorbool<bool>::referencevb refelem1 = vb[1];
	vectorbool<bool>::referencevb refelem2 = vb[2];
	vectorbool<bool>::referencevb refelem3 = vb[3];
	vectorbool<bool>::referencevb refelem4 = vb[4];

	bool b0 = refelem0;
	bool b1 = refelem1;
	bool b2 = refelem2;
	bool b3 = refelem3;
	bool b4 = refelem4;
	cout << "The original value of the 0st element stored in a bool: " << b0 << endl;
	cout << "The original value of the 1nd element stored in a bool: " << b1 << endl;
	cout << "The original value of the 2rd element stored in a bool: " << b2 << endl;
	cout << "The original value of the 3rd element stored in a bool: " << b3 << endl;
	cout << "The original value of the 4rd element stored in a bool: " << b4 << endl;
	cout << endl;

	refelem2 = refelem3;
	cout << " refelem2 = refelem3 " << refelem2 << endl;
	cout << std::bitset<sizeof(vb.getbits())*CHAR_BIT>(vb.getbits()) << endl << endl;

	refelem4 = false;
	cout << " refelem4 = false    " << refelem4 << endl;
	cout << std::bitset<sizeof(vb.getbits())*CHAR_BIT>(vb.getbits()) << endl << endl;
	
}
catch (const char* err)
{
	cout << "  ERROR:  " << err <<  endl;
}

