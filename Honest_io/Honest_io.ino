// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.
//by Open-Smart Team and Catalex Team
//catalex_inc@163.com
//Store:   http://dx.com
//Demo Function: Display graphics, characters

//Arduino IDE: 1.6.5
// Board: Arduino UNO R3, Arduino Mega2560,Arduino Leonardo

#include <SPI.h>          // f.k. for Arduino-1.5.2
//#define USE_SDFAT
#include <SD.h>

#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>

#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif
//***********************************************//
// If you use OPEN-SMART TFT breakout board                 //
// Reconmmend you to add 5V-3.3V level converting circuit.
// Of course you can use OPEN-SMART UNO Black version with 5V/3.3V power switch,
// you just need switch to 3.3V.
// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
//----------------------------------------|
// TFT Breakout  -- Arduino UNO / Mega2560 / OPEN-SMART UNO Black
// GND              -- GND
// 3V3               -- 3.3V
// CS                 -- A3
// RS                 -- A2
// WR                -- A1
// RD                 -- A0
// RST                -- 2
// LED                -- GND
// DB0                -- 8
// DB1                -- 9
// DB2                -- 10
// DB3                -- 11
// DB4                -- 4
// DB5                -- 13
// DB6                -- 6
// DB7                -- 7


// SD_CS              -- 5
// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

uint16_t g_identifier;

#define SD_CS 5
#define LCD_RESET 2

int tela = 0;
// Tela 0 - inicio
// Tela 1 - info
// Tela 2 - Pix1.50
// Tela 3 - Pix2.00
// Tela 4 - Pix2.50
// Tela 5 - Agradecimentos

File root;
int pathlen;
uint8_t         spi_save;


uint8_t YP = A1;  // must be an analog pin, use "An" notation!
uint8_t XM = A2;  // must be an analog pin, use "An" notation!
uint8_t YM = 7;   // can be a digital pin
uint8_t XP = 6;   // can be a digital pin
uint8_t SwapXY = 0;

uint16_t TS_LEFT = 870;
uint16_t TS_RT  = 165;
uint16_t TS_TOP = 870;
uint16_t TS_BOT = 110;
//char *name = "Unknown controller";

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 270 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 250);
TSPoint tp;

#define MINPRESSURE 20
#define MAXPRESSURE 1000

// Usado para inverter os paramentros quando usar a tela "de cabeça pra baixo"
#define SWAP(a, b) {uint16_t tmp = a; a = b; b = tmp;}

int16_t BOXSIZE;
int16_t PENRADIUS = 3;
uint16_t identifier, oldcolor, currentcolor;
uint8_t Orientation = 0;    //PORTRAIT

void setup(void) {

    uint16_t ID;
    Serial.begin(115200);
    Serial.print("Show BMP files on TFT with ID:0x");

    ID = tft.readID();
    Serial.println(ID, HEX);
 
    tft.begin(115200);
    
    tft.reset();
    
    switch (Orientation) {      // adjust for different aspects
        case 0:   break;        //no change,  calibrated for PORTRAIT
//        case 1:   tmp = TS_LEFT, TS_LEFT = TS_BOT, TS_BOT = TS_RT, TS_RT = TS_TOP, TS_TOP = tmp;  break;
        case 2:   SWAP(TS_LEFT, TS_RT);  SWAP(TS_TOP, TS_BOT); break;
//        case 3:   tmp = TS_LEFT, TS_LEFT = TS_TOP, TS_TOP = TS_RT, TS_RT = TS_BOT, TS_BOT = tmp;  break;
    }
 
    tft.begin(0x7793);//to enable ST7793 driver code
//    show_Serial();
    tft.setRotation(Orientation);
    tft.fillScreen(BLACK);
 //  show_tft();

 
  //tft.setRotation(2);
    bool good = SD.begin(SD_CS);
    if (!good) {
        Serial.print(F("cannot start SD"));
        while (1);
    }

//###### SETUP ######


//    tft.begin(g_identifier);

//    SD.begin(SD_CS);
  
 bmpDraw("inicio.bmp", 0, 0);//
  tela=0;
  delay(500);
}

int clean = 1;

String conteudo = "";
char caractere;

void loop(void) {
  
  uint16_t xpos, ypos;  //screen coordinates
  tp = ts.getPoint();   //tp.x, tp.y are ADC values
  
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  pinMode(XP, OUTPUT);
  pinMode(YM, OUTPUT);
  
    //Serial.println(tp.z);

 if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
   if (tela==0){
    if (tp.x < 480  && tp.y > 820){ 
      bmpDraw("info.bmp", 0, 0);//
      tela=1;
      
      delay(500);
    //tft.fillScreen(BLACK);
    }
    if (tp.y > 600 && tp.y < 780){
      bmpDraw("pix1.bmp", 0, 0);//
      tela=2;
      Serial.println("2.00,pix_vendedor1@gmail.com(EMAIL)");
      //digitalWrite(A5,HIGH);
      delay(500);
    }
    if (tp.y > 390 && tp.y < 570){
      bmpDraw("pix2.bmp", 0, 0);//
      Serial.println("4.00,pix_vendedor2@gmail.com(EMAIL)");
      tela=3;
      //digitalWrite(A5,LOW);
      delay(500);
    }
    if (tp.y > 175 && tp.y < 350){
      bmpDraw("pix3.bmp", 0, 0);//
      Serial.println("35.00,cpf_vendedor3(CPF)");
      tela=4;
      delay(500);
    }
    } else if (tela==1){
      if (tp.x < 480  && tp.y > 820){ 
        bmpDraw("inicio.bmp", 0, 0);//
        tela=0;
        delay(500);
      }
    } else if(tela==2||tela==3||tela==4){
        if (tp.x < 480  && tp.y > 820){ 
        bmpDraw("inicio.bmp", 0, 0);//
        tela=0;
        delay(500);
        }


      }

  }

    // ######### Leitura Serial ###############
  


  // Enquanto receber algo pela serial
  while(Serial.available() > 0) {
    if(clean){
      clean=0;
      conteudo="";
    }
    // Lê byte da serial
    caractere = Serial.read();
    // Ignora caractere de quebra de linha
    if (caractere != '\n'){
      // Concatena valores
      conteudo.concat(caractere);
    }
    
    // Aguarda buffer serial ler próximo caractere
    delay(10);
  } 
  clean = 1; //nos garante limpar a variavel sempre que for ler os dados da proxima linha

// msg enviada "valor,chave(key_type)"

 // key_type
      // CPF      // string      // Chave do tipo CPF

      // CNPJ      // string      // Chave do tipo CNPJ

      // EMAIL      // string      // Chave do tipo EMAIL

      // PHONE      // string      // Chave do tipo PHONE

      // EVP      // string      // Chave aleátoria


//  Serial.println(conteudo);

  if(conteudo!=""){  //esperar "Pagamento Recebido!"
    if(conteudo=="pago"){
        conteudo="";
        bmpDraw("agrad.bmp", 0, 0);//
        delay(1500);
        bmpDraw("inicio.bmp", 0, 0);
        tela=0;
        delay(500);
    }
  }
 
delay(100);
}


void func_inicio(){ //TSPoint *tp){

  bmpDraw("inicio.bmp", 0, 0);//
  tela=0;
  delay(1000);

}



 // This function opens a Windows Bitmap (BMP) file and
 // displays it at the given coordinates.  It's sped up
 // by reading many pixels worth of data at a time
 // (rather than pixel by pixel).  Increasing the buffer
 // size takes more of the Arduino's precious RAM but
 // makes loading a little faster.	20 pixels seems a
 // good balance.
 #define BUFFPIXEL 80
 
 void bmpDraw(char *filename, int x, int y) {
   File 	bmpFile;
   int		bmpWidth, bmpHeight;   // W+H in pixels
   uint8_t	bmpDepth;			   // Bit depth (currently must be 24)
   uint32_t bmpImageoffset; 	   // Start of image data in file
   uint32_t rowSize;			   // Not always = bmpWidth; may have padding
   uint8_t	sdbuffer[3*BUFFPIXEL]; // pixel in buffer (R+G+B per pixel)
   uint16_t lcdbuffer[BUFFPIXEL];  // pixel out buffer (16-bit per pixel)
   uint8_t	buffidx = sizeof(sdbuffer); // Current position in sdbuffer
   boolean	goodBmp = false;	   // Set to true on valid header parse
   boolean	flip	= true; 	   // BMP is stored bottom-to-top
   int		w, h, row, col;
   uint8_t	r, g, b;
   uint32_t pos = 0, startTime = millis();
   uint8_t	lcdidx = 0;
   boolean	first = true;
 
   if((x >= tft.width()) || (y >= tft.height())) return;
 
   //Serial.println();
   //Serial.print("Loading image '");
   //Serial.print(filename);
   //Serial.println('\'');
   // Open requested file on SD card
   SPCR = spi_save;
   if ((bmpFile = SD.open(filename)) == NULL) {
	 Serial.print("File not found");
	 return;
   }
 
   // Parse BMP header
   if(read16(bmpFile) == 0x4D42) { // BMP signature
	 read32(bmpFile);
	 (void)read32(bmpFile); // Read & ignore creator bytes
	 bmpImageoffset = read32(bmpFile); // Start of image data
	 //Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
	 // Read DIB header
	 read32(bmpFile);
	 bmpWidth  = read32(bmpFile);
	 bmpHeight = read32(bmpFile);
	 if(read16(bmpFile) == 1) { // # planes -- must be '1'
	   bmpDepth = read16(bmpFile); // bits per pixel
	   //Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
	   if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed
 
		 goodBmp = true; // Supported BMP format -- proceed!
		// Serial.print(F("Image size: "));
		// Serial.print(bmpWidth);
		// Serial.print('x');
		// Serial.println(bmpHeight);
 
		 // BMP rows are padded (if needed) to 4-byte boundary
		 rowSize = (bmpWidth * 3 + 3) & ~3;
 
		 // If bmpHeight is negative, image is in top-down order.
		 // This is not canon but has been observed in the wild.
		 if(bmpHeight < 0) {
		   bmpHeight = -bmpHeight;
		   flip 	 = false;
		 }
 
		 // Crop area to be loaded
		 w = bmpWidth;
		 h = bmpHeight;
		 if((x+w-1) >= tft.width())  w = tft.width()  - x;
		 if((y+h-1) >= tft.height()) h = tft.height() - y;
 
		 // Set TFT address window to clipped image bounds
		 SPCR = 0;
		 tft.setAddrWindow(x, y, x+w-1, y+h-1);
 
		 for (row=0; row<h; row++) { // For each scanline...
		   // Seek to start of scan line.  It might seem labor-
		   // intensive to be doing this on every line, but this
		   // method covers a lot of gritty details like cropping
		   // and scanline padding.  Also, the seek only takes
		   // place if the file position actually needs to change
		   // (avoids a lot of cluster math in SD library).
		   if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
			 pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
		   else 	// Bitmap is stored top-to-bottom
			 pos = bmpImageoffset + row * rowSize;
		   SPCR = spi_save;
		   if(bmpFile.position() != pos) { // Need seek?
			 bmpFile.seek(pos);
			 buffidx = sizeof(sdbuffer); // Force buffer reload
		   }
 
		   for (col=0; col<w; col++) { // For each column...
			 // Time to read more pixel data?
			 if (buffidx >= sizeof(sdbuffer)) { // Indeed
			   // Push LCD buffer to the display first
			   if(lcdidx > 0) {
				 SPCR	= 0;
				 tft.pushColors(lcdbuffer, lcdidx, first);
				 lcdidx = 0;
				 first	= false;
			   }
			   SPCR = spi_save;
			   bmpFile.read(sdbuffer, sizeof(sdbuffer));
			   buffidx = 0; // Set index to beginning
			 }
 
			 // Convert pixel from BMP to TFT format
			 b = sdbuffer[buffidx++];
			 g = sdbuffer[buffidx++];
			 r = sdbuffer[buffidx++];
			 lcdbuffer[lcdidx++] = tft.color565(r,g,b);
		   } // end pixel
		 } // end scanline
		 // Write any remaining data to LCD
		 if(lcdidx > 0) {
		   SPCR = 0;
		   tft.pushColors(lcdbuffer, lcdidx, first);
		 } 
		 //Serial.print(F("Loaded in "));
		 //Serial.print(millis() - startTime);
		 //Serial.println(" ms");
	   } // end goodBmp
	 }
   }
 
   bmpFile.close();
   if(!goodBmp) Serial.println("BMP format not recognized.");
 }
 
 // These read 16- and 32-bit types from the SD card file.
 // BMP data is stored little-endian, Arduino is little-endian too.
 // May need to reverse subscript order if porting elsewhere.
 
 uint16_t read16(File f) {
   uint16_t result;
   ((uint8_t *)&result)[0] = f.read(); // LSB
   ((uint8_t *)&result)[1] = f.read(); // MSB
   return result;
 }
 
 uint32_t read32(File f) {
   uint32_t result;
   ((uint8_t *)&result)[0] = f.read(); // LSB
   ((uint8_t *)&result)[1] = f.read();
   ((uint8_t *)&result)[2] = f.read();
   ((uint8_t *)&result)[3] = f.read(); // MSB
   return result;
 }
 


