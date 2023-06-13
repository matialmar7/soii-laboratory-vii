# About

This is the implementation of the [Free RTOS](./README.old.md) assignment from the Operative Systems II course of the Faculty of Exact, Physical and Natural Sciences from the National University of Cordoba.

# How to use it

## Requirements
    [*] Ubuntu 20.04 or docker for compiling
    [*] Build-essentials
    [*] Qemu

## Compiling the binaries
### Build
Use `docker build -t ubuntu -f Dockerfile .` to build the docker image

Run the docker using the `start_docker.sh <project_path>` in the docker folder and use ´make´ to compile the binaries
## Excecution

### On windows
```powershell
& 'C:\Program Files\qemu\qemu-system-arm.exe' -serial stdio -machine lm3s811evb -kernel 'gcc\TP7.axf'
```
### On linux
```bash
qemu-system-arm -serial stdio -machine lm3s811evb -kernel gcc/TP7.axf
```

# Development

## Tasks

Se crearon 4 tareas, cada una encargada de una funcion especifica.
```c
void xTaskTemperatureSensor( void *pvParameters );
void xTaskFilter( void *pvParameters );
void xTaskDisplay( void *pvParameters );
void xTaskTop( void *pvParameters );
```