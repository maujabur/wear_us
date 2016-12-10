// Arduino IDE 1.6.11
// Placa UNO
// 2016/Dez/12
//
// Wear-us http://www.wearus.co/
// iniciativa de LILO.Zone
//
// Código por
// Mau Jabur
// Rodrigo Rezende
// Luis Leão
//
// licença (CC BY-SA 4.0)
// Atribuição-CompartilhaIgual 4.0 Internacional
// https://creativecommons.org/licenses/by-sa/4.0/deed.pt_BR

// biblioteca para tirar o ruído dos botões
#include <Bounce2.h>
Bounce botao1 = Bounce();
const int tempo1 = 200; // tempo para filtrar ruido
const int entrada1 = A0; // para o botao têxtil

// definições para a bomba e válvula
const int bomba1 = 11;
const int valv1 = 10;

const int ABRE = LOW;
const int FECHA = HIGH;

const int LIGA = HIGH;
const int DESL = LOW;

// variavel  para lembrar o estado a bomba
int estado1;

// nomes dos estados
const int ESTAVEL = 0;
const int ENCHE = 1;
const int ESVAZIA = 2;
const int ESVAZIA_LENTO = 3; // ???

void setup() {
  // configura o botao e o filtro de ruido
  pinMode(entrada1, INPUT_PULLUP);
  botao1.attach(entrada1);
  botao1.interval(tempo1);


  // configura as saídas da bomba e da válvula
  pinMode(bomba1, OUTPUT);
  pinMode(valv1, OUTPUT);
  // inicializa bomba e válvula
  esvazia();

  // porta serial para debug
  Serial.begin(9600);
  Serial.println("Wear-Us");
  Serial.println("");
  Serial.println("[*] estavel");
  Serial.println("[+] enche");
  Serial.println("[-] esvazia");
  Serial.println("[/] esvazia lento");

}

void loop() {

  // lê o botão
  botao1.update();

  // Se o botão foi apertado
  if ( botao1.fell() ) {
    switch (estado1) { // avalia caso a caso

      case ESVAZIA:
        enche();
        break;

      case ENCHE:
        estavel();
        break;

      case ESTAVEL:
        esvazia();
        break;
    } // fim do switch
  } // fim do if
  //comandos pela serial

  // verifica comandos pela serial
  debug_serial();
}

void debug_serial() {
  // se houver uma letra enviada, lê e faz eco na Serial
  if (Serial.available() > 0 ) {
    char letra = Serial.read();
    Serial.print(letra);

    // para cada letra
    switch (letra) {
      case '*':
        estavel();
        break;

      case '+':
        enche();
        break;

      case '-':
        esvazia();
        break;

      case '/':
        esvazia_lento();
        break;
    } // fim do switch
  } // fim do if
} // fim da função

void estavel() {
  estado1 = ESTAVEL;
  digitalWrite (bomba1, DESL);
  digitalWrite (valv1, FECHA);
  Serial.println(" estavel");
}

void enche() {
  estado1 =  ENCHE;
  Serial.println(" enche");
  digitalWrite (bomba1, LIGA);
  digitalWrite (valv1, FECHA);
}

void esvazia() {
  estado1 =  ESVAZIA;
  Serial.println(" esvazia");
  digitalWrite (bomba1, DESL);
  digitalWrite (valv1, ABRE);
}

void esvazia_lento() {
  estado1 =  ESVAZIA_LENTO;
  Serial.println(" esvazia lento");
  digitalWrite (bomba1, LIGA);
  digitalWrite (valv1, ABRE);
}

