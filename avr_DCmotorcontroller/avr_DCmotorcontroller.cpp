#define MAXFREQ 780
#define refreshTime 20

int pwm = 5;
unsigned long elapsedRefreshTime = 0;
unsigned long startTime = 0;
unsigned long medElapsedTime = 0;
unsigned long elapsedTime[5]= {0,0,0,0,0};
unsigned long medFrequency = 0;
unsigned long lastMedFrequency = 150;


int elapsedTimeIndex = 0;

void setup() {                
  Serial.begin(9600);
  Serial.setTimeout(5);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);     
  pinMode(pwm, OUTPUT);
  digitalWrite (8, HIGH);
  digitalWrite (9, LOW);  
  attachInterrupt (0, countRevolutions, FALLING);
}

void loop() {
	  //Verifica se há bytes na porta serial
	   if (Serial.available() > 0) {
			//Escreve o valor recebido pelo software supervisor na saída para o motor
			analogWrite(pwm, Serial.parseInt());
	   }
	   if(millis()-elapsedRefreshTime>refreshTime){
		  elapsedRefreshTime = millis();
	   }    
	   if (millis()-elapsedRefreshTime==refreshTime){
		  //frequency &= 0b1111111111111100; // suprimindo 2 bits da leitura

      medElapsedTime = getMedElapsedTime();
      
      if(medElapsedTime > 0){
      
      medFrequency = (1000000/medElapsedTime);

          //Serial.println (medFrequency);
        
          //Tratamento necessário para evitar ruídos na leitura da velocidade
          if(medFrequency < MAXFREQ){
            Serial.println ((int)medFrequency);
            lastMedFrequency = medFrequency;
          }else{
            Serial.println ((int)lastMedFrequency);
          }
        
        
      }else{
          Serial.println ((int)(0));
      }
		  //attachInterrupt (0, countRevolutions, FALLING);
		  elapsedRefreshTime = millis();
	  }
} 

void countRevolutions()
{
    elapsedTime[elapsedTimeIndex]=micros()-startTime;
    
    if(elapsedTimeIndex==4){
      elapsedTimeIndex=0;
    }else{
      elapsedTimeIndex++;
    }
    
    startTime=micros();
}

int getMedElapsedTime(){
  
  int i, lower = elapsedTime[0], higher=elapsedTime[0],medElapsedTime,total=0;
  
  for(i=0;i<5;i++){
    total=total+elapsedTime[i];
    if(elapsedTime[i]>higher){
      higher = elapsedTime[i];
    }
    if(elapsedTime[i]<lower){
      lower = elapsedTime[i];
    }
  }
    
   medElapsedTime = (total - lower - higher)/3;
    
   return medElapsedTime;
}
