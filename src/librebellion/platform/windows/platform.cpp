// Rebellion
//
// File: platform.cpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
// License: LGPLv3

#include <windows.h>

#include <tchar.h>
#include <strsafe.h>

#include <iostream>
#include <string>
#include <map>

#include "niproto_pimpl.hpp"
#include "platform.hpp"

char ldebstr[1024];

void dumpDataWithLen(const char *prefix, const unsigned char *data, size_t len) {
	if (len > 5000) {
		printf("\t\t--- %s --------------- %li\n\t\t", prefix, (long) len);
		printf("data len is > 5000, output omitted\n");
	} else {
		printf("\t\t--- %s --------------- %li\n\t\t", prefix, (long) len);
		for (int i = 0; i < len; i++) {
			printf("0x%02x[%c] ", data[i], (data[i] > 31 ? data[i] : ' '));
			if (i > 0 && (i + 1) % 4 == 0) {
				printf("\t");
			}
			if (i > 0 && (i + 1) % 8 == 0) {
				printf("\n\t\t");
			}
		}
		printf("\n\t\t=============================\n\n");
	}
}

void dumpData(const char *prefix, const unsigned char *data) {
	dumpDataWithLen(prefix, data, sizeof(data));
}

void ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}

HANDLE createFile(const char *PortName) {
	HANDLE hCreateFile;
	try {
		hCreateFile = CreateFileA(
			PortName,
			GENERIC_READ | GENERIC_WRITE,
			0 /*FILE_SHARE_READ | FILE_SHARE_WRITE*/,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (hCreateFile == INVALID_HANDLE_VALUE) {
			std::cerr << "createFile> ERROR: Port creation failed: " << PortName << std::endl;
			std::cerr << "createFile> " << GetLastErrorStdStr() << std::endl;

			//ERROR_SHARING_VIOLATION
			//DWORD error = GetLastError();
			//if (error) {}

			// Exit if an error other than ERROR_PIPE_BUSY occurs. 
		/*
			if (GetLastError() != ERROR_PIPE_BUSY) 
			{
				std::cerr << "createFile> ERROR: Could not open pipe. GLE=" << GetLastError() << std::endl; 
				return 0;
			}
		
			
			// All pipe instances are busy, so wait for 20 seconds. 
			if ( ! WaitNamedPipeA(PortName, 20000)) 
			{ 
				std::cerr << "createFile> ERROR: Could not open pipe: 20 second wait timed out." << std::endl; 
				return 0;
			} 			

		*/
		}


	}
	catch (std::exception& e) {
		std::cout << "Error on CreateFile: " << e.what() << std::endl;
		return 0;
	}
	return hCreateFile;
}

BOOL readFromFile(HANDLE& hCreateFile, char szReadFileBuffer[], 
				DWORD dwReadFileBufferSize, 
				DWORD& dwNoBytesReadForReading) {
	BOOL bReadFile;
	try {
		bReadFile = ReadFile(
			hCreateFile,
			szReadFileBuffer,
			dwReadFileBufferSize,
			&dwNoBytesReadForReading,
			NULL
		);
		//std::cout << "Read from pipe: " << szReadFileBuffer << std::endl;
	}
	catch (std::exception& e) {
		bReadFile = FALSE;
		std::cout << "Error on ReadFile: " << e.what() << std::endl;
	}
	return bReadFile;
}

BOOL writeToFile(HANDLE& hCreateFile, char szWriteFileBuffer [], 
				DWORD dwWriteFileBufferSize, 
				DWORD& dwNoBytesReadForWriting) {
	BOOL bWriteFile;
	try {
		bWriteFile = WriteFile(
			hCreateFile,
			szWriteFileBuffer,
			dwWriteFileBufferSize,
			&dwNoBytesReadForWriting,
			NULL
		);
	}
	catch (std::exception& e) {
		bWriteFile = FALSE;
		std::cout << "Error on WriteFile: " << e.what() << std::endl;
	}

	if (bWriteFile == FALSE) {
		ErrorExit((LPTSTR)"writeToFile");
	}

	return bWriteFile;
}

//-----
// Create a string with last error message
std::string GetLastErrorStdStr()
{
	DWORD error = GetLastError();
	if (error)
	{
		LPVOID lpMsgBuf;
		DWORD bufLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);
		if (bufLen)
		{
			LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
			std::string result(lpMsgStr, lpMsgStr + bufLen);

			LocalFree(lpMsgBuf);

			return result;
		}
	}
	return std::string();
}

/*
int
waitForPipe(char *PortName, DWORD timeout)
{
	//BOOL fSuccess;
	//DWORD dwMode;
	int ret = 0;

	printf("=== Portname: %s ===\n", PortName);

	return WaitNamedPipeA(PortName, timeout);
}
*/

int
pushMsg(HANDLE hPipe, uint8_t *msg, size_t size)
{
	//BOOL fSuccess;
	//DWORD dwMode;
	int ret = 0;

	BOOL bWriteFile;
	DWORD dwNoBytesReadForWriting;

	dumpDataWithLen("pushMsg >>>>>:", (unsigned char*)msg, size);
	bWriteFile = writeToFile(hPipe, (char*)msg /*szWriteFileBuffer*/, 
						(DWORD) size /*dwWriteFileBufferSize*/, 
						dwNoBytesReadForWriting);
	if (bWriteFile == FALSE) {
		std::cout << "Writefile failed" << std::endl;
		return ret;
	}

	return dwNoBytesReadForWriting;
}

uint8_t_data
sendMsg(HANDLE hPipe, uint8_t *msg, size_t size)
{
	uint8_t_data ret = { NULL, 0 };
	BOOL fSuccess;
	//BOOL bReadFile;
	DWORD dwNoBytesReadForReading;//, dwMode;
//TODO: BUFFER IS TOO SMALL! WHAT IF BIGGER SIZE WILL BE RECEIVED?	
	char szReadFileBuffer[1023];
	DWORD dwReadFileBufferSize = sizeof(szReadFileBuffer);

	dumpDataWithLen("sendMsg >>>>>:", (unsigned char*)msg, size);

	DWORD lErr = 0;
	int cnt=0;

	/*
	https://docs.microsoft.com/en-us/windows/win32/api/namedpipeapi/nf-namedpipeapi-transactnamedpipe
	If the message to be read is longer than the buffer specified by the nOutBufferSize parameter, 
	TransactNamedPipe returns FALSE and the GetLastError function returns ERROR_MORE_DATA. 
	The remainder of the message can be read by a subsequent call to ReadFile, ReadFileEx, or PeekNamedPipe.
	*/

	do {
		fSuccess = TransactNamedPipe(
			hPipe,
			msg,
			(DWORD)size,
			szReadFileBuffer,
			dwReadFileBufferSize,
			&dwNoBytesReadForReading,
			NULL
		);
		lErr = GetLastError();
		if (lErr == ERROR_PIPE_BUSY) {
			Sleep(100);
		}
		cnt++;
	} while (lErr == ERROR_PIPE_BUSY && cnt < 10);

	if (cnt >= 10) {
		std::cout << "sendMsg: ERROR> TransactNamedPipe transfer try count too big" << std::endl;
	}

	if (!fSuccess) {
		if (lErr == ERROR_PIPE_BUSY) { //all pipe instances are busy
			std::cout << "sendMsg: ERROR> ALL PIPE INSTANCES ARE BUSY (" << cnt << ")" << std::endl;
		} else if (lErr == ERROR_NO_DATA) { //pipe is being closed
			std::cout << "sendMsg: ERROR> PIPE IS BEING CLOSED" << std::endl;
		} else if (lErr == ERROR_PIPE_NOT_CONNECTED) {
			std::cout << "sendMsg: ERROR> PIPE IS NOT CONNECTED" << std::endl;
		} else if (lErr != ERROR_MORE_DATA) {
			std::cout << "sendMsg: error writing file => " << std::hex << lErr;
			std::cout << " ("  << lErr << "): " << GetLastErrorStdStr() << std::endl;
		}
	}

	if (fSuccess) {
		dumpDataWithLen("recvMsg <<<<<:", (unsigned char*)szReadFileBuffer, dwNoBytesReadForReading);
		if (dwNoBytesReadForReading > 0) {
			ret.size = dwNoBytesReadForReading;
			ret.data = (uint8_t*)malloc(sizeof(uint8_t) * ret.size);
			memcpy(ret.data, &szReadFileBuffer, ret.size);
		}
	}

	return ret;
}

uint8_t_data
sendMsgWriteRead(HANDLE hPipe, uint8_t *msg, size_t size)
{
	uint8_t_data ret = { NULL, 0 };
	BOOL fSuccess;

	BOOL bWriteFile;
	BOOL bReadFile;

	//BOOL bReadFile;
	DWORD dwNoBytesReadForReading;//, dwMode;
//TODO: BUFFER IS TOO SMALL! WHAT IF BIGGER SIZE WILL BE RECEIVED?	
	char szReadFileBuffer[1023];
	DWORD dwReadFileBufferSize = sizeof(szReadFileBuffer);

	DWORD dwNoBytesReadForWriting;

	dumpDataWithLen("sendMSG WRITE >>>>>:", (unsigned char*)msg, size);
	bWriteFile = writeToFile(hPipe, (char*)msg /*szWriteFileBuffer*/, 
						(DWORD) size /*dwWriteFileBufferSize*/, 
						dwNoBytesReadForWriting);
	if (bWriteFile == FALSE) {
		std::cout << "sendMSG: Writefile failed" << std::endl;
		return ret;
	}


	try {
		bReadFile = ReadFile(
			hPipe,
			szReadFileBuffer,
			dwReadFileBufferSize,
			&dwNoBytesReadForReading,
			NULL
		);
		//std::cout << "Read from pipe: " << szReadFileBuffer << std::endl;
	}
	catch (std::exception& e) {
		bReadFile = FALSE;
		std::cout << "sendMSG: Error on ReadFile: " << e.what() << std::endl;
	}

	if (bReadFile) {
		dumpDataWithLen("sendMSG READ <<<<<:", (unsigned char*)szReadFileBuffer, dwNoBytesReadForReading);
		if (dwNoBytesReadForReading > 0) {
			ret.size = dwNoBytesReadForReading;
			ret.data = (uint8_t*)malloc(sizeof(uint8_t) * ret.size);
			memcpy(ret.data, &szReadFileBuffer, ret.size);
		}
	}

	return ret;
}

bool createServer(LPPIPEINST Pipe, HANDLE *hEvent, const char *PortName) {
	int i = 0;

	std::cout << "ServerPort: " << PortName << std::endl;
//	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");

	// The initial loop creates several instances of a named pipe 
	// along with an event object for each instance.  An 
	// overlapped ConnectNamedPipe operation is started for 
	// each instance. 

	// Create an event object for this instance. 

	*hEvent = CreateEvent(
		NULL,    // default security attribute 
		TRUE,    // manual-reset event 
		TRUE,    // initial state = signaled 
		NULL);   // unnamed event object 

	if (*hEvent == NULL)
	{
		printf("CreateEvent failed with %d.\n", GetLastError());
		return false;
	}

	Pipe->oOverlap.hEvent = *hEvent;

	Pipe->hPipeInst = CreateNamedPipeA(
		PortName,            // pipe name 
		PIPE_ACCESS_DUPLEX |     // read/write access 
		FILE_FLAG_OVERLAPPED,    // overlapped mode 
		PIPE_TYPE_MESSAGE |      // message-type pipe 
		PIPE_READMODE_MESSAGE |  // message-read mode 
		PIPE_WAIT,               // blocking mode 
		1,               // number of instances 
		BUFSIZE,   // output buffer size 
		BUFSIZE,   // input buffer size 
		PIPE_TIMEOUT,            // client time-out 
		NULL);                   // default security attributes 

	if (Pipe->hPipeInst == INVALID_HANDLE_VALUE)
	{
		printf("CreateNamedPipe failed with %d.\n", GetLastError());
		if (GetLastError() == ERROR_INVALID_NAME) {
			printf("Invalid name\n");
		}
		return false;
	}

	// Call the subroutine to connect to the new client

	Pipe->fPendingIO = ConnectToNewClient(
		Pipe->hPipeInst,
		&Pipe->oOverlap);

	Pipe->dwState = Pipe->fPendingIO ?
		CONNECTING_STATE : // still connecting 
		READING_STATE;     // ready to read 


	std::cout << "Created" << std::endl;

	return true;
}

// DisconnectAndReconnect(DWORD) 
// This function is called when an error occurs or when the client 
// closes its handle to the Pipe. Disconnect from this client, then 
// call ConnectNamedPipe to wait for another client to connect. 

VOID DisconnectAndReconnect(LPPIPEINST Pipe)
{
	// Disconnect the pipe instance. 

	if (!DisconnectNamedPipe(Pipe->hPipeInst))
	{
		printf("DisconnectNamedPipe failed with %d.\n", GetLastError());
	}

	// Call a subroutine to connect to the new client. 

	Pipe->fPendingIO = ConnectToNewClient(
		Pipe->hPipeInst,
		&Pipe->oOverlap);

	Pipe->dwState = Pipe->fPendingIO ?
		CONNECTING_STATE : // still connecting 
		READING_STATE;     // ready to read 
}

// ConnectToNewClient(HANDLE, LPOVERLAPPED) 
// This function is called to start an overlapped connect operation. 
// It returns TRUE if an operation is pending or FALSE if the 
// connection has been completed. 

BOOL ConnectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo)
{
	BOOL fConnected, fPendingIO = FALSE;

	// Start an overlapped connection for this pipe instance. 
	fConnected = ConnectNamedPipe(hPipe, lpo);

	// Overlapped ConnectNamedPipe should return zero. 
	if (fConnected)
	{
		printf("ConnectNamedPipe failed with %d: %s\n", GetLastError(), GetLastErrorStdStr());
		return 0;
	}

	switch (GetLastError())
	{
		// The overlapped connection in progress. 
	case ERROR_IO_PENDING:
		fPendingIO = TRUE;
		break;

		// Client is already connected, so signal an event. 
	case ERROR_PIPE_CONNECTED:
		if (SetEvent(lpo->hEvent))
			break;

		// If an error occurs during the connect operation... 
	default:
		{
			printf("ConnectNamedPipe failed with %d.\n", GetLastError());
			return 0;
		}
	}

	return fPendingIO;
}

VOID GetAnswerToRequest(LPPIPEINST Pipe, NIIPC *niipc)
{
	dumpDataWithLen("serverPipeRecv <<<<<:", (unsigned char *) Pipe->chRequest, Pipe->cbRead);

	auto vdata = std::make_unique<NIIPC::Data>(Pipe->chRequest, Pipe->chRequest + Pipe->cbRead);
	if (niipc) {
		printf("C> GetAnswerToRequest: calling queueCallbackResult\n");
		niipc->queueCallbackResult(std::move(vdata));
		/*
		auto result = niipc->fireCallback(std::move(vdata));
		if (result != nullptr) {
			printf("TODO: send return result from fireCallback - really needed???\n");
			//printf("_port_callback: got return result, trying to sent it back via return\n");
			//OK, something was returned - return this to sender
			//CFDataRef res = CFDataCreate(kCFAllocatorDefault, (uint8_t*) result->data(), result->size());
			//return res;
		}
		*/
	}	

}


int loopServer(NIIPC *niipc, LPPIPEINST Pipe, HANDLE *hEvent, DWORD dwmili) {
	DWORD i, dwWait, cbRet, dwErr;
	BOOL fSuccess;

		dwWait = WaitForSingleObject(
			*hEvent,
			dwmili
		);

		if (dwWait != WAIT_OBJECT_0) {
			return 0;
		}

		// Get the result if the operation was pending. 

		if (Pipe->fPendingIO)
		{
			fSuccess = GetOverlappedResult(
				Pipe->hPipeInst, // handle to pipe 
				&Pipe->oOverlap, // OVERLAPPED structure 
				&cbRet,            // bytes transferred 
				FALSE);            // do not wait 

			switch (Pipe->dwState)
			{
				// Pending connect operation 
			case CONNECTING_STATE:
				if (!fSuccess)
				{

			// The read operation is still pending. 
					dwErr = GetLastError();
					/*
					if (dwErr == ERROR_IO_INCOMPLETE) {

						printf("loopServer hEvent: 0x%x dwWait: 0x%x\n", *hEvent, dwWait);

					} else */
					if (dwErr == ERROR_IO_PENDING)
					{
						Pipe->fPendingIO = TRUE;
					} else 
					{
						printf("CONNECTING_STATE: Error %d> %s\n", dwErr, GetLastErrorStdStr());
					}				
					return 0;
				}
				Pipe->dwState = READING_STATE;
				break;

				// Pending read operation 
			case READING_STATE:
				if (!fSuccess || cbRet == 0)
				{
					DisconnectAndReconnect(Pipe);
//					continue;
					return 0;
				}
				Pipe->cbRead = cbRet;
				Pipe->dwState = WRITING_STATE;
				break;

				// Pending write operation 
			case WRITING_STATE:
				if (!fSuccess || cbRet != Pipe->cbToWrite)
				{
					DisconnectAndReconnect(Pipe);
//					continue;
					return 0;
				}
				Pipe->dwState = READING_STATE;
				break;

			default:
			{
				printf("Invalid pipe state: %s\n", GetLastError(), GetLastErrorStdStr());
				return 0;
			}
			}
		}

		// The pipe state determines which operation to do next. 

		switch (Pipe->dwState)
		{
			// READING_STATE: 
			// The pipe instance is connected to the client 
			// and is ready to read a request from the client. 

		case READING_STATE:
			fSuccess = ReadFile(
				Pipe->hPipeInst,
				Pipe->chRequest,
				BUFSIZE * sizeof(TCHAR),
				&Pipe->cbRead,
				&Pipe->oOverlap);

			// The read operation completed successfully. 

			if (fSuccess && Pipe->cbRead != 0)
			{
				Pipe->fPendingIO = FALSE;
				Pipe->dwState = WRITING_STATE;
//				continue;
				return 0;
			}

			// The read operation is still pending. 

			dwErr = GetLastError();
			if (!fSuccess && (dwErr == ERROR_IO_PENDING))
			{
				Pipe->fPendingIO = TRUE;
//				continue;
				return 0;
			}

			// An error occurred; disconnect from the client. 

			DisconnectAndReconnect(Pipe);
			break;

			// WRITING_STATE: 
			// The request was successfully read from the client. 
			// Get the reply data and write it to the client. 

		case WRITING_STATE:
			GetAnswerToRequest(Pipe, niipc);

			fSuccess = WriteFile(
				Pipe->hPipeInst,
				Pipe->chReply,
				Pipe->cbToWrite,
				&cbRet,
				&Pipe->oOverlap);

			// The write operation completed successfully. 

			if (fSuccess && cbRet == Pipe->cbToWrite)
			{
				Pipe->fPendingIO = FALSE;
				Pipe->dwState = READING_STATE;
//				continue;
				return 0;
			}

			// The write operation is still pending. 

			dwErr = GetLastError();
			if (!fSuccess && (dwErr == ERROR_IO_PENDING))
			{
				Pipe->fPendingIO = TRUE;
//				continue;
				return 0;
			}

			// An error occurred; disconnect from the client. 

			DisconnectAndReconnect(Pipe);
			break;

		default:
		{
			printf("Invalid pipe state.\n");
			return 0;
		}
		}
//	}

	return 0;
}
