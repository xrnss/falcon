# Falcon
A system to remotely define and execute Java classes in a target JVM using JVMTI/JNI.

## Overview
<img width="565" height="606" alt="image" src="https://github.com/user-attachments/assets/f664407a-173b-4c9e-a2bf-596c6925d7ea" />

<br>

See [protocol.md](protocol.md) for the socket packet protocol.

## Prerequisites
- MSVC 2022 Build Tools
- Java 8
- Desired IDEs (e.g. IntelliJ IDEA, CLion)
- Knowledge of target process (window titles, relevant libraries e.g. `LWJGL` or `GLFW3` for Minecraft)

## Tutorial
1. Build the DLLs in [Helper](/Helper) & [Loader](/Loader). Recommended with MSVC 2022 Build Tools.
2. Place them within the resources folder of the [Launcher](/Launcher) project, under the names of `FalconHelper.dll` & `FalconLoader.dll`.
3. Place your desired classes to be loaded alongside `club/rnss/bootstrap`, updating the bootstrap method in the `Bootstrap` class to call an entry point into your code. Everything under `club/rnss/bootstrap`, unless modified, will be compiled and used as the classes inserted by Loader.  
4. Compile the [Launcher](/Launcher) project and execute with the target process open and fully loaded, with the generated `Bootstrap.jar` in the same running directory as `Launcher.jar`.   

## Use Cases
A system like this would would be most useful when analysing a JVM with DisableAttachMechanism flags enabled. If configured correctly (finding the correct ClassLoader), injected classes can interact directly with already loaded classes in the JVM (e.g. net/minecraft). This can be used for modding, debugging, analysis etc. 

## Missing features
This project is a WIP. TBC is listed below.
- [ ] Resource loading into the JVM (fonts, images etc. for use)
- [ ] Native registering of bootstrap class (getters and setters for class bytes, to allow ASM + hooking etc.)

## Notes
Due to the nature of RDI (Reflective DLL Injection), this project may be flagged by antiviruses, resulting in false positives. RDI is used to load the DLL directly into the JVM's memory space without writing to the disk or using Windows' standard `LoadLibrary`. It handles the DLL's relocation and initialisation in memory.
