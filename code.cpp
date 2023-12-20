//--------------------------------------------------------------------------
//Trabalho prático 3                                                        |
//Objetivo: Sequência de melhores manobras Prog dinamica                    |
//Aluno: João Marcos Tomáz Silva Campos                                     |
//Matrícula: 2022043728                                                     |
//Disciplina: Algoritmos 1                                                  |
//Professor: GG                                                             |
//--------------------------------------------------------------------------|

#include <iostream>
#include <vector>
#include <climits>
#define MAX_SECTIONS 100
#define MAX_POSSIBILITIES 1024


using namespace std;

//converte um numero decimal para binario
vector<bool> convert_binary(long long int number, long long int K){
    vector<bool> binary(K, false);
    long long int j = 0;
    for(long long int i = binary.size() - 1; i >= 0; i--){
        long long int bit = number >> i;
        if(bit & 1){
            binary[j] = true;
        }
        j++;
    }
    return binary;
}

//struct para armazenar as manobras
struct maneuver{
    long long int duration;
    long long int score;
};
// struct para armazenar os resultados da prog dinamica
struct dp{
    long long int points;
    long long int maneuvers;
};

// Variaveis globais para o código
maneuver pre_computed_matrix[MAX_POSSIBILITIES][MAX_POSSIBILITIES];
dp dp_matrix[MAX_SECTIONS + 1][MAX_POSSIBILITIES];
vector<pair<long long int,long long int> > sections;
vector<maneuver> maneuvers;
long long int N, K;

//Função para precomputar a matriz de possibilidades, dado que o numero maximo = 2^10
void precomputeMatrix(vector<maneuver> maneuvers){
    for(int i = 0; i < MAX_SECTIONS + 1; i++){
        for(int j = 0; j < MAX_POSSIBILITIES; j++){
            dp_matrix[i][j] = {LLONG_MIN, -1};
        }
    }

    //Utiliza mascara de bits para percorrer todas as possibilidades de manobras
    //Caso todas não seja, a matriz fica com lixo de memória porém não é acessado
    long long int bit_mask = 1 << K;

    for(long long int mask = 0; mask < bit_mask; mask++){
        vector<bool> maneuvers_used;
        maneuvers_used = convert_binary(mask, K);

        for(long long int next_mask = 0; next_mask < bit_mask; next_mask++){
            long long int current_score = 0;
            long long int current_duration = 0;
            long long int quantity = 0;
            vector<bool> maneuvers_now;
            maneuvers_now = convert_binary(next_mask, K);

            //Checa se a manobra foi utilizada na seção anterior e conta a quantidade de manobras
            for(long long int j = 0; j < K;j++){
                if(maneuvers_now[j]){
                    quantity++;
                    current_score += maneuvers_used[j] ? maneuvers[j+1].score / 2 : maneuvers[j+1].score;
                    current_duration += maneuvers[j+1].duration;
                }
            }
            //Armazena na matriz de precomputação os valores multiplicados pela quantidade de manobras e a duração
            pre_computed_matrix[mask][next_mask].duration = current_duration;
            pre_computed_matrix[mask][next_mask].score = current_score * quantity;
        }
    }
}

//Função para calcular o maior score dado uma seção e uma quantidade de manobras
dp calculate_max_score(long long int current_section, long long int maneuvers, long long int N){

    //Caso já tenha sido calculado, retorna o valor
    if(dp_matrix[current_section][maneuvers].points != LLONG_MIN){
        return dp_matrix[current_section][maneuvers];
    }

    //Caso seja a ultima seção, retorna o valor máximo
    if(current_section == N){
        long long int max_points = 0;
        long long int best_sequence = 0;
        for(long long int i = 0; i < (1<<K); i++){
            if((pre_computed_matrix[maneuvers][i].score > max_points) && (pre_computed_matrix[maneuvers][i].duration <= sections[current_section].second)){
                max_points = pre_computed_matrix[maneuvers][i].score;
                best_sequence = i;
            }
        }
        max_points = max_points * sections[current_section].first;
        dp_matrix[current_section][maneuvers] = {max_points, best_sequence};
        return dp_matrix[current_section][maneuvers];
    }

    dp max_score;
    long long int score_aux = 0;
    //Percorre todas as possibilidades de manobras
    for(long long int mask = 0; mask < (1 << K); mask++){
        long long int current_duration = pre_computed_matrix[maneuvers][mask].duration;

        //Caso a duração seja maior que o tempo da seção, não é possível fazer a manobra
        if(current_duration > sections[current_section].second){
            continue;
        }

        //Caso seja possível, calcula o score por meio da recursão
        long long int current_score = pre_computed_matrix[maneuvers][mask].score;
        max_score = calculate_max_score(current_section + 1, mask, N);

        long long int new_score = max_score.points + current_score * sections[current_section].first;

        //Caso o score seja maior que o atual, atualiza o valor
        if(new_score > score_aux){
            score_aux = new_score;
            max_score.points = new_score;
            max_score.maneuvers = mask;
            dp_matrix[current_section][maneuvers] = max_score;
        }

    }
    return dp_matrix[current_section][maneuvers];
}



int main(){

    //N = numero de seções na pista, M = numero de manobras

    cin >> N >> K;

    //Adiciona uma seção e uma manobra vazia para facilitar a indexação e inicializar as estruturas
    sections.push_back({});
    maneuvers.push_back({});

    //Lê as seções e as manobras
    for(int i = 0; i < N;i++){
        //first = bonificação, second = tempo de travessia
        long long int bonification, tempo;
        cin >> bonification;
        cin >> tempo;
        sections.push_back({bonification, tempo});
    }
    for(int i = 0; i < K; i++){
        long long int score, duration;
        cin >> score;
        cin >> duration;
        maneuvers.push_back({duration, score});
    }
    
    //Precomputa a matriz de possibilidades
    precomputeMatrix(maneuvers);

    //Calcula o maior score
    dp max_score = calculate_max_score(1, 0, N);

    //Imprime o resultado
    cout << max_score.points << endl;

    //Imprime a quantidade de manobras e quais foram utilizadas
    for(int j = 1; j <= N; j++){
        vector<bool> maneuvers_used;
        maneuvers_used = convert_binary(max_score.maneuvers, K);
        int quantity = 0;
        for(int i = 0; i < K; i++){
            if(maneuvers_used[i]){
                quantity++;
            }
        }
        cout << quantity << " ";
        for(int i = 0; i < K; i++){
            if(maneuvers_used[i]){
                cout << i + 1 << " ";
            }
        }
        cout << endl;
        max_score = dp_matrix[j+1][max_score.maneuvers];
    }
    return 0;

}