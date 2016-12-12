const int small=+49,big=-17;
const char z='a';
int i,j;
char op;
void fun(int num,char useless2)
{
    int tmp2;
    char tmp;
	tmp=op;
	tmp2=op;
	printf(tmp);
	printf(tmp2;
    return;
}
char inputchar()
{
    char a;
    scanf(a);
    if(a=='+')
    {
        op=a;
    }
    else
    {
            if(a== '-')op=a;
            else op='*';
    }
    return (op);
}
int input()
{
    const int useless3=1;
    int questiontimes,num;
    num=0;
    scanf(questiontimes);
    if(questiontimes>0) do
    {
        op=inputchar();
        if(op!='*')
        {
            return (num);
        }
        questiontimes=questiontimes-1;
    }while(questiontimes>0)
    return (num);
}
void main()
{
    const char useless2='u';
    char a;
    a=input();
    fun(a,useless2);
    return;
}