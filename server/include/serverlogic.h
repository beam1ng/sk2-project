#ifndef MYWIDGET_H
#define MYWIDGET_H

#include "datastructures.h"

class ServerLogic
{
public:
    explicit ServerLogic();

    class User{
    public:
        User(int sock);
        ~User(){}

        int sock;
        char nickname[32] = {'\0'};
        std::string toString(){return std::string(nickname);}
    };

protected:
    class Lobby {
    public:
        ServerLogic * partent;
        Lobby(User * host,ServerLogic * parent);
        ~Lobby(){};

        ServerLogic * parent;
        int id;
        User * host;
        std::vector<User*> players;
        DataStructures::quizNames quizName;
    };


    class Game {
    public:
        Game(User * host, ServerLogic * parent);
        ~Game(){};

        ServerLogic * parent;
        User * host;
        std::vector<User*> players;
        DataStructures::quizNames quizName;
        std::vector<int> scores;
        std::vector<User*> answerSentPlayers;
        int currentQuestionIndex = -1;
        int successfullCurrentQuestionAnswers = 0;
        int currentQuestionCorrectAnswerIndex = 0;
        int totalQuestionsCount = 0;
        std::string summary;

        int nextPhase();//returns 1 if there is summary to be shown
    private:
        DataStructures::qaBundle getCorrectQaBundle();
    };

    int servSock;

    std::vector<Lobby*> lobbies;
    std::vector<Game*> games;

public:
    std::vector<User*> users;
    void userDisconnected(int socket);
    void readPackage(int socket);

private:
    std::vector<std::string> nicknames;
    void readPackage(User* u);
    void userDisconnected(User* u);
    void updateLobbyViews(ServerLogic::Lobby *l);
    void readStruct(ServerLogic::User *user, char *buffer, int size);
    void sendStruct(ServerLogic::User *user, char *data, int size);
    void nextGamePhase(Game *g);
};

#endif // MYWIDGET_H
