#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <iostream>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <sstream>
int main() {
	setlocale(LC_ALL, "RU");
	char buffer[1024];
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cerr << "Ошибка запуска\n";
		return 1;
	}
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in adr;
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = 0;
	adr.sin_port = htons(8000);
	if (bind(s, (struct sockaddr*)&adr, sizeof(adr)) == SOCKET_ERROR) {
		std::cerr << "Ошибка связи\n";
		closesocket(s);
		WSACleanup();
		return 1;
	}
	if (listen(s, 5) == SOCKET_ERROR) {
		std::cerr << "Ошибка прослушивания\n";
		closesocket(s);
		WSACleanup();
		return 1;
	}
	while (true) {
		SOCKET client = accept(s, NULL, NULL);
		if (client == INVALID_SOCKET) continue;
		std::cout << "Новый пользователь подключен\n";
			int bytes = recv(client, buffer, 1023, 0);
			if (bytes > 0) {
				buffer[bytes] = '\0';
				std::istringstream is(buffer);
				double height, weight;
				if (is >> height >> weight && height > 0 && weight > 0) {
					double imt = weight / pow(height, 2);
					std::ostringstream os;
					os << std::fixed << imt;
					std::string res = os.str();
					send(client, res.c_str(), res.length(), 0);
				}
				else {
					const char* error = "Некорректные данные";
					send(client, error, strlen(error), 0);
					break;
				}
			}
		closesocket(client);
		std::cerr << "Клиент отключился ";
	}
	closesocket(s);
	WSACleanup();
	return 0;
}
