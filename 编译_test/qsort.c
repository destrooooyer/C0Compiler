int d[13];

void sort(int i,int j)
{
	int x,y,z;
	int tmp;
	x=i;
	y=j;
	z=d[(i+j)/2];
	
	do
	{
		if(z<d[y]) do
			y=y-1;
			while(z<d[y]);
		if(z>d[x]) do
			x=x+1;
			while(z>d[x]);
		if(x<=y)
		{
			tmp=d[x];
			d[x]=d[y];
			d[y]=tmp;
			x=x+1;
			y=y-1;
		}
	}
	while(x<=y);
	if(x<j) sort(x,j);
	if(i<y) sort(i,y);
	
	
}

void main()
{
	int i;
	d[0]=-234*((356/4) + 83 - 44*23)*-1;
	d[1]=-3234*-1;
	d[2]=+-34-23;
	sort(0,2);
	for(i=0;i<3;i=i+1)
		printf(d[i]);
}