print('compiler running....')
while True:
    inp = input('>>>')
    arr_n =[]
    temp = ''
    for i in inp:
        if i in '0123456789':
            temp = temp + i
        elif i in '+-*/':
            arr_n.append(int(temp))
            arr_n.append(i)
            temp = ''
        else:
            continue
    while 
    stopper = input('ended...')
