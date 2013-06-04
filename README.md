Convert gpx to csv with additional info for csv2osm.

make
./gpx2csv.bin file_type.gpx > out.csv

type is line,station or substation.
It is need for additional info in result csv. 
If need only csv - this info may be ignored. If csv will be
convert to osm (csv2osm) this info tell csv2osm add lines or
poly from points of input csv.


