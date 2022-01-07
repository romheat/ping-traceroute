#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include "b-time.h"
#include "b-print.h"

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

struct resp_pong *pong(int);

// bsd checksum
unsigned short in_cksum(unsigned short *addr, int len)
{
  int nleft = len;
  int sum = 0;
  unsigned short *w = addr;
  unsigned short answer = 0;

  while (nleft > 1)
  {
    sum += *w++;
    nleft -= 2;
  }

  if (nleft == 1)
  {
    *(unsigned char *)(&answer) = *(unsigned char *)w;
    sum += answer;
  }

  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  answer = ~sum;
  return (answer);
}

void ping(struct sockaddr *ai_addr, int sock, uint16_t seq)
{
  // packet 64 bits as linux ping
  struct packet
  {
    struct icmp m_icmp;
    char m_data[64 - sizeof(struct icmp)];
  } m_packet;

  m_packet.m_icmp.icmp_type = ICMP_ECHO;
  m_packet.m_icmp.icmp_code = ICMP_ECHOREPLY;
  m_packet.m_icmp.icmp_cksum = 0;

  // icmp_hun.ih_idseq.icd_id is the identifier of
  // the ICMP datagram header
  m_packet.m_icmp.icmp_hun.ih_idseq.icd_id = htons(1000);

  // icmp_seq icmp_hun.ih_idseq.icd_seq is the sequence
  // number field
  m_packet.m_icmp.icmp_hun.ih_idseq.icd_seq = htons(seq);

  memset(&m_packet.m_icmp.icmp_dun,
         0,
         sizeof(m_packet.m_icmp.icmp_dun));

  // icmp_dun.id_data , Can be set as the time to send the datagram

  // checksum (full packet)
  m_packet.m_icmp.icmp_cksum = in_cksum((unsigned short *)&m_packet,
                                        sizeof(m_packet));

  ssize_t send = sendto(sock,
                        &m_packet,
                        sizeof(m_packet),
                        0,
                        ai_addr,
                        sizeof(*ai_addr));

  if (send < 0)
  {
    perror("sendto");
    exit(1);
  }
}

struct resp_pong *pong(int sock)
{

  struct resp_pong *r_pong = malloc(sizeof(struct resp_pong));
  socklen_t addrlen = sizeof(r_pong->addr_in);

  ssize_t recv = recvfrom(sock,
                          &r_pong->m,
                          sizeof(r_pong->m),
                          0,
                          (struct sockaddr *)&r_pong->addr_in,
                          &addrlen);

  if (recv < 0)
  {
    r_pong->err = errno;
    return r_pong;
  }
  else
  {
    return r_pong;
  }
}

int main(int argc, char *argv[])
{

  if (argc != 2)
  {
    fprintf(stderr, "rping <ip>\n");
    exit(1);
  }

  char *host = argv[1];

  struct addrinfo *ai;
  if (getaddrinfo(host, NULL, NULL, &ai) < 0)
  {
    perror("getaddrinfo");
    exit(1);
  }

  int sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);

  // set socket timeout
  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

  if (sock < 0)
  {
    perror("socket");
    exit(1);
  }

  uint16_t seq = 0;
  struct resp_pong *r_pong;
  char hostname[128];
  Elapsed();
  struct time_stats *stats = init_time_stats();

  for (; seq < 10; seq++)
  {
    ElapsedStart();

    ping(ai->ai_addr, sock, seq);
    r_pong = pong(sock);

    ElapsedEnd();

    update_time_stats(ELAPSED_MS, stats);

    getnameinfo((struct sockaddr *)&r_pong->addr_in,
                sizeof(r_pong->addr_in),
                hostname,
                sizeof(hostname), NULL, 0, 0);

    printf(fSIZEL " bytes from=" fSTRING " icmp_seq=" fNUMD
                  " ttl=" fNUMD " time=" fMILS " err=%d\t\n",
           sizeof(r_pong->m),
           hostname,
           seq,
           r_pong->m.m_iphdr.ttl,
           stats->last_value,
           r_pong->err);

    // todo: add option to show this
    // print_iphdr(r_pong->m.m_iphdr);

    if (r_pong->err != 0)
    {
      print_icmphdr(r_pong->m.m_icmphdr);
    }

    free(r_pong);
  }

  printf(fNUMD " packets - min=" fMILS2 " max=" fMILS2
               " avg=" fMILS2 " total time=" fMILS "\n",
         seq,
         stats->min,
         stats->max,
         stats->total / seq,
         stats->total);

  free(stats);
  return 0;
}