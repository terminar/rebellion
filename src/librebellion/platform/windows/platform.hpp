// Rebellion
//
// File: platform.hpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020,2021
// License: LGPLv3

#ifndef _PLATFORM_HPP_
#define _PLATFORM_HPP_

#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string>

#include "niproto_pimpl.hpp"

typedef struct _uint8_t_data {
	uint8_t *data;
	long size;
} uint8_t_data;

extern char ldebstr[1024];

#define IPCDEB(...) {\
	FILE *ipcdebfp;\
	char debstr[1024];\
	sprintf(debstr,__VA_ARGS__);\
	if (strcmp(ldebstr,debstr) != 0) {\
 		ipcdebfp = fopen("DEB.log","a");\
 		if (ipcdebfp) {\
 			fwrite(debstr, sizeof(char), strlen(debstr),ipcdebfp);\
		}\
 		strcpy(ldebstr,debstr);\
 		fclose(ipcdebfp);\
 	}\
}

void dumpDataWithLen(const char *prefix, const unsigned char *data, size_t len);
int loopServer(NIIPC *niipc, LPPIPEINST Pipe, HANDLE *hEvent, DWORD dwmili);
std::string GetLastErrorStdStr();

HANDLE createFile(const char *PortName);
bool createServer(LPPIPEINST Pipe, HANDLE *hEvent, const char *PortName);
VOID DisconnectAndReconnect(LPPIPEINST);
BOOL ConnectToNewClient(HANDLE, LPOVERLAPPED);
VOID GetAnswerToRequest(LPPIPEINST, NIIPC *);

uint8_t_data sendMsg(HANDLE hPipe, uint8_t *msg, size_t size);
int pushMsg(HANDLE hPipe, uint8_t *msg, size_t size);
#endif
