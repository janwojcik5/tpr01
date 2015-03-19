#!/usr/bin/env python
from mpi4py import MPI
import time
comm = MPI.COMM_WORLD
rank = comm.Get_rank()

if rank == 0:
   	data = ('abc',123)
   	req = comm.isend(data, dest=1)
	while not req.Get_status():
		print("waiting to send")
		time.sleep(0.1)
	print("message sent")
	
elif rank == 1:
   	req = comm.irecv(None,0)
	while not req.Get_status():
		print("waiting for reception")
		time.sleep(0.1)
	data=req.wait()
   	print data
else:
   print "Expected only two nodes"
