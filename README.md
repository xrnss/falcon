# Falcon
A system to remotely define and execute Java classes in a target JVM using JVMTI/JNI.

## Overview
<img width="565" height="606" alt="image" src="https://github.com/user-attachments/assets/f664407a-173b-4c9e-a2bf-596c6925d7ea" />


The Launcher will find the target process, and using the Injection Helper, it will call a native method that injects the Master DLL into the Target JVM. From here, a connection is established between the Launcher and Target JVM, creating a tunnel in which class bytes can be exchanged. The Master DLL uses JVMTI to force the Target JVM's Class Loader to define the classes desired, then initiates execution through a bootstrap class. 

<br>

See [protocol.md](protocol.md) for the socket packet protocol.

## Use Cases
A system like this would would be most useful when analysing a JVM with DisableAttachMechanism flags enabled. If configured correctly (finding the correct ClassLoader), injected classes can interact directly with already loaded classes in the JVM (e.g. net/minecraft). 

## WIP
This project is a WIP. Completed and to be completed is listed below.
- [x] Launcher & Helper loading mechanism + target finder 
- [x] Launcher GUI 
- [x] Launcher socket server
- [ ] Launcher socket server protocol implementation
- [x] Helper implementation
- [x] Loader socket client
- [ ] Loader socket client protocol implementation
- [ ] Loader find launch class loader
- [ ] Loader define classes via JVMTI (part of protocol implementation)

## Notes
Due to the nature of RDI (Reflective DLL Injection), this project may be flagged by antiviruses, resulting in false positives. RDI is used to load the DLL directly into the JVM's memory space without writing to the disk or using Windows' standard `LoadLibrary`. It handles the DLL's relocation and initialisation in memory.
