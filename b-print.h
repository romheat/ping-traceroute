#ifndef B_PRINT_H
#define B_PRINT_H

#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET "\033[0m"
#define COLOR_CYAN "\033[36m"

#define fNUMBER(s) COLOR_BLUE s COLOR_RESET
#define fNUMD COLOR_BLUE "%d" COLOR_RESET
#define fSTRING COLOR_GREEN "%s" COLOR_RESET
#define fMILS COLOR_CYAN "%.4fms" COLOR_RESET
#define fMILS2 COLOR_CYAN "%.2fms" COLOR_RESET
#define fSIZEL COLOR_YELLOW "%lu" COLOR_RESET
#define fSIZE COLOR_YELLOW "%d" COLOR_RESET
#define fERR COLOR_RED "%d" COLOR_RESET

void print_iphdr(struct iphdr m_iphdr);
void print_icmphdr(struct icmphdr m_icmphdr);
void print_icmp(struct icmp m_icmp);

#endif
