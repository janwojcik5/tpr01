#!/usr/bin/env python
from mpi4py import MPI
from sys import argv
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

#usage:
#token_ring.py [number_of_sends] [bytes_per_send] [number_of_nodes] [-v]
	
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
if rank==0:	
	for i in range(number_of_sends):
   		data = bytearray(bytes_per_send)
  		data = comm.recv(source=(rank-1)%size)
		if verbose:
			print "Process "+rank+" sent data"
		comm.send(data,dest=(rank+1)%size)
		if verbose:
			print "Process 0 received data"

else: 
        for i in range(number_of_sends):
                data = bytearray(bytes_per_send)
                comm.send(data, dest=(rank+1)%size)
                if verbose:
                        print "Process "+rank+" sent data"
                data = comm.recv(source=(rank-1)%size)
                if verbose:
                        print "Process 0 received data"

#substract the times
time=MPI.Wtime()-time
comm.Barrier()
#perform scaling
capacity=float(bytes_per_send*number_of_sends*8)/(float(1000000)*time)
#at this time 'time' is of unit Mbit/s
print str(capacity)+"Mbit/s capacity "
print str(time/float(number_of_sends))+"s of delay on single message"

