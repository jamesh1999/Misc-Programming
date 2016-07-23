import math
import random

def miller_rabin(n, k):
    s=0
    d = n-1
    while d%2 == 0:
        d = d//2
        s += 1

    if s:
        for test in range(0,k):
            r = random.randrange(1,n)
            y = pow(r, d, n)
            prime = False
            if (y == 1):
                prime = True
            for i in range(0,s):
                if (y == n-1):
                    prime = True
                    break;
                else:
                    y = (y*y)%n
            if not prime:
                return False
        return True
    else:
        return False

while True:
    while True:
        try:
            a=int(input("What number do you want to start at? (e1000) "))*10**10000
            break;
        except:
            pass
    while True:
        try:
            k=int(input("What accuracy do you want to test at?"))
            break;
        except:
            pass

    n=a
    while True:
        if miller_rabin(n,k):
            break;
        else:
            n+=1

    #Save any prime numbers
    log_write=open("output.txt","w")
    log_write.write(str(n)+"\n")
    log_write.close()
    input("Press enter to continue")