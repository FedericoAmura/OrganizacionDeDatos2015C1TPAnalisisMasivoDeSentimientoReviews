
#include "filters.h"

void jsonArrayToMap(JSON::Array dataJson, map<string,int>& map){

    int cont = 0;
    for (vector<JSON::Value>::iterator it = dataJson.begin(); it != dataJson.end(); it++) {        
        string jsonKey = to_string(cont);
        string mapKey = it[0][jsonKey][0].as_string();
        int mapValue = it[0][jsonKey][1].as_int();

        map[mapKey] = mapValue; 
        
        cont++;
    }        
}

void levantargrafo(string& path){
    
    JSON::Value jsonWords = parse_file(path.c_str());
    JSON::Array nodos = ((jsonWords["nodos"]).operator JSON::Array());
    JSON::Array aristas = ((jsonWords["aristas"]).operator JSON::Array());

    map<string,int> mapNodos;
    map<string,int> mapAristas;
    
    jsonArrayToMap(nodos, mapNodos);
    jsonArrayToMap(aristas, mapAristas);
    
    cout<< "MAP NODOS " <<endl;
    for (map<string, int>::const_iterator iter = mapNodos.begin(); iter != mapNodos.end(); iter++){
        cout << "Key: " << iter->first << " Value:" << iter->second << endl;
    }    

    cout<< "MAP ARISTAS " <<endl;
    for (map<string, int>::const_iterator iter = mapAristas.begin(); iter != mapAristas.end(); iter++){
        cout << "Key: " << iter->first << " Value:" << iter->second << endl;
    }    

}

void mapToJsonArray(map<string,int> mapa, JSON::Array& jsonArray){
    
    int contAux = 0;
    for (map<string, int>::const_iterator iter = mapa.begin(); iter != mapa.end(); iter++){
        cout << "Key: " << iter->first << " Value:" << iter->second << endl;
        
        JSON::Object content;
        JSON::Array info;
        info.push_back(iter->first); // <- Key del mapAristas
        info.push_back(iter->second); // <- Value del mapAristas
        
        content[to_string(contAux)] = info;

        jsonArray.push_back(content);

        contAux++;
    }
}

void persistGrafo(){
    
    // voy a tener acceso a los maps que ya van a tener valores
    // esto es para generarme 2 maps para probar la funcion
//    JSON::Value jsonWords = parse_file("data/json/examplePersist.json");
//    JSON::Array nodosArray = ((jsonWords[TAGNODOS]).operator JSON::Array());
//    JSON::Array aristasArray = ((jsonWords[TAGARISTAS]).operator JSON::Array());
//
    map<string,int> mapNodos, mapAristas;
//    
//    jsonArrayToMap(nodosArray, mapNodos);
//    jsonArrayToMap(aristasArray, mapAristas);

    ////////////////////////////////////

    JSON::Object grafoFile;
    JSON::Array nodos, aristas;

    mapToJsonArray(mapNodos, nodos);
    mapToJsonArray(mapAristas, aristas);

    grafoFile[TAGNODOS] = nodos;
    grafoFile[TAGARISTAS] = aristas;

    fstream jsonFile;
    jsonFile.open(PERSISTENCEFILE_POS,fstream::out);
    jsonFile << grafoFile;
}