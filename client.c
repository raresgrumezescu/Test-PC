#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>  
#include <unistd.h>     
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define MAXLEN 1500


/**
 * Citeste maxim maxlen octeti de pe socket-ul sockfd in
 * buffer-ul vptr. Intoarce numarul de octeti cititi.
 */
ssize_t read_line(int sockd, char *vptr, size_t maxlen)
{
    ssize_t n, rc;
    char c, *buffer;

    buffer = vptr;

    for (n = 1; n < maxlen; n++)
    {
        if ((rc = read(sockd, &c, 1)) == 1)
        {
            *buffer++ = c;

            if (c == '\n')
            {
                break;
            }
        }
        else if (rc == 0)
        {
            if (n == 1)
            {
                return 0;
            }
            else
            {
                break;
            }
        }
        else
        {
            if (errno == EINTR)
            {
                continue;
            }

            return -1;
        }
    }

    *buffer = 0;
    return n;
}

/**
 * Trimite o comanda SMTP si asteapta raspuns de la server. Comanda
 * trebuie sa fie in buffer-ul sendbuf. Sirul expected contine
 * inceputul raspunsului pe care trebuie sa-l trimita serverul
 * in caz de succes (de exemplu, codul 250). Daca raspunsul
 * semnaleaza o eroare, se iese din program.
 */
void send_command(int sockfd, const char sendbuf[], char *expected)
{
    char recvbuf[MAXLEN];
    int nbytes;
    char CRLF[2] = {13, 10};

    printf("Trimit: %s\n", sendbuf);
    write(sockfd, sendbuf, strlen(sendbuf));
    write(sockfd, CRLF, sizeof(CRLF));

    nbytes = read_line(sockfd, recvbuf, MAXLEN - 1);
    recvbuf[nbytes] = 0;
    printf("Am primit: %s", recvbuf);

    if (strstr(recvbuf, expected) != recvbuf)
    {
        printf("Am primit mesaj de eroare de la server!\n");
        exit(-1);
    }
}

int main(int argc, char **argv)
{
	char *buffer = (char*) malloc(1024 * sizeof(char));
	int sockfd;
	
	while (1) {
		memset(buffer, 0, 1024);
		fgets(buffer, 1024, stdin);
		strcat (buffer, "\n");
		if (strstr (buffer, "send") != NULL) {
			char *ptr1 = strstr(buffer, " ");
			ptr1++;
			//printf ("%s\n", ptr1);
			char *ptr2 = strstr(ptr1, " ");
			ptr2++;
			char *ptr3 = strstr(ptr2, " ");
			ptr3++;
			
			char *client = strtok(ptr1, " ");
			char *dest = strtok (ptr2, " ");
			char *mail = strtok (ptr3, "\n");
			//printf ("%s\n\n%s\n\n%s\n\n", client, dest, mail);
			if (strstr (client, "@") == NULL || strstr (client, ".") == NULL || strstr (dest, "@") == NULL || strstr (dest, ".") == NULL) {
				perror ("Eroare la formatul e-mail-ului");
				exit(-1);
			}
		
    			int port = atoi(argv[3]);
    			int ret;
    			struct sockaddr_in servaddr;
    			char server_ip[INET_ADDRSTRLEN];
    			char sendbuf[MAXLEN];
    			char recvbuf[MAXLEN];

    			if (argc != 4)
    			{
        			printf("Utilizare: ./client id_client ip_server port\n");
        			exit(-1);
    			}



    			strncpy(server_ip, argv[2], INET_ADDRSTRLEN);

    			// TODO 1: creati socket-ul TCP client
    			sockfd = socket(AF_INET, SOCK_STREAM, 0);

    			// TODO 2: completati structura sockaddr_in cu
    			// portul si adresa IP ale serverului SMTP
    			servaddr.sin_family = AF_INET;
    			servaddr.sin_port = htons(port);
    			ret = inet_aton(argv[2], &servaddr.sin_addr);

    			if (ret == 0)
    			{
    				perror("INET_ATON\n");
    			}

    			// TODO 3: conectati-va la server
    			ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    			if (ret < 0)
    	    			perror("Connect\n");

    			// se primeste mesajul de conectare de la server
    			read_line(sockfd, recvbuf, MAXLEN - 1);
    			printf("Am primit: %s\n", recvbuf);

    			// se trimite comanda de HELO
    			char *helocmd = (char*) malloc (200 * sizeof (char));
    			strcpy (helocmd, "HELO ");
    			strcat (helocmd, argv[1]);
    			sprintf(sendbuf, helocmd);
    			send_command(sockfd, sendbuf, "250");

    			// TODO 4: trimiteti comanda de MAIL FROM
    			char *fromcmd = (char*) malloc (200 * sizeof (char));
    			strcpy (fromcmd, "MAIL FROM: ");
    			strcat (fromcmd, client);
    			
    			sprintf(sendbuf, fromcmd);
    			send_command(sockfd, sendbuf, "250");

    			// TODO 5: trimiteti comanda de RCPT TO
    			char *tocmd = (char*) malloc (200 * sizeof (char));
    			strcpy (tocmd, "RCPT TO: ");
    			strcat (tocmd, dest);
    			sprintf(sendbuf, tocmd);
    			send_command(sockfd, sendbuf, "250");

    			// TODO 6: trimiteti comanda de DATA
    			sprintf(sendbuf, "DATA");
    			send_command(sockfd, sendbuf, "354");

    			char *mailFinal = (char*) malloc(3000 * sizeof (char));
    			//strcpy (mailFinal, mail);
    			//strcat (mailFrom, "\r\n");
    			
    			char *from = (char*) malloc (100 * sizeof (char));
    			strcpy (from, "From: ");
    			strcat (from, client);
    			strcat (from, "\r\n");
    			
    			char *to = (char*) malloc (100 * sizeof (char));
    			strcpy (to, "To: ");
    			strcat (to, dest);
    			strcat (to, "\r\n");
    			
    			strcpy (mailFinal, "MIME-Version: 1.0\r\n");
    			strcat (mailFinal, from);
    			strcat (mailFinal, to);
    			strcat (mailFinal, "Subject: Test\r\nContent-Type: multipart/mixed; boundary=abc\r\n\r\n--abc\r\nContent-Type: text/plain\r\n")
;    			strcat (mailFinal, mail);
    			strcat (mailFinal, "\r\n");
    			strcat(mailFinal, "--abc\r\nContent-Type: text/plain\r\nContent-Disposition: attachment; filename=\citire_de_la_tastatura.txt\"\r\n.");
                     
    			send_command(sockfd, mailFinal, "250");
    			// TODO 8: trimiteti comanda de QUIT
    			sprintf(sendbuf, "QUIT");
    			send_command(sockfd, mailFinal, "221");
			
			
			
		}
		
		if (strcmp (buffer, "quit") == 0) {
			close(sockfd);
			break;
		}
	}
	return 0;
}
