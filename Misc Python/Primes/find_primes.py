from timeit import default_timer
import math,threading



FILE_NAME="primes.txt"
NUM_THREADS=4096



primes=['2']



def to_time(duration):
    s=int(duration*100)/100
    m=(s-(s%60))/60
    s-=60*m
    h=(m-(m%60))/60
    m-=60*h
    print(str(int(h))+"h",str(int(m))+"m",str(s)+"s",end=" ")



class Worker(threading.Thread):
    def __init__(self,start,stop):
        self.a=start
        self.b=stop
        threading.Thread.__init__(self)

    def run(self):
        global primes
        for i in range((self.b-self.a)+1):
            num=i+self.a
            for j in test:
                if num%j==0:
                    break;
            else:
                primes.append(str(num))



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



# old_primes=primes[:]
# primes=[]
# for p in old_primes:
#     try:
#         a=int(p)
#         primes.append(p)
#     except:
#         pass



#Ask user for input and calculate primes
while True:
    start=int(primes[len(primes)-1])
    chosen=False
    while not chosen:
        try:
            iterations=int(input("How many numbers should be tested?"))
            root=math.sqrt(start+iterations)
            if root>start:
                print("That number is too high. Performing maximum possible.")
                iterations=start**2-start
            chosen=True
        except:
            print("That is not an integer")
    start_time = default_timer()
    test=[]
    for p in primes:
        if int(p)<=root:
            test.append(int(p))
        else:
            break;

    #Creating threads
    threads=[]
    blocksize=int((iterations-(iterations%(NUM_THREADS-1)))/(NUM_THREADS-1))
    print("Blocksize: "+str(blocksize))
    for i in range(NUM_THREADS):
        threads.append(Worker(start+(i*blocksize),start+((i+1)*blocksize)-1))

    #Starting threads
    for i in threads:
        i.start()

    #Checking to see if threads are finished
    for i in threads:
        i.join()

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
for prime in primes:
    log_write.write(prime+"\n")
duration = default_timer() - start
print("Done in",end=" ")
to_time(duration)
print("\n")
log_write.close()
print(FILE_NAME,"closed\n")
input("Press enter to exit")
