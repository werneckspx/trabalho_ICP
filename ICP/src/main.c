#include <stdio.h>
#include "fila.h"
#include "string.h"


struct Caixa {
    int id;
    bool estado;
    Fila fila;
};

void cadastras_client(Cliente *c) {
    char buffer[100];  

    printf("Digite o nome do cliente: ");
    fgets(c->nome, 101, stdin);
    c->nome[strcspn(c->nome, "\n")] = '\0'; 

    while (1) {
        printf("Digite o CPF (11 dígitos): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        char cpf[12] = {0};
        int j = 0;
        for (int i = 0; buffer[i] && j < 11; i++) {
            if (isdigit(buffer[i])) {
                cpf[j++] = buffer[i];
            }
        }

        if (strlen(cpf) == 11) {
            strcpy(c->cpf, cpf);
            break;
        } else {
            printf("Erro: CPF inválido. Deve conter 11 dígitos.\n");
        }
    }

    while (1) {
        printf("Digite a prioridade (1 = alta, 2 = média, 3 = baixa): ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            int prioridade;
            if (sscanf(buffer, "%d", &prioridade) == 1) {
                if (prioridade == 1 || prioridade == 2 || prioridade == 3) {
                    c->prioridade = prioridade;
                    break;
                }
            }
        }
        printf("Erro: Prioridade invalida.\n");
    }

    while (1) {
        printf("Digite a quantidade de itens: ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &c->itens) == 1) {
                break;
            }
        }
        printf("Erro: Quantidade de itens inválida.\n");
    }
}

void atender_clinte(Fila *fila){
    if(isVazia(fila)){
        printf("Fila está vazia.\n");
        return;
    }
    printf("Atendendo cliente:\n");
    Cliente atendido;
    Desenfileira(fila, &atendido);
    printf("Atendido: Nome: %s, CPF: %s, Prioridade: %d, Itens: %d\n\n",
           atendido.nome, atendido.cpf, atendido.prioridade, atendido.itens);
}

void fechar_caixa(struct Caixa caixas[], int num_caixas, int id_caixa) {
    // Verifica se o ID do caixa é válido
    if (id_caixa >= num_caixas || id_caixa < 0) {
        printf("Erro: Caixa %d inválido.\n", id_caixa);
        return;
    }

    struct Caixa *caixa_fechar = &caixas[id_caixa];

    // Verifica se o caixa já está fechado
    if (!caixa_fechar->estado) {
        printf("Caixa %d já está fechado.\n", id_caixa);
        return;
    }

    // Verifica se há outro caixa aberto
    bool existe_caixa_aberto = false;
    for (int i = 0; i < num_caixas; i++) {
        if (caixas[i].estado && caixas[i].id != id_caixa) {
            existe_caixa_aberto = true;
            break;
        }
    }

    // Se não houver outro caixa aberto e ainda há clientes na fila, impede o fechamento
    if (!existe_caixa_aberto && !isVazia(&caixa_fechar->fila)) {
        printf("Erro: Caixa %d não pode ser fechado. Não há outros caixas abertos e a fila ainda possui clientes.\n", id_caixa);
        return;
    }

    // Realoca os clientes para outros caixas abertos
    while (!isVazia(&caixa_fechar->fila)) {
        Cliente cliente;
        Desenfileira(&caixa_fechar->fila, &cliente);

        // Encontra o próximo caixa aberto para realocar o cliente
        for (int i = 0; i < num_caixas; i++) {
            if (caixas[i].estado && caixas[i].id != id_caixa) {
                Enfileira_prioridade(&caixas[i].fila, cliente);
                printf("Cliente %s (CPF: %s) realocado do Caixa %d para o Caixa %d.\n",
                       cliente.nome, cliente.cpf, id_caixa, caixas[i].id);
                break;
            }
        }
    }

    caixa_fechar->estado = false;
    printf("Caixa %d foi fechado com sucesso.\n", id_caixa);
}

void abrir_caixa(struct Caixa caixas[], int num_caixas, int id_caixa){
    
    if (id_caixa >= num_caixas || id_caixa < 0) {
        printf("Erro: Caixa %d inválido.\n", id_caixa);
        return;
    }

    if (caixas[id_caixa].estado) {
        printf("Caixa %d já está aberto.\n", id_caixa);
        return;
    }else
        caixas[id_caixa].estado = true;

}

void estado_caixas(struct Caixa caixas[], int num_caixas){
    for (int i = 0; i < num_caixas; i++)
    {
        printf("Caixa %d:\n", caixas[i].id);
        if(caixas[i].estado == true){
            printf("Estado: Aberto\n");
        }else  
            printf("Estado: Fechado\n");
        int num_clientes_na_fila = Tamanho(&caixas[i].fila);
        printf("Clientes na fila: %d\n", num_clientes_na_fila);
        
        printf("\n");
    }
    
}

void menu(){
    printf("Digite a opção que deseja realizar: ");
    printf("\n1 - Cadastrar cliente\n");
    printf("2 - Atender cliente\n");
    printf("3 - Fechar caixa\n");
    printf("4 - Abrir caixa\n");
    printf("5 - Lista de clientes em espera\n");
    printf("6 - Listar o estado dos caixas\n");
    printf("0 - Sair\n");
}

int main() {
    struct Caixa caixas[5]; 
    Cliente c;
    int escolha;

    for (int i = 0; i < 5; i++) {
        caixas[i].id = i ;
        caixas[i].estado = true; 
        Vazia(&caixas[i].fila);
    }
    
    do
    {
        menu();
        scanf("%d", &escolha);
        getchar(); 
        int id_caixa;
        char aux_c;
        switch (escolha)
        {
        case 1:
            printf("Em qual caixa esse cliente sera cadastrado? ");
            if (scanf("%d%c", &id_caixa, &aux_c) != 2 || aux_c != '\n' || id_caixa >= 5 || id_caixa < 0) {
                printf("Caixa invalido\n");
                while (getchar() != '\n');
                break;
            }
            if (caixas[id_caixa].estado)
            {
                cadastras_client(&c);
                Enfileira_prioridade(&caixas[id_caixa].fila,c);
            }else
                printf("Caixa fechado.\n");
            
            break;
        case 2:
            printf("Em qual caixa esse cliente sera atendido? ");
            if (scanf("%d%c", &id_caixa, &aux_c) != 2 || aux_c != '\n' || id_caixa >= 5 || id_caixa < 0) {
                printf("Caixa invalido\n");
                while (getchar() != '\n');
                break;
            }
            atender_clinte(&caixas[id_caixa].fila);
            break;
        case 3:
            printf("Qual caixa sera fechado? ");
            if (scanf("%d%c", &id_caixa, &aux_c) != 2 || aux_c != '\n' || id_caixa >= 5 || id_caixa < 0) {
                printf("Caixa invalido\n");
                while (getchar() != '\n');
                break;
            }
            fechar_caixa(caixas,5,id_caixa);
            break;
        case 4:
            printf("Qual caixa sera aberto? ");
            if (scanf("%d%c", &id_caixa, &aux_c) != 2 || aux_c != '\n' || id_caixa >= 5 || id_caixa < 0) {
                printf("Caixa invalido\n");
                while (getchar() != '\n');
                break;
            }
            abrir_caixa(caixas,5,id_caixa);
            break;
        case 5:
            for (int i = 0; i < 5; i++) {
                printf("Fila da Caixa %d:\n", caixas[i].id);
                Imprime(&caixas[i].fila);
                printf("\n");
            }
            break;
        case 6:
            estado_caixas(caixas,5);
            break;
        case 0:
            break;
        default:
            printf("Valor invalido.\n");
            break;
        }
    } while (escolha != 0);

    return 0;
}
