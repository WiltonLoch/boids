#include <vector>
#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>
#include "Boid.hpp"

#define QUANTIDADE_BOIDS 50
#define ANGULO_VISAO 180.0f
#define ALCANCE_VISAO 90.0f

void steeringBehaviour(std::vector<Boid> &boids, std::vector<std::vector<int>> &vizinhos);

void trocarLado(std::vector<Boid> &boids, sf::Vector2u tamanho_janela);

void atualizarVizinhos(std::vector<Boid> boids, std::vector<std::vector<int>> &vizinhos);
float calcularDistancia(float x1, float y1, float x2, float y2);
float calcularDistanciaBoids(Boid &a, Boid &b);
float anguloRelativo(float x1, float x2, float distancia);
bool dentroCampoVisao(Boid &a, Boid &b, float distancia = -1.0f);

int main()
{
	sf::RenderWindow janela(sf::VideoMode(600, 600), "Boids");

	std::vector<Boid> boids;
	for (int i = 0; i < QUANTIDADE_BOIDS; ++i)
		boids.push_back(Boid(190.0f + 10.0f * i, 200.0f, 270.0f + i));		

	std::vector<std::vector<int>> vizinhos(QUANTIDADE_BOIDS);

	while (janela.isOpen())
	{
		sf::Event event;
		while (janela.pollEvent(event))
		{
			    if (event.type == sf::Event::Closed)
				janela.close();
		}

		atualizarVizinhos(boids, vizinhos);
		steeringBehaviour(boids, vizinhos);
		
		janela.clear();
		for (int i = 0; i < boids.size(); ++i) {
			boids[i].mover();
			janela.draw(boids[i].getForma());
			trocarLado(boids, janela.getSize());
		}
		janela.display();
	}

	return 0;
}

void steeringBehaviour(std::vector<Boid> &boids, std::vector<std::vector<int>> &vizinhos){
	/* coesão */
	for (int i = 0; i < boids.size(); ++i) {
		if(!vizinhos.empty()){
			float media_x, media_y = 0;
			for (auto j : vizinhos[i]) {
				media_x += boids[j].getX();			
				media_y += boids[j].getY();			
			}
			media_x = media_x/vizinhos[i].size();
			media_y = media_y/vizinhos[i].size();
			float dst = calcularDistancia(boids[i].getX(), boids[i].getY(), media_x, media_y);

			boids[i].girarEmDirecao(anguloRelativo(boids[i].getX(), media_x, dst), 0.25f);
		}
	}	

	/* alinhamento */
	for (int i = 0; i < boids.size(); ++i) {
		if(!vizinhos.empty()){
			float media = 0;
			for (auto j : vizinhos[i]) {
				media += boids[j].getDirecao();			
			}
			media = media/vizinhos[i].size();
			boids[i].girarEmDirecao(media, 0.1f);
		}
	}	

	/* separacao */
	for (int i = 0; i < boids.size(); ++i) {
		for (auto j : vizinhos[i]){
			float dst = calcularDistanciaBoids(boids[i], boids[j]);
			if(dst < 45){
				boids[i].girarEmDirecao(anguloRelativo(boids[i].getX(), boids[j].getX(), dst) + 180, 9/dst);
			}						
		}	
	}
}

void atualizarVizinhos(std::vector<Boid> boids, std::vector<std::vector<int>> &vizinhos){
	for (int i = 0; i < boids.size(); ++i){
		vizinhos[i].clear();
		for (int j = 0; j < boids.size(); ++j) {
			float dst = calcularDistanciaBoids(boids[i], boids[j]);	
			if(i != j && dst < ALCANCE_VISAO && dentroCampoVisao(boids[i], boids[j], dst)){
				vizinhos[i].push_back(j);
			}
		}
	}
}

float calcularDistancia(float x1, float y1, float x2, float y2){
	float dst_x = x2 - x1;
	float dst_y = y2 - y1;

	dst_x *= dst_x;
	dst_y *= dst_y;

	return std::sqrt(dst_x + dst_y);
}

float calcularDistanciaBoids(Boid &a, Boid &b){
	return calcularDistancia(a.getX(), a.getY(), b.getX(), b.getY());
}

float inline anguloRelativo(float x1, float x2, float distancia){
	float cateto_adjacente = x2 - x1;
	return std::acos(cateto_adjacente/distancia) * 180/M_PI;
}

bool dentroCampoVisao(Boid &a, Boid &b, float distancia){
	if(distancia < 0) distancia = calcularDistanciaBoids(a, b);
	else if(distancia == 0) return true;

	float angulo = anguloRelativo(a.getX(), b.getX(), distancia);

	angulo -= a.getDirecao();
	if(angulo < 0) angulo += 360;
	else if(angulo > 360) angulo -= 360;

	if(angulo < ANGULO_VISAO || angulo > 360 - ANGULO_VISAO) return true;
	return false;
}

void trocarLado(std::vector<Boid> &boids, sf::Vector2u tamanho_janela){
	for (auto &i : boids) {
		if(i.getX() < 0)
			i.setX(i.getX() + tamanho_janela.x);
		else if(i.getX() > tamanho_janela.x)
			i.setX(i.getX() - tamanho_janela.x);

		if(i.getY() < 0)
			i.setY(i.getY() + tamanho_janela.y);
		else if(i.getY() > tamanho_janela.x)
			i.setY(i.getY() - tamanho_janela.y);

	}
}
