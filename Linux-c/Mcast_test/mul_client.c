#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>


#define MCAST_PORT 8888
#define MCAST_ADDR "224.0.0.88"
#define MCAST_DATA "BROADCAST TEST DATA"
#define MCAST_INTERVAL 5
int main(int argc, char*argv)
{
    int s;
    struct sockaddr_in mcast_addr;
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == -1)
    {
        perror("socket()");
        return -1;
    }

    memset(&mcast_addr, 0, sizeof(mcast_addr));
    mcast_addr.sin_family = AF_INET;
    mcast_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);
    mcast_addr.sin_port = htons(MCAST_PORT);

    //while(1) {
        int n = sendto(s,
                                    MCAST_DATA,
                                    sizeof(MCAST_DATA),
                                    0,
                                    (struct sockaddr*)&mcast_addr,
                                    sizeof(mcast_addr)) ;
        if( n < 0)
        {
            perror("sendto()");
            return -2;
        }

        //sleep(MCAST_INTERVAL); 
    //}

    return 0;
}
