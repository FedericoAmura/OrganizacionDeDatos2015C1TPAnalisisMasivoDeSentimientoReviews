/* 
 * File:   Filter.h
 * Author: mcortes
 *
 * Created on May 6, 2015, 4:10 PM
 */

#ifndef FILTER_H
#define	FILTER_H

#include <iostream>
#include <string>
#include <sstream>
#include <JSON/json.hh>
#include <list>
#include "../include/DefaultValues.h"

using namespace std;

class Filter {
private:
    map<string, int> stopWords;

    void getMapStopWords();
    
public:
    Filter();
    //filtra todos los signos de un texto.
    void signs(string &txt);

    /* Recibe una lista de palabras a evaluar si son StopWords o no.
    ** Las StopWords son borradas de la lista */
    void filterStopWords(list<string>* listWords);
    
    /* TRUE si la palabra ES stopWord
     * FALSE si la palabra NO ES stopWord */
    bool filterIsStopWords(string word);
    
    virtual ~Filter();
};

#endif	/* FILTER_H */
