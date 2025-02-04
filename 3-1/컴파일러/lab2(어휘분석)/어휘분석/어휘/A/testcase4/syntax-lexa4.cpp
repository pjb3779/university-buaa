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
void parseConstDef();
void parseConstInitVal();
void parseVarDecl();
void parseVarDef();
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
void parseErrorI();
void parseErrorJ();
void parseErrorK();
void print_err_list();

ifstream inputFile;
ofstream outputFile;
ofstream errorFile;
string input_text = "";
string Token = "";

int input_text_len = 0;
int curpos = 0;
int glb_linenum = 1;
int last_nonT;

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
//
//enum FuncType{
//	void,
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
	//sort(error_list, error_list + err_top, compareErrLine);
	
	for(int i = 0; i < err_top ; i++){
		cout << error_list[i].linenum << " " << error_list[i].err_type << endl;
		errorFile << error_list[i].linenum << " " << error_list[i].err_type << endl;
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
    } else if(isdigit(input_text[curpos])){
		while(isdigit(input_text[curpos])){
			Token += input_text[curpos];
			curpos++;
		}
		push_Token_list("INTCON", Token);
//	} else if(isalpha(input_text[curpos])){
//		while(isalpha(input_text[curpos])){
//			Token += input_text[curpos];
//			curpos++;
//		}
//		push_Token_list("STRCON", Token);
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
	        if(input_text[curpos] == 'n' || input_text[curpos] == 't' || 
	           input_text[curpos] == '\\' || input_text[curpos] == '\'' || 
	           input_text[curpos] == '0') {
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
    while(input_text[curpos] == '\n' || input_text[curpos] == '\r'){
    	glb_linenum++;
    	curpos++;
	}
}

void skipBlank() {
    while (curpos < input_text_len &&
	(input_text[curpos] == ' ' || input_text[curpos] == '\n' )) {
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
		if (input_text[curpos] == '\n') {
            glb_linenum++;  // 줄 바꿈이 있을 때마다 줄 번호 업데이트
        }
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
        check_LineNum();     // 줄번호 처리
        skipBlank();       // 공백 문자 건너뛰기
        
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
	printcurToken();//const
	nextToken();
	printcurToken();//Btype
	nextToken();
	
	parseConstDef();
	while(curToken.word == ","){ // ,후 연속선언 처리  
		printcurToken();
		nextToken();
		parseConstDef();
	}	
	
	parseErrorI(); //  ;
	
	printSyntax("ConstDecl");
}

//ConstDef → Ident [ '[' ConstExp ']' ] '=' ConstInitVal // k
void parseConstDef() {
	last_nonT = curToken.line_num;
	
	printcurToken();//Ident
	nextToken();
	
	if(curToken.word == "["){	//배열 선언 처리 
		printcurToken();//[
		nextToken();
		
		parseConstExp();
		
		parseErrorK(); //  ]
	}
	
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
	nextToken();
	
	parseVarDef();
	
	while(curToken.word == ","){
		printcurToken();	//,
		nextToken();
		
		parseVarDef();
	}
	
	parseErrorI(); //  ;
	
	printSyntax("VarDecl");
};

//VarDef → Ident [ '[' ConstExp ']' ] | Ident [ '[' ConstExp ']' ] '=' InitVal // k
void parseVarDef() {
	last_nonT = curToken.line_num;
	
	printcurToken();//Ident
	nextToken();
	
	if(curToken.word == "["){	//배열 선언 처리 
		printcurToken();//[
		nextToken();
		
		parseConstExp();
		
		parseErrorK(); //  ]
	}
	
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
	if(curToken.word == "int" || curToken.word == "char" || curToken.word == "void"){
		parseFuncType();
	}
		
	printcurToken();// Ident
	nextToken();
	
	printcurToken();// (
	last_nonT = curToken.line_num;
	nextToken();
	
	if(curToken.word != ")"){
		parseFuncFParams();
	}
	
	parseErrorJ(); //  )
	
	parseBlock();
	
	printSyntax("FuncDef");
}

//MainFuncDef → 'int' 'main' '(' ')' Block // j
void parseMainFuncDef() {
	printcurToken();// int
	nextToken();
		
	printcurToken();// main
	nextToken();
	
	printcurToken();// (
	last_nonT = curToken.line_num;
	nextToken();
	
	parseErrorJ(); //  )
	
	parseBlock();
	
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
	nextToken();
	printcurToken();//	Ident
	nextToken();
	
	if(curToken.word == "["){
		printcurToken();//	[
		last_nonT = curToken.line_num;
		nextToken();
		
		parseErrorK();
	}
	
	printSyntax("FuncFParam");	
}

//'{' { BlockItem } '}' 
void parseBlock() {
	printcurToken();//	{
	nextToken();
	
	
	while (curToken.word != "}") {
		parseBlockItem();		
	}
	printcurToken();//	}
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
	} 
	//'break' ';' | 'continue' ';' // 
	else if(curToken.word == "break" || curToken.word == "continue"){
		printcurToken();//	break or continue
		last_nonT = curToken.line_num;
		nextToken();
		
		parseErrorI();
	} 
	//'return' [Exp] ';' // i
	else if(curToken.word == "return"){
		printcurToken();//	return
		last_nonT = curToken.line_num;
		nextToken();
		
		if(curToken.word != ";"){//exp 확인 
			parseExp();
		} 
		
		parseErrorI();
	}
	//'printf''('StringConst {','Exp}')'';' // i j
	else if(curToken.word == "printf"){
		printcurToken();//	printf
		nextToken();
		
		printcurToken();//	(
		nextToken();
		
		printcurToken();//	StringConst
		last_nonT = curToken.line_num;
		nextToken();
		
		while(curToken.word == ","){
			printcurToken();//	,
			nextToken();
			
			parseExp();
		}
		
		parseErrorJ();
		
		parseErrorI();
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
void parseForStmt() {
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
	} else if(curToken.Token == "CHRCON"){//Character
		parseCharacter();
	} else{
		cout << "PrimaryExp error" << endl;
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
		printcurToken();  // ident
		last_nonT = curToken.line_num;
    	nextToken();
    
    	printcurToken();  //(
    	nextToken();
    	
    	if(curToken.word != ")"){
    		parseFuncRParams();
		}
		
		parseErrorJ();
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
	parseExp();  // 첫 번째 매개변수 처리
    
    while (curToken.word == ",") {
        printcurToken();  // ,
        nextToken();
        
        parseExp(); 
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
 
void parseErrorI(){
	if (curToken.word != ";"){
		push_err_list('i', last_nonT);
		cout << "sibal" << endl << endl;
	}
	else{
		printcurToken();
		nextToken();
	}
}

void parseErrorJ(){
	if (curToken.word != ")"){
		push_err_list('j', last_nonT);
	}
	else{
		printcurToken();
		nextToken();
	}
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

int main() {
    init();
    Lexical_Analysis();
//    output_Syntax();
//    print_Lexical();
    Syntax_Analysis();
    print_err_list();
    
    
    inputFile.close();
    outputFile.close();
    errorFile.close();
    return 0;
}

