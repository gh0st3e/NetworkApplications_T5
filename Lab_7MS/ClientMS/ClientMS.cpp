#include <iostream>
#include "windows.h"
#include <string>
#include "ClientMS.h"

using namespace std;

string GetErrorMail(int code)
{
	string msgText = "";
	switch (code)
	{
	case WSAEINTR: msgText = "WSAEINTR"; break;
	case WSAEACCES:	msgText = "WSAEACCES"; break;
	case WSAEFAULT:	msgText = "WSAEFAULT"; break;
	default: msgText = "Error";
	};
	return msgText;
}

string SetErrorMail(string msgText, int code)
{
	return msgText + GetErrorMail(code);
}


int main()
{
	setlocale(LC_ALL, "rus");

	try {
		HANDLE clientMailSlot;
		double t1, t2;

		if ((clientMailSlot = CreateFile(L"\\\\.\\mailslot\\BOX",
			GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			NULL,
			NULL)) == INVALID_HANDLE_VALUE)
			throw SetErrorMail("CreateFile: ", GetLastError());

		cout << "Hello i am Client" << endl;

		char writeBuf[50] = "Hello from Client-Mailslot";
		DWORD writeMsg;

		t1 = clock();

		for (int i = 1; i <= 1000; i++) {
			if(!WriteFile(clientMailSlot,writeBuf,sizeof(writeBuf),&writeMsg,NULL))
				throw SetErrorMail("WriteFile: ", GetLastError());

			cout << "Message " << i << " was sent" << endl;
		}

		t2 = clock();

		if (!CloseHandle(clientMailSlot))
			throw "Error: CloseHandle";

		cout << endl << "Время передачи: " << (t2 - t1) / 1000 << " сек." << endl << endl;
		system("pause");
	}
	catch (string e) {
		cout << e << endl;
	}
	
	return 0;
}
