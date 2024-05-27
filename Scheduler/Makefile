# makefile for scheduling program
#
# make rr_p - for round-robin scheduling
# make edf - for edf scheduling

CC=gcc
CFLAGS=-Wall
OUTDIR=bin/RoundRobinWithPriority

clean:
	rm -rf $(OUTDIR)/*.o
	rm -rf $(OUTDIR)/rr_p
	rm -rf edf

$(OUTDIR):
	mkdir -p $(OUTDIR)

rr_p: $(OUTDIR)/driver.o $(OUTDIR)/list.o $(OUTDIR)/schedule_rr_p.o
	$(CC) $(CFLAGS) -o $(OUTDIR)/rr_p $(OUTDIR)/driver.o $(OUTDIR)/schedule_rr_p.o $(OUTDIR)/list.o

edf: driver.o list.o CPU.o schedule_edf.o
	$(CC) $(CFLAGS) -o edf driver.o schedule_edf.o list.o CPU.o

$(OUTDIR)/driver.o: driver.c
	$(CC) $(CFLAGS) -c driver.c -o $(OUTDIR)/driver.o

$(OUTDIR)/schedule_rr_p.o: schedule_rr_p.c
	$(CC) $(CFLAGS) -c schedule_rr_p.c -o $(OUTDIR)/schedule_rr_p.o

$(OUTDIR)/schedule_edf.o: schedule_edf.c
	$(CC) $(CFLAGS) -c schedule_edf.c -o $(OUTDIR)/schedule_edf.o

$(OUTDIR)/list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c -o $(OUTDIR)/list.o

CPU.o: CPU.c CPU.h
	$(CC) $(CFLAGS) -c CPU.c