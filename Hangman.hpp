#ifndef HANGMAN_HPP
#define HANGMAN_HPP

#include <string>
#include <vector>
#include <map>

class HangmanGame {
private:
    std::string word;                    // Tahmin edilecek kelime
    std::string currentState;            // Kelimenin mevcut durumu (_A__E gibi)
    std::vector<char> guessedLetters;    // Tahmin edilen harfler
    int remainingAttempts;               // Kalan tahmin hakkı
    bool isActive;                       // Oyun aktif mi?
    std::vector<std::string> wordList;   // Kelime listesi

    void initializeWordList();

public:
    HangmanGame();
    void startNewGame();
    std::string makeGuess(char letter);
    bool isGameActive() const { return isActive; }
    std::string getCurrentState() const { return currentState; }
    int getRemainingAttempts() const { return remainingAttempts; }
    std::string getGuessedLetters() const;
    std::string getHangmanArt() const;
};

#endif