#!/usr/bin/env python
from mpi4py import MPI
comm = MPI.COMM_WORLD
rank = comm.Get_rank()

if rank == 0:
   	data = ('abc',123)
	buffer=bytearray(150)
	MPI.Attach_buffer(buffer)
   	comm.bsend(data, dest=1)
elif rank == 1:
   data = comm.recv(source=0)
   print data
else:
   print "Expected only two nodes"
