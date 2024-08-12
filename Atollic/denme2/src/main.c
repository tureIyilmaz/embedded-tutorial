#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_i2c.h"


#define LCD_I2C               I2C1
#define LCD_I2C_RCC           RCC_APB1Periph_I2C1
#define LCD_I2C_GPIO_RCC      RCC_AHB1Periph_GPIOB
#define LCD_I2C_SCL_PIN       GPIO_Pin_6
#define LCD_I2C_SDA_PIN       GPIO_Pin_7  // Deðiþtirilen pin

#define LCD_ADDRESS           0x3F

int count;

void I2C_LCD_Init(void);
void I2C_LCD_SendCommand(uint8_t command);
void I2C_LCD_SendData(uint8_t data);
void I2C_LCD_SendString(char *str);
void delay_ms(int);
int main(void) {
    // I2C ve GPIO saat sinyallerini etkinleþtirme
    RCC_APB1PeriphClockCmd(LCD_I2C_RCC, ENABLE);
    RCC_AHB1PeriphClockCmd(LCD_I2C_GPIO_RCC, ENABLE);

    // I2C GPIO pinlerini yapýlandýrma
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = LCD_I2C_SCL_PIN | LCD_I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // GPIO alternatif fonksiyonunu I2C olarak ayarlama
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);  // Deðiþtirilen pin

    // I2C yapýlandýrma
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 100000; // I2C hýzý (100kHz)
    I2C_Init(LCD_I2C, &I2C_InitStructure);

    // I2C etkinleþtirme
    I2C_Cmd(LCD_I2C, ENABLE);

    // LCD ekranýný baþlatma
    I2C_LCD_Init();

    // "Hello, LCD!" mesajýný ekrana yazma
    I2C_LCD_SendString("Hello, LCD!");

    while (1) {
    }
}

// I2C LCD ekranýný baþlatma
void I2C_LCD_Init(void) {
    delay_ms(50); // Güçlendirme süresi

    // 4-bit modunda komut gönderme
    I2C_LCD_SendCommand(0x33);
    delay_ms(5);
    I2C_LCD_SendCommand(0x32);
    delay_ms(5);

    // Fonksiyon seti
    I2C_LCD_SendCommand(0x28);
    delay_ms(5);

    // Ekraný temizleme
    I2C_LCD_SendCommand(0x01);
    delay_ms(5);

    // Ekraný etkinleþtirme
    I2C_LCD_SendCommand(0x0C);
    delay_ms(5);
}

// I2C LCD ekranýna komut gönderme
void I2C_LCD_SendCommand(uint8_t command) {
    uint8_t data[4];
    data[0] = (command & 0xF0) | 0x0C; // RS=0, RW=0, EN=1
    data[1] = (command & 0xF0) | 0x08; // RS=0, RW=0, EN=0
    data[2] = ((command & 0x0F) << 4) | 0x0C; // RS=0, RW=0, EN=1
    data[3] = ((command & 0x0F) << 4) | 0x08; // RS=0, RW=0, EN=0

    // I2C ile veri gönderme
    I2C_GenerateSTART(LCD_I2C, ENABLE);
    while (!I2C_CheckEvent(LCD_I2C, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(LCD_I2C, LCD_ADDRESS << 1, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(LCD_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    for (int i = 0; i < 4; i++) {
        I2C_SendData(LCD_I2C, data[i]);
        while (!I2C_CheckEvent(LCD_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }

    I2C_GenerateSTOP(LCD_I2C, ENABLE);
    delay_ms(1);
}

// I2C LCD ekranýna veri gönderme
void I2C_LCD_SendData(uint8_t data) {
    uint8_t d[4];
    d[0] = (data & 0xF0) | 0x0D; // RS=1, RW=0, EN=1
    d[1] = (data & 0xF0) | 0x09; // RS=1, RW=0, EN=0
    d[2] = ((data & 0x0F) << 4) | 0x0D; // RS=1, RW=0, EN=1
    d[3] = ((data & 0x0F) << 4) | 0x09; // RS=1, RW=0, EN=0

    // I2C ile veri gönderme
    I2C_GenerateSTART(LCD_I2C, ENABLE);
    while (!I2C_CheckEvent(LCD_I2C, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(LCD_I2C, LCD_ADDRESS << 1, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(LCD_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    for (int i = 0; i < 4; i++) {
        I2C_SendData(LCD_I2C, d[i]);
        while (!I2C_CheckEvent(LCD_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }

    I2C_GenerateSTOP(LCD_I2C, ENABLE);
    delay_ms(1);
}

// I2C LCD ekranýna string gönderme
void I2C_LCD_SendString(char *str) {
    while (*str) {
        I2C_LCD_SendData(*str++);
    }
}

void delay_ms(int time)
{
	count = time;
	while(count);
}
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}


uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
