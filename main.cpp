#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
#define COLS 8
#define INSERT_BEFORE false

unsigned long djb2(string str) {
    const char *ptr = str.c_str();
    unsigned long hash = 5381;
    int c;
    while ((c = *ptr++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

class NodeRecord{
  NodeRecord *next;
  string *record;
  public:
    NodeRecord(){
        this->next = nullptr;
        this->record = new string[COLS];
        for (int i = 0; i < COLS;i++){
            this->record[i] = "";
        }

    } 
    NodeRecord(string *record){
        this->record = record;
        this->next = nullptr;
    } 
    NodeRecord *getNext(){
        return this->next;
    }
    void setNext(NodeRecord *next){
        this->next = next;
    }
    string *getRecord(){
        return this->record;
    }
    void setRecord(string *record){
        this->record = record;
    }
};



//put your node class here
class Node{
        Node *next;
        string key;
        int count = 0;
        NodeRecord *head;
        public:
        // assign values using this pointer
        Node(){
            this->next = nullptr;
            this->key = "";
            this->count = 0;
            this->head = new NodeRecord();
        }
        Node(string keyVal, string *record){
            this->key = keyVal;
            this->next = nullptr;
            this->head = new NodeRecord(record);
            this->count++;
        }
        Node *getNext(){
            return this->next;
        }
        void setNext(Node *next){
            this->next = next;
        }
        string getKey(){
            return this->key;
        }
        void setKey(string key){
            this->key = key;
        }
        void insertRecordBefore(string *record){
            NodeRecord *newRecord = new NodeRecord(record);
            newRecord->setNext(this->head);
            this->head = newRecord;
            this->count++; 
        }
        void insertRecordAfter(string *record){
            NodeRecord *newRecord = new NodeRecord(record);
            NodeRecord *cur = this->head;
            while( cur->getNext() != nullptr)
                cur = cur->getNext();

            cur->setNext( newRecord );
            this->count++; 
        }
        void print(){
            NodeRecord *cur = this->head;
            if( this->key !=""){
                cout << this->key;
                cout << " (" << this->count << ")," ;
            }
        }
        void getKeyNode(string keyVal){
            NodeRecord *cur = head;
        }
        void printRecords(){
            NodeRecord *cur = this->head;
            int i = 0;
            while (cur != nullptr){
                string *record = cur->getRecord();
                for ( i = 0; i < COLS;i++){
                    cout << record[i];
                    if (i != COLS - 1){
                        cout << ", ";
                    }
                }
                cur = cur->getNext();
                cout << endl;
            }
        }
};
//This is the linked list inside each index of the array
class HashIndexList{
    Node *node;
    Node *head;
    int size;
    public:
    HashIndexList(){
        this->node = new Node();
        head = node;
        size = -1;
    }
    void insertAfter( string keyVal, string *record){
        node = new Node( keyVal, record);
        Node* cur = this->head;
        while( cur->getNext() != nullptr)
            cur = cur->getNext();
        
        cur->setNext( node );
        this->size++;

    }
    void insertBefore(string keyVal, string *record){
        node = new Node(keyVal,record);
        node->setNext(head);
        head = node;
        this->size++;
        
    }
    void insertRecordAt(int keyPos, string *record, bool isInsertBefore ){
        Node *keyValNode = getNodeAt(keyPos, isInsertBefore);
        //cout << keyValNode->getKey() << endl;  
        isInsertBefore ? keyValNode->insertRecordBefore(record) : keyValNode->insertRecordAfter(record);
    }
    Node *getNodeAt(int pos, bool isInsertBefore ){
        Node* cur = isInsertBefore ? this->head : this->head->getNext();
        int index = 0;
        while(cur != nullptr){
            if (index == pos){
                return cur;
            }
            index++;
            cur = cur->getNext();
        }
        return nullptr;
    }
    int getSize(){
        return size+1;
    }
    void setSize(int size){
        this->size = size;
    }
    void print(){
        Node *cur = this->head;
        //cout << " size: " << this->getSize() << " ";
        while(cur != nullptr){
            cur->print();
            cur = cur->getNext();
        }
        //cout << "("; //To do: Need to get the number of people for every key value
        cout<<endl;
    
    }
    int keyExistAt(string keyVal, bool isInsertBefore){
        Node *cur = isInsertBefore ?  this->head : this->head->getNext();
        int index = 0;
        while(cur != nullptr){
            if (cur->getKey() == keyVal){
                return index;
            }
            index++;
            cur = cur->getNext();
        }
        return -1;  
    }
};



class HashTable {
    int slots;
    HashIndexList *hil; 
    public: 
    HashTable(int slots){
        this->slots = slots;
        hil = new HashIndexList[slots];
        for( int i =0; i< slots; i++){
            hil[i];
        }       // hil = createList( slots);
    }  
    int getSlots(){
        return this->slots;
    }
    void setSlots(int slots){
        this->slots = slots;
    }
    HashIndexList *getList(){
        return this->hil;

    }
    void setList(HashIndexList *list){
        this->hil = list;
    }
    int hash(string keyVal){
        return djb2(keyVal) % this->slots;
    }
    void insert(string **data, int colNum, int rows, bool insertBefore = true){
        string val;
        string *record;
        int index;
        for (int r = 1; r < rows; r++){
            record = data[r];
            val = data[r][colNum];
            //cout << val << ":Val ";
            index = hash(val);
            //cout << index << ":Index" << endl;
            int keyExistAt = hil[index].keyExistAt(val, insertBefore);
            //cout << keyExistAt << endl;
            if (keyExistAt == -1){
                insertBefore ? hil[index].insertBefore(val, record) : hil[index].insertAfter(val, record);
            } 
            else{
                hil[index].insertRecordAt(keyExistAt, record, insertBefore ); 
            }
            //hil[index].print();

        }
    }
    HashIndexList getListAt(int index){
        return hil[index];
    }
    void print(){
        HashIndexList list;
        for (int i = 0; i < this->getSlots();i++){
            list = this->getListAt(i);
            if( list.getSize() >0){
                cout << i << ": ";
                list.print();
            }    
        }
    }
    

};
int searchColumn( string **database, string column, int col) {
    //cout << column;
    for (int c = 0; c < col ;c++){
        if (column == database[0][c]){
            
            return c;
        }
    }
    return -1;
}

void getVal(istream &is, string &str) {
    char ch;
    string line;

    is >> ch;
    getline(is, line);

    str = ch + line;
}

bool isValidKey(const string &key) {
    string validKeys[8] = { "Id", "FirstName", "LastName", "Email", "Phone", "City", "State", "PostalCode" };
    for (int i = 0; i < 8; i++) {
        if (key == validKeys[i]) {
            return true;
        }
    }
    return false;
}
string ** create2Darray( int row, int col ){
    
    string **data = new string*[row];
    for (int i = 0; i < row; i++)
        data[i] = new string[col];
        
    return data;
}
void print( string **data, int row, int col, int width){
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++)
            cout<<"  " << data[i][j];
        cout<<endl;
    }
}
int numOfRows(ifstream &fp){
    string line;
    int row = 0;
    while (getline(fp, line)){
        row++;
    }
    row +=1;
    return row;
}
void assignValuesTo2DArray(string **data, int row, int col, string word){
    data[row][col] = word;
}

void parseFile( ifstream& iFp , string **data){
    int index = 0;
    int curCol = 0, curRow = 0;
    string line;
    while(!iFp.eof()){
        getline( iFp, line);
        char word[100];
    
        for(int i= 0; i< line.length(); i++){
            if( line[i] != '\t' && line[i] !='\r'){
                word[index] = line[i];
                index++;
            }
            if( line[i] =='\t' || line[i] =='\r' || i == line.length()-1 ){
                word[index] = '\0';
                assignValuesTo2DArray( data, curRow, curCol, word);
                curCol++;
                index=0;
            }
        }
        curCol=0;
        curRow++;
    }
    
}
void printCols(string **data){
    for (int i = 0; i < COLS;i++){
        cout << data[0][i];
        if (i != COLS - 1){
            cout << ", ";
        }
    }
    cout << endl;
}
void copyColumnData(string **data, string **db, int colNum, string colName, int colPos, int rows){
    data[0][colPos] = colName;
    for (int i = 1; i < rows; i++){
        data[i][colPos] = db[i][colNum];
    }
}
int main(int argc, char **argv) {
    if (argc != 4) {
        cout << "Usage: ./a.out filename.txt table_size key" << endl;
        return 1;
    }
    int rows = 0, cols = 0;
    string filename = argv[1];
    int tableSize = stoi(argv[2]);
    string key = argv[3];

    
    ifstream file(filename);
    if (!file) {
        cout << "Unable to open " << filename << endl;
        return 2;
    }

    if (!isValidKey(key)) {
        cout << "Invalid key: " << key << endl;
        return 3;
    }
    HashTable *hash = new HashTable(tableSize);
    rows = numOfRows(file) - 1;
    cols = COLS;
    
    string **dataWithoutColOrder = create2Darray(rows, cols);

    ifstream file1(filename);
    
    parseFile(file1, dataWithoutColOrder);
    
    //reorder the data basded on column name
    string **data = create2Darray(rows, cols);
    string validOrder[COLS] = { "Id", "FirstName", "LastName", "Email", "Phone", "City", "State", "PostalCode" };
    for( int i =0; i< COLS; i++ ){
        int colNum = searchColumn(dataWithoutColOrder, validOrder[i], cols);
        copyColumnData(data, dataWithoutColOrder, colNum, validOrder[i], i, rows);
    }
    //print(data, rows, cols, 10);
    // This is an example of how to retreive the tokens from the input file
    // You will need to modify this to fit your needs to build the hash table
    string line, token;
    while (getline(file, line)) {
        istringstream iss(line);
        while (getline(iss, token, '\t')) {
            cout << token << "\t";
        }
        cout << endl;
    }
    int colNum = searchColumn(data, key, COLS);
    hash->insert(data, colNum, rows, INSERT_BEFORE);
    
    //printHashTable(hash);//

    cout << "Commands:" << endl << "\tprint" << endl << "\tlookup <key>" << endl << "\tquit" << endl;
    string cmd, val;
    while (1) {
        cout << endl << "Enter a command:" << endl;
        cin >> cmd;
        if (cmd == "quit") {
            break;
        }
        else if (cmd == "print") {
            hash->print();
        }
        else if (cmd == "lookup") {
            getVal(cin, val);
            // lookup code here
            printCols(data);
            int hashIndex = hash->hash(val);
            //cout << hashIndex << fflush;
            
            int keyPos = hash->getListAt(hashIndex).keyExistAt(val, INSERT_BEFORE);
            if (keyPos != -1){
                Node *keyValNode = hash->getListAt(hashIndex).getNodeAt(keyPos, INSERT_BEFORE);
                keyValNode->printRecords();
            }
            else{
                cout << "No results" << endl;
            }
        }
        else {
            getline(cin, val);
            cout << "Invalid command" << endl;
        }
    }

    return 0;
}
