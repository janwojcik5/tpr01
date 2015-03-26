#!/usr/bin/env python
from mpi4py import MPI
from sys import argv
import time as t
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
def SUM(x, y):
	return x + y

if len(argv)>3 and argv[3]=='-v':
	verbose=True
else:
	verbose=False
if len(argv)>2:
	bytes_per_send=int(argv[2])
else:
	bytes_per_send=16
if len(argv)>1:
	number_of_sends=int(argv[1])
else:
	number_of_sends=25
indata = bytearray(bytes_per_send)
outdata = bytearray(bytes_per_send)
comm.Barrier()
time=MPI.Wtime()
for i in range(number_of_sends):
	x = rank
	for i in range(bytes_per_send):
		indata[i] = x % 256
		x = x / 256
	comm.Scan(indata, outdata, Op = SUM)
	if verbose:
		print "Scan executed"
time=time-MPI.Wtime()
if rank == 0:
	print str(comm.size)+" "+str(number_of_sends)+" "+str(time)