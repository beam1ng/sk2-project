#include "serverlogic.h"
#include "datastructures.h"
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <chrono>

ServerLogic::ServerLogic(){
}

void ServerLogic::userDisconnected(User* u){

    for(int i = 0;i< ServerLogic::lobbies.size();i++){
        Lobby* currentLobby = ServerLogic::lobbies[i];
        auto playerIterator = std::find(currentLobby->players.begin(),currentLobby->players.end(),u);
        bool isPlayerContained = playerIterator!=currentLobby->players.end();
        if(isPlayerContained){
            if(currentLobby->host==u){
                for(int j = 0;j<lobbies[i]->players.size();j++){
                    DataStructures::gameClosedByHostStruct gcbh{};
                    sendStruct(lobbies[i]->players[j],reinterpret_cast<char*>(&gcbh),sizeof(gcbh));
                }

                lobbies.erase(lobbies.begin()+i);
            }else{
                currentLobby->players.erase(playerIterator);
            }
        }
    }

    for(int i =0;i<games.size();i++){
        auto playerIterator = std::find(games[i]->players.begin(),games[i]->players.end(),u);
        bool isPlayerContained = playerIterator!=games[i]->players.end();
        int playerIndex = playerIterator-games[i]->players.begin();
        if(isPlayerContained){
            auto answerSentPlayerIterator = std::find(games[i]->answerSentPlayers.begin(),games[i]->answerSentPlayers.end(),u);
            bool isAnswerSentByPlayer = games[i]->answerSentPlayers.end()!=answerSentPlayerIterator;
            if(isAnswerSentByPlayer){
                games[i]->answerSentPlayers.erase(answerSentPlayerIterator);
            }
            if(games[i]->host==u){
                for(int j = 0;j<games[i]->players.size();j++){
                    DataStructures::gameClosedByHostStruct gcbh{};
                    sendStruct(games[i]->players[j],reinterpret_cast<char*>(&gcbh),sizeof(gcbh));
                }
                games.erase(games.begin()+i);
            }
            games[i]->scores.erase(games[i]->scores.begin()+playerIndex);
            games[i]->players.erase(playerIterator);
        }
    }
    ServerLogic::nicknames.erase(std::find(ServerLogic::nicknames.begin(),ServerLogic::nicknames.end(),std::string(u->nickname)));
    ServerLogic::users.erase(std::find(ServerLogic::users.begin(),ServerLogic::users.end(),u));
}

void ServerLogic::userDisconnected(int sock){
    bool playerFound = false;
    User * u;
    for(int i = 0;i<ServerLogic::users.size();i++){
        if(ServerLogic::users[i]->sock==sock){
            playerFound = true;
            u = ServerLogic::users[i];
        }
    }
    if(playerFound){
        ServerLogic::userDisconnected(u);
    }
}

void ServerLogic::readPackage(int sock){
    bool playerFound = false;
    User * u;
    for(int i = 0;i<ServerLogic::users.size();i++){
        if(ServerLogic::users[i]->sock==sock){
            playerFound = true;
            u = ServerLogic::users[i];
        }
    }
    if(playerFound){
        ServerLogic::readPackage(u);
    }
}

ServerLogic::User::User(int sock){
    this->sock = sock;
}

ServerLogic::Lobby::Lobby(User * host,ServerLogic * parent){
    this->host = host;
    this->parent = parent;
    this->players.push_back(host);
    int lobbyId = 100;
    for(int i = 0;i<parent->lobbies.size();i++){
        if(parent->lobbies[i]->id!=lobbyId){
            break;
        }
        lobbyId++;
    }
    this->id = lobbyId;
}

void ServerLogic::updateLobbyViews(ServerLogic::Lobby * l){
    DataStructures::lobbyDetailsStruct ld{};
    strncpy(ld.hostNickname,l->host->nickname,32);
    for(int k = 0;k<l->players.size();k++){
        strncpy(ld.nicknames[k],l->players[k]->nickname,32);
    }

    char* data = reinterpret_cast<char*>(&ld);
    int size = sizeof(ld);

    for(int i = 0;i<l->players.size();i++){
        auto player = l->players[i];
        sendStruct(player,data,size);
    }
}

int ServerLogic::Game::nextPhase()
{
    //todo: start timer with nextPhase() lambda on timer end

    ServerLogic::Game::currentQuestionIndex++;
    ServerLogic::Game::questionStart = std::chrono::high_resolution_clock::now();

    if(currentQuestionIndex>=totalQuestionsCount){
        //summary
        summary = "Game ended, final player scores:\n";
        for(int i = 0;i<players.size();i++){
            std::string currentPlayerSummary = "Player "+std::string(players[i]->nickname)+": "+std::to_string(scores[i])+"\n";
            summary.append(currentPlayerSummary);
        }

        return 1;
    }

    DataStructures::qaBundle qaBundle = ServerLogic::Game::getCorrectQaBundle();

    ServerLogic::Game::currentQuestionCorrectAnswerIndex = qaBundle.correctAnswerIndex;

    DataStructures::gameQuestionStruct gq{};
    strncpy(gq.question,qaBundle.question.c_str(),64);
    strncpy(gq.answers[0],qaBundle.answerA.c_str(),32);
    strncpy(gq.answers[1],qaBundle.answerB.c_str(),32);
    strncpy(gq.answers[2],qaBundle.answerC.c_str(),32);
    strncpy(gq.answers[3],qaBundle.answerD.c_str(),32);

    char* data = reinterpret_cast<char*>(&gq);
    int size = sizeof(gq);

    answerSentPlayers.clear();
    successfullCurrentQuestionAnswers = 0;

    for(int i = 0;i<players.size();i++){
        parent->sendStruct(players[i],data,size);
    }
    return 0;
}

ServerLogic::Game::Game(User * host, ServerLogic * parent){
    this->host = host;
    this-> parent = parent;
}



void ServerLogic::readPackage(User *u){
    DataStructures::clientCommands clientCommand;
    readStruct(u,reinterpret_cast<char*>(&clientCommand),sizeof(clientCommand));

    switch(clientCommand){
    case DataStructures::createLobby:{
        DataStructures::createLobbyRequest req{};

        Lobby * l = new Lobby(u,this);
        lobbies.push_back(l);

        DataStructures::joinLobbyResultStruct j{};
        j.lobbyId = l->id;
        j.isSuccess = true;

        sendStruct(u,reinterpret_cast<char*>(&j),sizeof(j));

        DataStructures::quizCategoriesStruct qc{};

        std::string mathCategoryName = "math";
        std::string reflexCategoryName = "reflex";

        strncpy(qc.categoryNames[0],mathCategoryName.c_str(),16);
        strncpy(qc.categoryNames[1],reflexCategoryName.c_str(),16);

        sendStruct(u,reinterpret_cast<char*>(&qc),sizeof(qc));

        break;
    }
    case DataStructures::chooseQuizCategory:{
        DataStructures::chooseQuizCategoryRequest req{};

        readStruct(u,reinterpret_cast<char*>(&req)+sizeof(clientCommand),sizeof(req)-sizeof(clientCommand));

        Lobby* lobby = nullptr;

        for(int i = 0;i<lobbies.size();i++){
            printf("li:%d\n",i);
            auto currentLobby = lobbies[i];
            bool isPlayerContained = std::find(currentLobby->players.begin(),currentLobby->players.end(),u)!=currentLobby->players.end();
            if(isPlayerContained){
                lobby = currentLobby;
            }
        }
        if(lobby==nullptr){
            printf("return\n");
            return;
        }

        switch(req.categoryIndex){
        case 0:{
            lobby->quizName = DataStructures::quizNames::math;
            break;
        }
        case 1:{
            lobby->quizName = DataStructures::quizNames::reflex;
            break;
        }
        default:{
            lobby->quizName = DataStructures::quizNames::reflex;
        }
        };

        DataStructures::chooseQuizCategoryResultStruct qcr{};
        sendStruct(u,reinterpret_cast<char*>(&qcr),sizeof(qcr));

        break;
    }
    case DataStructures::joinLobby:{
        DataStructures::joinLobbyRequest req{};

        readStruct(u,reinterpret_cast<char*>(&req)+sizeof(clientCommand),sizeof(req)-sizeof(clientCommand));
        bool isLobbyFound = false;
        for(int i = 0;i<lobbies.size();i++){
            auto currentLobby = lobbies[i];
            std::string currentCode = std::to_string(currentLobby->id);
            if(currentCode==std::string(req.lobbyCode)){
                isLobbyFound = true;
                currentLobby->players.push_back(u);

                DataStructures::joinLobbyResultStruct j{};
                j.isSuccess = true;
                j.lobbyId = currentLobby->id;
                sendStruct(u,reinterpret_cast<char*>(&j),sizeof(j));
                break;
            }
        }
        if(!isLobbyFound){
            DataStructures::joinLobbyResultStruct j{};
            j.isSuccess = false;
            j.lobbyId = 0;
            sendStruct(u,reinterpret_cast<char*>(&j),sizeof(j));
        }
        break;
    }
    case DataStructures::getLobbyDetails:{
        for(int i = 0;i<lobbies.size();i++){
            auto currentLobby = lobbies[i];
            bool isPlayerContained = std::find(currentLobby->players.begin(),currentLobby->players.end(),u)!=currentLobby->players.end();
            if(isPlayerContained){
                updateLobbyViews(currentLobby);
            }
        }
        break;
    }
    case DataStructures::startGame:{
        Game * g = new Game(u, this);
        games.push_back(g);

        DataStructures::reflexQuiz q;
        g->totalQuestionsCount = q.qaBundles.size();

        for(int i = 0;i<lobbies.size();i++){
            auto currentLobby = lobbies[i];
            if(currentLobby->host==u){
                g->quizName = currentLobby->quizName;
                for(int k = 0;k<currentLobby->players.size();k++){
                    g->players.push_back(currentLobby->players[k]);
                }
                lobbies.erase(lobbies.begin()+i);
            }
        }

        g->scores = std::vector<int>(g->players.size());

        //todo: start counting time

        g->nextPhase();
        break;
    }
    case DataStructures::answerQuestion:{
        DataStructures::answerQuestionRequest req{};

        readStruct(u,reinterpret_cast<char*>(&req)+sizeof(clientCommand),sizeof(req)-sizeof(clientCommand));

        int gameIndex;
        int playerIndex;
        bool isGameFound = false;

        for(int i = 0;i<games.size();i++){
            auto playerIterator = std::find(games[i]->players.begin(),games[i]->players.end(),u);
            gameIndex = i;
            playerIndex = playerIterator-games[i]->players.begin();
            if(playerIterator!=games[i]->players.end()){
                isGameFound = true;
                break;
            }
        }


        if(isGameFound){
            Game* game = games[gameIndex];

            if(std::find(game->answerSentPlayers.begin(),game->answerSentPlayers.end(),u)!=game->answerSentPlayers.end()){
                return;
            }

            if(req.questionIndex==game->currentQuestionIndex){
                game->answerSentPlayers.push_back(game->players[playerIndex]);

                if(req.answerIndex == game->currentQuestionCorrectAnswerIndex){
                    game->scores[playerIndex]+=(1+game->players.size()-game->successfullCurrentQuestionAnswers++);
                }
            }

            if(game->answerSentPlayers.size()==game->players.size()){
                nextGamePhase(game);
            }
        }

        break;
    }
    case DataStructures::proposeNickname:{
        DataStructures::proposeNicknameRequest req{};

        readStruct(u,reinterpret_cast<char*>(&req)+sizeof(clientCommand),sizeof(req)-sizeof(clientCommand));

        std::string nickname = std::string(req.nickname);
        DataStructures::answerNicknameProposalStruct a{};
        auto iterator = std::find(this->nicknames.begin(),this->nicknames.end(),std::string(req.nickname));
        bool isNicknameContained = iterator!=this->nicknames.end();
        a.isApproved = !isNicknameContained;
        if(a.isApproved){
            this->nicknames.push_back(nickname);
            strncpy(u->nickname,nickname.c_str(),32);
        }

        sendStruct(u,reinterpret_cast<char*>(&a),sizeof(a));
        break;
    }
    default:{
        break;
    }
    }
}

void ServerLogic::nextGamePhase(Game* g){
    bool shouldSummarise = (1==g->nextPhase());
    if(shouldSummarise){
        g->shouldTimerTick=false;
        for(int i = 0;i<g->players.size();i++){
            User* u = g->players[i];
            DataStructures::gameSummaryStruct gs{};
            strncpy(gs.summaryString,g->summary.c_str(),sizeof(gs.summaryString));
            sendStruct(u,reinterpret_cast<char*>(&gs),sizeof(gs));
        }
        games.erase(std::find(games.begin(),games.end(),g));
    }
}

void ServerLogic::sendStruct(ServerLogic::User* user, char* data, int size){
    int sentSize = 0;

    while(sentSize<size){
        sentSize+= write(user->sock,data+sentSize,size-sentSize);
    }
}

void ServerLogic::readStruct(ServerLogic::User * user,char* buffer,int size){
    int redSize = 0;

    while(redSize<size){
        redSize+= read(user->sock,buffer,size-redSize);
        fflush(stdout);
    }
}

DataStructures::qaBundle ServerLogic::Game::getCorrectQaBundle(){
    switch(quizName){
    case DataStructures::math:{
        DataStructures::mathQuiz q;
        return q.qaBundles[currentQuestionIndex];
        break;
    }
    case DataStructures::reflex:{
        DataStructures::reflexQuiz q;
        return q.qaBundles[currentQuestionIndex];
        break;
    }
    default:{
        DataStructures::mathQuiz q;
        return q.qaBundles[currentQuestionIndex];
        break;
    }
    };
}

void ServerLogic::updateGames(){
    int gamesCount = games.size();
    auto now = std::chrono::high_resolution_clock::now();
    for(int i = 0;i<gamesCount;i++){
        if(games[i]->shouldTimerTick){
            auto questionDuration = std::chrono::duration_cast<std::chrono::seconds>(now-games[i]->questionStart);
            if(questionDuration.count()>=games[i]->secondsPerQuestion){
                nextGamePhase(games[i]);
            }
        }
    }
}
