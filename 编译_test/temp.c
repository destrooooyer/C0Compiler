int add(int a,int b)
{
	return (a+b);
}

void main()
{
	int a,b,c,d;
	a=1;
	b=2;
	c=a+b;
	d=a+b+c+add(a,b)+(a+b);
	
}