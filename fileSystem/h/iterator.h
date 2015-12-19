//File: iterator.h
#ifndef _iterator_h_
#define _iterator_h_

class IndexAlloc;
typedef unsigned long BytesCnt;

class Iterator{
protected:
	bool smallFile = false;
	BytesCnt current;
	IndexAlloc* index;
public:
	Iterator(IndexAlloc *index){
		this->index = index;
	}
  virtual ~Iterator(){}

	virtual Iterator* copy() const =0;
	void onFirst();
	virtual void next();
	virtual bool done();
	BytesCnt curr() const{
		return current;
	}
};

class ReadIterator: public Iterator{
public:
	ReadIterator(IndexAlloc *index) : Iterator(index){}

	ReadIterator* copy() const override{
		return new ReadIterator(*this);
	}

	void next() override;
};

class WriteIterator : public Iterator{
public:
	WriteIterator(IndexAlloc *index) : Iterator(index){}

	WriteIterator* copy() const override{
		return new WriteIterator(*this);
	}

	void next() override;
	bool done() override;
};

class SeekIterator : public Iterator{
public:
	SeekIterator(IndexAlloc *index) : Iterator(index){}
	SeekIterator* copy() const override{
		return new SeekIterator(*this);
	}
	void next() override;
};
#endif
