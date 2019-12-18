# Advanced-compiler-in-C-
Making a compiler from Java to Assembly language in c++, using Binary Syntax Tree parsing method.

The compiler can take a simple like java code file:

int a = 0;
int b = 1;
int c = 0;


if(a < 0)
{ 
	a = 0;
}
if(b!= 1)
{
	b = 1;
}

for(7,0,1)
{
	c = a;
	c += b;
	a = b;
	b = c;
}

and convert it to:

a  DWORD  0
b  DWORD  1
c  DWORD  0

MOV dx,a
CMP dx, 0
JG line0
MOV ax,0
MOV a,ax

line0:
MOV dx,b
CMP dx, 1
 line1
MOV ax,1
MOV b,ax

line1:

MOV cx,0
for_loop_0:
MOV ax,b
ADD c,ax
MOV ax,a
MOV c,ax
MOV ax,b
MOV a,ax
MOV ax,c
MOV b,ax

ADD cx,1
CMP cx,7
JL for_loop_0

