#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>
#include <cstdlib>
#include <cctype>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

void init();
void Lexical_Analysis();
void Lexical_Analysis_run();
void skipBlank();
void check_LineNum();
void push_TypeToken();
void push_Token_list(string Token, string word);
void print_Lexical();
bool check_Char_Type(int check);
void skip_line_Comment();
void skip_block_Comment();
void push_err_list(char err_type);
void output_Lexical();
void output_Syntax();
void Syntax_Analysis();
void nextToken();
void preToken();
void printcurToken();
void printSyntax(string syntax);
void parseCompUnit();
void parseDecl();
void parseConstDecl();
void parseBType();
void parseConstDef();
void parseConstInitVal();
void parseVarDecl();
void parseVarDef();
void parseInitVal();
void parseFuncDef();
void parseMainFuncDef();
void parseFuncType();
void parseFuncFParams();
void parseFuncFParam();
void parseBlock();
void parseBlockItem();
void parseStmt();
void parseForStmt();
void parseExp();
void parseCond();
void parseLVal();
void parsePrimaryExp();
void parseNumber();
void parseCharacter();
void parseUnaryExp();
void parseUnaryOp();
void parseFuncRParams();
void parseMulExp();
void parseAddExp();
void parseRelExp();
void parseEqExp();
void parseLAndExp();
void parseLOrExp();
void parseConstExp();
void parseErrorA();
bool parseErrorB(string name, int curScope);
void parseErrorC(string name);
void parseErrorD(string name, int cnt, int funcline);
void parseErrorE();
void parseErrorF(int linenum);
void parseErrorG();
void parseErrorH(string name);
void parseErrorI();
bool parseErrorJ();
void parseErrorK();
void parseErrorL(string formatString, int print_param_cnt, int printlinenum);
void parseErrorM();
void print_err_list();
void add_s_Table(string name, string type);

ifstream inputFile;
ofstream outputFile;
ofstream outputFile2;
ofstream errorFile;
string input_text = "";
string Token = "";

int input_text_len = 0;
int curpos = 0;
int glb_linenum = 1;
int last_nonT;
int last_func_line; //errore 처리 
//enum symbol_Token {
//    IDENFR, INTCON, STRCON, CHRCON, MAINTK, CONSTTK, INTTK, CHARTK, BREAKTK, CONTINUETK, IFTK, 
//    ELSETK, NOT, AND, OR, FORTK, GETINTTK, GETCHARTK, PRINTFTK, RETURNTK, PLUS, MINU,
//    VOIDTK, MULT, DIV, MOD, LSS, LEQ, GEQ, EQL, NEQ, ASSIGN, 
//    SEMICN, COMMA, LPARENT, RPARENT, LBRACK, RBRACK, LBRACE, RBRACE
//};
//
//string symbol_Token_print[42] = {
//    "IDENFR", "INTCON", "STRCON", "CHRCON", "MAINTK", "CONSTTK", "INTTK", "CHARTK", "BREAKTK", "CONTINUETK", "IFTK",
//    "ELSETK", "NOT", "AND", "OR", "FORTK", "GETINTTK", "GETCHARTK", "PRINTFTK", "RETURNTK", "PLUS", "MINU",
//    "VOIDTK", "MULT", "DIV", "MOD", "LSS", "LEQ", "GRE", "GEQ", "EQL", "NEQ", "ASSIGN",
//    "SEMICN", "COMMA", "LPARENT", "RPARENT", "LBRACK", "RBRACK", "LBRACE", "RBRACE"
//};

//enum BType{
//	int,
//	char
//};

struct Token_word {
    string Token;
    string word;
    int line_num;
}; 
Token_word Token_list[100001];
int Tlist_top = 0;
Token_word curToken;

struct error_word{
	int linenum;
	char err_type;
};
error_word error_list[100001];
int err_top = 0;

struct SymbolTable{
	string name;
	string type;
	int scope;
};
int glb_SS= 1;	//symbol_scope	블이 시작할때마다++ 
bool isglbscope = true;		//전역변수라면 
int ST_top = 0;		//symbotabletop
int curScope = 1;	
int funcStartScope = 0; //함수시작시 스코프
bool isglbScope = false;	//함수 시작 스코프와 같으면 

struct funcparam{
	string type;
	string name;
	int paramcnt = 0;
	vector<string> paramtype;
};
funcparam funcInfo[100001];
funcparam ErrEtemp;
int funcInfo_top = 0;

vector<string> check_param;	//에러 DE처리 일단 방치 
int paramscnt = 0;

bool isneedreturn = true;		//리턴이 필요한 함수판달 
bool isstmtreturn = false;		// stmt에 리턴이 나왔는지 판단 
bool isBorC = false;			// 브레이크나 컨티뉴인지  
bool isfuncparam = false;
bool isfuncDef = false;		//에러e처리 함수선언인지 아닌지 확인 

string DeclType;
string TypeTemp;
SymbolTable symbolTable[100001];

void add_s_Table(string name, string type){
	symbolTable[ST_top].name = name;
	symbolTable[ST_top].type = type;
	if(isglbscope){
		symbolTable[ST_top].scope = 1;
	}else if(isglbScope) {
		symbolTable[ST_top].scope = curScope;
	}else {
		symbolTable[ST_top].scope = glb_SS;
	}
	ST_top++;
}

bool compareSymbolScope(const SymbolTable &a, const SymbolTable &b){
	return a.scope < b.scope;
}

void print_symbol_list(){
	stable_sort(symbolTable, symbolTable + ST_top, compareSymbolScope);
	
	for(int i = 0; i < ST_top ; i++){
		cout << symbolTable[i].scope << " " << symbolTable[i].name << " " << symbolTable[i].type << endl;
		outputFile2 << symbolTable[i].scope << " " << symbolTable[i].name << " " << symbolTable[i].type << endl;
	}
}

void push_Token_list(string Token, string word) {
    Token_list[Tlist_top].Token = Token;
    Token_list[Tlist_top].word = word;
    Token_list[Tlist_top].line_num = glb_linenum;
    Tlist_top++;
}

void push_err_list(char err_type) {
    error_list[err_top].err_type = err_type;
    error_list[err_top].linenum = curToken.line_num;
    err_top++;
}

void push_err_list(char err_type, int err_line) {
    error_list[err_top].err_type = err_type;
    error_list[err_top].linenum = err_line;
    err_top++;
}

bool compareErrLine(const error_word& a, const error_word& b) {
    return a.linenum < b.linenum;
}

void print_err_list(){
	int last_print_line = -1;
	sort(error_list, error_list + err_top, compareErrLine);
	
	for(int i = 0; i < err_top ; i++){
		if(error_list[i].linenum != last_print_line){	//동일라인에서 발생한에러는 출력x 
			cout << error_list[i].linenum << " " << error_list[i].err_type << endl;
			errorFile << error_list[i].linenum << " " << error_list[i].err_type << endl;
			last_print_line = error_list[i].linenum;
		}
	}
}

void Lexical_Analysis_run() {
	char err_type = 'a';
    Token = "";
    if (isalpha(input_text[curpos]) || input_text[curpos] == '_') {
        while (isdigit(input_text[curpos]) || isalpha(input_text[curpos]) || input_text[curpos] == '_') {
            Token += input_text[curpos];
            curpos++;
        }
        push_TypeToken();
    } 
	// if test has float
        /* bool isFloat = false;
        while(isdigit(input_text[curpos])||input_text[curpos]=='.') {
            if(input_text[curpos]=='.')
                isFloat = true;
            token += input_text[curpos];
            curpos++;
		}
        if(isFloat)
        {
    		push_Token_list("FLOATCON?", Token);
        }
        else{
            push_Token_list("INTCON", Token);
        } */ 
	else if(isdigit(input_text[curpos])){
		while(isdigit(input_text[curpos])){
			Token += input_text[curpos];
			curpos++;
		}
		push_Token_list("INTCON", Token);
	} else if(input_text[curpos] == '"'){
		Token += input_text[curpos];
		curpos++;
		while(input_text[curpos] != '"'){
			Token += input_text[curpos];
			curpos++;
		}
		Token += input_text[curpos];
		curpos++;
		push_Token_list("STRCON", Token);
	} else if(input_text[curpos] == '&') {
		if(input_text[curpos + 1] == '&'){
			push_Token_list("AND", "&&");
        	curpos += 2;	
		} else{
//			push_err_list(err_type);	어휘분석중 오류 주석해제시 구문분석중 오류랑 중복 출력 
			push_Token_list("EAND", "&");
			curpos++;
		}
    } else if(input_text[curpos] == '|'){
        if(input_text[curpos + 1] == '|'){
			push_Token_list("OR", "||");
        	curpos += 2;	
		} else{
//			push_err_list(err_type);
			push_Token_list("EOR", "|");
			curpos++;
		}
    } else if(input_text[curpos] == '+') {
        push_Token_list("PLUS", "+");
        curpos++;
    } else if(input_text[curpos] == '-') {
        push_Token_list("MINU", "-");
        curpos++;
    } else if(input_text[curpos] == '*') {
        push_Token_list("MULT", "*");
        curpos++;
    } else if(input_text[curpos] == '/') {
        if(input_text[curpos + 1] == '/'){
        	skip_line_Comment();
		} else if(input_text[curpos + 1] == '*'){
			skip_block_Comment();
		} else{
			push_Token_list("DIV", "/");
        	curpos++;
		}
    } else if(input_text[curpos] == '%') {
        push_Token_list("MOD", "%");
        curpos++;
    } else if(input_text[curpos] == ';') {
        push_Token_list("SEMICN", ";");
        curpos++;
    } else if(input_text[curpos] == ',') {
        push_Token_list("COMMA", ",");
        curpos++;
    } else if(input_text[curpos] == '(') {
        push_Token_list("LPARENT", "(");
        curpos++;
    } else if(input_text[curpos] == ')') {
        push_Token_list("RPARENT", ")");
        curpos++;
    } else if(input_text[curpos] == '[') {
        push_Token_list("LBRACK", "[");
        curpos++;
    } else if(input_text[curpos] == ']') {
        push_Token_list("RBRACK", "]");
        curpos++;
    } else if(input_text[curpos] == '{') {
        push_Token_list("LBRACE", "{");
        curpos++;
    } else if(input_text[curpos] == '}') {
        push_Token_list("RBRACE", "}");
        curpos++;
    } else if(input_text[curpos] == '!') {
        if (input_text[curpos + 1] == '=') {
            push_Token_list("NEQ", "!=");
            curpos += 2;
        } else{
            push_Token_list("NOT", "!");
            curpos++;
        }
    } else if(input_text[curpos] == '<') {
        if(input_text[curpos + 1] == '=') {
            push_Token_list("LEQ", "<=");
            curpos += 2;
        } else{
            push_Token_list("LSS", "<");
            curpos++;
        }
    } else if(input_text[curpos] == '>') {
        if(input_text[curpos + 1] == '=') {
            push_Token_list("GEQ", ">=");
            curpos += 2;
        } else{
            push_Token_list("GRE", ">");
            curpos++;
        }
    } else if(input_text[curpos] == '=') {
        if(input_text[curpos + 1] == '=') {
            push_Token_list("EQL", "==");
            curpos += 2;
        } else{
            push_Token_list("ASSIGN", "=");
            curpos++;
        }
    } else if(input_text[curpos] == '\'') {
    Token = ""; 
    Token += '\''; 
    curpos++; 

    if(input_text[curpos] == '\\') {
	        Token += '\\';
	        curpos++;
	        if(input_text[curpos] == 'n' || input_text[curpos] == 't' || input_text[curpos] == 'v' ||
	           input_text[curpos] == '\\' || input_text[curpos] == '\'' || input_text[curpos] == '\"' ||
	           input_text[curpos] == 'a' || input_text[curpos] == 'b' || 
	           input_text[curpos] == '0' || input_text[curpos] == 'f') {
	            Token += input_text[curpos];
	            curpos++;
	        }
	    } else {
	        int check_ASCII = input_text[curpos];
	        if(check_Char_Type(check_ASCII)) {
	            Token += input_text[curpos];
	            curpos++;
	        }
	    }

	    if(input_text[curpos] == '\'') {
	        Token += '\'';
	        curpos++;
	        push_Token_list("CHRCON", Token);
	   	}
	} else {
		curpos++;
	}
}

bool check_Char_Type(int check){
	if((check >= 32 && check <= 126 ) || (check >= 7 && check <= 12 ) || check == 34 
	|| check == 39 || check == 92 || check == 0) 
		return true;
	return false;
}

void push_TypeToken() {
    if(Token == "main") {
        push_Token_list("MAINTK", "main");
    } else if(Token == "const") {
        push_Token_list("CONSTTK", "const");
    } else if(Token == "int") {
        push_Token_list("INTTK", "int");
    } else if(Token == "char") {
        push_Token_list("CHARTK", "char");
    } else if(Token == "break") {
        push_Token_list("BREAKTK", "break");
    } else if(Token == "continue") {
        push_Token_list("CONTINUETK", "continue");
    } else if(Token == "if") {
        push_Token_list("IFTK", "if");
    } else if(Token == "else") {
        push_Token_list("ELSETK", "else");
    } else if(Token == "for") {
        push_Token_list("FORTK", "for");
    } else if(Token == "getint") {
        push_Token_list("GETINTTK", "getint");
    } else if(Token == "getchar") {
        push_Token_list("GETCHARTK", "getchar");
    } else if(Token == "printf") {
        push_Token_list("PRINTFTK", "printf");
    } else if(Token == "return") {
        push_Token_list("RETURNTK", "return");
    } else if(Token == "void") {
        push_Token_list("VOIDTK", "void");
    } else{
        push_Token_list("IDENFR", Token);
    }
}



void check_LineNum() {
    while(curpos < input_text_len && (input_text[curpos] == '\n')){
    	glb_linenum++;
    	curpos++;
	}
}

void skipBlank() {
    while (curpos < input_text_len &&
	(input_text[curpos] == ' ' || input_text[curpos] == '\t' || input_text[curpos] == '\r')) {
    	curpos++;
    }
}

void skip_line_Comment(){
	while(input_text[curpos] != '\n' && input_text[curpos] != '\r' && curpos < input_text_len){
		curpos++;
	}
}

void skip_block_Comment(){
	while(!(input_text[curpos] == '*' && input_text[curpos + 1] == '/') && curpos < input_text_len){
		curpos++;
	}
	if(curpos < input_text_len){
		curpos += 2;
	}
}

void print_Lexical() {
	if(err_top != 0){
		for(int i = 0; i < err_top; i++){
			cout << error_list[i].linenum << " " << error_list[i].err_type << "\n";
		}
	} else{
		for (int i = 0; i < Tlist_top; i++) {
        cout << Token_list[i].Token << " " << Token_list[i].word << " " << Token_list[i].line_num << "\n";
   		}
	}
}

void output_Lexical() {
	outputFile.open("lexer.txt");
    if (!outputFile.is_open()) {
        cerr << "can't open output file" << endl;
        return;
    }
	if(err_top != 0){
        for(int i = 0; i < err_top; i++){
            outputFile << error_list[i].linenum << " " << error_list[i].err_type << "\n";
        }
    } else {
        for (int i = 0; i < Tlist_top; i++) {
            outputFile << Token_list[i].Token << " " << Token_list[i].word << "\n";
        }
    }
}


void output_Syntax() {
	if(err_top != 0){
        for(int i = 0; i < err_top; i++){
            outputFile << error_list[i].linenum << " " << error_list[i].err_type << "\n";
        }
    } else {
        for (int i = 0; i < Tlist_top; i++) {
            outputFile << Token_list[i].Token << " " << Token_list[i].word << "\n";
        }
    }
}

void init() {
    inputFile.open("testfile.txt");
    if (!inputFile.is_open()) {
        cerr << "can't open file" << endl;
        return;
    }
	
	outputFile.open("parser.txt");
    if (!outputFile.is_open()) {
        cerr << "can't open output file" << endl;
        return;
    }
    
    outputFile2.open("symbol.txt");
    if (!outputFile.is_open()) {
        cerr << "can't open symbol file" << endl;
        return;
    }
    
    errorFile.open("error.txt");
    if (!outputFile.is_open()) {
        cerr << "can't open output file" << endl;
        return;
    }
    
    string input_buffer;

    while (getline(inputFile, input_buffer)) {
    	input_text += input_buffer + '\n';
    }

    input_text_len = input_text.length();
}

void Lexical_Analysis() {
    while (curpos < input_text_len) {
    	skipBlank();       // 공백 문자 건너뛰기
        check_LineNum();     // 줄번호 처리
        
        // 토큰이 인식되지 않는 경우에도 curpos를 증가시켜 무한 루프를 방지
        int previous_pos = curpos;
        Lexical_Analysis_run();
        
        // Lexical_Analysis_run 함수가 curpos를 업데이트하지 않았으면 강제로 증가
        if (curpos == previous_pos) {
            curpos++;
        }
        
        // 파일 끝을 체크하여 루프 탈출
        if (curpos >= input_text_len) {
            break;
        }
    }
}

//구문 분석
void Syntax_Analysis() {
	curpos = 0;
	curToken = Token_list[curpos];
	parseCompUnit();
}

void nextToken() {
	if (curpos + 1 <= Tlist_top) { // curpos가 Tlist_top을 초과하지 않도록 보호
        curpos++;
        curToken = Token_list[curpos];
    } else {
        cerr << "Error: Token list out of range." << endl;
    }
}

void preToken() {
	if (curpos > 0) { // curpos가 0보다 작아지지 않도록 보호
        curpos--;
        curToken = Token_list[curpos];
    } else {
        cerr << "Error: Token list out of range." << endl;
    }
}

void printcurToken() {
//	cout << Token_list[curpos].Token << " " << Token_list[curpos].word << endl;
	outputFile << Token_list[curpos].Token << " " << Token_list[curpos].word << "\n";
}

void printSyntax(string syntax) {
	//cout << "<" << syntax << ">" << endl;
	outputFile << "<" << syntax << ">" << endl;
}

// CompUnit → {Decl} {FuncDef} MainFuncDef 
void parseCompUnit() {
	string temp1 = Token_list[curpos].Token;
	nextToken();
	string temp2 = Token_list[curpos].Token;
	nextToken();
	string temp3 = Token_list[curpos].Token;
	preToken();
	preToken();
	if(temp1 == "INTTK" && temp2 == "MAINTK"){
		parseMainFuncDef();
		printSyntax("CompUnit");
		return;
	} //함수선언 처리 
	else if((temp1 == "INTTK" && temp3 == "LPARENT") || (temp1 == "CHARTK" && temp3 == "LPARENT") || (temp1 == "VOIDTK" && temp3 == "LPARENT")){
		parseFuncDef();
		parseCompUnit();
	} //선언부 처리 
	else{
		parseDecl();
		parseCompUnit();
	}
	
}

//Decl → ConstDecl | VarDecl
void parseDecl() {
	if(curToken.Token == "INTTK" || curToken.Token == "CHARTK"){
		parseVarDecl();
	} else if(curToken.Token == "CONSTTK"){
		parseConstDecl();
	}
	
}

//ConstDecl → 'const' BType ConstDef { ',' ConstDef } ';' // i
void parseConstDecl() {
	printcurToken();//const
	DeclType = curToken.word;
	nextToken();
	printcurToken();//Btype
	DeclType += curToken.word;	//심볼테이블에 저장할 타입저장 
	string TypeTemp = DeclType;	//추가 선언대비 
	nextToken();
	
	parseConstDef();
	while(curToken.word == ","){ // ,후 연속선언 처리  
		printcurToken();
		nextToken();
		
		DeclType = TypeTemp; 
		parseConstDef();
	}	
	
	parseErrorI(); //  ;
	
	printSyntax("ConstDecl");
}

//ConstDef → Ident [ '[' ConstExp ']' ] '=' ConstInitVal // k
void parseConstDef() {
	last_nonT = curToken.line_num;
	
	printcurToken();//Ident
	string Identname = curToken.word;
	nextToken();
	
	if(curToken.word == "["){	//배열 선언 처리 
		printcurToken();//[
		DeclType += "array";
		nextToken();
		
		parseConstExp();
		
		parseErrorK(); //  ]
	}
	parseErrorB(Identname, glb_SS);
	
	printcurToken();//=
	nextToken();
		
	parseConstInitVal();
	
	printSyntax("ConstDef");
}

//ConstInitVal → ConstExp | '{' [ ConstExp { ',' ConstExp } ] '}' | StringConst
void parseConstInitVal() {
	last_nonT = curToken.line_num;
	
	if(curToken.word == "{"){//배열 처리 
		printcurToken();//{
		nextToken();
		
		if(curToken.word != "}"){
			parseConstExp();
		 
			while(curToken.word == ","){
				printcurToken();//	,
				nextToken();
	
				parseConstExp();
			}
		}
		
		printcurToken();//	}
		nextToken();
	} 
	// 상수문자열 처리 
	else if(curToken.Token == "STRCON"){
		printcurToken();// stringConst
		nextToken();
	} 
	//그냥 상수처리 
	else {
		parseConstExp();
	}
	
	printSyntax("ConstInitVal");
}

//VarDecl → BType VarDef { ',' VarDef } ';' // i
void parseVarDecl() {
	printcurToken(); // BType
	TypeTemp = curToken.word;	//심볼테이블을위한 변수타입 저장 
	DeclType = TypeTemp;
	nextToken();
	
	parseVarDef();
	
	while(curToken.word == ","){
		printcurToken();	//,
		nextToken();
		
		DeclType = TypeTemp;
		parseVarDef();
	}
	
	parseErrorI(); //  ;
	
	printSyntax("VarDecl");
};

//VarDef → Ident [ '[' ConstExp ']' ] | Ident [ '[' ConstExp ']' ] '=' InitVal // k
void parseVarDef() {
	last_nonT = curToken.line_num;
	
	printcurToken();//Ident
	string Identname = curToken.word;
	nextToken();
	
	if(curToken.word == "["){	//배열 선언 처리 
		printcurToken();//[
		DeclType += "array";
		nextToken();
		
		parseConstExp();
		
		parseErrorK(); //  ]
	}
	parseErrorB(Identname, glb_SS);
	
	if(curToken.word == "="){	//배열 선언 + 초기화 
		printcurToken();//=
		nextToken();
		
		parseInitVal();
	} 
	// = 처리 오류 있을거같은데? 
	printSyntax("VarDef");
}

//InitVal → Exp | '{' [ Exp { ',' Exp } ] '}' | StringConst 
void parseInitVal() {
	if(curToken.word == "{"){//배열 처리 
		printcurToken();//{
		nextToken();
		
		parseExp();
		 
		while(curToken.word == ","){
			printcurToken();//	,
			nextToken();
			
			parseExp();
		}
		
		printcurToken();//	}
		nextToken();
	} 
	// 상수문자열 처리 
	else if(curToken.Token == "STRCON"){
		printcurToken();// stringConst
		nextToken();
	} 
	//그냥 상수처리 
	else{
		parseExp();
	}
	printSyntax("InitVal");
}
 
//FuncDef → FuncType Ident '(' [FuncFParams] ')' Block // j
void parseFuncDef() {
	isfuncDef = true; 
	isstmtreturn = false;
	if(curToken.word == "int" || curToken.word == "char"){
		DeclType = curToken.word + "func";
		funcInfo[funcInfo_top].type = curToken.word;
		isneedreturn = true;
		parseFuncType();
	}
	if(curToken.word == "void"){
		DeclType = curToken.word + "func";
		funcInfo[funcInfo_top].type = curToken.word;
		isneedreturn = false;
		parseFuncType();
	}
	
	printcurToken();// Ident
	string Identname = curToken.word;
	funcInfo[funcInfo_top].name = curToken.word;
	nextToken();
	
	bool valid_func = false;
	if(parseErrorB(Identname, 1)){
		valid_func = true;
	}
	
	printcurToken();// (
	last_nonT = curToken.line_num;
	glb_SS++;	//함수 지역변수 처리를 위해+ 
	curScope = glb_SS; 
	isglbscope = false;
	nextToken();
	
	if(curToken.word != ")"){	//일단 이경우만 예외처리 
		//함수 선언시 닫는 괄호가 없을떄 바로 블럭파싱 
		if(curToken.word != "{"){	 
			parseFuncFParams();
		}
	}
	
	parseErrorJ(); //  )
	
	glb_SS--;	//매개변수 처리 복구 
	curScope = glb_SS; 
	funcStartScope = glb_SS + 1;
	parseBlock();
	
	curScope = glb_SS + 1; 
	isglbscope = true;
	if(valid_func){
		funcInfo_top++;	//함수정보 ++	
	}
	
	isfuncDef = false;
	printSyntax("FuncDef");
}

//MainFuncDef → 'int' 'main' '(' ')' Block // j
void parseMainFuncDef() {
	isneedreturn = true;
	isstmtreturn = false;	//함수 시작시 false로 초기화 
	
	printcurToken();// int
	nextToken();
		
	printcurToken();// main
	nextToken();
	
	printcurToken();// (
	last_nonT = curToken.line_num;
	nextToken();
	
	parseErrorJ(); //  )
	
	isglbscope = false; 
	curScope = glb_SS;
	funcStartScope = glb_SS + 1;
	
	parseBlock();
	
	isglbscope = true;
	
	printSyntax("MainFuncDef");	
}

void parseFuncType() {
	printcurToken();// FuncType int void char
	nextToken();
	
	printSyntax("FuncType");	
}

//FuncFParams → FuncFParam { ',' FuncFParam } 
void parseFuncFParams() {
	last_nonT = curToken.line_num;
	
	parseFuncFParam();
	
	while(curToken.word == ","){
		printcurToken();//	,
		nextToken();

		parseFuncFParam();
	}
	
	printSyntax("FuncFParams");		
}

//FuncFParam → BType Ident ['[' ']'] // k
void parseFuncFParam() {
	printcurToken();//	BType
	DeclType = curToken.word;
	funcInfo[funcInfo_top].paramcnt++;
	nextToken();
	
	printcurToken();//	Ident
	string Identname = curToken.word;
	nextToken();
	
	if(curToken.word == "["){
		printcurToken();//	[
		DeclType += "array";
		
		last_nonT = curToken.line_num;
		nextToken();
		
		parseErrorK();
	}
	parseErrorB(Identname, glb_SS);
	funcInfo[funcInfo_top].paramtype.push_back(DeclType);	//parseB에서 DeclType 형식화 
	
	printSyntax("FuncFParam");	
}

//'{' { BlockItem } '}' 
void parseBlock() {
	printcurToken();//	{
	glb_SS++;	//블럭이면 증가 
	curScope++;
	if(funcStartScope == curScope){
		isglbScope = true;
	} else{
		isglbScope = false;
	}
	nextToken();
	
	
	while (curToken.word != "}") {
		parseBlockItem();		
	}
	
	if(funcStartScope == curScope){		//함수와 동일스코프에서 리턴체크 
		parseErrorG();
	}
	
	printcurToken();//	}
	isstmtreturn = false;
	
	curScope--;
	if(funcStartScope == curScope){	//블럭 탈출할시 다시한번 비교 
		isglbScope = true;
	}
	nextToken();
	
	
	printSyntax("Block");	
}

//Decl | Stmt 
void parseBlockItem() {
	//Decl
	if(curToken.Token == "INTTK" || curToken.Token == "CHARTK" || curToken.Token == "CONSTTK") {
		parseDecl();
	}
	//Stmt
	else {
		parseStmt();
	}
}

void parseStmt() {
	last_nonT = curToken.line_num;
	
	if(curToken.word == "if") {// 'if' '(' Cond ')' Stmt [ 'else' Stmt ] // j
		printcurToken();//	if
		nextToken();
		
		printcurToken();//	(
		nextToken();
		
		parseCond();
		
		parseErrorJ();
		
		parseStmt();
		
		if(curToken.word == "else") {
			printcurToken();//	else
			nextToken();	
			
			parseStmt();
		}
	} 
	//'for' '(' [ForStmt] ';' [Cond] ';' [ForStmt] ')' Stmt 
	else if(curToken.word == "for") {
		isBorC = true;
		
		printcurToken();//	for
		nextToken();
		
		printcurToken();//	(
		nextToken();
		
		if(curToken.word != ";"){
			parseForStmt();
		}
		
		printcurToken();//	첫번째;
		nextToken();
		
		if(curToken.word != ";"){
			parseCond();
		}
		
		printcurToken();//	두번째;
		nextToken();
		
		if(curToken.word != ")"){
			parseForStmt();
		}
		
		printcurToken();//	)
		nextToken();
		
		parseStmt();
		isBorC = false;	//반복문 밖 브레이크 컨티뉴 처리 
	} 
	//'break' ';' | 'continue' ';' // 
	else if(curToken.word == "break" || curToken.word == "continue"){
		parseErrorM();
		printcurToken();//	break or continue
		last_nonT = curToken.line_num;
		nextToken();
		
		parseErrorI();
	} 
	//'return' [Exp] ';' // i
	else if(curToken.word == "return"){
		printcurToken();//	return
		last_nonT = curToken.line_num;
		int returnline = curToken.line_num;		//라스트논터미널 써도되지만 어지러울수있으니 새로 선언 
		
		isstmtreturn = true;
		
		nextToken();
		
		if(curToken.word != ";"){//exp 확인 
			parseErrorF(returnline);
			parseExp();
		} 
		
		parseErrorI();
	}
	//'printf''('StringConst {','Exp}')'';' // i j
	else if(curToken.word == "printf"){
		printcurToken();//	printf
		int printlinenum = curToken.line_num;
		nextToken();
		
		printcurToken();//	(
		nextToken();
		
		string temp = curToken.word;
		printcurToken();//	StringConst
		last_nonT = curToken.line_num;
		nextToken();
		
		int expcnt = 0;
		while(curToken.word == ","){
		
			printcurToken();//	,
			nextToken();
			
			parseExp();
			expcnt++;
		}
		
		parseErrorJ();	// ) 처리 
		parseErrorI();	// ; 처리 
		parseErrorL(temp, expcnt, printlinenum);
	}else if(curToken.word == "{"){
		parseBlock();
	}
	//LVal + exp
	else{
		//일단 LVal인지 판단 
		int index = 0;
		int isLVal = 0;
		while (true)
		{
			if(curToken.word == ";")
				break;
			if(curToken.word == "=")
			{
				isLVal = 1;
				break;
			}
			index++;
			
			if(curpos >= Tlist_top){
				break;
			}else {
				nextToken();
			}
		}
		// LVal의 위치로 돌아가기
		while(index)
		{
			preToken();
			index--;
		}
		//LVal '=' 'getint''('')'';' // i j
		//LVal '=' 'getchar''('')'';' // i j
		if(isLVal){
			parseErrorH(curToken.word);		//좌향식 ident가 const값일경우 처리 
			parseLVal();
			
			printcurToken();//	=
			nextToken();
			
			if(curToken.word == "getint" || curToken.word == "getchar"){
				printcurToken();//getint
				nextToken();
				
				printcurToken();//	(
				last_nonT = curToken.line_num;
				nextToken();
				
				parseErrorJ();
		
				parseErrorI();
			} 
			// LVal '=' Exp ';' // i
			else{
				parseExp();
				
				parseErrorI();
			}
		} 
		//[Exp] ';' // i
		else{
			if(curToken.word != ";"){
				parseExp();
			}
			parseErrorI();
		}
	}
	printSyntax("Stmt");
}

//ForStmt → LVal '=' Exp
//if test for문에 int선언이 될때 
void parseForStmt() {
//	if(curToken.word == "int"){
//		printcurToken();//decl for in int 
//		nextToken();
//	}
	parseErrorH(curToken.word);	//cosnt값 변환 에러 처리 

	parseLVal();
	
	printcurToken();//	=
	nextToken();
	
	parseExp();
	
	printSyntax("ForStmt");
}

//Exp → AddExp 
void parseExp() {
	last_nonT = curToken.line_num;
	
	parseAddExp();
	
	printSyntax("Exp");
}

//Cond → LOrExp 
void parseCond() {
	last_nonT = curToken.line_num;
	
	parseLOrExp();
	
	printSyntax("Cond");
}

//LVal → Ident ['[' Exp ']'] // k
void parseLVal() {
	printcurToken();//	Ident
	parseErrorC(curToken.word);
	nextToken();
	
	if(curToken.word == "["){
		printcurToken();//	[
		last_nonT = curToken.line_num;
		nextToken();
		
		parseExp();
		
		parseErrorK();
	}
	
	printSyntax("LVal");
}

//PrimaryExp → '(' Exp ')' | LVal | Number | Character// j
void parsePrimaryExp() {	
	if (curToken.word == "(") {//exp
        printcurToken();  // (
        nextToken();
        
        parseExp(); 
        
        parseErrorJ();
    } else if(isdigit(curToken.word[0])){//Number
    	parseNumber();
	} else if(curToken.Token == "IDENFR"){
		parseLVal();
	} else {//Character
		parseCharacter();
	}
	 printSyntax("PrimaryExp");
}

//Number → IntConst 
void parseNumber() {
	printcurToken();  // IntConst
    nextToken();
    
    printSyntax("Number"); 
}

//Character → CharConst 
void parseCharacter() {
	printcurToken();  // charConst
    nextToken();
    
    printSyntax("Character"); 
}

//UnaryExp → PrimaryExp | Ident '(' [FuncRParams] ')' | UnaryOp UnaryExp // j
void parseUnaryExp() {
	nextToken();
	string temp1 = curToken.word;	//ident일때(받기 
	preToken();
	 
	if(curToken.word == "+" || curToken.word == "-" || curToken.word == "!"){
		parseUnaryOp();
		
		parseUnaryExp();
	} else if(curToken.Token == "IDENFR" && temp1 == "("){
		isfuncparam = true;	//함수 매개변수 받기 
		check_param.clear();
		
		printcurToken();  // ident
		parseErrorC(curToken.word);
		last_nonT = curToken.line_num;
    	last_func_line = curToken.line_num;
    	string identname = curToken.word;
    	
    	
    	for (int i = 0; i < funcInfo_top; i++){	//현재 함수의 매개변수 
    		if (funcInfo[i].name == identname) {
            	ErrEtemp =  funcInfo[i];
				break;
       		}
		}
    	
		nextToken();
    
    	printcurToken();  //(
    	nextToken();
    	
    	if(curToken.word != ")"){
    		parseFuncRParams();
		}
		
		if(!parseErrorJ()){		//에러j가 우선순위를 가짐 
			parseErrorD(identname, 	paramscnt, last_func_line);
		}
		
		isfuncparam = false;	//함수 매개변수 끄기 
	} else{
		parsePrimaryExp();
	}
	printSyntax("UnaryExp");
}

//UnaryOp → '+' | '-' | '!' !는 Cond에만 출현 
void parseUnaryOp() {
	printcurToken();// + || - || !
    nextToken();
    
    printSyntax("UnaryOp"); 
}

//FuncRParams → Exp { ',' Exp } 
void parseFuncRParams() {
	paramscnt = 0;	//errorD 매개변수 갯수 초기화 
	
	parseErrorE();	 
	parseExp();  // 첫 번째 매개변수 처리
	if(isfuncparam){	//errorD처리중 
		paramscnt++;
	}
    
    while (curToken.word == ",") {
        printcurToken();  // ,
        nextToken();
        
        parseErrorE();
        parseExp(); 
        
        if(isfuncparam){	//errorD처리중 
			paramscnt++;
		}
    }
    
    printSyntax("FuncRParams");
}

//MulExp → UnaryExp | MulExp ('*' | '/' | '%') UnaryExp 
void parseMulExp() {
	parseUnaryExp(); //UnaryExp+ 의 형태 

	printSyntax("MulExp");

	if(curToken.word == "*" || curToken.word == "/" || curToken.word == "%"){
		printcurToken();  // '*' | '/' | '%'
        nextToken();
		
		parseMulExp();
	}
}

//AddExp → MulExp | AddExp ('+' | '-') MulExp 
void parseAddExp() {
	parseMulExp();
	
	printSyntax("AddExp");
	
	if(curToken.word == "+" || curToken.word == "-"){
		printcurToken();  // '+' | '-'
        nextToken();
        
		parseAddExp();
	}
}

//RelExp → AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp 
void parseRelExp() {
	parseAddExp();
	
	printSyntax("RelExp");
	
	if(curToken.word == "<" || curToken.word == ">" || curToken.word == "<=" || curToken.word == ">="){
		printcurToken();  // '<' | '>' | <= | >=
        nextToken();
		
		parseRelExp();
	}
}

//EqExp → RelExp | EqExp ('==' | '!=') RelExp 
void parseEqExp() {
	parseRelExp();

	printSyntax("EqExp");
		
	if(curToken.word == "==" || curToken.word == "!="){
		printcurToken();  // == | !=
        nextToken();
		
		parseEqExp();
	}
}

//LAndExp → EqExp | LAndExp '&&' EqExp
void parseLAndExp() {
	parseEqExp();
	
	printSyntax("LAndExp");
	
	parseErrorA();
	
	if(curToken.Token == "AND"){
		printcurToken();  // &&
        nextToken();
		
		parseLAndExp();
	}
}

//LOrExp → LAndExp | LOrExp '||' LAndExp
void parseLOrExp() {
	parseLAndExp();
	
	printSyntax("LOrExp");
	
	parseErrorA();
	
	if(curToken.Token == "OR"){
		printcurToken();  // ||
        nextToken();
		
		parseLOrExp();
	}
}

//ConstExp → AddExp 주의 ident는 반드시 챵량 
void parseConstExp() {
	last_nonT = curToken.line_num;
	
	parseAddExp();
	
	printSyntax("ConstExp");
}
 
void parseErrorA(){
	if(curToken.Token == "EAND"){
		push_err_list('a');
		nextToken();
		
		parseLAndExp();
	}else if(curToken.Token == "EOR"){
		push_err_list('a');
		nextToken();
		
		parseLOrExp();
	}
}

bool parseErrorB(string name, int nowScope){
	for (int i = ST_top - 1; i >= 0; i--) {
        if (symbolTable[i].scope == nowScope) {
            if (symbolTable[i].name == name) { 
                push_err_list('b');
                return false;
            }
        }
        if(isglbScope){
        	if (symbolTable[i].scope == curScope) {	
        		if (symbolTable[i].name == name) { 
                	push_err_list('b');
                	return false;
            	}
			}
		}
    }
    if(DeclType == "int"){
    	DeclType = "Int";
	} else if(DeclType == "intarray"){
    	DeclType = "IntArray";
	} else if(DeclType == "constint"){
    	DeclType = "ConstInt";
	} else if(DeclType == "constintarray"){
    	DeclType = "ConstIntArray";
	} else if(DeclType == "char"){
    	DeclType = "Char";
	} else if(DeclType == "chararray"){
    	DeclType = "CharArray";
	} else if(DeclType == "constchar"){
    	DeclType = "ConstChar";
	} else if(DeclType == "constchararray"){
    	DeclType = "ConstCharArray";
	} else if(DeclType == "intfunc"){
    	DeclType = "IntFunc";
	} else if(DeclType == "charfunc"){
    	DeclType = "CharFunc";
	} else if(DeclType == "voidfunc"){
    	DeclType = "VoidFunc";
	} 
    add_s_Table(name, DeclType);
    
	return true;
}

void parseErrorC(string name) {
    bool isFound = false;
    for (int i = 0; i < ST_top; i++) {
        if (symbolTable[i].name == name) {
            isFound = true;
            break;
        }
    }
    if (!isFound) {
        push_err_list('c'); // 정의되지 않은 변수 오류 처리
    }
}

void parseErrorD(string name, int cnt, int funcline) {
    for (int i = 0; i < funcInfo_top; i++){
    	if (funcInfo[i].name == name) {
            if(funcInfo[i].paramcnt != cnt){
            	push_err_list('d', funcline);
            	return; 
			}
        }
	}
}

void parseErrorE() {
	if(!isfuncDef){	//func선언인지 확인 
		if(isdigit(curToken.word[0])){	//매개변수가 숫자인경우 
			if(ErrEtemp.paramtype[paramscnt] != "Int"){
				push_err_list('e', last_func_line);
			} 
			return; 
		}
		for(int i = ST_top - 1; i >= 0 ; i--){
			if(curToken.word == symbolTable[i].name){
				string identtype = symbolTable[i].type;
				
				// const 는 허용 
				size_t pos = identtype.find("Const");
				
				if(pos != string::npos){
					identtype.erase(pos, 5);  // 5글자니까 5만큼 제거
       				 // 앞뒤 공백 제거
        			while(identtype.front() == ' ') {
            		identtype.erase(identtype.begin());
       				}
        			while(identtype.back() == ' ') {
            			identtype.pop_back();
        			}
				}
				
				if(identtype != ErrEtemp.paramtype[paramscnt]){
					push_err_list('e', last_func_line);
					}
				}
			}		
	}
	return;
}

void parseErrorF(int linenum){
	if(!isneedreturn){
		push_err_list('f', linenum);
	}
}

void parseErrorG(){
	if(isneedreturn){
		if(!isstmtreturn){
			push_err_list('g');
		}
	}
}

void parseErrorH(string name){
	for (int i = 0; i < ST_top; i++) {
        if (symbolTable[i].name == name) {
            if(symbolTable[i].type == "ConstChar" || symbolTable[i].type == "ConstInt"
			|| symbolTable[i].type == "ConstCharArray" || symbolTable[i].type == "ConstIntArray"){
				push_err_list('h');
			}
            break;
        }
    }	
}

void parseErrorI(){
	if (curToken.word != ";"){
		push_err_list('i', last_nonT);
	}
	else{
		printcurToken();
		nextToken();
	}
}

bool parseErrorJ(){
	if (curToken.word != ")"){
		push_err_list('j', last_nonT);
		return true;
	}
	else{
		printcurToken();
		nextToken();
	}
	return false;
}

void parseErrorK(){
	if (curToken.word != "]"){
		push_err_list('k', last_nonT);
	}
	else{
		printcurToken();
		nextToken();
	}
}

void parseErrorL(string formatString, int print_param_cnt, int printlinenum) {
    int formatCount = 0;
    for (int i = 0; i < formatString.length(); i++) {
        if (formatString[i] == '%' && (formatString[i + 1] == 'd' || formatString[i + 1] == 'c')) { // '%' 문자가 포맷 문자인 경우
            formatCount++;
        }
    }
    
    if (formatCount != print_param_cnt) {
        push_err_list('l', printlinenum); 
    }
}

void parseErrorM(){
	if(!isBorC){
		push_err_list('m');
	}
}

int main() {
    init();
    Lexical_Analysis();
//    output_Syntax();
//    print_Lexical();
    Syntax_Analysis();
    print_symbol_list();
    cout << "\n\n";
    print_err_list();

//	함수저장 출력 
    cout << "\n\n";
	for(int i = 0; i < funcInfo_top; i++){
		cout << funcInfo[i].type << " " << funcInfo[i].name << " " <<
		funcInfo[i].paramcnt << " ";
		for (const string& type : funcInfo[i].paramtype) {
        cout << type << endl;
    	}
    	cout << "\n";
	} 
    
    inputFile.close();
    outputFile.close();
    outputFile2.close();
    errorFile.close();
    return 0;
}
