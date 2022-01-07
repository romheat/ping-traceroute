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
#include "b-icmp.h"

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
  free(stats);
  freeaddrinfo(ai);

  return 0;
}