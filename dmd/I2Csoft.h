/*
 * I2Csoft.h
 *
 *  Created on: Jul 22, 2022
 *      Author: eltop
 */

#ifndef SRC_I2CSOFT_H_
#define SRC_I2CSOFT_H_

#include "bus_dev.h"
#include "gpio.h"
#include "error.h"

#ifndef UINT8
typedef unsigned char           u8;
#endif
#define I2C_Pin_SCL        GPIO_P24
#define I2C_Pin_SDA        GPIO_P23

#define SCL_H       AP_GPIO->swporta_dr |= BIT(I2C_Pin_SCL)
#define SCL_L       AP_GPIO->swporta_dr &= ~BIT(I2C_Pin_SCL)
#define SDA_H       AP_GPIO->swporta_dr |= BIT(I2C_Pin_SDA)
#define SDA_L       AP_GPIO->swporta_dr &= ~BIT(I2C_Pin_SDA)
#define SCL_read    hal_gpio_read( I2C_Pin_SCL )
#define SDA_read    hal_gpio_read( I2C_Pin_SDA )
#define SCL_out     AP_GPIO->swporta_ddr |= BIT(I2C_Pin_SCL)  //hal_gpio_pin_init(I2C_Pin_SCL, GPIO_OUTPUT)
#define SCL_in      AP_GPIO->swporta_ddr &= ~BIT(I2C_Pin_SCL) //hal_gpio_pin_init(I2C_Pin_SCL, GPIO_INPUT)
#define SDA_out     AP_GPIO->swporta_ddr |= BIT(I2C_Pin_SDA)  //hal_gpio_pin_init(I2C_Pin_SDA, GPIO_OUTPUT)
#define SDA_in      AP_GPIO->swporta_ddr &= ~BIT(I2C_Pin_SDA) //hal_gpio_pin_init(I2C_Pin_SDA, GPIO_INPUT)


void I2CInit(void);
int I2C_7bit_Single_Write(u8 SlaveAddress,u8 REG_data);
u8 I2C_7bit_Single_Read(u8 SlaveAddress);
int I2C_7bit_Mult_Read(u8 SlaveAddress,u8 * ptChar,u8 size);

int I2C_10bit_Single_Write(u8 SlaveAddress,u8 REG_Address,u8 REG_data);
int I2C_10bit_Mult_Write(u8 SlaveAddress,u8 REG_Address,u8 * ptChar,u8 size);
u8 I2C_10bit_Single_Read(u8 SlaveAddress,u8 REG_Address);
int I2C_10bit_Mult_Read(u8 SlaveAddress,u8 REG_Address,u8 * ptChar,u8 size);

void I2C_delaysoft(void);
int I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
int I2C_WaitAck(void);      //返回为:=1有ACK,=0无ACK
void I2C_SendByte(u8 SendByte);
u8 I2C_ReadByte(void);  //数据从高位到低位//

int at24c_Write(u8 SlaveAddress,int addr,u8 * ptChar,u8 size);
int at24c_Read(u8 SlaveAddress, int addr, u8 * ptChar, u8 size);
int at24c_WriteByte(u8 SlaveAddress, int addr, u8 REG_data);
u8 at24c_ReadByte(u8 SlaveAddress, int addr);
u8 bcdToDec(uint8_t val);
u8 decToBcd(uint8_t val) ;

#endif /* SRC_I2CSOFT_H_ */
