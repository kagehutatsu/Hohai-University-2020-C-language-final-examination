
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rational.h"

// -----------------------------------------------------------------------------

struct Link;
enum Sign;

typedef struct Link   Link;
typedef enum Sign     Sign;
typedef long long     Int;

// -----------------------------------------------------------------------------

struct Link {
    Int n, x;

    Link* prior;
    Link* next;
};

enum Sign {
    plus  = 1,
    minus = -1
};

struct BigInt {
    Sign sign;
    Int  size;

    Link* head;
    Link* tail;
};

// -----------------------------------------------------------------------------

static const Int ignore = 0;

static BigInt* get_big_int()
{
    BigInt* bi = malloc(sizeof(BigInt));
    bi->sign = plus;
    bi->size = 0;
    bi->head = bi->tail = 0;
    return bi;
}

static Link* get_link(Int n, Int x)
{
    Link* link = malloc(sizeof(Link));
    link->n     = n;
    link->x     = x;
    link->prior = link->next = 0;
    return link;
}

static void insert_front(BigInt* bi, Int n, Int x)
{
    Link* link = get_link(n, x);
    if (bi->size == 0)
        bi->head = bi->tail = link;
    else {
        bi->head->prior = link;
        link->next      = bi->head;
        bi->head = link;
    }
    ++bi->size;
}

static void insert_back(BigInt* bi, Int n, Int x)
{
    Link* link = get_link(n, x);
    if (bi->size == 0)
        bi->head = bi->tail = link;
    else {
        bi->tail->next = link;
        link->prior    = bi->tail;
        bi->tail       = link;
    }
    ++bi->size;
}

static void big_int_swap(BigInt* x, BigInt* y)
{
    BigInt z = *x;
    *x = *y;
    *y = z;
}

static int big_int_is_zero(const BigInt* bi)
{
    return bi && bi->size == 1 && bi->head->n == 0;
}

// -----------------------------------------------------------------------------

BigInt* big_int_from_ll(long long x)
{
    char x_str[512];
    snprintf(x_str, 512, "%lld", x);
    return big_int_from_str(x_str);
}

BigInt* big_int_copy(const BigInt* x)
{
    if (!x)
        return 0;

    BigInt* y = get_big_int();
    for (Link* link = x->head; link; link = link->next)
        insert_back(y, link->n, link->x);
    y->sign = x->sign;
    return y;
}

void big_int_free(BigInt* bi)
{
    if (!bi)
        return;

    Link* link = bi->head;
    while (link) {
        Link* p = link;
        link = link->next;
        free(p);
    }
    free(bi);
}

long long big_int_to_ll(const BigInt* bi)
{
    long long n     = -1;
    char*     n_str = big_int_to_str(bi);
    if (n_str) { sscanf(n_str, "%lld", &n); }
    free(n_str);
    return n;
}

// -----------------------------------------------------------------------------

char* big_int_to_str(const BigInt* a)
{
	if (a==NULL) return NULL;
	Link* ptr=a->head;
	char* str=calloc(1,a->size+0x10);
	if (a->sign==-1) strncpy(str,"-",1);
	for (int i=0; ptr!=NULL; i++)
	{
		if (str[i]=='-') continue;
		if (ptr->x<=9&&ptr->x>=0)
		{
			*(str+i)=(char)(ptr->x+0x30);
			ptr=ptr->next;
		}
		else break;
	}
	return str;
}

BigInt* big_int_from_str(const char* str)
{
	long long length=strlen(str);
	BigInt* temp=get_big_int();
	if (str[0]=='-') temp->sign=-1;
	for (int i=0; i<=length-1; i++)
	{
		if (str[i]=='-') continue;
		int num=str[i]-0x30;
		insert_back(temp,i+1,num);
	}
	return temp;
}

int big_int_cmp(const BigInt* a, const BigInt* b)
{
	int int_cmp(int sign,int x,int y)
	{
		if (x>y) return sign;
		if (x==y) return 0;
		if (x<y) return -sign;
	}
	
	if (a->sign!=b->sign) return int_cmp(1,a->sign,b->sign);
	int std_cmp_sign=a->sign;
	if (a->size!=b->size) return int_cmp(std_cmp_sign,a->size,b->size);
	Link* ptr_a=a->head;
	Link* ptr_b=b->head;
	for ( ;ptr_a!=NULL; )
	{
		if (ptr_a->x!=ptr_b->x) return int_cmp(std_cmp_sign,ptr_a->x,ptr_b->x);
		else ptr_a=ptr_a->next,ptr_b=ptr_b->next;
	}
	return 0;
}

static void link_swap(Link* a,Link *b)
{
	Link temp = *a;
	*a = *b;
	*b = temp;
}

static BigInt* unsigned_big_int_add(const BigInt* a,const BigInt* b)
{
	BigInt* temp=get_big_int();
	Link* ptr_a=a->tail;
	Link* ptr_b=b->tail;
	int num,plus=0;
	for (int i=1; ptr_b!=NULL&&ptr_a!=NULL; i++,ptr_a=ptr_a->prior,ptr_b=ptr_b->prior)
	{
		num=ptr_a->x+ptr_b->x+plus;
		if (num>=10) plus=1,num-=10; else plus=0;
		insert_front(temp,i,num);
	}
	Link* ptr=NULL; int size=0;
	if (ptr_a==NULL) ptr=ptr_b,size=a->size; else ptr=ptr_a,size=b->size;
	for (int i=1; ptr!=NULL; i++,ptr=ptr->prior)
	{
		num=ptr->x+plus;
		if (num>=10) plus=1,num-=10; else plus=0;
		insert_front(temp,size+i,num);
	}
	if (plus==1) insert_front(temp,temp->size+1,1);
	return temp;	
}

static BigInt* unsigned_big_int_sub(const BigInt* a,const BigInt* b)
{
	BigInt* temp=get_big_int();
	Link* ptr_a=a->tail;
	Link* ptr_b=b->tail;
	int num,minus=0;
	for (int i=1; ptr_b!=NULL&&ptr_a!=NULL; i++,ptr_a=ptr_a->prior,ptr_b=ptr_b->prior)
	{
		num=ptr_a->x-ptr_b->x-minus;
		if (num<0) minus=1,num+=10; else minus=0;
		insert_front(temp,i,num);
	}	
	Link* ptr=NULL; int size=0;
	ptr=ptr_a; size=b->size;
	for (int i=1; ptr!=NULL; i++,ptr=ptr->prior)
	{
		num=ptr->x-minus;
		if (num<0) minus=1,num+=10; else minus=0;
		insert_front(temp,size+i,num);
	}
	if (temp->head->x==0&&temp->size!=1) 
	{
		Link* ptr=temp->head;
		temp->head=temp->head->next;
		temp->head->prior=0;
		temp->size--;
		free(ptr); ptr=NULL;
	}
	return temp;
}

BigInt* big_int_add(const BigInt* a,const BigInt* b)
{
	BigInt* ans=NULL;
	if (a->sign==b->sign)
	{
		ans=unsigned_big_int_add(a,b);
		if (a->sign==-1) ans->sign=-1;
	}
	else
	{
		if (a->sign==-1)
		{
			BigInt* abs_a=get_big_int();
			abs_a=big_int_copy(a);
			abs_a->sign=1;
			if (big_int_cmp(abs_a,b)==1)
			{
				ans=big_int_sub(abs_a,b);
				ans->sign=-1;
			}
			else ans=big_int_sub(b,abs_a);
			free(abs_a); abs_a=NULL;
		}
		else
		{
			BigInt* abs_b=get_big_int();
			abs_b=big_int_copy(b);
			abs_b->sign=1;
			if (big_int_cmp(abs_b,a)==1)
			{
				ans=big_int_sub(abs_b,a);
				ans->sign=-1;
			}
			else ans=big_int_sub(a,abs_b);
			free(abs_b); abs_b=NULL;
		}
	}
	return ans;
}

BigInt* big_int_sub(const BigInt* a,const BigInt* b)
{
	BigInt* ans=NULL;
	int cmp_ans=big_int_cmp(a,b);
	if (a->sign==b->sign) 
	{
		if (a->sign==1)
		{
			if (cmp_ans==1||cmp_ans==0) ans=unsigned_big_int_sub(a,b);
			if (cmp_ans==-1) ans=unsigned_big_int_sub(b,a);
		}
		if (a->sign==-1)
		{
			if (cmp_ans==-1||cmp_ans==0) ans=unsigned_big_int_sub(a,b),ans->sign=-1;
			if (cmp_ans==1) ans=unsigned_big_int_sub(b,a);	
		}
	}
	else
	{
		ans=unsigned_big_int_add(a,b);
		if (a->sign==-1) ans->sign=-1;
	}
	return ans;
}

static BigInt* ll_big_int_mul(const BigInt* a,long long mul)
{
	BigInt* temp=get_big_int();
	Link* ptr=a->tail;
	int num,plus=0;
	for (int i=1; ptr!=NULL; i++,ptr=ptr->prior)
	{
		num=ptr->x*mul+plus;
		if (num>=10) plus=num/10,num%=10; else plus=0;
		insert_front(temp,i,num);
	}
	if (plus!=0) insert_front(temp,temp->size+1,plus);
	return temp;
}

BigInt* big_int_mul(const BigInt* a,const BigInt* b)
{
	BigInt* temp=get_big_int();
	Link* ptr=b->tail;
	for (int i=1; ptr!=NULL; i++,ptr=ptr->prior)
	{
		BigInt* position=get_big_int();
		position=ll_big_int_mul(a,ptr->x);
		for (int j=1; j<i; j++) position=ll_big_int_mul(position,10);
		temp=big_int_add(temp,position);
	}
	if (a->sign!=b->sign) temp->sign=-1;
	return temp;
}

BigInt* big_int_div(const BigInt* a,const BigInt* b,BigInt** rem)
{
	BigInt* temp=get_big_int();
	long long x=big_int_to_ll(a);
	long long y=big_int_to_ll(b);
	temp=big_int_from_ll(x/y);
	*rem=big_int_from_ll(x%y);
	return temp;
}

