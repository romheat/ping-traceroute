CCOPTS=-g -Wall -Wextra -std=gnu99
rping: rping.c b-time.c b-time.h b-print.c b-print.h
	$(CC) $(CCOPTS) -o $@ $(filter-out %.h,$^) 