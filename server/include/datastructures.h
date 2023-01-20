#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H


#include <map>
#include <string>
#include <vector>
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


    enum quizNames{
        math,
        reflex
    };

    struct qaBundle{
        std::string question;
        std::string answerA;
        std::string answerB;
        std::string answerC;
        std::string answerD;
        int correctAnswerIndex;
    };

    struct  mathQuiz{
        std::vector<qaBundle> qaBundles{
            {
                "1+1=",
                "0",
                "1",
                "2",
                "3",
                2
            },
            {
                "1+2=",
                "0",
                "1",
                "2",
                "3",
                3
            },
            {
                "1-1=",
                "0",
                "1",
                "2",
                "3",
                0
            },
            {
                "10+1=",
                "0",
                "11",
                "12",
                "35",
                1
            },
            {
                "33+1=",
                "0",
                "1",
                "211",
                "34",
                3
            }
        };
    };

    struct  reflexQuiz{
        std::vector<qaBundle> qaBundles{
            {
                "SELECT \"p\"!",
                "q",
                "p",
                "d",
                "b",
                1
            },
            {
                "SELECT \"11101101\"!",
                "11110101",
                "10110101",
                "10101101",
                "11101101",
                3
            },
            {
                "SELECT I!",
                "1",
                "|",
                "I",
                "]",
                2
            },
            {
                "SELECT \"LEFT-UP\"",
                "LEFT-DOWN",
                "RIGHT-UP",
                "LEFT-UP",
                "RIGHT-DOWN",
                2
            },
            {
                "SELECT \'",
                "`",
                "\"",
                ";",
                "\'",
                3
            }
        };
    };
};

#endif // DATASTRUCTURES_H
