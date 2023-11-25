

bin/satellite:
	mkdir -p bin
	g++ src/satellite/main.ino ./ext/*.cpp -o satellite


bin/atmega:
	mkdir -p bin
	g++ src/atmega/main.ino ./ext/*.cpp -o satellite
