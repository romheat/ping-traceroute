#ifndef B_ICMP_H
#define B_ICMP_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>

#define M_HEADERS (sizeof(struct iphdr) +   \
                   sizeof(struct icmphdr) + \
                   sizeof(struct icmp))

struct resp_pong
{
  struct sockaddr_in addr_in;
  int err;
  struct
  {
    struct iphdr m_iphdr;        // 20
    struct icmphdr m_icmphdr;    // 8
    struct icmp m_icmp;          // 28
    char m_data[64 - M_HEADERS]; // 8
  } m;
};

void ping(struct sockaddr *ai_addr, int sock, uint16_t seq);
struct resp_pong *create_r_pong();
void pong(int sock, struct resp_pong *r_pong);

#endif
