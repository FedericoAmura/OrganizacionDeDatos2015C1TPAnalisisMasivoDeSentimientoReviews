/*
 * File:   main.cpp
 * Author: popCorn
 *
 * Created on April 21, 2015, 10:01 AM
 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include "lib/Filter.h"
#include "lib/Review.h"
#include "lib/grafo.h"
#include <unordered_map>


using namespace std;

struct Detalles {
    string idReview;
    float sentiment;
};

//Funcion de puntaje para el review segun los grafos.
//Cuenta la cantidad de veces que gana cada grafo sin importar los valores
//Devuelve Prob(positivo) = victoriasPositivas/evaluaciones
float puntajeDiferencial(grafo* positivo, grafo* negativo, Review review) {
    //variables para el calculo de puntaje diferencial
    int puntosPositivos = 0;
    int puntosNegativos = 0;
    int contadorAristasPositivo = 0;
    int contadorAristasNegativo = 0;
    int mejorPositivo = 0;
    int mejorNegativo = 0;
    bool contadorEnPositivo = true;
    
    int contador = 0;
    //lista de palabras de la review a analisar
    list<string>* listaDePalabras = review.getPalabras();

    //veo la primer key para setear todo en un ciclo
    string prevKey = listaDePalabras->front();
    //consigo la diferencia, si es positiva, la sumo a positivo y al reves
    if (positivo->valorVertice(prevKey) > negativo->valorVertice(prevKey)) {
        puntosPositivos++;
    } else {
        puntosNegativos++;
    }
    //avanzo primer step
    listaDePalabras->pop_front();
    
    //ciclo evaluador de lista
    for (list<string>::iterator it = listaDePalabras->begin(); it != listaDePalabras->end(); it++) {
        string key = *it;
        //agrego valor de la arista
        if (positivo->valorArista(prevKey, key) > negativo->valorArista(prevKey, key)) {
            if (!contadorEnPositivo){
                if (mejorNegativo < contadorAristasNegativo){
                    mejorNegativo = contadorAristasNegativo;
                }
                contadorAristasNegativo = 0;
                contadorEnPositivo = true;                
            }
            
            puntosPositivos++;
            contadorAristasPositivo++;
        }
        
        if (positivo->valorArista(prevKey, key) < negativo->valorArista(prevKey, key)) {
            if (contadorEnPositivo){
                if (mejorPositivo < contadorAristasPositivo){
                    mejorPositivo = contadorAristasPositivo;
                }
                contadorAristasPositivo = 0;
                contadorEnPositivo = false;
            }

            puntosNegativos++;
            contadorAristasNegativo++;
        }
        //agrego valor del sig vertice
        if (positivo->valorVertice(prevKey) > negativo->valorVertice(prevKey)) {
            puntosPositivos++;
        } else {
            puntosNegativos++;
        }
        
        prevKey = key;
        contador++;
    }

    puntosPositivos += mejorPositivo;
    puntosNegativos += mejorNegativo;
    
    //devuelvo la relacion de puntos positivos frente al total de puntos
    return (puntosPositivos / float(puntosPositivos + puntosNegativos));
}

//Funcion que levanta todos los archivos del disco y evalua reviews
void leerTestData() {
    
    //declaro valores para evaluar reviews
    int valorPos, valorNeg;
    float valorFinal;
    
    cout << "Cargamos grafos..." << endl;
    //declara y carga el grafo positivo
    grafo* grafoPositivo = new grafo;
    string pathPos = PERSISTENCEFILE_POS;
    grafoPositivo->setPath(pathPos);
    grafoPositivo->cargarGrafo(pathPos);

    //declara y carga el grafo negativo
    grafo* grafoNegativo = new grafo;
    string pathNeg = PERSISTENCEFILE_NEG;
    grafoNegativo->setPath(pathNeg);
    grafoNegativo->cargarGrafo(pathNeg);

    //declaro lista de resultados y un elemento en particular
    list<Detalles> listaDeResultados;
    Detalles unDetalle;

    //aca creo el archivo de salida con los registros de la lista
    ofstream output;
    output.open("output.csv");

    //declaro y leo archivo de reviews a evaluar
    cout << "Cargamos archivo de reviews y evaluamos" << endl;
    fstream file;
    file.open(TESTDATA, fstream::in);
    if (!file.is_open()) {
        cout << "Error al arbir el archivo a evaluar." << endl; //algo paso, encontro file?
    } else {

        //string que corresponde a una entrada del file (id sentiment review)
        string registro;
        //flag para conteo de lecturas
        int flag = 1;
        //saco el header del archivo
        getline(file, registro);
        Filter filter;
        //leo una entrada del file y la guardo en registro (y chequeo el flag)
        while (getline(file, registro) ){
            Review unReview;

            //declaro estructura para agregar a la lista
            Detalles elementoLista;
            //saco de la entrada leida los campos
            //string que corresponde a un campo de las entradas
            string campo;
            stringstream sRegistro(registro);

            //guardo el registro que estoy leyendo
            getline(sRegistro, campo, '\t'); // campo <- id
            unReview.setID(campo);
            getline(sRegistro, campo, '\t'); // campo <- review  
            unReview.setReview(campo);
            unReview.aplicarFiltros(&filter);
            elementoLista.idReview = unReview.getID();
            elementoLista.sentiment = puntajeDiferencial(grafoPositivo, grafoNegativo, unReview);
            listaDeResultados.push_back(elementoLista);
            if (flag % 1000 == 0){
                cout << "Vamos " << flag << " registros. Este saco: " << elementoLista.sentiment << endl;
            }
            flag++;

        }
        cout << "Ya terminamos de evaluar" << endl;
    }
    
    if (output.is_open()) {
        //evaluo los reviews y predigo su positividad
        unDetalle = listaDeResultados.front();
        cout << "Guardamos todas las predicciones en un file..." << endl;
        output << "id,sentiment\n";
        for (list<Detalles>::iterator it = listaDeResultados.begin(); it != listaDeResultados.end(); it++) {
            unDetalle = *it;
            output << unDetalle.idReview << "," << unDetalle.sentiment << endl;
        }
        output.close();
    } else {
        cout << "Error al generar archivo de salida" << endl;
    }

    delete grafoPositivo;
    delete grafoNegativo;
}

//Funcion que genera grafos y los entrena
void entrenarGrafos(bool nuevosGrafos) {

    /**************************************************************************
     * INICIO LECTURA DEL ARCHIVO Y PARSER.                                   *   
     **************************************************************************/
    //declara file de reviews
    fstream file;

    //declara objeto review
    Filter filter;

    //declara y precarga el grafo positivo
    grafo* grafoPositivo = new grafo;
    string pathPos = PERSISTENCEFILE_POS;
    grafoPositivo->setPath(pathPos);

    //declara y precarga el grafo negativo
    grafo* grafoNegativo = new grafo;
    string pathNeg = PERSISTENCEFILE_NEG;
    grafoNegativo->setPath(pathNeg);

    if (!nuevosGrafos){
        grafoPositivo->cargarGrafo(pathPos);
        grafoNegativo->cargarGrafo(pathNeg);
    }
    //abre el file de reviews
    file.open(LEBELEDTRAINDATA, fstream::in);
    if (!file.is_open()) {
        cout << "Error al arbir el archivo. Verifiquelo y regrese." << endl; //algo paso, encontro file?
    } else {
        //string que corresponde a una entrada del file (id sentiment review)
        string registro;
        //flag para leer solo algunas de review o llevar un conteo
        int flag = 1;
        //saco el header del archivo
        getline(file, registro);

        //leo una entrada del file y la guardo en registro (y chequeo el flag)
        while (getline(file, registro) && flag < 25000) {

            Review unReview;
            //chequeo iteraciones
            if (flag % 1000 == 0){
                cout << "Ya procese " << flag << " registros." << endl;
            }
            //saco de la entrada leida los campos
            //string que corresponde a un campo de las entradas
            string campo;
            stringstream sRegistro(registro);
            //leo los campos de las reviews del set de entrenamiento
            getline(sRegistro, campo, '\t'); // campo <- id
            unReview.setID(campo);
            getline(sRegistro, campo, '\t'); // campo <- sentiment
            unReview.setSentiment(campo);
            getline(sRegistro, campo, '\t'); // campo <- review  
            unReview.setReview(campo);
            //Procesamos el review y lo agrgamos a los grafos
            if (unReview.isPositive()) {
                unReview.aplicarFiltros(&filter, grafoPositivo);
            } else {
                unReview.aplicarFiltros(&filter, grafoNegativo);
            }
            flag++;
        }
        cout << "Terminado el archivo de entrenamiento. Finalizamos..." << endl;
    }

    //guardo el grafo para no tener que volver a entrenar el programa   
    grafoPositivo->guardarGrafo();
    grafoNegativo->guardarGrafo();

    //libero la memoria de todo y cierro el file
    delete grafoPositivo;
    delete grafoNegativo;
    if (file.is_open()) file.close();
}

void promediar(){

    //archivo de salida promediado
    ofstream output;
    output.open("outputPromedio.csv");

    //archivos a promediar
    fstream file1, file2;
    file1.open("outputTrigrama.csv", fstream::in);
    file2.open("outputPerceptron.csv", fstream::in);
    
    //mapper de archivos
    map<string, double> file1Map, file2Map;

    if ((!file1.is_open()) || (!file2.is_open())) {
        cout << "Error al arbir archivos a promediar." << endl;
    } else {
        string registro;

        // paso a mapper los archivos
        while (getline(file1, registro) ){
            string id, puntaje;
            stringstream sRegistro(registro);

            getline(sRegistro, id, ','); //  id
            getline(sRegistro, puntaje, ','); // puntaje  
            
            file1Map[id] = atof(puntaje.c_str());
        }        

        while (getline(file2, registro) ){
            string id, puntaje;
            stringstream sRegistro(registro);

            getline(sRegistro, id, ','); //  id
            getline(sRegistro, puntaje, ','); // puntaje  
            
            file2Map[id] = atof(puntaje.c_str());
        }        
        
        int cont = 0;        
        // calculo promedio y agrego al archivo salida        
        output << "id,sentiment\n"; // inicializo el archivo
        for(map<string,double>::iterator it = file2Map.begin(); it != file2Map.end(); it++){
            
            double valueFile1 = file1Map[it->first];            
            double sum = it->second + valueFile1;
            double value = sum / 2;
            
            output << it->first << "," << value << endl;
        }        
        output.close();
    }    
}



int main(int argc, char** argv) {    
    
    if (argc > 1) {
        cout << "Entendido." << endl;
        if (strcmp(argv[1], "ENTRENAR") == 0) {
            cout << "Generando nuevos grafos y entrenandolos, por favor espere..." << endl;
            entrenarGrafos(true);
            cout << "Fin del entrenamiento, ya puede evaluar reviews" << endl;
            return 0;
        }
        if (strcmp(argv[1], "REFORZAR") == 0) {
            cout << "Cargando los grafos y reforzandolos, por favor espere..." << endl;
            entrenarGrafos(false);
            cout << "Fin del entrenamiento, ya puede evaluar reviews" << endl;
            return 0;
        }
        if (strcmp(argv[1], "RUN") == 0) {
            cout << "Analisando nuevos reviews, por favor espere..." << endl;
            leerTestData();
            cout << "Listo, file de predicciones creado" << endl;
            return 0;
        }
        if (strcmp(argv[1], "PROMEDIAR") == 0) {
            cout << "Promediando los archivos.." << endl;
            promediar();
            cout << "FIN DE PROMEDIO,gracias vuelva prontosss" << endl;
            return 0;
        }
    } else {
        cout << "No se agrego ningun parametro por consola, las opciones son: " << endl;
        cout << " ENTRENAR  " << endl;
        cout << " REFORZAR " << endl;
        cout << " PROMEDIAR" << endl;
        cout << " RUN " << endl;
        return 0;
    }
}
