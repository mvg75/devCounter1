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
    for( i = 0; i <= 5000; i++ )
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
int main( void )
{   uint8_t count = 0;
    GPIO_InitTypeDef ldPort;    // переменная с характеристиками порта
    
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
//  конфигурим порт B	  
		ldPort.GPIO_Mode         = GPIO_Mode_Out_PP;
    ldPort.GPIO_Speed        = GPIO_Speed_10MHz;
    ldPort.GPIO_Pin          = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12;    
    GPIO_Init(GPIOB, &ldPort);                
// off lcd	
GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET); 
GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_SET); 
GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET); 
GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_SET); 			

setSegment(0); // вывод символа в текущий сегмент
//digit1 on
GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET); 


      while( 1 )
    {


			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);     
       GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);             
       Delay();
       GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
       GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);                
       Delay();        
			setSegment(count); // вывод символа в текущий сегмент
			count++;
			if (count>9) {count = 0;};
    }
}

