roket_num = [1,2,3,4,5,6,7,8,9,10,11,12]
for number in roket_num:
    if number > 7:
        break
    if number % 2 == 0:
        print(f'{number} is even')
    else:
        continue