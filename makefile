main.exe: muon.o detector.o
	g++ muon.o detector.o main.cpp -o main.exe

muon.o: muon.cpp
	g++ -c muon.cpp

detector.o: detector.cpp
	g++ -c detector.cpp

clean:
	rm *.o main.exe
