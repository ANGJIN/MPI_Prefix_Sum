MPI_EXEC = 2_a_block 2_a_nonblock 2_b_block 2_b_nonblock 2_scan_block 2_scan_nonblock
SERIAL_EXEC = 2_serial

all : $(MPI_EXEC) $(SERIAL_EXEC)

2_a_block : 2_a_block.c
	mpicc $^ -o $@

2_a_nonblock : 2_a_nonblock.c
	mpicc $^ -o $@

2_b_block : 2_b_block.c
	mpicc $^ -o $@

2_b_nonblock : 2_b_nonblock.c
	mpicc $^ -o $@

2_scan_block : 2_scan_block.c
	mpicc $^ -o $@

2_scan_nonblock : 2_scan_nonblock.c
	mpicc $^ -o $@

2_serial : 2_serial.c
	gcc $^ -o $@

clean :
	rm $(MPI_EXEC) $(SERIAL_EXEC)
