/**************************************************************************/
/* This file contains the defined names for all the registers that        */
/* can be read/written by REG_Level_Read and REG_Level_Write.  The        */
/* following information is provided for each register.                   */
/*                                                                        */
/*                                                                        */
/* Symbolic Name:                                                         */
/*    This is the defined name for the register.  Use this name in your   */
/*    code to help the readability of your program.                       */
/*                                                                        */
/* Value:                                                                 */
/*    The index associated with the given register.                       */
/*                                                                        */
/* Register Name:                                                         */
/*    This is the name of the register as it appears in the user manual   */
/*    for the particular board.                                           */
/*                                                                        */
/* Register Offset:                                                       */
/*    Register offsets are given as an additional cross reference to the  */
/*    user manual for each board.                                         */
/*                                                                        */
/* Register Types:                                                        */
/*    R  - Readable.  REG_Level_Read is supported for this register.      */
/*   W/S - Writeable/Stored.  REG_Level_Write is fully supported for      */
/*         this register.                                                 */
/*    W  - Writeable (Unstored).  REG_Level_Write is supported but the    */
/*         bitsAffected field must be 0xFFFFFFFF (i.e. all bits must      */
/*         be affected).                                                  */
/*                                                                        */
/* Register Size:                                                         */
/*    The size of the register.  Some registers can be accessed as 8-bit  */
/*    or 16-bit registers.  There are two indexes for such registers.  One*/
/*    is for 8-bit reads/writes and the other is for 16-bit reads/writes. */
/*                                                                        */
/**************************************************************************/



/*        Symbolic Name             |  Value     |     Register Name        |  Offset | Type |  Size  */
/*----------------------------------+------------+--------------------------+---------+------+--------*/




/*------------------------*/
/* DEFINES FOR AT-DIO-32F */
/*------------------------*/
#define ATDIO32F_CNFG1_REG                0    /* Config Register 1         |   0x00  | W/S  | 16-bit */
#define ATDIO32F_CNFG2_REG                1    /* Config Register 2         |   0x02  | W/S  | 16-bit */
#define ATDIO32F_CNFG3_REG                2    /* Config Register 3         |   0x04  | W/S  | 16-bit */
#define ATDIO32F_CNFG4_REG                3    /* Config Register 4         |   0x14  | W/S  | 16-bit */
#define ATDIO32F_STAT_REG                 4    /* Status Register           |   0x00  |  R   | 16-bit */
#define ATDIO32F_CNTINTCLR_REG            5    /* CNTINTCLR Register        |   0x0a  |  W   | 16-bit */
#define ATDIO32F_DMACLR1_REG              6    /* DMACLR1 Register          |   0x0c  |  W   | 16-bit */
#define ATDIO32F_DMACLR2_REG              7    /* DMACLR2 Register          |   0x0e  |  W   | 16-bit */
#define ATDIO32F_PORTA_REG                8    /* PORT A Register           |   0x06  | R/W/S|  8-bit */
#define ATDIO32F_PORTB_REG                9    /* PORT B Register           |   0x07  | R/W/S|  8-bit */
#define ATDIO32F_PORTC_REG               10    /* PORT C Register           |   0x08  | R/W/S|  8-bit */
#define ATDIO32F_PORTD_REG               11    /* PORT D Register           |   0x09  | R/W/S|  8-bit */
#define ATDIO32F_PORTA_B_REG             12    /* PORT A Register (16-bit)  |   0x06  | R/W/S| 16-bit */
#define ATDIO32F_PORTC_D_REG             13    /* PORT C Register (16-bit)  |   0x08  | R/W/S| 16-bit */
#define ATDIO32F_RTSI_SHFT_REG           14    /* RTSI Switch Shift Register|   0x10  |  W   |  8-bit */
#define ATDIO32F_RTSI_STRB_REG           15    /* RTSI Switch Strobe Reg    |   0x12  |  W   |  8-bit */
#define ATDIO32F_CNTR1_REG               16    /* CNTR 1 Register           |   0x18  | R/W  |  8-bit */
#define ATDIO32F_CNTR2_REG               17    /* CNTR 2 Register           |   0x1a  | R/W  |  8-bit */
#define ATDIO32F_CNTR3_REG               18    /* CNTR 3 Register           |   0x1c  | R/W  |  8-bit */
#define ATDIO32F_CNTRCMD_REG             19    /* CNTR Command Register     |   0x1e  | W/S  |  8-bit */
#define ATDIO32F_MAX                     19




/*------------------------*/
/* DEFINES FOR AT-MIO-16  */
/*------------------------*/
#define ATMIO16_COMM1_REG                 0    /* Command Register 1        |   0x00  | W/S  | 16-bit */
#define ATMIO16_STATUS_REG                1    /* Status Register           |   0x00  |  R   | 16-bit */
#define ATMIO16_COMM2_REG                 2    /* Command Register 2        |   0x02  | W/S  | 16-bit */
#define ATMIO16_STRT_CNVRT_REG            3    /* Start Convert Register    |   0x08  |  W   | 16-bit */
#define ATMIO16_STRT_DAQ_REG              4    /* Start DAQ Register        |   0x0a  |  W   | 16-bit */
#define ATMIO16_AD_CLR_REG                5    /* A/D Clear Register        |   0x0c  |  W   | 16-bit */
#define ATMIO16_XTRN_STRB_REG             6    /* External Strobe Register  |   0x0e  |  W   | 16-bit */
#define ATMIO16_DAC0_REG                  7    /* DAC0 Register             |   0x10  |  W   | 16-bit */
#define ATMIO16_DAC1_REG                  8    /* DAC1 Register             |   0x12  |  W   | 16-bit */
#define ATMIO16_INT2CLR_REG               9    /* INT2CLR Register          |   0x14  |  W   | 16-bit */
#define ATMIO16_MUX_CNTR_REG             10    /* Mux-Counter Register      |   0x04  |  W   | 16-bit */
#define ATMIO16_MUX_GAIN_REG             11    /* Mux-Gain Register         |   0x06  | W/S  | 16-bit */
#define ATMIO16_AD_FIFO_REG              12    /* A/D FIFO Register         |   0x16  |  R   | 16-bit */
#define ATMIO16_TCINT_CLR_REG            13    /* DMA TC INT Clear Register |   0x16  |  W   | 16-bit */
#define ATMIO16_Am9513A_DATA_REG         14    /* Am9513A Data Register     |   0x18  | R/W  | 16-bit */
#define ATMIO16_Am9513A_COMM_REG         15    /* Am9513A Command Register  |   0x1A  |  W   | 16-bit */
#define ATMIO16_Am9513A_STATUS_REG       16    /* Am9513A Status Register   |   0x1A  |  R   | 16-bit */
#define ATMIO16_DIGIN_REG                17    /* Digital Input Register    |   0x1C  |  R   | 16-bit */
#define ATMIO16_DIGOUT_REG               18    /* Digital Output Register   |   0x1c  | W/S  | 16-bit */
#define ATMIO16_RTSI_SHFT_REG            19    /* RTSI Switch Shift Register|   0x1e  |  W   |  8-bit */
#define ATMIO16_RTSI_STRB_REG            20    /* RTSI Switch Strobe Reg    |   0x1f  |  W   |  8-bit */
#define ATMIO16_MAX                      20




/*------------------------*/
/* DEFINES FOR AT-MIO-16D */
/*------------------------*/
#define ATMIO16D_COMM1_REG                0    /* Command Register 1        |   0x00  | W/S  | 16-bit */
#define ATMIO16D_STATUS_REG               1    /* Status Register           |   0x00  |  R   | 16-bit */
#define ATMIO16D_COMM2_REG                2    /* Command Register 2        |   0x02  | W/S  | 16-bit */
#define ATMIO16D_STRT_CNVRT_REG           3    /* Start Convert Register    |   0x08  |  W   | 16-bit */
#define ATMIO16D_STRT_DAQ_REG             4    /* Start DAQ Register        |   0x0a  |  W   | 16-bit */
#define ATMIO16D_AD_CLR_REG               5    /* A/D Clear Register        |   0x0c  |  W   | 16-bit */
#define ATMIO16D_XTRN_STRB_REG            6    /* External Strobe Register  |   0x0e  |  W   | 16-bit */
#define ATMIO16D_DAC0_REG                 7    /* DAC0 Register             |   0x10  |  W   | 16-bit */
#define ATMIO16D_DAC1_REG                 8    /* DAC1 Register             |   0x12  |  W   | 16-bit */
#define ATMIO16D_INT2CLR_REG              9    /* INT2CLR Register          |   0x14  |  W   | 16-bit */
#define ATMIO16D_MUX_CNTR_REG            10    /* Mux-Counter Register      |   0x04  |  W   | 16-bit */
#define ATMIO16D_MUX_GAIN_REG            11    /* Mux-Gain Register         |   0x06  | W/S  | 16-bit */
#define ATMIO16D_AD_FIFO_REG             12    /* A/D FIFO Register         |   0x16  |  R   | 16-bit */
#define ATMIO16D_TCINT_CLR_REG           13    /* DMA TC INT Clear Register |   0x16  |  W   | 16-bit */
#define ATMIO16D_Am9513A_DATA_REG        14    /* Am9513A Data Register     |   0x18  | R/W  | 16-bit */
#define ATMIO16D_Am9513A_COMM_REG        15    /* Am9513A Command Register  |   0x1A  |  W   | 16-bit */
#define ATMIO16D_Am9513A_STATUS_REG      16    /* Am9513A Status Register   |   0x1A  |  R   | 16-bit */
#define ATMIO16D_DIGIN_REG               17    /* Digital Input Register    |   0x1C  |  R   | 16-bit */
#define ATMIO16D_DIGOUT_REG              18    /* Digital Output Register   |   0x1c  | W/S  | 16-bit */
#define ATMIO16D_RTSI_SHFT_REG           19    /* RTSI Switch Shift Register|   0x1e  |  W   |  8-bit */
#define ATMIO16D_RTSI_STRB_REG           20    /* RTSI Switch Strobe Reg    |   0x1f  |  W   |  8-bit */
#define ATMIO16D_DIO24_PORTA_REG         21    /* DIO-24 PORT A Register    |   0x00  | R/W/S|  8-bit */
#define ATMIO16D_DIO24_PORTB_REG         22    /* DIO-24 PORT B Register    |   0x01  | R/W/S|  8-bit */
#define ATMIO16D_DIO24_PORTC_REG         23    /* DIO-24 PORT C Register    |   0x02  | R/W/S|  8-bit */
#define ATMIO16D_DIO24_CNFG_REG          24    /* DIO-24 Config Register    |   0x03  | W/S  |  8-bit */
#define ATMIO16D_MAX                     24




/*--------------------------*/
/* DEFINES FOR AT-MIO-16F-5 */
/*--------------------------*/
#define ATMIO16F5_COMM1_REG               0    /* Command Register 1        |   0x00  | W/S  | 16-bit */
#define ATMIO16F5_COMM2_REG               1    /* Command Register 2        |   0x02  | W/S  | 16-bit */
#define ATMIO16F5_STATUS_REG              2    /* Status Register           |   0x00  |  R   | 16-bit */
#define ATMIO16F5_STRT_CNVRT_REG          3    /* Start Convert Register    |   0x08  |  W   |  8-bit */
#define ATMIO16F5_STRT_DAQ_REG            4    /* Start DAQ Register        |   0x0a  |  W   |  8-bit */
#define ATMIO16F5_AD_CLR_REG              5    /* A/D Clear Register        |   0x0c  |  W   |  8-bit */
#define ATMIO16F5_XTRN_STRB_REG           6    /* External Strobe Register  |   0x0e  |  W   |  8-bit */
#define ATMIO16F5_TCINT_CLR_REG           7    /* DMA TC INT Clear Register |   0x16  |  W   | 16-bit */
#define ATMIO16F5_DAC0_REG                8    /* DAC0 Register             |   0x10  |  W   | 16-bit */
#define ATMIO16F5_DAC1_REG                9    /* DAC1 Register             |   0x12  |  W   | 16-bit */
#define ATMIO16F5_DAC_INTCLR_REG         10    /* DAC Update INT Clear Reg  |   0x14  |  W   |  8-bit */
#define ATMIO16F5_DAC_UPDT_REG           11    /* DAC Update Register       |   0x0e  |  R   |  8-bit */
#define ATMIO16F5_MUXMEMLD_REG           12    /* MUXMEMLD Register         |   0x04  |  W   |  8-bit */
#define ATMIO16F5_MUXMEMCLR_REG          13    /* MUXMEMCLR Register        |   0x05  |  W   |  8-bit */
#define ATMIO16F5_MUXMEM_REG             14    /* MUXMEM  Register          |   0x06  | W/S  | 16-bit */
#define ATMIO16F5_AD_FIFO_REG            15    /* A/D FIFO Register         |   0x16  |  R   | 16-bit */
#define ATMIO16F5_Am9513A_DATA_REG       16    /* Am9513A Data Register     |   0x18  | R/W  | 16-bit */
#define ATMIO16F5_Am9513A_COMM_REG       17    /* Am9513A Command Register  |   0x1a  |  W   | 16-bit */
#define ATMIO16F5_Am9513A_STATUS_REG     18    /* Am9513A Status Register   |   0x1a  |  R   | 16-bit */
#define ATMIO16F5_DIGIN_REG              19    /* Digital Input Register    |   0x1c  |  R   | 16-bit */
#define ATMIO16F5_DIGOUT_REG             20    /* Digital Output Register   |   0x1c  | W/S  | 16-bit */
#define ATMIO16F5_RTSI_SHFT_REG          21    /* RTSI Switch Shift Register|   0x1e  |  W   |  8-bit */
#define ATMIO16F5_RTSI_STRB_REG          22    /* RTSI Switch Strobe Reg    |   0x1f  |  W   |  8-bit */
#define ATMIO16F5_MAX                    22




/*------------------------*/
/* DEFINES FOR AT-MIO-16X */
/*------------------------*/
#define ATMIO16X_COMM1_REG                0    /* Command Register 1        |   0x00  | W/S  | 16-bit */
#define ATMIO16X_COMM2_REG                1    /* Command Register 2        |   0x02  | W/S  | 16-bit */
#define ATMIO16X_COMM3_REG                2    /* Command Register 3        |   0x04  | W/S  | 16-bit */
#define ATMIO16X_COMM4_REG                3    /* Command Register 4        |   0x06  | W/S  | 16-bit */
#define ATMIO16X_STATUS1_REG              4    /* Status Register 1         |   0x18  |  R   | 16-bit */
#define ATMIO16X_STATUS2_REG              5    /* Status Register 2         |   0x1a  |  R   | 16-bit */
#define ATMIO16X_AD_FIFO_REG              6    /* A/D FIFO Register         |   0x00  |  R   | 16-bit */
#define ATMIO16X_CONFIGMEM_REG            7    /* CONFIGMEM Register        |   0x08  |  W   | 16-bit */
#define ATMIO16X_DAC0_REG                 8    /* DAC0 Register             |   0x10  |  W   | 16-bit */
#define ATMIO16X_DAC1_REG                 9    /* DAC1 Register             |   0x12  |  W   | 16-bit */
#define ATMIO16X_CONFIGMEMCLR_REG        10    /* CONFIGMEMCLR Register     |   0x1b  |  R   |  8-bit */
#define ATMIO16X_CONFIGMEMLD_REG         11    /* CONFIGMEMLD Register      |   0x1b  |  W   |  8-bit */
#define ATMIO16X_DAQ_CLR_REG             12    /* DAQ Clear Register        |   0x19  |  R   |  8-bit */
#define ATMIO16X_DAQ_START_REG           13    /* DAQ Start Register        |   0x1d  |  R   |  8-bit */
#define ATMIO16X_SINGL_CNVRT_REG         14    /* Single Conversion Register|   0x1d  |  W   |  8-bit */
#define ATMIO16X_ADC_CAL_REG             15    /* ADC Calibration Register  |   0x1f  |  W   |  8-bit */
#define ATMIO16X_TMRREQ_CLR_REG          16    /* TMRREQ Clear Register     |   0x1f  |  R   |  8-bit */
#define ATMIO16X_DAC_UPDT_REG            17    /* DAC Update Register       |   0x18  |  W   | 16-bit */
#define ATMIO16X_DAC_CLR_REG             18    /* DAC Clear Register        |   0x1e  |  R   |  8-bit */
#define ATMIO16X_TCA_INT_CLR_REG         19    /* DMA TC A INT Clear Reg    |   0x19  |  W   |  8-bit */
#define ATMIO16X_TCB_INT_CLR_REG         20    /* DMA TC B INT Clear Reg    |   0x09  |  R   |  8-bit */
#define ATMIO16X_XTRN_STRB_REG           21    /* External Strobe Register  |   0x1e  |  W   |  8-bit */
#define ATMIO16X_CAL_DAC0_LD_REG         22    /* Calibration DAC 0 Load Reg|   0x0a  |  W   |  8-bit */
#define ATMIO16X_CAL_DAC1_LD_REG         23    /* Calibration DAC 1 Load Reg|   0x1a  |  W   |  8-bit */
#define ATMIO16X_Am9513A_DATA_REG        24    /* Am9513A Data Register     |   0x14  | R/W  | 16-bit */
#define ATMIO16X_Am9513A_COMM_REG        25    /* Am9513A Command Register  |   0x16  |  W   | 16-bit */
#define ATMIO16X_Am9513A_STATUS_REG      26    /* Am9513A Status Register   |   0x16  |  R   | 16-bit */
#define ATMIO16X_DIGIN_REG               27    /* Digital Input Register    |   0x1c  |  R   | 16-bit */
#define ATMIO16X_DIGOUT_REG              28    /* Digital Output Register   |   0x1c  | W/S  | 16-bit */
#define ATMIO16X_RTSI_SHFT_REG           29    /* RTSI Switch Shift Register|   0x0c  |  W   |  8-bit */
#define ATMIO16X_RTSI_STRB_REG           30    /* RTSI Switch Strobe Reg    |   0x0e  |  W   |  8-bit */
#define ATMIO16X_MAX                     30




/*-------------------------*/
/* DEFINES FOR AT-MIO-64F5 */
/*-------------------------*/
#define ATMIO64F5_COMM1_REG               0    /* Command Register 1        |   0x00  | W/S  | 16-bit */
#define ATMIO64F5_COMM2_REG               1    /* Command Register 2        |   0x02  | W/S  | 16-bit */
#define ATMIO64F5_COMM3_REG               2    /* Command Register 3        |   0x04  | W/S  | 16-bit */
#define ATMIO64F5_COMM4_REG               3    /* Command Register 4        |   0x06  | W/S  | 16-bit */
#define ATMIO64F5_STATUS1_REG             4    /* Status Register 1         |   0x18  |  R   | 16-bit */
#define ATMIO64F5_STATUS2_REG             5    /* Status Register 2         |   0x1a  |  R   | 16-bit */
#define ATMIO64F5_AD_FIFO_REG             6    /* A/D FIFO Register         |   0x00  |  R   | 16-bit */
#define ATMIO64F5_CONFIGMEM_REG           7    /* CONFIGMEM Register        |   0x08  |  W   | 16-bit */
#define ATMIO64F5_DAC0_REG                8    /* DAC0 Register             |   0x10  |  W   | 16-bit */
#define ATMIO64F5_DAC1_REG                9    /* DAC1 Register             |   0x12  |  W   | 16-bit */
#define ATMIO64F5_CONFIGMEMCLR_REG       10    /* CONFIGMEMCLR Register     |   0x1b  |  R   |  8-bit */
#define ATMIO64F5_CONFIGMEMLD_REG        11    /* CONFIGMEMLD Register      |   0x1b  |  W   |  8-bit */
#define ATMIO64F5_DAQ_CLR_REG            12    /* DAQ Clear Register        |   0x19  |  R   |  8-bit */
#define ATMIO64F5_DAQ_START_REG          13    /* DAQ Start Register        |   0x1d  |  R   |  8-bit */
#define ATMIO64F5_SINGL_CNVRT_REG        14    /* Single Conversion Register|   0x1d  |  W   |  8-bit */
#define ATMIO64F5_TMRREQ_CLR_REG         15    /* TMRREQ Clear Register     |   0x1f  |  R   |  8-bit */
#define ATMIO64F5_DAC_UPDT_REG           16    /* DAC Update Register       |   0x18  |  W   | 16-bit */
#define ATMIO64F5_DAC_CLR_REG            17    /* DAC Clear Register        |   0x1e  |  R   |  8-bit */
#define ATMIO64F5_DMA_CHAN_CLR_REG       18    /* DMA Channel Clear Register|   0x0b  |  R   |  8-bit */
#define ATMIO64F5_TCA_INT_CLR_REG        19    /* DMA TC A INT Clear Reg    |   0x19  |  W   |  8-bit */
#define ATMIO64F5_TCB_INT_CLR_REG        20    /* DMA TC B INT Clear Reg    |   0x09  |  R   |  8-bit */
#define ATMIO64F5_XTRN_STRB_REG          21    /* External Strobe Register  |   0x1e  |  W   |  8-bit */
#define ATMIO64F5_CAL_DAC0_LD_REG        22    /* Calibration DAC 0 Load Reg|   0x0a  |  W   |  8-bit */
#define ATMIO64F5_Am9513A_DATA_REG       23    /* Am9513A Data Register     |   0x14  | R/W  | 16-bit */
#define ATMIO64F5_Am9513A_COMM_REG       24    /* Am9513A Command Register  |   0x16  |  W   | 16-bit */
#define ATMIO64F5_Am9513A_STATUS_REG     25    /* Am9513A Status Register   |   0x16  |  R   | 16-bit */
#define ATMIO64F5_DIGIN_REG              26    /* Digital Input Register    |   0x1c  |  R   | 16-bit */
#define ATMIO64F5_DIGOUT_REG             27    /* Digital Output Register   |   0x1c  | W/S  | 16-bit */
#define ATMIO64F5_RTSI_SHFT_REG          28    /* RTSI Switch Shift Register|   0x0c  |  W   |  8-bit */
#define ATMIO64F5_RTSI_STRB_REG          29    /* RTSI Switch Strobe Reg    |   0x0e  |  W   |  8-bit */
#define ATMIO64F5_MAX                    29




/*-----------------------------------------------*/
/* DEFINES FOR Lab-PC, Lab-PC+, and DAQCard-1200 */
/*-----------------------------------------------*/
#define LABPC_COMM1_REG                   0    /* Command Register 1        |   0x00  | W/S  |  8-bit */
#define LABPC_STATUS_REG                  1    /* Status Register           |   0x00  |  R   |  8-bit */
#define LABPC_COMM2_REG                   2    /* Command Register 2        |   0x01  | W/S  |  8-bit */
#define LABPC_COMM3_REG                   3    /* Command Register 3        |   0x02  | W/S  |  8-bit */
#define LABPC_COMM4_REG                   4    /* Command Reg 4 (Lab_PC+ only)| 0x0f  | W/S  |  8-bit */
#define LABPC_AD_FIFO_REG                 5    /* A/D FIFO Register         |   0x0a  |  R   |  8-bit */
#define LABPC_AD_CLR_REG                  6    /* A/D Clear Register        |   0x08  |  W   |  8-bit */
#define LABPC_STRT_CNVRT_REG              7    /* Start Convert Register    |   0x03  |  W   |  8-bit */
#define LABPC_TC_INT_CLR_REG              8    /* DMATC Interrupt Clear Reg |   0x0a  |  W   |  8-bit */
#define LABPC_DAC0_LB_REG                 9    /* DAC0 Low-Byte Register    |   0x04  |  W   |  8-bit */
#define LABPC_DAC0_HB_REG                10    /* DAC0 High-Byte Register   |   0x05  |  W   |  8-bit */
#define LABPC_DAC1_LB_REG                11    /* DAC1 Low-Byte Register    |   0x06  |  W   |  8-bit */
#define LABPC_DAC1_HB_REG                12    /* DAC1 High-Byte Register   |   0x07  |  W   |  8-bit */
#define LABPC_A0_DATA_REG                13    /* Counter A0 Data Register  |   0x14  | R/W  |  8-bit */
#define LABPC_A1_DATA_REG                14    /* Counter A1 Data Register  |   0x15  | R/W  |  8-bit */
#define LABPC_A2_DATA_REG                15    /* Counter A2 Data Register  |   0x16  | R/W  |  8-bit */
#define LABPC_CNTRA_MODE_REG             16    /* Counter A Mode Register   |   0x17  |  W   |  8-bit */
#define LABPC_TMR_INT_CLR_REG            17    /* Timer Interrupt Clear Reg |   0x0c  |  W   |  8-bit */
#define LABPC_B0_DATA_REG                18    /* Counter B0 Data Register  |   0x18  | R/W  |  8-bit */
#define LABPC_B1_DATA_REG                19    /* Counter B1 Data Register  |   0x19  | R/W  |  8-bit */
#define LABPC_B2_DATA_REG                20    /* Counter B2 Data Register  |   0x1a  | R/W  |  8-bit */
#define LABPC_CNTRB_MODE_REG             21    /* Counter A Mode Register   |   0x1b  |  W   |  8-bit */
#define LABPC_PORTA_REG                  22    /* PORT A Register           |   0x10  | R/W/S|  8-bit */
#define LABPC_PORTB_REG                  23    /* PORT B Register           |   0x11  | R/W/S|  8-bit */
#define LABPC_PORTC_REG                  24    /* PORT C Register           |   0x12  | R/W/S|  8-bit */
#define LABPC_DIG_CNTRL_REG              25    /* Digital Control Register  |   0x13  | W/S  |  8-bit */
#define LABPC_MAX                        25




/*-----------------------*/
/* DEFINES FOR PC-DIO-24 */
/*-----------------------*/
#define PCDIO24_PORTA_REG                 0    /* PORT A Register           |   0x00  | R/W/S|  8-bit */
#define PCDIO24_PORTB_REG                 1    /* PORT B Register           |   0x01  | R/W/S|  8-bit */
#define PCDIO24_PORTC_REG                 2    /* PORT C Register           |   0x02  | R/W/S|  8-bit */
#define PCDIO24_CNFG_REG                  3    /* CNFG Register             |   0x03  | W/S  |  8-bit */
#define PCDIO24_MAX                       3




/*---------------------------*/
/* DEFINES FOR DAQCARD-DIO24 */
/*---------------------------*/
#define DAQCARDDIO24_PORTA_REG            0    /* PORT A Register           |   0x00  | R/W/S|  8-bit */
#define DAQCARDDIO24_PORTB_REG            1    /* PORT B Register           |   0x01  | R/W/S|  8-bit */
#define DAQCARDDIO24_PORTC_REG            2    /* PORT C Register           |   0x02  | R/W/S|  8-bit */
#define DAQCARDDIO24_CNFG_REG             3    /* CNFG Register             |   0x03  | W/S  |  8-bit */
#define DAQCARDDIO24_INT_CNTRL1_REG       4    /* Interrupt Control Reg 1   |   0x14  | W/S  |  8-bit */
#define DAQCARDDIO24_INT_CNTRL2_REG       5    /* Interrupt Control Reg 2   |   0x15  | W/S  |  8-bit */
#define DAQCARDDIO24_MAX                  5




/*---------------------------------------*/
/* DEFINES FOR PC-DIO-96/PnP, PCI-DIO-96 */
/*---------------------------------------*/
#define PCDIO96_PPIA_PORTA_REG            0    /* 8255A PPI A PORT A Reg    |   0x00  | R/W/S|  8-bit */
#define PCDIO96_PPIA_PORTB_REG            1    /* 8255A PPI A PORT B Reg    |   0x01  | R/W/S|  8-bit */
#define PCDIO96_PPIA_PORTC_REG            2    /* 8255A PPI A PORT C Reg    |   0x02  | R/W/S|  8-bit */
#define PCDIO96_PPIA_CNFG_REG             3    /* 8255A PPI A CNFG Register |   0x03  | W/S  |  8-bit */
#define PCDIO96_PPIB_PORTA_REG            4    /* 8255A PPI B PORT A Reg    |   0x04  | R/W/S|  8-bit */
#define PCDIO96_PPIB_PORTB_REG            5    /* 8255A PPI B PORT B Reg    |   0x05  | R/W/S|  8-bit */
#define PCDIO96_PPIB_PORTC_REG            6    /* 8255A PPI B PORT C Reg    |   0x06  | R/W/S|  8-bit */
#define PCDIO96_PPIB_CNFG_REG             7    /* 8255A PPI B CNFG Register |   0x07  | W/S  |  8-bit */
#define PCDIO96_PPIC_PORTA_REG            8    /* 8255A PPI C PORT A Reg    |   0x08  | R/W/S|  8-bit */
#define PCDIO96_PPIC_PORTB_REG            9    /* 8255A PPI C PORT B Reg    |   0x09  | R/W/S|  8-bit */
#define PCDIO96_PPIC_PORTC_REG           10    /* 8255A PPI C PORT C Reg    |   0x0a  | R/W/S|  8-bit */
#define PCDIO96_PPIC_CNFG_REG            11    /* 8255A PPI C CNFG Register |   0x0b  | W/S  |  8-bit */
#define PCDIO96_PPID_PORTA_REG           12    /* 8255A PPI D PORT A Reg    |   0x0c  | R/W/S|  8-bit */
#define PCDIO96_PPID_PORTB_REG           13    /* 8255A PPI D PORT B Reg    |   0x0d  | R/W/S|  8-bit */
#define PCDIO96_PPID_PORTC_REG           14    /* 8255A PPI D PORT C Reg    |   0x0e  | R/W/S|  8-bit */
#define PCDIO96_PPID_CNFG_REG            15    /* 8255A PPI D CNFG Register |   0x0f  | W/S  |  8-bit */
#define PCDIO96_8253_PORTA_REG           16    /* 8253 PORT A Register      |   0x10  | R/W  |  8-bit */
#define PCDIO96_8253_PORTB_REG           17    /* 8253 PORT B Register      |   0x11  | R/W  |  8-bit */
#define PCDIO96_8253_PORTC_REG           18    /* 8253 PORT C Register      |   0x12  | R/W  |  8-bit */
#define PCDIO96_8253_CNFG_REG            19    /* 8253 CNFG Register        |   0x13  | W/S  |  8-bit */
#define PCDIO96_INT_CNTRL1_REG           20    /* Interrupt Control Reg 1   |   0x14  | W/S  |  8-bit */
#define PCDIO96_INT_CNTRL2_REG           21    /* Interrupt Control Reg 2   |   0x15  | W/S  |  8-bit */
#define PCDIO96_MAX                      21
/*  (The following two constants are for the PCI-DIO-96 only)  */
#define PCIDIO96_INT_CLEAR_REG           22    /* Interrupt Clear Register  |   0x16  |  W   |  8-bit */
#define PCIDIO96_MAX                     22




/*-------------------------*/
/* DEFINES FOR PC-LPM-16   */
/*-------------------------*/
#define PCLPM16_COMM1_REG                 0    /* Command Register 1        |   0x00  | W/S  |  8-bit */
#define PCLPM16_STATUS_REG                1    /* Status Register           |   0x00  |  R   |  8-bit */
#define PCLPM16_COMM2_REG                 2    /* Command Register 2        |   0x07  | R/W/S|  8-bit */
#define PCLPM16_AD_FIFO_LB_REG            3    /* A/D FIFO Low-Byte Register|   0x02  |  R   |  8-bit */
#define PCLPM16_AD_FIFO_HB_REG            4    /* A/D FIFO High-Byte Reg    |   0x03  |  R   |  8-bit */
#define PCLPM16_AD_CLR_REG                5    /* A/D Clear Register        |   0x01  |  W   |  8-bit */
#define PCLPM16_CNTR0_DATA_REG            6    /* Counter 0 Data Register   |   0x08  | R/W  |  8-bit */
#define PCLPM16_CNTR1_DATA_REG            7    /* Counter 1 Data Register   |   0x09  | R/W  |  8-bit */
#define PCLPM16_CNTR2_DATA_REG            8    /* Counter 2 Data Register   |   0x0a  | R/W  |  8-bit */
#define PCLPM16_CNTR_MODE_REG             9    /* Counter Mode Register     |   0x0b  |  W   |  8-bit */
#define PCLPM16_TMR_INT_CLR_REG          10    /* Timer Interrupt Clear Reg |   0x06  |  W   |  8-bit */
#define PCLPM16_DIGOUT_REG               11    /* Digital Output Register   |   0x04  | W/S  |  8-bit */
#define PCLPM16_DIGIN_REG                12    /* Digital Input Register    |   0x05  |  R   |  8-bit */
#define PCLPM16_MAX                      12
