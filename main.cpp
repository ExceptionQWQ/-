#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <map>
#include <unistd.h>
using namespace std;

const char* name_char_list = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const char* int_char_list = "0123456789";
const char* float_char_list = "0123456789.";
const char* operator_char_list = "()=+-*/%><!;";

enum TOKEN_TYPE{
    TOKEN_TYPE_NAME = 1,
    TOKEN_TYPE_INT = 2,
    TOKEN_TYPE_FLOAT = 3,
    TOKEN_TYPE_OPERATOR = 4,
};
enum OPERATOR_TYPE{
    OPERATOR_TYPE_LP  = 1, // (
    OPERATOR_TYPE_RP  = 2, // )
    OPERATOR_TYPE_ES  = 3, // =
    OPERATOR_TYPE_ADD = 4, // +
    OPERATOR_TYPE_SUB = 5, // -
    OPERATOR_TYPE_MUL = 6, // *
    OPERATOR_TYPE_DIV = 7, // /
    OPERATOR_TYPE_MOD = 8, // %
    OPERATOR_TYPE_END = 9, // ;
    OPERATOR_TYPE_GT  = 10,// >
    OPERATOR_TYPE_GE  = 11,// >=
    OPERATOR_TYPE_LT  = 12,// <
    OPERATOR_TYPE_LE  = 13,// <=
    OPERATOR_TYPE_EQ  = 14,// ==
    OPERATOR_TYPE_NE  = 15,// !=
};
enum VAR_TYPE{
    VAR_TYPE_NUM = 1,
    VAR_TYPE_MAT = 2,
};
struct Token{
    TOKEN_TYPE token_type;
    OPERATOR_TYPE operator_type;
    char data[128];
};
struct Matrix{
    int row;
    int col;
    double **data;
};
struct Var{
    VAR_TYPE var_type;
    double num;
    Matrix matrix;
};
int op_pri_list[]={0,0,0,0,1,1,2,2,0,0,0,0,0,0,0,0};
map<string,Var> global_var_map;

class Lana{
private:
    char origin_data[1024];
    //清除表达式中多余的空格
    void clear(char* input,char* output){
        int len = strlen(input);
        int offset = 0;
        for(int i=0;i<len;i++){
            if(input[i]!=' ' && input[i]!='\n'){
                output[offset++] = input[i];
            }
        }
    }
    //检查字符
    int check_char(const char* char_list,char ch){
        int len = strlen(char_list);
        for(int i=0;i<len;i++){
            if(char_list[i] == ch){
                return 1;
            }
        }
        return 0;
    }

public:
    int get_tokens(vector<Token>& v){
        char buff[1024];
        memset(buff,0,1024);
        clear(origin_data,buff);

        int len = strlen(buff);
        for(int i=0;i<len;i++){
            //先检查是否是运算符
            if(check_char(operator_char_list,buff[i])){
                char op[8];
                memset(op,0,8);
                op[0] = buff[i];
                //检查下一个是否是运算符
                if(check_char(operator_char_list,buff[i+1])){
                    op[1] = buff[i+1];
                    if(strcmp(op,"==")==0){
                        Token t;
                        t.token_type = TOKEN_TYPE_OPERATOR;
                        t.operator_type = OPERATOR_TYPE_EQ;
                        memcpy(t.data,op,8);
                        v.push_back(t);
                        i+=1;
                        continue;
                    }else if(strcmp(op,">=")==0){
                        Token t;
                        t.token_type = TOKEN_TYPE_OPERATOR;
                        t.operator_type = OPERATOR_TYPE_GE;
                        memcpy(t.data,op,8);
                        v.push_back(t);
                        i+=1;
                        continue;
                    }else if(strcmp(op,"<=")==0){
                        Token t;
                        t.token_type = TOKEN_TYPE_OPERATOR;
                        t.operator_type = OPERATOR_TYPE_LE;
                        memcpy(t.data,op,8);
                        v.push_back(t);
                        i+=1;
                        continue;
                    }else if(strcmp(op,"!=")==0){
                        Token t;
                        t.token_type = TOKEN_TYPE_OPERATOR;
                        t.operator_type = OPERATOR_TYPE_NE;
                        memcpy(t.data,op,8);
                        v.push_back(t);
                        i+=1;
                        continue;
                    }
                    //其他情况按单个运算符处理
                }
                //单个运算符
                op[1]=0;
                if(buff[i] == '>'){
                    Token t;
                    t.token_type = TOKEN_TYPE_OPERATOR;
                    t.operator_type = OPERATOR_TYPE_GT;
                    memcpy(t.data,op,8);
                    v.push_back(t);
                }else if(buff[i] == '<'){
                    Token t;
                    t.token_type = TOKEN_TYPE_OPERATOR;
                    t.operator_type = OPERATOR_TYPE_LT;
                    memcpy(t.data,op,8);
                    v.push_back(t);
                }else if(buff[i] == '='){
                    Token t;
                    t.token_type = TOKEN_TYPE_OPERATOR;
                    t.operator_type = OPERATOR_TYPE_ES;
                    memcpy(t.data,op,8);
                    v.push_back(t);
                }else if(buff[i] == ';'){
                    Token t;
                    t.token_type = TOKEN_TYPE_OPERATOR;
                    t.operator_type = OPERATOR_TYPE_END;
                    memcpy(t.data,op,8);
                    v.push_back(t);
                }else if(buff[i] == '+'){
                    Token t;
                    t.token_type = TOKEN_TYPE_OPERATOR;
                    t.operator_type = OPERATOR_TYPE_ADD;
                    memcpy(t.data,op,8);
                    v.push_back(t);
                }else if(buff[i] == '-'){
                    Token t;
                    t.token_type = TOKEN_TYPE_OPERATOR;
                    t.operator_type = OPERATOR_TYPE_SUB;
                    memcpy(t.data,op,8);
                    v.push_back(t);
                }else if(buff[i] == '*'){
                    Token t;
                    t.token_type = TOKEN_TYPE_OPERATOR;
                    t.operator_type = OPERATOR_TYPE_MUL;
                    memcpy(t.data,op,8);
                    v.push_back(t);
                }else if(buff[i] == '/'){
                    Token t;
                    t.token_type = TOKEN_TYPE_OPERATOR;
                    t.operator_type = OPERATOR_TYPE_DIV;
                    memcpy(t.data,op,8);
                    v.push_back(t);
                }else if(buff[i] == '%'){
                    Token t;
                    t.token_type = TOKEN_TYPE_OPERATOR;
                    t.operator_type = OPERATOR_TYPE_MOD;
                    memcpy(t.data,op,8);
                    v.push_back(t);
                }else if(buff[i] == '('){
                    Token t;
                    t.token_type = TOKEN_TYPE_OPERATOR;
                    t.operator_type = OPERATOR_TYPE_LP;
                    memcpy(t.data,op,8);
                    v.push_back(t);
                }else if(buff[i] == ')'){
                    Token t;
                    t.token_type = TOKEN_TYPE_OPERATOR;
                    t.operator_type = OPERATOR_TYPE_RP;
                    memcpy(t.data,op,8);
                    v.push_back(t);
                }else{
                    printf("!!! unknown operator \"%s\"",op);
                }
                continue;
            }
            //读取数字
            if(check_char(int_char_list,buff[i])){ //第一个字符是数字
                int is_float = 0;
                int end_offset = len;
                for(int j=i+1;j<len;j++){
                    if(!check_char(int_char_list,buff[j])){
                        if(!check_char(float_char_list,buff[j])){
                            end_offset = j;
                            break;
                        }else{
                            is_float = 1;
                        }
                    }
                }
                char number[128];
                memset(number,0,128);
                memcpy(number,buff+i, end_offset-i);
                i+=strlen(number)-1;

                Token t;
                if(is_float){
                    t.token_type = TOKEN_TYPE_FLOAT;
                }else{
                    t.token_type = TOKEN_TYPE_INT;
                }
                memcpy(t.data,number,128);
                v.push_back(t);
                continue;
            }
            //读取名字
            if(check_char(name_char_list,buff[i])){
                int end_offset = len;
                for(int j=i+1;j<len;j++){
                    if(!check_char(name_char_list,buff[j])){
                        end_offset = j;
                        break;
                    }
                }
                char name[128];
                memset(name,0,128);
                memcpy(name,buff+i, end_offset-i);

                i+=strlen(name)-1;

                Token t;
                t.token_type = TOKEN_TYPE_NAME;
                memcpy(t.data,name,128);
                v.push_back(t);
                continue;
            }
            printf("input error!!! s:%s\n",buff+i);
        }
        return 0;
    }
    Lana(char* line){
        memset(origin_data,0,1024);
        memcpy(origin_data,line,strlen(line)+1);
    }
    Lana(){

    }
    ~Lana(){

    }
};

//中缀转后缀，目前只支持+-*/()
int infix_to_postfix(vector<Token>& in,vector<Token>& out){
    vector<Token> stack;
    for(int i=0;i<in.size();i++){
        Token t = in[i];
        if(t.token_type != TOKEN_TYPE_OPERATOR){
            out.push_back(t);
            continue;
        }
        //单独处理括号
        if(t.operator_type == OPERATOR_TYPE_LP){
            stack.push_back(t);
            continue;
        }
        if(t.operator_type == OPERATOR_TYPE_RP){
            while(stack.size()){
                Token t2 = stack[stack.size()-1];
                //一直遇到左括号
                if(t2.operator_type == OPERATOR_TYPE_LP) {
                    stack.pop_back();
                    break;
                }
                out.push_back(t2);
                stack.pop_back();
            }
            continue;
        }
        //处理+-*/
        while(stack.size()){
            Token t2 = stack[stack.size()-1];
            //如果栈顶元素优先级大于t,则输出到out
            if(op_pri_list[t2.operator_type] >= op_pri_list[t.operator_type]){
                out.push_back(t2);
                stack.pop_back();
            }else{
                break;
            }
        }
        stack.push_back(t);
    }
    //最后清空stack
    while(stack.size()){
        Token t2 = stack[stack.size()-1];
        out.push_back(t2);
        stack.pop_back();
    }
    return 0;
}

//矩阵加法 检查是否为同型矩阵在调用之前检测
Matrix operator+(Matrix x,Matrix y){
    Matrix z;
    z.row = x.row;
    z.col = x.col;
    z.data = (double**) malloc(sizeof(double) * z.row);
    for(int i=0;i<z.row;i++){
        z.data[i] = (double*) malloc(sizeof(double) * z.col);
    }
    for(int i=0;i<z.row;i++){
        for(int j=0;j<z.col;j++){
            z.data[i][j] = x.data[i][j] + y.data[i][j];
        }
    }
    return z;
}
//矩阵减法 检查是否为同型矩阵在调用之前检测
Matrix operator-(Matrix x,Matrix y){
    Matrix z;
    z.row = x.row;
    z.col = x.col;
    z.data = (double**) malloc(sizeof(double) * z.row);
    for(int i=0;i<z.row;i++){
        z.data[i] = (double*) malloc(sizeof(double) * z.col);
    }
    for(int i=0;i<z.row;i++){
        for(int j=0;j<z.col;j++){
            z.data[i][j] = x.data[i][j] - y.data[i][j];
        }
    }
    return z;
}
//矩阵数乘
Matrix operator*(double x,Matrix y){
    Matrix z;
    z.row = y.row;
    z.col = y.col;
    z.data = (double**) malloc(sizeof(double) * z.row);
    for(int i=0;i<z.row;i++){
        z.data[i] = (double*) malloc(sizeof(double) * z.col);
    }
    for(int i=0;i<z.row;i++){
        for(int j=0;j<z.col;j++){
            z.data[i][j] = x * y.data[i][j];
        }
    }
    return z;
}
Matrix operator*(Matrix x,double y){
    Matrix z;
    z.row = x.row;
    z.col = x.col;
    z.data = (double**) malloc(sizeof(double) * z.row);
    for(int i=0;i<z.row;i++){
        z.data[i] = (double*) malloc(sizeof(double) * z.col);
    }
    for(int i=0;i<z.row;i++){
        for(int j=0;j<z.col;j++){
            z.data[i][j] = y * x.data[i][j];
        }
    }
    return z;
}
//矩阵乘法 调用之前检查要求
Matrix operator*(Matrix x,Matrix y){
    Matrix z;
    z.row = x.row;
    z.col = y.col;
    z.data = (double**) malloc(sizeof(double) * z.row);
    for(int i=0;i<z.row;i++){
        z.data[i] = (double*) malloc(sizeof(double) * z.col);
    }
    for(int i=0;i<z.row;i++){
        for(int j=0;j<z.col;j++){
            double sum = 0;
            for(int k=0;k<x.col;k++){
                sum += x.data[i][k] * y.data[k][j];
            }
            z.data[i][j] = sum;
        }
    }
    return z;
}

Var cal_postfix(vector<Token>& v){
    vector<Var> ret;
    for(int i=0;i<v.size();i++){
        Token t = v[i];
        if(t.token_type == TOKEN_TYPE_OPERATOR){
            if(ret.size() < 2){
                printf("后缀表达式错误\n");
                Var var;
                var.var_type = VAR_TYPE_NUM;
                var.num = 0;
                return var;
            }
            //先实数运算 左右两边都是实数
            if(ret[ret.size()-1].var_type == VAR_TYPE_NUM && ret[ret.size()-2].var_type == VAR_TYPE_NUM){
                double x = ret[ret.size()-2].num;
                double y = ret[ret.size()-1].num;
                ret.pop_back();ret.pop_back();
                if(t.operator_type == OPERATOR_TYPE_ADD){
                    Var var;
                    var.var_type = VAR_TYPE_NUM;
                    var.num = x + y;
                    ret.push_back(var);
                }else if(t.operator_type == OPERATOR_TYPE_SUB){
                    Var var;
                    var.var_type = VAR_TYPE_NUM;
                    var.num = x - y;
                    ret.push_back(var);
                }else if(t.operator_type == OPERATOR_TYPE_MUL){
                    Var var;
                    var.var_type = VAR_TYPE_NUM;
                    var.num = x * y;
                    ret.push_back(var);
                }else if(t.operator_type == OPERATOR_TYPE_DIV){
                    Var var;
                    var.var_type = VAR_TYPE_NUM;
                    var.num = x / y;
                    ret.push_back(var);
                }else{
                    printf("不支持运算符 %d\n",t.operator_type);
                    Var var;
                    var.var_type = VAR_TYPE_NUM;
                    var.num = 0;
                    ret.push_back(var);
                }
            }
            //矩阵运算 左右两边都是矩阵
            if(ret[ret.size()-1].var_type == VAR_TYPE_MAT && ret[ret.size()-2].var_type == VAR_TYPE_MAT){
                Var x = ret[ret.size()-2];
                Var y = ret[ret.size()-1];
                ret.pop_back();ret.pop_back();
                if(t.operator_type == OPERATOR_TYPE_ADD){
                    Var var;
                    var.var_type = VAR_TYPE_MAT;
                    var.matrix.row = 0;
                    var.matrix.col = 0;
                    //检查是否为同型矩阵
                    if(x.matrix.row != y.matrix.row || x.matrix.col != y.matrix.col){
                        printf("矩阵加法运算错误，非同型矩阵\n");
                        ret.push_back(var);
                        continue;
                    }
                    var.matrix = x.matrix + y.matrix;
                    ret.push_back(var);
                }else if(t.operator_type == OPERATOR_TYPE_SUB){
                    Var var;
                    var.var_type = VAR_TYPE_MAT;
                    var.matrix.row = 0;
                    var.matrix.col = 0;
                    //检查是否为同型矩阵
                    if(x.matrix.row != y.matrix.row || x.matrix.col != y.matrix.col){
                        printf("矩阵加法运算错误，非同型矩阵\n");
                        ret.push_back(var);
                        continue;
                    }
                    var.matrix = x.matrix - y.matrix;
                    ret.push_back(var);
                }else if(t.operator_type == OPERATOR_TYPE_MUL){
                    Var var;
                    var.var_type = VAR_TYPE_MAT;
                    var.matrix.row = 0;
                    var.matrix.col = 0;
                    //检查是否满足矩阵乘法要求
                    if(x.matrix.col != y.matrix.row){
                        printf("矩阵乘法运算错误\n");
                        ret.push_back(var); //返回0矩阵
                        continue;
                    }
                    var.matrix = x.matrix * y.matrix;
                    ret.push_back(var);
                }else{
                    printf("不支持运算符 %d\n",t.operator_type);
                    Var var;
                    var.var_type = VAR_TYPE_MAT;
                    var.matrix.row = 0;
                    var.matrix.col = 0;
                    ret.push_back(var);
                }
            }
            //矩阵数乘
            if((ret[ret.size()-1].var_type == VAR_TYPE_NUM && ret[ret.size()-2].var_type == VAR_TYPE_MAT) ||
            (ret[ret.size()-1].var_type == VAR_TYPE_MAT && ret[ret.size()-2].var_type == VAR_TYPE_NUM)){
                Var x = ret[ret.size()-2];
                Var y = ret[ret.size()-1];
                ret.pop_back();ret.pop_back();
                if(t.operator_type == OPERATOR_TYPE_MUL){
                    if(x.var_type == VAR_TYPE_NUM){
                        Var var;
                        var.var_type = VAR_TYPE_MAT;
                        var.matrix.row = 0;
                        var.matrix.col = 0;
                        var.matrix = x.num * y.matrix;
                        ret.push_back(var);
                    }else{
                        Var var;
                        var.var_type = VAR_TYPE_MAT;
                        var.matrix.row = 0;
                        var.matrix.col = 0;
                        var.matrix = x.matrix * y.num;
                        ret.push_back(var);
                    }
                }else{
                    printf("不支持运算符 %d\n",t.operator_type);
                    Var var;
                    var.var_type = VAR_TYPE_MAT;
                    var.matrix.row = 0;
                    var.matrix.col = 0;
                    ret.push_back(var);
                }
            }

        }else if(t.token_type == TOKEN_TYPE_INT || t.token_type == TOKEN_TYPE_FLOAT){
            Var var;
            var.var_type = VAR_TYPE_NUM;
            var.num = atof(t.data);
            ret.push_back(var);
        }else{
            ret.push_back(global_var_map[t.data]);
        }
    }
    return ret[0];
}

void print_matrix(Matrix matrix){
    printf("mat:%dx%d\n",matrix.row,matrix.col);
    for(int i=0;i<matrix.row;i++){
        for(int j=0;j<matrix.col;j++){
            printf("%12.6lf ",matrix.data[i][j]);
        }
        cout<<endl;
    }
}

void CalLoop(){
    char line[1024];
    while(true){
        printf(">>>");
        memset(line,0,1024);
        cin.getline(line,1024);

        //词法分析
        vector<Token> v;
        Lana lana(line);
        lana.get_tokens(v);

        if(v.size() == 0){
            continue;
        }

        //命令
        //InsertMatrix = [var_name] ; [row] ;[col] 接着输入row行col列的矩阵 矩阵暂时仅支持实数
        if(strcmp(v[0].data,"InsertMatrix")==0){
            //检查命令格式是否正确,ver_name是字符串，row，col是整数
            if(v.size() != 7 || v[2].token_type != TOKEN_TYPE_NAME || v[4].token_type != TOKEN_TYPE_INT || v[6].token_type != TOKEN_TYPE_INT ){
                printf("命令错误，InsertMatrix = [var_name] ; [row] ;[col] \n");
                continue;
            }
            int row = atoi(v[4].data);
            int col = atoi(v[6].data);
            Var var;
            var.var_type = VAR_TYPE_MAT;
            var.matrix.row = row;
            var.matrix.col = col;
            var.matrix.data = (double**) malloc(sizeof(double) * row);
            for(int i=0;i<row;i++){
                var.matrix.data[i] = (double*) malloc(sizeof(double) * col);
            }
            //输入矩阵
            for(int i=0;i<row;i++){
                for(int j=0;j<col;j++){
                    double num;
                    cin>>num;
                    var.matrix.data[i][j] = num;
                }
            }
            getchar();//吃掉最后一个回车
            //保存矩阵到全局变量中
            global_var_map[v[2].data] = var;
            continue;
        }

        //赋值
        if(v[0].token_type == TOKEN_TYPE_NAME && v[1].token_type == TOKEN_TYPE_OPERATOR && v[1].operator_type == OPERATOR_TYPE_ES){
            vector<Token> temp;
            for(int i=2;i<v.size();i++){
                temp.push_back(v[i]);
            }
            vector<Token> out;
            infix_to_postfix(temp,out);
            Var var_ret = cal_postfix(out);

            global_var_map[v[0].data] = var_ret;
            continue;
        }
        //直接表达式求值
        vector<Token> out;
        infix_to_postfix(v,out);
        Var var_ret = cal_postfix(out);
        if(var_ret.var_type == VAR_TYPE_NUM){
            printf("%lf\n",var_ret.num);
        }else if(var_ret.var_type == VAR_TYPE_MAT){//打印矩阵
            print_matrix(var_ret.matrix);
        }

    }
}

int main() {
    CalLoop();
    return 0;
}
