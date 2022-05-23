import queue as Q

q = Q.PriorityQueue()

q.put((-45, 'A'))
q.put((-23, 'B'))
q.put((-8, 'C'))
q.put((-21, 'D'))
q.put((-7, 'E'))
q.put((-1, 'F'))
q.put((-31, 'G'))
q.put((-55, 'H'))

while not q.empty():
    This = q.get()
    if This[0] < -1:
        print(f'{This[1]} {-This[0]:2} → {This[1]} {int(-This[0]/2):2}')
        q.put((int(This[0]/2), This[1]))
    else:
        print(f'{This[1]} {-This[0]:2}')