#include <iostream>
#include "Winsock2.h"			   // Заголовок WS2_32.dll

#pragma comment(lib, "WS2_32.lib") // экспорт WS2_32.dll
#pragma warning(suppress : 4996)
#pragma warning(disable : 4996)

using namespace std;

string GetErrorMsgText(int code);
string SetErrorMsgText(string msgText, int code);


int main()
{
	setlocale(LC_ALL, "rus");
	int i;
	int t;
	try {
		WSADATA wsaData;
		SOCKET serverSocket; // серверный сокет

		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)                   // инициализация библиотеки
			throw  SetErrorMsgText("Startup:", WSAGetLastError());

		if ((serverSocket = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET) // создать сокет
			throw  SetErrorMsgText("socket:", WSAGetLastError());

		SOCKADDR_IN serv;				   // параметры сокета sS
		serv.sin_family = AF_INET;		   // используется IP-адресация
		serv.sin_port = htons(2000);	   // порт 2000
		serv.sin_addr.s_addr = INADDR_ANY; // любой собственный IP - адрес

		if (bind(serverSocket, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR) // установка параметров serv для сокета serverSocket
			throw  SetErrorMsgText("bind:", WSAGetLastError());

		if (SOCKET_ERROR == listen(serverSocket, 2)) // установка сокета сервера в режим прослушивания (2 параметр - макс. длина очереди)
		{
			cout << "Listen: " << WSAGetLastError << endl;
		}
		cout << "Сервер начал прослушку!" << endl;

		SOCKET clientSocket;	                 // сокет для обмена данными с клиентом 
		SOCKADDR_IN clnt;						 // параметры  сокета клиента
		memset(&clnt, 0, sizeof(clnt));			 // обнулить память
		int lclnt = sizeof(clnt);			     // размер SOCKADDR_IN

		while (true) {
			i = 1;
			// Функция accept создает канал на стороне сервера и иоздает сокет для обмена данными по этому каналу
			if ((clientSocket = accept(serverSocket, (sockaddr*)&clnt, &lclnt)) == INVALID_SOCKET)
				throw  SetErrorMsgText("accept:", WSAGetLastError());

			cout << "Соединение с клиентом успешно!" << endl;
			cout << "Адрес клиента: " << inet_ntoa(clnt.sin_addr) << ":" << htons(clnt.sin_port) << "\n\n\n\n\n";

			char ibuf[50],                     //буфер ввода 
				obuf[50] = "sever: принято ";  //буфер вывода
			int  libuf = 0,                    //количество принятых байт
				lobuf = 0;

			while (true) {
				
				t = clock();

				if ((libuf = recv(clientSocket, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR)
				{
					cout << "Recv: " << WSAGetLastError() << endl;
					break;
				}


				_itoa(lobuf, obuf + sizeof("sever: принято ") - 1, 10);

				cout << "Принято сообщение " << i << ": " << ibuf << endl;
				i++;

				if ((lobuf = send(clientSocket, ibuf, strlen(ibuf) + 1, NULL)) == SOCKET_ERROR)
				{
					cout << "Send: " << WSAGetLastError() << endl;
					break;
				}



			}

			cout << "\n\nКлиент отключился" << endl;
			cout << "Затраченное время: " << ((float)t) / CLOCKS_PER_SEC << " секунд";
		}
		if (closesocket(serverSocket) == SOCKET_ERROR)							// закрыть сокет
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