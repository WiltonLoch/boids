#pragma once
#include <SFML/Graphics.hpp>

class Boid{
	public:
		Boid(float x, float y, float direcao = 90.0f, float velocidade = 0.25f);
		~Boid();

		void setDirecao(float direcao){this->direcao = direcao;};
		float getDirecao(){return direcao;};

		void setVelocidade(float velocidade){this->velocidade = velocidade;};
		float getVelocidade(){return velocidade;};

		float getX(){return x;};
		void setX(float x){this->x = x;};
		float getY(){return y;};
		void setY(float y){this->y = y;};

		sf::ConvexShape& getForma();
		void mover();
		void girarEmDirecao(float direcao_destino, float passo);

	private:
		float direcao = 270.0f;
		float velocidade;
		float x, y;

		sf::ConvexShape forma;

};
