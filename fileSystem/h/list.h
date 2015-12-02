//File: list.h
#ifndef _list_h_
#define _list_h_
template <class T> class List{

	struct Elem{
		T dat;
		int myID;
		Elem *next;
		Elem(T dd, int ii, Elem *nn = 0) : dat(dd), myID(ii), next(nn) {}
		~Elem(){
			delete dat;
		} 
	};

	Elem *first,*last;

public:
	List(){
		first = last = 0;
	} 

	~List();

	void add(T t, int id);

	void del(int);

	T find(int);
}; 

template<class T> List<T>::~List(){
	while(first){
		Elem *old = first;
		first = first->next;
		delete old;
	} 
} 

template <class T> void List<T>::add(T t, int id){
	last = !first ? first : last->next = new Elem(t, id);
} 
template <class T> void List<T>::del(int id){
	Elem *curr, *prev = 0; 
	for(curr = first;curr && curr->myID <= id; prev = curr, curr = curr->next)
		if(curr->myID == id){
			if(!prev)
				first = curr->next;
			else
				prev->next = curr->next;
			delete curr;
			break;
		} 
} 

template <class T> T List<T>::find(int id){
	for(Elem *curr = first; curr && curr->myID <= id; curr = curr->next)
		if(curr->myID == id)
			return curr->dat;
	return 0;
} 
#endif
