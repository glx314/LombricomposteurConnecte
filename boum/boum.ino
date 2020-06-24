void setup() {
  pinMode(7,INPUT_PULLUP);// put your setup code here, to run once:
  
  
}

int i=1;
int j=2;
void loop() {
    if(i==1){
      delay(5000);
      Serial.println("             . . .                         ");
  Serial.println("              \\|/                          ");
  Serial.println("            `--+--'                        ");
  Serial.println("              /|\\                          ");
  Serial.println("             ' | '                         ");
  Serial.println("               |                           ");
  Serial.println("               |                           ");
  Serial.println("           ,--'#`--.                       ");
  Serial.println("           |#######|                       ");
  Serial.println("        _.-'#######`-._                    ");
  Serial.println("     ,-'###############`-.                 ");
  Serial.println("   ,'#####################`,               ");
  Serial.println("  /#########################\\              ");
  Serial.println(" |###########################|             ");
  Serial.println("|#############################|            ");
  Serial.println("|#############################|            ");
  Serial.println("|#############################|            ");
  Serial.println("|#############################|            ");
  Serial.println(" |###########################|             ");
  Serial.println("  \\#########################/              ");
  Serial.println("   `.#####################,'               ");
  Serial.println("     `._###############_,'                 ");
  Serial.println("        `--..#####..--'");
  i++;
      }
    if(not(digitalRead(7)) && j==2){
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("                             ____");
    Serial.println("                     __,-~~/~    `---.");
    Serial.println("                   _/_,---(      ,    )");
    Serial.println("               __ /        <    /   )  \\___");
    Serial.println("- ------===;;;'====------------------===;;;===----- -  -");
    Serial.println("                 \\/  ~\"~\"~\"~\"~\"~\\~\"~)~\"/");
    Serial.println("                  (_ (   \\  (     >    \\)");
    Serial.println("                   \\_( _ <         >_>'");
    Serial.println("                      ~ `-i' ::>|--\"");
    Serial.println("                          I;|.|.|");
    Serial.println("                          I;|.|.|");
    Serial.println("                          I;|.|.|");
    Serial.println("                          I;|.|.|");
    Serial.println("                          I;|.|.|");
    Serial.println("                          I;|.|.|");
    Serial.println("                         <|i::|i|`.");
    Serial.println("                        (` ^'\"`-' \")");
    Serial.println("");
   
    j++;
    }// put your main code here, to run repeatedly:
  delay(100);
}
