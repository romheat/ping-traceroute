CCOPTS=-g -Wall -Wextra -std=gnu99
rping: rping.c b-time.c b-time.h b-print.c b-print.h b-icmp.c b-icmp.h
	$(CC) $(CCOPTS) -o $@ $(filter-out %.h,$^) 

rtrace: rtrace.c b-time.c b-time.h b-print.c b-print.h b-icmp.c b-icmp.h
	$(CC) $(CCOPTS) -o $@ $(filter-out %.h,$^) 