//File bitvector.h
#ifndef _bitvector_h_
#define _bitvector_h_

class BitVectorBase {
protected:
	class bitref { 
		char &a_char;
		char mask;

	public:
		bitref(char &a_char_, char mask_) : a_char(a_char_), mask(mask_){
		}

		const bitref &operator =(bool val) {
			if(val)
				a_char |= mask;
			else
				a_char &= ~mask;
			return *this;
		}

		const bitref &operator =(const bitref &br) {
			return this->operator =(bool(br));
		}

		operator bool() const {
			return ((a_char & mask) != 0) ? true : false;
		}
	};
};

class BitVector : public BitVectorBase{

	char *chars;

public:
	BitVector(char *chars){
		this->chars = chars;
	}

	bool operator [](unsigned long bitnum) const {
		const unsigned long bytenum = bitnum / 8;
		bitnum = bitnum % 8;
		return ((chars[bytenum] & (char(1) << bitnum)) != 0) ? true : false;
	}

	bitref operator[](unsigned long bitnum) {
		const unsigned long bytenum = bitnum / 8;
		bitnum = bitnum % 8;
		char mask = char(1) << bitnum;
		return bitref(chars[bytenum], mask);
	}

	void clearBitVector(){
		memset(chars,0,ClusterSize);
	}
};
#endif
