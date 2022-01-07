#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include "b-print.h"
#include "b-icmp-strings.h"

void print_iphdr(struct iphdr m_iphdr)
{
  printf("\thdr IP   Version " fNUMBER("%d") " Bytes " fSIZE " Service %d Total %d Protocol %d TTL %d\n",
         m_iphdr.version,
         ((unsigned int)(m_iphdr.ihl)) * 4,
         m_iphdr.tos,
         ntohs(m_iphdr.tot_len),
         m_iphdr.protocol,
         m_iphdr.ttl);
}

void print_icmphdr(struct icmphdr m_icmphdr)
{
  printf("\thdr ICMP Type %2d (%s)\n\tCode %2d (%s)\n\tId %d Seq %d\n",
         m_icmphdr.type,
         s_icmp_types[m_icmphdr.type],
         m_icmphdr.code,
         m_icmphdr.type == ICMP_DEST_UNREACH ? s_icmp_unreach_codes[m_icmphdr.code] : "",
         m_icmphdr.un.echo.id,
         m_icmphdr.un.echo.sequence);
}

void print_icmp(struct icmp m_icmp)
{
  printf("\tdat ICMP Type %2d Code %2d Id %d Seq %d\n",
         m_icmp.icmp_type,
         m_icmp.icmp_code,
         m_icmp.icmp_hun.ih_idseq.icd_id,
         m_icmp.icmp_hun.ih_idseq.icd_seq);
}
