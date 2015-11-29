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

n=int(open("output.txt","r").readline())
print("Read!")
den=1
while True:
    if miller_rabin(n,1):
        print("Success")
        den*=4
        print("Odds: 1/"+str(den))
    else:
        print("Failed!")
        break;