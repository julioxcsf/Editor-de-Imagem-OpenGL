#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include "OpenGL_Utils.h"
#include "Imagem.h"

#define numVAOs 1
#define numVBOs 3
#define CONSOLE_DEBUG   false
#define TRACEJANTE      true
#define PINCEL_GIZ1     false
#define PINCEL_GIZ2     false
#define ALTURA_TELA     1001
#define LARGURA_TELA    1001
#define TAMANHO_PINCEL  2

#define DX_TELA     1.0/LARGURA_TELA
#define DY_TELA     1.0/ALTURA_TELA
//se trocar a ordem, fica igual o fnaf (listrado)

using namespace std;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

//para geração de pixels
const float dx = 2.0f / (float(LARGURA_TELA) - 1);
const float dy = 2.0f / (float(ALTURA_TELA) - 1);
const int tamanho = LARGURA_TELA * ALTURA_TELA; // 2 para x e y
float* vertices = new float[tamanho * 2];
float antigo_xpos, antigo_ypos;
int contador_pos = 0;
float cor_pincel[4] = { 0.6,0.1,0.4,1.0 };


//variaveis do mouse
double xpos, ypos;
int state;

/* FUNCOES DE ESCRITA DE ARQUIVO */

// int funcao_escrever_RAW (2) 

// int funcao_ler_imagem (1) primeira

// int funcap_ler_RAW (2)

// int funcao_ler_PNG (1)

// int funcao_ler_JPG (1)

// int funcao_conversao_RAW_TIPO(jpg,png)

/* FUNCOES DE DESENHO */

// int funcao_display_matriz_pixels (3)                                                      OK
// com resolução(tamanho de cada pixel dependente da resolução)
// com uso de Vertex Shader e Fragment Shader 

/* FUNCOES DE INTERACOES */

// int funcao_zoom_scroll_mouse (5)

// int funcao_aplicar_cor_na_matriz_pixels (4)                                               OK

// int funcao_coletor_de_cor (5)

// int funcao_undo_reundo (6)

// deve haver uma barra delgada acima com um menu de opções:
// Arquivos; (7)
    // Carregar imagem
    // Salvar Imagem
    // Salvar imagem como
    // Fechar imagem

// Opções: (8)
    // Desativar fator "Aplha" - torna cada pixel com 3 bytes
    // Alterar cor do fundo
    // Alterar cor barra ferramentas
    // Tornar barra de cores móvel
    // Alterar uso de ferramentas:
        // Gerar circulo pelo: centro e Raio, 3 pontos, 

    // Alterar barra de opções:
        // Adicionar "alguma ferramenta" 

    // Alterar quantidade "ultimas cores"
    // Full Screen "F11"

// POS PROCESSAMENTO ****************** (99)

// Alterar tamanho da imagem (98)
// tamanho do pincel; (10)
// ultimas "5" cores; (8)
// ferramentas:
    // criar retas (9)
    // circulos (9)
    // reta curvada (10)
    // balde (preencher) (11)
    // Texto???? (12)

// Info da imagem:
    // Resolução
    // Tipo: (RGB ou RGBA)
    // nome: caso tenha sido salvo
    // extensão: .raw , .png , .jpg
    // Tamanho do arquivo.raw em bytes
    // Data de criação
    // Ultima modificação
    // Algum parametro estatístico??
    // Cor predominante ???
 //

// toda vez que a barra for clicada, a troca da cor deve ser feita evocando 
// o metodo de alteração do atrubuto r,g,b do objeto da Classe Pincel
// a trilha de arrasto deve conter pelo menos 255 pixels de comprimento 
// para poder alterar para todas as cores e deve haver um quadrado 
// com a demonstração da cor escolhida

//preciso criar uma barra de texto ou arrasto para escolher a cor RGB e aplha do pincel

void createConsole() {
    AllocConsole(); // Cria uma nova janela de console

    // Redireciona a saída padrão (stdout) para a nova janela de console
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
}

void pintar(GLFWwindow* window, float* dados) {
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    float r = 0.0;
    float g = 0.0;
    float b = 0.0;
    //float cor[4] = { r, 1.0f, b, 1.0f }; // Cor amarela
    // Captura a posição do cursor a qualquer momento
    glfwGetCursorPos(window, &xpos, &ypos);

    ypos = ALTURA_TELA - ypos;
    //cout << "Y: " << ypos << endl;

    if (state == GLFW_PRESS) {
        //cout << "Pressed" << endl;
        int indice;
        
        if (xpos >= 0 && xpos < LARGURA_TELA && ypos >= 0 && ypos < ALTURA_TELA) {
            if (TRACEJANTE && contador_pos > 0) {
                float pixelAlterados[LARGURA_TELA + ALTURA_TELA];
                int aux_x = xpos;
                int aux_y = ypos;
                int i = 0;
                float dif;

                if (xpos - antigo_xpos == 0) { // delta x = 0
                    dif = ypos - antigo_ypos;
                    if (dif < 0) dif *= -1;
                    for (int j = 0; j <= dif; j++) {
                        pixelAlterados[i] = antigo_xpos;
                        if (ypos > antigo_ypos) {
                            pixelAlterados[i + 1] = antigo_ypos + j;
                        }
                        else {
                            pixelAlterados[i + 1] = antigo_ypos - j;
                        }
                        i += 2;
                    }
                }

                else if (ypos - antigo_ypos == 0) { // delta y = 0
                    dif = xpos - antigo_xpos;
                    if (dif < 0) dif *= -1;
                    for (int j = 0; j <= dif; j++) {
                        if (xpos > antigo_xpos) {
                            pixelAlterados[i] = antigo_xpos + j;
                        }
                        else {
                            pixelAlterados[i] = antigo_xpos - j;
                        }
                        pixelAlterados[i + 1] = antigo_ypos;
                        i += 2;
                    }
                }

                else { // a = dy / dx
                    float a = (ypos - antigo_ypos) / (xpos - antigo_xpos);

                    i = 0;

                    int dx = abs(xpos - antigo_xpos);
                    int dy = abs(ypos - antigo_ypos);
                    int maior = max(dx, dy); // o número de passos é o maior deslocamento

                    for (int j = 0; j <= maior; j++) {
                        if (dx > dy) { // linha mais horizontal que vertical
                            if (xpos > antigo_xpos) {
                                pixelAlterados[i] = antigo_xpos + j;
                                pixelAlterados[i + 1] = antigo_ypos + a * j;
                            }
                            else {
                                pixelAlterados[i] = antigo_xpos - j;
                                pixelAlterados[i + 1] = antigo_ypos - a * j;
                            }
                        }
                        else { // linha mais vertical que horizontal
                            if (ypos > antigo_ypos) {
                                pixelAlterados[i] = antigo_xpos + j / a;
                                pixelAlterados[i + 1] = antigo_ypos + j;
                            }
                            else {
                                pixelAlterados[i] = antigo_xpos - j / a;
                                pixelAlterados[i + 1] = antigo_ypos - j;
                            }
                        }
                        i += 2;
                    }

                }

                antigo_xpos = aux_x; antigo_ypos = aux_y;
                int contador = i;
                i = 0;
                while (i < contador) { // o codigo dificilmente será mais otimizado que isso... preciso de threads
                    for (int y = -1 * TAMANHO_PINCEL / 2; y <= TAMANHO_PINCEL / 2; y++) {
                        for (int x = -1 * TAMANHO_PINCEL / 2; x <= TAMANHO_PINCEL / 2; x++) {
                            //cout << "x,y: " << x << "," << y << endl;
                            int linha = (int)pixelAlterados[i + 1] + y;
                            int coluna = (int)pixelAlterados[i + 0] + x;


                            indice = coluna + LARGURA_TELA * linha;

                            //o programa quebra se eu desenhar alem dos limites superiores e inferiores
                            if (indice < 0 || indice >= LARGURA_TELA * ALTURA_TELA * 4) break; // solucao

                            /*r = linha * DY_TELA;
                            b = 1.0f - linha * DX_TELA;
                            float cor[4] = { r, g, b, 1.0f };*/

                            /*dados[indice * 4 + 0] = cor[0];
                            dados[indice * 4 + 1] = cor[1];
                            dados[indice * 4 + 2] = cor[2];
                            dados[indice * 4 + 3] = cor[3];*/

                            dados[indice * 4 + 0] = cor_pincel[0];
                            dados[indice * 4 + 1] = cor_pincel[1];
                            dados[indice * 4 + 2] = cor_pincel[2];
                            dados[indice * 4 + 3] = cor_pincel[3];

                            // APLICACAO DA COR
                            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
                            glBufferSubData(GL_ARRAY_BUFFER, indice * 4 * sizeof(float), 4 * sizeof(float), cor_pincel);
                            //}
                        }
                    }
                    i += 2;
                }
            }
            else if (PINCEL_GIZ1 && contador_pos > 0) {
                //cout << "vetor linha " << xpos << "," << ypos << "," << antigo_xpos << "," << antigo_ypos << endl;
                float pixelAlterados[LARGURA_TELA + ALTURA_TELA];
                int aux_x = xpos;
                int aux_y = ypos;
                int i = 0;
                float dif;

                /*if (xpos - antigo_xpos == 0 && ypos - antigo_ypos == 0) {
                    contador_pos = 0;
                }*/

                if (xpos - antigo_xpos == 0) { // delta x = 0
                    dif = ypos - antigo_ypos;
                    if (dif < 0) dif *= -1;
                    for (int j = 0; j <= dif; j++) {
                        pixelAlterados[i] = antigo_xpos;
                        if (ypos > antigo_ypos) {
                            pixelAlterados[i + 1] = antigo_ypos + j;
                        }
                        else {
                            pixelAlterados[i + 1] = antigo_ypos - j;
                        }
                        i += 2;
                    }
                }

                else if (ypos - antigo_ypos == 0) { // delta y = 0
                    dif = xpos - antigo_xpos;
                    if (dif < 0) dif *= -1;
                    for (int j = 0; j <= dif; j++) {
                        if (xpos > antigo_xpos) {
                            pixelAlterados[i] = antigo_xpos + j;
                        }
                        else {
                            pixelAlterados[i] = antigo_xpos - j;
                        }
                        pixelAlterados[i + 1] = antigo_ypos;
                        i += 2;
                    }
                }

                else { // a = dy / dx
                    float a = (ypos - antigo_ypos) / (xpos - antigo_xpos);

                    i = 0;

                    int dx = abs(xpos - antigo_xpos);
                    int dy = abs(ypos - antigo_ypos);
                    int maior = max(dx, dy); // o número de passos é o maior deslocamento

                    for (int j = 0; j <= maior; j++) {
                        if (dx > dy) { // linha mais horizontal que vertical
                            if (xpos > antigo_xpos) {
                                pixelAlterados[i] = antigo_xpos + j;
                                pixelAlterados[i + 1] = antigo_ypos + a * j;
                            }
                            else {
                                pixelAlterados[i] = antigo_xpos - j;
                                pixelAlterados[i + 1] = antigo_ypos - a * j;
                            }
                        }
                        else { // linha mais vertical que horizontal
                            if (ypos > antigo_ypos) {
                                pixelAlterados[i] = antigo_xpos + j / a;
                                pixelAlterados[i + 1] = antigo_ypos + j;
                            }
                            else {
                                pixelAlterados[i] = antigo_xpos - j / a;
                                pixelAlterados[i + 1] = antigo_ypos - j;
                            }
                        }
                        i += 2;
                    }

                }

                antigo_xpos = aux_x; antigo_ypos = aux_y;
                int contador = i;
                i = 0;
                int sinal = 1;
                while (i < contador) {
                    int delta_linha = 0;
                    int delta_coluna = 0;
                    for (int j = 0; j < TAMANHO_PINCEL * TAMANHO_PINCEL; j++) {
                        if (j % 2 == 0) {
                            sinal *= -1;
                        }
                        int linha = (int)pixelAlterados[i + 1] + delta_linha * sinal;
                        int coluna = (int)pixelAlterados[i + 0] + delta_coluna * sinal;
                        //cout << " j:" << j;
                        i += 2;
                        indice = coluna + LARGURA_TELA * (linha);
                        //cout << " indice:" << indice;

                        delta_linha++;
                        if (delta_linha == TAMANHO_PINCEL) {
                            delta_linha = 0;
                            delta_coluna++;
                        }

                        if (linha >= 0 && linha < LARGURA_TELA && coluna >= 0 && coluna < ALTURA_TELA) {  // Verifique os limites
                            indice = coluna + LARGURA_TELA * linha;

                            r = linha * (1.0f / ALTURA_TELA);
                            b = 1.0f - linha * (1.0f / LARGURA_TELA);
                            float cor[4] = { r, g, b, 1.0f };

                            dados[indice * 4 + 0] = cor[0];
                            dados[indice * 4 + 1] = cor[1];
                            dados[indice * 4 + 2] = cor[2];
                            dados[indice * 4 + 3] = cor[3];

                            // APLICACAO DA COR
                            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
                            glBufferSubData(GL_ARRAY_BUFFER, indice * 4 * sizeof(float), 4 * sizeof(float), cor);
                        }
                    }
                }
            }
            else if (PINCEL_GIZ2 && contador_pos > 0) {
                //cout << "vetor linha " << xpos << "," << ypos << "," << antigo_xpos << "," << antigo_ypos << endl;
                float pixelAlterados[LARGURA_TELA + ALTURA_TELA];
                int aux_x = xpos;
                int aux_y = ypos;
                int i = 0;
                float dif;

                /*if (xpos - antigo_xpos == 0 && ypos - antigo_ypos == 0) {
                    contador_pos = 0;
                }*/

                if (xpos - antigo_xpos == 0) { // delta x = 0
                    dif = ypos - antigo_ypos;
                    if (dif < 0) dif *= -1;
                    for (int j = 0; j <= dif; j++) {
                        pixelAlterados[i] = antigo_xpos;
                        if (ypos > antigo_ypos) {
                            pixelAlterados[i + 1] = antigo_ypos + j;
                        }
                        else {
                            pixelAlterados[i + 1] = antigo_ypos - j;
                        }
                        i += 2;
                    }
                }

                else if (ypos - antigo_ypos == 0) { // delta y = 0
                    dif = xpos - antigo_xpos;
                    if (dif < 0) dif *= -1;
                    for (int j = 0; j <= dif; j++) {
                        if (xpos > antigo_xpos) {
                            pixelAlterados[i] = antigo_xpos + j;
                        }
                        else {
                            pixelAlterados[i] = antigo_xpos - j;
                        }
                        pixelAlterados[i + 1] = antigo_ypos;
                        i += 2;
                    }
                }

                else { // a = dy / dx
                    float a = (ypos - antigo_ypos) / (xpos - antigo_xpos);

                    i = 0;

                    int dx = abs(xpos - antigo_xpos);
                    int dy = abs(ypos - antigo_ypos);
                    int maior = max(dx, dy); // o número de passos é o maior deslocamento

                    for (int j = 0; j <= maior; j++) {
                        if (dx > dy) { // linha mais horizontal que vertical
                            if (xpos > antigo_xpos) {
                                pixelAlterados[i] = antigo_xpos + j;
                                pixelAlterados[i + 1] = antigo_ypos + a * j;
                            }
                            else {
                                pixelAlterados[i] = antigo_xpos - j;
                                pixelAlterados[i + 1] = antigo_ypos - a * j;
                            }
                        }
                        else { // linha mais vertical que horizontal
                            if (ypos > antigo_ypos) {
                                pixelAlterados[i] = antigo_xpos + j / a;
                                pixelAlterados[i + 1] = antigo_ypos + j;
                            }
                            else {
                                pixelAlterados[i] = antigo_xpos - j / a;
                                pixelAlterados[i + 1] = antigo_ypos - j;
                            }
                        }
                        i += 2;
                    }

                }

                antigo_xpos = aux_x; antigo_ypos = aux_y;
                int contador = i;
                i = 0;

                while (i < contador) {
                    int delta_linha = 0;
                    int delta_coluna = 0;
                    int medida = 1;
                    int sinal = +1;
                    for (int j = 0; j < TAMANHO_PINCEL * TAMANHO_PINCEL; j++) {
                        int linha = (int)pixelAlterados[i + 1] + delta_linha * sinal;
                        int coluna = (int)pixelAlterados[i + 0] + delta_coluna * sinal;
                        //cout << " j:" << j;

                        /*if (j % 2 == 1) {
                            sinal *= -1;
                        }*/
                        sinal *= -1;


                        indice = coluna + LARGURA_TELA * (linha);
                        //cout << " indice:" << indice;

                        delta_linha++;
                        if (delta_linha == TAMANHO_PINCEL - 1) {//+1 isso que gera o crescimento do rastro pelo deslocamento
                            delta_linha = 1;
                            delta_coluna++;
                            medida++;
                            //sinal *= -1;
                        }

                        if (linha >= 0 && linha < LARGURA_TELA && coluna >= 0 && coluna < ALTURA_TELA) {  // Verifique os limites
                            indice = coluna + LARGURA_TELA * linha;

                            r = linha * (1.0f / ALTURA_TELA);
                            b = 1.0f - linha * (1.0f / LARGURA_TELA);
                            float cor[4] = { r, g, b, 1.0f };

                            dados[indice * 4 + 0] = cor[0];
                            dados[indice * 4 + 1] = cor[1];
                            dados[indice * 4 + 2] = cor[2];
                            dados[indice * 4 + 3] = cor[3];

                            // APLICACAO DA COR
                            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
                            glBufferSubData(GL_ARRAY_BUFFER, indice * 4 * sizeof(float), 4 * sizeof(float), cor);
                        }
                        i += 2;
                    }
                }
            }
            else {
                for (int dx = -TAMANHO_PINCEL / 2; dx <= TAMANHO_PINCEL / 2; ++dx) {
                    for (int dy = -TAMANHO_PINCEL / 2; dy <= TAMANHO_PINCEL / 2; ++dy) {
                        int linha = (int)(ypos)+dy; // / float(ALTURA_TESTE));
                        int coluna = (int)(xpos)+dx; // / float(LARGURA_TESTE));

                        if (linha >= 0 && linha < LARGURA_TELA && coluna >= 0 && coluna < ALTURA_TELA) {  // Verifique os limites
                            int indice = coluna + LARGURA_TELA * linha;

                            r = linha * (1.0 / (float)ALTURA_TELA);
                            //g = 1.0 - 0.5 / (float)ALTURA_TESTE - 0.5 / (float)LARGURA_TESTE;
                            b = 1 - linha * (1.0 / (float)LARGURA_TELA);

                            float cor[4] = { r, g, b, 1.0f };

                            antigo_xpos = xpos; antigo_ypos = ypos;
                            contador_pos = 1;

                            // Aplica a cor nos pixels dentro da área do pincel
                            dados[indice * 4 + 0] = cor[0];
                            dados[indice * 4 + 1] = cor[1];
                            dados[indice * 4 + 2] = cor[2];
                            dados[indice * 4 + 3] = cor[3];

                            // APLICACAO DA COR
                            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
                            glBufferSubData(GL_ARRAY_BUFFER, indice * 4 * sizeof(float), 4 * sizeof(float), cor);
                        }
                    }
                }
            }
        }
    }
    else { contador_pos = 0; }
}

/*void setConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}*/

void init(GLFWwindow* window, float* dados) {
    renderingProgram = createShaderProgram("vertexShader.glsl", "fragmentShader.glsl", CONSOLE_DEBUG);

    int i = 0;
    int j = 0;
    int indice = 0;
    float gradacao_cor_y = 0;// 1.0 / ALTURA_TESTE;
    float gradacao_cor_x = 0;// 1.0 / LARGURA_TESTE;
    for (int row = 0; row < ALTURA_TELA; ++row) {
        float y = -1.0f + row * dy; // y vai de -1.0 a 1.0
        for (int col = 0; col < LARGURA_TELA; ++col) {
            float x = -1.0f + col * dx; // x vai de -1.0 a 1.0
            vertices[i++] = x;// +dx;
            vertices[i++] = y;// +dy;

            //cout << "Indice: " << indice << endl;

            dados[indice++] = 1.0; // 226.0 / 255.0;
            //cout << "Dados depois de escrever: " << dados[indice - 1] << ",";
            dados[indice++] = 1.0; // 241.0 / 255.0;
            //cout << dados[indice - 1] << ",";
            dados[indice++] = 1.0; // 252.0 / 255.0;
            //cout << dados[indice - 1] << ",";
            dados[indice++] = 1.0f;
            //cout << dados[indice - 1] << endl;
        }
    }


    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(1, &vbo[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, tamanho * 2 * sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vbo[1]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, tamanho * 4 * sizeof(float), dados, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &vbo[2]); // esperando possivel reta ou outros

    //desativa VBO e VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //delete[] vertices;//futuramente eu precisarei deixar aqui..
    //delete[] pixelColor;// para alterar
}

void display(GLFWwindow* window, double currentTime, float* dados) {
    glUseProgram(renderingProgram);
    //glPointSize(10.0f);
    //glPointSize(3.0f);
    /*glDrawArrays(GL_POINTS, 0, 1);*/

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);
    glBindVertexArray(vao[0]);

    pintar(window, dados);

    //glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);
    if (contador_pos) {
        glDrawArrays(GL_LINES, 0, 2);
    }

    glDrawArrays(GL_POINTS, 0, LARGURA_TELA * ALTURA_TELA);
    
    //glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);
    glBindVertexArray(0);

}

void TESTE_ESCRITA() {
    cout << "Console window created." << endl;

    // Exemplo de mensagens com cores no console
    setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
    cout << "Mensagem vermelha no console." << endl;

    setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "Mensagem verde no console." << endl;

    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "Mensagem azul no console." << endl;

    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "Mensagem branca (intensa) no console." << endl;

    setConsoleColor(BACKGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "Texto com fundo vermelho e texto ciano no console." << endl;

    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    if (CONSOLE_DEBUG) {
        createConsole();
        //TESTE_ESCRITA();
    }

    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW." << endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Pega o monitor primário (o monitor principal do sistema)
    //GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    // Pega o modo de vídeo do monitor
    //const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // Cria uma janela em modo "janela cheia" usando as dimensões do monitor
    //GLFWwindow* window = glfwCreateWindow(mode->width, mode->height-60, "Editor de Imagem 0.7V", monitor, NULL);
    //glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    GLFWwindow* window = glfwCreateWindow(1000, 1000, "Editor de Imagem 0.7V", NULL, NULL);
    
    if (!window) {
        cerr << "Failed to create GLFW window." << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        cerr << "Failed to initialize GLEW." << endl;
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);
    Imagem imagem("teste", ".raw",  ALTURA_TELA, LARGURA_TELA);
    init(window, &imagem.dados[0]);//, imagem);

    //glfwSetCursorPosCallback(window, cursor_position_callback);

    while (!glfwWindowShouldClose(window)) {
        //cout << "3" << endl;
        display(window, glfwGetTime(), &imagem.dados[0]);
        glfwSwapBuffers(window);
        glfwPollEvents();
        //cursor_position_callback(window, xpos, ypos);
    }
    //imagem.imSave("imagem1", ".raw2");
    //imagem.imSave("imagem1", ".raw");
    //imagem.imSave("imagem1",".png");
    //imagem.imSave("imagem1", ".jpg");
    imagem.~Imagem();
    //glfwWaitEventsTimeout(3.0);
    delete[] vertices;
    //delete[] pixelColor;
    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}