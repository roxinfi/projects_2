nmax = 4
xmax = 10

for i in range(1,nmax+1):
    for j in range(1,xmax+1):
        print("%10d"%j**i,end="")
    print()