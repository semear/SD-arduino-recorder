#include <SPI.h>
#include <SD.h>
#include <Vector.h>

/*pinos SPI para o uno
  ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 */
 
const int sd_cs = 10;
bool err;

class Recorder{
	private:
		const int numArq = 2;
		static char *nomes[];
		File dataWrite;
		File dataRead;
		Vector<int> archiveStatus;//marca qual arquivo está sendo usado(1-leitura,2-escrita)
		String data;//teste

		void dataTreat(){

		}//trata os dados brutos obtidos dos sensores para obter os valores que serão guardados no cartão sd

	public: 
		void init(){
			if (!SD.begin(sd_cs)) {
	   			Serial.println("Card failed, or not present");
				return;
			}
			Serial.println("card initialized.");//inicialização do sd
			for(int i=0; i<numArq; i++){
				archiveStatus.push_back(0);
			}
		}//construtor para ser chamado no setup cria e inicializa o Recorder
		
		void rawDataGet(){
			Serial.println("digite um dado");
			while(!Serial.available()){
				;
			}
			data = Serial.readString();//teste
			//dataTreat(); no momento é desnecessauro
		}//coleta os dados dos sensores
		
		void data_sd_record(){
			if(!dataWrite)Serial.println("cade o meu arquivo?");
			dataWrite.print(data);
			Serial.println("recorded");
        	Serial.println(data);
      		dataWrite.flush();
		}//grava os dados no arquivo selecionado

		void data_sd_read(){
			dataRead.seek(0);
      		String temp = dataRead.readString();
      		Serial.println("ok");
	      	while(temp != NULL){
       			Serial.println(temp);
  				temp = dataRead.readString();
        		Serial.println("read");
			}
			Serial.println("fim do arquivo");
		}//le os dados do arquivo selecionado

		void clearFile(int num){
			File aux;
			SD.remove(nomes[num]);//exclui o arquivo
			aux = SD.open(nomes[num]);//recria o arquivo, mas em branco
			aux.close();
		}//deleta o arquivo num
		
		void dataClear(){
			File aux;
			for(int i = 0; i<numArq; i++){
				SD.remove(nomes[i]);//exclui o arquivo
				aux = SD.open(nomes[i]);//recria o arquivo, mas em branco
				aux.close();
			}
		}//limpa os dados gravados no cartão sd
		
		bool setFileR(int num){
			if (archiveStatus[num]) return 1;

			dataRead.close();
			dataRead = SD.open(nomes[num],FILE_READ);
			for(int i=0;i<numArq;i++){
				if(archiveStatus[i] == 1)archiveStatus[i] = 0;
			}
			archiveStatus[num] = 1;
			return 0;
		}//seleciona o arquivo de leitura 
		//retorna 1 caso o arquivo já esteja aberto como escrita
		//ou 0 se foi bem sucedido

		bool setFileW(int num){
			if (archiveStatus[num]) return 1;

			dataWrite.close();
			dataWrite = SD.open(nomes[num],FILE_WRITE);
			for(int i=0;i<numArq;i++){
				if(archiveStatus[i] == 2)archiveStatus[i] = 0;
			}
			archiveStatus[num] = 2;
			return 0;
		}//seleciona o arquivo de escrita
		//retorna 1 caso o arquivo já esteja aberto como leitura
		//ou 0 se foi bem sucedido

		void closeR(){
			dataRead.close();
			for(int i=0;i<numArq;i++){
				if(archiveStatus[i] == 1)archiveStatus[i] = 0;
			}
		}//fecha o arquivo de leitura

		void closeW(){
			dataWrite.close();
			for(int i=0;i<numArq;i++){
				if(archiveStatus[i] == 2)archiveStatus[i] = 0;
			}
		}//fecha o arquivo de escrita
};//classe que grava no SD

char* Recorder::nomes[]={"dados/a.txt","dados/b.txt"};
//nomes dos arquivos que serão editados
//as pastas ja devem estar criadas, os arquivos não necessáriamente
Recorder rec;
