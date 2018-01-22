//
//  Rp1210Test.cpp
//
//  Copyright © 2016 IDSC Holdings, Inc. All rights reserved.
//

#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <android/log.h>
#include "rp1210_base.h"
#include "com_nexiq_rp1210test_Rp1210Lib.h"
#include <ctype.h>
#include <dlfcn.h>
#include <sys/time.h>
#include <unistd.h>


#define TEST_CIL7R32	(1)
#define TEST_NULN2R32	(2)
#define TEST_NEDL3R32	(3)
#define TEST_CIMR32		(4)
#define TEST_NBLR32		(5)

// Define the device we want to test
#define TEST_DEVICE TEST_NULN2R32

#define MAC_ADDRESS_STR_LEN	(17)
char g_mac_address[MAC_ADDRESS_STR_LEN+1] = {0};

void Main_HexDumpBuf(uint8_t * buf, uint32_t len);
bool SetupAPI(void * dll_handle);
void TestRp1210(void);
void Discovery(void);

typedef void (*Nexiq_SetDataPathProc) (const char * data_path);

RP1210ConnectProc RP1210_ClientConnect = 0;
RP1210DisconnectProc RP1210_ClientDisconnect = 0;
RP1210ReadMessageProc RP1210_ReadMessage = 0;
RP1210SendMessageProc RP1210_SendMessage = 0;
RP1210SendCommandProc RP1210_SendCommand = 0;
RP1210ReadDetailedVersionProc RP1210_ReadDetailedVersion = 0;
RP1210ReadSerialNumberProc RP1210_ReadSerialNumber = 0;
RP1210GetErrorMsgProc RP1210_GetErrorMsg = 0;
RP1210GetLastErrorMsgProc RP1210_GetLastErrorMsg = 0;
RP1210GetHardwareStatusProc RP1210_GetHardwareStatus = 0;
RP1210IoctlProc RP1210_Ioctl = 0;
RP1210ReadVersionProc RP1210_ReadVersion = 0;

bool SetupAPI(void * dll_handle)
{
    __android_log_print(ANDROID_LOG_INFO,"debug_tag","SetupAPI() dlsym on extern \"C\" functions RP1210_xxxx()");
    RP1210_ClientConnect = (RP1210ConnectProc) dlsym(dll_handle,"RP1210_ClientConnect");
    RP1210_ClientDisconnect = (RP1210DisconnectProc) dlsym(dll_handle,"RP1210_ClientDisconnect");
    RP1210_ReadMessage = (RP1210ReadMessageProc)dlsym(dll_handle,"RP1210_ReadMessage");
    RP1210_SendMessage = (RP1210SendMessageProc)dlsym(dll_handle,"RP1210_SendMessage");
    RP1210_SendCommand = (RP1210SendCommandProc)dlsym(dll_handle,"RP1210_SendCommand");
    RP1210_ReadDetailedVersion = (RP1210ReadDetailedVersionProc)dlsym(dll_handle,"RP1210_ReadDetailedVersion");
    RP1210_ReadSerialNumber = (RP1210ReadSerialNumberProc) dlsym(dll_handle,"RP1210_ReadSerialNumber");
    RP1210_GetErrorMsg = (RP1210GetErrorMsgProc)dlsym(dll_handle,"RP1210_GetErrorMsg");
    RP1210_GetLastErrorMsg = (RP1210GetLastErrorMsgProc)dlsym(dll_handle,"RP1210_GetLastErrorMsg");
    RP1210_GetHardwareStatus = (RP1210GetHardwareStatusProc)dlsym(dll_handle,"RP1210_GetHardwareStatus");
    RP1210_Ioctl = (RP1210IoctlProc) dlsym(dll_handle,"RP1210_Ioctl");
    RP1210_ReadVersion = (RP1210ReadVersionProc)dlsym(dll_handle,"RP1210_ReadVersion");

    if((RP1210_ClientConnect == 0) ||
    	(RP1210_ClientDisconnect == 0) ||
		(RP1210_ReadMessage == 0) ||
		(RP1210_SendMessage == 0) ||
		(RP1210_SendCommand == 0) ||
		(RP1210_ReadDetailedVersion == 0) ||
		(RP1210_ReadSerialNumber == 0) ||
		(RP1210_GetErrorMsg == 0) ||
		(RP1210_GetLastErrorMsg == 0) ||
		(RP1210_GetHardwareStatus == 0) ||
		(RP1210_Ioctl == 0) ||
		(RP1210_ReadVersion == 0) )
    {
    	__android_log_print(ANDROID_LOG_INFO,"debug_tag","SetupAPI() failed to resolve RP1210_xxxx() functions");
    	return false;
    }

    return true;
}


void Discovery(void)
{

    short ret_val;
    RP1210_DISCOVERY scan;
    RP1210_DISCOVERY_RESULT *scan_result;

    int alloc_results = 10;

    __android_log_print(ANDROID_LOG_INFO,"debug_tag","Malloc %u bytes",sizeof(unsigned int) + RP1210_DISCOVERY_MAX_ENTRY_LENGTH * alloc_results);

    scan_result = (RP1210_DISCOVERY_RESULT*)malloc(sizeof(unsigned int) + (RP1210_DISCOVERY_MAX_ENTRY_LENGTH * alloc_results));


    if(scan_result == 0)
    {
    	__android_log_print(ANDROID_LOG_INFO,"debug_tag","Could not allocate space for scan results");
        return;
    }

    scan.max_entries = alloc_results;
    scan.scan_interval_sec = 5;

	ret_val = RP1210_Ioctl(0, IOCTL_DISCOVERY, &scan, scan_result);

	char * p_mac = 0;
	// Discovery Records
	for (int k = 0; k < scan_result->num_entries_found; k++)
	{
		char *p = &scan_result->buf[k* RP1210_DISCOVERY_MAX_ENTRY_LENGTH];
		__android_log_print(ANDROID_LOG_INFO,"debug_tag","%d. = %s",k+1, p);
		// find and store first paired Bluetooth device
		if(p_mac == 0)
		{
			if((p_mac = strstr(p,":")) != 0)
			{
				p_mac -= 2;
				memset(g_mac_address, 0, sizeof(g_mac_address));
				strncpy(g_mac_address, p_mac, sizeof(g_mac_address) - 1);
			}
		}
	}
}

/*
 * Class:     com_nexiq_rp1210test_Rp1210Lib
 * Method:    Rp1210Test
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_nexiq_rp1210test_Rp1210Lib_Rp1210Test
  (JNIEnv * pEnv, jobject pThis, jstring str_path)
{
	void * dll_handle;

#if TEST_DEVICE == TEST_CIL7R32
	__android_log_print(ANDROID_LOG_INFO,"debug_tag","dlopen(libcil7r32.so)");
	dll_handle = dlopen("libcil7r32.so",RTLD_GLOBAL|RTLD_NOW);
#elif TEST_DEVICE == TEST_NULN2R32
	__android_log_print(ANDROID_LOG_INFO,"debug_tag","dlopen(libnuln2r32.so)");
	dll_handle = dlopen("libnuln2r32.so",RTLD_GLOBAL|RTLD_NOW);
#elif TEST_DEVICE == TEST_CIMR32
	__android_log_print(ANDROID_LOG_INFO,"debug_tag","dlopen(libcimr32.so)");
	dll_handle = dlopen("libcimr32.so",RTLD_GLOBAL|RTLD_NOW);
#elif TEST_DEVICE == TEST_NBLR32
	__android_log_print(ANDROID_LOG_INFO,"debug_tag","dlopen(libnblr32.so)");
	dll_handle = dlopen("libnblr32.so",RTLD_GLOBAL|RTLD_NOW);
#else
	__android_log_print(ANDROID_LOG_INFO,"debug_tag","dlopen(libnedl3r32.so)");
	dll_handle = dlopen("libnedl3r32.so",RTLD_GLOBAL|RTLD_NOW);
#endif

	// Setup data path so that INI files can be accessed by the drivers
	Nexiq_SetDataPathProc set_data_path_proc = (Nexiq_SetDataPathProc)dlsym(dll_handle,"NexiqSetDataPath");

	// Setup file path and Bluetooth MAC address
	const char* temp_path = pEnv->GetStringUTFChars(str_path, NULL);
	set_data_path_proc(temp_path);
	pEnv->ReleaseStringUTFChars(str_path, temp_path);

	if(SetupAPI(dll_handle) == false)
	{
		return;
	}

	TestRp1210();
}

void TestRp1210(void)
{
	int k;
	short client_id;
	short n_ret;
	char extended_prot_str[100];

	__android_log_print(ANDROID_LOG_INFO,"debug_tag","---Test rp1210 client connect to J1708");

	Discovery();
	if(strlen(g_mac_address) != MAC_ADDRESS_STR_LEN )
	{
		__android_log_print(ANDROID_LOG_INFO,"debug_tag","No Bluetooth MAC address found");
		return;
	}
	else
	{
		__android_log_print(ANDROID_LOG_INFO,"debug_tag","Bluetooth MAC Address = %s", g_mac_address);
	}

	sprintf(extended_prot_str,"J1708:DiscoveryString=%s",g_mac_address);
	n_ret = RP1210_ClientConnect(0, 2, extended_prot_str, (1024 * 1024), (1024 * 1024), 0);

	__android_log_print(ANDROID_LOG_INFO,"debug_tag","---Client connect n_ret = %d", n_ret);

	if(n_ret >= 0 && n_ret < 128)
	{
		client_id = n_ret;
		__android_log_print(ANDROID_LOG_INFO,"debug_tag","-----Send command 3");
		char cmd_3_msg[1] = {0};	// not even used
		n_ret = RP1210_SendCommand(3, client_id, cmd_3_msg, 0);
		__android_log_print(ANDROID_LOG_INFO,"debug_tag","---Sleep for 5 seconds");
		usleep(5000*1000);

		__android_log_print(ANDROID_LOG_INFO,"debug_tag","---Send Messages for 5 seconds");
		char msg[100];
		for(k = 0; k < 100; k++)
			msg[k] = k;
		msg[0] = 7;		// priority byte

		for(k = 0; k < 5; k++)
		{
			unsigned char t_msg[] = {0x00, 0x20, 0x00, 0x06, 0xf9, 0x00, 0x01, 0x02, 0x03, 0x04};
			Main_HexDumpBuf((uint8_t *)t_msg, (uint32_t)10);
			n_ret = RP1210_SendMessage(client_id, (char*)t_msg, 10, 0, 0);
			if(n_ret == RP1210_OK)
				__android_log_print(ANDROID_LOG_INFO,"debug_tag","---Msg sent successfully");
			else
			{
				__android_log_print(ANDROID_LOG_INFO,"debug_tag","---Msg send failure");
				break;
			}
			usleep(1000*1000);
		}

		char buf[1024];

		__android_log_print(ANDROID_LOG_INFO,"debug_tag","-----RP1210_ReadMessage for 5 seconds");

		for(k = 0; k < 50; )
		{
			n_ret = RP1210_ReadMessage(client_id, buf, 1024, 0);
			if(n_ret > 0)
			{
				__android_log_print(ANDROID_LOG_INFO,"debug_tag","---RP1210_ReadMessage %d bytes", n_ret);
				Main_HexDumpBuf((uint8_t*)buf, (uint32_t)n_ret);
			}
			else if(n_ret < 0)
			{
				__android_log_print(ANDROID_LOG_INFO,"debug_tag","---RP1210 ReadMessage error = %d",n_ret);
				break;
			}
			else
			{
				k++;
				usleep(100*1000);
			}
		}
		__android_log_print(ANDROID_LOG_INFO,"debug_tag","---Sleep for 3 seconds");
		usleep(3000*1000);
		__android_log_print(ANDROID_LOG_INFO,"debug_tag","---Close client %d",client_id);

		n_ret = RP1210_ClientDisconnect(client_id);
		if(n_ret == RP1210_OK)
		{
			__android_log_print(ANDROID_LOG_INFO,"debug_tag","---Client %d closed successfully", client_id);
		}
		else
		{
			__android_log_print(ANDROID_LOG_INFO,"debug_tag","---Client %d close returned %d", client_id, n_ret);
		}
	}
	__android_log_print(ANDROID_LOG_INFO,"debug_tag","---End TestRp1210");
}


void Main_HexDumpBuf(uint8_t * buf, uint32_t len)
{
	unsigned int k;
	UCHAR line_buf[120];
	char *p = (char*)line_buf;
	UCHAR n;
	unsigned int line_len = 16;
	unsigned int print_chars = 0;
	int written = 0;

	for(k = 0; k < len; k+= line_len)
	{
		p = (char*)line_buf;
		print_chars = len - k;

		if(print_chars > line_len)
			print_chars = line_len;

		for(n = 0; n < print_chars; n++)
		{
			written = sprintf(p, "%02x ", buf[k + n]);
			p += written;
		}
		for(n = 0; n < line_len - print_chars; n++)
		{
			written = sprintf(p, "   ");
			p += written;
		}
		written = sprintf(p, " | ");
		p += written;
		for(n = 0; n < print_chars; n++)
		{
			if(isprint(buf[k + n]))
			{
				written = sprintf(p, "%c",buf[k + n]);
				p += written;
			}
			else
			{
				written = sprintf(p, ".");
				p += written;
			}
		}
		__android_log_print(ANDROID_LOG_INFO,"debug_tag","%s", (char*)line_buf);
	}
}


