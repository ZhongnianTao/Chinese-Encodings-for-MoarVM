/*
	Function: 	Generate Arrays in gb2312_codeindex.h
	Usage: 		Compile and run gb2312_ct_gen1.cpp, 
				gb2312_ct_gen2.py, gb2312_ct_gen3.cpp.
				The arrays will be stored in gb2312_conversion_table.txt
	
	Individual Functions:
	
	gb2312_ct_gen1.cpp generates a list of all 7445 gb2312 characters 
	togetner with their codepoints into gb2312_full_character_list.txt
	
	gb2312_ct_gen2.py translate the codepoints into unicode, and store
	the results into gb2312_full_character_list_converted.txt
	
	gb2312_ct_gen3.cpp generates the final conversion table into 
	gb2312_conversion_table.txt
*/ 

#include<bits/stdc++.h>
using namespace std;

bool check(int zone, int point)
{
	if (zone < 1 || (10 <= zone && zone <= 15) || zone > 87)
		return false;
	if (point < 1 || point > 94)
		return false;
	if (zone == 2 && (point <= 16 || point == 67 || point == 68 || point == 79 
		|| point == 80 || point >= 93))
		return false;
	if (zone == 4 && point >= 84)
		return false;
	if (zone == 5 && point >= 87)
		return false;
	if (zone == 6 && (point >= 25 && point <= 32 || point >= 57))
		return false;
	if (zone == 7 && (point >= 34 && point <= 48 || point >= 82))
		return false;
	if (zone == 8 && (point >= 27 && point <= 36 || point >= 74))
		return false;
	if (zone == 9 && (point <= 3 || point >= 80))
		return false;
	if (zone == 55 && point >= 90)
		return false;
	
	return true;
}

int main()
{
	freopen("gb2312_full_character_list.txt","w",stdout);
	int cnt=0;
	for(int i=0xa1;i<=0xfe;i++)
	{
		for(int j=0xa1;j<=0xfe;j++)
		{
			int zone=i-160;
			int point=j-160;
			if (check(zone, point))
			{
				string s1="";
				s1=s1+(char)(i);
				s1=s1+(char)(j);
				cout<<s1<<' ';
				cout<<hex<<i<<j<<endl;
				cnt++; 
			}
		}
	}
	assert(cnt==7445);
	return 0;
}

