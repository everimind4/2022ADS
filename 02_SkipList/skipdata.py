import random
import sys

random.seed(3022)

def skipdata(N, fname):
    fname= fname + ".txt"
    outfile = open( fname, "w")
    for (i,w) in enumerate(range(N)):
        p = random.random( )
        if p <= 0.1 : #Query case
            rank = random.randrange(1,int(i*0.4))
            print(f"? {rank}", file=outfile )
        else :
            myval = random.randrange(1,N)
            print(f"+ {myval}", file=outfile )

    print(f"$", file=outfile )
    print(f"Done {N} lines at FILE { fname} ")
    outfile.close( )


if len(sys.argv) != 3 :
    print("잘못된 argument: Usage: >> python this.py N outfile")


N    = int( sys.argv[1 ])
fname= sys.argv[2]

skipdata(N, fname )
