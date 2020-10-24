# AnimalRescue
Multithread IPC client-server application with POSIX threads

# General info

University evaluation project in system programming.

# Description

Animal Rescue Sofia needs help reporting its animals. Often new ones come, the old ones 
are adopted and between these two events - they get castrated. The application should manage
a list of all animals containing data for these three events and allow statistical samplings.

# Details

### The aplication need to:

 * Ensure the maintenance of animal data from the server;
 * Perform initial loading of data in a list from file at startup (animalrescue.txt);
 * Establish and maintain connection between the client (multiple clients) and the server via IPC (Inter-process communication);
 * Ensure synchronization in the transmission of messages;
 
# Compile
 
### Server
```
cd /server/src 
make clean 
make
```
 
### Client
```
cd /client/src 
make clean 
make
```
 
 # Run 
 
 ### First start the server
 
 ./animalrescueserver
 
 ### Client/s
 
  ./animalrescueclient
