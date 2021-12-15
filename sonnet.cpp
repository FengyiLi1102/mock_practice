#include <iostream>
#include <cstring>
#include <cctype>
#include <cassert>
#include <map>
#include <string>

using namespace std;

#include "sonnet.hpp"

const int MAX_NUMBER_WORD = 512;

/* PRE-SUPPLIED HELPER FUNCTIONS START HERE */

/* NOTE: It is much more important to understand the interface to and
   the "black-box" operation of these functions (in terms of inputs and
   outputs) than it is to understand the details of their inner working. */

/* get_word(...) retrieves a word from the input string input_line
   based on its word number. If the word number is valid, the function
   places an uppercase version of the word in the output parameter
   output_word, and the function returns true. Otherwise the function
   returns false. */

bool get_word(const char *input_line, int word_number, char *output_word) {
  char *output_start = output_word;
  int words = 0;

  if (word_number < 1) {
    *output_word = '\0';
    return false;
  }
  
  do {
    while (*input_line && !isalnum(*input_line))
      input_line++;

    if (*input_line == '\0')
      break;

    output_word = output_start;
    while (*input_line && (isalnum(*input_line) || *input_line=='\'')) {
      *output_word = toupper(*input_line);
      output_word++;
      input_line++;
    }
    *output_word = '\0';

    if (++words == word_number)
      return true;

  } while (*input_line);

  *output_start = '\0';
  return false;
}

/* char rhyming_letter(const char *ending) generates the rhyme scheme
   letter (starting with 'a') that corresponds to a given line ending
   (specified as the parameter). The function remembers its state
   between calls using an internal lookup table, such that subsequents
   calls with different endings will generate new letters.  The state
   can be reset (e.g. to start issuing rhyme scheme letters for a new
   poem) by calling rhyming_letter(RESET). */

char rhyming_letter(const char *ending) {

  // the next rhyming letter to be issued (persists between calls)
  static char next = 'a';
  // the table which maps endings to letters (persists between calls)
  static map<string, char> lookup;

  // providing a way to reset the table between poems
  if (ending == RESET) {
    lookup.clear();
    next = 'a';
    return '\0';
  }

  string end(ending);

  // if the ending doesn't exist, add it, and issue a new letter
  if (lookup.count(end) == 0) {
    lookup[end]=next;
    assert(next <= 'z');
    return next++;
  }

  // otherwise return the letter corresponding to the existing ending
  return lookup[end];
}

/* START WRITING YOUR FUNCTION BODIES HERE */
int count_words(char* input_line) {
    int n = 1;
    char word[MAX_NUMBER_WORD];

    while (get_word(input_line, n, word)) {
        n++;
    }

    return (n-1);
}

bool inVowel(char c) {
    if (c == 'A' || c == 'E' || c == 'I' || c =='O' || c == 'U') return true;
    else return false;
}

bool find_phonetic_ending(const char *word, char* phonetic_ending) {
    ifstream in_stream;
    in_stream.open("dictionary.txt");


    char c;
    string oneWord;
    string thisWord = word;

    while (in_stream.get(c)) {
        if (c != ' ') oneWord.push_back(c);
        else if (c == ' ') {
            if (oneWord == thisWord) {
                string ending;
                bool flag = false;

                while (in_stream.get(c)) {
                    if (c == '\n') break;

                    if (c == ' ') continue;

                    if (inVowel(c)) {
                        if (flag) ending = "";
                        ending.push_back(c);
                        flag = true;
                        continue;
                    }

                    if (flag) ending.push_back(c);
                }

                for (int x = 0; x < ending.length(); x++) {
                    phonetic_ending[x] = ending.at(x);
                }
                return true;
            } else {
                while (in_stream.get(c)) {
                    if (c == '\n') {
                        oneWord = "";
                        break;
                    }
                }
            }
        }
    }

    in_stream.close();
    return false;
}


bool find_rhyme_scheme(char *filename, char *scheme) {
    ifstream in_stream;
    in_stream.open(filename);

    if (in_stream.fail()) {
        return false;
    }

    char oneLine[MAX_NUMBER_WORD];
    char word[MAX_NUMBER_WORD];
    char ending[MAX_NUMBER_WORD];
    int n = 0;
    rhyming_letter(RESET);

    while (cin.getline(oneLine, MAX_NUMBER_WORD)) {
        for (int i = MAX_NUMBER_WORD - 1; i >= 0; i--) {
            if (get_word(oneLine, i, word)) break;
        }

        find_phonetic_ending(word, ending);

        scheme[n] = rhyming_letter(ending);

        n++;
    }

    in_stream.close();
    return true;
}


string identify_sonnet(char *filename) {
    char scheme[MAX_NUMBER_WORD];
    char type[MAX_NUMBER_WORD];
    bool success;

    success = find_rhyme_scheme(filename, scheme);

    if (success) {
        if (!strcmp(scheme, "ababcdcdefefgg")) {
            strcpy(type, "Shakespearean");
            return type;
        } else if (!strcmp(scheme, "abbaabbacdcdcd")) {
            strcpy(type, "Petrarchan");
            return type;
        } else if (!strcmp(scheme, "ababbcbccdcdee")) {
            strcpy(type, "Spenserian");
            return type;
        } else
            return "UNKNOWN";
    } else {
        cerr << "Unsuccessful to open the file" << endl;
    }

    return " ";

}