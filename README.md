# AnimalRescue
Multithread IPC client-server application with POSIX threads

# General info

University evaluation project in system programming.

# Description

IPC application that helps keeping track of animals in Animal Rescue center located in Sofia.
The application should manage a list of all animals containing data for few events - arrival, 
adoption and castration of an animal and should allow statistical samplings.

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
