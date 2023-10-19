#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum {VALID_INPUT, TOO_FEW, INVALID_BASE, INVALID_NUMBER} Return_value;
typedef enum {BINARY = 2, OCTAL = 8, DECIMAL = 10, HEXADECIMAL = 16} Value;

typedef struct {
    Value value;
    char alphabet[20];
} Bases;

char resultado[100];
char options[][20] = {"--b", "--o", "--d", "--h", "--all"};

//retorna o valor de um digito em seu alfabeto
int comp_alph(char digit, Bases base) {
    int i;

    for(i = 0; i < strlen(base.alphabet); i++)
        if(digit == base.alphabet[i])
            return i;
}

//converte um numero em qualquer base para decimal
long convert_to_dec(char num[], Bases base) {
    int i, j, digit; 
    long total = 0;

    for(i = strlen(num) - 1, j = 0; i >= 0; i--, j++) {
        digit = comp_alph(num[i], base);
        total += digit * pow(base.value, j);
    }

    return total;
}

//coloca o numero convertido em ordem certa
void format_result(int len) {
    char aux;
    int i;

    for(i = 0; i < len / 2; i++) {
        aux = resultado[i];
        resultado[i] = resultado[len - i - 1];
        resultado[len - i - 1] = aux;
    }
}

//converte um numero decimal para qualquer outra base
void convert_base(long num, Bases base) {
    int ctrl, i = 0;

    while(1) {
        ctrl = num % base.value;
        resultado[i] = base.alphabet[ctrl];
        num /= base.value;
        i++;

        if(num < base.value) {
            resultado[i] = base.alphabet[num];
            resultado[i + 1] = '\0';
            format_result(strlen(resultado));
            break;
        }
    }
}

//encontra a base de acordo com a opção escolhida
Bases find_base(char str_base[]) {
    Bases all_bases[] = {{BINARY, "01"}, {OCTAL, "01234567"}, {DECIMAL, "0123456789"}, {HEXADECIMAL, "0123456789ABCDEF"}};
    int i;

    for(i = 0; i < 5; i++)
        if(!strcmp(str_base, options[i]))
            return all_bases[i];
}

//imprime os numeros convertidos
void print_result(long initial_number, Bases initial_base, int total, char str_bases[][20]) {
    int i;
    Bases current_base;

    for(i = 0; i < total; i++) {
        current_base = find_base(str_bases[i]);
        convert_base(initial_number, current_base);

        if(current_base.value != initial_base.value)
            printf("%s(%d) ", resultado, current_base.value);
    }
    printf("\n");
}

//mostra a mensagem de erro adequada
void errors(Return_value value) {
    switch(value) {
        case TOO_FEW:
            printf("ERRO: parâmetros insuficientes.\n");
            break;
        case INVALID_BASE:
            printf("ERRO: uma ou mais bases inválidas.\n");
            break;
        case INVALID_NUMBER:
            printf("ERRO: o numero não condiz com a base informada.\n");
            break;
    }
}

//verifica se os parametros estão certos para a execução
int check_input(int len, char parameters[][20]) {
    if(len < 3) {
        errors(TOO_FEW);
        return -1;
    }
    
    int i, j, cont = 0;
    for(i = 1; i < len; i++)
        for(j = 0; j < 5; j++)
            if(!strcmp(parameters[i], options[j]))
                cont++;
    
    if(cont != len - 1) {
        errors(INVALID_BASE);
        return -1;
    }

    cont = 0;
    Bases initial_base = find_base(parameters[1]);
    for(i = 0; i < strlen(parameters[0]); i++) {
        for(j = 0; j < strlen(initial_base.alphabet); j++) {
            if(parameters[0][i] == initial_base.alphabet[j]) {
                cont++;
            }
        }
    }
    if(cont != strlen(parameters[0])) {
        errors(INVALID_NUMBER);
        return -1;
    }

    return VALID_INPUT;
}

//divide uma linha do arquivo e colocar cada parte em um vetor de strings
void divide_input(char input[], int *argc, char argv[][20]) {
    char divider[] = " ";
    
    char *result = strtok(input, divider) ;

    while(result != NULL) {
        strcpy(argv[*argc], result);
        result = strtok(NULL, divider);
        (*argc)++;
    }
}

void execute() {
    long initial_number;
    Bases initial_base;
    int total_lines, argc, cont = 1;
    char input[100], argv[5][20];
    
    scanf("%d", &total_lines);
    
    while(cont <= total_lines) {
        scanf(" %[^\n]", input);
        argc = 0;
        divide_input(input, &argc, argv);
        printf("Conversão %02d: ", cont);

        if(check_input(argc, argv) == VALID_INPUT) {
            initial_base = find_base(argv[1]);
            initial_number = convert_to_dec(argv[0], initial_base);
            printf("%s(%d) = ", argv[0], initial_base.value);

            if(!strcmp(argv[2], "--all"))
                print_result(initial_number, initial_base, 4, options);
            else
                print_result(initial_number, initial_base, argc - 2, &argv[2]);
        }
        cont++;
    }
}

int main() {
    execute();
    return 0;
}