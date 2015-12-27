#include "fcb.h"

bool operator==(const FCBid& lhs, const FCBid& rhs) {
	return lhs.part == rhs.part && lhs.entry == rhs.entry;
}

bool operator!=(const FCBid& lhs, const FCBid& rhs) {
	return !(lhs == rhs);
}

FCBid::operator int() const {
	return (entry + part)*(entry + part + 1) / 2 + part;
}


void FCB::startMode(char mode) {
	filesOp++;
	if (mode == 'r')
		rw.startRead();
	else
		rw.startWrite();
}

void FCB::closeMode(char mode) {
	if (mode == 'r')
		rw.stopRead();
	else
		rw.stopWrite();
	filesOp--;
}

void FCB::parseName(char *fpath, char *name) {
	char p_len = 3;
	char *pch = strchr(fpath, '.');
	strncpy_s(name,/*sizeof name*/8, fpath + p_len, pch - fpath - p_len);
}

void FCB::parseExt(char *fpath, char *ext) {
	char* pch = strchr(fpath, '.');
	strncpy_s(ext,/*sizeof ext*/4, fpath + (pch - fpath + 1), 3);
}
