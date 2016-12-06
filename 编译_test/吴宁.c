const int _a=+1;
const int b=-1;
const char ch1='-'; 
const char ch2='n';
int result[3000];
char enter;


void testChildexpression(){
    int a,b,c,d,e;
    a=-10;
    b=-10;
    c=-10;
    b=a*c;
    c=a*c;
    d=a*c*b;
    e=a*c*b;
    printf(enter);
    printf(d+e);
}
void manyParameter(int a,int b,int c,int d,int e,int f,int g){
    int tt;
    tt=-0;
    tt=-3+a*b;
    tt=+7+tt;
    printf("MPValue: ",tt);
    printf(enter);  
    printf(+tt+a*b*c*d*e*(f-g));
}
void testCompare(){
    int a;
    a=1;
    if(a>=1)
        a=a+2;
    if(a<3)
        a=a+3;
    if(a>=3)
        a=a+4;
    if(a<7)
        a=a+4;
    printf("testCompare: ",a);
    printf(enter);
}

void testswitch(){
    int a;
    a=48;
    if(a=='0')printf("result1");
    else if(a==0)printf("result2");
    else printf("default1");
    
    printf(enter);
    a=-48;
    if(a==-48)printf("result3");
    else if(a==48)printf("result4");
    else printf("default2");
    
    printf(enter);
    if(((a+96)*4/4+19-19)==+48) printf("result5");
    else if(((a+96)*4/4+19-19)==48) printf("result6");
    else printf("default3");
    
    printf(enter);
   	if (((a+96)*4/4+19-19)==1)printf("result7");
    else if (((a+96)*4/4+19-19)==-1) printf("result8");
    else printf("default4");
    
    printf(enter);
}

void pr(int i,int j){
    if(i==-1)
        return;
    if(j==1);
        else{
        if((j-(j/50)*50)==0)
            printf(enter);
        }
   
    j=j+1;
    printf(result[i]);
    i=i-1;
    pr(i,j);
} 
int getNum(){
    const int x=1;
    int y;
    y=0;
    if(y<1000)do{
        if(y<100)
            y=y+1;
        else
            y=y+2;
    }while(y<1000)
    return (y);
}

int getZero(){
    int x;
    int re;
    x=1;
    re=0;
    if(x<=10) do{
        re=re+x;
        x=x+1;
    }while(x<=10)
    return (re-55);
}

char getChar(){
    char ch;
    ch='m';
    return (ch+1);
}

void main(){
    char ttt[10];
    int n,i,pce,j,length,t;
    char ch3;
    ch3=getChar();
    ttt[0]=getChar();
    result[0]=2;
    enter=10;
    scanf(n);
    i=3;
    if(i<=n) do{
    pce=getZero();        
    j=getZero();
    if(j<=1000) do{
    t=result[j]*i;
    result[j]=(result[j]*i+pce)-((result[j]*i+pce)/10)*10;
    pce=(t+pce)/10;
    j=j+_a;
    }while(j<=1000)
    i=i+_a;
    }while(i<=n)
    length=getNum();
    if(result[length]==0)do
        length=length+b;
        while(result[length]==0)
    i=length;
    printf("start");
    printf(ch1);
    printf(ch2);
    printf(enter);
    pr(i,getZero());
    printf("\",ch3);
    printf(enter);;;;;;;;;;;;;;;
    testCompare();
    testswitch();
    manyParameter(1,2,3,4,5,6,7);
    testChildexpression();
    printf(ttt[0]);
    printf(getChar());
    printf('m'+1);   
    
}