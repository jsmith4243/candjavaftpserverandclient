#include <errno.h>
//#include <error.h>

#include <stdio.h>

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
//#include <error.h>

#include <netdb.h>
#include <dirent.h>




#define filename "file1.txt"
#define PORT 7000


void communicate(int newsockfd, char* buffer, char* request, char* response, int n);
void communicateSendOnly(int newsockfd, char* buffer, char* request, char* response, int n);

int sendFile();

void sendDirectory();



void* thread_proc(void *arg);

int main(int argc, char *argv[])
{
    
    if (argc < 2)
    {

            printf("Please include the port number\n");

            exit(0);

    }

    else if (argc > 2)
    {

            printf("Usage: ./a.out <portnumber>\n");

            exit(0);

    }
    
    


    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n, pid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
    {

            perror("ERROR opening socket");
            exit(1);

    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);   

    printf("Waiting for client on port: %i\n", portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            perror("ERROR on binding");
            exit(1);
    }

    listen(sockfd, 5);


    clilen = sizeof(cli_addr);

    while(1)
    {

        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

        if (newsockfd < 0) {
          perror("ERROR on accept");
          exit(1);
        }

        pid = fork();

        if (pid < 0)
        {

            perror("ERROR on fork");

            exit(1);


        }

        if (pid == 0)
        {

            //client
            close(sockfd);

            printf("Client connected.\n");

            char request[256];
            char response[256];

            char receivedString[256];
            
            /*

            bzero(request, 256);
            bzero(response, 256);
            bzero(buffer, 256);

            strcpy(request, "ser1");
            communicate(newsockfd, buffer, request, response, n);
            
            bzero(request, 256);
            bzero(response, 256);
            bzero(buffer, 256);


            strcpy(request, "ser2");
            communicate(newsockfd, buffer, request, response, n);   
            
              
            */ 
                  


            
            int isQuit = 0;

            while(1)
            {
                bzero(request, 256);

                strcpy(request, "messagefromserver\n");

                //printf("inf loop 2 run\n");
                
                
                bzero(response, 256);
                bzero(buffer, 256);

                communicate(newsockfd, buffer, request, response, n);

                printf("Client: %s\n", response);

                strcpy(receivedString, response);




                //strcpy(receivedString, "onez, twoz, threez");



                char *pch;
                pch = strtok(receivedString, " ");
                while (pch != NULL)
                {

                    printf("token: %s\n", pch);

                    if (strcmp(pch, "quit") == 0)
                    {

                        printf("Quit command received.\n");

                        close(sockfd);
                        close(newsockfd);

                        isQuit = 1;

                        pch = strtok(NULL, " ");
                        
                       
                        
                        break;

                    }

                    else if (strcmp(pch, "-l") == 0)
                    {

                        sendDirectory();
                        
                        //printf("-l detected\n");
                        
                        break;


                    }

                    else if (strcmp(pch, "-g") == 0)
                    {

                        sendFile();
                        
                        break;


                    }                


                    pch = strtok(NULL, " ");






                }
                
                 

                if (isQuit == 1)
                {
                    //printf("FLAGXXXy<\n");
                    
                    
                    //exit(0);

                    break;

                }




            }
             
        //}                   


            exit(0);
        }

        else
        {


            close(newsockfd);
            
            //exit(0);

        }

    }

}

void communicate(int newsockfd, char* buffer, char* request, char* response, int n)
{
    
    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);

    if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }
    
    bzero(response, 256);
    
    strcpy(response, buffer);

    printf("Here is the message from client: %s\n", response);
    
    
	
	
    bzero(buffer, 256);
    
   
    
    
    strcpy(buffer, request);
    
    //printf("strlen is: %i\n", strlen(buffer));
    
    //n = write(newsockfd, buffer, 255);
    n = write(newsockfd, buffer, strlen(buffer));
    //n = write(newsockfd,"I got your message\n",20);
    
   
    if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }
    
    //printf("Number of bytes written is: %i\n", n);
    
    return;
    
    
    
}


void communicateSendOnly(int newsockfd, char* buffer, char* request, char* response, int n)
{

	
	
    bzero(buffer, 256);
    
   
    
    
    strcpy(buffer, request);
    
   // printf("strlen is: %i\n", strlen(buffer));
    
    //n = write(newsockfd, buffer, 255);
    n = write(newsockfd, buffer, strlen(buffer));
    //n = write(newsockfd,"I got your message\n",20);
    
   
    if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }
    
    //printf("Number of bytes written is: %i\n", n);
    
    return;
    
    
    
}

int sendFile()
{
    
    

    
    
    struct sockaddr_in sAddr;
    int sockfd, connfd;
    int status;
    pthread_t thread_id;
    int val;
    
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    val = 1;
    status = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (status < 0)
    {
        perror("Error - port");
        return 0;
    }
    
    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(PORT);
    sAddr.sin_addr.s_addr = INADDR_ANY;
    
    status = bind(sockfd, (struct sockaddr *) &sAddr, sizeof(sAddr));
    
    if (status < 0)
    {
        perror("Error - Bind");
        return 0;
    }
    
    status = listen(sockfd, 5);
    if (status < 0)
    {
        perror("Error - Listen");
        return 0;
    }
    
    while(1)
    {
        
        connfd = accept(sockfd, NULL, NULL);
        if (connfd < 0)
        {
            printf("Accept error on server\n");
            //error("ERROR on accept");
            return 0;
        }
        
        printf("client connected to child thread %i with pid %i.\n", pthread_self(), getpid());
        status = pthread_create(&thread_id, NULL, thread_proc, (void *) connfd);
        
        if (status != 0)
        {
            printf("Could not create thread.\n");
            return 0;
        }
        
        sched_yield();
    }
    pthread_join(thread_id, NULL);
    
    
}

void sendDirectory()
{



    
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
    {

        perror("ERROR opening socket");
        exit(1);

    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = 6000;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);   

    printf("Waiting for client on port: %i\n", portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            perror("ERROR on binding");
            exit(1);
    }
    
    

    listen(sockfd, 5);
    
    

    clilen = sizeof(cli_addr);

    
    //printf("FLAG1111\n");
            
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    
    if (newsockfd < 0) {
        perror("ERROR on accept");
        exit(1);
    }

    printf("Server connected for directory.\n");

    char request[256];
    char response[256];

    char receivedString[256];
    
    

    
    
    bzero(request, 256);
    bzero(response, 256);
    bzero(buffer, 256);
    
    
    
    strcpy(request, "The server's directory is: \n");
    communicate(newsockfd, buffer, request, response, n);

    
    DIR *dp;
    struct dirent *ep;

    dp = opendir ("./");
    if (dp != NULL)
    {
        while (ep = readdir (dp))
        {
            puts (ep->d_name);
            
            
            bzero(request, 256);
            bzero(response, 256);
            bzero(buffer, 256);
            strcpy(request, ep->d_name);
            strcat(request,"\n");
            strcat(request, "\0");
            
            communicateSendOnly(newsockfd, buffer, request, response, n);
             
            
            
        }
        
        
            
        (void) closedir (dp);
    }
    else
        perror ("Couldn't open the directory");
    
    
    
    bzero(request, 256);
    strcpy(request, "end\n");
    communicateSendOnly(newsockfd, buffer, request, response, n);
    
    
    //printf("AAAFLAGXXXYYYZZZ\n");
    close(newsockfd);
    close(sockfd);


    
}

void* thread_proc(void *arg)
{
    
    int connfd;
    int nread, n;
    char buffer[1000];
    FILE *fp;
    
    connfd = (int) arg;
    
    fp = fopen(filename, "ab");
    
    if (fp == NULL)
    {
        printf("File not found!\n");
        return NULL;
        
    }
    
    else
    {
        
        printf("Found file %s\n", filename);
    }
        
    while(( n = recv(connfd, buffer, sizeof buffer, 0)) > 0)
    {
        
        fwrite(buffer, sizeof(char), n, fp);
       // fprintf(stdout, "Received %d byte\n", n);
        
        
    }
    fclose(fp);
    close(connfd);
    printf("client disconnected from child thread %i with pid %i.\n", pthread_self(), getpid());
    
    return NULL;
   
   
}