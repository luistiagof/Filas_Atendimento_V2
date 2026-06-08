#include <iostream>
#include <string>
#include <sstream>

using namespace std;

// Nome: Luis Tiago

struct NoSenha {
    int numero;
    NoSenha* proximo;
};

struct FilaSenha {
    NoSenha* frente;
    NoSenha* tras;
    int quantidade;
};

FilaSenha* criarFilaSenha() {
    FilaSenha* f = new FilaSenha();
    f->frente = nullptr;
    f->tras = nullptr;
    f->quantidade = 0;
    return f;
}

bool filaSenhaVazia(FilaSenha* f) {
    return (f->frente == nullptr);
}

void enfileirarSenha(FilaSenha* f, int num) {
    NoSenha* novo = new NoSenha();
    novo->numero = num;
    novo->proximo = nullptr;
    
    if (filaSenhaVazia(f)) {
        f->frente = novo;
    } else {
        f->tras->proximo = novo;
    }
    f->tras = novo;
    f->quantidade++;
}

int desenfileirarSenha(FilaSenha* f) {
    if (filaSenhaVazia(f)) return -1;
    
    NoSenha* aux = f->frente;
    int num = aux->numero;
    f->frente = f->frente->proximo;
    
    if (f->frente == nullptr) {
        f->tras = nullptr;
    }
    
    delete aux;
    f->quantidade--;
    return num;
}

void listarFilaSenha(FilaSenha* f) {
    if (filaSenhaVazia(f)) {
        cout << "Nenhuma senha registrada neste guichê ainda.\n";
        return;
    }
    NoSenha* atual = f->frente;
    while (atual != nullptr) {
        cout << "[Senha #" << atual->numero << "] ";
        atual = atual->proximo;
    }
    cout << "\n";
}

struct Guiche {
    int id;
    FilaSenha* senhasAtendidas;
    Guiche* proximo;
};

struct ListaGuiches {
    Guiche* inicio;
    int quantidade;
};

ListaGuiches* criarListaGuiches() {
    ListaGuiches* l = new ListaGuiches();
    l->inicio = nullptr;
    l->quantidade = 0;
    return l;
}

Guiche* buscarGuiche(ListaGuiches* l, int idBuscado) {
    Guiche* atual = l->inicio;
    while (atual != nullptr) {
        if (atual->id == idBuscado) {
            return atual;
        }
        atual = atual->proximo;
    }
    return nullptr;
}

void adicionarGuiche(ListaGuiches* l, int idNovo) {
    if (buscarGuiche(l, idNovo) != nullptr) {
        cout << "Erro: Já existe um guichê aberto com o ID " << idNovo << ".\n";
        return;
    }
    
    Guiche* novo = new Guiche();
    novo->id = idNovo;
    novo->senhasAtendidas = criarFilaSenha();
    novo->proximo = l->inicio;
    l->inicio = novo;
    l->quantidade++;
    cout << "Guichê " << idNovo << " aberto com sucesso!\n";
}

int lerInteiro(string mensagem) {
    string entrada;
    int numero;
    while (true) {
        cout << mensagem;
        getline(cin, entrada);
        stringstream ss(entrada);
        if (ss >> numero && ss.eof()) {
            return numero;
        }
        cout << "Entrada inválida! Digite apenas números inteiros.\n";
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese_Brazil.1252");
    
    FilaSenha* senhasGeradas = criarFilaSenha();
    ListaGuiches* listaGuiches = criarListaGuiches();
    
    int controleSenhas = 0;
    int totalAtendidosGeral = 0;
    int opcao = -1;
    
    while (true) {
        cout << "\n==================================================\n";
        cout << "              SISTEMA DE ATENDIMENTO 2.0          \n";
        cout << "==================================================\n";
        cout << " Senhas aguardando: " << senhasGeradas->quantidade << " | Guichês abertos: " << listaGuiches->quantidade << "\n";
        cout << "--------------------------------------------------\n";
        cout << "0. Sair\n";
        cout << "1. Gerar senha\n";
        cout << "2. Abrir guichê\n";
        cout << "3. Realizar atendimento\n";
        cout << "4. Listar senhas atendidas por guichê\n";
        cout << "--------------------------------------------------\n";
        
        opcao = lerInteiro("Escolha uma opção: ");
        cout << "==================================================\n\n";
        
        if (opcao == 0) {
            if (!filaSenhaVazia(senhasGeradas)) {
                cout << "Aviso: O programa não pode ser fechado enquanto houver senhas aguardando na fila!\n";
                continue;
            }
            break;
        }
        
        switch (opcao) {
            case 1: {
                controleSenhas++;
                enfileirarSenha(senhasGeradas, controleSenhas);
                cout << "Sucesso: Senha #" << controleSenhas << " gerada e colocada na fila.\n";
                break;
            }
            case 2: {
                int id = lerInteiro("Digite o ID (número) do guichê que deseja abrir: ");
                adicionarGuiche(listaGuiches, id);
                break;
            }
            case 3: {
                if (filaSenhaVazia(senhasGeradas)) {
                    cout << "Não há nenhuma senha na fila para ser atendida.\n";
                    break;
                }
                if (listaGuiches->quantidade == 0) {
                    cout << "Não há guichês abertos para realizar o atendimento. Abra um guichê primeiro.\n";
                    break;
                }
                
                int id = lerInteiro("Digite o ID do guichê que vai realizar o atendimento: ");
                Guiche* g = buscarGuiche(listaGuiches, id);
                
                if (g == nullptr) {
                    cout << "Erro: Guichê " << id << " não foi encontrado ou não está aberto.\n";
                } else {
                    int senhaChamada = desenfileirarSenha(senhasGeradas);
                    enfileirarSenha(g->senhasAtendidas, senhaChamada);
                    totalAtendidosGeral++;
                    cout << ">> Guichê " << id << " chamando a Senha #" << senhaChamada << " <<\n";
                }
                break;
            }
            case 4: {
                if (listaGuiches->quantidade == 0) {
                    cout << "Nenhum guichê cadastrado no sistema ainda.\n";
                    break;
                }
                
                int id = lerInteiro("Digite o ID do guichê que deseja consultar: ");
                Guiche* g = buscarGuiche(listaGuiches, id);
                
                if (g == nullptr) {
                    cout << "Erro: Guichê " << id << " não foi encontrado.\n";
                } else {
                    cout << "Senhas atendidas pelo Guichê " << id << ":\n";
                    listarFilaSenha(g->senhasAtendidas);
                }
                break;
            }
            default:
                cout << "Opção inválida! Escolha um número de 0 a 4.\n";
                break;
        }
    }
    
    cout << "\nEncerrando o sistema...\n";
    cout << "Total de senhas atendidas nesta sessão: " << totalAtendidosGeral << "\n";
    cout << "Obrigado por usar o sistema!\n";
    
    Guiche* gAtual = listaGuiches->inicio;
    while (gAtual != nullptr) {
        Guiche* gAux = gAtual;
        gAtual = gAtual->proximo;
        
        while(!filaSenhaVazia(gAux->senhasAtendidas)) {
            desenfileirarSenha(gAux->senhasAtendidas);
        }
        delete gAux->senhasAtendidas;
        delete gAux;
    }
    delete listaGuiches;
    delete senhasGeradas;
    
    return 0;
}
