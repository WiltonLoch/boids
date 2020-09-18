#include <cmath>

#include "Boid.hpp"

Boid::Boid(float x, float y, float direcao, float velocidade) : x(x), y(y), direcao(direcao), velocidade(velocidade){
	forma.setPointCount(3);

	forma.setPoint(0, sf::Vector2f(20, 5));
	forma.setPoint(1, sf::Vector2f(0, 0));
	forma.setPoint(2, sf::Vector2f(0, 10));

	forma.setOrigin(5, 10);
}

Boid::~Boid(){};

sf::ConvexShape& Boid::getForma(){
	forma.setRotation(-direcao);
	forma.setPosition(x, y);
	return forma;
}

void Boid::mover(){
	float direcao_radianos = direcao * (M_PI/180);
	x += std::cos(direcao_radianos) * velocidade;	
	y -= std::sin(direcao_radianos) * velocidade;	
}

void Boid::girarEmDirecao(float direcao_destino, float passo){
	if(direcao > 360) direcao -= 360;

	if(direcao_destino > direcao){
		if(direcao_destino - direcao <= 180)
			direcao += passo;
		else
			direcao -= passo;
	}else if(direcao_destino < direcao){
		if(direcao - direcao_destino <= 180)
			direcao -= passo;
		else
			direcao += passo;
	}

	if(direcao < 0) direcao += 360;
	else if(direcao > 360) direcao -= 360;
}
