#include "iostream"
#include "cstring"

using namespace std;

class Node
{
	public:
		char 
			name[ 40 ];
		int
			type;
	      	Node 
			*next;

		Node( char _name[], int _type )
		{
			int
				i = 0;
	
			while( (_name[ i ] != '\0') && (i != 39) )
			{
				name[ i ] = _name[ i ];
				++i;
			}
			name[ i ] = '\0';
			type = _type;
			next = 0;
		}
};

class SinglyLL
{
	public:
		Node *head;
		Node *tail;

		SinglyLL()
		{	
			head = tail = 0;
		}

		~SinglyLL()
		{
			truncateList();
		}

	      	bool isEmpty();
	      	void addToHead( char [], int );
	      	int isInList( char [] );
	      	void truncateList();
	      	void display();
};

bool SinglyLL :: isEmpty()
{
 	if( head == tail && tail == 0 )
	{
		return 1;
	}
	return 0;
}

void SinglyLL :: addToHead( char ele[], int _type )
{
     	if( isEmpty() )
     	{
		head = new Node( ele, _type );
		tail = head;
     	}
     	else
     	{
		Node 
			*temp;

		temp = new Node( ele, _type );
		temp -> next = head;
		head = temp;
     	}
	return;
}

void SinglyLL :: truncateList()
{
	Node
		*temp = head;
	
	while( head != 0 )
	{
		temp = head;
		head = head -> next;
		delete temp;
	}
	head = tail = 0;
	return;
}

int SinglyLL :: isInList( char ele[] )
{
	if( isEmpty() )
	{
		return 0;
	}

	Node
		*temp = head;

	while( temp != 0 )
	{
		if( !strcmp( ele, temp -> name ) )
		{
			return 1;
		}
		temp = temp -> next;
	}
	return 0;
}

void SinglyLL :: display()
{
	if( isEmpty() )
	{	
		return;
	}
	else
	{    
		for( Node *temp = head; temp != 0; temp = temp -> next )
		{
	      		if( (temp -> type) != -1 )
			{
				cout << temp -> name << "\t - \t" << dataTypes[(temp -> type)] << "\n";;
			}
			else
			{
				cout << temp -> name << ", ";
			}
		}
	}
	return;
}


