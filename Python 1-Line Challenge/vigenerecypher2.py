for l in open('i.txt','r').read().lower().split('/')[2]: v=[v[0] if 'v' in vars() else open('i.txt','r').read().lower().split('/')[1],v[1] if 'v' in vars() else open('i.txt').read().split('/')[0]=='e',(v[2]+1)%len(v[0]) if 'v' in vars() else 1,open('o.txt','a').write(chr((((ord(l)-96)+(ord(v[0][v[2]])-96)*(1 if v[1] else -1))-1)%26+97) if 96<ord(l)<123 else l) if 'v' in vars() else open('o.txt','w').write(chr((((ord(l)-96)+(ord(open('i.txt','r').read().lower().split('/')[1][0])-96)*(1 if open('i.txt').read().split('/')[0]=='e' else -1))-1)%26 + 97) if 96<ord(l)<123 else l),open('i.txt','w').write('')]