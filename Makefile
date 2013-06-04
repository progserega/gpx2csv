gpx2csv.bin: gpx2csv.o
	gcc gpx2csv.o -o gpx2csv.bin -lxml2

gpx2csv.o: gpx2csv.c
	gcc -O3 -c -I/usr/include/libxml2 gpx2csv.c -o gpx2csv.o
	#gcc -g3 -DDEBUG -c -I/usr/include/libxml2 gpx2csv.c -o gpx2csv.o

clean:
	rm gpx2csv.bin gpx2csv.o
