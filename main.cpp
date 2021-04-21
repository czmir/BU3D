/**************************************************
  Universidade Federal da Grande Dourados - UFGD
  Trabalho Computação Gráfica - Batalha Urbana 3D
  Bianca Andréia, Fabio Amaral Godoy da Silveira
**************************************************/

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#define PI 3.14159265
#define G 9.81


typedef float color[3];

color vermelho     = {0.85, 0.12, 0.00};
color verde        = {0.00, 1.00, 0.00};
color azul         = {0.00, 0.15, 0.35};
color preto        = {0.00, 0.00, 0.00};
color branco       = {1.00, 1.00, 1.00};
color branco_gelo  = {0.88, 0.91, 0.89};
color amarelo      = {1.00, 1.00, 0.00};
color violeta      = {0.54, 0.17, 0.88};
color cinza        = {0.80, 0.80, 0.80};
color cinza_escuro = {0.67, 0.67, 0.67};
color laranja      = {1.00, 0.60, 0.20};

void texto();
void tiro_1();
void tiro_2();
void trajetoria_balistica_1();
void trajetoria_balistica_2();
void nova_rodada();

char buf_1[100] = {0};

float tam = 3.5f, Tempo = 0, R = 0, L = 0,
      r, g, b,                   // Para a cor do fundo.
      c1_cor = 1,                // Para a cor do Carrinho 1.
      c2_cor = 1,                // Para a cor do Carrinho 2.
      c1_1, c1_2, c2_1, c2_2;    // Coordenadas de spawn dos carrinhos.

int n1, n2, jogada = 0, rodada = 0, numero_aleatorio, ponto1 = 0, ponto2 = 0, ponto_1 = 0, ponto_2 = 0,
            Velocidade_1 = 30, Velocidade_2 = 30, // Velocidade inicial dos projeteis dos canhoes.
            angulo_1 = 85, angulo_2 = 85; // Angulo inicial dos canhoes.

bool bola1 = false, bola2 = false, colisao1 = false, colisao2 = false, vez1 = false, vez2 = false;


void Atualiza_Tamanho(int w, int h)
{
    glViewport(0, 0, w, h);
    float aspect = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(3*30.0, aspect, 0.1, 500.0);

    glMatrixMode(GL_MODELVIEW);
}

void desenha(float p1[3], float p2[3], float p3[3], float p4[3])
{
    glBegin(GL_QUADS);
    glVertex3fv(p1);
    glVertex3fv(p2);
    glVertex3fv(p3);
    glVertex3fv(p4);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3fv(p1);
    glVertex3fv(p2);
    glVertex3fv(p3);
    glVertex3fv(p4);
    glEnd();
}

void retangulo()
{
    float c = 4, // Comprimento
          l = 6, // Largura
          p = 4; // Pronfundidade

    float v1[3] = {-c, l,  p},
          v2[3] = {-c, 0,  p},
          v3[3] = { c, 0,  p},
          v4[3] = { c, l,  p};

    float v5[3] = { c, l, -p},
          v6[3] = { c, 0, -p},
          v7[3] = {-c, 0, -p},
          v8[3] = {-c, l, -p};

    //Face 1 - Frente
    desenha(v1,v2,v3,v4);

    //Face 2 - Direita
    desenha(v3,v4,v5,v6);

    //Face 3 - Tras
    desenha(v5,v6,v7,v8);

    //Face 4 - Esquerda
    desenha(v1,v2,v7,v8);

    //Face 5 - Topo
    desenha(v1,v4,v5,v8);

    //Face 6 - Baixo
    desenha(v2,v3,v6,v7);
}

void quadrado()
{
    float c = 4, // Comprimento
          l = 4, // Largura
          p = 4; // Pronfundidade

    float v1[3] = {-c, l,  p},
          v2[3] = {-c, 0,  p},
          v3[3] = { c, 0,  p},
          v4[3] = { c, l,  p};

    float v5[3] = { c, l, -p},
          v6[3] = { c, 0, -p},
          v7[3] = {-c, 0, -p},
          v8[3] = {-c, l, -p};

    //Face 1 - Frente
    desenha(v1,v2,v3,v4);

    //Face 2 - Direita
    desenha(v3,v4,v5,v6);

    //Face 3 - Tras
    desenha(v5,v6,v7,v8);

    //Face 4 - Esquerda
    desenha(v1,v2,v7,v8);

    //Face 5 - Topo
    desenha(v1,v4,v5,v8);

    //Face 6 - Baixo
    desenha(v2,v3,v6,v7);
}

void eixo()
{
    float teta = 0.0f, teta2 = 0.1, raio = 1.0;

    glBegin(GL_POLYGON);
    while(teta < 360.0)
    {
        glVertex2f(1.5*cos(teta/180.0 * PI),1.5*sin(teta/180.0 * PI));
        teta += 0.1;
    }
    glEnd();

    glBegin(GL_LINES);
    while(teta2 < 360.0)
    {
        raio = 1.5;
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f((cos(teta2) * raio),(sin(teta2) * raio));
        raio = 1.2;

        glVertex2f((cos(teta2) * raio),(sin(teta2) * raio));
        teta2 +=1.0;
    }
    glEnd();
}

void cilindro()
{
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    gluCylinder(quadratic, 1.0, 1.0, 10, 32, 32);
}

void desenha_carrinho_1()
{
    glPushMatrix();
        glColor3f(0.5,0.5,0.5);
        glTranslatef(1,1,-10);
        retangulo();
    glPopMatrix();
}

void spawn_carrinho_1()
{
    glPushMatrix();

    switch(n1)
    {
    case 0:
        // Posicao 1.
        glTranslatef(-123,-16,0.0);
        c1_1 = -123;
        c1_2 = -16;
        desenha_carrinho_1();
        break;

    case 1:
        // Posicao 2.
        glTranslatef(-90,7,0.0);
        c1_1 = -90;
        c1_2 = 7;
        desenha_carrinho_1();
        break;

    case 2:
        // Posicao 3.
        glTranslatef(-17,-16,0.0);
        c1_1 = -17;
        c1_2 = -16;
        desenha_carrinho_1();
        break;

    case 3:
        // Posicao 4.
        glTranslatef(150,11,0.0);
        c1_1 = 150;
        c1_2 = 11;
        desenha_carrinho_1();
        break;
    }
    glPopMatrix();
}

void spawn_carrinho_2()
{
    glPushMatrix();

    switch(n2)
    {
    case 0:
        // Posicao 1.
        glTranslatef(-180,7.7,0.0);
        c2_1 = -180;
        c2_2 = 7.7;
        //desenha_carrinho_2();
        break;

    case 1:
        // Posicao 2.
        glTranslatef(60,7.7,0.0);
        c2_1 = 60;
        c2_2 = 7.7;
        //desenha_carrinho_2();
        break;

    case 2:
        // Posicao 3.
        glTranslatef(120,-16,0.0);
        c2_1 = 120;
        c2_2 = -16;
        //desenha_carrinho_2();
        break;

    case 3:
        // Posicao 4.
        glTranslatef(180,7.7,0.0);
        c2_1 = 180;
        c2_2 = 7.7;
        //desenha_carrinho_2();
        break;
    }
    glPopMatrix();
}

void Atualiza_Desenho(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslated(0.0,0.0,-20.0);
    glRotated(R,1,0,0);
    glRotated(L,0,1,0);

    glColor3f(0.5,0.5,0.5);
    //retangulo();
    desenha_carrinho_1();

    glutPostRedisplay();
    glutSwapBuffers();
}

void Teclado( unsigned char tecla, int x, int y)
{
    switch (tecla)
    {
    case 27 :
    case 'q':
        exit(0);
        break;
    case 'r':
        R+=1;
        break;
    case 't':
        L+=1;
        break;
    case 'y':
        R-=1;
        break;
    case 'u':
        L-=1;
        break;
    }
}

void nova_rodada()
{
    srand(time(NULL));
    // Gera numeros aleatorios de 0 a 3, para definir as novas posicoes dos carrinhos.
    n1 = rand()%4;
    n2 = rand()%4;

    ponto1 = 0;
    ponto2 = 0;
    jogada = 0;

    numero_aleatorio = rand()%3;

    if(numero_aleatorio == 0)   // Se o numero e 0, o fundo e verde.
    {
        r = 0.0;
        g = 0.5;
        b = 0.5;
    }
    if(numero_aleatorio == 1)   // Se o número e 1, o fundo e roxo.
    {
        r = 0.5;
        g = 0.5;
        b = 1.0;
    }
    if(numero_aleatorio == 2)   // Se o numero e 2, o fundo e amarelo.
    {
        r = 0.5;
        g = 0.5;
        b = 0.0;
    }
    glClearColor(r, g, b, 0);

    c1_cor = 1;
    c2_cor = 1;

    vez1 = false;
    vez2 = false;

    rodada++;
}

void vez_jogada()
{
    jogada++;   // Incrementa o número da jogada.
}

void tempo(int value)
{
    Tempo += 0.1;   // O tempo para utilizar na equacao de lancamento oblíquo.
    glutPostRedisplay();
}

void LeTeclado(unsigned char tecla, int x, int y)
{
    switch (tecla)
    {
    case'q':
        // Sai do jogo.
        exit(0);
        break;

    case 'b':
        // Novo jogo.
        nova_rodada();
        ponto_1 = 0;
        ponto_2 = 0;
        rodada = 0;
        glLoadIdentity();
        glutPostRedisplay();
    }

    if(jogada%2 == 0)   // Se a jogada for par, e a vez do Player 1.
    {
        switch (tecla)
        {

//########################## CONTROLES PLAYER 1 ##########################
        // Movimenta o cano do canhao 1.
        case 'a':
            if(vez1 == false)   // Condicao para bloquear os controles do carrinho 1 enquanto o projetil estiver no ar.
            {
                // Incrementa em 1 o angulo se nao estiver no limite.
                if(angulo_1 <= 87) angulo_1 = angulo_1 + 3;
                glutPostRedisplay();
            }
            break;

        case 'd':
            if(vez1 == false)   // Condicao para bloquear os controles do carrinho 1 enquanto o projetil estiver no ar.
            {
                // Decrementa em 1 o angulo se nao estiver no limite.
                if(angulo_1 >= -87) angulo_1 = angulo_1 - 3;
                glutPostRedisplay();
            }
            break;

        case 'w':
            if(vez1 == false)   // Condicao para bloquear os controles do carrinho 1 enquanto o projetil estiver no ar.
            {
                // Incrementa em 1 a velocidade do canhao 1.
                if(Velocidade_1 <= 199) Velocidade_1 = Velocidade_1 + 1;
                glutPostRedisplay();
            }
            break;

        case 's':
            if(vez1 == false)   // Condicao para bloquear os controles do carrinho 1 enquanto o projetil estiver no ar.
            {
                // Decrementa em 1 a velocidade do canhao 1.
                if(Velocidade_1 >= 1) Velocidade_1 = Velocidade_1 - 1;
                glutPostRedisplay();
            }
            break;

        case 'f':
            // Atira com o canhao 1.
            vez1 = true;
            vez2 = false;
            bola1 = true;
            if(bola1)
            {
                glutPostRedisplay();
            }
            break;
        }
    }

    if(jogada%2 != 0)   // Se a jogada for impar, e a vez do Player 2.
    {
        switch (tecla)
        {
//########################## CONTROLES PLAYER 2 ##########################
        // Movimenta o cano do canhao 2.
        case 'j':
            if(vez2 == false) // Condicao para bloquear os controles do carrinho 2 enquanto o projetil estiver no ar.
            {
                // Incrementa em 1 o angulo se nao estiver no limite.
                if(angulo_2 <= 87) angulo_2 = angulo_2 + 3;
                glutPostRedisplay();
            }
            break;

        case 'l':
            if(vez2 == false)   // Condicao para bloquear os controles do carrinho 2 enquanto o projetil estiver no ar.
            {
                // Decrementa em 1 o angulo se nao estiver no limite.
                if(angulo_2 >= -87) angulo_2 = angulo_2 - 3;
                glutPostRedisplay();
            }
            break;

        case 'i':
            if(vez2 == false)   // Condicao para bloquear os controles do carrinho 2 enquanto o projetil estiver no ar.
            {
                // Incrementa em 1 a velocidade do canhao 2.
                if(Velocidade_2 <= 199) Velocidade_2 = Velocidade_2 + 1;
                glutPostRedisplay();
            }
            break;

        case 'k':
            if(vez2 == false)   // Condicao para bloquear os controles do carrinho 2 enquanto o projetil estiver no ar.
            {
                // Decrementa em 1 a velocidade do canhao 2.
                if(Velocidade_2 >= 1) Velocidade_2 = Velocidade_2 - 1;
                glutPostRedisplay();
            }
            break;

        case 'h':
            // Atira com o canhao 2.
            vez1 = false;
            vez2 = true;
            bola2 = true;
            if(bola2)
            {
                glutPostRedisplay();
            }
            break;
        }
    }
}

void tiro_1()
{
    float Voy = Velocidade_1 * sin((angulo_1 + 90)/180.0 * PI),
          Vox = Velocidade_1 * cos((angulo_1 + 90)/180.0 * PI),

          x = Vox * Tempo,
          y = Voy * Tempo - G * Tempo * Tempo / 2,

          posicao_x = 0.0 + x,
          posicao_y = 2.0 + y; // Posicao (x,y) do projetil.

    // Mostra a trajetoria do projetil durante o tiro.
    if(bola1 == true)
    {
        trajetoria_balistica_1();
    }

    // Limites para +x, -x e -y para a posicao do projetil.
    if(posicao_x >= -400.00 && posicao_y >= -50.00 && posicao_x <= 400.00)
    {
        glTranslatef(posicao_x, posicao_y, 0.0);
        eixo();
        glutTimerFunc(30, tempo, 0);

        // Verifica se acertou o carrinho 2.
        if(posicao_x+c1_1 >= c2_1-4 && posicao_x+c1_1 <= c2_1+4 && posicao_y+c1_2 >= c2_2-2 && posicao_y+c1_2 <= c2_2+6)
        {
            colisao1 = true;
            ponto1++;   // Pontuacao para alterar a cor do carrinho.
            ponto_1++;  // Pontuacao para mostrar na tela.
            bola1 = false;  // Sai da condicao de tiro.

            Tempo = 0;
            vez_jogada();   // Alterna jogada.

            if(ponto1 == 1)
            {
                c2_cor = 0.5; // O Carrinho 2 muda de cor ao ser atingido.
                srand(time(NULL));
                // Gera números aleatorios de 0 a 3, para definir as novas posicoes dos carrinhos.
                n1 = rand()%4;
                n2 = rand()%4;
            }

            if(ponto1 == 2)
            {
                c2_cor = 0.0; // O Carrinho 2 muda de cor ao ser atingido.
                srand(time(NULL));
                // Gera números aleatorios de 0 a 3, para definir as novas posicoes dos carrinhos.
                n1 = rand()%4;
                n2 = rand()%4;
            }

            if(ponto1 == 3)   // No terceiro ponto, depois de um tempo, inicia uma nova partida.
            {
                glutTimerFunc(1000, tempo, 0.1);
                glutTimerFunc(1000, tempo, 0.1);
                nova_rodada();
            }
        }

        if( (posicao_x+c1_1 >= -30-7          && posicao_x+c1_1 <= -30+7         && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
                (posicao_x+c1_1 >= -45-7          && posicao_x+c1_1 <= -45+7         && posicao_y+c1_2 >= -3-15         && posicao_y+c1_2 <= -3+15)     ||
                (posicao_x+c1_1 >= -75-7          && posicao_x+c1_1 <= -75+7         && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
                (posicao_x+c1_1 >= -90-7          && posicao_x+c1_1 <= -90+7         && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
                (posicao_x+c1_1 >= -105-7         && posicao_x+c1_1 <= -105+7        && posicao_y+c1_2 >= -12-15        && posicao_y+c1_2 <= -12+15)    ||
                (posicao_x+c1_1 >= -135-7         && posicao_x+c1_1 <= -135+7        && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
                (posicao_x+c1_1 >= -150-7         && posicao_x+c1_1 <= -150+7        && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
                (posicao_x+c1_1 >= -165-7         && posicao_x+c1_1 <= -165+7        && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
                (posicao_x+c1_1 >= -180-7         && posicao_x+c1_1 <= -180+7        && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
                (posicao_x+c1_1 >= -195-7         && posicao_x+c1_1 <= -195+7        && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
                (posicao_x+c1_1 >= -195-7         && posicao_x+c1_1 <= -195+7        && posicao_y+c1_2 >= 8.5-0         && posicao_y+c1_2 <= 8.5+7)     ||
                (posicao_x+c1_1 >= +15-7          && posicao_x+c1_1 <= +15+7         && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
                (posicao_x+c1_1 >= +30-7          && posicao_x+c1_1 <= +30+7         && posicao_y+c1_2 >= -8.5-15       && posicao_y+c1_2 <= -8.5+15)   ||
                (posicao_x+c1_1 >= +45-7          && posicao_x+c1_1 <= +45+7         && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
                (posicao_x+c1_1 >= 45-7           && posicao_x+c1_1 <= 45+7          && posicao_y+c1_2 >= 8.5-0         && posicao_y+c1_2 <= 8.5+7)     ||
                (posicao_x+c1_1 >= 60-7           && posicao_x+c1_1 <= 60+7          && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
                (posicao_x+c1_1 >= -60-7          && posicao_x+c1_1 <= -60+7         && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
                (posicao_x+c1_1 >= +75-7          && posicao_x+c1_1 <= +75+7         && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
                (posicao_x+c1_1 >= 90-7           && posicao_x+c1_1 <= 90+7          && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
                (posicao_x+c1_1 >= 90-7           && posicao_x+c1_1 <= 90+7          && posicao_y+c1_2 >= 8.5-0         && posicao_y+c1_2 <= 8.5+7)     ||
                (posicao_x+c1_1 >= +105-7         && posicao_x+c1_1 <= +105+7        && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
                (posicao_x+c1_1 >= +135-7         && posicao_x+c1_1 <= +135+7        && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
                (posicao_x+c1_1 >= +150-7         && posicao_x+c1_1 <= +150+7        && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
                (posicao_x+c1_1 >= +165-7         && posicao_x+c1_1 <= +165+7        && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
                (posicao_x+c1_1 >= +180-7         && posicao_x+c1_1 <= +180+7        && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
                (posicao_x+c1_1 >= +205-7         && posicao_x+c1_1 <= +205+7        && posicao_y+c1_2 >= -7.5-1500     && posicao_y+c1_2 <= -7.5+1500) ||
                (posicao_x+c1_1 >= -205-7         && posicao_x+c1_1 <= -205+7        && posicao_y+c1_2 >= -7.5-1500     && posicao_y+c1_2 <= -7.5+1500) ||
                (posicao_x+c1_1 >= +0-1000        && posicao_x+c1_1 <= +0+1000       && posicao_y+c1_2 >= -40-7         && posicao_y+c1_2 <= -40+24))
        {
            bola1 = false;  // Colidiu nos predios ou nas bordas laterais/inferior, entao sai da condicao de tiro.
            Tempo = 0;
            vez_jogada();   // Alterna jogada.
        }
    }
    else
    {
        bola1 = false;  // Sai da condicao de tiro.
        Tempo = 0;
        vez_jogada();   // Alterna jogada.
    }
}

void tiro_2()
{
    float Voy = Velocidade_2 * sin((angulo_2 + 90)/180.0 * PI),
          Vox = Velocidade_2 * cos((angulo_2 + 90)/180.0 * PI),

          x = Vox * Tempo,
          y = Voy * Tempo - G * Tempo * Tempo / 2,

          posicao_x = 0.0 + x,
          posicao_y = 2.0 + y; // Posicao (x,y) do projetil.

    if(bola2 == true)
    {
        // Mostra a trajetoria do projetil durante o tiro.
        trajetoria_balistica_2();
    }

    // Limites para +x, -x e -y para a posicao do projetil.
    if(posicao_x >= -400.00 && posicao_y >= -50.00 && posicao_x <= 400.00)
    {
        glTranslatef(posicao_x, posicao_y, 0.0);
        eixo();
        glutTimerFunc(30, tempo, 0);

        if(posicao_x+c2_1 >= c1_1-4 && posicao_x+c2_1 <= c1_1+4 && posicao_y+c2_2 >= c1_2-2 && posicao_y+c2_2 <= c1_2+6)
        {
            colisao2 = true;
            ponto2++;   // Pontuacao para alterar a cor do carrinho.
            ponto_2++;  // Pontuacao para mostrar na tela.
            bola2 = false;  // Sai da condicao de tiro.
            Tempo = 0;
            vez_jogada();    // Alterna jogada.

            if(ponto2 == 1)
            {
                c1_cor = 0.5; // O Carrinho 1 muda de cor ao ser atingido.
                srand(time(NULL));
                // Gera números aleatorios de 0 a 3, para definir as novas posicoes dos carrinhos.
                n1 = rand()%4;
                n2 = rand()%4;
            }

            if(ponto2 == 2)
            {
                c1_cor = 0.0; // O Carrinho 1 muda de cor ao ser atingido.
                srand(time(NULL));
                // Gera números aleatorios de 0 a 3, para definir as novas posicoes dos carrinhos.
                n1 = rand()%4;
                n2 = rand()%4;
            }

            if(ponto2 == 3)   // No terceiro ponto, depois de um tempo, inicia uma nova partida.
            {
                glutTimerFunc(1000, tempo, 0.1);
                glutTimerFunc(1000, tempo, 0.1);
                nova_rodada();
            }
        }

        if( (posicao_x+c2_1 >= -30-7          && posicao_x+c2_1 <= -30+7         && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
                (posicao_x+c2_1 >= -45-7          && posicao_x+c2_1 <= -45+7         && posicao_y+c2_2 >= -3-15         && posicao_y+c2_2 <= -3+15)     ||
                (posicao_x+c2_1 >= -75-7          && posicao_x+c2_1 <= -75+7         && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
                (posicao_x+c2_1 >= -90-7          && posicao_x+c2_1 <= -90+7         && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
                (posicao_x+c2_1 >= -105-7         && posicao_x+c2_1 <= -105+7        && posicao_y+c2_2 >= -12-15        && posicao_y+c2_2 <= -12+15)    ||
                (posicao_x+c2_1 >= -135-7         && posicao_x+c2_1 <= -135+7        && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
                (posicao_x+c2_1 >= -150-7         && posicao_x+c2_1 <= -150+7        && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
                (posicao_x+c2_1 >= -165-7         && posicao_x+c2_1 <= -165+7        && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
                (posicao_x+c2_1 >= -180-7         && posicao_x+c2_1 <= -180+7        && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
                (posicao_x+c2_1 >= -195-7         && posicao_x+c2_1 <= -195+7        && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
                (posicao_x+c2_1 >= -195-7         && posicao_x+c2_1 <= -195+7        && posicao_y+c2_2 >= 8.5-0         && posicao_y+c2_2 <= 8.5+7)     ||
                (posicao_x+c2_1 >= +15-7          && posicao_x+c2_1 <= +15+7         && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
                (posicao_x+c2_1 >= +30-7          && posicao_x+c2_1 <= +30+7         && posicao_y+c2_2 >= -8.5-15       && posicao_y+c2_2 <= -8.5+15)   ||
                (posicao_x+c2_1 >= +45-7          && posicao_x+c2_1 <= +45+7         && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
                (posicao_x+c2_1 >= 45-7           && posicao_x+c2_1 <= 45+7          && posicao_y+c2_2 >= 8.5-0         && posicao_y+c2_2 <= 8.5+7)     ||
                (posicao_x+c2_1 >= 60-7           && posicao_x+c2_1 <= 60+7          && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
                (posicao_x+c2_1 >= -60-7          && posicao_x+c2_1 <= -60+7         && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
                (posicao_x+c2_1 >= +75-7          && posicao_x+c2_1 <= +75+7         && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
                (posicao_x+c2_1 >= 90-7           && posicao_x+c2_1 <= 90+7          && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
                (posicao_x+c2_1 >= 90-7           && posicao_x+c2_1 <= 90+7          && posicao_y+c2_2 >= 8.5-0         && posicao_y+c2_2 <= 8.5+7)     ||
                (posicao_x+c2_1 >= +105-7         && posicao_x+c2_1 <= +105+7        && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
                (posicao_x+c2_1 >= +135-7         && posicao_x+c2_1 <= +135+7        && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
                (posicao_x+c2_1 >= +150-7         && posicao_x+c2_1 <= +150+7        && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
                (posicao_x+c2_1 >= +165-7         && posicao_x+c2_1 <= +165+7        && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
                (posicao_x+c2_1 >= +180-7         && posicao_x+c2_1 <= +180+7        && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
                (posicao_x+c2_1 >= +205-7         && posicao_x+c2_1 <= +205+7        && posicao_y+c2_2 >= -7.5-1500     && posicao_y+c2_2 <= -7.5+1500) ||
                (posicao_x+c2_1 >= -205-7         && posicao_x+c2_1 <= -205+7        && posicao_y+c2_2 >= -7.5-1500     && posicao_y+c2_2 <= -7.5+1500) ||
                (posicao_x+c2_1 >= +0-1000        && posicao_x+c2_1 <= +0+1000       && posicao_y+c2_2 >= -40-7         && posicao_y+c2_2 <= -40+24))
        {
            bola2 = false;  // Colidiu nos predios ou nas bordas laterais/inferior, entao sai da condicao de tiro.
            Tempo = 0;
            vez_jogada();   // Alterna jogada.
        }
    }
    else
    {
        bola2 = false;  // Sai da condicao de tiro.
        Tempo = 0;
        vez_jogada();   // Alterna jogada.
    }
}

void trajetoria_balistica_1()
{
    float Voy = Velocidade_1 * sin((angulo_1 + 90)/180.0 * PI),
          Vox = Velocidade_1 * cos((angulo_1 + 90)/180.0 * PI),

          tempo = 0.0, X = 0.0, Y = 2.0;

    glBegin(GL_LINE_STRIP);
    while(tempo <= 4 * (Voy * 2 / G))   // Desenha a linha da trajetoria, ate um dado tempo.
    {
        glVertex2f(X + Vox * tempo, Y + Voy * tempo - G * tempo * tempo / 2);
        tempo += 0.1;
    }
    glEnd();
}

void trajetoria_balistica_2()
{
    float Voy = Velocidade_2 * sin((angulo_2 + 90)/180.0 * PI),
          Vox = Velocidade_2 * cos((angulo_2 + 90)/180.0 * PI),

          tempo = 0.0, X = 0.0, Y = 2.0;

    glBegin(GL_LINE_STRIP);
    while(tempo <= 4 * (Voy * 2 / G))   // Desenha a linha da trajetoria, ate um dado tempo.
    {
        glVertex2f(X + Vox * tempo, Y + Voy * tempo - G * tempo * tempo / 2);
        tempo += 0.1;
    }
    glEnd();
}

//------------------ Funcoes para imprimir o texto na tela. ------------------
void renderbitmap(float x, float y, void *font, char* string)
{
    char *c;
    glRasterPos2f(x, y);
    for(c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void texto()
{
    char buf[100] = {0};

    if(jogada%2 == 0) // Se a jogada for par, e a vez do Player 1.
    {
        glColor3f(0.0f, 1.0f, 1.0f);
        sprintf(buf,"Velocidade: %d m/s, Angulo: %d", Velocidade_1, angulo_1+90);
        renderbitmap(-38,38,GLUT_BITMAP_HELVETICA_18, buf);
    }
    // Controles do Player 1.
    sprintf(buf,"(PLAYER 1)");
    glColor3f(0.0f, 1.0f, 1.0f);
    renderbitmap(-38,36,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf(buf,"+ VELOCIDADE:    W");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-39,35,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf(buf,"- VELOCIDADE:    S");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-39,34,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf(buf,"+ ANGULO:            D");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-39,33,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf(buf,"- ANGULO:            A");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-39,32,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf(buf,"ATIRAR:                  F");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-39,31,GLUT_BITMAP_HELVETICA_10, buf);

    if(jogada%2 != 0)   // Se a jogada for impar, e a vez do Player 2.
    {
        glColor3f(0.5,0.0,0.0);
        sprintf(buf,"Velocidade: %d m/s, Angulo: %d", Velocidade_2, angulo_2+90);
        renderbitmap(-38,38,GLUT_BITMAP_HELVETICA_18, buf);
    }

    // Controles do Player 2.
    sprintf(buf,"(PLAYER 2)");
    glColor3f(0.5,0.0,0.0);
    renderbitmap(-26,36,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf(buf,"+ VELOCIDADE:    I");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-27,35,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf(buf,"- VELOCIDADE:    K");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-27,34,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf(buf,"+ ANGULO:            J");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-27,33,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf(buf,"- ANGULO:            L");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-27,32,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf(buf,"ATIRAR:                  H");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-27,31,GLUT_BITMAP_HELVETICA_10, buf);

    sprintf(buf,"NOVO JOGO:         B");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-39,29,GLUT_BITMAP_HELVETICA_10, buf);

    sprintf(buf,"SAIR:                      Q");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-39,28,GLUT_BITMAP_HELVETICA_10, buf);

    // Informacoes da partida.
    sprintf(buf,"RODADA %d   (PLAYER 1) %d x %d (PLAYER 2)", rodada+1, ponto_1,ponto_2);
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(15,38,GLUT_BITMAP_HELVETICA_18, buf);

    sprintf(buf,"*O primeiro a marcar 3 pontos, ganha a rodada.");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(15,36,GLUT_BITMAP_HELVETICA_10, buf);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    // Gera números aleatorios de 0 a 3, para definir as posicoes dos carrinhos.
    n1 = rand()%4;
    n2 = rand()%4;

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB);
    glutInitWindowPosition(10,10);
    glutInitWindowSize(1200,800);
    glutCreateWindow("Batalha Urbana - 3D");
    glutDisplayFunc(Atualiza_Desenho);
    glutReshapeFunc(Atualiza_Tamanho);
    glutKeyboardFunc(Teclado);

    // Numeros aleatorios de 0 a 3, para definir a cor do fundo.
    numero_aleatorio = rand()%3;

    if(numero_aleatorio == 0)   // Se o numero e 0, o fundo e verde.
    {
        r = 0.0;
        g = 0.5;
        b = 0.5;
    }
    if(numero_aleatorio == 1)   // Se o número e 1, o fundo e roxo.
    {
        r = 0.5;
        g = 0.5;
        b = 1.0;
    }
    if(numero_aleatorio == 2)   // Se o numero e 2, o fundo e amarelo.
    {
        r = 0.5;
        g = 0.5;
        b = 0.0;
    }
    //glClearColor(r, g, b, 0);
    glClearColor(1, 1, 1, 0);

    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}
