/*****************************************
Command line typing test created by Neal A.
4/29/2021 
*******************************************/
#include <iostream> // std::cout, std::cin
#include <string> // std::string
#include <fstream> // std::ifstream 
#include <chrono> // std::chrono::steady_clock::now()
#include <vector> // std::vector
#include <cstdlib> // rand()
#include <stdlib.h> // system("clear/cls")
#include <unistd.h> // usleep() ## 1000000 = 1s
#include <sstream> // std::stringstream
#include <iomanip> // std::setprecision

struct TypingStat {
    int wpm;
    double accuracy;
};

class EmptyWordList{}; // exception class

bool readFromFile(std::vector<std::string> *&wordList, const char *filename);
void clearScreen();
TypingStat typeTest(std::vector<std::string> *&wordList); 
std::ostream &operator<<(std::ostream &out, const TypingStat &userStat);

int main() {
    srand(time(NULL));
    std::vector<std::string> *wordList = new std::vector<std::string>; // store in words in vector for O(1) access
    std::vector<TypingStat> userStats;
    char input;
    const char *defaultWordFile = "default.txt";
    if(!readFromFile(wordList, defaultWordFile)) {
        std::cout << "Default File Not Opened" << std::endl;
    }
    std::string filenameInput;
    TypingStat gameStat;
    while(1) {
        std::cout << "===== Type Test =====" << std::endl;
        std::cout << "[A.] Start Type Test  " << std::endl;
        std::cout << "[B.] Load Custom List " << std::endl;
        std::cout << "[C.] View Stats       " << std::endl;
        std::cout << "[D.] End Program      " << std::endl;
        std::cout << "======================" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> input;
        std::cin.ignore(250, '\n'); // if additional characters are added skip over
        switch(input) {
            case('A'):
            case('a'):
                try {
                    gameStat = typeTest(wordList);
                    userStats.push_back(gameStat);
                    clearScreen();
                } catch (EmptyWordList emptyListError) {
                    std::cout << "Error: No words were generated " << std::endl;
                    std::cout << "Add a word list file into folder" << std::endl;
                }
                break;
            case('B'):
            case('b'):
                std::cout << "Enter filename: ";
                std::cin >> filenameInput;
                clearScreen();
                if(!readFromFile(wordList, filenameInput.c_str())) {
                    std::cout << filenameInput << " does not exist" << std::endl;                    
                } else {
                    std::cout << "Wordlist has been updated" << std::endl;
                }
                break;
            case('C'):
            case('c'):
                clearScreen();
                if(userStats.size() == 0) {
                    std::cout << "No Recorded Data" << std::endl;
                }
                for(int i = 0; i < userStats.size(); i++) {
                    std::cout << "| Game " << i + 1 << " |" << std::endl;
                    std::cout << userStats[i] << std::endl;
                }
                break;
            case('D'):
            case('d'):
                return 0;
            default:
                std::cout << "Invalid Input. \nEnter choice: " << std::endl;
                break;
        }
    }
    return 0;
}

/*********************************************************
const char string instead of std::string to skip string to
c-string conversion to open file
std::vector<std::string> *wordList pointer object
passed by reference to avoid reconstruction

Return true if file was opened, false if not
***********************************************************/
bool readFromFile(std::vector<std::string> *&wordList, const char *filename) {
    if(wordList->size() != 0) { // empty the vector if a new list is added
        wordList->clear(); 
    }
    std::ifstream fin;
    fin.open(filename);
    if(!fin.is_open()) {
        return false;
    }
    std::string words;
    while(!fin.eof()) {
        fin >> words;
        wordList->push_back(words);
    }
    return true;
}

/********************************************************
const std::vector<std::string> *&wordList - pointer
object pass by reference so no reconstuction
const no changes can be made
********************************************************/
TypingStat typeTest(std::vector<std::string> *&wordList) {
    if(wordList->size() == 0) {
        throw EmptyWordList();
    }

    std::string word, userInput; 
    // word stores a single word from stringstream userInput is the user's typing
    std::vector<std::string> randomWords; // store words to be typed and compared
    unsigned int listSize = wordList->size(), numOfWords = rand() % 15 + 10; // generate the number of words to be displayed on screen
    TypingStat userStat;
    for(int i = 0; i < numOfWords; i++) { // generate random numbers into the vector
        randomWords.push_back(wordList->at(rand() % listSize));
    }
    clearScreen();
    std::cout << "Get ready to type in: " << std::endl;
    for(int i = 5; i > 0; i--) { // countdown
        std::cout << i << std::endl;
        usleep(1000000);
    }
    std::cout << "=========================================================================" << std::endl;
    for(int i = 0; i < randomWords.size(); i++) {
        if(i != 0 && i % 9 == 0) std::cout << std::endl;
        std::cout << randomWords[i] << " ";
    }
    std::cout << "\n=========================================================================" << std::endl;
    auto start = std::chrono::steady_clock::now();
    getline(std::cin, userInput);
    auto end = std::chrono::steady_clock::now(); 
    // while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < 60)); future reference
    std::chrono::duration<double> timeElapsed = end - start;
    double timeElapsedCount = timeElapsed.count(); 
    
    std::stringstream ss(userInput);
    int tempctr = 0, missed = 0, correct = 0;// tempctr used to access randWords vector for comparison and the number of words entered
    while(ss >> word) {
        randomWords[tempctr++] != word ? missed++ : correct++;
    }
    userStat.accuracy = (100.0 * ((numOfWords - (double)missed) / (double)numOfWords));
    userStat.wpm = (tempctr / (double)(timeElapsedCount / 60.0)); // convert timeElapsed 
    std::cout << "\n=========================================================================" << std::endl;
    std::cout << "Time taken: " << std::setprecision(2) << std::fixed << timeElapsedCount << "s" << std::endl;
    std::cout << "Total number of words: " << numOfWords << std::endl;
    std::cout << "Number of words typed: " << tempctr << std::endl;
    std::cout << "Correct: " << correct << std::endl;
    std::cout << "Missed: " << missed << std::endl;
    std::cout << userStat << std::endl;
    std::cout << "Press enter to continue: ";
    std::cin.ignore();
    return userStat;
}

std::ostream &operator<<(std::ostream &out, const TypingStat &userStat) {
    out << "==== Stats ====" << std::endl;
    out << "WPM: " << userStat.wpm << std::endl;
    out << "Accuracy: " << std::setprecision(2) << std::fixed << userStat.accuracy << "%" << std::endl;
    out << "===============" << std::endl;
    return out;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}