from timeit import default_timer
from math import *



FILE_NAME="primes_atkin.txt"
NUM_THREADS=4096



prlist=[]



def to_time(duration):
    s=int(duration*100)/100
    m=(s-(s%60))/60
    s-=60*m
    h=(m-(m%60))/60
    m-=60*h
    print(str(int(h))+"h",str(int(m))+"m",str(s)+"s",end=" ")

def atkins13(limit=1000000):
    '''use sieve of atkins to find primes <= limit.'''
    primes = [0] * limit
 
    # n = 3x^2 + y^2 section
    xx3 = 3
    for dxx in range( 0, 12*int( sqrt( ( limit - 1 ) / 3 ) ), 24 ):
        xx3 += dxx
        y_limit = int(12*sqrt(limit - xx3) - 36)
        n = xx3 + 16
        for dn in range( -12, y_limit + 1, 72 ):
            n += dn
            primes[n] = not primes[n]
 
        n = xx3 + 4
        for dn in range( 12, y_limit + 1, 72 ):
            n += dn
            primes[n] = not primes[n]
 
    # n = 4x^2 + y^2 section
    xx4 = 0
    for dxx4 in range( 4, 8*int(sqrt((limit - 1 )/4)) + 4, 8 ):
        xx4 += dxx4
        n = xx4+1
 
        if xx4%3:
            for dn in range( 0, 4*int( sqrt( limit - xx4 ) ) - 3, 8 ):
                n += dn
                primes[n] = not primes[n]
 
        else:
            y_limit = 12 * int( sqrt( limit - xx4 ) ) - 36
 
            n = xx4 + 25
            for dn in range( -24, y_limit + 1, 72 ):
                n += dn
                primes[n] = not primes[n]
 
            n = xx4+1
            for dn in range( 24, y_limit + 1, 72 ):
                n += dn
                primes[n] = not primes[n]
 
    # n = 3x^2 - y^2 section
    xx = 1
    for x in range( 3, int( sqrt( limit / 2 ) ) + 1, 2 ):
        xx += 4*x - 4
        n = 3*xx
 
        if n > limit:
            min_y = ((int(sqrt(n - limit))>>2)<<2)
            yy = min_y*min_y
            n -= yy
            s = 4*min_y + 4
 
        else:
            s = 4
 
        for dn in range( s, 4*x, 8 ):
            n -= dn
            if n <= limit and n%12 == 11:
                    primes[n] = not primes[n]
 
    xx = 0
    for x in range( 2, int( sqrt( limit / 2 ) ) + 1, 2 ):
        xx += 4*x - 4
        n = 3*xx
 
        if n > limit:
            min_y = ((int(sqrt(n - limit))>>2)<<2)-1
            yy = min_y*min_y
            n -= yy
            s = 4*min_y + 4
 
        else:
            n -= 1
            s = 0
 
        for dn in range( s, 4*x, 8 ):
            n -= dn
            if n <= limit and n%12 == 11:
                    primes[n] = not primes[n]
 
    # eliminate squares        
    for n in range(5,int(sqrt(limit))+1,2):
        if primes[n]:
            for k in range(n*n, limit, n*n):
                primes[k] = False
 
    return [2,3] + list(filter(primes.__getitem__, range(5,limit,2)))



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
        prlist.append(line)
log_read.close()
duration = default_timer() - start
print("Done in",end=" ")
to_time(duration)
print("\n")

start = default_timer()
print("Converting numbers...")
count=0
last_percent=0
total=len(prlist)
for prime in prlist:
    length=len(prime)
    if prime[length-1:]=="\n":
        prlist[count]=prime[:length-1]
    else:
        prlist[count]=prime
    if count/total*100>=last_percent+1:
        print(str(int(count/total*100))+"%")
        last_percent=int(count/total*100)
    count+=1
duration = default_timer() - start
print("Done in",end=" ")
to_time(duration)
print("\n")



# old_primes=prlist[:]
# prlist=[]
# for p in old_primes:
#     try:
#         a=int(p)
#         prlist.append(p)
#     except:
#         pass



#Ask user for input and calculate primes
while True:
    chosen=False
    while not chosen:
        try:
            iterations=int(input("How many numbers should be tested?"))
            chosen=True
        except:
            print("That is not an integer")
    start_time = default_timer()
    prlist=atkins13(limit=iterations)
    duration = default_timer() - start_time
    print("Done in",end=" ")
    to_time(duration)
    print("\n")
    chosen=False
    while not chosen:
        choice=input("Do you want to continue? Y/N ")
        if choice=="Y" or choice=="y":
            chosen=True
            break;
        elif choice=="N" or choice=="n":
            chosen=True
            break;
        print("That is not a valid answer")
    if choice=="N" or choice=="n":
        break;



#Save any prime numbers
start = default_timer()
print("\nSaving primes to '"+FILE_NAME+"'")
log_write=open(FILE_NAME,"w")
for prime in prlist:
    log_write.write(str(prime)+"\n")
duration = default_timer() - start
print("Done in",end=" ")
to_time(duration)
print("\n")
log_write.close()
print(FILE_NAME,"closed\n")
input("Press enter to exit")
