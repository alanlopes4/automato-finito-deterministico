#include <stdio.h>
#include <string.h>


typedef struct
{
    int start;
    char symbol;
    int end;
}transicao;


int main() {
    /** VARIAVEIS DO AUTOMATO**/
    int numEstados = 0; //armazena o número de estados total do automato
    int numEstadosFinais = 0; //armazena o número de estados finais do automato
    char alfabeto[1024];    //armazena o alfabeto do automato
    int estadoInicial = 0; //armazea o estado inicial do automato
    char entrada[102400]; //armazena a fita de entrada do automato

    //armazena a linha de estado final digitado pelo usuário
    //estadosFinais_backup é utilizado para correção de um bug com o strtok
    char estadosFinais[256], estadosFinais_backup[256];
    //delimita a separação da linha, o limite é o whitespace, porque a entrada de estados finais serão separadas por espaço em branco
    char delimitador[] = " ";
    //utilizaoo para armazenar os dífitos dos estados finais
    char *token;



    printf("Quantidade de estados:");
    //O %*c , ele digitaliza e descarta um caractere, para este caso estamos descartando o \n
    scanf("%d%*c", &numEstados);
    //printf("%d\n", numEstados);
    //após cada entrada (scanf, fgets...) é uma boa prática limpar o buffer de entrada(stdin)
    //Essa limpeza evita futuros conflitos
    fflush(stdin);

    printf("Alfabeto:");
    //lerAlfabeto(alfabeto, 1024);
    fgets(alfabeto, 1024, stdin);
    fflush(stdin);
    //printf("%s\n", alfabeto);

    printf("Estado incial:");
    scanf("%d%*c", &estadoInicial);
    fflush(stdin);



    printf("Estados finais:");
    fgets(estadosFinais, 256, stdin);
    //faz a cópia dos estados finais
    //correção de bug
    strcat(estadosFinais_backup, estadosFinais);
    //strtok faz split dos estadosFinais separando-os por espaço em branco
    token = strtok(estadosFinais, delimitador );

    //Calcula a quantidade de estados finais
    while(token != NULL)
    {
        numEstadosFinais++;
        token = strtok(NULL, delimitador);//avança para a próxima string
    }
    //array que armazena os estados finais
    int listaEstadosFinais[numEstadosFinais];
    int index = 0;
    token = strtok(estadosFinais_backup, delimitador);
    //Preenche a lista de estadosFinais, convertendo (atoi()) os dados para inteiro
    while(token != NULL)
    {
        //atoi converte uma string para inteiro
        listaEstadosFinais[index] = atoi(token);
        index++;
        token = strtok(NULL, delimitador);
    }

    fflush(stdin);
    //armazena a quantidade de transições
    int qntd_transicoes = numEstados*(strlen(alfabeto)-1);

    //cria um array da struct transicao
    transicao transicoes[qntd_transicoes];

    printf("Para esse automato são necessario %d transicoes.\n\n", qntd_transicoes);

    //Pega as transições digitadas pelo usuário
    for(int i = 0; i < qntd_transicoes; i++)
    {
        printf("Entre com a transicao[%d]:", i+1);

        scanf("%d %c %d%*c", &transicoes[i].start, &transicoes[i].symbol, &transicoes[i].end);
        fflush(stdin);
    }

    printf("\n\nAUTOMATO GERADO COM SUCESSO!!");
    printf("\nDigite o alfabeto de entrada:\n");
    fflush(stdin);

    //Enquanto o usuário estiver digitando, faça
    while(fgets(entrada, 102400, stdin) != NULL)
    {
        //estado onde o automato parou após todo o processamento com a fita de entrada
        int estado = processamento(entrada, estadoInicial, transicoes, qntd_transicoes);
        //Se o estado onde o automato parou pertencer ao conjunto de estados finais
        //Então a entrada foi ACEITA
        //Caso  contrário, REJEITADA
        if(compararEstados(estado, listaEstadosFinais, numEstadosFinais) == 1)
            printf("ACEITA\n");
        else
            printf("REJEITADA\n");
    }

    return 0;
}

 //Filtra uma transicao pelo estado e simbolo
 transicao filtrar_por_estadoEsimbolo(int estado, char simbolo, transicao* transicoes, int tamanho_transicoes)
{
    //laço passando por todas as transições
    for(int i=0;i<tamanho_transicoes;i++)
    {
        //se o estado e o simbolo daquela transição baterem, então retorna-a
        if(transicoes[i].start == estado && transicoes[i].symbol == simbolo)
        {
            return transicoes[i];
        }
    }

    //Caso contrário, retorna uma transição onde o start é -1
    //-1 pois não tem nenhuma transição com o start -1
    transicao t;
    t.start = -1;
    return t;
}

//Compara o estadoAtual com os estadosFinais
//Se forem iguais quer dizer que o automato, após todo o processamento com a fita de entrada, parou em um estado final
//Caso contrário, não
int compararEstados(int estado, int estadosFinais[], int numEstadosFinais)
{
    int aceita = 0;
    for(int i=0;i<numEstadosFinais;i++)
    {
        if(estado == estadosFinais[i])
        {
            aceita = 1;
            break;
        }
    }
   return aceita;
}

//Realiza o processamento dos dados
//Verifica cada simbolo da fita
int processamento(char entrada[], int estadoAtual, transicao transicoes[], int qntd_transicoes)
{
    int tam_entrada = strlen(entrada) - 1;
    int transicao_encontrada = 1;

    //passa por todos os simbolos da fita, ex: aab -> vai passar por a, a e b.
    //Trabalha com cada letra da string
    for(int i=0; i< tam_entrada; i++)
    {
        //atualiza o simbolo com o char lido
        char simbolo = entrada[i];
        transicao transicao_filtrada = filtrar_por_estadoEsimbolo(estadoAtual, simbolo, transicoes, qntd_transicoes);
        //printf("transicao filtrada: %d %c %d\n", transicao_filtrada.start, transicao_filtrada.symbol, transicao_filtrada.end);

        //Se a transição retornada for diferente de -1, atualiza o estado atual
        //Em outras palavras, se for encontrada alguma transição atualiza o estado Atual para o próximo estado da transição(end)
        if(transicao_filtrada.start != -1)
            estadoAtual = transicao_filtrada.end;
        else
        {
            transicao_encontrada = 0;
            break;

        }

    }
        //Se após todo o processamento todas as transições foram encontradas
        //Retorn o estadoAtual
        //Caso contrário, retorne -1
        if(transicao_encontrada == 1)
            return estadoAtual;
        else
            return -1;

}
