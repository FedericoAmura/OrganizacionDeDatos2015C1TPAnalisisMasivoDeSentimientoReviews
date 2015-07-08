/* 
 * File:   filters.h
 * Author: pipulina
 *
 * Created on May 8, 2015, 4:41 PM
 */

#ifndef FILTERS_H
#define	FILTERS_H

#include <string>
#include <string.h>
#include <JSON/json.hh>
#include <fstream>
#include "../include/DefaultValues.h"

using namespace std;

void levantargrafo(string& path);
void persistGrafo();
void jsonArrayToMap(JSON::Array dataJson, map<string,int>& map);
void mapToJsonArray(map<string,int> map, JSON::Array& jsonArray);

#endif	/* FILTERS_H */

