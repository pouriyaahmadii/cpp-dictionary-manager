#include "dictionary.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// Function definitions
void insertNode(Node*& head, Node* newNode) {
  if (head == nullptr || newNode->word < head->word) {
    newNode->next = head;
    head = newNode;
    return;
  }

  Node* current = head;
  while (current->next != nullptr && current->next->word < newNode->word) {
    current = current->next;
  }
  newNode->next = current->next;
  current->next = newNode;
}

void removeNode(Node*& head, Node* node) {
  if (head == node) {
    head = head->next;
    delete node;
    return;
  }

  Node* current = head;
  while (current->next != nullptr && current->next != node) {
    current = current->next;
  }

  if (current->next != nullptr) {
    current->next = current->next->next;
    delete node;
  }
}

Node* searchNode(Node* head, const string& word) {
  Node* current = head;
  while (current != nullptr && current->word != word) {
    current = current->next;
  }
  return current;
}

string updateSynonyms(const string& synonyms, const string& synonymToRemove, const std::string& newWord) {
  string updatedSynonyms;
  stringstream ss(synonyms);
  string word;

  while (getline(ss, word, ',')) {
    if (word != synonymToRemove) {
      if (!updatedSynonyms.empty()) {
        updatedSynonyms += ",";
      }
      updatedSynonyms += newWord; // Use the provided newWord
    }
  }
  return updatedSynonyms;
}

void addWord(Node*& head, const string& word, const string& synonyms) {
  // Check if the word already exists
  if (searchNode(head, word)) {
    cout << "Word already exists. Adding synonym..." << endl;
    addSynonym(head, word, synonyms);
    return;
  }

  Node* newNode = new Node(word, synonyms);
  insertNode(head, newNode);
}

void addSynonym(Node*& head, const string& word, const string& synonym) {
  Node* node = searchNode(head, word);

  if (node) {
    node->synonyms += "," + synonym;
    } else {
    cout << "Word not found." << endl;
  }
}

void deleteWord(Node*& head, const string& word) {
  Node* nodeToDelete = searchNode(head, word);

  if (nodeToDelete) {
    removeNode(head, nodeToDelete);
  } else {
    cout << "Word not found." << endl;
  }
}

void deleteSynonym(Node*& head, const string& word, const string& synonym) {
  Node* node = searchNode(head, word);

  if (node) {
    string updatedSynonyms = updateSynonyms(node->synonyms, synonym);

    if (updatedSynonyms.empty()) {
      deleteWord(head, word);
    } else {
      node->synonyms = updatedSynonyms;
    }
  } else {
    cout << "Word not found." << endl;
  }
}

// ... rest of the code ...
void searchWord(Node*& head, const string& word) {
  Node* current = head;
  while (current != nullptr) {
    // Check if the word or any synonym matches the search term
    if (current->word == word || current->synonyms.find(word) != string::npos) {
      cout << "Word: " << current->word << endl;
      cout << "Synonyms: " << current->synonyms << endl;
      return; // Found a match, exit the loop
    }
    current = current->next;
  }
  cout << "Word not found." << endl;
}

void displayAllWords(Node*& head) {
  // Traverse the linked list and print each word and its synonyms
  Node* current = head;
  while (current) {
    cout << "Word: " << current->word << endl;
    cout << "Synonyms: " << current->synonyms << endl;
    current = current->next;
  }
}

void changeSpelling(Node*& head, const string& oldWord, const string& newWord) {
  Node* node = searchNode(head, oldWord);

  if (node) {
    // Update the word spelling only
    node->word = newWord;
    cout << "Spelling of word '" << oldWord << "' changed to '" << newWord << "'." << endl;
  } else {
    cout << "Word not found." << endl;
  }
}

// Function to save the dictionary to a file
void saveToFile(Node*& head, const string& filename) {
  ofstream outfile(filename);

  // Write each word and its synonyms to the file
  Node* current = head;
  while (current) {
    outfile << current->word << "," << current->synonyms << endl;
    current = current->next;
  }

  outfile.close();
  cout << "Dictionary saved to file: " << filename << endl;
}

// Function to read the dictionary from a file (fixed to avoid duplicates)
void readFromFile(Node*& head, const string& filename) {
  ifstream infile(filename);

  if (!infile.is_open()) {
    cout << "Error opening file: " << filename << endl;
    return;
  }

  string line;
  while (getline(infile, line)) {
    // Split the line into word and synonyms (excluding the first token)
    vector<string> tokens;
    string word;
    stringstream ss(line);
    getline(ss, word, ','); // Get the word (first token)
    while (getline(ss, word, ',')) {
      // Trim leading and trailing spaces from each synonym
      string trimmedSynonym = trim(word);

      // Check if synonym already exists in the list for this word
      bool synonymExists = false;
      for (const string& existingSynonym : tokens) {
        if (existingSynonym == trimmedSynonym) {
          synonymExists = true;
          break;
        }
      }

      if (!synonymExists) {
        tokens.push_back(trimmedSynonym);
      }
    }

    // Check if word already exists
    Node* existingNode = searchNode(head, word);

    if (existingNode) {
      // Update synonyms for existing node
      string updatedSynonyms;
      for (const string& synonym : tokens) {
        if (!updatedSynonyms.empty()) {
          updatedSynonyms += ",";
        }
        updatedSynonyms += synonym;
      }
      existingNode->synonyms = updatedSynonyms;
    } else {
      // Create and add new node if word doesn't exist
      Node* newNode = new Node(word);
      if (!tokens.empty()) {
        newNode->synonyms = tokens[0];
        for (int i = 1; i < tokens.size(); ++i) {
          newNode->synonyms += "," + tokens[i];
        }
      }
      insertNode(head, newNode);
    }
  }

  infile.close();
  cout << "Dictionary loaded from file: " << filename << endl;
}

// Function to trim leading and trailing spaces from a string
std::string trim(std::string& str) {
    // Remove leading spaces
  str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char c) {
    return !std::isspace(c);
  }));

  // Remove trailing spaces
  str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char c) {
    return !std::isspace(c);
  }).base(), str.end());

  return str;
}

// Function to prompt the user to save before exiting
bool exitProgram(Node*& head) {
  char choice;
  cout << "Do you want to save the dictionary before exiting (y/n)? ";
  cin >> choice;

  if (choice == 'y' || choice == 'Y') {
    string filename;
    cout << "Enter filename to save: ";
    cin >> filename;
    saveToFile(head, filename);
  }

  return true;
}

int main() {
  Node* head = nullptr;

  while (true) {
    cout << endl
         << "======================" << endl;
    cout << "\nDictionary Menu:" << endl;
    cout << "1. Add Word and Synonyms" << endl;
    cout << "2. Delete Word" << endl;
    cout << "3. Delete Synonym" << endl;
    cout << "4. Search for Word" << endl;
    cout << "5. Display All Words" << endl;
    cout << "6. Change Spelling of Word" << endl;
    cout << "7. Save to File" << endl;
    cout << "8. Read from File" << endl;
    cout << "9. Exit" << endl;

    int choice;
    cout << "Enter your choice: ";
    cin >> choice;

    cout << endl
         << "======================" << endl;

    switch (choice) {
      case 1: {
        string word, synonyms;
        cin >> word;
        cout << "Enter synonyms (comma-separated): ";
        cin.ignore(); // Consume newline character
        getline(cin, synonyms);
        addWord(head, word, synonyms);
        break;
      }
      case 2: {
        string word;
        cout << "Enter word to delete: ";
        cin >> word;
        deleteWord(head, word);
        break;
      }
      case 3: {
        string word, synonym;
        cout << "Enter word: ";
        cin >> word;
        cout << "Enter synonym to delete: ";
        cin >> synonym;
        deleteSynonym(head, word, synonym);
        break;
      }
      case 4: {
        string word;
        cout << "Enter word to search: ";
        cin >> word;
        searchWord(head, word);
        break;
      }
      case 5:
        displayAllWords(head);
        break;
      case 6: {
        string oldWord, newWord;
        cout << "Enter word to change spelling: ";
        cin >> oldWord;
        cout << "Enter new spelling: ";
        cin >> newWord;
        changeSpelling(head, oldWord, newWord);
        break;
      }
      case 7: {
        string filename;
        cout << "Enter filename to save: ";
        cin >> filename;
        saveToFile(head, filename);
        break;
      }
      case 8: {
        string filename;
        cout << "Enter filename to read: ";
        cin >> filename;
        readFromFile(head, filename);
        break;
      }
      case 9:
        if (exitProgram(head)) {
          return 0;
        }
        break;
      default:
        cout << "Invalid choice." << endl;
    }
  }

  return 0;
}

