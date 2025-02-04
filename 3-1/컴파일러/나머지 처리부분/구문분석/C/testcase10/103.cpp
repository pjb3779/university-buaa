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
void parseConstDef(int type, int  kind);
void parseConstInitVal();
void parseVarDecl();
void parseVarDef(int type, int kind);
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
int parseFuncRParams();
int parseMulExp(int type);
int parseAddExp(int type);
void parseRelExp();
void parseEqExp();
void parseLAndExp();
void parseLOrExp();
void parseConstExp();
void parseErrorA();
bool parseErrorB(string name, int type, int kind, int isconst, int nowScope);
void parseErrorC(string name);
void parseErrorD(int funcline);
void parseErrorE(int param_num);
void parseErrorF(int linenum);
void parseErrorG();
void parseErrorH(string name);
bool parseErrorI();
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
	int kind = -1;	//0 = var, 1 = func, 2 = param, 3 = arr
	int isconst = 0;	//cosnt = 1, not const = 0
	int scope;
};
SymbolTable symbolTable[100001];
SymbolTable SymbolTemp;
SymbolTable printfSymbol[10001];
int print_top = 0;

int ST_top = 0;		//symbotabletop
int glb_SS= 1;	//symbol_scope	블이 시작할때마다++ 
int curScope = 1;	
int funcStartScope = 0; //함수시작시 스코프
int last_func_line; //errord 처리 

bool isglbscope = true;		//전역변수라면 
bool isequalScope = false;	//함수 시작 스코프와 같으면 

bool isneedreturn = true;		//리턴이 필요한 함수판달 
bool isstmtreturn = false;		// stmt에 리턴이 나왔는지 판단 
bool isBorC = false;			// 브레이크나 컨티뉴인지  
bool isfuncparam = false;
bool isfuncDef = false;		//에러e처리 함수선언인지 아닌지 확인 
bool isifelse = false;		//if else시 리턴 무시 
struct funcparam{
	int type;//0 = int, 1 = char, 2 = void
	string name;
	int paramcnt = 0;
	vector<int> paramtype;
	vector<int> paramkind;
	int expectedcnt = 0;
};
funcparam funcInfo[100001];
int funcInfo_top = 0;
vector<funcparam> funcparse;

void add_s_Table(string name, int type, int kind, int isconst){
	symbolTable[ST_top].name = name;
	symbolTable[ST_top].type = type;
	symbolTable[ST_top].kind = kind;
	symbolTable[ST_top].isconst = isconst;
	if(isglbscope){
		symbolTable[ST_top].scope = 1;
	}else if(isequalScope) {
		symbolTable[ST_top].scope = curScope;
	}else {
		symbolTable[ST_top].scope = glb_SS;
	}
	ST_top++;
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

//void print_err_list(){
//	sort(error_list, error_list + err_top, compareErrLine);
//	
//	for(int i = 0; i < err_top ; i++){
//		cout << error_list[i].linenum << " " << error_list[i].err_type << endl;
//		errorFile << error_list[i].linenum << " " << error_list[i].err_type << endl;
//	}
//}
void print_err_list() {
    // 에러 리스트를 먼저 알파벳순으로 정렬한 다음, 줄 번호를 기준으로 다시 정렬
    sort(error_list, error_list + err_top, [](const error_word& a, const error_word& b) {
       if (a.linenum == b.linenum) {
           return a.err_type > b.err_type; // 같은 줄에서는 에러 유형을 알파벳 순으로 정렬
       }
        return a.linenum < b.linenum; // 줄 번호를 기준으로 정렬
    });

    int last_line = -1; // 마지막 출력된 줄 번호

    for (int i = 0; i < err_top; i++) {
        int line_num = error_list[i].linenum;
        char err_type = error_list[i].err_type;

//        // 이전과 같은 줄에서의 에러는 건너뜀
        if (line_num == last_line) {
            continue;
        }

        // 에러 출력
        cout << line_num << " " << err_type << endl;
        errorFile << line_num << " " << err_type << endl;

        // 마지막 출력된 줄 번호 갱신
        last_line = line_num;
    }
}

bool compareSymbolScope(const SymbolTable &a, const SymbolTable &b){
	return a.scope < b.scope;
}

void print_symbol_list(){
	stable_sort(symbolTable, symbolTable + ST_top, compareSymbolScope);
	
	for(int i = 0; i < ST_top ; i++){
		string temp;
		if(symbolTable[i].isconst == 1){
			if(symbolTable[i].type == 0 && symbolTable[i].kind == 0){
    			temp = "ConstInt";
			} else if(symbolTable[i].type == 0 && symbolTable[i].kind == 3){
	   	 		temp = "ConstIntArray";
			} else if(symbolTable[i].type == 1 && symbolTable[i].kind == 0){
	    		temp = "ConstChar";
			} else if(symbolTable[i].type == 1 && symbolTable[i].kind == 3){
    			temp = "ConstCharArray";
			}
		} else if(symbolTable[i].type == 0 && symbolTable[i].kind == 0){
    		temp = "Int";
		} else if(symbolTable[i].type == 0 && symbolTable[i].kind == 3){
    		temp = "IntArray";
		} else if(symbolTable[i].type == 1 && symbolTable[i].kind == 0){
	   	 	temp = "Char";
		} else if(symbolTable[i].type == 1 && symbolTable[i].kind == 3){
    		temp = "CharArray";
		} else if(symbolTable[i].type == 0 && symbolTable[i].kind == 1){
   		 	temp = "IntFunc";
		} else if(symbolTable[i].type == 1 && symbolTable[i].kind == 1){
    		temp = "CharFunc";
		} else if(symbolTable[i].type == 2 && symbolTable[i].kind == 1){
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

void skipBlank() {
    while (curpos < input_text_len &&
	(input_text[curpos] == ' ' || input_text[curpos] == '\t' || 
	input_text[curpos] == '\r' || input_text[curpos] == '\n')) {
		if(input_text[curpos] == '\n'){
			glb_linenum++;
		}
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
    if (!outputFile2.is_open()) {
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
 //       check_LineNum();     // 줄번호 처리  	
        
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
	cout << Token_list[curpos].Token << " " << Token_list[curpos].word << endl;
	outputFile << Token_list[curpos].Token << " " << Token_list[curpos].word << "\n";
}

void printSyntax(string syntax) {
	cout << "<" << syntax << ">" << endl;
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
	int kind = 0;
	
	printcurToken();//const
	nextToken();
	printcurToken();//Btype
	if(curToken.word == "int"){
		type = INT;
	} else {	//char처리 
		type = CHAR;
	}
	nextToken();
	
	parseConstDef(type, kind);
	while(curToken.word == ","){ // ,후 연속선언 처리  
		printcurToken();
		nextToken();
		parseConstDef(type, kind);
	}	
	
	parseErrorI(); //  ;
	
	printSyntax("ConstDecl");
}

//ConstDef → Ident [ '[' ConstExp ']' ] '=' ConstInitVal // k
void parseConstDef(int type, int kind) {
	last_nonT = curToken.line_num;
	
	printcurToken();//Ident
	string identname = curToken.word;
	nextToken();
	
	if(curToken.word == "["){	//배열 선언 처리 
		printcurToken();//[
		nextToken();
		
		parseConstExp();
		
		parseErrorK(); //  ]
		
		kind = 3;
	}
	
	parseErrorB(identname, type, kind, 1, glb_SS);
	
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
	int kind = 0;
	if(curToken.word == "int"){
		type = INT;
	} else{		//char 처리 
		type = CHAR;
	}
	
	nextToken();
	
	parseVarDef(type, kind);
	
	while(curToken.word == ","){
		printcurToken();	//,
		nextToken();
		
		parseVarDef(type, kind);
	}
	
	parseErrorI(); //  ;
	
	printSyntax("VarDecl");
};

//VarDef → Ident [ '[' ConstExp ']' ] | Ident [ '[' ConstExp ']' ] '=' InitVal // k
void parseVarDef(int type, int kind) {
	last_nonT = curToken.line_num;
	
	printcurToken();//Ident
	string identname = curToken.word;
	nextToken();
	
	if(curToken.word == "["){	//배열 선언 처리 
		printcurToken();//[
		nextToken();
		
		parseConstExp();
		
		parseErrorK(); //  ]
		kind = ARR;
	}
	
	parseErrorB(identname, type, kind, 0, glb_SS);
	
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
//	isfuncDef = true; 
	isstmtreturn = false;
	isneedreturn = true;
	
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
	string identname = curToken.word;
	nextToken();
	
	funcInfo[funcInfo_top].type = type;
	funcInfo[funcInfo_top].name = identname;
	
	bool valid_func = false;
	if(parseErrorB(identname, type, kind, 0, 1)){
		valid_func = true;
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
	
	if(valid_func){
		funcInfo_top++;	//함수정보 ++	
	}
	
//	isfuncDef = false; 
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
	string identname = curToken.word;
	nextToken();
	
	int kind = VAR;
	if(curToken.word == "["){
		printcurToken();//	[
		last_nonT = curToken.line_num;
		nextToken();
		
		parseErrorK();
		
		kind = ARR;
	}	
	parseErrorB(identname, type, kind, 0, glb_SS);
	funcInfo[funcInfo_top].paramtype.push_back(type);	//int = 0. char = 1
	funcInfo[funcInfo_top].paramkind.push_back(kind);   //var = 0, arr = 3 
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
		isifelse = true;
		 
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
		isifelse = false;
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
		printcurToken();//	break or continue
		last_nonT = curToken.line_num;
		
		parseErrorM();
		
		nextToken();
		
		parseErrorI();
	} 
	//'return' [Exp] ';' // i
	else if(curToken.word == "return"){
		printcurToken();//	return
		last_nonT = curToken.line_num;
		int returnline = curToken.line_num;	
		
		if(!isifelse){
			isstmtreturn = true;
		}
		
		nextToken();
		
		if(isdigit(curToken.word[0]) || curToken.Token == "CHRCON" || curToken.Token == "IDENFR"){
			parseErrorF(returnline);
			parseExp();
			parseErrorI();
		} else {
			parseErrorI();
		}	
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
		if(expcnt != 0){
			parseErrorL(temp, expcnt, printlinenum);
		}
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
	parseErrorH(curToken.word);	//cosnt값 변환 에러 처리 
	
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
        
        type = parseExp(); 
        
        parseErrorJ();
    } else if(isdigit(curToken.word[0])){//Number
    	type = INT;
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
//		isfuncparam = true;    //함수 매개변수 받기 
		
		printcurToken();  // ident
		last_nonT = curToken.line_num;
		last_func_line = curToken.line_num;
    	string identname = curToken.word;

    	 bool isFound = false;
        for (int i = 0; i <= funcInfo_top; i++) {  // 함수 정의 목록에서 매칭 검사
            if (funcInfo[i].name == identname) {
                funcparse.push_back(funcInfo[i]);
                isFound = true;
                break;
            }
        }

        if (!isFound) {
            // 정의되지 않은 함수일 경우
            push_err_list('c', curToken.line_num);  // 'c' 에러 추가
            nextToken();  // '('
            while (curToken.word != ")" && curToken.word != ";") {  // ')' 또는 ';'까지 건너뜀
                nextToken();
                if (curpos >= Tlist_top) break;  // 파일 끝에 도달했으면 탈출
            }
            if (curToken.word == ")") nextToken();  // ')' 처리
            printSyntax("UnaryExp");
            return type;  // 함수 호출 처리 종료
        }
		
		type = funcparse.back().type;
		
		printfSymbol[print_top].name = funcparse.back().name + " " 
		+ to_string(funcparse.back().paramcnt) + " " + to_string(funcparse.back().expectedcnt);	//print only
		print_top++;
		
    	nextToken();
    	
    	printcurToken();  //(
    	nextToken();
    	
    	if(curToken.word != ")"){
    		if(curToken.word != ";"){// 그냥 )parse err 
    			parseFuncRParams();
			}
		}
		
		if(!parseErrorJ()){		//에러j가 우선순위를 가짐 
			if(funcparse.back().expectedcnt != funcparse.back().paramcnt){
				parseErrorD(last_func_line);
			}
		}
		
		funcparse.pop_back();
//		isfuncparam = false;	//함수 매개변수 끄기 
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
int parseFuncRParams() {
	int paramscnt = 0;
	
	parseErrorE(paramscnt + 1);	 //1 일때 cnt = 0 
	int type = parseExp();  // 첫 번째 매개변수 처리
	
//	if(isfuncparam){	//errorD처리중 
		paramscnt++;
		funcparse.back().expectedcnt++;
		printfSymbol[print_top].name = funcparse.back().name + "end " 
		+ to_string(funcparse.back().paramcnt) + " " + to_string(funcparse.back().expectedcnt);	//print only
		print_top++;
//	}
    
    while (curToken.word == ",") {
        printcurToken();  // ,
        nextToken();
        
        parseErrorE(paramscnt + 1);	    
        type = parseExp();  

//        if(isfuncparam){	//errorD처리중 
			paramscnt++;
			funcparse.back().expectedcnt++;
			printfSymbol[print_top].name = funcparse.back().name + "end " 
		+ to_string(funcparse.back().paramcnt) + " " + to_string(funcparse.back().expectedcnt);	//print only
		print_top++;
//		}
    }
    
    printSyntax("FuncRParams");
    return paramscnt;
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
 
bool parseErrorB(string name, int type, int kind, int isconst, int nowScope){
	for (int i = ST_top - 1; i >= 0; i--) {
        if (symbolTable[i].scope == nowScope && symbolTable[i].name == name) {
            push_err_list('b');
            return false;
        }
        if(isequalScope){
        	if (symbolTable[i].scope == curScope) {	
        		if (symbolTable[i].name == name) { 
                	push_err_list('b');
                	return false;
            	}
			}
		} 
    }

   add_s_Table(name, type, kind, isconst);
    
	return true;
}

void parseErrorC(string name) {
    bool isFound = false;
    for (int i = 0; i <= ST_top; i++) {
        if (symbolTable[i].name == name) {
            isFound = true;
            break;
        }
    }
    if (!isFound) {
        push_err_list('c'); // 정의되지 않은 변수 오류 처리
    }
}

void parseErrorD(int funcline) {
    push_err_list('d', funcline);
    return; 
}

void findAndSetSymbol(const string& tokenName) {
    for (int i = ST_top - 1; i >= 0; i--) { // 심볼 테이블을 역순으로 검색
        if (tokenName == symbolTable[i].name) {
            // 찾은 심볼의 정보를 SymbolTemp에 저장
            SymbolTemp = symbolTable[i];

            // 심볼 정보를 printfSymbol 배열에 저장하여 나중에 출력할 수 있도록 합니다.
            printfSymbol[print_top].name = SymbolTemp.name;
            printfSymbol[print_top].kind = SymbolTemp.kind;
            printfSymbol[print_top].type = SymbolTemp.type;
            print_top++; // print_top을 증가시켜 다음 출력 정보 위치를 업데이트

            break; // 심볼을 찾으면 루프 종료
        }
    }
}

void parseErrorE(int param_num) {
    funcparam& curFunc = funcparse.back();

    if (curFunc.paramcnt == 0) { // 매개변수 없을 경우 체크 X
        return;
    }

    // 변수일 때
    if (curToken.Token == "IDENFR") {
        findAndSetSymbol(curToken.word);
        
        // 기대값이 배열일 때
        if (curFunc.paramkind[param_num - 1] == 3) {
            nextToken(); // 배열을 단일로 받는 경우 처리
            if (curToken.word == "[") {
                nextToken();
                if (isdigit(curToken.word[0])) {
                    preToken();
                    preToken();
                    push_err_list('e', last_func_line);
                    return; // 배열이 아닌 경우 함수 종료
                }
                preToken();
            }
            preToken();

            // 매개변수 타입과 심볼 테이블 타입 비교
            if (SymbolTemp.type != curFunc.paramtype[param_num - 1] ||
                SymbolTemp.kind != curFunc.paramkind[param_num - 1]) {
                push_err_list('e', last_func_line);
            }
        } else if (curFunc.paramkind[param_num - 1] == 0) { // 일반 변수
            nextToken(); // 배열을 단일로 받는 경우 처리
            if (curToken.word == "[") {
                nextToken();
                if (isdigit(curToken.word[0])) { // 배열로 전달된 경우
                    preToken();
                    preToken();
                    return; // 일반 변수 기대 시 함수 종료
                }
                preToken();
            }
            preToken();

            // 일반 변수인데 배열일 때 오류 추가
            if (SymbolTemp.kind == 3) {
                push_err_list('e', last_func_line);
            }
        }
    } else if (isdigit(curToken.word[0])) { // 숫자이고 배열이 아니면 인트
        printfSymbol[print_top].name = curToken.word; // print only
        printfSymbol[print_top].kind = 0;
        printfSymbol[print_top].type = 0;
        print_top++;

        // 배열 기대 시 오류 추가
        if (curFunc.paramkind[param_num - 1] == 3) {
            push_err_list('e', last_func_line);
        }
    } else if (isalpha(curToken.word[0]) || curToken.word[0] == '\'') { 
        printfSymbol[print_top].name = curToken.word; // print only
        printfSymbol[print_top].kind = 0;
        printfSymbol[print_top].type = 1;
        print_top++;

        // 배열 기대 시 오류 추가
        if (curFunc.paramkind[param_num - 1] == 3) {
            push_err_list('e', last_func_line);
        }
    }

    return;
}


//void parseErrorE(int param_num) {
//    funcparam& curFunc = funcparse.back();
//    
//    if (curFunc.paramcnt == 0) { // 매개변수 없을 경우 체크 X
//        return;
//    }
//    
//    int nextTokencnt = 0;
//    bool first = true;
//    
//    while (curToken.word != ")" && curToken.word != ",") {
//        if (first) {
//            first = false;
//        } else {
//            nextToken();
//            nextTokencnt++;
//        }
//        
//        // 변수일 때
//        if (curToken.Token == "IDENFR") {
//            findAndSetSymbol(curToken.word);
//            if (curFunc.paramkind[param_num - 1] == 3) { // 기대값이 배열일 때
//                nextToken(); // 배열을 단일로 받는 경우 처리
//                if (curToken.word == "[") {
//                    nextToken();
//                    if (isdigit(curToken.word[0])) {
//                        preToken();
//                        preToken();
//                        push_err_list('e', last_func_line);
//                        break;
//                    }
//                    preToken();
//                }
//                preToken();
//                
//                if (SymbolTemp.type != curFunc.paramtype[param_num - 1] || 
//                    SymbolTemp.kind != curFunc.paramkind[param_num - 1]) {
//                    push_err_list('e', last_func_line);
//                }
//            } else if (curFunc.paramkind[param_num - 1] == 0) { // 일반 변수
//                bool isfind = false; 
//                nextToken(); // 배열을 단일로 받는 경우 처리
//                if (curToken.word == "[") {
//                    nextToken();
//                    if (isdigit(curToken.word[0])) {
//                        isfind = true;
//                    }
//                    preToken();
//                }
//                preToken();
//                
//                if (!isfind && SymbolTemp.kind == 3) {
//                    push_err_list('e', last_func_line);
//                }
//            }
//        } else if (isdigit(curToken.word[0])) { // 숫자이고 배열이 아니면 인트
//            printfSymbol[print_top].name = curToken.word; // print only
//            printfSymbol[print_top].kind = 0;
//            printfSymbol[print_top].type = 0;
//            print_top++;
//
//            if (curFunc.paramkind[param_num - 1] == 3) { // 배열 기대 시 에러 추가
//                push_err_list('e', last_func_line);
//            }
//        } else if (isalpha(curToken.word[0]) || curToken.word[0] == '\'') { 
//            printfSymbol[print_top].name = curToken.word; // print only
//            printfSymbol[print_top].kind = 0;
//            printfSymbol[print_top].type = 1;
//            print_top++;
//
//            if (curFunc.paramkind[param_num - 1] == 3) { // 배열 기대 시 에러 추가
//                push_err_list('e', last_func_line);
//            }
//        } 
//    }
//
//    // 반복문 종료 후, nextToken 호출 횟수만큼 preToken 호출
//    while (nextTokencnt > 0) {
//        preToken();
//        nextTokencnt--;
//    }
//
//    return;
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
	for (int i = ST_top; i >= 0; i--) {
        if (symbolTable[i].name == name && (symbolTable[i].scope == curScope || 
		symbolTable[i].scope == funcStartScope || symbolTable[i].scope == 1)) {
            if(symbolTable[i].isconst == 1){
				push_err_list('h');
			}
            break;
        }
    }	
}

bool parseErrorI(){
	if (curToken.word != ";"){
		push_err_list('i', last_nonT);
		return true;;
	}
	else{
		printcurToken();
		nextToken();
	}
	return false;
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
 //   output_Syntax();
//    print_Lexical();
    Syntax_Analysis();
//    print_symbol_list();
//    cout << "\n\n" << endl;
    print_err_list();
    
//    //	함수저장 출력 
//    cout << "\n\n";
//	for(int i = 0; i < funcInfo_top; i++){
//		string temp1;
//		if(funcInfo[i].type == 0){
//			temp1 = "Int";
//		}else if(funcInfo[i].type == 1){
//			temp1 = "Char";
//		}else {
//			temp1 = "Void";
//		}
//		cout << temp1 << " " << funcInfo[i].name << " " <<
//		funcInfo[i].paramcnt << " ";
//		for (int j = 0; j < funcInfo[i].paramcnt; ++j) {
//    	int type1 = funcInfo[i].paramtype[j];
//    	int type2 = funcInfo[i].paramkind[j];
//		string temp2;
//		if(type1 == 0){
//			temp2 = "Int";
//		}else if(type1 == 1){
//			temp2 = "Char";
//		}else if(type1 == 2){
//			temp2 = "Void";
//		}
//		if(type2 == 3){
//			temp2 += "Arr";
//		}
//        cout << temp2 << endl;
//    	}
//    	cout << "\n";
//	} 
//	
//	for(int i = 0; i < print_top; i++){
//		string temp;
//		if(printfSymbol[i].type == 0){
//			temp = "Int";
//		}else if(printfSymbol[i].type == 1){
//			temp = "Char";
//		}else if(printfSymbol[i].type == 2){
//			temp = "Void";
//		}
//		if(printfSymbol[i].kind == 3){
//			temp += "Arr";
//		}else if(printfSymbol[i].kind == 1){
//			temp += "Func";
//		}
//		cout << printfSymbol[i].name << " " 
//		<< temp << endl;
//	}
	
    inputFile.close();
    outputFile.close();
    outputFile2.close();
    errorFile.close();
    return 0;
}

