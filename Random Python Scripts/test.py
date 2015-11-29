i=0
while True:
    if not ((1/26)**i)*((31556908800000*16)+1-i) >= 0.5:
        break;
    else:
        i+=1

print(str(i-1))