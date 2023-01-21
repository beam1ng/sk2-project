#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <string>



class DataStructures
{
public:
    DataStructures();

    enum clientCommands{
        createLobby,
        joinLobby,
        startGame,
        answerQuestion,
        proposeNickname,
        getLobbyDetails,
        chooseQuizCategory
    };

    enum serverCommands{
        lobbyDetails,
        joinLobbyResult,
        gameQuestion,
        gameSummary,
        answerNicknameProposal,
        quizCategories,
        chooseQuizCategoryResult,
        gameClosedByHost
    };

    struct createLobbyRequest{
        clientCommands commandIndex = DataStructures::createLobby;
    };

    struct joinLobbyRequest{
        clientCommands commandIndex = DataStructures::joinLobby;
        char lobbyCode[32];
    };

    struct startGameRequest{
        clientCommands commandIndex = DataStructures::startGame;
    };

    struct answerQuestionRequest{
        clientCommands commandIndex = DataStructures::answerQuestion;
        int questionIndex;
        int answerIndex;
    };

    struct proposeNicknameRequest{
        clientCommands commandIndex = DataStructures::proposeNickname;
        char nickname[32];
    };

    struct getLobbyDetailsStruct{
        clientCommands commandIndex = DataStructures::getLobbyDetails;
    };

    struct chooseQuizCategoryRequest{
        clientCommands commandIndex = DataStructures::chooseQuizCategory;
        int categoryIndex;
    };

    //hardcoded maximum 4 players in a lobby
    struct lobbyDetailsStruct{
        serverCommands commandIndex = DataStructures::lobbyDetails;
        char hostNickname[32] = {'\0'};
        char nicknames[4][32] = {'\0'};
    };

    struct joinLobbyResultStruct{
        serverCommands commandIndex = DataStructures::joinLobbyResult;
        int lobbyId;
        bool isSuccess;
    };

    struct gameQuestionStruct{
        serverCommands commandIndex = DataStructures::gameQuestion;
        char question[64];
        char answers[4][32] = {'\0'};
    };

    struct gameSummaryStruct{
        serverCommands commandIndex = DataStructures::gameSummary;
        char summaryString[256];
    };

    struct answerNicknameProposalStruct{
        serverCommands commandIndex = DataStructures::answerNicknameProposal;
        bool isApproved;
    };

    //hardcoded 8 category names
    struct quizCategoriesStruct{
        serverCommands commandIndex = DataStructures::quizCategories;
        char categoryNames[8][16];
    };

    struct chooseQuizCategoryResultStruct{
        serverCommands commandIndex = DataStructures::chooseQuizCategoryResult;
    };

    struct gameClosedByHostStruct{
        serverCommands commandIndex = DataStructures::gameClosedByHost;
    };

    struct clientQaBundle{
        std::string question;
        std::string answerA;
        std::string answerB;
        std::string answerC;
        std::string answerD;
    };
};

#endif // DATASTRUCTURES_H
