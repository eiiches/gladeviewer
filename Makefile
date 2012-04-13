all: gladeviewer

gladeviewer: gladeviewer.cpp
	$(CXX) -Wall -o $@ $< `pkg-config --libs --cflags gtkmm-3.0`
