from timeit import default_timer
import math



FILE_NAME="primes.txt"



primes=['2']

def to_time(duration):
    s=int(duration*100)/100
    m=(s-(s%60))/60
    s-=60*m
    h=(m-(m%60))/60
    m-=60*h
    print(str(int(h))+"h",str(int(m))+"m",str(s)+"s",end=" ")

#Read and add all prime numbers to list
print("Attempting to open log...")
while True:
    try:
        log_read=open(FILE_NAME,"r")
        print("Log opened\n")
        break;
    except:
        print("'"+FILE_NAME+"'","was not found")
        print("Creating","'"+FILE_NAME+"'")
        open(FILE_NAME,"a").close()
        print("Re-attempting to open log...")

start = default_timer()
print("Reading log...")
log_read.readline()
while True:
    line=log_read.readline()
    if not line:
        break;
    else:
        primes.append(line)
log_read.close()
duration = default_timer() - start
print("Done in",end=" ")
to_time(duration)
print("\n")

start = default_timer()
print("Converting numbers...")
count=0
last_percent=0
total=len(primes)
for prime in primes:
    length=len(prime)
    if prime[length-1:]=="\n":
        primes[count]=prime[:length-1]
    else:
        primes[count]=prime
    if count/total*100>=last_percent+1:
        print(str(int(count/total*100))+"%")
        last_percent=int(count/total*100)
    count+=1
duration = default_timer() - start
print("Done in",end=" ")
to_time(duration)
print("\n")

results=[]

for i in range(10000):
    i=2*i+1

    if(str(2*i+1) in primes and str(2*i-1) in primes):
        results.append("b")
    elif(str(2*i+1) in primes):
        results.append("p")
    elif(str(2*i-1) in primes):
        results.append("m")
    else:
        results.append("n")

    print(i)

klen=1
while True:
    key=results[:klen+1]

    for i in range(len(results)//klen):
        if not key==results[klen*i:klen*(i+1)+1]:
            break;
    else:
        break;

    print(klen)
    klen+=1

print(klen)
input()
print(key)
input()