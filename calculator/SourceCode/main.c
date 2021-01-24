#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "rational.h"

void Init()
{
	setbuf(stdin,0);
	setbuf(stdout,0);
	setbuf(stderr,0);
}

void menu()
{
	puts("1.plus");
	puts("2.minus");
	puts("3.multiply");
	printf("Your Choice :");
}

long long my_read()
{
	char buf[0x10];
	read(0,buf,0xC);
	return atoi(buf);
}

int main()
{
	Init();
	char a[0x10]="";
	char b[0x10]="";
	long long choice;
	while (1)
	{
		printf("Tell me the first Number :"); read(0,a,0x10);
		printf("Tell me the second Number :"); read(0,b,0x10);
		long long len;
		len=strlen(a); if (a[len-1]=='\n') a[len-1]='\x00';
		len=strlen(b); if (b[len-1]=='\n') b[len-1]='\x00';
		BigInt* x=big_int_from_str(a);
		BigInt* y=big_int_from_str(b);
		menu();
		choice=my_read();
		if (choice==1)
		{
			char* str=big_int_to_str(big_int_add(x,y));
			printf("ans = ");
			puts(str);
		}
		else if(choice==2)
		{
			char* str=big_int_to_str(big_int_sub(x,y));
			printf("ans = ");
			puts(str);
		}
		else if(choice==3)
		{
			char* str=big_int_to_str(big_int_mul(x,y));
			printf("ans = ");
			puts(str);
		}
		big_int_free(x);
		big_int_free(y);
	}
}
