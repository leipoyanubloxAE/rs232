OUTPUTDIR=output

all:
	mkdir -p $(OUTPUTDIR)
	gcc demo_rx.c rs232.c utils/utils.c utils/dictionary.c utils/iniparser.c -Wall -Wextra -o2 -o $(OUTPUTDIR)/test_rx
	gcc demo_tx.c rs232.c utils/utils.c utils/dictionary.c utils/iniparser.c -Wall -Wextra -o2 -o $(OUTPUTDIR)/test_tx
	gcc demo_command.c rs232.c utils/utils.c utils/dictionary.c utils/iniparser.c -Wall -Wextra -o2 -o $(OUTPUTDIR)/test_command

clean:
	rm -rf $(OUTPUTDIR)
