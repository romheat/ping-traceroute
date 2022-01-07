#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#include "b-icmp.h"

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

  bzero(&m_packet, sizeof(m_packet));

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