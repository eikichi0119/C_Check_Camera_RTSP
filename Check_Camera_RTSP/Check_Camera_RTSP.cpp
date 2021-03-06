// ConnectSocket2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>

//20180730_Ian
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <tlhelp32.h>
#include <wchar.h>

#include <cstdio>
#include <iomanip>
#define _CRT_SECURE_NO_WARNINGS
#define UNINITIALIZED 0xFFFFFFFF
#include<fstream>
#include <vector>
#include <time.h>
#include <random>
#include <chrono>
#include <conio.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#include <cstdio>
//using namespace System;


DWORD FindProcessId(const std::wstring& processName);
float randomValue();

#define DEFAULT_BUFLEN 512
//20180730_Ian - removed port pre-define.
//#define DEFAULT_PORT "80"

int __cdecl main(int argc, char **argv)
{
	//20180730_Ian
	std::wstring processName;
	//	std::wcout << "Enter the process name: ";
	//	std::getline(std::wcin, processName);
	//	DWORD processID = FindProcessId(processName);


	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
	*ptr = NULL,
	hints;
	char sendbuf[] = "this is a test";
	char *ptrTosendbuf = sendbuf;

	//char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	
	// Validate the parameters
	//20180730_Ian - Increase argc length.
	if (argc != 7) {
		std::wcout << "usage: " << argv[0] << " Cam-IP-Address Cam-HTTP-port Cam-login-account Cam-login-password Cam-RTSP-Port VLC-Program-Folder" << std::endl;
		return 1;
	}
	while (1)
	{
		//		std::random_device rd;
		//		std::mt19937_64 generator(rd());
		//		std::uniform_real_distribution<float> unif(1.0, 10.0);
		//		float gap_old = unif(generator);
		//		int gap_new = static_cast<int>(gap_old);

	Start:
		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			std::wcout << "WSAStartup failed with error: " << iResult << std::endl;
			return 1;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		//20180730_Ian - Changed using argv[2] for port information.
		//printf("%s %s", argv[1], argv[2]);
		// Resolve the server address and port
		iResult = getaddrinfo(argv[1], argv[2], &hints, &result);
		std::wstring CamIP;
		std::wstring CamPort;

		std::wofstream savefile;
		savefile.open("Cam_info.txt");
		savefile << argv[1] << std::endl;
		savefile << argv[2] << std::endl;
		savefile << "vlc.exe" << std::endl;
		savefile << argv[3] << std::endl;
		savefile << argv[4] << std::endl;
		savefile << argv[5] << std::endl;
		savefile << argv[6] << std::endl;
		//savefile << argv[7] << std::endl;
		savefile.close();

		std::wifstream loadfile;
		loadfile.open("Cam_info.txt");
		std::wstring inputStr;
		std::vector<std::wstring> inputContent;
		while (getline(loadfile, inputStr)) {
			//			cout << inputStr << endl;
			inputContent.push_back(inputStr);
		}
		//		for (int i = 0; i < inputContent.size(); i++) {
		//			cout << inputContent[i] << endl;
		//		}
		loadfile.close();
		//		cout << inputContent[0] << endl;
		//		cout << inputContent[1] << endl;
		//		cout << inputContent[2] << endl;
		CamIP = inputContent[0];
		CamPort = inputContent[1];
		processName = inputContent[2];
		//		std::wcout << CamIP << std::endl;
		//		std::wcout << CamPort << std::endl;
		//		std::wcout << processName << std::endl;
		//		std::wcout << inputContent[3] << std::endl;
		//		std::wcout << inputContent[4] << std::endl;
		//		std::wcout << inputContent[5] << std::endl;
		DWORD processID = FindProcessId(processName);

		if (iResult != 0) {
			std::wcout << "getaddrinfo failed with error: " << iResult << std::endl;
			WSACleanup();
			return 1;
		}

		// Attempt to connect to an address until one succeeds
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			// Create a SOCKET for connecting to server
			ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (ConnectSocket == INVALID_SOCKET) {
				std::wcout << "socket failed with error: " << WSAGetLastError() << std::endl;
				WSACleanup();
				return 1;
			}

			// Connect to server.
			iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			float Random1 = randomValue();
			DWORD gap_new1 = static_cast<int>(Random1);
//			cout << "gap_new1: " << gap_new1 << endl;
			gap_new1 = gap_new1 * 1000;
			Sleep(gap_new1);
			if (iResult == SOCKET_ERROR) {
				closesocket(ConnectSocket);
				ConnectSocket = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(result);

		if (ConnectSocket == INVALID_SOCKET) {
			std::wcout << "Unable to connect to server!" << std::endl;

			
			if (processID == 0) {
				std::wcout << "Could not find " << processName.c_str() << std::endl;
			}
			else {
			//20180803_Ian below excluded



			//std::string first_arge = argv[1];
			//std::string second_arge = argv[2];
			//	std::wstring cmd1;
			//cmd1 << "D:\\vlc-4.0.0-dev\\vlc.exe -vvv rtsp://admin:admin@" << argv[1] << ":" << argv[2] << "/video.pro1 --network-caching=200 --no-quiet --verbose=3 --file-logging --logfile=vlc-log.txt telnet --telnet-host=\"0.0.0.0\" --telnet-port=\"6419\" --telnet-password=\"nuuono1\"";
			//		std::wcout << "Process ID is " << processID << std::endl;
			//char cmd[65535] = "";
			//sprintf(cmd, "D:\\vlc-4.0.0-dev\\vlc.exe -vvv rtsp://admin:admin@%s:%s/video.pro1 --network-caching=200 --no-quiet --verbose=3 --file-logging --logfile=vlc-log.txt telnet --telnet-host=\"0.0.0.0\" --telnet-port=\"6419\" --telnet-password=\"nuuono1\"",argv[1], argv[2]);
			//char *toCall = "D:\\vlc-4.0.0-dev\\vlc.exe -vvv rtsp://admin:admin@" + argv[1] + ":" + argv[2] + "/video.pro1 --network-caching=200 --no-quiet --verbose=3 --file-logging --logfile=vlc-log.txt telnet --telnet-host=\"0.0.0.0\" --telnet-port=\"6419\" --telnet-password=\"nuuono1\"";
			/*
			const char* processName_ = "vlc.exe";
			DWORD  processID_ = 0;
			DWORD  processBaseAddress_ = UNINITIALIZED;
			// Get the process ID
			{
			PROCESSENTRY32 processEntry_; // Entry into process you wish to inject to
			HANDLE hProcSnapshot_ = NULL;
			// Takes a snapshot of the system's processes
			hProcSnapshot_ = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //?

			// While process has not been found, keep looking for it
			while (!processID_)
			{
			// If a process on the system exists
			if (Process32First(hProcSnapshot_, &processEntry_)) //?
			{
			// Check all processes in the system's processes snapshot
			do
			{
			std::wstring s(processEntry_.szExeFile);
			const WCHAR * ProcessName_ = s.c_str();

			// Compare the name of the process to the one we want
			if (!wcscmp(processEntry_.szExeFile, ProcessName_))
			//if (!strcmp(processEntry_.szExeFile, processName_)) //?
			{
			// Save the processID and break out
			processID_ = processEntry_.th32ProcessID;
			break;
			}
			} while (Process32Next(hProcSnapshot_, &processEntry_));
			}

			// Didnt find process, sleep for a bit
			if (!processID_)
			{
			system("CLS");
			//std::cout << "Make sure " << processName_ << " is running." << std::endl;
			std::cout << "Make sure vlc.exe is running." << std::endl;

			Reconnect:
			// Attempt to connect to an address until one succeeds
			for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			// Create a SOCKET for connecting to server
			ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
			if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
			}

			// Connect to server.
			iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
			}
			break;
			}

			//freeaddrinfo(result);

			if (ConnectSocket == INVALID_SOCKET) {
			printf("Unable to connect to server!\n");
			Sleep(3000);
			goto Reconnect;
			}

			}
			}

			// Process found
			//std::cout << "Found Process: " << processName_ << std::endl;
			std::cout << "Found Process: vlc.exe" << std::endl;
			system("taskkill /IM vlc.exe /F");
			Sleep(5000);
			//system(cmd1);
			system("D:\\vlc-4.0.0-dev\\vlc.exe -vvv rtsp://admin:admin@10.0.9.140/video.pro1 --network-caching=200 --no-quiet --verbose=3 --file-logging --logfile=vlc-log.txt telnet --telnet-host=\"0.0.0.0\" --telnet-port=\"6419\" --telnet-password=\"nuuono1\"");
			//	system("PAUSE");
			}
			*/
			/*
			// Find Base Address of process
			{
			HANDLE moduleSnapshotHandle_ = INVALID_HANDLE_VALUE;
			MODULEENTRY32 moduleEntry_;

			// Take snapshot of all the modules in the process
			moduleSnapshotHandle_ = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID_);

			// Snapshot failed
			if (moduleSnapshotHandle_ == INVALID_HANDLE_VALUE)
			{
			std::cout << "Module Snapshot error" << std::endl;
			return 0;
			}

			// Size the structure before usage
			moduleEntry_.dwSize = sizeof(MODULEENTRY32);

			// Retrieve information about the first module
			if (!Module32First(moduleSnapshotHandle_, &moduleEntry_))
			{
			std::cout << "First module not found" << std::endl;
			CloseHandle(moduleSnapshotHandle_);
			return 0;
			}

			// Find base address
			while (processBaseAddress_ == UNINITIALIZED)
			{
			// Find module of the executable
			do
			{
			std::wstring s(moduleEntry_.szModule);
			const WCHAR * ProcessName_ = s.c_str();

			// Compare the name of the process to the one we want
			if (!wcscmp(moduleEntry_.szModule, ProcessName_)) //?
			{
			// Save the processID and break out
			processBaseAddress_ = (unsigned int)moduleEntry_.modBaseAddr;
			break;
			}

			} while (Module32Next(moduleSnapshotHandle_, &moduleEntry_));


			if (processBaseAddress_ == UNINITIALIZED)
			{
			system("CLS");
			std::cout << "Failed to find module" << processName_ << std::endl;
			Sleep(200);
			}
			}

			// Found module and base address successfully
			std::cout << "Base Address: " << std::hex << processBaseAddress_ << std::dec << std::endl;
			CloseHandle(moduleSnapshotHandle_);
			}

			*/



			//20180803_Ian above excluded
			std::wcout << "VLC process id is " << processID << std::endl;
			system("taskkill /IM vlc.exe /F");
			//20180803_Ian below excluded


			//Reconnect:
			/*
			// Initialize Winsock
			iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			return 1;
			}

			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			*/
			//20180730_Ian - Changed using argv[2] for port information.
			//printf("%s %s", argv[1], argv[2]);
			// Resolve the server address and port



			//iResult = getaddrinfo(argv[1], argv[2], &hints, &result);



			/*
			if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return 1;
			}
			*/
			// Attempt to connect to an address until one succeeds



			//for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			// Create a SOCKET for connecting to server
			//				ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			/*
			if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			Sleep(3000);
			WSACleanup();
			goto Reconnect;
			}
			*/
			// Connect to server.
			//iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			//if (iResult == SOCKET_ERROR) {
			//closesocket(ConnectSocket);
			//ConnectSocket = INVALID_SOCKET;
			//continue;
			//}
			//break;
			//}

			//freeaddrinfo(result);

			//if (ConnectSocket == INVALID_SOCKET) {
			//	printf("Unable to connect to server!\n");
			//	Sleep(3000);
			//			freeaddrinfo(result);
			//	WSACleanup();
			//	goto Reconnect;
			//}
			//else
			//	Sleep(5000);
			//system(cmd1);
			//system("D:\\vlc-4.0.0-dev\\vlc.exe -vvv rtsp://admin:admin@10.0.9.140/video.pro1 --network-caching=200 --no-quiet --verbose=3 --file-logging --logfile=vlc-log.txt telnet --telnet-host=\"0.0.0.0\" --telnet-port=\"6419\" --telnet-password=\"nuuono1\"");
			//	system("PAUSE");

			//				return 1;

			//20180803_Ian above excluded
			
			}
			
			closesocket(ConnectSocket);
			WSACleanup();
		}


		// Send an initial buffer
		iResult = send(ConnectSocket, ptrTosendbuf, (int)strlen(ptrTosendbuf), 0);
		if (iResult == SOCKET_ERROR) {
			std::wcout << "send failed with error: " << WSAGetLastError() << std::endl;

			closesocket(ConnectSocket);
			WSACleanup();
			//				return 1;
		}
		/*
		else
		for (int i = 0; i < 2; i++) {
		printf("Bytes Sent: %ld\n", iResult);
		printf("Execute VLC application again.\n");
		//system("D:\\vlc-4.0.0-dev\\vlc.exe -vvv rtsp://admin:admin@10.0.9.140/video.pro1 --network-caching=200 --no-quiet --verbose=3 --file-logging --logfile=vlc-log.txt telnet --telnet-host=\"0.0.0.0\" --telnet-port=\"6419\" --telnet-password=\"nuuono1\"");
		system("D:\\vlc-4.0.0-dev\\schedule.vbs");
		printf("VLC executed.\n");
		goto Start;
		}
		*/
		float Random2 = randomValue();
		DWORD gap_new2 = static_cast<int>(Random2);
		//		cout << "gap_new2: " << gap_new2 << endl;
		gap_new2 = gap_new2 * 1000;
		Sleep(gap_new2);
		std::wcout << "Bytes Sent: " << iResult << std::endl;
		std::wcout << "Checking camera connection done. Still alive." << std::endl;

		DWORD processID1 = FindProcessId(processName);

		if (processID1 == 0) {
			std::wcout << "Could not find " << processName.c_str() << std::endl;
			float Random3 = randomValue();
			DWORD gap_new3 = static_cast<int>(Random3);
			gap_new3 = gap_new3 * 1000;
			Sleep(gap_new3);
			std::wcout << "Execute VLC application again." << std::endl;
			system("schedule.vbs");
			std::wcout << "VLC executed." << std::endl;
		}

		// shutdown the connection since no more data will be sent
		iResult = shutdown(ConnectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			std::wcout << "shutdown failed with error: " << WSAGetLastError() << std::endl;
			//	system("taskkill /IM vlc.exe /F");
			closesocket(ConnectSocket);
			WSACleanup();
			//			return 1;
		}

		// Receive until the peer closes the connection
		if (iResult > 0) {
			iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0) {
				std::wcout << "Bytes received: " << iResult << std::endl;
			}
			else if (iResult == 0) {
				std::wcout << "Connection closed" << std::endl;
				closesocket(ConnectSocket);
				WSACleanup();
			}
			else {
				std::wcout << "recv failed with error: " << WSAGetLastError() << std::endl;
				closesocket(ConnectSocket);
				WSACleanup();
			}
		}
		/*

		if (ConnectSocket != INVALID_SOCKET) {

			if (processID == 0) {
				float Random3 = randomValue();
				DWORD gap_new3 = static_cast<int>(Random3);

		//20180803_Ian below excluded
		//				cout << "gap_new3: " << gap_new3 << endl;
		//20180803_Ian above excluded
		
				gap_new3 = gap_new3 * 1000;
				Sleep(gap_new3);
				std::wcout << "Execute VLC application again." << std::endl;
				system("schedule.vbs");
				std::wcout << "VLC executed." << std::endl;
			}
		}
		*/

		// cleanup
		closesocket(ConnectSocket);
		WSACleanup();


		/*
		do {
		std::wcout << "Could not find " << processName.c_str() << std::endl;
		Sleep(3000);
		printf("in loop\n");
		printf("Execute VLC application again.\n");
		system("D:\\vlc-4.0.0-dev\\schedule.vbs");
		printf("VLC executed.\n");
		goto Start;
		} while (processID == 0);
		*/
		//			system("D:\\vlc-4.0.0-dev\\vlc.exe -vvv rtsp://admin:admin@10.0.9.140/video.pro1 --network-caching=200 --no-quiet --verbose=3 --file-logging --logfile=vlc-log.txt telnet --telnet-host=\"0.0.0.0\" --telnet-port=\"6419\" --telnet-password=\"nuuono1\"");
		//			Sleep(3000);
		//			goto Start;


		//		return 0;
		goto Start;
	}
	return 0;
}


//20180730_Ian
DWORD FindProcessId(const std::wstring& processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}

float randomValue() {
	// Seed a Mersenne Twister (good RNG) with the current system time
	std::mt19937_64 generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<float> distribution(5.0, 10.0);
	float gap = distribution(generator);
	return (gap);
}
