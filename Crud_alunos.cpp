#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <iomanip>
using namespace std;

#if defined _WIN32 || defined _WIN64

// Limpar tela

void limpa(void) {
    HANDLE TELA;
    DWORD ESCRITA = 0;
    COORD POS;
    TELA = GetStdHandle(STD_OUTPUT_HANDLE);
    POS.X = 0;
    POS.Y = 0;
    FillConsoleOutputCharacter(TELA, 32, 100 * 100, POS, &ESCRITA);
}


// Posicionar cursor nos limites 80 x 24

void position(int LINHA, int COLUNA) {
    if (COLUNA >= 1 and COLUNA <= 80 and LINHA >= 1 and LINHA <= 24) {
        HANDLE TELA;
        COORD POS;
        TELA = GetStdHandle(STD_OUTPUT_HANDLE);
        POS.X = COLUNA - 1;
        POS.Y = LINHA - 1;
        SetConsoleCursorPosition(TELA, POS);
    }
}


// Limpar linha a partir da posicao do cursor
void clearline(void) {
    HANDLE TELA;
    COORD POS;
    CONSOLE_SCREEN_BUFFER_INFO VIDEO;
    DWORD ESCRITA = 0;
    TELA = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(TELA, &VIDEO);
    POS.Y = VIDEO.dwCursorPosition.Y;
    POS.X = VIDEO.dwCursorPosition.X;
    FillConsoleOutputCharacter(TELA, 32, 80 - POS.X, POS, &ESCRITA);
}

#else

// ACOES EXCLUSIVAS PARA MODO: TERMINAL ANSI

// LIMPAR TELA

void limpa(void) {
    cout << "\033[2J";
}

// LIMPAR LINHA A PARTIR DA POSICAO DO CURSOR

void position(int LINHA, int COLUNA) {
    if (COLUNA >= 1 && COLUNA <= 80 && LINHA >= 1 && LINHA <= 24)
        cout << "\033[" << LINHA << ";" << COLUNA << "H";
}

// LIMPAR LINHA A PARTIR DA POSICAO DO CURSOR

void clearline(void) {
    cout << "\033[K";
}

#endif


void lbuffer(void) {
    cin.ignore(100, '\n');
    cout << "\n";
}

void pause(void) {
    cin.get( );
    cout << "PRESSIONE QUALQUER TECLA PARA CONTINUAR.  .  .";
    cout << endl;
}

struct escola {
    char status = '0';
    int matricula;
    char nome[40];
    float nota[4];
    float media;
};

escola aluno;
int i, j;
int nmatricula;

void criarArquivo(void) {
    fstream cadastro;
    cadastro.open("arquivo.dat", ios_base::out | ios_base::binary);
    cadastro.close();
}

void tracalinha(string tipo) {
    limpa( );
    position(1, 1);
    for (i = 0; i <= 79; i++)
        cout << "-";
    cout << endl;
    position(2, 26);
    cout << "PROGRAMA DE CADASTRO ESCOLAR" << endl;
    position(3, 33 );
    cout << tipo;
    cout << endl;
    position(4, 1 );
    for (i = 0; i <= 79; i++)
        cout << "-";
}


void menu(void) {
    limpa( );
    string tipo;
    tipo = "Menu Principal";
    tracalinha(tipo);
    position( 8,26);
    cout << "Cadastrar .............: [1]";
    position(10,26);
    cout << "Consultar .............: [2]";
    position(12,26);
    cout << "Remover ...............: [3]";
    position(14,26);
    cout << "Alterar ...............: [4]";
    position(16,26);
    cout << "Finalizar .............: [5]";
    position(22,26);
    cout << "Entre com a opcao: --> ";
}

void tela(void) {
    position(10,17);
    clearline();
    position(11,17);
    clearline();
    position(12,17);
    clearline();
    position(13,17);
    clearline();
    position(14,17);
    clearline();
    position(15,17);
    clearline();
    position(10, 2);
    cout << "Matricula ...: ";
    position(11, 2);
    cout << "Nome ........: ";
    position(12, 2);
    cout << "1a. Nota ....: ";
    position(13, 2);
    cout << "2a. Nota ....: ";
    position(14, 2);
    cout << "3a. Nota ....: ";
    position(15, 2);
    cout << "4a. Nota ....: ";
    position(16, 2);
    cout << "Media........: ";
}

void mostradados(void) {
    float media = 0;
    cout << setprecision(2);
    cout << setiosflags(ios::right);
    cout << setiosflags(ios::fixed);
    position(10,17);
    cout << nmatricula;
    position(11,17);
    cout << aluno.nome;
    for (i = 0; i <= 3; i++) {
        position(12 + i,17);
        cout << aluno.nota[i];
        media += aluno.nota[i];
    }
    media = media / 4;
    position(16, 17);
    cout << media;

}

bool pesquisa(int nmatricula) {
    fstream cadastro("arquivo.dat", ios_base::in | ios_base::binary);
    bool acha = false;
    while ((not cadastro.eof()) and acha != true) {
        cadastro.read(reinterpret_cast<char*>(&aluno), sizeof(struct escola));
        if (nmatricula == aluno.matricula and aluno.status == '1')
            acha = true;
    }
    cadastro.flush();
    cadastro.close();
    return acha;
}

void cadastrar(void) {
    do {
        limpa( );
        string tipo = "Cadastro";
        tracalinha(tipo);
        position(6, 2);
        cout << "Digite os dados abaixo:";
        position(22,2);
        clearline();
        position(22, 2);
        cout << "Digite [0] para voltar ao menu: ";
        tela( );
        position(10, 17);
        cin >> nmatricula;
        lbuffer();
        if (nmatricula !=0) {
            if(pesquisa(nmatricula)) {
                mostradados();
                position(22, 1);
                clearline();
                position(23, 1);
                clearline();
                position(22, 2);
                cout << "Registro existente.";
                position(23, 2);
                cout << "Tecle <ENTER> para tentar novamente.";
                lbuffer();
            } else {
                fstream cadastro;
                float media = 0;
                cout << setprecision(2);
                cout << setiosflags(ios::right);
                cout << setiosflags(ios::fixed);
                cadastro.open("arquivo.dat", ios_base::out | ios_base::app |  ios_base::binary);
                position(11, 17);
                cin.getline(aluno.nome, sizeof(aluno.nome));
                aluno.media = 0;
                for (i = 0; i <= 3; i++) {
                    position(12 + i, 17);
                    cin >> aluno.nota[i];
                    aluno.nota[i];
                    lbuffer();
                    media += aluno.nota[i];
                }
                media = media / 4;
                position(16, 17);
                cout << media;
                aluno.status = '1';
                aluno.matricula = nmatricula;
                cadastro.seekp(0, ios::end);
                cadastro.write(reinterpret_cast<char*>(&aluno), sizeof(struct escola));
                position(22, 1);
                clearline();
                position(23, 1);
                clearline();
                position(22, 1);
                cout << "Registro cadastrado.";
                position(23, 1);
                cout << "Tecle <ENTER> para novo cadastro.";
                lbuffer();
                cadastro.flush();
                cadastro.close();
            }
        }
    } while (nmatricula != 0);
}

void consultar(void) {
    do {
        limpa( );
        string tipo = "Modulo de Consulta";
        tracalinha(tipo);
        position(6, 2);
        cout << "Digite os dados abaixo:";
        position(22,2);
        clearline();
        position(22, 2);
        cout << "Digite [0] para voltar ao menu: ";
        tela( );
        position(10, 17);
        cin >> nmatricula;
        lbuffer();
        if (nmatricula !=0) {
            if(pesquisa(nmatricula)) {
                mostradados();
                position(22, 1);
                clearline();
                position(23, 1);
                clearline();
                position(22, 2);
                cout << "Registro apresentado.";
                position(23, 2);
                cout << "Tecle <ENTER> para pesquisar novamente.";
                lbuffer();
            } else {
                position(22, 1);
                clearline();
                position(23, 1);
                clearline();
                position(22, 1);
                cout << "Registro nao cadastrado.";
                position(23, 1);
                cout << "Tecle <ENTER> para tentar novamente.";
                lbuffer();
            }
        }
    } while(nmatricula != 0);
}

int encontraPosicao(int numeromatricula) {
    fstream cadastro("arquivo.dat", ios_base::in | ios_base::binary);
    unsigned int pos = -1, contador = 0;
    bool acha = false;
    while (not(cadastro.eof()) and acha != true) {
        cadastro.read(reinterpret_cast<char*>(&aluno), sizeof(struct escola));
        if (nmatricula == aluno.matricula and aluno.status == '1') {
            acha = true;
            pos = contador;
        }
        contador++;
    }
    cadastro.flush();
    cadastro.close();
    return pos;
}

void excluirCadastro(int pos) {
    aluno.matricula = '0';
    fstream cadastro("arquivo.dat",ios_base::in | ios_base::out | ios_base::binary);
    cadastro.seekp(pos * sizeof(aluno));
    cadastro.write(reinterpret_cast<char*>(&aluno), sizeof(struct escola));
    cadastro.flush();
    cadastro.close();
}

void remover(void) {
    string resp;
    do {
        limpa( );
        string tipo = "Remover";
        tracalinha(tipo);
        position(6, 2);
        cout << "Digite os dados abaixo:";
        position(22,2);
        clearline();
        position(22, 2);
        cout << "Digite [0] para voltar ao menu: ";
        tela( );
        position(10, 17);
        cin >> nmatricula;
        lbuffer();
        if (nmatricula !=0) {
            if(pesquisa(nmatricula)) {
                mostradados();
                position(22, 1);
                clearline();
                position(23, 1);
                clearline();
                position(22, 2);
                cout << "Remove registro? [S/N]: ";
                cin >> resp;
                lbuffer();
                if (resp == "s" or resp == "S") {
                    excluirCadastro(encontraPosicao(nmatricula));
                    position(22, 1);
                    clearline();
                    position(23, 1);
                    clearline();
                    position(22, 2);
                    cout <<"Registro removido do arquivo.";
                    position(23, 2);
                    cout << "Tecle <Enter> para nova remocao. ";
                    lbuffer();
                }
            } else {
                position(22, 1);
                clearline();
                position(23, 1);
                clearline();
                position(22, 1);
                cout << "Registro nao cadastrado.";
                position(23, 1);
                cout << "Tecle <ENTER> para tentar novamente.";
                lbuffer();
            }
        }
    } while(nmatricula != 0);
}

void alteraNome(int pos) {
    cin.getline(aluno.nome, sizeof(aluno.nome));
    fstream cadastro("arquivo.dat",ios_base::in | ios_base::out | ios_base::binary);
    cadastro.seekp(pos * sizeof(aluno));
    cadastro.write(reinterpret_cast<char*>(&aluno), sizeof(struct escola));
    cadastro.flush();
    cadastro.close();
}

void alternome(void) {
    do {
        limpa( );
        string tipo = "Alteracao de nome";
        tracalinha(tipo);
        position(6, 2);
        cout << "Digite os dados abaixo:";
        position(22,2);
        clearline();
        position(22, 2);
        cout << "Digite [0] para voltar ao menu: ";
        tela( );
        position(10, 17);
        cin >> nmatricula;
        lbuffer();
        if (nmatricula !=0) {
            if(pesquisa(nmatricula)) {
                mostradados();
                position(17, 2);
                cout << "Novo nome ...: ";
                alteraNome(encontraPosicao(nmatricula));
                position(22, 1);
                clearline();
                position(23, 1);
                clearline();
                position(22, 2);
                cout <<"Nome atualizado no arquivo.";
                position(23, 2);
                cout << "Tecle <Enter> para nova alteracao. ";
                position(17, 16);
                clearline();
                lbuffer();
            } else {
                position(22, 1);
                clearline();
                position(23, 1);
                clearline();
                position(22, 1);
                cout << "Registro nao cadastrado.";
                position(23, 1);
                cout << "Tecle <ENTER> para tentar novamente.";
                position(23,38);
                lbuffer();
            }
        }
    } while(nmatricula != 0);
}

void alteraNota(int pos) {
    int i;
    position(17, 2);
    cout << "Qual nota ...: ";
    cin >> i;
    lbuffer();
    position(18, 2);
    cout << "Nova nota ...: ";
    cin >> aluno.nota[i - 1];
    fstream cadastro("arquivo.dat",ios_base::in | ios_base::out | ios_base::binary);
    cadastro.seekp(pos * sizeof(aluno));
    cadastro.write(reinterpret_cast<char*>(&aluno), sizeof(struct escola));
    cadastro.flush();
    cadastro.close();
    lbuffer();
}

void alternota(void) {
    do {
        limpa( );
        string tipo = "Alteracao de notas";
        tracalinha(tipo);
        position(6, 2);
        cout << "Digite os dados abaixo:";
        position(22,2);
        clearline();
        position(22, 2);
        cout << "Digite [0] para voltar ao menu: ";
        tela( );
        position(10, 17);
        cin >> nmatricula;
        lbuffer();
        if (nmatricula !=0) {
            if(pesquisa(nmatricula)) {
                mostradados();
                alteraNota(encontraPosicao(nmatricula));
                position(22, 1);
                clearline();
                position(23, 1);
                clearline();
                position(22, 2);
                cout <<"Nota atualizada no arquivo.";
                position(23, 2);
                cout << "Tecle <Enter> para nova alteracao. ";
                lbuffer();
            } else {
                position(22, 1);
                clearline();
                position(23, 1);
                clearline();
                position(22, 1);
                cout << "Registro nao cadastrado.";
                position(23, 1);
                cout << "Tecle <ENTER> para tentar novamente.";
                lbuffer();
            }
        }
    } while(nmatricula != 0);
}

void menusec(void) {

    int opcao;

    do {
        limpa( );
        string tipo = "Menu Alterar";
        tracalinha(tipo);
        position( 8,26);
        cout << "Nome ..................: [1]";
        position(10,26);
        cout << "Nota ..................: [2]";
        position(12,26);
        cout << "Volta Menu Principal ..: [3]";
        position(22,26);
        cout << "Entre com a opcao: --> ";
        cin >> opcao;
        lbuffer();
        switch(opcao) {
        case 1 :
            alternome();
            break;
        case 2 :
            alternota();
            break;
        }
    } while (opcao != 3);
}

int main() {
    int opcao;
    fstream cadastro("arquivo.dat", ios_base::in | ios_base::binary);
    if(cadastro.fail())
        criarArquivo();
    do {
        menu( );
        do {
            cin >> opcao;
            if (cin.fail()) {
                cout << "Entre apenas valores numericos...";
                cin.clear( );
                cout << endl;
                opcao = 0;
            }
            if (opcao <= 0 or opcao >= 6) {
                cout << "Entre valores entre 1 e 5...";
                lbuffer( );
                pause( );
                limpa( );
                menu( );
            }
        } while(opcao <= 0 or opcao >= 6 );
        switch(opcao) {
        case 1 :
            cadastrar();
            break;
        case 2 :
            consultar();
            break;
        case 3 :
            remover();
            break;
        case 4 :
            menusec();
            break;
        }
    } while (opcao != 5);
    cadastro.close();
    return 0;
}
