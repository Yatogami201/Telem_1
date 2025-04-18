#include "../SocketUtil/socketutil.h"

void startListeningAndPrintDataOnThread(int socketFD);
void listenAndPrint(int socketFD);


int main() {

   int socketFD = createTCPIpv4Socket();
   struct sockaddr_in *address = createIpv4Address("127.0.0.1", 2000); 
  
   int result = connect(socketFD, (struct sockaddr*)address, sizeof(*address));

   if (result == 0)  
      printf ("Connection was successful\n");

   char *name = NULL;
   size_t nameSize = 0;   
   printf("Please enter your name\n");
   ssize_t nameCount = getline(&name, &nameSize, stdin);
   name[nameCount - 1] = '\0'; 
    
   char *line = NULL;
   size_t lineSize = 0;   
   printf("Type and we will send(type exit)...\n");
   
   startListeningAndPrintDataOnThread(socketFD);

   char buffer[1024];

   while(true){

      
      ssize_t charCount = getline(&line, &lineSize, stdin);
      line[charCount - 1] = '\0';

      sprintf(buffer, "%s:%s", name, line);

      if (charCount > 0) {

         if(strcmp(line, "exit") == 0) 
            break;

         ssize_t amountWasSent = send(socketFD, buffer, strlen(buffer), 0);
         }
      }

   close(socketFD);      

   return 0;
}

void startListeningAndPrintDataOnThread(int socketFD) {

   pthread_t threadID;
   pthread_create(&threadID, NULL, listenAndPrint, socketFD);
 
} 

void listenAndPrint(int socketFD) {
   
   char buffer[1024]; 

   while (true) {
       memset(buffer, 0, sizeof(buffer)); 
       ssize_t amountReceived = recv(socketFD, buffer, sizeof(buffer), 0);
   
       if (amountReceived > 0) {
           fwrite(buffer, 1, amountReceived, stdout);
           printf("\n");
           fflush(stdout);

       }
   
       if (amountReceived <= 0)
           break;
   }

   close(socketFD);

}