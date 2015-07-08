/* 
 * File:   Review.cpp
 * Author: mauro
 * 
 * Created on 12 de mayo de 2015, 22:42
 */

#include "Review.h"


Review::Review() {
}

Review::Review(string &review) {
    this->review = review;
}

int Review::setID(string id){
    this->id = id;
    return 0;
}

int Review::setSentiment(string sentiment){
    this->sentiment = sentiment;
    return 0;
}

int Review::setReview(string review){    
    this->review = review;
    return 0;
}

string Review::getID(){
    return this->id;
}
string Review::getSentiment(){
    return this->sentiment;
}
string Review::getReview(){
    return this->review;
}

bool Review::isPositive(){
    return (this->sentiment == "1");
}

void Review::aplicarFiltros(Filter* filter) {
    string palabra;           
    stringstream sPalabra(this->review);   
    //se recorre todo el review.
    while (getline(sPalabra, palabra, ' ')) {
        //aplico el filtro a cada palabra.
        filter->signs(palabra);        

        if (!(filter->filterIsStopWords(palabra))){
            this->lReview.push_back(palabra);                        
        }        
    }      
}

void Review::aplicarFiltros(Filter* filter, grafo* grafo) {

    string palabra; // palabra actual 
    string prevPalabra; // palabra anterior - para armar las aristas
    stringstream sPalabra(this->review);   
    
    // Inicializo la palabra anterior, y la seteo en el grafo
    bool primeraPalabraSeteada = false;
    while(!primeraPalabraSeteada){
        
        getline(sPalabra, prevPalabra, ' ');
        filter->signs(prevPalabra);   
        
        if (!(filter->filterIsStopWords(prevPalabra))){
            // solo es una palabra valida si pasa todos los filtros
            //grafo->agregarVertice(prevPalabra, 1);
            grafo->verticeMasMas(prevPalabra);
            this->lReview.push_back(prevPalabra);            
        } else {
            // sino reseteo
            prevPalabra.clear();
        }
        
        if ( prevPalabra.size() == 0){
            primeraPalabraSeteada = false;
        } else {
            primeraPalabraSeteada = true;
        }                
    }
        
    //se recorro el resto del review.
    while (getline(sPalabra, palabra, ' ')) {
        //aplico el filtro a cada palabra.
        filter->signs(palabra);   
        
        if (!(filter->filterIsStopWords(palabra))){
            //no es necesario chequear si estan porque si no se inicializan en 0 automaticamente
            grafo->verticeMasMas(palabra);
            grafo->aristaMasMas(prevPalabra, palabra);
            prevPalabra = palabra;
            this->lReview.push_back(palabra);            
            
        }        
    }      
}

list<string>* Review::getPalabras() {
    return &this->lReview;
}

Review::~Review() {
//    cout << "Destructor Review" << endl;
    
}

