/* 
 * File:   Review.h
 * Author: mauro
 *
 * Created on 12 de mayo de 2015, 22:42
 */

#ifndef REVIEW_H
#define	REVIEW_H

#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Filter.h"
#include "grafo.h"

using namespace std;

class Review {
    
private:
    list<string> lReview;
    string id;
    string review;
    string sentiment;

public:
    Review();
    Review(string &review);
    //setea un id al review
    int setID(string id);
    //setea un sentimiento al review, lo hace en string por simplicidad
    int setSentiment(string sentiment);
    //setea un string al review.
    int setReview(string review);
    //metodos de prueba para ver los datos cargados
    //TODO sacarlos
    string getID();
    string getSentiment();
    string getReview();
    void aplicarFiltros(Filter* filter);
    void aplicarFiltros(Filter* filter, grafo* grafo);
    
    bool isPositive();
    
    list<string>* getPalabras();
    
    virtual ~Review();
};

#endif	/* REVIEW_H */

