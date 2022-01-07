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

  char hostname[128];
  Elapsed();
  struct time_stats *stats = init_time_stats();
  struct resp_pong *r_pong = create_r_pong();

  for (; seq < 10; seq++)
  {
    ElapsedStart();

    ping(ai->ai_addr, sock, seq);
    pong(sock, r_pong);

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
  }

  printf(fNUMD " packets - min=" fMILS2 " max=" fMILS2
               " avg=" fMILS2 " total time=" fMILS "\n",
         seq,
         stats->min,
         stats->max,
         stats->total / seq,
         stats->total);

  free(r_pong);
  free(stats);
  freeaddrinfo(ai);
  return 0;
}