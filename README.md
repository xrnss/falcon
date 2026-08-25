# Falcon
A system to remotely define and execute Java classes in a target JVM using JVMTI/JNI 

## Overview
<img width="565" height="606" alt="image" src="https://github.com/user-attachments/assets/f664407a-173b-4c9e-a2bf-596c6925d7ea" />


The Launcher will find the target process, and using the Injection Helper, it will call a native method that injects the Master DLL into the Target JVM. From here, a connection is established between the Launcher and Target JVM, creating a tunnel in which class bytes can be exchanged. The Master DLL uses JVMTI to force the Target JVM's Class Loader to define the classes desired, then initiates execution through a bootstrap class. 

## Use Cases
A system like this would would be most useful when analysing a JVM with DisableAttachMechanism flags enabled.
