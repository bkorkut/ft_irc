#include "Hangman.hpp"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <sstream>

void HangmanGame::initializeWordList() {
    wordList.push_back("IRC");
    wordList.push_back("SERVER");
    wordList.push_back("CHANNEL");
    wordList.push_back("NETWORK");
    wordList.push_back("MESSAGE");
    wordList.push_back("COMMAND");
    wordList.push_back("PASSWORD");
    wordList.push_back("NICKNAME");
    wordList.push_back("USERNAME");
    wordList.push_back("OPERATOR");
}

HangmanGame::HangmanGame() : remainingAttempts(6), isActive(false) {
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    initializeWordList();
}

void HangmanGame::startNewGame() {
    // rastgele kelime seçiyorum
    int index = std::rand() % wordList.size();
    word = wordList[index];
    
    // kelimeyi büyük harfe çeviriyor (türkçe karakterler hariç)
    for (std::string::iterator it = word.begin(); it != word.end(); ++it) {
        *it = std::toupper(*it);
    }
    
    // oyunu burda başlatıyorum
    currentState = std::string(word.length(), '_');
    guessedLetters.clear();
    remainingAttempts = 6;
    isActive = true;
}

std::string HangmanGame::makeGuess(char letter) {
    letter = std::toupper(letter);
    
    // harf eskiden tahmin edilmişmi kontrol ediliyor
    for (std::vector<char>::iterator it = guessedLetters.begin(); it != guessedLetters.end(); ++it) {
        if (*it == letter) {
            return "Bu harfi zaten tahmin ettiniz!";
        }
    }
    
    guessedLetters.push_back(letter);
    
    // tahmin edilen harf seçilen kelimede varmı
    bool found = false;
    for (size_t i = 0; i < word.length(); ++i) {
        if (word[i] == letter) {
            currentState[i] = letter;
            found = true;
        }
    }
    
    if (!found) {
        remainingAttempts--;
    }
    
    // oyun bittimi kontrolü
    if (currentState == word) {
        isActive = false;
        return "Tebrikler! Kelimeyi buldunuz: " + word;
    }
    
    if (remainingAttempts <= 0) {
        isActive = false;
        return "Oyun bitti! Kelime: " + word;
    }
    
    return found ? "Doğru tahmin!" : "Yanlış tahmin!";
}

std::string HangmanGame::getGuessedLetters() const {
    std::string result = "Tahmin edilen harfler: ";
    for (std::vector<char>::const_iterator it = guessedLetters.begin(); it != guessedLetters.end(); ++it) {
        result += *it;
        result += " ";
    }
    return result;
}

std::string HangmanGame::getHangmanArt() const {
    const char* hangmanStates[] = {
        "\n  +---+\n  |   |\n      |\n      |\n      |\n      |\n=========",  // 6 hak
        "\n  +---+\n  |   |\n  O   |\n      |\n      |\n      |\n=========",  // 5 hak
        "\n  +---+\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========",  // 4 hak
        "\n  +---+\n  |   |\n  O   |\n /|   |\n      |\n      |\n=========",  // 3 hak
        "\n  +---+\n  |   |\n  O   |\n /|\\  |\n      |\n      |\n=========",  // 2 hak
        "\n  +---+\n  |   |\n  O   |\n /|\\  |\n /    |\n      |\n=========",  // 1 hak
        "\n  +---+\n  |   |\n  O   |\n /|\\  |\n / \\  |\n      |\n========="   // 0 hak
    };
    
    return std::string(hangmanStates[6 - remainingAttempts]);
}