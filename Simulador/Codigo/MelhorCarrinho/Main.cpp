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
        void movimentaQuadrados(int quantLados);
        int colisao();
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

void Robo::movimentaQuadrados(int quantLados){
    
    for(int x = 1; x <= quantLados; x++){ // Correção: alterado para '<=' para rodar a quantidade exata de lados
        for(int y = 0; y < 4; y++){
            
            // Atualiza o estado de colisão na struct do robô
            this->dados.colidindo = this->colisao();
            
            if(!this->colisao()) {
                edubotLib->move(0.9);        
                edubotLib->sleepMilliseconds(500 * x);
        
                edubotLib->rotate(90);
                edubotLib->sleepMilliseconds(2000);
            } else {
                // Se colidir, para o robô por segurança
                edubotLib->neutral();
                edubotLib->sleepMilliseconds(2000);
                std::cout << "Colisao detectada!\n Movimento interrompido." << std::endl;
                return; // Sai da função
            }
        }
    }
}

int main(void){
	//instancia objeto dentro da main e nomeia;
	Robo roboAtual;
	//Tenta conectar Robo
	if(edubotLib->connect()){

		// shows edubot sensors
		int showSensorsTimes = 5;
		while (showSensorsTimes > 0) {
			
			edubotLib->sleepMilliseconds(500);

			// Colhe info dos Sonares - For para colher a info dos 7 sonares em um loop, e retorna a medida em metros;
			for (int i=0; i<7; i++) {
				std::cout << "Sonar " << i << ": " << edubotLib->getSonar(i) << "m, ";
			}
			std::cout << "\n";
			// Colhe info dos Bumpers - sensores ativados na batida - resposta booleana (true/false)
			for (int i=0; i<4; i++) {
				std::cout << "Bumpers" << i << ": " << (edubotLib->getBumper(i) == true? "true":"false") << ", ";
			}
			std::cout << "\n";
			//Colhe info dos Encoders
			std::cout << "Encoder lado esquerdo: " << edubotLib->getEncoderCountLeft() << ", ";
			std::cout << "Encoder lado direito: " << edubotLib->getEncoderCountRight() << ", ";
			std::cout << "dt(looptime): " << edubotLib->getEncoderCountDT() << ", ";
			std::cout << "\n";
			//Medida caluclada do eixo X,Y e do Theta para localização
			std::cout << "Cordenada X: " << edubotLib->getX() << ", ";
			std::cout << "Cordenada Y: " << edubotLib->getY() << ", ";
			std::cout << "Rotacao Theta: " << edubotLib->getTheta() << ", ";
			std::cout << "\n";

			//Colhe as informações corelacionadas a 3 celulas de materia
			for (int i=0; i<3; i++){
			std::cout << "Celula de Bateria " << i << ": "<< edubotLib->getBatteryCellVoltage(i) << "V, ";
			}
			std::cout << "\n";
			std::cout << "\n";

			// line break
			std::cout << std::endl;

			showSensorsTimes--;

		}
		//chamando metodo(função) - movimentaQuadrados vou colocar 4;
		roboAtual.movimentaQuadrados(4);
		
		//Disconecta Robo
		edubotLib->disconnect();
	}
	else{
		//mensagem de erro
		std::cout << "Nao foi possivel se conectar ao robo!" << std::endl;
	}

	return 0;
}
