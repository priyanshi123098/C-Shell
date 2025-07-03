#include "headers.h"
#define BUFFER_SIZE 100000
void print_webpage(const char *html) 
{
    const char *start = html;
    const char *end = NULL;

    // Ensure 'html' is not NULL
    if (html == NULL) {
        fprintf(stderr, "Invalid HTML content.\n");
        return;
    }

    while ((start = strchr(start, '>')) != NULL)  
    {
        start++;  // Move past the '>'
        end = strchr(start, '<');  // Find the next '<'    
        
        if (end != NULL && start != end) 
        {
            // Print the content between '>' and '<' 
            printf("%.*s\n", (int)(end - start), start);  
        } 
        else if (end == NULL) 
        {
            break;
        }
        start = end;  
    }
    printf("\n");
}
// Function to fetch the webpage
void fetch_page(const char *host, const char *path) {
    int sock;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char request[1024], response[BUFFER_SIZE];
    int received, total_received = 0;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Get host by name
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr, "No such host\n");
        exit(1);
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(1);
    }

    // Send HTTP GET request
    snprintf(request, sizeof(request),
             "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, host);
    if (send(sock, request, strlen(request), 0) < 0) {
        perror("Send failed");
        close(sock);
        exit(1);
    }

    // Receive the response
    while ((received = recv(sock, response + total_received, BUFFER_SIZE - total_received - 1, 0)) > 0) {
        total_received += received;
        response[total_received] = '\0';  // Null-terminate the response
    }

    if (received < 0) {
        perror("Receive failed");
    }
    close(sock);
    print_webpage(response);
}
void iman (char full_comm[], char dir_of_file[])
{
    char * token= strtok (full_comm, " ");
    token= strtok (NULL, " ");
    char* comm= token;
    if (comm != NULL)
    {
        while (*comm == ' ')
            comm++;
    }
    int len = strlen(comm);
    while (len > 0 && comm[len-1]==' ') 
    {
        len--;
    }
    comm[len] = '\0';
    char path[4096];
    snprintf(path, sizeof(path), "/?topic=%s&section=all", comm);
    fetch_page("man.he.net", path);
}