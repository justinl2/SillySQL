// PROJECT IDENTIFIER: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include <unordered_map>
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include <getopt.h>
#include <map>
#include "TableEntry.h"

using namespace std;

bool quiet = false;
unordered_map<TableEntry, vector<size_t> > hashIndex;
map<TableEntry, vector<size_t> > BSTindex;
EntryType hashType = EntryType::Bool;
EntryType bstType = EntryType::Bool;

struct Table{
    size_t numRows = 0;
    vector<string> colnameVec;
    vector<EntryType> coltypeVec;
    vector <vector<TableEntry> > data;

};

unordered_map<string, Table> metaData;

unordered_map<TableEntry, vector<size_t> > generateHash(size_t index, string tablename) {
    unordered_map<TableEntry, vector<size_t> > hash_map;
    for (size_t i = 0; i < metaData[tablename].numRows; i++) {
        if (hash_map.find(metaData[tablename].data[i][index]) == hash_map.end()) {
            hash_map[metaData[tablename].data[i][index]] = {};
        }
        hash_map[metaData[tablename].data[i][index]].push_back(i);
    }
    return hash_map;
}

map<TableEntry, vector<size_t> > generateBST(size_t index, string tablename) {
    map<TableEntry, vector<size_t> > bst_map;
    for (size_t i = 0; i < metaData[tablename].numRows; i++) {
        if (bst_map.find(metaData[tablename].data[i][index]) == bst_map.end()) {
            bst_map[metaData[tablename].data[i][index]] = {};
        }
        bst_map[metaData[tablename].data[i][index]].push_back(i);
    }
    return bst_map;
}

void notQuiet(size_t rowNum, vector<size_t> indices, string tablename) {
    for (size_t j = 0; j < indices.size(); j++) {
        if (metaData[tablename].coltypeVec[indices[j]] == EntryType::Bool) {
            if (metaData[tablename].data[rowNum][indices[j]] == true) {
                cout << "true ";
            }
            else {
                cout << "false ";
            }
        } else {
            cout << metaData[tablename].data[rowNum][indices[j]] << " ";
        }
    }
}

void createFunc(){
    Table ht;
    string tablename;
    size_t size;
    
    cin >> tablename;
    cin >> size;
    
    cout << "New table " << tablename << " with column(s) ";

    for(size_t i = 0; i < size; i++){
        string coltype;
        cin >> coltype;

        if(coltype == "int"){
             ht.coltypeVec.push_back(EntryType::Int);
        } 
        else if(coltype == "double"){
             ht.coltypeVec.push_back(EntryType::Double);
        }
        else if(coltype == "string"){
             ht.coltypeVec.push_back(EntryType::String);
        } else {
             ht.coltypeVec.push_back(EntryType::Bool);
        }
    }

    for(size_t i = 0; i < size; i++){
        string colname;
        cin >> colname;
        ht.colnameVec.push_back(colname);
        
        cout << colname << " ";

    }

    cout << "created\n";
    metaData.insert({tablename, ht});

}

void removeFunc(){
    string beGone;
    cin >> beGone;
    if(metaData.find(beGone) == metaData.end()){
        cout << "Error during REMOVE: " << beGone << " does not name a table in the database\n";
        return;
    }

    metaData.erase(beGone);

    cout << "Table " << beGone << " deleted\n";
}

void insertFunc(){
    string tablename;
    size_t size;
    string junk;

    int intVar;
    double doubleVar;
    string stringVar;
    string boolVar;

    cin >> junk; //holds into
    cin >> tablename;
    cin >> size;
    
    if(metaData.find(tablename) == metaData.end()){
        cout << "Error during INSERT: " << tablename << " does not name a table in database\n";
        return;
    }
      
    cin >> junk; //hold rows
    
    vector <TableEntry> temp;
    
    for(size_t i = 0; i < size; i++){     
        for(size_t j = 0; j < metaData[tablename].coltypeVec.size(); j++){
            
            if(metaData[tablename].coltypeVec[j] == EntryType::Int){
                cin >> intVar;
                temp.push_back(TableEntry(intVar));
            }
            else if(metaData[tablename].coltypeVec[j] == EntryType::Double){
                cin >> doubleVar;
                temp.push_back(TableEntry(doubleVar));
            }
            else if(metaData[tablename].coltypeVec[j] == EntryType::String){
                cin >> stringVar;
                temp.push_back(TableEntry(stringVar));
            } else {
                cin >> boolVar;
                if(boolVar == "false"){
                    temp.push_back(TableEntry(false));
                } else {
                    temp.push_back(TableEntry(true));
                }
                
            }
            
        }
        metaData[tablename].data.push_back(temp);
        temp.clear();
    }

    cout << "Added " << size << " rows to " << tablename << " from position " << metaData[tablename].numRows << " to ";
    cout << metaData[tablename].numRows + size - 1 << "\n";
    
    metaData[tablename].numRows = metaData[tablename].numRows + size;
    
}

void printFunc(){
    string tablename;
    string junk;
    string variable;
    size_t size;
    size_t rowsPrinted = 0;
    
    vector <string> temp;
    vector <size_t> indices;
    
    cin >> junk; //holds from
    cin >> tablename; //holds tablename
    
    if(metaData.find(tablename) == metaData.end()){
        cout << "Error during PRINT: " << tablename << " does not name a table in the database\n";
        return;
    }

    cin >> size;

    for (size_t i = 0; i < size; i++){
        
        cin >> variable;

        for(size_t j = 0; j < metaData[tablename].colnameVec.size(); j++){
            if(variable == metaData[tablename].colnameVec[j]){
                temp.push_back(variable); 
            }
        }
        if(temp.size() != i + 1){
            cout << "Error during PRINT: " << variable << " does not name a column in " << tablename << "\n";
            getline(cin, variable);
            return;
        }
    }

    for(size_t i = 0; i < size; i++){
        for(size_t j = 0; j < metaData[tablename].colnameVec.size(); j++){
            if(temp[i] == metaData[tablename].colnameVec[j]){
                indices.push_back(j);
            }
        }
        
    }

    cin >> variable;
    

    if(variable == "ALL"){

        for(size_t i = 0; i < temp.size(); i++){
            cout << temp[i] << " ";
        }
        cout << "\n";

        for(size_t i = 0; i < metaData[tablename].numRows; i++){
            for(size_t j = 0; j < indices.size(); j++){
                cout << metaData[tablename].data[i][indices[j]] << " ";  
            }
            rowsPrinted++;
            cout << "\n";
        }
        
    } else {
        string colname;
        char symbol;
        size_t windex = 666;
        EntryType type;

        cin >> colname;
        
        for(size_t i = 0; i < metaData[tablename].colnameVec.size(); i++){
            if(colname == metaData[tablename].colnameVec[i]){
                windex = i;
            }
        }

        if(windex == 666){
            cout << "Error during PRINT: " << colname << " does not name a column in " << tablename << "\n";
            getline(cin, colname);
            return;
        }
        
        for(size_t i = 0; i < temp.size(); i++){
            cout << temp[i] << " ";
        }
        
        cout << "\n";
        

        type = metaData[tablename].coltypeVec[windex];

        cin >> symbol;

        if(type == EntryType::Int){

            int intVar;
            cin >> intVar;
            if(symbol == '<'){
                for(size_t i = 0; i < metaData[tablename].numRows; i++){
                    if(metaData[tablename].data[i][windex] < intVar){
                        for(size_t j = 0; j < indices.size(); j++){
                        cout << metaData[tablename].data[i][indices[j]] << " ";
                        }
                        rowsPrinted++;
                        cout << "\n";
                    }
                }
            }
            else if(symbol == '>'){
                for(size_t i = 0; i < metaData[tablename].numRows; i++){
                    if(metaData[tablename].data[i][windex] > intVar){
                        for(size_t j = 0; j < indices.size(); j++){
                        cout << metaData[tablename].data[i][indices[j]] << " ";
                        }
                        rowsPrinted++;
                        cout << "\n";
                    }
                }
            }
            else{
                for(size_t i = 0; i < metaData[tablename].numRows; i++){
                    if(metaData[tablename].data[i][windex] == intVar){
                        for(size_t j = 0; j < indices.size(); j++){
                        cout << metaData[tablename].data[i][indices[j]] << " ";
                        }
                        rowsPrinted++;
                        cout << "\n";
                    }
                }
            }
        }
        else if(type == EntryType::Double){

            double doubleVar;
            cin >> doubleVar;
            if(symbol == '<'){
                for(size_t i = 0; i < metaData[tablename].numRows; i++){
                    if(metaData[tablename].data[i][windex] < doubleVar){
                        for(size_t j = 0; j < indices.size(); j++){
                        cout << metaData[tablename].data[i][indices[j]] << " ";
                        }
                        rowsPrinted++;
                        cout << "\n";
                    }
                }
            }
            else if(symbol == '>'){
                for(size_t i = 0; i < metaData[tablename].numRows; i++){
                    if(metaData[tablename].data[i][windex] > doubleVar){
                        for(size_t j = 0; j < indices.size(); j++){
                        cout << metaData[tablename].data[i][indices[j]] << " ";
                        }
                        rowsPrinted++;
                        cout << "\n";
                    }
                }
            }
            else{
                for(size_t i = 0; i < metaData[tablename].numRows; i++){
                    if(metaData[tablename].data[i][windex] == doubleVar){
                        for(size_t j = 0; j < indices.size(); j++){
                        cout << metaData[tablename].data[i][indices[j]] << " ";
                        }
                        rowsPrinted++;
                        cout << "\n";
                    }
                }
            }
        }
        else if(type == EntryType::String){

            string stringVar;
            cin >> stringVar;
            if(symbol == '<'){
                for(size_t i = 0; i < metaData[tablename].numRows; i++){
                    if(metaData[tablename].data[i][windex] < stringVar){
                        for(size_t j = 0; j < indices.size(); j++){
                        cout << metaData[tablename].data[i][indices[j]] << " ";
                        }
                        rowsPrinted++;
                        cout << "\n";
                    }
                }
            }
            else if(symbol == '>'){
                for(size_t i = 0; i < metaData[tablename].numRows; i++){
                    if(metaData[tablename].data[i][windex] > stringVar){
                        for(size_t j = 0; j < indices.size(); j++){
                        cout << metaData[tablename].data[i][indices[j]] << " ";
                        }
                        rowsPrinted++;
                        cout << "\n";
                    }
                }
            } else {
                for(size_t i = 0; i < metaData[tablename].numRows; i++){
                    if(metaData[tablename].data[i][windex] == stringVar){
                        for(size_t j = 0; j < indices.size(); j++){
                        cout << metaData[tablename].data[i][indices[j]] << " ";
                        }
                        rowsPrinted++;
                        cout << "\n";
                    }
                }
            }

        } else {

            string boolVar;
            bool boolVar2;
            cin >> boolVar;
            if(boolVar == "true"){
                boolVar2 = true;
            } else {
                boolVar2 = false;
            }

            if(symbol == '<'){
                for(size_t i = 0; i < metaData[tablename].numRows; i++){
                    if(metaData[tablename].data[i][windex] < boolVar2){
                        for(size_t j = 0; j < indices.size(); j++){
                        cout << metaData[tablename].data[i][indices[j]] << " ";
                        }
                        rowsPrinted++;
                        cout << "\n";
                    }
                }
            }
            else if(symbol == '>'){

                for(size_t i = 0; i < metaData[tablename].numRows; i++){
                    if(metaData[tablename].data[i][windex] > boolVar2){
                        for(size_t j = 0; j < indices.size(); j++){
                        cout << metaData[tablename].data[i][indices[j]] << " ";
                        }
                        rowsPrinted++;
                        cout << "\n";
                    }
                }
            } else {

                for(size_t i = 0; i < metaData[tablename].numRows; i++){
                    
                    if(metaData[tablename].data[i][windex] ==  boolVar2){
                        
                        for(size_t j = 0; j < indices.size(); j++){
                        cout << metaData[tablename].data[i][indices[j]] << " ";
                        }
                        rowsPrinted++;
                        cout << "\n";
                    }
                }
            }
        }
    }

    cout << "Printed " << rowsPrinted  << " matching rows from " << tablename << "\n";

}

void deleteFunc(){
    string junk;
    string tablename;
    size_t rowsDeleted = 0;
    string colname;
    char symbol;
    size_t windex = 666;
    EntryType type;

    cin >> junk; //holds from
    cin >> tablename;

    if(metaData.find(tablename) == metaData.end()){
        cout << "Error during DELETE: " << tablename << " does not name a table in database\n";
        return;
    }
    
    cin >> junk; //holds where

    cin >> colname;


    for(size_t i = 0; i < metaData[tablename].colnameVec.size(); i++){
        if(colname == metaData[tablename].colnameVec[i]){
            windex = i;
        }
    }

    if(windex == 666){
        cout << "Error during DELETE: " << colname << " does not name a column in " << tablename << "\n";
    }

    type = metaData[tablename].coltypeVec[windex];
    cin >> symbol;
    
    
    if(type == EntryType::Int){
        
        int intVar;
        cin >> intVar;
        if(symbol == '<'){
            
            for(int i = 0; unsigned(i) < metaData[tablename].numRows; i++){
                if(metaData[tablename].data[unsigned(i)][windex] < intVar){
                    rowsDeleted++;
                    metaData[tablename].data.erase(metaData[tablename].data.begin() + i);
                    metaData[tablename].numRows--;
                    i--;
                }
            }

        }
        else if(symbol == '>'){
            
            for(int i = 0; unsigned(i) < metaData[tablename].numRows; i++){
                if(metaData[tablename].data[unsigned(i)][windex] > intVar){
                    rowsDeleted++;
                    metaData[tablename].data.erase(metaData[tablename].data.begin() + i);
                    metaData[tablename].numRows--;
                    i--;
                }
            }

        }
        else{
            
            for(int i = 0; unsigned(i) < metaData[tablename].numRows; i++){
                if(metaData[tablename].data[unsigned(i)][windex] == intVar){
                    rowsDeleted++;
                    metaData[tablename].data.erase(metaData[tablename].data.begin() + i);
                    metaData[tablename].numRows--;
                    i--;
                }
            }

        }
    }
    else if(type == EntryType::Double){
        
        double doubleVar;
        cin >> doubleVar;
        if(symbol == '<'){
            
            for(int i = 0; unsigned(i) < metaData[tablename].numRows; i++){
                if(metaData[tablename].data[unsigned(i)][windex] < doubleVar){
                    rowsDeleted++;
                    metaData[tablename].data.erase(metaData[tablename].data.begin() + i);
                    metaData[tablename].numRows--;
                    i--;
                }
            }

        }
        else if(symbol == '>'){
            
            for(int i = 0; unsigned(i) < metaData[tablename].numRows; i++){
                if(metaData[tablename].data[unsigned(i)][windex] > doubleVar){
                    rowsDeleted++;
                    metaData[tablename].data.erase(metaData[tablename].data.begin() + i);
                    metaData[tablename].numRows--;
                    i--;
                }
            }

        }
        else{
            
            for(int i = 0; unsigned(i) < metaData[tablename].numRows; i++){
                if(metaData[tablename].data[unsigned(i)][windex] == doubleVar){
                    rowsDeleted++;
                    metaData[tablename].data.erase(metaData[tablename].data.begin() + i);
                    metaData[tablename].numRows--;
                    i--;
                }
            }
        }
    }
    else if(type == EntryType::String){
        
        string stringVar;
        cin >> stringVar;
        if(symbol == '<'){
            
            for(int i = 0; unsigned(i) < metaData[tablename].numRows; i++){
                if(metaData[tablename].data[unsigned(i)][windex] < stringVar){
                    rowsDeleted++;
                    metaData[tablename].data.erase(metaData[tablename].data.begin() + i);
                    metaData[tablename].numRows--;
                    i--;
                }
            }

        } 
        else if(symbol == '>'){
            
            for(int i = 0; unsigned(i) < metaData[tablename].numRows; i++){
                if(metaData[tablename].data[unsigned(i)][windex] > stringVar){
                    rowsDeleted++;
                    metaData[tablename].data.erase(metaData[tablename].data.begin() + i);
                    metaData[tablename].numRows--;
                    i--;
                }
            }

        
        } else {
            for(int i = 0; unsigned(i) < metaData[tablename].numRows; i++){
                if(metaData[tablename].data[unsigned(i)][windex] == stringVar){
                    rowsDeleted++;
                    metaData[tablename].data.erase(metaData[tablename].data.begin() + i);
                    metaData[tablename].numRows--;
                    i--;
                }
            }
            
        }

    } else {
        
        string boolVar;
        cin >> boolVar;
        bool boolVar2;
        if(boolVar == "true"){
            boolVar2 = true;
        } else {
            boolVar2 = false;
        }
        if(symbol == '<'){
            vector<int> indices;
            for(int i = 0; unsigned(i) < metaData[tablename].numRows; i++){
                if(metaData[tablename].data[unsigned(i)][windex] < boolVar2){
                    rowsDeleted++;
                    metaData[tablename].data.erase(metaData[tablename].data.begin() + i);
                    metaData[tablename].numRows--;
                    i--;
                }
            }

        }
        else if(symbol == '>'){
            vector<int> indices;
            for(int i = 0; unsigned(i) < metaData[tablename].numRows; i++){
                if(metaData[tablename].data[unsigned(i)][windex] > boolVar2){
                    rowsDeleted++;
                    metaData[tablename].data.erase(metaData[tablename].data.begin() + i);
                    metaData[tablename].numRows--;
                    i--;
                }
            }

        } else {
            
            vector<int> indices;

            for(int i = 0; unsigned(i) < metaData[tablename].numRows; i++){
                if(metaData[tablename].data[unsigned(i)][windex] == boolVar2){
                    rowsDeleted++;
                    metaData[tablename].data.erase(metaData[tablename].data.begin() + i);
                    metaData[tablename].numRows--;
                    i--;
                }
            }
        
        }

    }
cout << "Deleted " << rowsDeleted << " rows from " << tablename << "\n";
}

void joinFunc(){
    string tablename1;
    string tablename2;
    string junk;
    string colname1;
    string colname2;
    size_t printNum;
    size_t index = 666;
    size_t index2 = 666;
    int oneOrtwo;
    int rowCount = 0;
    vector<pair<size_t, int> > indices;

    unordered_map <TableEntry, vector <vector<TableEntry> > > hashBrowns;

    cin >> tablename1;
    cin >> junk; //holds and
    cin >> tablename2;
    cin >> junk; //holds where
    cin >> colname1;
    cin >> junk; //holds =
    cin >> colname2;
    cin >> junk; //holds and
    cin >> junk; //holds print
    cin >> printNum;

    if(metaData.find(tablename1) == metaData.end()){
        cout << "Error during JOIN: " << tablename1 << " does not name a table in database\n";
        return;
    }
    if(metaData.find(tablename2) == metaData.end()){
        cout << "Error during JOIN: " << tablename2 << " does not name a table in database\n";
        return;
    }

    for(size_t i = 0; i < metaData[tablename1].colnameVec.size(); i++){
        if(colname1 == metaData[tablename1].colnameVec[i]){
            index = i;
        }   
    }

    for(size_t i = 0; i < metaData[tablename2].colnameVec.size(); i++){
        if(colname2 == metaData[tablename2].colnameVec[i]){
            index2 = i;
        }  
    }

    
    if(index == 666){
        cout << "Error during JOIN: " << colname1 << " does not name a column in " << tablename1 << "\n";
        return;
    }

    if(index2 == 666){
        cout << "Error during JOIN: " << colname2 << " does not name a column in " << tablename2 << "\n";
        return;
    }
    
    for(size_t i = 0; i < metaData[tablename2].numRows; i++) {
        hashBrowns[TableEntry(metaData[tablename2].data[i][index])].push_back(metaData[tablename2].data[i]);
    }
    
    
    
    for (size_t i = 0; i < printNum; i++) {
        cin >> junk;
        cin >> oneOrtwo;
        
        if (oneOrtwo == 1) {
            for (size_t j = 0; j < metaData[tablename1].coltypeVec.size(); j++) {


                if (junk == metaData[tablename1].colnameVec[j]) {
                    indices.push_back( { j, oneOrtwo});
                }
            }
            cout << junk << " ";
        } else {
            for (size_t j = 0; j < metaData[tablename2].coltypeVec.size(); j++) {
                if (junk == metaData[tablename2].colnameVec[j]) {
                    indices.push_back( { j, oneOrtwo});
                }
            }
            cout << junk << " ";
        }

    }
    
    cout << "\n";    

    
    for (size_t j = 0; j < metaData[tablename1].colnameVec.size(); j++) {
        if (metaData[tablename1].colnameVec[j] == colname1) {
            index = j;
        }
    }


    for (size_t j = 0; j < metaData[tablename2].colnameVec.size(); j++) {
        if (metaData[tablename2].colnameVec[j] == colname2) {
            index2 = j;
        }
    }


    unordered_map<TableEntry, vector<size_t> > hashMap2 = generateHash(index2, tablename2);

    for (size_t i = 0; i < metaData[tablename1].numRows; i++) {
        vector<size_t> &bucket = hashMap2[metaData[tablename1].data[i][index]];

        for(size_t j = 0; j < bucket.size(); j++) {
            if(!quiet) {
                for (size_t k = 0; k < indices.size(); k++) {
                    rowCount++;
                    if (indices[k].second == 1) {
                        notQuiet(i, {indices[k].first}, tablename1);
                    }
                    if(indices[k].second == 2) {
                        notQuiet(bucket[j], {indices[k].first}, tablename2);
                    }
                }
                cout << "\n";
            }
        }
    }
    cout << "Printed " << rowCount / 3 << " rows from joining " << tablename1 << " to " << tablename2 <<  "\n";
    metaData[tablename1].colnameVec.clear();

}

void generateFunc(){
    hashIndex.clear();
    BSTindex.clear();
    
    string junk;
    string tablename;
    string indexType;
    string colname;
    size_t indexInto = 666;

    cin >> junk; // holds for
    cin >> tablename;
    cin >> indexType;
    cin >> junk; //holds index
    cin >> junk; //holds on
    cin >> colname;

    if(metaData.find(tablename) == metaData.end()){
        cout << "Error during GENERATE: " << tablename << " does not name a table in database\n";
        return;
    }

    for(size_t i = 0; i < metaData[tablename].colnameVec.size(); i++){
        if(colname == metaData[tablename].colnameVec[i]){
            indexInto = i;
        }
    }
    
    if(indexInto == 666){
        cout << "Error during GENERATE: " << colname << " does not name a column in " << tablename << "\n";
        return;
    }

    if(indexType == "hash"){
        hashType = metaData[tablename].coltypeVec[indexInto];
        hashIndex = generateHash(indexInto, tablename);
    } else {
        bstType = metaData[tablename].coltypeVec[indexInto];
        BSTindex = generateBST(indexInto, tablename);
    }

    cout << "Created " << indexType << " index for table " << tablename << " on column " << colname << "\n";

}

int main(int argc, char** argv){


ios_base::sync_with_stdio(false);
cin >> std::boolalpha;
cout << std::boolalpha;

        int option_index = 0, option = 0;
        
        opterr = false;
        
        struct option longOpts[] = {{"help", no_argument, nullptr, 'h'},
                                    {"quiet", no_argument, nullptr, 'q'},
                                    {nullptr, 0, nullptr, '\0'}};
    
        while ((option = getopt_long(argc, argv, "hq", longOpts, &option_index)) != -1) {
            switch (option) {
                case 'h':
                    exit(0);
                    break;

                case 'g':
                    quiet = true;
                    break;   

                default:
                    exit(1);

            }  
        }

string cmd;

while(cmd != "QUIT"){
    cout << "% ";
    cin >> cmd;

    if(cmd[0] == '#'){
        getline(cin,cmd);
    }
    else if(cmd == "CREATE"){
        createFunc();
    }
    else if(cmd == "REMOVE"){
        removeFunc();
    }
    else if(cmd == "INSERT"){
        insertFunc();
    }
    else if(cmd == "PRINT"){
        printFunc();
    }
    else if(cmd == "DELETE"){
        deleteFunc();      
    }
    else if(cmd == "JOIN"){
        joinFunc(); 
    }
    else if(cmd == "GENERATE"){
        generateFunc();
    } 
    else if(cmd != "QUIT"){
        cout << "Error: unrecognized command\n";
        getline(cin, cmd);
    }
    

}

cout << "Thanks for being silly!\n";
return 0;
}