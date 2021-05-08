#include <iostream>
#include <utility>
#include <vector>
#include <string.h>

using namespace std;

unsigned short GRID_WIDTH = 0, GRID_HEIGHT = 0;
unsigned int NODE_COUNT = 0;

const unsigned short MAX_SIZE = 46340;
const unsigned NODE_MAX_COUNT = (MAX_SIZE * MAX_SIZE);

bool visited[NODE_MAX_COUNT];

enum MOV_TYPE
{
    DIRECT,
    REQUIRES_INTERMIDIATE,
    INVALID
};

pair<int, int> getCoords(int number)
{
    return make_pair<int, int>(number / GRID_WIDTH, number % GRID_HEIGHT);
}

int getNode(pair<int, int> coords)
{
    int node = 0;
    node += coords.first * GRID_WIDTH;
    node += coords.second;
    return node;
}

//Subtração entre pairs
pair<int, int> operator-(pair<int, int> a, pair<int, int> b)
{
    return make_pair(a.first - b.first, a.second - b.second);
}

//Divisão inteira de pairs por ints
pair<int, int> operator/(pair<int, int> a, int b)
{
    return make_pair(a.first / b, a.second / b);
}

//Soma de pairs
pair<int, int> operator+(pair<int, int> a, pair<int, int> b)
{
    return make_pair(a.first + b.first, a.second + b.second);
}

MOV_TYPE classifyMovement(pair<int, int> diff)
{
    int absFirst = abs(diff.first);
    int absSecond = abs(diff.second);
    int absDiff = abs(absSecond - absFirst);

    if (absDiff == 1) //Casos: 01 10 21 12
        return DIRECT;

    if (absFirst == 1 and absSecond == 1) //11
        return DIRECT;

    if (absFirst == 2 and absSecond == 2) //22
        return REQUIRES_INTERMIDIATE;

    if (absFirst == 0 and absSecond == 0) //00
        return INVALID;

    if (absDiff == 2) //02 ou 20
        return REQUIRES_INTERMIDIATE;

    return INVALID;
}

unsigned long long int soma = 0;
void nextNode(int digitos, int atual)
{
    if (digitos == 0)
    {
        ++soma;
        return;
    }

    //Define o nó atual como visitado (até o fim desse nível recursivo)
    visited[atual] = true;

    //Calcula as coordenadas e distâncias dos nós atual e prox
    pair<int, int> atualCoords = getCoords(atual);
    pair<int, int> proxCoords;
    pair<int, int> diff;

    for (int prox = 0; prox < NODE_COUNT; prox++)
    {
        //Se o prox já foi visitado, não tem mágica
        if (visited[prox])
            continue;

        //Calcula as coords e diff para cada prox
        proxCoords = getCoords(prox);
        diff = proxCoords - atualCoords;

        //Checa se o movimento é válido, inválido ou requer visita prévia ao nó intermediário
        MOV_TYPE movType = classifyMovement(diff);
        if (movType == INVALID)
            continue;

        if (movType == REQUIRES_INTERMIDIATE)
        { //Se o movimento precisar de um nó intermediário:
            //Obtém as coordenadas do ponto que precisa ter sido visitado (nó intermediário)
            auto requiredNodeCoords = atualCoords + (diff / 2);
            int requiredNode = getNode(requiredNodeCoords);

            //Se não tiver sido visitado, o movimento é inválido
            if (!visited[requiredNode])
                continue;

            //Senão ele executa a linha abaixo fora do if
        }

        nextNode(digitos - 1, prox);
    }

    //Saindo do nível atual de recursão, ainda não foi visitado
    visited[atual] = false;
}

int main(int argc, char const *argv[])
{
    cout << "Welcome to android-pattern-counter" << endl;
    cout << "Type in width and height of your pattern grid (typically it's 3x3)" << endl;

    while (GRID_WIDTH <= 0 || GRID_WIDTH > MAX_SIZE)
    {
        cout << "Width (1-"<<MAX_SIZE<<"): ";
        cin >> GRID_WIDTH;
    }
    while (GRID_HEIGHT <= 0 || GRID_HEIGHT > MAX_SIZE)
    {
        cout << "Height (1-"<<MAX_SIZE<<"): ";
        cin >> GRID_HEIGHT;
    }

    NODE_COUNT = GRID_WIDTH * GRID_HEIGHT;

    int minDigs, maxDigs;

    do
    {
        cout << "Minimum connections in pattern (0-" << NODE_COUNT << "): ";
        cin >> minDigs;
    } while (minDigs < 0 || minDigs > NODE_COUNT);

    do
    {
        cout << "Maximum connections in pattern (" << minDigs << "-" << NODE_COUNT << "): ";
        cin >> maxDigs;
    } while (maxDigs < minDigs || maxDigs > NODE_COUNT);

    int somaTotal = 0;
    //Determina o tamanho da senha
    for (int digitos = minDigs; digitos <= maxDigs; digitos++)
    {
        //Escolhe o primeiro nó
        for (int inicio = 0; inicio <= maxDigs - 1; inicio++)
        {
            //Calcula o numero de senhas com o inicio definido e soma na var global soma
            nextNode(digitos - 1, inicio);
        }
        cout << "Passwords with exactly " << digitos << " digits: " << soma << endl;
        somaTotal += soma;
        soma = 0;
    }

    cout << "All passwords with length between " << minDigs << " and " << maxDigs << ": " << somaTotal << endl;

    return 0;
}
