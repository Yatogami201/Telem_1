#ifndef SOCKETUTIL_SOCKETUTIL_H
#define SOCKETUTIL_SOCKETUTIL_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

int createTCPIpv4Socket() { 
    return socket(AF_INET, SOCK_STREAM, 0); }

struct sockaddr_in* createIpv4Address(char *ip, int port) {
   
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));
    address->sin_family = AF_INET; 
    address->sin_port = htons(port); 

    if(strlen(ip) == 0) 
        address->sin_addr.s_addr = INADDR_ANY; 
     else 
        inet_pton(AF_INET, ip, &address->sin_addr.s_addr);
        return address; 

    
 }
 
struct sockaddr_in* createIpv4Address(char *ip, int port);

int createTCPIpv4Socket();
pthread_mutex_t socketsMutex;

struct AcceptedSocket
{
    int acceptedSocketFD;
    struct sockaddr_in address;
    int error;
    bool acceptedSuccessfully;
};

struct AcceptedSocket * acceptIncomingConnection(int serverSocketFD);

void acceptNewConnectionAndListenAndprintData(int serverSocketFD);

void receiveAndPrintIncomingData(int socketFD);

void startAcceptingIncomingConnections(int serverSocketFD);

void receiveAndPrintIncomingDataOnSepparateThread(struct AcceptedSocket* pSocket);

void sendReceivedDataBack(char* buffer, int socketFD);

void removeClientSocket(int socketFD);

void writeToLog(const char* message);

struct AcceptedSocket acceptedSockets[10];
int acceptedSocketsCount = 0;

void startAcceptingIncomingConnections(int serverSocketFD) {

    while (true) {

        struct AcceptedSocket* clientSocket = acceptIncomingConnection(serverSocketFD);
        acceptedSockets[acceptedSocketsCount++] = *clientSocket;

        receiveAndPrintIncomingDataOnSepparateThread(clientSocket); 

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientSocket->address.sin_addr), clientIP, INET_ADDRSTRLEN);

        char logMessage[256];
        sprintf(logMessage, "Cliente conectado desde %s:%d (socketFD: %d)",
            clientIP, ntohs(clientSocket->address.sin_port), clientSocket->acceptedSocketFD);
        writeToLog(logMessage);
        
    }

}

void receiveAndPrintIncomingDataOnSepparateThread(struct AcceptedSocket* pSocket) {
    
    pthread_t threadID;
    pthread_create(&threadID, NULL, (void* (*)(void*))receiveAndPrintIncomingData, (void*)(intptr_t)pSocket->acceptedSocketFD);
    
}

void receiveAndPrintIncomingData(int socketFD) {
    
    char buffer[1024]; 

    while (true) {
        memset(buffer, 0, sizeof(buffer)); 
        ssize_t amountReceived = recv(socketFD, buffer, sizeof(buffer), 0);
    
        if (amountReceived > 0) {
            fwrite(buffer, 1, amountReceived, stdout);
            printf("\n");
            fflush(stdout);

            sendReceivedDataBack(buffer, socketFD);
        }
    
        if (amountReceived <= 0) {
            printf("Cliente desconectado (socketFD: %d)\n", socketFD);
            removeClientSocket(socketFD); 

            char logMessage[128];
            sprintf(logMessage, "Cliente desconectado (socketFD: %d)", socketFD);
            writeToLog(logMessage);

            break;
        }
    }

    close(socketFD);
}

void sendReceivedDataBack(char* buffer, int socketFD) {

    pthread_mutex_lock(&socketsMutex);
    for (int i = 0; i < acceptedSocketsCount; i++) {
        if (acceptedSockets[i].acceptedSocketFD != socketFD) {
            send(acceptedSockets[i].acceptedSocketFD, buffer, strlen(buffer), 0);
        }
    }
    pthread_mutex_unlock(&socketsMutex);
}

struct AcceptedSocket* acceptIncomingConnection(int serverSocketFD) {

    struct sockaddr_in clientAddress;
    int clientAddressSize = sizeof(struct sockaddr_in);
    int clientSocketFD = accept(serverSocketFD, (struct sockaddr*)&clientAddress, &clientAddressSize);
    
    struct AcceptedSocket* acceptedSocket = malloc(sizeof(struct AcceptedSocket));
    acceptedSocket->address = clientAddress;
    acceptedSocket->acceptedSocketFD = clientSocketFD;
    acceptedSocket->acceptedSuccessfully = clientSocketFD > 0;
    
    if (!acceptedSocket->acceptedSuccessfully)
        acceptedSocket->error = clientSocketFD;
    
    return acceptedSocket;
}

void removeClientSocket(int socketFD) {

    pthread_mutex_lock(&socketsMutex);
    for (int i = 0; i < acceptedSocketsCount; i++) {
        if (acceptedSockets[i].acceptedSocketFD == socketFD) {
            acceptedSockets[i] = acceptedSockets[acceptedSocketsCount - 1];
            acceptedSocketsCount--;
            break;
        }
    }
    pthread_mutex_unlock(&socketsMutex);
}

void writeToLog(const char* message) {

    FILE* logFile = fopen("log.txt", "a");
    if (logFile == NULL) {
        perror("Error abriendo log.txt");
        return;
    }

    time_t now = time(NULL);
    struct tm* t = localtime(&now);

    fprintf(logFile, "[%02d-%02d-%04d %02d:%02d:%02d] %s\n",
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
            t->tm_hour, t->tm_min, t->tm_sec,
            message);

    fclose(logFile);
}

int main() {

    if (pthread_mutex_init(&socketsMutex, NULL) != 0) {
        perror("Mutex initialization failed");
        return 1;
    }

    int serverSocketFD = createTCPIpv4Socket();
    struct sockaddr_in* serverAddress = createIpv4Address("", 2000);

    int result = bind(serverSocketFD, (struct sockaddr*)serverAddress, sizeof(*serverAddress));
    if (result == 0) 
        printf("Socket was bound successfully\n");

    listen(serverSocketFD, 10);
    
    startAcceptingIncomingConnections(serverSocketFD);

    pthread_mutex_destroy(&socketsMutex);

    shutdown(serverSocketFD, SHUT_RDWR);  

    return 0;
}
    
