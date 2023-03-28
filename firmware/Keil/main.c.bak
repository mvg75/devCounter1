#include "stm32f10x.h"
//#include "stm32f10x_gpio.h"     // Файл с функциями управления ножками контроллера
//#include "stm32f10x_rcc.h"      // Управление тактированием
#include "stdint.h"             // Правильные типы данных, вместо всяких int, char и тому подобных


// Далее будут ссылки на страницы референс мануала, его можно сказать по ссылке:
// https://www.st.com/resource/en/reference_manual/CD00171190.pdf


// Примитивнейшая функция задержки

void Delay( void )
{
    uint32_t i,j;
    for( i = 0; i <= 250000; i++ )
        for( j = 0; j <= 5; j++ );
}
int main( void )
{
    GPIO_InitTypeDef ldPort;    // переменная с характеристиками порта
    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE );  // ---------------- Включаем тактирование порта GPIOC
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );  // ---------------- Включаем тактирование порта GPIOA
    // конфигурим порт с  

    
    ldPort.GPIO_Mode         = GPIO_Mode_Out_PP;     // Выход пуш-пул, см 164 стр.
    ldPort.GPIO_Speed        = GPIO_Speed_10MHz;     // По сути это ток который сможет обеспечить вывод
    ldPort.GPIO_Pin          = GPIO_Pin_13;          // Номер ноги
    GPIO_Init(GPIOC, &ldPort);                       // Применяем настройки
    //  конфигурим порт А
    ldPort.GPIO_Mode         = GPIO_Mode_Out_PP;
    ldPort.GPIO_Speed        = GPIO_Speed_10MHz;
    ldPort.GPIO_Pin          = GPIO_Pin_7;    
    GPIO_Init(GPIOA, &ldPort);                
    
      while( 1 )
    {
  
       GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);     
       GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);             
       Delay();
       GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
       GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);                
       Delay();        
    }
}

