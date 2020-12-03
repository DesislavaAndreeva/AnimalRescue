# AnimalRescue
Multithread IPC client-server application with POSIX threads

# General info

University evaluation project in system programming.

# Description

IPC(Inter-process communication) application that helps keeping track of the animals in Animal Rescue center located in Sofia.
The application should manage a list of information about all the animals and data for few events - arrival, adoption and 
castration of an animal. The application should allow statistical samplings.

# Details

### The aplication need to:

 * Ensure the maintenance of the database from the server;
 * Perform initial loading of data in a list from file at startup (animalrescue.txt);
 * Establish and maintain connection between multiple clients and the server via IPC (Inter-process communication);
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
