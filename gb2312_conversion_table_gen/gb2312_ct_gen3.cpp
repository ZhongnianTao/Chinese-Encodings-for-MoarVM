#include<bits/stdc++.h>
using namespace std;

string input_rec[7500][2];
string ret[94][94];
int a[65511];

int f(char c)
{
	if ('0'<=c && c<='9') return c-'0';
	return c-'a'+10;
}

void gen1()
{
	for(int i=0;i<94;i++)
	{
		for(int j=0;j<94;j++)
		{
			ret[i][j]="-1";
		}
	}
	
	for(int i=1;i<=7445;i++)
	{
		string s1=input_rec[i][0];
		string s2=input_rec[i][1];
		
		int a=f(s1[0])*16+f(s1[1])-161;
		int b=f(s1[2])*16+f(s1[3])-161;
		
		ret[a][b]=s2;
	}
	
	cout<<"const static MVMGrapheme32 gb2312_index_to_cp_record[87][94]="<<endl;
	cout<<'{';
	
	int len=1;
	for(int i=0xa1-0xa1;i<=0xf7-0xa1;i++)
	{
		if (i) cout<<',';
		cout<<endl;
		cout<<'{';
		len=1;
		for(int j=0;j<94;j++)
		{
			if (j) cout<<',';
			cout<<ret[i][j];
			len+=1+ret[i][j].length();
			if (len>=75)
			{
				cout<<endl;
				len=0;
			}
		}
		cout<<'}';
	}
	cout<<"};"<<endl;
	cout<<endl;
}

void gen2()
{
	memset(a,0,sizeof(a));
	int x,y;
	
	for(int i=1;i<=7445;i++)
	{
		stringstream ss;
		ss<<input_rec[i][0]<<' '<<input_rec[i][1];
		ss>>hex>>x>>y;
		a[y]=x;
	}
	
	int len=0;
	
	cout<<"const static MVMint32 gb2312_cp_to_index_record[65511]="<<endl;
	cout<<'{';
	len=1;
	for(int i=0;i<65511;i++)
	{
		if (i) {cout<<',';len++;}
		if (len>=75)
		{
			cout<<endl;
			len=0;
		}
		cout<<a[i];
		if (a[i]==0) len++;
		else
		{
			int t=a[i];
			while (t)
			{
				t/=10;
				len++;
			}
		}
		
	}
	
	cout<<"};"<<endl;
}

int main()
{
	freopen("gb2312_full_character_list_converted.txt","r",stdin);
	freopen("gb2312_conversion_table.txt","w",stdout);
	
	for(int i=1;i<=7445;i++)
	{
		cin>>input_rec[i][0]>>input_rec[i][1];
	}
	
	gen1();
	gen2();
	
	return 0;
}

