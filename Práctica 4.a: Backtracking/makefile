CXX = g++
CXXFLAGS = -O2 -std=c++11 -g

# ************ Generación de ejecutable ************
continental: continental.cpp continental.hpp main.cpp
	$(CXX) $(CXXFLAGS) -o continental continental.cpp main.cpp

# ************ Limpieza ************
clean:
	-rm continental
