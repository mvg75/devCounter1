#include "stm32f10x.h"
//#include "stm32f10x_gpio.h"     // Файл с функциями управления ножками контроллера
//#include "stm32f10x_rcc.h"      // Управление тактированием
#include "stdint.h"             // Правильные типы данных, вместо всяких int, char и тому подобных


// Далее будут ссылки на страницы референс мануала, его можно сказать по ссылке:
// https://www.st.com/resource/en/reference_manual/CD00171190.pdf
const uint8_t zg[]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x0};//знакогенератор 0123456789пробел

// Примитивнейшая функция задержки

void Delay( void )
{
    uint32_t i,j;
    for( i = 0; i <= 50000; i++ ) //50000
        for( j = 0; j <= 5; j++ );
}
void setSegment(uint8_t segm) // вывод символа в текущий сегмент
{
segm = zg[segm];
//segm = 6;
//декодирование seg
if (segm & 1) {GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);} else {GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);}; 
if (segm & 2) {GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);} else {GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);}; 
if (segm & 4) {GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);} else {GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);}; 
if (segm & 8) {GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);} else {GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_RESET);}; 
if (segm & 16) {GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);} else {GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);}; 
if (segm & 32) {GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);} else {GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);}; 
if (segm & 64) {GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);} else {GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);}; 
}
uint8_t statusIntLed = 0;// состояние встроенного светодиода
uint16_t statusChangeCounter = 1000; //счетчик изменений встроенного светодиода
uint8_t count = 0; //счетчик динамической индикации
uint8_t digs[4] = {10,10,10,10};// 10 - "пусто" на индикаторе
void SysTick_Handler (void) // прерывание от системного таймера
{
    if (!statusChangeCounter)  { //счетчие =0, обработка мигания встроенным светодиодом
                                 statusChangeCounter = 1000;
                                 
                                 if (statusIntLed) {GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);
                                                    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);}
                                                    else  {GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);
                                                    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);        
                                                                   };
                                 statusIntLed =!statusIntLed; 
    }
    statusChangeCounter--;
    // динамическая индикация
    // all digits off	
GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET); 
GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_SET); 
GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET); 
GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_SET); 			
        
			setSegment(digs[count]); // вывод символа в текущий сегмент
            if (count==0 ) {//digit 1 on
                GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);};
            if (count==1 ) {//digit 2 on
                GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_RESET);};
            if (count==2 ) {//digit 3 on
                GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_RESET);};
            if (count==3 ) {//digit 4 on
                GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_RESET);};
            count++;
			if (count>3) {count = 0;};    
    
}
int main( void )
{   uint8_t keyRrattling;
    GPIO_InitTypeDef ldPort;    // переменная с характеристиками порта индикатора
    GPIO_InitTypeDef keyPort;    // переменная с характеристиками порта кнопок
    
    uint16_t value,tempValue;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE );  // ---------------- Включаем тактирование порта GPIOC
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );  // ---------------- Включаем тактирование порта GPIOA
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );  // ---------------- Включаем тактирование порта GPIOB

	// конфигурим порт с  

    
    ldPort.GPIO_Mode         = GPIO_Mode_Out_PP;     // Выход пуш-пул, см 164 стр.
    ldPort.GPIO_Speed        = GPIO_Speed_10MHz;     // По сути это ток который сможет обеспечить вывод
    ldPort.GPIO_Pin          = GPIO_Pin_13;          // Номер ноги
    GPIO_Init(GPIOC, &ldPort);                       // Применяем настройки
    //  конфигурим порт А
    ldPort.GPIO_Mode         = GPIO_Mode_Out_PP;
    ldPort.GPIO_Speed        = GPIO_Speed_10MHz;
    ldPort.GPIO_Pin          = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0 ;    
    GPIO_Init(GPIOA, &ldPort);                
    //  конфигурим порт B. управление индикатором	  
		ldPort.GPIO_Mode         = GPIO_Mode_Out_PP;
    ldPort.GPIO_Speed        = GPIO_Speed_10MHz;
    ldPort.GPIO_Pin          = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12;    
    GPIO_Init(GPIOB, &ldPort);                
   
    //  конфигурим порт B.кнопки	  
		keyPort.GPIO_Mode         = GPIO_Mode_IPU;
    keyPort.GPIO_Speed        = GPIO_Speed_2MHz;
    keyPort.GPIO_Pin          = GPIO_Pin_6; 
    GPIO_Init(GPIOB, &keyPort);  
    
    
    //конфигурация системного таймера
    SysTick_Config(SystemCoreClock/1000); //интервал 1мкс
/*
// all digits off	
GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET); 
GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_SET); 
GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET); 
GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_SET); 			

setSegment(0); // вывод символа в текущий сегмент
//digit 1 on
GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET); 
*/    
    value = 0;  
            // выделение  тысяч
    digs[0] = value / 1000;
    tempValue = value - digs[0] * 1000;
    // выделение сотен
    digs[1] =  tempValue / 100;
    tempValue -=digs[1] * 100;    
    // выделение десятков
    digs[2] = tempValue / 10;
    tempValue -=digs[2] * 10;    
    // выделение единиц
    digs[3] = (uint8_t) tempValue;
    
    while( 1 )
    {


/*	   GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);     
       GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);             
       Delay();
       GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
       GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);                */
       
      

                // выделение  тысяч
    digs[0] = value / 1000;
    tempValue = value - digs[0] * 1000;
    // выделение сотен
    digs[1] =  tempValue / 100;
    tempValue -=digs[1] * 100;    
    // выделение десятков
    digs[2] = tempValue / 10;
    tempValue -=digs[2] * 10;    
    // выделение единиц
    digs[3] = (uint8_t) tempValue;
  // убирание нулей справа
    if (digs[0]==0) {digs[0]=10;};//х1000
    if ((digs[1]==0) && (digs[0]==10) ){digs[1]=10;};//х100
    if ((digs[2]==0) && (digs[1]==10) ){digs[2]=10;};//х10
    //ожидание устойчивой 1
    wait1:keyRrattling = 100;
    while (keyRrattling>0) 
    {
    if (GPIO_ReadInputDataBit (GPIOB, GPIO_Pin_6) !=1) {goto wait1;}
    keyRrattling--;
    }
    //ожидание устойчивоuj 0
    wait0:keyRrattling = 100;
    while (keyRrattling>0) 
    {
    if (GPIO_ReadInputDataBit (GPIOB, GPIO_Pin_6) ==1) {goto wait0;}
    keyRrattling--;
    }
    if (value <9999) {value++;};
    
    
  //  goto l1; 
//if (GPIO_ReadInputDataBit (GPIOB, GPIO_Pin_6) !=1) {if (GPIO_ReadInputDataBit (GPIOB, GPIO_Pin_6) ==1) {value ++;};};
  //  l1:
    }
}

