all:
	g++ -Wall -std=c++17 -O3 TimeCode.cpp TimeCodeTests.cpp -o tct
	g++ -Wall -std=c++17 -O3 TimeCode.cpp NasaLaunchAnalysis.cpp -o nasa
	g++ -Wall -std=c++17 -O3 TimeCode.cpp PaintDryTimer.cpp -o pdt

clean:
	rm -f tct nasa pdt