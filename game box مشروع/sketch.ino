/*
  TFT_BLACK
 TFT_ORANGE
  TFT_DARKGREEN
 TFT_DARKCYAN
   TFT_MAROON
  TFT_PURPLE
  TFT_OLIVE
  TFT_DARKGREY
 TFT_ORANGE
TFT_BLUE
TFT_GREEN
TFT_CYAN
 TFT_RED
TFT_NAVY
TFT_YELLOW
TFT_WHITE
*/





#include <TFT_eSPI.h>
#include <SPI.h>       // this is needed for display

#include <Keypad.h>



#define buzzer 21
// تعيين الاطراف 
#define TFT_CS 15    
#define TFT_DC 2
#define TFT_MOSI 23
#define TFT_SCLK 18
// ///////////////////////////
#define c 261
#define d 294
#define f 349
#define c 523


TFT_eSPI tft = TFT_eSPI(); // Invoke custom library with default width and height

unsigned long currentMillis;
unsigned long interval;

unsigned long time_score_timer;
unsigned long seconds;
unsigned long minutes;
char seconds_array[2];
char minutes_array[2];



volatile char key_press ;



TFT_eSprite spr = TFT_eSprite(&tft);  //تعريف رسم للطباعة بإسم  spr


uint16_t y =0;
uint16_t x = tft.width() / 2; // Position of centre of arc
uint16_t h = tft.height()/2;
uint16_t count  =0;

const uint8_t ROWS = 4;
const uint8_t COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
uint8_t colPins[COLS]  = { 26, 25, 33, 32 }; // Pins connected to C1, C2, C3, C4
uint8_t rowPins[ROWS] = { 13, 12, 14, 27 }; // Pins connected to R1, R2, R3, R4

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

/////////////state variable   متغيرات الخطوة
volatile uint8_t page_count =0;    // الصفحة
volatile uint8_t welcome_steps = 0;  // الصفحة الترحيبية
volatile uint8_t level_steps = 0;   // تحديد المستوى
volatile uint8_t game_steps = 0;   // اللعبة
volatile uint8_t last_steps = 0;   // الصفحة النهائية للنتيجة


/////////////////////////

void welcome();
void level();
void game();
void last();
void buzr ();


void setup(void) {
  Serial.begin(9600);
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
 welcome();
}






void loop()
{

 currentMillis = millis();
 key_press = keypad.getKey();

  // if (key_press != NO_KEY) {
  //   Serial.println(key_press);
  // }

/////////////// page scan
if( key_press == 65){
  buzr();
 key_press = NO_KEY;
page_count ++;
  if (page_count == 4){
page_count = 0;

  }
}



//////////////////// page print


if( page_count == 0){
welcome();

level_steps=0;
game_steps = 0;
last_steps = 0;

}
if(page_count == 1){
  level();

welcome_steps =0; 
game_steps = 0;
last_steps = 0;

}

if(page_count == 2){
game();

welcome_steps =0; 
level_steps=0;
last_steps = 0;

}

if(page_count == 3){
last();

welcome_steps =0; 
level_steps=0;
game_steps = 0;


}

//////////////////////////






}





void welcome(){

if(welcome_steps == 0){
 tft.fillScreen(TFT_BLACK);

tft.setCursor(20, 150, 4);
tft.setTextColor(TFT_DARKCYAN,TFT_BLACK);
tft.print("press A to continue");

 tft.setCursor(70, 60, 4);
tft.setTextColor(TFT_BLUE,TFT_BLACK);
tft.print("welcome");


tft.setTextColor(TFT_YELLOW,TFT_BLACK);
tft.setCursor(10, 260, 4);
tft.print("YASEER");


tft.setCursor(10, 290, 4);
tft.print("SAMER");


tft.setTextColor(TFT_DARKCYAN,TFT_BLACK);
tft.setCursor(60, 200, 4);
tft.print("GAME_BOX");

 welcome_steps++;
}




}



volatile int t_x0 =170 ;
volatile int t_y0 =75 ;
volatile int t_x1 =210 ;
volatile int t_y1 =60 ;
volatile int t_x2 =210 ;
volatile int t_y2 =85 ;

volatile int inc_level =0 ;    ///للزيادة


void level(){

if(level_steps == 0){
 tft.fillScreen(TFT_BLACK);
 level_steps++;
}

if(level_steps == 1){

tft.setCursor(75, 60, 4);
tft.setTextColor(TFT_BLUE,TFT_BLACK);
tft.println("Hard");
tft.setCursor(75, 100, 4);
tft.setTextColor(TFT_YELLOW,TFT_BLACK);
tft.println("Normal");
tft.setCursor(75, 140, 4);
tft.setTextColor(TFT_BLUE,TFT_BLACK);
tft.println("easy");

level_steps++;

}





if(level_steps == 2){

 ///               x ,y  , x1,y1, x2,y2
tft.fillTriangle(t_x0,(t_y0 + inc_level) ,t_x1,(t_y1+ inc_level),t_x2,(t_y2+ inc_level),TFT_BLUE);

 if( key_press == '8'){
   buzr();
   ///               x ,y  , x1,y1, x2,y2
tft.fillTriangle(t_x0,(t_y0 + inc_level) ,t_x1,(t_y1+ inc_level),t_x2,(t_y2+ inc_level),TFT_BLACK);
 inc_level = inc_level +40;
 interval = interval + 5000;
 }

 
 if( inc_level == 120  ){
inc_level = 0;
interval = 1000;
 }




}
}


volatile int c_x =150 ;   //موقع الدائرة
volatile int c_y =70 ;   // مقع الدائرة

volatile int inc_game_x =0 ;    ///للزيادة
volatile int inc_game_y =0 ;    ///للزيادة


uint16_t rec_x = 130; // نقطة بداية الطباعة للمربع  
uint16_t rec_y = 10; //  نقطة بداية الطباعة للمربع

uint16_t rec2_x = 130; // نقطة بداية الطباعة للمربع  
uint16_t rec2_y = 300; //  نقطة بداية الطباعة للمربع
 

uint16_t rec3_x = 40; // نقطة بداية الطباعة للمربع  
uint16_t rec3_y = 300; //  نقطة بداية

volatile int b_l = 20 ;    ///حجم قطر الدائرة
//////////////////////////// لجمع النقاط
volatile int b_2 = 10 ;    ///حجم قطر الدائرة
volatile int p1_x = 150;    //مكان النقطة الاولى
volatile int p1_y = 110;   //مكان النقطة الاولى
volatile int p1_f = 0;    //متغير التقاط النقطة

volatile int b_3 = 10 ;    ///حجم قطر الدائرة
volatile int p2_x = 150;    //مكان النقطة الثاني
volatile int p2_y = 170;   //مكان النقطة الاولى
volatile int p2_f = 0;    //متغير التقاط النقطة

volatile int b_4 = 10 ;    ///حجم قطر الدائرة
volatile int p3_x = 150;    //مكان النقطة الاولى
volatile int p3_y = 210;   //مكان النقطة الاولى
volatile int p3_f = 0;    //متغير التقاط النقطة

volatile int score = 1;  //// مجموع النقاط



void game(){
//spr.drawString(String(score),20,230,4);
spr.drawString(String(score),20,230,7); // طباعة القيمة بالمتغير x  بالموقع 20 و 240 و حجم خط 7
if(game_steps == 0){
tft.fillScreen(TFT_BLACK);

c_x = 150;
c_y = 70;

//  المربع   x y  العرض الطول
tft.fillRect(rec_x, rec_y, 40,    20, TFT_YELLOW);
tft.fillRect(rec2_x, rec2_y, 40,    20, TFT_YELLOW);
tft.fillRect(rec3_x, rec3_y, 40,    20, TFT_YELLOW);
tft.fillSmoothCircle(c_x  , c_y  , b_l, TFT_RED,TFT_RED) ;

tft.fillSmoothCircle(p1_x  , p1_y  , b_2, TFT_BLUE,TFT_BLUE) ;
tft.fillSmoothCircle(p2_x  , p2_y  , b_3, TFT_BLUE,TFT_BLUE) ;
tft.fillSmoothCircle(p3_x  , p3_y  , b_4, TFT_BLUE,TFT_BLUE) ;
 game_steps ++;
}

//////////////////////////////////////
if (millis() - time_score_timer >= 500) {

    seconds++;
    if(seconds > 60){
      minutes++;  
      seconds = 0;
    }
    itoa(seconds, seconds_array, 10);
    tft.setCursor(195, 0, 2);
    tft.print(":");
    tft.setCursor(200, 0, 2);
    tft.setTextColor(TFT_GREEN,TFT_BLACK);
    tft.print(seconds_array);
    time_score_timer = millis();   
  }


if(game_steps == 1){
//if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
   // previousMillis = currentMillis;
   // page_count++;
   
    



tft.fillSmoothCircle((c_x + inc_game_x), (c_y + inc_game_y), b_l, TFT_RED,TFT_RED) ;

if( ((c_y + inc_game_y) == p1_y) && ((c_x + inc_game_x) == p1_x) && (p1_f == 0)  ){
Serial.println("print me");
p1_f =1;
score ++;
}
if( ((c_y + inc_game_y) == p2_y) && ((c_x + inc_game_x) == p2_x) && (p2_f == 0)  ){
Serial.println("print mex");
p2_f =1;
score ++;
}
if( ((c_y + inc_game_y) == p3_y) && ((c_x + inc_game_x) == p3_x) && (p3_f == 0)  ){
Serial.println("print mey");
p3_f =1;
score ++;
}


if (key_press == '2' ){
  buzr();
  tft.fillSmoothCircle((c_x + inc_game_x), (c_y + inc_game_y), b_l, TFT_BLACK,TFT_BLACK) ;
  
  inc_game_y = inc_game_y -20 ;

if (   ((c_y + inc_game_y) <= (rec_y +50) )
    && ((c_x + inc_game_x) >= ( rec_x -20) )
    && ((c_x + inc_game_x) <=  (rec_x +50) )      ){


      inc_game_y = inc_game_y + 20 ;



    }


if (   ((c_y + inc_game_y) <= (rec2_y +50) )
    && ((c_x + inc_game_x) >= ( rec2_x -20) )
    && ((c_x + inc_game_x) <=  (rec2_x +50) )      ){


      inc_game_y = inc_game_y + 20 ;



    }
    if (   ((c_y + inc_game_y) <= (rec3_y +50) )
    && ((c_x + inc_game_x) >= ( rec3_x -20) )
    && ((c_x + inc_game_x) <=  (rec3_x +50) )      ){


      inc_game_y = inc_game_y + 20 ;



    }





Serial.println("X");
Serial.println((c_x + inc_game_x));
Serial.println("Y");
Serial.println((c_y + inc_game_y));
}

if (key_press == '8' ){
  buzr();
  tft.fillSmoothCircle((c_x + inc_game_x), (c_y + inc_game_y), b_l, TFT_BLACK,TFT_BLACK) ;

inc_game_y = inc_game_y +20 ;

if (   ((c_y + inc_game_y) >= (rec_y -20) )
    && ((c_x + inc_game_x) >= ( rec_x -20) )
    && ((c_x + inc_game_x) <=  (rec_x +50) )      ){

      inc_game_y = inc_game_y - 20 ;

    

    }

inc_game_y = inc_game_y +20 ;

if (   ((c_y + inc_game_y) >= (rec2_y -20) )
    && ((c_x + inc_game_x) >= ( rec2_x -20) )
    && ((c_x + inc_game_x) <=  (rec2_x +50) )      ){

      inc_game_y = inc_game_y - 20 ;

    

    }    
    if (   ((c_y + inc_game_y) >= (rec3_y -20) )
    && ((c_x + inc_game_x) >= ( rec3_x -20) )
    && ((c_x + inc_game_x) <=  (rec3_x +50) )      ){

      inc_game_y = inc_game_y - 20 ;

    

    }    
    Serial.println("X");
Serial.println((c_x + inc_game_x));
Serial.println("Y");
Serial.println((c_y + inc_game_y));
  
}
if (key_press == '4' ){
  buzr();
  tft.fillSmoothCircle((c_x + inc_game_x), (c_y + inc_game_y), b_l, TFT_BLACK,TFT_BLACK) ;
  
  inc_game_x = inc_game_x -20 ;

if (   ((c_x + inc_game_x) <= (rec_x +50) )
    && ((c_y + inc_game_y) >= ( rec_y -20) )
    && ((c_y + inc_game_y) <=  (rec_y +40) )      ){

inc_game_x = inc_game_x + 20 ;



    }

inc_game_x = inc_game_x -20 ;

if (   ((c_x + inc_game_x) <= (rec2_x +50) )
    && ((c_y + inc_game_y) >= ( rec2_y -20) )
    && ((c_y + inc_game_y) <=  (rec2_y +40) )      ){

      inc_game_x = inc_game_x + 20 ;



    } 
if (   ((c_x + inc_game_x) <= (rec3_x +50) )
    && ((c_y + inc_game_y) >= ( rec3_y -20) )
    && ((c_y + inc_game_y) <=  (rec3_y +40) )      ){

      inc_game_x = inc_game_x + 20 ;



    } 
}
Serial.println("X");
Serial.println((c_x + inc_game_x));
Serial.println("Y");
Serial.println((c_y + inc_game_y));
if (key_press == '6' ){
  buzr();
  tft.fillSmoothCircle((c_x + inc_game_x), (c_y + inc_game_y), b_l, TFT_BLACK,TFT_BLACK) ;
inc_game_x = inc_game_x +20 ;

if (   ((c_x + inc_game_x) >= (rec_x -20) )
    && ((c_y + inc_game_y) >= ( rec_y -20) )
    && ((c_y + inc_game_y) <=  (rec_y +40) )      ){

      inc_game_x = inc_game_x - 20 ;



    }

inc_game_x = inc_game_x +20 ;

if (   ((c_x + inc_game_x) >= (rec2_x -20) )
    && ((c_y + inc_game_y) >= ( rec2_y -20) )
    && ((c_y + inc_game_y) <=  (rec2_y +40) )      ){

      inc_game_x = inc_game_x - 20 ;



    }    
 inc_game_x = inc_game_x +20 ;

if (   ((c_x + inc_game_x) >= (rec3_x -20) )
    && ((c_y + inc_game_y) >= ( rec3_y -20) )
    && ((c_y + inc_game_y) <=  (rec3_y +40) )      ){

      inc_game_x = inc_game_x - 20 ;



    }    
 inc_game_x = inc_game_x +20 ; 
  
}






}



 


}
void last(){
  tft.setCursor(75, 60, 4);
tft.setTextColor(TFT_RED,TFT_BLACK);
 tft.drawString(String(score),20,230,7); // طباعة القيمة بالمتغير x  بالموقع 20 و 240 و حجم خط 7
  if(last_steps == 0){
 tft.fillScreen(TFT_BLACK);
tft.setCursor(50, 10, 4);
tft.setTextColor(TFT_YELLOW,TFT_BLACK);
tft.print("                   ");
tft.setCursor(50, 10, 4);
tft.setTextColor(TFT_RED,TFT_BLACK);
tft.print("GAME");
tft.print(" ");
tft.print("OVER");

last_steps ++;
}




}

void buzr (){

  tone(buzzer,3000);
  delay(10);
  tone(buzzer,0);


}