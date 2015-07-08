/* 
 * File:   Filter.cpp
 * Author: mcortes
 * 
 * Created on May 6, 2015, 4:10 PM
 */

#include "Filter.h"

Filter::Filter() {
    this->getMapStopWords();
}

int charToLower(char* letra) {
    if ((*letra >= 'A') && (*letra <= 'Z')) {
        *letra -= ('A' - 'a');
    }
}


//Filtra todo signo que se encuentre al final de la palabra, por ejemplo "hola, como estas?"
//Poss: "hola como estas"
void Filter::signs(string &palabra) {
    string nuevaPalabra;
    bool dosLetras = false;
    for (int i = 0; i < palabra.size(); i++) {
        char unaLetra = palabra.at(i);
        
        if (unaLetra >= 'A' && unaLetra <= 'Z') {
            charToLower(&unaLetra);
        }
        if (unaLetra >= 'a' && unaLetra <= 'z') { //Guarda una letra entre [a-z]            
            nuevaPalabra.push_back(unaLetra);
        }
    }
    
    palabra = nuevaPalabra;
}

void Filter::getMapStopWords() {

    JSON::Value jsonFile = parse_file(STOPWORDSFILE);
    JSON::Array jsonWords = ((jsonFile[TAGWORDS]).operator JSON::Array());

    for (vector<JSON::Value>::iterator it = jsonWords.begin(); it != jsonWords.end(); it++) {
        this->stopWords[it[0].as_string()] = 0;
    }

}

bool Filter::filterIsStopWords(string word) {

    if (word.size() <= 2) {
        return true;
    }

    map<string, int>::iterator endMap = this->stopWords.end();
    map<string, int>::iterator iterMap = this->stopWords.find(word);

    if (iterMap != endMap) {
        return true;
    }

    return false;
}

void Filter::filterStopWords(list<string>* listWords) {

    map<string, int>::iterator endMap = this->stopWords.end();

    list<string> toDelete;
    for (list<string>::iterator iter = listWords->begin(); iter != listWords->end(); iter++) {
        map<string, int>::iterator iterMap = this->stopWords.find(*iter);
        if (iterMap != endMap) {
            toDelete.push_back(*iter);
        }
    }

    toDelete.sort();
    toDelete.unique();

    for (list<string>::iterator iter = toDelete.begin(); iter != toDelete.end(); iter++) {
        listWords->remove(*iter);
    }
}

Filter::~Filter() {
}

