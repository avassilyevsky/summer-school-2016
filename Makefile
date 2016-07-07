
all: day2 day3

day2:
	cd Day2/user_space/threads;make
day3:
	cd Day3/user_space;make

clean:
	cd Day2/user_space/threads;make clean
	cd Day3/user_space/;make clean
