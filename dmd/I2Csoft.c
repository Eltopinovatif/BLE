
#include "I2Csoft.h"
#include "i2c.h"
//ģ��IIC��ʼ��

u8 bcdToDec(u8 val) {
// Convert binary coded decimal to normal decimal numbers
	return ( (val/16*10) + (val%16) );
}
u8 decToBcd(u8 val) {
 //Convert normal decimal numbers to binary coded decimal
	return ( (val/10*16) + (val%10) );
}

void I2CInit(void)
{
    SCL_out;
    SDA_out;
}

void I2C_delaysoft(void)
{
  // mDelayuS(100);
	uint8_t i=5;

    while(i)
    {
        i--;
    }
}

int I2C_Start(void)
{
    SDA_H;
    SDA_out;
    SCL_H;
    I2C_delaysoft();
    if(!SDA_read)return 0;    //SDA��Ϊ�͵�ƽ������æ,�˳�
    SDA_L;
    I2C_delaysoft();
    if(SDA_read) return 0;    //SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
    SDA_L;
    I2C_delaysoft();
    return 1;
}

void I2C_Stop(void)
{
    SCL_L;
    I2C_delaysoft();
    SDA_L;
    SDA_out;
    I2C_delaysoft();
    SCL_H;
    I2C_delaysoft();
    SDA_H;
    I2C_delaysoft();
}

void I2C_Ack(void)
{
    SCL_L;
    I2C_delaysoft();
    SDA_L;
    SDA_out;
    I2C_delaysoft();
    SCL_H;
    I2C_delaysoft();
    I2C_delaysoft();
    SCL_L;
    I2C_delaysoft();
}

void I2C_NoAck(void)
{
    SCL_L;
    I2C_delaysoft();
    SDA_H;
    SDA_out;
    I2C_delaysoft();
    SCL_H;
    I2C_delaysoft();
    SCL_L;
    I2C_delaysoft();
}

int I2C_WaitAck(void)      //����Ϊ:=1��ACK,=0��ACK
{
    SCL_L;
    I2C_delaysoft();
    I2C_delaysoft();
    SCL_H;
    SDA_in;
    I2C_delaysoft();
    I2C_delaysoft();
    if(SDA_read)
    {
        SCL_L;
        return 0;
    }
    SCL_L;
    return 1;
}

void I2C_SendByte(u8 SendByte) //���ݴӸ�λ����λ//
{
    u8 i=8;
    SDA_out;
    while(i--)
    {
        SCL_L;
        I2C_delaysoft();
        if(SendByte&0x80)
            SDA_H;
        else
            SDA_L;
        SendByte<<=1;
        I2C_delaysoft();
        SCL_H;
        I2C_delaysoft();
        I2C_delaysoft();
    }
    SCL_L;
}

u8 I2C_ReadByte(void)  //���ݴӸ�λ����λ//
{
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;
    SDA_out;
    while(i--)
    {
        ReceiveByte<<=1;
        SCL_L;
        I2C_delaysoft();
        I2C_delaysoft();
        SCL_H;
        SDA_in;
        I2C_delaysoft();
        I2C_delaysoft();
        if(SDA_read)
        {
            ReceiveByte|=0x01;
        }
    }
    SCL_L;
    return ReceiveByte;
}

//7bit��ַ���ֽ�д��*******************************************
int I2C_7bit_Single_Write(u8 SlaveAddress, u8 REG_data)
{
    if(!I2C_Start())return 0;
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//���ø���ʼ��ַ+������ַ
    if(!I2C_WaitAck()){I2C_Stop(); return 0;}
    I2C_SendByte(REG_data);
    I2C_WaitAck();
    I2C_Stop();
    return 1;
}

//7bit��ַ���ֽڶ�ȡ*****************************************
u8 I2C_7bit_Single_Read(u8 SlaveAddress)
{
    unsigned char REG_data;
    if(!I2C_Start())return 0;
    I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);// Set high start address + device address
    if(!I2C_WaitAck())
    {
        I2C_Stop();
        return 0;
    }
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

    REG_data= I2C_ReadByte();
    I2C_NoAck();
    I2C_Stop();
    //return TRUE;
    return REG_data;

}

//7bit��ַ���ֽڶ�ȡ*****************************************
int I2C_7bit_Mult_Read(u8 SlaveAddress,u8 * ptChar,u8 size)
{
    u8 i;

    if(size < 1)
        return 0;
    if(!I2C_Start())
        return 0;
    I2C_SendByte(SlaveAddress);
    if(!I2C_WaitAck())
    {
        I2C_Stop();
        return 0;
    }
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

    for(i=1;i<size; i++)
    {
        *ptChar++ = I2C_ReadByte();
        I2C_Ack();
    }
    *ptChar++ = I2C_ReadByte();
    I2C_NoAck();
    I2C_Stop();
    return 1;
}



//10bit��ַ���ֽ�д��*******************************************
int I2C_10bit_Single_Write(u8 SlaveAddress,u8 REG_Address,u8 REG_data)
{
    if(!I2C_Start())return 0;
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//���ø���ʼ��ַ+������ַ
    if(!I2C_WaitAck()){I2C_Stop(); return 0;}
    I2C_SendByte(REG_Address );   //���õ���ʼ��ַ
    I2C_WaitAck();
    I2C_SendByte(REG_data);
    I2C_WaitAck();
    I2C_Stop();
    return 1;
}

//10bit��ַ���ֽڶ�ȡ*****************************************
int I2C_10bit_Mult_Write(u8 SlaveAddress,u8 REG_Address,u8 * ptChar,u8 size)
{
    //u8 i;
    if(size < 1)   return 0;
    if(!I2C_Start())   return 0;
    I2C_SendByte(SlaveAddress);
    if(!I2C_WaitAck())   {    I2C_Stop();   return 0;  }

    I2C_SendByte(REG_Address);
    I2C_WaitAck();

    for(u8 i=1;i<size; i++)
    {
    	I2C_SendByte( ptChar[ i ] );
    	 if(!I2C_WaitAck())   {    I2C_Stop();   return 0;  }
    }
    //*ptChar++ = I2C_ReadByte();
  //  I2C_NoAck();
    I2C_Stop();
    return 1;
}

//10bit��ַ���ֽڶ�ȡ*****************************************
u8 I2C_10bit_Single_Read(u8 SlaveAddress,u8 REG_Address)
{
    unsigned char REG_data;
    if(!I2C_Start())return 0;
    I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//���ø���ʼ��ַ+������ַ
    if(!I2C_WaitAck())
    {
        I2C_Stop();
        return 0;
    }
    I2C_SendByte((u8) REG_Address);   //���õ���ʼ��ַ
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

    REG_data= I2C_ReadByte();
    I2C_NoAck();
    I2C_Stop();
    //return TRUE;
    return REG_data;

}

//10bit��ַ���ֽڶ�ȡ*****************************************
int I2C_10bit_Mult_Read(u8 SlaveAddress,u8 REG_Address,u8 * ptChar,u8 size)
{
    u8 i;
    if(size < 1)   return 0;
    if(!I2C_Start())  return 0;
    I2C_SendByte(SlaveAddress);
    if(!I2C_WaitAck())  {  I2C_Stop();  return 0;   }

    I2C_SendByte(REG_Address);
    I2C_WaitAck();

    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

    for(i=1;i<size; i++)
    {
        *ptChar++ = I2C_ReadByte();
        I2C_Ack();
    }
    *ptChar++ = I2C_ReadByte();
    I2C_NoAck();
    I2C_Stop();
    return 1;
}

int at24c_Write(u8 SlaveAddress,int addr,u8 * ptChar,u8 size)
{
	//u8 i;
	    if(size < 1)   return 0;
	    if(!I2C_Start())   return 0;
	    I2C_SendByte(SlaveAddress);
	    if(!I2C_WaitAck())   {    I2C_Stop();   return 0;  }

	    I2C_SendByte( addr>>8  );  I2C_WaitAck();
	    I2C_SendByte( addr  );   I2C_WaitAck();
	    for(u8 i=1;i<size; i++)   {
	    	I2C_SendByte( ptChar[ i ] );
	       	 if(!I2C_WaitAck())   {    I2C_Stop();   return 0;  }
	       }
	       //*ptChar++ = I2C_ReadByte();
	     //  I2C_NoAck();
	       I2C_Stop();
	       return 1;
}

int at24c_Read(u8 SlaveAddress, int addr, u8 * ptChar, u8 size)
{
    u8 i;
    if(size < 1)   return 0;
    if(!I2C_Start())  return 0;
    I2C_SendByte(SlaveAddress);
    if(!I2C_WaitAck())  {  I2C_Stop();  return 0;   }

    I2C_SendByte( addr>>8  );  I2C_WaitAck();
    I2C_SendByte( addr  );  I2C_WaitAck();

    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

    for(i=1;i<size; i++)
    {
        *ptChar++ = I2C_ReadByte();
        I2C_Ack();
    }
    *ptChar++ = I2C_ReadByte();
    I2C_NoAck();
    I2C_Stop();
    return 1;
}

int at24c_WriteByte(u8 SlaveAddress, int addr, u8 REG_data)
{
    if(!I2C_Start())return 0;
    I2C_SendByte(SlaveAddress);
    if(!I2C_WaitAck()){I2C_Stop(); return 0;}
    I2C_SendByte( addr>>8  );
    I2C_WaitAck();
    I2C_SendByte( addr  );
    I2C_WaitAck();
    I2C_SendByte(REG_data);
    I2C_WaitAck();
    I2C_Stop();
    return 1;
}

u8 at24c_ReadByte(u8 SlaveAddress, int addr)
{
    unsigned char REG_data;
    if(!I2C_Start())return 0;
    I2C_SendByte(SlaveAddress);
    if(!I2C_WaitAck())   {  I2C_Stop();  return 0;  }
    I2C_SendByte( addr>>8  );
    I2C_WaitAck();
    I2C_SendByte( addr  );
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

    REG_data= I2C_ReadByte();
    I2C_NoAck();
    I2C_Stop();
    //return TRUE;
    return REG_data;
}
