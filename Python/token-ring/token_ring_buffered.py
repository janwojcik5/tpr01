#!/usr/bin/env python
from mpi4py import MPI
from sys import argv
#from numpy import empty
from array import array
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

#usage:
#ping_pong.py [number_of_sends] [bytes_per_send] [buffer_size] [-v]
	
if len(argv)>4 and argv[4]=='-v':
	verbose=True
else:
	verbose=False
if len(argv)>3:
	buffer_size=int(argv[3])
else:
	buffer_size=1000
if len(argv)>2:
	bytes_per_send=int(argv[2])
else:
	bytes_per_send=16
if len(argv)>1:
	number_of_sends=int(argv[1])
else:
	number_of_sends=25

buf=array('b',[0]*buffer_size)
MPI.Attach_buffer(buf)
time=MPI.Wtime()

clean_buffer_count=0
for i in range(number_of_sends):
        data = bytearray(bytes_per_send)
	comm.bsend(data,dest=(rank+1)%size)
	if i%(buffer_size/bytes_per_send)==0:
		MPI.Detach_buffer()
		MPI.Attach_buffer(buf)
  	comm.recv(source=(rank-1)%size)
	if verbose:
		print "Process 0 sent data"
	if verbose:
		print "Process 0 received data"

MPI.Detach_buffer()

#substract the times
time=MPI.Wtime()-time
comm.Barrier()
#perform scaling
capacity=float(bytes_per_send*number_of_sends*8)/(float(1000000)*time)
#at this time 'time' is of unit Mbit/s
print str(capacity)+"Mbit/s capacity "
print str(time/float(number_of_sends))+"s of delay on single message"


