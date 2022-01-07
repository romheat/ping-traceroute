/* strings copied from 
 * https://www.iana.org/assignments/icmp-parameters/icmp-parameters.xhtml 
 */
 
const char *s_icmp_types[] = {
  "Echo Reply",
  "Unassigned",
  "Unassigned",
  "Destination Unreachable",
  "Source Quench (Deprecated)",
  "Redirect",
  "Alternate Host Address (Deprecated)",
  "Unassigned",
  "Echo",
  "Router Advertisement",
  "Router Solicitation",
  "Time Exceeded",
  "Parameter Problem",
  "Timestamp",
  "Timestamp Reply",
  "Information Request (Deprecated)",
  "Information Reply (Deprecated)",
  "Address Mask Request (Deprecated)",
  "Address Mask Reply (Deprecated)",
  "Reserved (for Security)",
  "Reserved (for Robustness Experiment)",
  "Traceroute (Deprecated)",
  "Datagram Conversion Error (Deprecated)",
  "Mobile Host Redirect (Deprecated)",
  "IPv6 Where-Are-You (Deprecated)",
  "IPv6 I-Am-Here (Deprecated)",
  "Mobile Registration Request (Deprecated)",
  "Mobile Registration Reply (Deprecated)",
  "Domain Name Request (Deprecated)",
  "Domain Name Reply (Deprecated)",
  "SKIP (Deprecated)",
  "Photuris",
  "ICMP messages utilized by experimental mobility protocols such as Seamoby",
  "Extended Echo Request",
  "Extended Echo Reply",
  "Unassigned",
  "RFC3692-style Experiment 1",
  "RFC3692-style Experiment 2",
  "Reserved"
};

const char *s_icmp_unreach_codes[] = {
  "Network Unreachable",
  "Host Unreachable",
  "Protocol Unreachable",
  "Port Unreachable",
  "Fragmentation Needed and DF Set",
  "Source Route Failed",
  "Destination Network Unknown",
  "Destination Host Unknown",
  "Source Host Isolated",
  "Network Administratively Prohibited",
  "Host Administratively Prohibited",
  "Network Unreachable for TOS",
  "Host Unreachable for TOS",
  "Communication Administratively Prohibited",
  "Host Precedence Violation",
  "Precedence cutoff in effect"
};


