COMPILER = g++
CFLAGS = -Wall -std=c++17 -Iinclude -I/usr/local/include -L/usr/local/lib -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

tlon: 
	$(COMPILER) $(CFLAGS) -o tlon main.cpp

clean:
	rm -f tlon