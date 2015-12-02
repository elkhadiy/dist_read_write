# Distributed Systems course Ensimag

Compile with :
```
mpicc --std=c99 <c file> -o <binary name>
```
Launch with :
```
mpirun -n <process number> -machinefile <hosts file> <binary name>
```
the hosts file contains the machine names where to run the mpi processes. For example :
```
localhost
pcserveur.ensimag.fr
ensipc375
ensipc376
```
