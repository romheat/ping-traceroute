# Sample Ping & Traceroute 

Follow me on TWITCH - https://www.twitch.tv/romheat LIVE CODE

Live session code

## rping

Very basic ping command using icmp - sudo required for SOCK_RAW 

```s
sudo ./rping <address/host>

sudo ./rping 8.8.8.8
56 bytes from=dns.google icmp_seq=0 ttl=116 time=11.2748ms err=0	
56 bytes from=dns.google icmp_seq=1 ttl=116 time=16.3218ms err=0	
56 bytes from=dns.google icmp_seq=2 ttl=116 time=16.3549ms err=0	
56 bytes from=dns.google icmp_seq=3 ttl=116 time=11.0381ms err=0	
56 bytes from=dns.google icmp_seq=4 ttl=116 time=24.8177ms err=0	
56 bytes from=dns.google icmp_seq=5 ttl=116 time=31.4521ms err=0	
56 bytes from=dns.google icmp_seq=6 ttl=116 time=19.1428ms err=0	
56 bytes from=dns.google icmp_seq=7 ttl=116 time=13.2115ms err=0	
56 bytes from=dns.google icmp_seq=8 ttl=116 time=15.9211ms err=0	
56 bytes from=dns.google icmp_seq=9 ttl=116 time=11.6467ms err=0	
10 packets - min=11.04ms max=31.45ms avg=17.12ms total time=171.1815ms

```

## rtrace

make rtrace

```s
 sudo ./rtrace 8.8.8.8
56 bytes from=dns.google ttl=116 time=13.0929ms err=0	
1 > 192.168.1.1 ttl=64 time=2.8600ms err=0	
	2 > 192.168.1.1 ttl=64 time=25.6133ms	
3 > 169.red-5-205-17.dynamicip.rima-tde.net ttl=62 time=12.1514ms err=0	
	4 > 169.red-5-205-17.dynamicip.rima-tde.net ttl=62 time=2.1201ms	
	5 > 169.red-5-205-17.dynamicip.rima-tde.net ttl=62 time=9.2344ms	
	6 > 169.red-5-205-17.dynamicip.rima-tde.net ttl=62 time=8.7174ms	
7 > 72.14.211.154 ttl=54 time=13.0626ms err=0	
8 > 172.253.50.43 ttl=246 time=19.3043ms err=0	
9 > 142.250.46.167 ttl=244 time=13.7126ms err=0	
10 > dns.google ttl=116 time=19.2220ms err=0	
Reached.
10 packets - min=2.12ms max=25.61ms avg=12.60ms total time=125.9981ms
```

### Maintainers

[romheat](https://github.com/romheat).

You can follow me on twitter: [![Twitter](http://i.imgur.com/wWzX9uB.png)@romheat](https://www.twitter.com/romheat)

and Twitch -> https://www.twitch.tv/romheat