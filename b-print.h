#ifndef B_PRINT_H
#define B_PRINT_H

#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET "\033[0m"
#define NUMBER(s) COLOR_BLUE s COLOR_RESET 


void print_iphdr(struct iphdr m_iphdr);
void print_icmphdr(struct icmphdr m_icmphdr);
void print_icmp(struct icmp m_icmp);

#endif
