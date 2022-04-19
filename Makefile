all:
	g++ Boid.cpp main.cpp -o boids -lsfml-graphics -lsfml-window -lsfml-system
clean:
	rm boids
