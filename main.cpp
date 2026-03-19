#include <iostream>
#include <fstream>
#include <conio.h>
#include <chrono>
#include <thread>
#include <string>
#include <random>
#include <windows.h>
#include <iomanip>
#include <sstream>
#include <unordered_set>

#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define GRAY    "\033[90m"
#define RESET   "\033[0m"
#define UNDERLINE "\033[4m"

std::ifstream file("words2.txt", std::ios::binary);

namespace sc =  std::chrono;

void drawSentence(const std::string& sentence, const std::string& userInput, std::string time, int wpm){
    std::cout << "\033[H"; 
    
    std::cout << "--- TYPE THE TEXT BELOW ---" << "\n\n";
    int wc = 0;
    
    for (int i = 0; i < sentence.length(); i++) {
         if (i > 0 && sentence[i-1] == ' ')
                wc++;
            if(wc == 8){
                  std::cout << "\n";
                  wc = 0;  
            }
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
    
    std::cout << "\n\n Time elapsed:" << time << "; words/min: " << wpm << "        ";
    std::cout << "\n Ctrl+Q to quit. it will not affect your stats.     ";
}

void showMessage(std::string message){
   std::cout << message << std::flush;
   Sleep(250);
   system("cls");
   Sleep(250);
   std::cout << message << std::flush;
   Sleep(3000);
}

std::string wordGen(){
   file.clear();
   file.seekg(0, std::ios::end);
   long fileSize = file.tellg();
   if (fileSize <= 0) return "";

   static std::random_device rd;
   static std::mt19937 gen(static_cast<unsigned int>(
    std::chrono::high_resolution_clock::now().time_since_epoch().count()));
   
   std::uniform_int_distribution<long> dis(0, fileSize - 1);
   
   long randomPos = dis(gen);

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

void newGame(bool instantDeath = false){
   if(instantDeath)
      std::cout << "Good luck. Instant death mode... One mistake and that's it!\n";
      
   std::cout << "Insert player name:";
   std::string playerName;
   std::cin >> playerName;

   std::string sentence = sentenceGen();
   if (!sentence.empty()) sentence.pop_back();

   std::string userInput = "";
   system("cls");
   std::cout<< "\033[?25l";
   
   int cursor = 0;
   std::unordered_set<int> mistakes;

   drawSentence(sentence, userInput, "00:00", 0);
   while(!_kbhit());
   auto start = sc::steady_clock::now();

   do{

      auto now = sc::steady_clock::now();
      auto diff_ms = sc::duration_cast<sc::milliseconds>(now - start).count();

      int total_seconds = diff_ms / 1000;
      int min = total_seconds / 60;
      int sec = total_seconds % 60;
      std::stringstream ss;
      ss << std::setfill('0') << std::setw(2) << min << ":" << std::setfill('0') << std::setw(2) << sec;
      std::string timeStr = ss.str();
      
      int wpm = 0;
      if(diff_ms > 1000){
         double minutes = static_cast<double>(diff_ms) / 60000.0;
         double words = static_cast<double>(userInput.size()) / 5.0; 
         wpm = static_cast<long int>(words / minutes);
      }

      drawSentence(sentence, userInput, timeStr, wpm);

      if (_kbhit()){
         char lastChar= _getch();
         if (lastChar == 17) {
            system("cls");
            break;
         }
         if (lastChar == 8 && userInput.size() != 0) {
            userInput.pop_back();
            cursor--;
            mistakes.erase(cursor);
         }
         else{
            if(lastChar != sentence[cursor]){
               if(instantDeath){
                  system("cls");
                  std::cout << "Game over! You made a mistake.\n";
                  std::cout << "WPM: " << wpm << " | Time: " << timeStr << "\n";
                  Sleep(2000);
                  return;
               }
               mistakes.insert(cursor);
            }
            userInput += lastChar;
            cursor++;
         }
      }

      Sleep(10);

   }while(userInput.size() != sentence.size());
   
   system("cls");
   std::cout << "Congrats, you did it!\n";
   auto final_now = sc::steady_clock::now();
   double final_ms = sc::duration_cast<sc::milliseconds>(final_now - start).count();
   int final_wpm = static_cast<int>((userInput.size() / 5.0) / (final_ms / 60000.0));
   std::cout << "Final WPM: " << final_wpm << "\nPress any key to return to menu.";
   _getch();
   system("cls");
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
               newGame(true);
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