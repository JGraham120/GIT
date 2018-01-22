//
//  rp1210_base.hpp
//
//  Copyright © 2016 IDSC Holdings, Inc. All rights reserved.
//

#ifndef rp1210_base_h
#define rp1210_base_h

/****************************************************************************
 NAME:		RP1210.h
 
 VERSION:		1.0
 
 CONTENTS:   	Client definitions for NEXIQ RP1210B implementations
 
 NOTES:
 
 Modification:
 9/27/00       jkuntz
 Initial version, Created based on WIN32 RP1210.h
 2/20/06	M. Roache
 - Add ISO15765 fields to RP1210SystemStatus struct
 5/17/06	M. Roache
 - Add RP1210B defines
 7/14/06	M. Roache
 - Add support for RP1210_ReadDetailedVersion
 10/25/06	M. Roache
 - Add support for RP1210_ReadSerialNumber
 11/05/08	M. Roache
 - Add support for RP1210_Ioctl
 ****************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif
    
    
//--------------------------------------------------------------
// RP1210A/RP1210B   RP1210_SendCommand Defines
//--------------------------------------------------------------
#define RP1210_Reset_Device                            0
#define RP1210_Set_All_Filters_States_to_Pass          3
#define RP1210_Set_Message_Filtering_For_J1939         4
#define RP1210_Set_Message_Filtering_For_CAN           5
#define RP1210_Set_Message_Filtering_For_J1708         7
#define RP1210_Set_Message_Filtering_For_J1850         8
#define RP1210_Set_Message_Filtering_For_ISO15765      9
#define RP1210_Generic_Driver_Command                 14
#define RP1210_Set_J1708_Mode                         15
#define RP1210_Echo_Transmitted_Messages              16
#define RP1210_Set_All_Filters_States_to_Discard      17
#define RP1210_Set_Message_Receive                    18
#define RP1210_Protect_J1939_Address                  19
#define RP1210_Set_Broadcast_For_J1708                20
#define RP1210_Set_Broadcast_For_CAN                  21
#define RP1210_Set_Broadcast_For_J1939                22
#define RP1210_Set_Broadcast_For_J1850                23
#define RP1210_Set_J1708_Filter_Type                  24
#define RP1210_Set_J1939_Filter_Type                  25
#define RP1210_Set_CAN_Filter_Type                    26
#define RP1210_Set_J1939_Interpacket_Time             27
#define RP1210_SetMaxErrorMsgSize                     28
#define RP1210_Disallow_Further_Connections           29
#define RP1210_Set_J1850_Filter_Type                  30
#define RP1210_Release_J1939_Address                  31
#define RP1210_Set_ISO15765_Filter_Type               32
#define RP1210_Set_Broadcast_For_ISO15765             33
#define RP1210_Set_ISO15765_Flow_Control              34
#define RP1210_Clear_ISO15765_Flow_Control            35
#define RP1210_Set_ISO15765_Link_Type				  36
#define RP1210_Set_J1939_Baud                         37
#define RP1210_Set_ISO15765_Baud                      38
#define RP1210_Flush_Tx_Rx_Buffers                    39
#define RP1210_Set_Broadcast_For_KWP2000              41
#define RP1210_Set_Broadcast_For_ISO9141              42
#define	RP1210_Get_Protocol_Connection_Speed          45
#define RP1210_Set_ISO9141KWP2000_Mode                46
#define RP1210_Set_CAN_Baud                           47
#define RP1210_Get_Wireless_State                     48
#define RP1210_Set_BlockTimeout                      215
#define RP1210_Set_J1708_Baud                        305
    
//--------------------------------------------------------------
// NEXIQ Proprietary Command Support
//--------------------------------------------------------------
#define GET_PROD_TEST_RESULT						0xE1	//225
#define SET_HEARTBEAT_CHECK_MODE					0xE4	//228
#define PROD_TEST_RESULT_LEN						0x03	//CMD, SUBTEST, RESULT
    
//--------------------------------------------------------------
// RP1210C Constants
//--------------------------------------------------------------
#define CONNECTED_WIRED                                0
#define CONNECTED_WIRELESS                             1

//--------------------------------------------------------------
// RP1210B Constants
//--------------------------------------------------------------
#define	MAX_PROTOCOL_SPEED_LEN                       17  // RP1210B string length
    
#define NULL_WINDOW                                   0  // Windows 3.1 is no longer supported.
    
#define BLOCKING_IO                                   1  // For Blocking calls to send/read.
#define NON_BLOCKING_IO                               0  // For Non-Blocking calls to send/read.
    
#define CONVERTED_MODE                                1  // RP1210Mode="Converted"
#define RAW_MODE                                      0  // RP1210Mode="Raw"
    
#define MAX_J1708_MESSAGE_LENGTH                    508  // Maximum size a J1708 message can be (+1)
#define MAX_J1939_MESSAGE_LENGTH                   1796  // Maximum size a J1939 message can be (+1)
#define MAX_ISO15765_MESSAGE_LENGTH                4108  // Maximum size an ISO15765 message can be (+1)
    
#define ECHO_OFF                                   0x00  // EchoMode
#define ECHO_ON                                    0x01  // EchoMode
    
#define RECEIVE_ON                                 0x01  // Set Message Receive
#define RECEIVE_OFF                                0x00  // Set Message Receive
    
#define ADD_LIST                                   0x01  // Add a message to the list.
#define VIEW_B_LIST                                0x02  // View an entry in the list.
#define DESTROY_LIST                               0x03  // Remove all entries in the list.
#define REMOVE_ENTRY                               0x04  // Remove a specific entry from the list.
#define LIST_LENGTH                                0x05  // Returns number of items in list.
#define	MAX_BROADCAST_ENTRIES                        32  // RP1210B broadcast list limit
#define	BCAST_ADD_MIN_LEN                             6
#define	BCAST_VIEW_MIN_LEN                            6
#define	BCAST_DESTROY_LEN                             2	//3
#define BCAST_REMOVE_LEN                              3
#define	BCAST_LIST_LEN                                2	//3
#define	BCAST_FUNCTION_INX                            0
#define	BCAST_RESPONSE_INX                            1
#define	BCAST_DATA_INX                                2
#define BCAST_MSG_INTERVAL_INX                        BCAST_DATA_INX
#define	BCAST_MSG_LEN_INX                             (BCAST_DATA_INX+2)
#define	BCAST_MSG_INX                                 (BCAST_DATA_INX+4)
#define BCAST_MSG_ENTRY_INX                           BCAST_DATA_INX
    
    
#define	ADD_BROADCAST_ENTRY_FAIL                   0xFF
#define VALID_BROADCAST_LIST_ENTRY                 0x00
#define NO_BROADCAST_LIST_ENTRY                    0x01
#define	REMOVE_BROADCAST_ENTRY_OK                  0x00
#define REMOVE_BROADCAST_ENTRY_FAIL                0x01
    
#define FILTER_PGN                           0x00000001  // Setting of J1939 filters
#define FILTER_PRIORITY                      0x00000002  // Setting of J1939 filters
#define FILTER_SOURCE                        0x00000004  // Setting of J1939 filters
#define FILTER_DESTINATION                   0x00000008  // Setting of J1939 filters
#define FILTER_INCLUSIVE                           0x00  // FilterMode
#define FILTER_EXCLUSIVE                           0x01  // FilterMode
    
#define SILENT_J1939_CLAIM                         0x00  // Claim J1939 Address
#define PASS_J1939_CLAIM_MESSAGES                  0x01  // Claim J1939 Address
#define	RP1210_J1939_BAM_MASK                      0x80  // RP1210 send/receive bitmask for BAM(set) or RTS(clear) long transfer
#define	RP1210_J1939_MAX_SF                        14    // RP1210 format:  PGNx3, How/Priority, SA, DA, Datax8
#define PROPRIETARY_CAN_MASK                       0x40  // Indicator of CAN ID being used for internal NX commands
    
#define CHANGE_BAUD_NOW                            0x00  // Change Baud
#define MSG_FIRST_CHANGE_BAUD                      0x01  // Change Baud
#define RP1210_BAUD_9600                           0x00  // Change Baud
#define RP1210_BAUD_19200                          0x01  // Change Baud
#define RP1210_BAUD_38400                          0x02  // Change Baud
#define RP1210_BAUD_57600                          0x03  // Change Baud
#define RP1210_BAUD_125k                           0x04  // Change Baud
#define RP1210_BAUD_250k                           0x05  // Change Baud
#define RP1210_BAUD_500k                           0x06  // Change Baud
#define RP1210_BAUD_1000k                          0x07  // Change Baud
    
#define	RP1210_NO_FRAME_PAD                        0x80  // Proprietary Tx indicator in Message Type
#define STANDARD_CAN                               0x00  // Filters
#define EXTENDED_CAN                               0x01  // Filters
#define STANDARD_CAN_ISO15765_EXTENDED             0x02  // 11-bit with ISO15765 extended address
#define EXTENDED_CAN_ISO15765_EXTENDED             0x03  // 29-bit with ISO15765 extended address
#define	STANDARD_MIXED_CAN_ISO15765                0x04  // 11-bit identifier with mixed addressing
#define ISO15765_ACTUAL_MESSAGE                    0x00  // ISO15765 ReadMessage - type of data
#define ISO15765_CONFIRM                           0x01  // ISO15765 ReadMessage - type of data
#define ISO15765_FF_INDICATION                     0x02  // ISO15765 ReadMessage - type of data
#define ISO15765_RX_ERROR_INDICATION               0x03  // RP1210C addition - ISO15765 ReadMessage error
#define	ISO15765_3_BITMASK						   0x03  // Filter mask for CAN ID
    
//Hardware Status Fields
#define	HARDWARE_LOCATED							0x01
#define HARDWARE_INTERNAL							0x02
#define HARDWARE_EXTERNAL							0x04
#define J1939_LINK_ACTIVE							0x01
#define J1939_TRAFFIC_DETECT						0x02
#define	J1939_CAN_BUS_OFF							0x04
#define J1708_LINK_ACTIVE							0x01
#define J1708_TRAFFIC_DETECT						0x02
#define	CAN_LINK_ACTIVE								0x01
#define	CAN_TRAFFIC_DETECT							0x02
#define	CAN_BUS_OFF									0x04
#define	J1850_LINK_ACTIVE							0x01
#define J1850_TRAFFIC_DETECT						0x02
#define ISO15765_LINK_ACTIVE						0x01
#define	ISO15765_TRAFFIC_DETECT						0x02
#define	ISO15765_CAN_BUS_OFF						0x04
#define	MAX_HARDWARE_STATUS_SIZE					0x40
    
#define IOCTL_DISCOVERY				(0x20003)

#ifndef TRUE
#define TRUE                                       0x01
#define FALSE                                      0x00
#endif
    
#ifndef RP1210_OK
#define RP1210_OK (0)
#endif

#define RP1210_VALID_CLIENTID(n)		((n >= 0) && (n < 128))
    
/* J1939 Filter types				*/
#define FILTER_J1939_ALL		(FILTER_PGN | FILTER_PRIORITY | FILTER_SOURCE | FILTER_DESTINATION)
    

#ifndef _WIN32
    
#define PACKED __attribute__ ((packed, aligned (1)))
#define WINAPI
#else
    
#define PACKED
#pragma pack(push)
#pragma pack(1)
    
#endif
    
typedef unsigned char UCHAR;
typedef unsigned int HANDLE;

struct Rp1210SystemStatus
{
	UCHAR					m_nHWStatus;
	UCHAR					m_nCount;
	UCHAR					m_nJ1939Status;
	UCHAR					m_nJ1939Count;
	UCHAR					m_nJ1708Status;
	UCHAR					m_nJ1708Count;
	UCHAR					m_nCANStatus;
	UCHAR					m_nCANCount;
	UCHAR					m_nJ1850Status;
	UCHAR					m_nJ1850Count;
	UCHAR					m_nOther1Status;
	UCHAR					m_nOther1Count;
	UCHAR					m_nOther2Status;
	UCHAR					m_nOther2Count;
	UCHAR					m_nOther3Status;
	UCHAR					m_nOther3Count;
	UCHAR					m_nISO15765Status;
	UCHAR					m_nISO15765Count;
	UCHAR					m_nISO9141Status;
	UCHAR					m_nISO9141Count;
	UCHAR					m_nKWP2000Status;
	UCHAR					m_nKWP2000Count;
	UCHAR					m_nALDLStatus;
	UCHAR					m_nALDLCount;
}PACKED;
typedef struct Rp1210SystemStatus RP1210_SYSTEM_STATUS;

    
    
#define RP1210_DISCOVERY_MAX_ENTRY_LENGTH  (256)
struct Rp1210Discovery
{
    unsigned int    scan_interval_sec;
    unsigned int    max_entries;
} PACKED;
typedef struct Rp1210Discovery RP1210_DISCOVERY;

struct RP1210DiscoveryResult
{
    unsigned int    num_entries_found;
    char            buf[0];        // allocated buffer start
} PACKED;
typedef struct RP1210DiscoveryResult RP1210_DISCOVERY_RESULT;



// function pointers to rp1210 functions
/* connect  */
typedef short (WINAPI *RP1210ConnectProc)
(
 HANDLE hMsgQClient,
 short nDeviceID,
 char *fpchProtocol,
 long lSendBuffer,
 long lReceiveBuffer,
 short nIsAppPacketizingIncomingMsgs
 );

typedef short (WINAPI *RP1210DisconnectProc)
(
 short nClientID
 );

typedef short (WINAPI *RP1210ReadMessageProc)
(
 short nClientID,
 char  *fpchAPIMessage,
 short nBufferSize,
 short nBlockOnRead
 );

typedef short (WINAPI *RP1210SendMessageProc)
(
 short nClientID,
 char  *fpchClientMessage,
 short nMessageSize,
 short nNotifyStatusOnTx,
 short nBlockOnSend
 );

typedef short (WINAPI *RP1210SendCommandProc)
(
 short nCommandNumber,
 short nClientID,
 char  *fpchClientCommand,
 short nMessageSize
 );

typedef short (WINAPI *RP1210ReadDetailedVersionProc)
(
 short nClientID,
 char *fpchAPIVersionInfo,
 char *fpchDLLVersionInfo,
 char *fpchFWVersionInfo
 );

typedef short (WINAPI *RP1210ReadSerialNumberProc)
(
 short nClientID,
 char *fpchSerialNumber
 );

typedef short (WINAPI *RP1210GetErrorMsgProc)
(
 short nErrorCode,
 char  *fpchDescription
 );

typedef short (WINAPI *RP1210GetLastErrorMsgProc)
(
 short nErrorCode,
 int  *subErrorCode,
 char  *fpchDescription,
 short nClientID
 );

typedef short (WINAPI *RP1210GetHardwareStatusProc)
(
 short nClientID,
 char  *fpchClientInfo,
 short nInfoSize,
 short nBlockOnRequest
 );

typedef short (WINAPI *RP1210IoctlProc)
(
 short			nClientID,
 unsigned long	nIoctlID,
 void			*pInput,
 void			*pOutput
 );

typedef void (WINAPI *RP1210ReadVersionProc)
(
	char *fpchDLLMajorVersion,
	char *fpchDLLMinorVersion,
	char *fpchAPIMajorVersion,
	char *fpchAPIMinorVersion
 );

typedef void (WINAPI *SetIniPathProc)
(
const char * str
);
    
    
    
#ifdef __cplusplus
}
#endif

#ifdef _WIN32
#pragma pack(pop)
#endif


#endif /* rp1210_base_h */
