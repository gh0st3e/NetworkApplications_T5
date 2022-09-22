#include <iostream>
#include "Winsock2.h"			   // Заголовок WS2_32.dll
#include "WS2tcpip.h"

#pragma comment(lib, "WS2_32.lib") // экспорт WS2_32.dll

using namespace std;

string GetErrorMsgText(int code);
string SetErrorMsgText(string msgText, int code);


int main()
{
	setlocale(LC_ALL, "rus");
	try {
		WSADATA wsaData;
		SOCKET clientSocket; // клиентский сокет

		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)                   // инициализация библиотеки
			throw  SetErrorMsgText("Startup:", WSAGetLastError());

		if ((clientSocket = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET) // создать сокет
			throw  SetErrorMsgText("socket:", WSAGetLastError());
		
		SOCKET serverSocket;

		SOCKADDR_IN serv;                    // параметры  сокета сервера
		serv.sin_family = AF_INET;           // используется IP-адресация  
		serv.sin_port = htons(2000);                   // TCP-порт 2000
		inet_pton(AF_INET, "127.0.0.1", &(serv.sin_addr)); // адрес сервера

		//serv.sin_addr.s_addr = inet_addr("127.0.0.1");  // адрес сервера

		if ((connect(clientSocket, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
			throw  SetErrorMsgText("connect:", WSAGetLastError());

		
		char ibuf[50],                     //буфер ввода 
			obuf[50] = "Hello from Client ";  //буфер вывода
		int  libuf = 0,                    //количество принятых байт
			lobuf = 0;                    //количество отправленных байь 

		cout << "Введите кол-во сообщений: ";
		int count;
		cin >> count;

		for (int i = 0; i < count; i++) {
			if ((lobuf = send(clientSocket, obuf, strlen(obuf) + 1, NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("send:", WSAGetLastError());

			if ((libuf = recv(clientSocket, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("recv:", WSAGetLastError());

			cout << " Сообщение от сервера " << ": " << ibuf << (i + 1) << endl;
		}
		if (SOCKET_ERROR == send(clientSocket, "Конец передачи", sizeof("Конец передачи"), NULL))
		{
			cout << "send exit : " << GetLastError() << endl;
		}


		



		if (closesocket(clientSocket) == SOCKET_ERROR)							// закрыть сокет
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());

		if (WSACleanup() == SOCKET_ERROR)							    // завершить работу с библиотекой
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());





	}
	catch (string errorMsgText) {
		{ cout << endl << "WSAGetLastError: " << errorMsgText; }
	}

	return 0;
}

string GetErrorMsgText(int code) // Функция позволяет получить сообщение ошибки
{
	string msgText;
	switch (code)
	{
	case WSAEINTR: msgText = "Работа функции прервана";
		break;
	case WSAEACCES: msgText = "Разрешение отвергнуто";
		break;
	case WSAEFAULT: msgText = "Ошибочный адрес";
		break;
	case WSASYSCALLFAILURE: msgText = "Аварийное завершение системного вызова";
		break;
	default: msgText = "***ERROR***";
		break;
	}
	//TODO: добавить коды
	return msgText;
}


string SetErrorMsgText(string msgText, int code) // Функция возвращает сообщение ошибки
{
	return msgText + GetErrorMsgText(code);
}