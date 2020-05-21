#include <iostream>
#include<cstdlib>
#include<ctime>
#include<string>
#include <fstream>
#include <windows.h>
#include<random>
#include <limits>
#include "BigIntegerLibrary.hh"
using namespace std;



double PCFreq = 0.0;
__int64 CounterStart = 0;


void StartCounter()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
    cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart)/1000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
}




//========================= PARA NÚMEROS MUITO GRANDES QUES ESTOURAM LONG LONG INT ===============================


/*

BigInteger bigpow (BigInteger x, BigInteger y)
{
    if(y == 1)
        return x;
    return x*(bigpow(x, y-1));
}


BigInteger karatsuba(BigInteger u, BigInteger v, int n)
{
    if(n<=3)
        return u*v;
    int m = n/2;
    BigInteger x = bigpow(10,m);
    BigInteger x2 = bigpow(10,2*m);
    BigInteger p = u/x;
    BigInteger q = u%x;
    BigInteger r = v/x;
    BigInteger s = v%x;
    BigInteger pr = karatsuba(p,r,m);
    BigInteger qs = karatsuba(q,s,m);
    BigInteger y= karatsuba(p + q, r + s, m+1);
    return pr*(x2) + (y - pr - qs)*x + qs;
}

int main()
{
    string A("284724812304981334590324");
    string B("984357298374619238698347");
    BigInteger a = stringToBigInteger(A);
    BigInteger b = stringToBigInteger(B);

    StartCounter();
    BigInteger normal = a*b;
    cout<<"\nnormal:     " << a << " x " << b << " = " << normal;
    cout<<"  com tempo: "<<GetCounter()<<"ms";

    StartCounter();
    BigInteger kara = karatsuba(a,b,24);
    cout<<"\nkaratsuba:  " << a << " x " << b << " = " << kara;
    cout<<"  com tempo: "<<GetCounter()<<"ms";
    return 0;
}

*/





//========================== PARA NÚMEROS MENORES QUE CABEM DENTRO DE LONG LONG INT ============================





#define GINT unsigned long long int //definicao do tipo de dado que sera usado
#define rep 50                     //quantidade de repeticoes do loop for
#define lower 100000000000          //limite inferior dos numeros gerados
#define upper 999999999999          //limite superior dos numeros gerados


//Versao Iterativa da funcao pow que aceita e retorna o tipo GINT
GINT Gpow(int a, GINT b)
{
    GINT aux=1;
    for(int i=0;i<b;i++)
    {
        aux *=a;
    }
    return aux;
}

//=================karatsuba-ofman para realizar u*v, tendo cada um n digitos======================\\

GINT karatsuba(GINT u, GINT v, GINT n)
{
    if(n<=3)
        return u*v;
    int m = n/2;
    GINT x = Gpow(10,m);
    GINT p = u/x;
    GINT q = u%x;
    GINT r = v/x;
    GINT s = v%x;
    GINT pr = karatsuba(p,r,m);
    GINT qs = karatsuba(q,s,m);
    GINT y= karatsuba(p + q, r + s, m+1);
    GINT x2 = Gpow(10,2*m);
    return pr*(x2) + (y - pr - qs)*x + qs;
}
//================================================================================================\\

int main()
{
    //================Codigo para geracao de uma distribuicao normal para obter os numeros aleatorios==============
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long long int> dis(lower, upper);
    //=============================================================================================================

    int TAM = (to_string(upper)).length(); //Quantidade de digitos dos numeros gerados

    GINT a, b, normal, kara;               //Variaveis para armazenamento temporario e manipulacao das operacoes


    double t1[rep], t2[rep], media1, media2;//Variaveis de controle de tempo de cada execucao

    int hits=0;                             //Variavel de controle da corretude do metodo

//=========================Laco principal do codigo===============================\\

    for(int i=0;i<rep;i++)
    {
        a = dis(gen);                        //Set da variavel a
        b = dis(gen);                        //Set da variavel b
        cout<<"a = "<<a<<"  ||  b = "<<b<<endl;

        //==============Bloco de excucao da operacao convencional ===============================
        StartCounter();                      //Inicia um contador de tempo
        normal = a*b;                        //Realiza a multiplicacao convencional
        printf("");                          //Apenas para garantir que o tempo sera contabilizado
        t1[i] = GetCounter();                //Armazena o tempo de execucao num vetor
        //========================================================================================

        //==============Bloco de excucao da operacao karatsuba-ofman ===============================
        StartCounter();                      //Inicia um contador de tempo
        kara = karatsuba(a,b,TAM);           //Realiza a multiplicacao de Karatsuba
        printf("");                          //Apenas para garantir que o tempo sera contabilizado
        t2[i]=GetCounter();                  //Armazena o tempo de execucao num vetor
        //==========================================================================================

        if(normal == kara) hits++; //Compara se os metodos obteram o mesmo resultado

        //Impressoes na tela
        cout<<"Metodo normal    a x b = "<<normal<< " em "<<t1[i]<<"ms\n";
        cout<<"Metodo karatsuba a x b = "<<kara<< " em "<<t2[i]<<"ms\n\n";
    }
//=====================Fim do laco principal======================================================\\

//=====================Calculo das medias de tempo======
    for(int i =0;i<rep;i++)
    {
        media1 +=t1[i];
        media2 +=t2[i];
    }
    media1 = media1/rep;
    media2 = media2/rep;
//======================================================

    //Impressoes finais e calculos percentuais
    cout<<"===============================================/========================================\n";
    cout<<"Tipo de dado usado                                    =  unsigned long long int\n";
    cout<<"Valor maximo suportado pelo tipo de dado              =  "<<std::numeric_limits<GINT>::max()<<"\n";
    cout<<"Quantidade de digitos dos numeros testados            =  "<<TAM<<"\n";
    cout<<"Quantidade de execucoes testadas                      =  "<<rep<<"\n";
    cout<<"Porcentagem de acertos do metodo karatsuba-ofman      =  "<<(100.0*hits/rep)<<"%\n";
    cout<<"Media de tempo de execucao normal em ms               =  "<<media1<<"\n";
    cout<<"Media de tempo de execucao Karatsuba-ofman em ms      =  "<<media2<<"\n";
    cout<<"Diferenca percentual dos dois metodos                 =  "<<(100.0 -100.0*media1/media2)<<"%";



    return 0;
}
