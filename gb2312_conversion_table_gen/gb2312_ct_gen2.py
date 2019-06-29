import unicodedata

fin=open('gb2312_full_character_list.txt','r')
fout=open('gb2312_full_character_list_converted.txt','w')

for line in fin:
    print(line[2:6],hex(ord(line[0])),file=fout)

