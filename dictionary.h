#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <iostream>
#include <string>
#include <vector>

// Structure to represent a node in the linked list
struct Node {
  std::string word;
  std::string synonyms;
  Node* next;

  Node(const std::string& word, const std::string& synonyms = "");
};

// Function prototypes
void insertNode(Node*& head, Node* newNode);
void removeNode(Node*& head, Node* node);
Node* searchNode(Node* head, const std::string& word);
std::string updateSynonyms(const std::string& synonyms, const std::string& synonymToRemove, const std::string& newWord = "");
void addWord(Node*& head, const std::string& word, const std::string& synonyms);
void addSynonym(Node*& head, const std::string& word, const std::string& synonym);
void deleteWord(Node*& head, const std::string& word);
void deleteSynonym(Node*& head, const std::string& word, const std::string& synonym);
void searchWord(Node*& head, const std::string& word);
void displayAllWords(Node*& head);
void changeSpelling(Node*& head, const std::string& oldWord, const std::string& newWord);
void saveToFile(Node*& head, const std::string& filename);
void readFromFile(Node*& head, const std::string& filename);
bool exitProgram(Node*& head);

// Function to trim leading and trailing spaces from a string (defined in this file)
std::string trim(std::string& str);

#endif
