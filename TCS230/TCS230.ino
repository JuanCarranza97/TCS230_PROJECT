#define  S0    2
#define  S1    3
#define  S2    4
#define  S3    5

#define OUT    6

#define RED    0
#define BLUE   1
#define GREEN  2
#define CLEAR  3

int selectedColor = RED;

int min_values[3] = {1023,1023,1023};
int max_values[3] = {0,0,0};

int Rojo,Verde,Azul;

int mapped = false;

void setup() {
  pinMode(S0,OUTPUT);
  pinMode(S1,OUTPUT);
  pinMode(S2,OUTPUT);
  pinMode(S3,OUTPUT);
  pinMode(OUT,INPUT);
  Serial.begin(115200);
  setFrecuency(1);
}

void loop() {
  if(Serial.available()){
    int caracter = Serial.read();

    switch(caracter){       
      case '0':
        setFrecuency(0);
        Serial.println("\n ---El valor de frecuencia esta en POWER DOWN---\n");
        break;

      case '1':
        setFrecuency(1);
        Serial.println("\n ---El valor de frecuencia esta en 2%---\n");
        break;

      case '2':
        setFrecuency(2);
        Serial.println("\n ---El valor de frecuencia esta en 20%---\n");
        break;

      case '3':
        setFrecuency(3);
        Serial.println("\n ---El valor de frecuencia esta en 100%---\n");
        break;

      case 'c':
        Serial.println("\n------Min------");
        Serial.println("R:"+String(min_values[0])+", G:"+String(min_values[1])+", B:"+String(min_values[2]));
        Serial.println("------Max------");
        Serial.println("R:"+String(max_values[0])+", G:"+String(max_values[1])+", B:"+String(max_values[2])+"\n");
        while(!Serial.available());
        caracter = Serial.read();
        break;

      case 'r':
        Serial.println("\n------Reset VALUES------");
        for(int i=0;i<3;i++){
          min_values[i]=1023;
          max_values[i]=0;
        }
        while(!Serial.available());
        caracter = Serial.read();
        break;
       
      case 'm':
        mapped = !mapped;
        if(mapped) Serial.println("\nLos valores se muestran mapeados\n");
        else       Serial.println("\nLos valores se muestran sin mapear\n");
        break;
        
      default:
        Serial.println("No valor detectado");
        break;
    }
    delay(250);
  }

  leer_valores();
  delay(50);
}

void leer_valores(void){
  for(int i=0;i<3;i++){
    int current = getColor(i);
    if(current < min_values[i])      min_values[i] = current;
    else if(current > max_values[i]) max_values[i] = current;

    if(mapped) current = map(current,min_values[i],max_values[i],0,255);
       
    if(i == 0) Serial.print("Rojo="+String(current));
    else if(i==1) Serial.print(", Verde="+String(current));
    else if(i==2) Serial.println(", Azul="+String(current));
    
       
  }
}

void setFrecuency(int frecuency){
  switch(frecuency){
    case 0:
      digitalWrite(S1,LOW);
      digitalWrite(S0,LOW);
      break;

    case 1:
      digitalWrite(S1,HIGH);
      digitalWrite(S0,LOW);
      break;

   case 2:
      digitalWrite(S1,LOW);
      digitalWrite(S0,HIGH);
      break;

   case 3:
      digitalWrite(S1,HIGH);
      digitalWrite(S0,HIGH);
      break;

   default:
      Serial.println("No se selecciono la frecuencia correctamente");
      break;
  }
}

int getColor(int color){
  switch(color){
    case RED:
      digitalWrite(S2,LOW);
      digitalWrite(S3,LOW);
      break;
      
    case GREEN:
      digitalWrite(S2,HIGH);
      digitalWrite(S3,HIGH);
      break;
      
    case BLUE:
      digitalWrite(S2,LOW);
      digitalWrite(S3,HIGH);
      break;
      
    default:
      Serial.print("Error en get color");
      return(-1);
      break;
  }
  float val = pulseIn(OUT,LOW)*.000001;
  return (1/(val));
}

