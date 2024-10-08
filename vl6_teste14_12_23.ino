//add a biblioteca wire --------------
//#include <SoftWire.h> //nunca vi essa mas quando add a wire no gerenciador de bibliotecas ela veio junto ent deve ser utilizada internamente na wire.h ou outro lugar
//Essa biblioteca  SoftWire ja tem dentro do wire.h no arduino então eu separei as bibliotecas internas dessa wire
#include <Wire.h>
//--------------------------

//add a biblioteca VL6180x---------
#include <VL6180X.h>
//----------------------------


//--------------------- code ----------------------------
//referências: https://github.com/pololu/vl6180x-arduino/blob/master/examples/RangeSingleShotScaling/RangeSingleShotScaling.ino
//             https://forum.pololu.com/t/issue-with-reading-multiple-vl6180x-sensor-breakout-boards/10792/4
//             https://github.com/adafruit/Adafruit_VL6180X/blob/master/examples/vl6180x_triple/vl6180x_triple.ino
//             https://www.st.com/resource/en/application_note/an4478-using-multiple-vl6180xs-in-a-single-design-stmicroelectronics.pdf



#define SCALING 1     //Definindo o valor da palavra SCALING q será utilizada para alterar o valor da escala
#define SHT_sens1 A7  //Substiuir n pelo pino do STM q estiver ligado o SHDN e...1
#define SHT_sens2 10  //...aqui o mesmo 1
#define SHT_sens3 A3  //...aqui o mesmo 3
#define SHT_sens4 A6  //Provalvemente esse vai dar errado  2


VL6180X sensor1;  //Pelo que entendi é necessário um objeto para cada sensor
VL6180X sensor2;
VL6180X sensor3;
VL6180X sensor4;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  //Instanciando os sensores desligados para resetar combase no link 3 porém possívelmente desnecessário ja que não vamos mudar o endereço inicial dos sensores (vamos?)

  //Baseado no link3 ==================================================================================================================================================================
  //Observação: pelo q entendi ligando e iniciando os sensores um por um permite q eles recebam diferentes endereços, caso contrário todos os sensores ligados ao mesmo tempo faz com q
  //            recebam o mesmo endereço e logo o código irá fornecer a leitura de todos os sensores como se fosse um

  pinMode(SHT_sens1, OUTPUT);
  pinMode(SHT_sens2, OUTPUT);
  pinMode(SHT_sens3, OUTPUT);
  pinMode(SHT_sens4, OUTPUT);

  //Todos resetados
  digitalWrite(SHT_sens1, LOW);
  digitalWrite(SHT_sens2, LOW);
  digitalWrite(SHT_sens3, LOW);
  digitalWrite(SHT_sens4, LOW);
  delay(500);  //o delay era de 10 mas optei por 500 acredito q oferecer mais tempo para código seja mais "saudável" (me da aflição apenas 10 milisegundos como estava na referência

  //Todos não resetados
  digitalWrite(SHT_sens1, HIGH);
  delay(1000);
  digitalWrite(SHT_sens2, HIGH);
  delay(1000);
  digitalWrite(SHT_sens3, HIGH);
  delay(1000);
  digitalWrite(SHT_sens4, HIGH);
  delay(1000); 
  // Mudando a ordem da inicialização dos sensores no void setup


  //Baseado no link1 mas com a leve presença do link3 e link2 =================================================================================================================================
  //Iniciando o sensor1
  digitalWrite(SHT_sens1, LOW);
  digitalWrite(SHT_sens2, LOW);
  digitalWrite(SHT_sens3, LOW);
  digitalWrite(SHT_sens4, HIGH);
  delay(500);  //aqui estava **delay(50)**; mas estava dando erro ent mudei
  sensor4.init();
  sensor4.configureDefault();
  sensor4.setScaling(SCALING);
  sensor4.setTimeout(500);
  sensor4.setAddress(0x84);  //0x84//0x34
  delay(500);
  digitalWrite(SHT_sens4, LOW);
 // delay(500);

  //Iniciando o sensor2
  digitalWrite(SHT_sens2, HIGH);
  delay(500);  //aqui estava **delay(50)**; mas estava dando erro ent mudei
  sensor2.init();
  sensor2.configureDefault();
  sensor2.setScaling(SCALING);
  sensor2.setTimeout(500);
  sensor2.setAddress(0x108);  //0x108//0x36
  delay(500);
  digitalWrite(SHT_sens2, LOW);
  //delay(500);

  //Iniciando o sensor3
  digitalWrite(SHT_sens3, HIGH);
  delay(500);  //aqui estava **delay(50)**; mas estava dando erro ent mudei
  sensor3.init();
  sensor3.configureDefault();
  sensor3.setScaling(SCALING);
  sensor3.setTimeout(500);
  sensor3.setAddress(0x72);  //0x72//0x40
  delay(500);
  digitalWrite(SHT_sens3, LOW);
  //delay(500);

  //Iniciando o sensor4
  digitalWrite(SHT_sens1, HIGH);
  delay(500);  //aqui estava **delay(50)**; mas estava dando erro ent mudei
  sensor1.init();
  sensor1.configureDefault();
  sensor1.setScaling(SCALING);
  sensor1.setTimeout(500);
  sensor1.setAddress(0x129);  //Esse que tá dando problema//0x54
  delay(500);
  digitalWrite(SHT_sens1, LOW);
  delay(500);




  //próximos sensores colocar setAddress(0x84) e logicamente assim por  diante
  //[https://www.st.com/resource/en/application_note/an4478-using-multiple-vl6180xs-in-a-single-design-stmicroelectronics.pdf]
  //neste link o teste seria com 8 sensores então os valores para o address possuem mais chances de estar corretos

  //se não funcionar com esses address testar com sens1 usando 0x29 e sen2 usando 0x52
  //OBS: o address 0x29 é o que vem de fabrica, todos os sensores vem com esse endereço e por isso dá problema se
  //não colocarmos endereços diferentes
}



void loop() {
  //Imprimindo a distância do primeiro sensor considerando a escala---------------------------
  /*Serial.print("\n(Scaling1 = ");
  Serial.print(sensor1.getScaling());
  Serial.print("x) ");
  */

  Serial.print("\nSensor1 = ");
  Serial.print(sensor1.readRangeSingleMillimeters());
  Serial.print("mm  ");
  if (sensor1.timeoutOccurred()) { Serial.print("\nTIMEOUT\n"); }


  //--------------------------------------------------------------------------------------------

  //Imprimindo a distância do segundo sensor considerando a escala
  /*Serial.print("(Scaling2 = ");
  Serial.print(sensor2.getScaling());
  Serial.print("x) ");
  */
  Serial.print("Sensor2 = ");
  Serial.print(sensor2.readRangeSingleMillimeters());
  Serial.print("mm  ");
  if (sensor2.timeoutOccurred()) { Serial.print("\nTIMEOUT\n"); }

  //--------------------------------------------------------------------------------------------

  //Imprimindo a distância do segundo sensor considerando a escala
  /*Serial.print("(Scaling3 = ");
  Serial.print(sensor3.getScaling());
  Serial.print("x) ");
  */
  Serial.print("Sensor3 = ");
  Serial.print(sensor3.readRangeSingleMillimeters());
  Serial.print("mm  ");
  if (sensor3.timeoutOccurred()) { Serial.print("\nTIMEOUT\n"); }

  //--------------------------------------------------------------------------------------------

  //Imprimindo a distância do segundo sensor considerando a escala
  /*Serial.print("(Scaling4 = ");
  Serial.print(sensor4.getScaling());
  Serial.print("x) ");
  */
  Serial.print("Sensor4 = ");
  Serial.print(sensor4.readRangeSingleMillimeters());
  Serial.print("mm  ");
  if (sensor4.timeoutOccurred()) { Serial.print("\nTIMEOUT\n"); }
}
