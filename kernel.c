//J Nichols Project B 

#include <stdio.h>

void printString(char*);
void printChar(char c*);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);

void main()
{
	char line[80];
	makeInterrupt21();
	interrupt(0x21, 1, line, 0, 0);
	interrupt(0x21, 0, line, 0, 0);
	while(1);
}

void printString(char* chars)
{
	int i = 0;
	while (chars[i] != 0x0)
	{
		interrupt(0x10, 0xe * 256 + chars[i], 0, 0, 0);
		i++;
	}
}

void printChar(char c)
{
	interrupt(0x10, 0xe * 256 + c, 0, 0, 0);
}

void readString(char* string)
{
	int stop = 0;
	int i = 0;
	printString("Enter a line:  \0");
	while (stop != 1)
	{
		char in = interrupt(0x16, 0, 0, 0, 0);
		if (i != 0)
		{
			printChar(0x8);
			printChar(' ');
			printChar(0x8);
			i--;
		}
		else  if (in != 0x8)
                {
                        printChar(in);  
                        string[i] = in;
                        i++;
                }
		else 
		{
			printChar(0xd);
			printChar(0xa);
			string[i] = 0xd;
			i++;
			string[i] = 0xa;
			i++;
			string[i] = 0x0;
			stop = 1;
		}
	}
}

void readSector(char* buffer, int sector)
{
	int ah = 2;
	int al = 1;
	int ch = 0;
	int cl = sector + 1;
	int dh = 0;
	int dl = 0x80;
	
	int ax = ah * 256 + al;
	int cx = ch * 256 + cl;
	int dx = dh * 256 + dl;

	interrupt(0x13, ax, buffer, cx, dx);
}

void handeInterrupt21(int ax, int bx, int cx, int dx)
{
	if(ax == 0)
	{
		printString(bx);
	}
	if(ax == 1)
	{
		readString(bx);
	} 
	if(ax == 2)
	{
		readSector(bx, cx);
 	}
	if(ax >= 3)
	{
		printString("Error\n");
	}
}


