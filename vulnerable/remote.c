#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// Remove all occurances of the given character `garbage` from the input string `str`.
//
// `str` The string to have a character removed.
// `garbage` The character to be removed from the string.
void removeChar(char *str, char garbage)
{
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

// The entry point to the vulnerable application. Requires 1 argument from the
// user which is the port number to listen on.
//
// `argc` The number of arguments passed from the user.
// `argv`[0] The name to be printed after Hello.
// return The exit status code of this application.
int main(int argc, char **argv)
{
    // Validate an argument has been passed.
    if (argc < 2)
    {
        printf("Usage: %s [port]\n", argv[0]);
        return 1;
    }

    // Socket create
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1)
    {
        printf("Failed to create socket...\n");
        exit(1);
    }

    struct sockaddr_in server, client;
    bzero(&server, sizeof(server));

    // Assign IP / PORT
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(atoi(argv[1]));

    // Bind socket
    if (bind(socketFd, (struct sockaddr*)&server, sizeof(server)) != 0)
    {
        printf("Failed to bind socket...\n");
        exit(2);
    }

    // Listen for traffic
    if (listen(socketFd, 5) != 0)
    {
        printf("Failed to listen...\n");
        exit(3);
    }

    // Listening for traffic
    printf("Listening for client connections...\n");

    int length = sizeof(client);

    // Accept the data packet from client
    int clientFd = accept(socketFd, (struct sockaddr *)&client, &length);
    if (clientFd < 0)
    {
        printf("Failed to accept connection from client...\n");
        exit(4);
    }

    printf("Accepted client connection!\n");

    // Allocate client buffer
    char buffer[32];

    // Wait for incoming data
    while (1)
    {
        bzero(buffer, sizeof(buffer));
  
        // Read the message from client and copy it in buffer
        int inputLength = read(clientFd, buffer, 0x100);

        if (inputLength != -1)
        {
            removeChar(buffer, '\x0A');

            // Print buffer which contains the client contents
            printf("Recieved: %s\n", buffer);

            // Allocate and build response
            char hello[6] = "Hello ";
            char* response = malloc(inputLength + 8);
            memcpy(response, hello, 6);
            memcpy(response + 6, buffer, inputLength);
            response[inputLength + 6] = '\0';

            // Send response to client
            printf("Sending: %s\n", response);

            // Append line break
            response[inputLength + 6] = '\x0A';
            response[inputLength + 7] = '\0';
            write(clientFd, response, inputLength + 7);

            // If message contains "exit" then server exit and chat ended.
            if (strncmp("exit", buffer, 4) == 0)
            {
                printf("Server exiting...\n");
                break;
            }
        }
    }

    // Close the socket
    close(clientFd);
    close(socketFd);

    return 0;
}
