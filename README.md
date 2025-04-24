# Proyecto 1 - Telemática: Aplicación de Chat

## Ejecución
Para descartar problemas de ejecución, utilice los archivos dentro de la carpeta Vbox Code, dentro estarán los main.c del servidor y el cliente preparado para conectarse a AWS.
# Video: https://youtu.be/QTTMTcs-n1c

## Introducción

En este proyecto, se desarrolló un sistema de comunicación basado en **sockets de Berkeley** utilizando el lenguaje de programación **C**. El objetivo principal fue construir una arquitectura cliente-servidor capaz de permitir la comunicación en tiempo real entre múltiples clientes.

El desarrollo de esta aplicación permite comprender de forma práctica el funcionamiento de las conexiones en red, el manejo de sockets, y los principios fundamentales del modelo OSI en la capa de transporte.

## Colaboradores

- Samuel Valencia  
- Carlos Arturo Díaz  
- Lorena Goez  

## Objetivos

- Desarrollar una aplicación de chat en lenguaje C utilizando sockets de Berkeley con arquitectura cliente-servidor y capacidad de comunicación simultánea entre múltiples usuarios.
- Implementar un servidor TCP/IP que acepte múltiples conexiones entrantes y distribuya los mensajes entre los clientes conectados.
- Diseñar un cliente que se conecte al servidor, envíe mensajes y escuche respuestas en tiempo real.

## Desarrollo

La aplicación fue desarrollada en lenguaje **C** y está compuesta por tres módulos principales: `SocketClient`, `SocketServer` y `SocketUtil`, cada uno con responsabilidades claramente definidas para implementar un sistema de chat cliente-servidor sobre **sockets TCP IPv4**.

La arquitectura se diseñó bajo el principio de **concurrencia**, permitiendo que múltiples clientes se conecten y se comuniquen de forma simultánea.

### Estructura del proyecto

- **SocketUtil**  
  Este módulo contiene funciones auxiliares para la creación de sockets y direcciones IPv4 (`createTCPIpv4Socket`, `createIpv4Address`). Su objetivo es abstraer la configuración de bajo nivel de los sockets y facilitar su reutilización en cliente y servidor.

- **SocketServer**  
  El servidor escucha conexiones en el **puerto 2000**, acepta nuevos clientes y redirige los mensajes entrantes hacia los demás clientes conectados, emulando un sistema de chat grupal. Utiliza **hilos (`pthreads`)** para manejar múltiples conexiones de forma simultánea. Además, implementa un sistema de **logging** en un archivo `log.txt` para registrar conexiones y desconexiones.

- **SocketClient**  
  El cliente se conecta al servidor mediante sockets TCP apuntando a `3.148.191.108` (localhost). Solicita al usuario su nombre y permite enviar mensajes que serán distribuidos al resto de los participantes conectados. Cada cliente también inicia un hilo para escuchar mensajes entrantes mientras escribe, asegurando comunicación en tiempo real.

### Comunicación cliente-servidor

1. El servidor acepta conexiones y crea un hilo para escuchar los mensajes de cada cliente.
2. Cada cliente que envía un mensaje lo hace al servidor.
3. El servidor reenvía el mensaje a todos los demás clientes conectados.
4. Los clientes muestran los mensajes en consola, simulando una conversación grupal en tiempo real.

---

## Resultados

Durante la ejecución del sistema, se logró establecer una **comunicación fluida entre múltiples clientes simultáneamente**. Cada mensaje enviado fue recibido correctamente por los demás clientes conectados, confirmando el correcto funcionamiento de la arquitectura diseñada.

---

## Conclusiones

- Este proyecto permitió afianzar conocimientos clave sobre redes, uso de sockets, y programación concurrente en C.
- La utilización de **sockets de Berkeley** y **multithreading** fue fundamental para lograr una arquitectura escalable.
- Se evidenció la importancia de sincronizar recursos compartidos y de mantener registros de eventos mediante logging.
- Como mejoras futuras se sugiere implementar:
  - Una interfaz gráfica amigable para el cliente.
  - Cifrado de mensajes y autenticación.
  - Un sistema más robusto de manejo de errores y reconexión automática.

---

## Referencias
- [Berkeley sockets - Wikipedia](https://en.wikipedia.org/wiki/Berkeley_sockets)

---
