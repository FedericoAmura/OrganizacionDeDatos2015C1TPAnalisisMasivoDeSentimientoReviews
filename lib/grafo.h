/* 
 * File:   grafo.h
 * Author: Fernando
 *
 * Created on 13 de mayo de 2015, 19:29
 */

#ifndef GRAFO_H
#define	GRAFO_H
#include <string>
#include <string.h>
#include <unordered_map>
#include <JSON/json.hh>
#include <fstream>
#include "../include/DefaultValues.h"

using namespace std;

class grafo {
public:
    grafo();
    grafo(const grafo& orig);
    int existo();
    
    void setPath(string& path);
    unordered_map<string, int>* getVertices();
    
    //Funciones de vertices y aristas
    int agregarVertice(string clave, int valor);
    int verticeMasMas(string clave);
    bool tieneVertice(string clave);
    int valorVertice(string clave);
    
    int agregarArista(string origen, string destino, int valor);
    int aristaMasMas(string origen, string destino);
    bool tieneArista(string origen, string destino);
    int valorArista(string origen, string destino);
    
    //Funciones de persistencia
    void guardarGrafo();
    void cargarGrafo(string& path);

    virtual ~grafo();    
private:

    unordered_map<string, int> aristas;
    unordered_map<string, int> vertices;
    string path;    // enlace donde se guarda el grafo
    
    void jsonArrayToMap(JSON::Array dataJson, unordered_map<string,int>& map);
    void mapToJsonArray(unordered_map<string,int> map, JSON::Array& jsonArray);
    

};

#endif	/* GRAFO_H */

