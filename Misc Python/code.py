while True:
    val=input("What number should the sequence be started on?")
    try:
        test=int(val)
    except:
        print("That isn't a number")
        continue;
    break;
done=0
while not done:
    iterations=input("How many iterations should be calculated? ")
    times=0
    try:
        times=int(iterations)
    except:
        print("That is an invalid answer")
        continue;
    for i in range(times):
        new=0
        skip=0
        for j in range(len(val)):
            if skip:
                skip-=1
                continue;
            num=int(val[j])
            k=int(val[j])
            l=0
            while k==num:
                l+=1
                try:
                    k=int(val[j+l])
                except:
                    break;
            skip=l-1
            new*=100
            new+=(l*10)+num
        val=str(new)
    print(val)
    while True:
        choice=input("Do you want to continue? Y/N ")
        if choice=="N" or choice=="n":
            done=1
            break;
        elif choice=="Y" or choice=="y":
            break;
        print("That is an invalid answer")
