#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>
#include <cstdlib>
#include <cctype>
#include <fstream>
#include <sstream>
#include <algorithm>

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
void parseConstDef(int type);
void parseConstInitVal();
void parseVarDecl();
void parseVarDef(int type);
void parseInitVal();
void parseFuncDef();
void parseMainFuncDef();
void parseFuncType();
void parseFuncType();
void parseFuncFParams();
void parseFuncFParam();
void parseBlock();
void parseBlockItem();
void parseStmt();
void parseForStmt();
int parseExp();
void parseCond();
int parseLVal(int type);
int parsePrimaryExp(int type);
void parseNumber();
void parseCharacter();
int parseUnaryExp(int type);
void parseUnaryOp();
void parseFuncRParams();
int parseMulExp(int type);
int parseAddExp(int type);
void parseRelExp();
void parseEqExp();
void parseLAndExp();
void parseLOrExp();
void parseConstExp();
void parseErrorA();
bool parseErrorB(string name, int type, int kind, int curscope);
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

enum KindEnum {
	VAR,
	FUNC,
	PARAM,
	ARR
};

enum TypeEnum {
	INT,
	CHAR,
	CONSTINT,
	CONSTCHAR,
	VOID
};

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
	int type = -1;	//0 = int, 1 = char, 2 = void
	int kind = -1;	//0 = const, 1 = var, 2 = func, 3 = param
	int scope;
};
SymbolTable symbolTable[100001];

int glb_SS= 1;	//symbol_scope	블이 시작할때마다++ 
int ST_top = 0;		//symbotabletop
int curScope = 1;	
int funcStartScope = 0; //함수시작시 스코프
bool isglbscope = true;		//전역변수라면 
bool isequalScope = false;	//함수 시작 스코프와 같으면 

bool isneedreturn = true;		//리턴이 필요한 함수판달 
bool isstmtreturn = false;		// stmt에 리턴이 나왔는지 판단 
bool isBorC = false;			// 브레이크나 컨티뉴인지  
bool isfuncparam = false;		//함수 매개변수 선언중인가 
bool isfuncDef = false;		//에러e처리 함수선언인지 아닌지 확인 
bool isifelse = false;		//errG if else문에서 return은 무시

void add_s_Table(string name, int type, int kind){
	symbolTable[ST_top].name = name;
	symbolTable[ST_top].type = type;
	symbolTable[ST_top].kind = kind;
	if(isglbscope){
		symbolTable[ST_top].scope = 1;
	}else if(isequalScope) {
		symbolTable[ST_top].scope = curScope;
	}else {
		symbolTable[ST_top].scope = glb_SS;
	}
	ST_top++;
}

struct funcparam{
	string name;
	int type;
	int kind;
	int paramcnt = 0;
	int paramcurpos = 0; 	//현재 매개변수 위치
	vector<int> paramtype;
};
funcparam funcInfo[10001];
vector<funcparam> ErrEtemp;
int funcInfo_top = 0;
int ErrEtemp_top = 0;

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

bool compareSymbolScope(const SymbolTable &a, const SymbolTable &b){
	return a.scope < b.scope;
}

void print_symbol_list(){
	stable_sort(symbolTable, symbolTable + ST_top, compareSymbolScope);
	
	for(int i = 0; i < ST_top ; i++){
		string temp;
		if(symbolTable[i].type == 0 && symbolTable[i].kind == 0){
    		temp = "Int";
		} else if(symbolTable[i].type == 0 && symbolTable[i].kind == 3){
    		temp = "IntArray";
		} else if(symbolTable[i].type == 2 && symbolTable[i].kind == 0){
    		temp = "ConstInt";
		} else if(symbolTable[i].type == 2 && symbolTable[i].kind == 3){
	   	 	temp = "ConstIntArray";
		} else if(symbolTable[i].type == 1 && symbolTable[i].kind == 0){
	   	 	temp = "Char";
		} else if(symbolTable[i].type == 1 && symbolTable[i].kind == 3){
    		temp = "CharArray";
		} else if(symbolTable[i].type == 3 && symbolTable[i].kind == 0){
	    	temp = "ConstChar";
		} else if(symbolTable[i].type == 3 && symbolTable[i].kind == 3){
    		temp = "ConstCharArray";
		} else if(symbolTable[i].type == 0 && symbolTable[i].kind == 1){
   		 	temp = "IntFunc";
		} else if(symbolTable[i].type == 1 && symbolTable[i].kind == 1){
    		temp = "CharFunc";
		} else if(symbolTable[i].type == 4 && symbolTable[i].kind == 1){
    		temp = "VoidFunc";
		} 
		cout << symbolTable[i].scope << " " 
		<< symbolTable[i].name << " " 
		<< temp << endl;
		
		outputFile2 << symbolTable[i].scope << " " 
		<< symbolTable[i].name << " " 
		<< temp << endl;
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
    while(input_text[curpos] != '"') {
        // 이스케이프된 큰따옴표 처리 추가
        if(input_text[curpos] == '\\' && input_text[curpos + 1] == '"') {
            Token += "\\\"";
            curpos += 2;
        } else {
            Token += input_text[curpos];
            curpos++;
        }
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
	           input_text[curpos] == '\\' || input_text[curpos] == '\'' || 
	           input_text[curpos] == '0' || input_text[curpos] == 'f' || input_text[curpos] == '"') {
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
	}
	check_LineNum();
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
    	input_text += input_buffer + "\n";
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
//	cout << "<" << syntax << ">" << endl;
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
	int type = -1;
	
	printcurToken();//const
	nextToken();
	printcurToken();//Btype
	if(curToken.word == "int"){
		type = CONSTINT;
	} else {	//char처리 
		type = CONSTCHAR;
	}
	nextToken();
	
	parseConstDef(type);
	while(curToken.word == ","){ // ,후 연속선언 처리  
		printcurToken();
		nextToken();
		parseConstDef(type);
	}	
	
	parseErrorI(); //  ;
	
	printSyntax("ConstDecl");
}

//ConstDef → Ident [ '[' ConstExp ']' ] '=' ConstInitVal // k
void parseConstDef(int type) {
	last_nonT = curToken.line_num;
	
	printcurToken();//Ident
	string Identname = curToken.word;
	nextToken();
	
	int kind = VAR;
	
	if(curToken.word == "["){	//배열 선언 처리 
		printcurToken();//[
		nextToken();
		
		parseConstExp();
		
		parseErrorK(); //  ]
		
		kind = ARR;
	}
	parseErrorB(Identname, type, kind, glb_SS);
	
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
	
	int type = -1;
	if(curToken.word == "int"){
		type = INT;
	} else{		//char 처리 
		type = CHAR;
	}
	
	nextToken();
	
	parseVarDef(type);
	
	while(curToken.word == ","){
		printcurToken();	//,
		nextToken();
		
		parseVarDef(type);
	}
	
	parseErrorI(); //  ;
	
	printSyntax("VarDecl");
};

//VarDef → Ident [ '[' ConstExp ']' ] | Ident [ '[' ConstExp ']' ] '=' InitVal // k
void parseVarDef(int type) {
	last_nonT = curToken.line_num;
	
	printcurToken();//Ident
	string Identname = curToken.word;
	nextToken();
	
	int kind = VAR;
	
	if(curToken.word == "["){	//배열 선언 처리 
		printcurToken();//[
		nextToken();
		
		parseConstExp();
		
		parseErrorK(); //  ]
		kind = ARR;
	}
	
	parseErrorB(Identname, type, kind, glb_SS);
	
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
	int type = - 1;
	int kind = FUNC;
	
	isfuncDef = true;
	isneedreturn = true;
	isstmtreturn = false;    //void따로 처리 
	
	if(curToken.word == "int" || curToken.word == "char" || curToken.word == "void"){
		if(curToken.word == "int"){
			type = INT;
		} else if(curToken.word == "char"){
			type = CHAR;
		} else{
			type = VOID;
			isneedreturn = false;
		}
		parseFuncType();
	}
		
	printcurToken();// Ident
	string Identname = curToken.word;
	nextToken();
	
	if(parseErrorB(Identname, type, kind, glb_SS)){		//유효한 함수면 추가  
		funcInfo[funcInfo_top].name = Identname;
		funcInfo[funcInfo_top].type = type;
		funcInfo[funcInfo_top].kind = kind;
	}
	
	printcurToken();// (
	last_nonT = curToken.line_num;
	glb_SS++;	//함수 지역변수 처리를 위해+ 
	curScope = glb_SS; 
	isglbscope = false;
	nextToken();
	
	if(curToken.word != ")" && curToken.word != "{"){
		parseFuncFParams();
	}
	
	parseErrorJ(); //  )
	
	glb_SS--;	//매개변수 처리 복구 
	curScope = glb_SS; 
	funcStartScope = glb_SS + 1;
	parseBlock();
	
	curScope = glb_SS + 1; 
	isglbscope = true;
	
	funcInfo_top++;
	
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
	int type = -1;
	if(curToken.word == "int"){
		type = INT;
	} else{
		type = CHAR;
	}
	nextToken();
	printcurToken();//	Ident
	string Identname = curToken.word;
	nextToken();
	
	int kind = VAR;
	if(curToken.word == "["){
		printcurToken();//	[
		last_nonT = curToken.line_num;
		nextToken();
		
		parseErrorK();
		
		kind = ARR;
	}
	parseErrorB(Identname, type, kind, glb_SS);
	funcInfo[funcInfo_top].paramtype.push_back(type);	//int = 0. char = 1
	funcInfo[funcInfo_top].paramcnt++;
	
	printSyntax("FuncFParam");	
}

//'{' { BlockItem } '}' 
void parseBlock() {
	printcurToken();//	{
	glb_SS++;	//블럭이면 증가 
	curScope++;
	if(funcStartScope == curScope){
		isequalScope = true;
	} else{
		isequalScope = false;
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
		isequalScope = true;
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
		
		isifelse = true;
		
		printcurToken();//	(
		nextToken();
		
		parseCond();
		
		parseErrorJ();
		
		parseStmt();
		
		if(curToken.word == "else") {
			printcurToken();//	else
			nextToken();	
			
			isifelse = true;
			
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
		int returnline = curToken.line_num;    //last_nonT로 대체가능 
		
		if(isifelse){
			isifelse = false;	
		} else{
			isstmtreturn = true;
		}
		
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
		
		parseErrorJ();
		parseErrorI();
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
			parseErrorH(curToken.word);
			parseLVal(-1);
			
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
	parseErrorH(curToken.word);
	parseLVal(-1);
	
	printcurToken();//	=
	nextToken();
	
	parseExp();
	
	printSyntax("ForStmt");
}

//Exp → AddExp 
int parseExp() {
	int type = INT;
	last_nonT = curToken.line_num;
	
	type = parseAddExp(type);
	
	printSyntax("Exp");
	return type;
}

//Cond → LOrExp 
void parseCond() {
	last_nonT = curToken.line_num;
	
	parseLOrExp();
	
	printSyntax("Cond");
}

//LVal → Ident ['[' Exp ']'] // k
int parseLVal(int type) {
	printcurToken();//	Ident
	parseErrorC(curToken.word);
	nextToken();
	
	if(curToken.word == "["){
		printcurToken();//	[
		last_nonT = curToken.line_num;
		nextToken();
		
		type = parseExp();
		
		parseErrorK();
	}
	
	printSyntax("LVal");
	return type;
}

//PrimaryExp → '(' Exp ')' | LVal | Number | Character// j
int parsePrimaryExp(int type) {
	 if (curToken.word == "(") {//exp
        printcurToken();  // (
        nextToken();
        
        parseExp(); 
        
        parseErrorJ();
    } else if(isdigit(curToken.word[0])){//Number
    	parseNumber();
	} else if(curToken.Token == "IDENFR"){
		type = parseLVal(type);
	} else {//Character
		type = CHAR;
		parseCharacter();
	} 
	 printSyntax("PrimaryExp");
	 return type;
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
int parseUnaryExp(int type) {
	nextToken();
	string temp1 = curToken.word;	//ident일때(받기 
	preToken();
	 
	if(curToken.word == "+" || curToken.word == "-" || curToken.word == "!"){
		parseUnaryOp();
		
		type = parseUnaryExp(type);
	} else if(curToken.Token == "IDENFR" && temp1 == "("){
		isfuncparam = true;	//함수의 매개변수인가  
		
		printcurToken();  // ident
		parseErrorC(curToken.word);
		last_nonT = curToken.line_num;
    	int last_func_line = curToken.line_num; //함수 호출 라인 
    	string Identname = curToken.word;
    	
		for (int i = 0; i < funcInfo_top; i++){	//현재 함수의 매개변수 
    		if (funcInfo[i].name == curToken.word) {
            	ErrEtemp.push_back(funcInfo[i]);
            	ErrEtemp_top++;
				break;
       		}
		}
		
		nextToken();
    
    	printcurToken();  //(
    	nextToken();
    	
    	if(curToken.word != ")"){
    		parseFuncRParams();
		}
		
		if(!parseErrorJ() && ErrEtemp_top > 0){		//에러j가 우선순위를 가짐 
			parseErrorD(Identname, ErrEtemp[ErrEtemp_top - 1].paramcurpos , last_func_line);
		}
		
		if(ErrEtemp_top > 0){
			ErrEtemp.pop_back();
			ErrEtemp_top--;		//매개변수로 선언된 함수처리 
		}
		isfuncparam = false;
	} else{
		type = parsePrimaryExp(type);
	}
	printSyntax("UnaryExp");
	return type;
}

//UnaryOp → '+' | '-' | '!' !는 Cond에만 출현 
void parseUnaryOp() {
	printcurToken();// + || - || !
    nextToken();
    
    printSyntax("UnaryOp"); 
}

//FuncRParams → Exp { ',' Exp } 
void parseFuncRParams() {
	
	//errore in
	parseExp();  // 첫 번째 매개변수 처리
    if(isfuncparam){	//errorD처리중 
    	if(ErrEtemp_top > 0){
    		ErrEtemp[ErrEtemp_top - 1].paramcurpos++;	
		}
	}
	
    while (curToken.word == ",") {
        printcurToken();  // ,
        nextToken();
        
        //errore in
        parseExp(); 
        
		if(isfuncparam){	//errorD처리중 
    		if(ErrEtemp_top > 0){
    			ErrEtemp[ErrEtemp_top - 1].paramcurpos++;	
			}
		}
    }
    
    printSyntax("FuncRParams");
}

//MulExp → UnaryExp | MulExp ('*' | '/' | '%') UnaryExp 
int parseMulExp(int type) {
	parseUnaryExp(type); //UnaryExp+ 의 형태 

	printSyntax("MulExp");

	if(curToken.word == "*" || curToken.word == "/" || curToken.word == "%"){
		printcurToken();  // '*' | '/' | '%'
        nextToken();
		
		type = parseMulExp(type);
	}
	return type;
}

//AddExp → MulExp | AddExp ('+' | '-') MulExp 
int parseAddExp(int type) {
	parseMulExp(type);
	
	printSyntax("AddExp");
	
	if(curToken.word == "+" || curToken.word == "-"){
		printcurToken();  // '+' | '-'
        nextToken();
        
		type = parseAddExp(type);
	}
	return type;
}

//RelExp → AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp 
void parseRelExp() {
	parseAddExp(-1);
	
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
	
	parseAddExp(-1);
	
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

bool parseErrorB(string name, int type, int kind, int curscope){
	for (int i = ST_top - 1; i >= 0; i--) {
        if (symbolTable[i].scope == curscope) {
            if (symbolTable[i].name == name) { 
                push_err_list('b');
                return false;
            }
        }
        if(isequalScope){
        	if (symbolTable[i].scope == curscope) {	
        		if (symbolTable[i].name == name) { 
                	push_err_list('b');
                	return false;
            	}
			}
		}
    }
    add_s_Table(name, type,kind);
    
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

//void parseErrorE(){//숫자 캐릭터 식별자 
//	if(!isfuncDef){  // 함수 선언인지 확인
//		if(ErrEtemp_top - 1 < ErrEtemp.size() && ErrEtemp[ErrEtemp_top - 1].paramcurpos < ErrEtemp[ErrEtemp_top - 1].paramtype.size()) {//배열초과 방지
//			//변수 값 받기 
//			string sindex;
//			int index = 0;
//			int isEnd = 0;
//			while(true){
//				if(curToken.word == "," || curToken.word == ")" || curToken.word == ";"){
//					isEnd = 1;
//					break;
//				}else if(curToken.word == "+" || curToken.word == "-" ||
//				   curToken.word == "*" || curToken.word == "/" || 
//				   curToken.word == "%" || curToken.word == "/'" ){
//				   	//'처리?
//					    index++;
//				}else if(curToken.Token == "INTTK" || curToken.Token == "CHARTK" || curToken.Token == "IDENFR"){
//					if(curToken.Token == "IDENFR"){
//						for(int i = ST_top - 1; i >= 0 ; i--){
//							if(curToken.word == symbolTable[i].name){
//								int identtype = symbolTable[i].type;
//							}
//						}
//				}
//				
//				
//				
//				if(curpos >= Tlist_top){
//				break;
//				}
//			} 
//			while(index)
//			{
//				preToken();
//				index--;
//			}
//		} else {
//			cout << "ErrEtemp arr err" << endl;  // 범위 초과 시 오류 처리
//		}
//	}
//	return;
//}

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
            if(symbolTable[i].type == 2 || symbolTable[i].type == 3){
            	push_err_list('h');
			}
            break;
        }
    }	
}

void parseErrorI(){
	if (curToken.word != ";"){
		push_err_list('i', last_nonT);
		cout << "sibal" << endl;
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
//   cout << "\n\n";
//		for(int i = 0; i < funcInfo_top; i++){
//		cout << funcInfo[i].type << " " << funcInfo[i].name << " " <<
//		funcInfo[i].paramcnt << " ";
//		for (const string& type : funcInfo[i].paramtype) {
//        cout << type << endl;
//    	}
//    	cout << "\n";
//	} 
    
    inputFile.close();
    outputFile.close();
    outputFile2.close();
    errorFile.close();
    return 0;
}

