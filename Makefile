CXX = g++
CXXFLAGS = 
TARGET = code
SRC = main.cpp src/Coordinates.cpp src/Missions.cpp src/Pilot.cpp

all:
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS)
run: all
	./$(TARGET)
clean:
	rm -f $(TARGET)
