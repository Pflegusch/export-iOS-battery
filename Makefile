CCFLAGSBASE_LINUX = -march=native -O3

all: linux

linux: battery.c Makefile
	gcc $(CCFLAGSBASE_LINUX) -o ssh_battery battery.c -lssh

docker:
	docker-compose up -d

clean:
	rm -rf ssh_battery
