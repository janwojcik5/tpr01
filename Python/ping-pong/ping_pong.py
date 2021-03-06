#!/usr/bin/env python
from mpi4py import MPI
from sys import argv
import time as t
comm = MPI.COMM_WORLD
rank = comm.Get_rank()

#usage:
#ping_pong.py [number_of_sends] [bytes_per_send] [-v]
	
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
time=MPI.Wtime()
if rank == 0:
	for i in range(number_of_sends):
   		data = bytearray(bytes_per_send)
  		comm.send(data, dest=1)
		if verbose:
			print "Process 0 sent data"
		data = comm.recv(source=1)
		if verbose:
			print "Process 0 received data"
elif rank == 1:
	for i in range(number_of_sends):
   		data = bytearray(bytes_per_send)
  		data=comm.recv(source=0)
		if verbose:
			print "Process 1 sent data"
		comm.send(data,dest=0)
		if verbose:
			print "Process 1 received data"
else:
	print "Expected two nodes"
#substract the times
time=MPI.Wtime()-time
comm.Barrier()
#perform scaling
capacity=float(bytes_per_send*number_of_sends*8)/(float(1000000)*time)
#at this time 'time' is of unit Mbit/s
print str(capacity)+"Mbit/s capacity "
print str(time/float(number_of_sends))+"s of delay on single message"
if rank==0:
	file=open('./output/results.txt','a')
	file.write(str(bytes_per_send)+' '+str(capacity)+' '+str(time/float(number_of_sends))+'\n')
#time=MPI.Wtime()
#t.sleep(1)
#time=MPI.Wtime()-time
#print time
