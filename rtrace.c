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

struct resp_pong
{
  struct sockaddr_in addr_in;
  int err;
  struct
  {
    struct iphdr m_iphdr;
    struct icmphdr m_icmphdr;
    struct icmp m_icmp;
  } m;
};

#define MTU 1472

void pong(int, struct resp_pong *);

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
  struct icmp m_icmp;
  m_icmp.icmp_type = ICMP_ECHO;
  m_icmp.icmp_code = ICMP_ECHOREPLY;
  m_icmp.icmp_cksum = 0;

  // icmp_hun.ih_idseq.icd_id is the identifier of
  // the ICMP datagram header
  m_icmp.icmp_hun.ih_idseq.icd_id = htons(1000);

  // icmp_seq icmp_hun.ih_idseq.icd_seq is the sequence
  // number field
  m_icmp.icmp_hun.ih_idseq.icd_seq = htons(seq);

  memset(&m_icmp.icmp_dun, 0, sizeof(m_icmp.icmp_dun));

  // icmp_dun.id_data , Can be set as the time to send the datagram

  // checksum
  m_icmp.icmp_cksum = in_cksum((unsigned short *)&m_icmp, sizeof(m_icmp));

  ssize_t send = sendto(sock,
                        &m_icmp,
                        sizeof(m_icmp),
                        0,
                        ai_addr,
                        sizeof(*ai_addr));

  if (send < 0)
  {
    perror("sendto");
    exit(1);
  }
}

struct resp_pong *create_r_pong()
{
  struct resp_pong *r_pong = malloc(sizeof(struct resp_pong));
  return r_pong;
}

void pong(int sock, struct resp_pong *r_pong)
{
  socklen_t addrlen = sizeof(r_pong->addr_in);
  r_pong->err = 0;

  ssize_t recv = recvfrom(sock,
                          &r_pong->m,
                          sizeof(r_pong->m),
                          0,
                          (struct sockaddr *)&r_pong->addr_in,
                          &addrlen);

  if (recv < 0)
  {
    r_pong->err = errno;
    //perror("recvfrom");
  }
}

int main(int argc, char *argv[])
{

  // todo: parameter to number of hops

  if (argc != 2)
  {
    fprintf(stderr, "rtrace <ip>\n");
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
  uint16_t hop = 0;

  struct resp_pong *r_pong = create_r_pong();

  char hostname[128];

  Elapsed();
  ElapsedStart();
  ping(ai->ai_addr, sock, seq);
  pong(sock, r_pong);
  ElapsedEnd();

  uint32_t target_address = ntohl(r_pong->addr_in.sin_addr.s_addr);

  getnameinfo((struct sockaddr *)&r_pong->addr_in,
              sizeof(r_pong->addr_in),
              hostname,
              sizeof(hostname), NULL, 0, 0);

  printf(fSIZEL " bytes from=" fSTRING
                " ttl=" fNUMD " time=" fMILS " err=%d\t\n",
         sizeof(r_pong->m),
         hostname,
         r_pong->m.m_iphdr.ttl,
         ELAPSED_MS,
         r_pong->err);

  struct time_stats *stats = init_time_stats();

  for (hop = 1; hop < 101; hop++)
  {
    // increase ttl
    setsockopt(sock, IPPROTO_IP, IP_TTL, &hop, sizeof(hop));
    ElapsedStart();
    ping(ai->ai_addr, sock, 0);
    pong(sock, r_pong);
    ElapsedEnd();

    double elapsed = ELAPSED_MS;

    getnameinfo((struct sockaddr *)&r_pong->addr_in,
                sizeof(r_pong->addr_in),
                hostname,
                sizeof(hostname), NULL, 0, 0);

    if (r_pong->err != EAGAIN)
    {

      printf(fNUMD " > " fSTRING " ttl=" fNUMD
                   " time=" fMILS " err=%d\t\n",
             hop,
             hostname,
             r_pong->m.m_iphdr.ttl,
             ELAPSED_MS,
             r_pong->err);
    }
    else
    {
      printf("\t" fNUMD " > " fSTRING
             " ttl=" fNUMD " time=" fMILS "\t\n",
             hop,
             hostname,
             r_pong->m.m_iphdr.ttl,
             ELAPSED_MS);
    }

    update_time_stats(elapsed, stats);

    if (target_address == ntohl(r_pong->addr_in.sin_addr.s_addr))
    {
      printf("Reached.\n");
      break;
    }
  }

  printf(fNUMD " packets - min=" fMILS2 " max=" fMILS2
               " avg=" fMILS2 " total time=" fMILS "\n",
         hop,
         stats->min,
         stats->max,
         stats->total / hop,
         stats->total);

  free(r_pong);
  return 0;
}