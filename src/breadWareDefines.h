// Defines

#define CHIP_A 5
#define CHIP_B 5
#define CHIP_C 5
#define CHIP_D 5
#define CHIP_E 5
#define CHIP_F 5
#define CHIP_G 5
#define CHIP_H 5
#define CHIP_I 5

#define UNCONN_A 6
#define UNCONN_D 6
#define UNCONN_E 6
#define UNCONN_H 6

#define CS_A 7
#define CS_B 7
#define CS_C 7
#define CS_D 7
#define CS_E 7
#define CS_F 7
#define CS_G 7
#define CS_H 7
#define CS_I 7

#define DATA 8
#define RESET 9
#define STROBE 10

#define AX0 11
#define AX1 11
#define AX2 11
#define AX3 11

#define AY0 12
#define AY1 12
#define AY2 12


#define GP_PC0 13    //anything that starts with GP_ goes to
#define GP_PC1 13    //the matrix chip on the control board
#define GP_PC2 13    //for "General Purpose"
#define GP_PC3 13

#define ADC_VDIVO 13    //for sensing voltages (after getting voltage
#define ADC_VDIV1 13    //divided by 5)
#define ADC_BIAS 13     //sets the bias so you can read negative voltages

#define PWR_NEG_EN 14   //senses dual power supply voltage / 5
#define PWR_LDAC 14  //turns on dual power rails for matrix chips
#define PWR_SENSE2 14   //senses power supply voltage / 5
#define PWR_ENABLE2 14  //turns on adjustable power supply


//#define USB_RX 15
//#define USB_TX 16

#define IN_RX 0
#define IN_TX 1

#define OUT_RX 0
#define OUT_TX 1

//#define I2C_SDA 6
//#define I2C_SCL 7

#define mcpAddress         0x2F
#define volatileWiper0     0x00
#define volatileWiper1     0x01
#define nonvolatileWiper0  0x02
#define nonvolatileWiper1  0x03
#define TCON               0x04
#define mcpSTATUS          0x05 
