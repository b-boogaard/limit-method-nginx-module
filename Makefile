.PHONY: all clean build

all : clean build

clean:
	rm -rf target

build:
	mkdir -p target
	cd target
	cmake ..