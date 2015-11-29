total=0
for i in range(10):
    num=""
    num+=str(i)
    for i in range(10):
        if not i==int(num[0]):
            num1=num+str(i)
            for i in range(10):
                if not i==int(num1[0]) and not i==int(num1[1]):
                    num2=num1+str(i)
                    for i in range(10):
                        if not i==int(num2[0]) and not i==int(num2[1]) and not i==int(num2[2]):
                            num3=num2+str(i)
                            for i in range(10):
                                if not i==int(num3[0]) and not i==int(num3[1]) and not i==int(num3[2]) and not i==int(num3[3]):
                                    num4=num3+str(i)
                                    for i in range(10):
                                        if not i==int(num4[0]) and not i==int(num4[1]) and not i==int(num4[2]) and not i==int(num4[3]) and not i==int(num4[4]):
                                            num5=num4+str(i)
                                            for i in range(10):
                                                if not i==int(num5[0]) and not i==int(num5[1] and not i==int(num5[2]) and not i==int(num5[3]) and not i==int(num5[4]) and not i==int(num5[5])):
                                                    num6=num5+str(i)
                                                    for i in range(10):
                                                        if not i==int(num6[0]) and not i==int(num6[1]) and not i==int(num6[2]) and not i==int(num6[3]) and not i==int(num6[4]) and not i==int(num6[5]) and not i==int(num6[6]):
                                                            num7=num6+str(i)
                                                            for i in range(10):
                                                                if not i==int(num7[0]) and not i==int(num7[1]) and not i==int(num7[2]) and not i==int(num7[3]) and not i==int(num7[4]) and not i==int(num7[5]) and not i==int(num7[6]) and not i==int(num7[7]):
                                                                    num8=num7+str(i)
                                                                    for i in range(10):
                                                                        if not i==int(num8[0]) and not i==int(num8[1]) and not i==int(num8[2]) and not i==int(num8[3]) and not i==int(num8[4]) and not i==int(num8[5]) and not i==int(num8[6]) and not i==int(num8[7]) and not i==int(num8[8]):
                                                                            num9=num8+str(i)
                                                                            for i in range(10):
                                                                                if not i==int(num9[0]) and not i==int(num9[1]) and not i==int(num9[2]) and not i==int(num9[3]) and not i==int(num9[4]) and not i==int(num9[5]) and not i==int(num9[6]) and not i==int(num9[7]) and not i==int(num9[8]) and not i==int(num9[9]):
                                                                                    num0=num9+str(i)
                                                                                    if int(num0[1:4])%2==0:
                                                                                        if int(num0[2:5])%3==0:
                                                                                            if int(num0[3:6])%5==0:
                                                                                                if int(num0[4:7])%7==0:
                                                                                                    if int(num0[5:8])%11==0:
                                                                                                        if int(num0[6:9])%13==0:
                                                                                                            if int(num0[7:])%17==0:
                                                                                                                total+=int(num0)
print(total)
