#include <iostream>
#include <fstream>
#include <conio.h>
#include <chrono>
#include <thread>
#include <string>
#include <random>
#include <windows.h>
#include <iomanip>


#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define GRAY    "\033[90m"
#define RESET   "\033[0m"
#define UNDERLINE "\033[4m"

std::ifstream file("words2.txt", std::ios::binary);

namespace sc =  std::chrono;

void drawSentence(const std::string& sentence, const std::string& userInput, std::string time, int wpm) {
    std::cout << "\033[H"; 
    
    std::cout << "--- TYPE THE TEXT BELOW ---" << "\n\n";

    for (int i = 0; i < sentence.length(); i++) {
        if (i < userInput.length()) {
            // Check if the character typed matches the sentence
            if (userInput[i] == sentence[i]) {
                std::cout << GREEN << sentence[i] << RESET;
            } else {

                std::cout << "\033[41m" << sentence[i] << RESET; 
            }
        } else if (i == userInput.length()) {
            std::cout << UNDERLINE << sentence[i] << RESET;
        } else {
            std::cout << GRAY << sentence[i] << RESET;
        }
    }
    
    std::cout << "\n\n Time elapsed:" << time << "; words/min: " << wpm;
    std::cout << "\nCtrl+Q to quit. it will not affect your stats.";
}

void showMessage(std::string message){
   std::cout << message << std::flush;
   Sleep(250);
   system("cls");
   Sleep(250);
   std::cout << message << std::flush;
   Sleep(250);
   system("cls");
   Sleep(250);
   std::cout << message << std::flush;
   Sleep(250);
   system("cls");
   Sleep(250);
   std::cout << message << std::flush;
   Sleep(3000);
   system("cls");
}

std::string wordGen(){
   file.clear();
   file.seekg(0, std::ios::end);
   long fileSize = file.tellg();
   if (fileSize <= 0) return "";

   long randomPos = ((rand() << 15) | rand()) % fileSize;
   
   //walkback
   char ch;
   while(randomPos > 0){
      file.seekg(randomPos - 1);
      file.get(ch);
      if (ch == '\n')
          break;
      randomPos--;
   }

   file.seekg(randomPos);
   std::string word;
   std::getline(file,word);

   if (!word.empty() && word.back() == '\r') {
    word.pop_back();
   }

   return word;
}


std::string sentenceGen(){
   std::string sentence = "";
   for(int i = 0; i < 20; i++){
      std::string word = wordGen();
      sentence +=  (word + " ");
   }
   return sentence;
}

void newGame(){
   std::cout << "Insert player name:";
   std::string playerName;
   std::cin >> playerName;
   std::string sentence = sentenceGen();
   std::string userInput = "";

   drawSentence(sentence, userInput, "00:00", 0);

   int wordCursor = 0, cursor = 0;
   auto start = sc::steady_clock::now();
   
   while(userInput.size() != sentence.size()-1){

      auto now = sc::steady_clock::now();
      auto diff = sc::duration_cast<sc::seconds>(now - start).count();

      int min = diff / 60;
      int sec = diff % 60;

      std::stringstream ss;
      ss << std::setfill('0') << std::setw(2) << min << ":" << std::setfill('0') << std::setw(2) << sec;
      std::string timeStr = ss.str();
      
      long wpm, rt;
      rt = min*60 + sec;
      if(rt != 0)
         wpm = (long)(wordCursor/(rt/60.0));
      else
         wpm = 0;
      drawSentence(sentence, userInput, timeStr, wpm);

      if (_kbhit()){
         char lastChar= _getch();
         if (lastChar == 17)
            system("cls");
            break;
         if (lastChar == 8 && userInput.size() != 0){
            userInput.pop_back();
            cursor--;
         }
         else{
         userInput += lastChar;
         if (lastChar == ' '){
            wordCursor++;
         }
         cursor++;
         }
      }
   }

}

void newIDGame(){

}

void showStats(){

}

void printMenu(){
   std:: cout << "___________________________________________\n";
   std:: cout << "  _____                           \n |_   _|_ _ _ __ ___ _ __ __ _  ___ ___ _ __\n   | | | | | '_ \\ / _ \\ '__/ _` |/ __/ _ \\ '__|\n   | | |_| | |_) |  __/ | | (_| | (_|  __/ |\n   |_|\\__, | .__/ \\___|_|  \\__,_|\\___\\___|_|\n      |___/|_|" << std::endl;
   std:: cout << "___________________________________________\n";
   std:: cout << "Please make a choice:\n";
   std:: cout << "Press '1' for a new game.\n";
   std:: cout << "Press '2' for a new instant-death game.\n";
   std:: cout << "Press '3' to see your stats.\n";
   std:: cout << "Press '4' to quit.\n";
}

int main(){
   char choice;
   printMenu();
   while(true){
      if (_kbhit()){
         system("cls");
         choice = _getch();
         switch(choice){
            case '1':
               newGame();
               break;
            case '2':
               newIDGame();
               break;
            case '3':
               showStats();
               break;
            case '4':
               showMessage("Goodbye!\n");
               return 0;
            default:
               break;
         }
         printMenu();
      }
   }

   return 0;
}
