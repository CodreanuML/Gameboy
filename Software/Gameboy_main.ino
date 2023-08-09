
/* definirea tastaturii */
#define OK 2
#define left 7
#define up 3
#define down 4
#define right 5
#define NOK 6

/*definirea interfetei cu display-ul Nokia 511 */
#define RST 8
#define DC 9
#define DIN 10
#define CLK 11
#define CE 12

int button1;
int button2;
int button3;
int button4;
int button5;
int button6;

char up_text[]="up";
char left_text[]="left";
char right_text[]="right";
char down_text[]="down";
char OK_text[]="ok";
char NOK_text[]="nok";

int print_ok=0;
int print_nok=0;
int print_up=0;
int print_down=0;
int print_left=0;
int print_right=0;

void read_buttons();
void debug_buttons();

void LCD_INIT();
void shiftOut(uint8_t dataPin, uint8_t clockPin,  uint8_t val);
void LcdWriteCmd(char cmd);
void LcdWriteData(char dat);

void setup() {
  /* setarea tastaturii */
  pinMode(OK,INPUT);
  pinMode(left,INPUT);
  pinMode(up,INPUT);
  pinMode(down,INPUT);
  pinMode(right,INPUT);

  LCD_INIT();

  Serial.begin(9600);
  // put your setup code here, to run once:

  
  
}

void loop() {
  read_buttons();
  debug_buttons();
  LcdWriteData('a');


}

void read_buttons(){
  button1=digitalRead(OK);
  button2=digitalRead(left);
  button3=digitalRead(up);
  button4=digitalRead(down);
  button5=digitalRead(right);
  button6=digitalRead(NOK);
}

void debug_buttons(){

  if (button1){
    while(print_ok<strlen(OK_text)){
      Serial.print(OK_text[print_ok]);
      print_ok++;
    }
  }

    if (button2){
    while(print_left<strlen(left_text)){
      Serial.print(left_text[print_left]);
      print_left++;
    }

  }

  if (button3){
    while(print_up<strlen(up_text)){
      Serial.print(up_text[print_up]);
      print_up++;
    }

  }

    if (button4){
    while(print_down<strlen(down_text)){
      Serial.print(down_text[print_down]);
      print_down++;
    }

    }
        if (button5){
    while(print_right<strlen(right_text)){
      Serial.print(right_text[print_right]);
      print_right++;
    }

  }

          if (button6){
    while(print_nok<strlen(NOK_text)){
      Serial.print(NOK_text[print_nok]);
      print_nok++;
    }

  }

    print_ok=0;
    print_nok=0;
    print_up=0;
    print_down=0;
    print_left=0;
    print_right=0;
    Serial.println('\n');

}


void LCD_INIT(){
		 pinMode(RST, OUTPUT);
		 pinMode(CE, OUTPUT);
		 pinMode(DC, OUTPUT);
		 pinMode(DIN, OUTPUT);
		 pinMode(CLK, OUTPUT);
		 digitalWrite(RST, LOW);
		 digitalWrite(RST, HIGH);
		 
		 LcdWriteCmd(0x21);  // LCD extended commands
		 LcdWriteCmd(0xB0);  // set LCD Vop (contrast)
		 LcdWriteCmd(0x04);  // set temp coefficent
		 LcdWriteCmd(0x14);  // LCD bias mode 1:40
		 LcdWriteCmd(0x20);  // LCD basic commands
		 LcdWriteCmd(0x0C);  // LCD basic commands
	
}

void shiftOut(uint8_t dataPin, uint8_t clockPin,  uint8_t val)
{
	uint8_t i;
	/// SHIFT OUT MSB FIRST
	for (i = 0; i < 8; i++)  {
		
		digitalWrite(dataPin, !!(val & (1 << (7 - i))));
		
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);
	}
}


void LcdWriteCmd(char cmd)
{
	digitalWrite(DC, LOW); //DC pin is low for commands
	digitalWrite(CE, LOW);
	shiftOut(DIN, CLK, cmd); //transmit serial commands
	digitalWrite(CE, HIGH);
}


void LcdWriteData(char dat)
{
	digitalWrite(DC, HIGH); //DC pin is high for data
	digitalWrite(CE, LOW);
	shiftOut(DIN, CLK, dat); //transmit serial data
	digitalWrite(CE, HIGH);
}








#define LINE_LENGTH_LVL0 84
#define COLUMN_LVL0 6


#define LINE_LENGTH_LVL2 42
#define COLUMN_LVL2 24

char display_lvl0[COLUMN_LVL0][LINE_LENGTH_LVL0];


bool display_lvl2[COLUMN_LVL2][LINE_LENGTH_LVL2];



int get_column(int column_lvl2){
  int column_ret;
  column_ret=column_lvl2/4;
  return column_ret;
  };

char get_pattern_on(int column_lvl2){
  char pattern;
  pattern=(char)column_lvl2%4;
  if (pattern==0){
    pattern=0x03;
    
  }
  else if ( pattern==1){
    pattern=0x0C;
    
  }
  else if ( pattern==2){
    pattern=0x30;
    
  }  
  else if ( pattern==3){
    pattern=0xC0;  
  } 
}


char get_pattern_off(int column_lvl2){
  char pattern;
  pattern=(char)column_lvl2%4;
  if (pattern==0){
    pattern=0xFC;
    
  }
  else if ( pattern==1){
    pattern=0xF3;
    
  }
  else if ( pattern==2){
    pattern=0xCF;
    
  }  
  else if ( pattern==3){
    pattern=0x3F;  
  } 
}
 
int get_line(int line_lvl2){
  return line_lvl2/2;
  
  };
