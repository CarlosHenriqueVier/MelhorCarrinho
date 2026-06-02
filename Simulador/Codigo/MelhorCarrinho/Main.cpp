/*
 * PROTOTIPO PARA O SIMULADOR
 * @Author: Carlos Henrique Vier
*/

#include <iostream>
#include "libs/EdubotLib.hpp"
#define QT_SONARES 3
#define QT_BUMPERS 4
#define QT_BATERIA 3

EdubotLib *edubotLib = new EdubotLib();

const float distanciaMinima = 0.2;

class Robo {
    public:
        struct posicaoRobo {
            float x;
            float y;
            float theta;
        }posicao;

        struct infoDados {
        	  int bumpers[QT_BUMPERS];
        	  int colidindo;
            float sonares[QT_SONARES];
            float encoderDireito;
            float encoderEsquerdo;
            float celulasBateria[QT_BATERIA];
        }dados;

        void movimentacao();
        int colisao();
        void colheSonar();
        void andaRobo();
};

int Robo::colisao(){
    for(int x = 0; x < QT_BUMPERS; x++){
        // Correção: mudado de 'i' para 'x' para bater com a variável do loop
        this->dados.bumpers[x] = edubotLib->getBumper(x);
        
        if(this->dados.bumpers[x] == true){
            return true; // Retorna verdadeiro se qualquer bumper bateu
        }
    }
    return false; // Correção: Faltava um retorno padrão caso não colida com nada
}

void Robo::colheSonar(void){
		
}

void Robo::andaRobo(){
            // Atualiza o estado de colisão na struct do robô
            this->dados.colidindo = this->colisao();
            
            if(!this->colisao()) {
            	 do{
	               edubotLib->move(0.2);        
	               edubotLib->sleepMilliseconds(50);
	               std::cout << "Sonar " << 3 << ": " << edubotLib->getSonar(3) << "m, ";
	               for (int i=2; i<=4; i++) {
					this->dados.sonares[i] = edubotLib->getSonar(i);
					std::cout << "Sonar " << i << ": " << edubotLib->getSonar(i) << "m, ";
		}
            	 }while(edubotLib->getSonar(3)>distanciaMinima);
            } else {
                // Se colidir, para o robô por segurança
                edubotLib->neutral();
                std::cout << "Colisao detectada!\n Movimento interrompido." << std::endl;
                edubotLib->sleepMilliseconds(2000);
                return; // Sai da função
            }
            edubotLib->neutral();
        }

int main(void){
	//instancia objeto dentro da main e nomeia;
	Robo roboAtual;
	//Tenta conectar Robo
	if(edubotLib->connect()){
		//chamando metodo(função) - movimentaQuadrados vou colocar 4;	
		roboAtual.andaRobo();
		//Disconecta Robo
		edubotLib->disconnect();
	}
	else{
		//mensagem de erro
		std::cout << "Nao foi possivel se conectar ao robo!" << std::endl;
	}

	return 0;
}
