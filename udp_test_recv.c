#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 1024
#define NPACK 10
#define PORT 20050

void diep( char* s )
{
    perror( s );
    exit( 1 );
}

int main(void)
{
    FILE * fp;
    fp = fopen ("udp_received.log", "w+");
    struct sockaddr_in si_me, si_other;
    int s, i, slen = sizeof( si_other );
    char buf[ BUFLEN ];
    ssize_t bytes_read;
    
    if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    {
         diep("socket");
    }

    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(bind(s, &si_me, sizeof(si_me))==-1)
    {
         diep("bind");
    }
    
    memset(buf, 0, BUFLEN);
    while(1)
    {
        bytes_read = recvfrom(s, buf, BUFLEN, 0, &si_other, &slen);
        if ( bytes_read == -1 ) 
        {
            diep("recvfrom()");
        }
        fprintf(fp,"Received packet from %s:%d\nData ASCII: %s\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);

        fprintf(fp,"Data hex: \n");
        for (i=0;i<bytes_read;i++)
        {
            fprintf(fp,"0x%02x ", buf[i]);
        }
        fprintf(fp,"\n");
        memset(buf, 0, BUFLEN);
        fflush(fp);
    }
    
    close(s);
    return 0;
}
