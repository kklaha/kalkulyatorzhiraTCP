#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <sstream>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#define SERVERHOST "tetaueta"
int main() {
	char buffer[1024];
	setlocale(LC_ALL, "RU");
	WSAData wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cerr << "Ошибка запуска\n";
		return 1;
	}
	SOCKET cl = socket(AF_INET, SOCK_STREAM, 0);
	if (cl == SOCKET_ERROR) {
		std::cerr << "Ошибка инициализации сокета\n";
		closesocket(cl);
		WSACleanup();
		return 1;
	}
	sockaddr_in adr;
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = inet_addr(SERVERHOST);
	adr.sin_port = htons(8000);
	if (connect(cl, (sockaddr*)&adr, sizeof(adr)) == SOCKET_ERROR) {
		std::cerr << "Не удалось подключиться к серверу\n";
		closesocket(cl);
		WSACleanup();
		return 1;
	}
	double height, weight;
	std::cerr << "Подлючение к КАЛЬКУЛЯТОРУ ЖИРА успешно\n";
	std::cerr << "Введит свой рост и вес\n";
	std::cin >> height >> weight;
	std::ostringstream os;
	os <<height <<" " << weight;
	std::string dan = os.str();
	send(cl, dan.c_str(), dan.length(), 0);
	int bytes = recv(cl, buffer, 1023, 0);
	if (bytes > 0) {
		buffer[bytes] = '\0';
		std::string res(buffer, bytes);
		std::cerr << "ВАШ ИМТ: " << res << '\n';
		double imt = std::stod(res);
		int imtchis;
		if (imt < 18.5) imtchis = 0;
		else if (imt < 25) imtchis = 1;
		else if (imt < 30) imtchis = 2;
		else imtchis = 3;
		switch (imtchis) {
		case 0:
			std::cout << "Недостаточный вес\n";
			break;
		case 1:
			std::cout << "Норма\n";
			break;
		case 2:
			std::cout << "Предожирение\n";
			break;
		case 3:
			std::cout << "Ожирение\n";
			break;
		default:
			std::cout << "ошибка\n";
			break;
		}
	}
	closesocket(cl);
	WSACleanup();
	return 0;
}