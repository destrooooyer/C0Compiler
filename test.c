const int constA=10,constB=20;
const char constC='2';
int globalArr[20];
int fibArr[10];
int i,n;

int fib(int x)
{
	if(fibArr[x]==0)
		fibArr[x]=fib(x-1)+fib(x-2);
	return(fibArr[x]);
}

void sort()
{
	int i,j;
	int temp;
	for(i=0;i<n;i=i+1)
	{
		for(j=i+1;j<n;j=j+1)
		{
			if(globalArr[i]>globalArr[j])
			{
				temp=globalArr[i];
				globalArr[i]=globalArr[j];
				globalArr[j]=temp;
			}
		}
	}
}

void main()
{
	int i,temp;
	fibArr[0]=1;
	fibArr[1]=1;
	fib(9);
	i=0;
	printf("fib[0-9] are as follows:");
	do
	{
		printf(fibArr[i]);
		i=i+1;
	}
	while(i<10)
	
	printf("please input a number n");
	scanf(n);
	if(n>10)
	{
		n=10;
		printf("n is too large, so it is set to 10");
	}
	printf("please input an array of n ints");
	
	for(i=0;i<n;i=i+1)
	{
		scanf(temp);
		globalArr[i]=temp;
	}
	
	sort();
	for(i=0;i<n;i=i+1)
	{
		printf(globalArr[i]);
	}
	
	n=constB/constA+constC-(432+((3-43)*(4/3))-5+3);
	printf(n);
	
	
}