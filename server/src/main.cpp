#include "serverlogic.h"
#include "poll.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <unistd.h>
#include <thread>
#define MAX_SOCKETS_COUNT 16

int serverSocket;
pollfd pollDescriptors[MAX_SOCKETS_COUNT];
int pollDescriptorAviability[MAX_SOCKETS_COUNT] = {0};//0 if aviable 1 elseway
ServerLogic sl = ServerLogic();

void handleServerSocketData(int revents){
    if(revents & ~POLLIN){
        printf("serverr:%x\n",revents);
    }
    if(revents & POLLIN){
        sockaddr_in clientAddr{};
        socklen_t clientAddrSize = sizeof(clientAddr);
        auto clientSocket = accept(serverSocket, (sockaddr*) &clientAddr, &clientAddrSize);
        for (int i = 0;i<MAX_SOCKETS_COUNT;i++){
            if(pollDescriptorAviability[i]==0){
                printf("i:%d\n",i);
                pollDescriptors[i].fd = clientSocket;
                pollDescriptors[i].events = POLLIN|POLLRDHUP;
                pollDescriptorAviability[i]=1;
                ServerLogic::User * u = new ServerLogic::User(clientSocket);
                sl.users.push_back(u);
                break;
            }
        }
    }
}

void handleClientSocketData(int socketIndex){
    auto clientSocket = pollDescriptors[socketIndex].fd;
    auto revents = pollDescriptors[socketIndex].revents;

    if(revents & POLLRDHUP){
        sl.userDisconnected(clientSocket);
        pollDescriptorAviability[socketIndex] = 0;
        shutdown(clientSocket,SHUT_RDWR);
        close(clientSocket);
        clientSocket = -1;
        printf("revent:%x\n",revents);
    }
    else if(revents & POLLIN){
        sl.readPackage(clientSocket);
    }else{
        printf("unexpected error:%x\n",revents);
    }
}

void stateUpdater(){
    while(true){
        sl.updateGames();
        usleep(100000);//10 ticks each second
    }
    return;
}

int main(int argc, char *argv[])
{
    if(argc!=2){
        printf("Expected number of arguments: 1, received: %d", argc-1);
        return -1;
    }
    int port;
    sscanf(argv[1],"%d",&port);
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    const int one = 1;
    setsockopt(serverSocket,SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));

    sockaddr_in serverAddr{.sin_family=AF_INET,.sin_port=htons((short)port),.sin_addr={INADDR_ANY}};
    bind(serverSocket, (sockaddr*)&serverAddr,sizeof(serverAddr));

    listen(serverSocket,1);

    pollDescriptors[0].fd=serverSocket;
    pollDescriptors[0].events=POLLIN;
    pollDescriptorAviability[0]=1;

    std::thread t1(stateUpdater);

    while(true){
        int readyCount = poll(pollDescriptors,MAX_SOCKETS_COUNT,-1);
        for(int i = 0;i<MAX_SOCKETS_COUNT && readyCount>0;i++){
            if(pollDescriptors[i].revents && pollDescriptorAviability[i]==1){
                if(pollDescriptors[i].fd==serverSocket){
                    handleServerSocketData(pollDescriptors[i].revents);
                }
                else
                {
                    handleClientSocketData(i);
                }
                readyCount--;       
            }
        }
    }
}
