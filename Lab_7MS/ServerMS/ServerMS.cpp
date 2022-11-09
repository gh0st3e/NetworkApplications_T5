#include <iostream>
#include "windows.h"
#include <string>

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
		HANDLE serverMailSlot;
		int i = 0;
		double t1, t2;

		if ((serverMailSlot = CreateMailslot(L"\\\\.\\mailslot\\BOX",
			300,
			MAILSLOT_WAIT_FOREVER,
			NULL)) == INVALID_HANDLE_VALUE)
			throw SetErrorMail("CreateMailslot", GetLastError());

		cout << "Hello i am server" << endl;

		char readBuf[50];
		DWORD readMsg;
		do {
			i++;
			if (!ReadFile(serverMailSlot, readBuf, sizeof(readBuf), &readMsg, NULL))
				throw SetErrorMail("ReadFile", GetLastError());
			SetMailslotInfo(serverMailSlot, 5000);
			if (i == 1)
				t1 = clock();
			cout << readBuf << " " << i << endl;
			
		} while (strcmp(readBuf, " ") != 0);

		t2 = clock();

		cout << "Время передачи: " << (t2 - t1) / 1000 << " сек." << endl;
		cout << "Количество сообщений: " << i - 1 << endl << endl;

		if (!CloseHandle(serverMailSlot))
			throw "Error: CloseHandle";

		system("pause");
	}
	catch (string e) {
		if (e == "ReadFileError") {
			cout << e << " Timeout";
			return 0;
		}
		cout << e << endl;
	}
	return 0;
}