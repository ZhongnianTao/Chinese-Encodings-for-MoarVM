#include<bits/stdc++.h>
using namespace std;

int input_rec[7500][2];
int ret[94][94];
int a[65511];

void gen1()
{
	/* This function generates mappings from gb2312(EUC-CN form) to unicode */
	/* Initialize */
	for(int i=0;i<94;i++)
	{
		for(int j=0;j<94;j++)
		{
			ret[i][j]=-1;
		}
	}
	/* Convert EUC-CN code into two 8-bit integer*/
	for(int i=1;i<=7445;i++)
	{
		int a=input_rec[i][0]/256-161;
		int b=input_rec[i][0]%256-161;
		ret[a][b]=input_rec[i][1];
	}
	
	cout<<"const static MVMGrapheme32 gb2312_index_to_cp_record[87][94]="<<endl;
	cout<<'{';
	
	/* 'len' is used to record the length of current line */
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
			if (ret[i][j]!=-1)
			{
				cout<<hex<<"0x"<<ret[i][j];
				len+=7;
			}
			else
			{
				cout<<"-1";
				len+=3;
			}
			if (len>=75)
			{
				/* Start a new line when there're enough characters */
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
	/* This function generates mappings for gb2312 characters 
	    from unicode to gb2312(EUC-CN form) */
	
	/* Initialize */
	memset(a,0,sizeof(a));
	for(int i=1;i<=7445;i++) a[input_rec[i][1]]=input_rec[i][0];
	
	cout<<"const static MVMint32 gb2312_cp_to_index_record[65511]="<<endl;
	cout<<'{';
	
	/* 'len' is used to record the length of current line */
	int len=1;
	for(int i=0;i<65511;i++)
	{
		if (i) {cout<<',';len++;}
		if (len>=75)
		{
			cout<<endl;
			len=0;
		}
		cout<<dec<<a[i];
		if (a[i]==0) len++;
		else
		{
			/* Calculate the length of current codepoint */
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

int main(int argc, char **argv)
{
	freopen(argv[1],"r",stdin);
	freopen(argv[2],"w",stdout);
	
	int cp_count=0;
	string s;
	while (getline(cin,s))
	{
		/* Skip the license */
		if (s[0]=='#') continue;

		/* Process the input again to read hexadecimal integers */
		cp_count++;
		stringstream ss(s);
		ss>>hex>>input_rec[cp_count][0]>>input_rec[cp_count][1];
		
		/* Convert to EUC-CN form by adding 0x8080 */
		input_rec[cp_count][0]+=0x8080;
	}
	
	gen1();
	gen2();
	
	return 0;
}

