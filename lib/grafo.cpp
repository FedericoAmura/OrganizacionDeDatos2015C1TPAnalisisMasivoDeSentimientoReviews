/* 
 * File:   grafo.cpp
 * Author: Fernando
 * 
 * Created on 13 de mayo de 2015, 19:29
 */

#include "grafo.h"

grafo::grafo() {
}

grafo::grafo(const grafo& orig) {
}

void grafo::setPath(string& path){
    this->path = path;
}

unordered_map<string, int>* grafo::getVertices(){
    return &this->vertices;
}

int grafo::existo() {
    return 1;
}

void grafo::mapToJsonArray(unordered_map<string,int> mapa, JSON::Array& jsonArray){    
    int contAux = 0;
    for (unordered_map<string, int>::const_iterator iter = mapa.begin(); iter != mapa.end(); iter++){
        
        JSON::Object content;
        JSON::Array info;
        info.push_back(iter->first); // <- Key del mapAristas
        info.push_back(iter->second); // <- Value del mapAristas
        
        content[to_string(contAux)] = info;

        jsonArray.push_back(content);

        contAux++;
    }
}

void grafo::guardarGrafo() {
    JSON::Object grafoFile;
    JSON::Array verticesArray, aristasArray;

    mapToJsonArray(this->vertices, verticesArray);
    mapToJsonArray(this->aristas, aristasArray);

    grafoFile[TAGNODOS] = verticesArray;
    grafoFile[TAGARISTAS] = aristasArray;

    fstream jsonFile;
    jsonFile.open(this->path,fstream::out);
    jsonFile << grafoFile;
}

void grafo::jsonArrayToMap(JSON::Array dataJson, unordered_map<string,int>& mapa){
    int cont = 0;
    for (vector<JSON::Value>::iterator it = dataJson.begin(); it != dataJson.end(); it++) {        
        string jsonKey = to_string(cont);
        string mapKey = it[0][jsonKey][0].as_string();
        int mapValue = it[0][jsonKey][1].as_int();

        mapa[mapKey] = mapValue; 
        
        cont++;
    }        
}

void grafo::cargarGrafo(string& path) {

    JSON::Value jsonWords = parse_file(path.c_str());
    JSON::Array nodosArray = ((jsonWords[TAGNODOS]).operator JSON::Array());
    JSON::Array aristasArray = ((jsonWords[TAGARISTAS]).operator JSON::Array());

    jsonArrayToMap(nodosArray, this->vertices);
    jsonArrayToMap(aristasArray, this->aristas);
}

int grafo::agregarVertice(string clave, int valor) {
    vertices[clave] = valor;
    return 0;
}

int grafo::verticeMasMas(string clave) {
    return (this->agregarVertice(clave, this->valorVertice(clave) + 1));
}

bool grafo::tieneVertice(string clave) {
    return (this->valorVertice(clave) != 0);;
}

int grafo::valorVertice(string clave) {
    return vertices[clave];
}

int grafo::agregarArista(string origen, string destino, int valor) {
    aristas[origen+destino] = valor;
    return 0;
}

int grafo::aristaMasMas(string origen, string destino) {
    return (this->agregarArista (origen, destino, this->valorArista(origen, destino) + 1));
}

bool grafo::tieneArista(string origen, string destino) {
    return (this->valorArista(origen, destino) != 0);
}

int grafo::valorArista(string origen, string destino) {
    return aristas[origen+destino];
}

grafo::~grafo() {
}
