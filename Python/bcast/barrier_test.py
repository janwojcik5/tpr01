#!/usr/bin/env python
from mpi4py import MPI
from sys import argv
import time as t
comm = MPI.COMM_WORLD
rank = comm.Get_rank()

if len(argv)>2 and argv[2]=='-v':
	verbose=True
else:
	verbose=False
if len(argv)>1:
	number_of_sends=int(argv[1])
else:
	number_of_sends=25
comm.Barrier()
time=MPI.Wtime()
for i in range(number_of_sends):
	comm.Barrier()
	if verbose:
		print "Processes synchronized"
time=time-MPI.Wtime()
if rank == 0:
	print str(comm.size)+" "+str(number_of_sends)+" "+str(time)