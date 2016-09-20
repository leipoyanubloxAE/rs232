
all:
	gcc demo_rx.c rs232.c -Wall -Wextra -o2 -o test_rx
	gcc demo_tx.c rs232.c -Wall -Wextra -o2 -o test_tx

clean:
	rm -rf test_rx
	rm -rf test_tx
