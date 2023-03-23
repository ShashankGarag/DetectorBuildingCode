//Corresponding port variables
int PressurePin = A5;
int LEDPin1 = 12;
int LEDPin2 = 11;
int LEDPin3 = 10;

//Raw ADC reading, calculated voltage and gram variables
float force;
float voltage;
float gram;

//Platform voltage and gram variables for taring
float platform;
float platformV;

//Loop counter, accumulating sum, and average ADC reading variables
float sum = 0;
int loop_count = 0;
float new_force;

//The final voltage and gram variables
float finalVoltage;
float finalGram;

//Sets up the arduino circuit
void setup(){
pinMode(LEDPin1, OUTPUT);
pinMode(LEDPin2, OUTPUT);
pinMode(LEDPin3, OUTPUT);
Serial.begin(9600);
}

//Performs the necessary tasks
void loop()
{
  //Voltages and grams lookup array for calibration
  float voltages[] = {0, 0.72, 0.96, 1.05, 1.10, 1.17, 1.22, 1.25, 1.27, 1.28, 1.29};
  float grams[] = {0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};

  //Defines the force variable (ADC reading)
  force = 1023 - analogRead(PressurePin);

  //Checks whether the loop function has been performed 100 times
  if (loop_count == 100) {
    new_force = sum/100;
    voltage = ((5*new_force)/1023);

    //Performs the task of turning on LED for corresponding mass ranges
    if(gram < 400){
      digitalWrite(LEDPin2, HIGH);
      digitalWrite(LEDPin1, LOW);
      digitalWrite(LEDPin3, LOW);
    }
    else if(gram > 400 && gram < 600) {
      digitalWrite(LEDPin3, HIGH);
      digitalWrite(LEDPin2, LOW);
      digitalWrite(LEDPin1, LOW);
    }
    else if(gram > 600){
      digitalWrite(LEDPin1, HIGH);
      digitalWrite(LEDPin3, LOW);
      digitalWrite(LEDPin2, LOW);
    }

    //Iterates through lookup arrays to calculate the mass
    for (int x=0; x<10; x++){
      if (voltage > voltages[x]){
        gram = grams[x] + (voltage-voltages[x])*((grams[x+1]-grams[x])/(voltages[x+1]-voltages[x]));        
      }
      
      //Keeps gram within range
      if (gram > 1000){
        gram = 1000;        
      }
      if (gram < 30){
        gram = 30;
      }

    //Displays the final voltage and mass of the object
    finalVoltage = voltage-platformV;
    finalGram = gram-platform;
    Serial.println("Voltage: " + String(finalVoltage, 1) + "V Grams: " + String(finalGram, 1) + "g");
  

    //Resets the counter and accumulating sum   
    sum = 0;
    loop_count = 0;        
    }

  //Checks if the loop function has not been performed 100 times yet  
  }    
  else{
      //Increments the loop counter and adds the current force reading to the accumulating sum
      sum = sum + force; 
      loop_count++;     
    }
    
  //Tares the platform weight if the 'k' button is clicked
  if (Serial.read() == 'k'){
        platform = gram;
        platformV = voltage;
      }
  
  delay(10);
}
