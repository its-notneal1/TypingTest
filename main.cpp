#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <chrono>

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

void clearScreen();
void countDown(); // count down
void typingGame(int& wordsPerMinute, double& accuracy, std::string sentence);
void viewStats(const std::vector<int> wordsPerMinute, const std::vector<double> accuracy);
int numberOfWords(const std::string sentence);

class Sentences {
public:
    Sentences() {
        sentenceList.push_back("Your time is limited, so don't waste it living someone else's life. Don't be trapped by dogma - which is living with the results of other people's thinking. - Steve Jobs");
        sentenceList.push_back("If life were predictable it would cease to be life, and be without flavor. - Eleanor Roosevelt");
        sentenceList.push_back("Fear is the path to the dark side. Fear leads to anger. Anger leads to hate. Hate leads to suffering. - Yoda");
    }
    std::string chooseSentence() {
        srand(time(NULL));
        int index = rand() % sentenceList.size();
        return sentenceList[index];
    }
private:
    std::vector<std::string> sentenceList;
};


int main() {
    Sentences sentencesClass;
    char response;

    // game statistics variables
    std::vector<int> wordsPerMinute_vec;
    std::vector<double> accuracy_vec;

    int wordsPerMinute;
    double accuracy;

    while(1) {
        wordsPerMinute = 100;
        accuracy = 100;
        std::cout << "|------------------------|" << std::endl;
        std::cout << "| Neal's C++ Typing Game |" << std::endl;
        std::cout << "|------------------------|" << std::endl;
        std::cout << "| [A.] Play Game         |" << std::endl;
        std::cout << "| [B.] View Stats        |" << std::endl;
        std::cout << "| [C.] Exit Application  |" << std::endl;
        std::cout << "|________________________|" << std::endl;
        std::cout << "Enter Response: ";
        std::cin >> response;
        std::cin.ignore(250, '\n');

        switch(response) {

            case('A'):
            case('a'):
                typingGame(wordsPerMinute, accuracy, sentencesClass.chooseSentence());
                wordsPerMinute_vec.push_back(wordsPerMinute);
                accuracy_vec.push_back(accuracy);
                break;
            case('B'):
            case('b'):
                viewStats(wordsPerMinute_vec, accuracy_vec);
                break;
            case('C'):
            case('c'):
                std::cout << "Goodbye!" << std::endl;
                return 0;
                break;
            default:
                std::cout << "No Option Selected!" << std::endl;
                std::cin.clear();
                break;
        }
    }
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void countDown() {
    #ifdef _WIN32
            sleep(1000000);
    #else
        usleep(1000000);
    #endif
}

void typingGame(int& wordsPerMinute, double& accuracy, std::string sentence) {
    std::string userText; // the user input
    int totalChar = 0, correct = 0, incorrect = 0;
    double game_duration;

    int numberOfWords_userText;

    std::cout << "You Will Be Given 120 Seconds To Complete This Sentence So Get Ready!" << std::endl;
    std::cout << "The Game Will Begin In: " << std::endl;
    for (int i = 5; i > 0; i--) {
        std::cout << i << std::endl;
        countDown();
    }
    std::cout << "Begin: " << std::endl;
    std::cout << "-------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << sentence << std::endl;
    std::cout << "-------------------------------------------------------------------------------------------------" << std::endl;

    auto start = std::chrono::steady_clock::now();
    std::cout << "Begin Typing: " << std::endl;
    getline(std::cin, userText);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedTime_seconds = end - start;
    game_duration = elapsedTime_seconds.count();
    std::cout << "Elapsed Time: " << game_duration << std::endl;

    // now to compare for accuracy

    for (std::string::size_type i = 0; i < sentence.length(); i++) {
        if (sentence[i] == userText[i]) {
            correct++;
        }
        else {
            incorrect++;
            std::cout << i << " - " << userText[i] << std::endl;
        }
    }
    clearScreen();
    std::cout << "The Sentence: " << sentence << std::endl;
    std::cout << "Your Typing: " << userText << std::endl;
    // calculate accuracy
    totalChar = correct + incorrect;
    accuracy *= (1.0 * correct) / (1.0 * totalChar);
    std::cout << "Correct Letters: " << correct << std::endl;
    std::cout << "Incorrect Letters: " << incorrect << std::endl;
    std::cout << "Your Accuracy: " << accuracy << "%" << std::endl;

    // calculate speed / WPM
    wordsPerMinute = numberOfWords(userText) / (game_duration / 60); // multiply 60 to 
    std::cout << "Your Words Per Minute: " << wordsPerMinute << " WPM" << std::endl;
}

void viewStats(const std::vector<int> wordsPerMinute, const std::vector<double> accuracy) {
    clearScreen();
    if (wordsPerMinute.size() == 0) {
        std::cout << "No Games Found" << std::endl;
        return;
    }
    for (int i = 0; i < wordsPerMinute.size(); i++) {
        std::cout << "----------------------------" << std::endl;
        std::cout << "Game " << i + 1 << std::endl;
                std::cout << "Accuracy: " << accuracy[i] << "%" << std::endl;
        std::cout << "Words Per Minute: " << wordsPerMinute[i] << "WPM" << std::endl;
        std::cout << "----------------------------" << std::endl;
    }
}

int numberOfWords(const std::string sentence) {
    int numWords = 1;
    for (std::string::size_type i = 0; i < sentence.size(); i++) {
        if (sentence[i] == ' ' && sentence[i + 1] != '-' && sentence[i + 1] != '&' && sentence[i + 1] != '/') {
            numWords++;
        }
    }
    return numWords;
}