#include <GL/glut.h>
#include <cmath>
#include <initializer_list> // Para listas de inicialização

#include <math.h>
#define PI 3.14159265358979323846
#define NUM_SEGMENTOS 100

#define STB_IMAGE_IMPLEMENTATION
#include <texture/stb_image.h>

//---------------------------------- VARIÁVEIS E FUNÇÕES DA GARRA -----------------------------------

// Variáveis da garra
float anguloArticulacao1X = 0.0;
float anguloArticulacao1Y = 0.0;
float anguloArticulacao2X = 0.0;
float anguloArticulacao2Y = 0.0;
float anguloArticulacao3X = 0.0;
float anguloArticulacao3Y = 0.0;

float anguloPegada = 0.0;
float pegar = false;

// angulos da camera
float cameraAngleHorizontal = 0.0;
float cameraAngleVertical = 45.0;
float cameraDistance = 5.0;
float cameraHeight = 0.0 ;

// Limites para o ângulo de pegada
const float ANGOLO_PEGADA_MAX = 75.0f;  // máximo de fechamento
const float ANGOLO_PEGADA_MIN = 0.0f;   // mínimo ao abrir

int sombraTipo = 0;

// Variáveis do objeto (bloco vermelho)
float objetoX = 1.0f;   
float objetoY = 1.2f;   
float objetoZ = 0.0f;
bool objetoPegado = false;

// variavel para carregar texturas
GLuint idsTextura[10];
bool loadHouseTex = true;

void desenhaCirculo(float raio)
{
 //GL_LINE_LOOP
 glBegin(GL_POLYGON);
   for (int i = 0; i <= NUM_SEGMENTOS; i++)
   {
    float angulo = 2.0f * PI * i / NUM_SEGMENTOS;
    float dx = raio * cos(angulo);
    float dy = raio * sin(angulo);
    glVertex2f(dx, dy);
   }
 glEnd();
}

// função auxiliar para desenhar pirâmide usada nos "dedos"
void solidPyramid(float height, float baseSize) {
  glPushMatrix();
    glBegin(GL_TRIANGLES);
      // face frontal
      glNormal3f(0.0, 0.5, 0.5);
      glVertex3f(-baseSize*0.5f, 0.0f,  baseSize*0.5f);
      glVertex3f( baseSize*0.5f, 0.0f,  baseSize*0.5f);
      glVertex3f( 0.0f, height, 0.0f);

      // face direita
      glNormal3f(0.5, 0.5, 0.0);
      glVertex3f( baseSize*0.5f, 0.0f,  baseSize*0.5f);
      glVertex3f( baseSize*0.5f, 0.0f, -baseSize*0.5f);
      glVertex3f( 0.0f, height, 0.0f);

      // face traseira
      glNormal3f(0.0, 0.5, -0.5);
      glVertex3f( baseSize*0.5f, 0.0f, -baseSize*0.5f);
      glVertex3f(-baseSize*0.5f, 0.0f, -baseSize*0.5f);
      glVertex3f( 0.0f, height, 0.0f);

      // face esquerda
      glNormal3f(-0.5, 0.5, 0.0);
      glVertex3f(-baseSize*0.5f, 0.0f, -baseSize*0.5f);
      glVertex3f(-baseSize*0.5f, 0.0f,  baseSize*0.5f);
      glVertex3f( 0.0f, height, 0.0f);
    glEnd();

    // base da pirâmide
    glBegin(GL_QUADS);
      glNormal3f(0.0, -1.0, 0.0);
      glVertex3f(-baseSize*0.5f, 0.0f,  baseSize*0.5f);
      glVertex3f( baseSize*0.5f, 0.0f,  baseSize*0.5f);
      glVertex3f( baseSize*0.5f, 0.0f, -baseSize*0.5f);
      glVertex3f(-baseSize*0.5f, 0.0f, -baseSize*0.5f);
    glEnd();
  glPopMatrix();
}

// funcao auxiliar para desenhar cubos que podem receber texturas
void solidCube(float size) {
  size = size/2;
  glPushMatrix();
    // face frontal
    glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0); glVertex3f(-size, -size, size);
      glTexCoord2f(1.0, 0.0); glVertex3f(size, -size, size);
      glTexCoord2f(1.0, 1.0); glVertex3f(size, size, size);
      glTexCoord2f(0.0, 1.0); glVertex3f(-size, size, size);

    // face traseira
      glTexCoord2f(0.0, 0.0); glVertex3f(-size, -size, -size);
      glTexCoord2f(1.0, 0.0); glVertex3f(size, -size, -size);
      glTexCoord2f(1.0, 1.0); glVertex3f(size, size, -size);
      glTexCoord2f(0.0, 1.0); glVertex3f(-size, size, -size);

    // face superior
      glTexCoord2f(0.0, 0.0); glVertex3f(-size, size, -size);
      glTexCoord2f(1.0, 0.0); glVertex3f(size, size, -size);
      glTexCoord2f(1.0, 1.0); glVertex3f(size, size, size);
      glTexCoord2f(0.0, 1.0); glVertex3f(-size, size, size);

    // face inferior
      glTexCoord2f(0.0, 0.0); glVertex3f(-size, -size, -size);
      glTexCoord2f(1.0, 0.0); glVertex3f(size, -size, -size);
      glTexCoord2f(1.0, 1.0); glVertex3f(size, -size, size);
      glTexCoord2f(0.0, 1.0); glVertex3f(-size, -size, size);

    // face direita
      glTexCoord2f(0.0, 0.0); glVertex3f(size, -size, -size);
      glTexCoord2f(1.0, 0.0); glVertex3f(size, size, -size);
      glTexCoord2f(1.0, 1.0); glVertex3f(size, size, size);
      glTexCoord2f(0.0, 1.0); glVertex3f(size, -size, size);

    // face esquerda
      glTexCoord2f(0.0, 0.0); glVertex3f(-size, -size, -size);
      glTexCoord2f(1.0, 0.0); glVertex3f(-size, size, -size);
      glTexCoord2f(1.0, 1.0); glVertex3f(-size, size, size);
      glTexCoord2f(0.0, 1.0); glVertex3f(-size, -size, size);
    glEnd();    
    
  glPopMatrix();
}

void carregarTextura(const char* nomeArquivo, int indice) {
  int largura, altura, canais;

  unsigned char *dados = stbi_load(nomeArquivo, &largura, &altura, &canais, 0);

  if (!dados)
    exit(1);

  // gerar textura
  glGenTextures(1, &idsTextura[indice]);
  glBindTexture(GL_TEXTURE_2D, idsTextura[indice]);

  // configurar parametros da textura
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, (canais == 4) ? GL_RGBA : GL_RGB, 
                largura, altura, 0, (canais == 4) ? GL_RGBA : GL_RGB,
                GL_UNSIGNED_BYTE, dados);

  // liberar a memoria da imagem
  stbi_image_free(dados);              
}

void desenhaGarra(GLfloat raio) {
  GLUquadricObj *quadratic = gluNewQuadric();
  gluQuadricNormals(quadratic, GLU_SMOOTH);

  // Materiais
  GLfloat difusoBraco[] = {0.7f, 0.7f, 0.7f, 1.0f};
  GLfloat ambienteBraco[] = {0.4f, 0.4f, 0.4f, 1.0f};
  GLfloat especularBraco[] = {0.4f, 0.4f, 0.4f, 1.0f};

  GLfloat difusoJunta[] = {0.4f, 0.4f, 0.4f, 1.0f};
  GLfloat ambienteJunta[] = {0.15f, 0.15f, 0.15f, 1.0f};
  GLfloat especularJunta[] = {0.1f, 0.1f, 0.1f, 1.0f};

  GLfloat difusoDedos[] = {0.1f, 0.1f, 0.1f, 1.0f};
  GLfloat ambienteDedos[] = {0.05f, 0.05f, 0.05f, 1.0f};
  GLfloat especularDedos[] = {0.0f, 0.0f, 0.0f, 1.0f};

  glPushMatrix();
    // Base da garra
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difusoBraco);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambienteBraco);
    glMaterialfv(GL_FRONT, GL_SPECULAR, especularBraco);
    glMaterialf(GL_FRONT, GL_SHININESS, 20.0f);

    // cilindro base
    glPushMatrix();
      glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
      glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
      gluCylinder(quadratic, 0.5, 0.5, 0.13, 20, 20);
    glPopMatrix();
    
    // topo cilindro base
    glPushMatrix();
      glRotatef(90.0, 1.0, 0.0, 0.0);
      desenhaCirculo(0.5);
    glPopMatrix();
    
    // rotação articulação maior
    glRotatef(anguloArticulacao1X, 0.0f, 1.0f, 0.0f);
    glRotatef(anguloArticulacao1Y, 0.0f, 0.0f, 1.0f);

    // primeiro braço + junta
    // junta (esfera)
    glPushMatrix();
      glMaterialfv(GL_FRONT, GL_DIFFUSE, difusoJunta);
      glMaterialfv(GL_FRONT, GL_AMBIENT, ambienteJunta);
      glMaterialfv(GL_FRONT, GL_SPECULAR, especularJunta);
      glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);
      glutSolidSphere(0.13, 20, 20);
    glPopMatrix();

    // braço (cilindro)
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difusoBraco);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambienteBraco);
    glMaterialfv(GL_FRONT, GL_SPECULAR, especularBraco);
    glMaterialf(GL_FRONT, GL_SHININESS, 20.0f);
    glPushMatrix();
      glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
      glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
      gluCylinder(quadratic, 0.1, 0.1, 1.0, 20, 20);
    glPopMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);

    // rotação articulação média
    glRotatef(anguloArticulacao2X, 0.0f, 1.0f, 0.0f);
    glRotatef(anguloArticulacao2Y, 0.0f, 0.0f, 1.0f);

    // segunda junta
    glPushMatrix();
      glMaterialfv(GL_FRONT, GL_DIFFUSE, difusoJunta);
      glMaterialfv(GL_FRONT, GL_AMBIENT, ambienteJunta);
      glMaterialfv(GL_FRONT, GL_SPECULAR, especularJunta);
      glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);
      glutSolidSphere(0.13, 20, 20);
    glPopMatrix();

    // segundo braço
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difusoBraco);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambienteBraco);
    glMaterialfv(GL_FRONT, GL_SPECULAR, especularBraco);
    glMaterialf(GL_FRONT, GL_SHININESS, 20.0f);
    glPushMatrix();
      glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
      gluCylinder(quadratic, 0.1, 0.1, 0.7, 20, 20);
    glPopMatrix();
    glTranslatef(0.7f, 0.0f, 0.0f);
    
    // rotação articulação menor
    glRotatef(anguloArticulacao3X, 0.0f, 1.0f, 0.0f);
    glRotatef(anguloArticulacao3Y, 0.0f, 0.0f, 1.0f);

    // pulso
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difusoBraco);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambienteBraco);
    glMaterialfv(GL_FRONT, GL_SPECULAR, especularBraco);
    glMaterialf(GL_FRONT, GL_SHININESS, 20.0f);
    glPushMatrix();
      glScalef(0.2f, 0.2f, 0.2f);
      glutSolidCube(1.0);
    glPopMatrix();

    // esfera da mão
    glPushMatrix();
      glMaterialfv(GL_FRONT, GL_DIFFUSE, difusoJunta);
      glMaterialfv(GL_FRONT, GL_AMBIENT, ambienteJunta);
      glMaterialfv(GL_FRONT, GL_SPECULAR, especularJunta);
      glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);
      glutSolidSphere(0.13, 20, 20);
    glPopMatrix();
    
    // Dedos (material escuro)
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difusoDedos);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambienteDedos);
    glMaterialfv(GL_FRONT, GL_SPECULAR, especularDedos);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);

    // dedo superior
    glPushMatrix();
      glRotatef(-anguloPegada, 0.0f, 0.0f, 1.0f);
      glTranslatef(0.0f, 0.1f, 0.0f);
      glRotatef(-20.0f, 0.0f, 0.0f, 1.0f);
      solidPyramid(0.2f, 0.07f);
    glPopMatrix();

    // dedo inferior
    glPushMatrix();
      glRotatef(anguloPegada, 0.0f, 0.0f, 1.0f);
      glTranslatef(0.0f, -0.1f, 0.0f);
      glRotatef(-160.0f, 0.0f, 0.0f, 1.0f);
      solidPyramid(0.2f, 0.07f);
    glPopMatrix();
    
    // dedo esquerdo
    glPushMatrix();
      glRotatef(-anguloPegada, 0.0f, 1.0f, 0.0f);
      glTranslatef(0.0f, 0.0f, -0.1f);
      glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
      glRotatef(-20.0f , 0.0f, 0.0f, 1.0f);
      solidPyramid(0.2f, 0.07f);
    glPopMatrix();
    
    // dedo direito
    glPushMatrix();
      glRotatef(anguloPegada, 0.0f, 1.0f, 0.0f);
      glTranslatef(0.0f, 0.0f, 0.1f);
      glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
      glRotatef(-20.0f, 0.0f, 0.0f, 1.0f);
      solidPyramid(0.2f, 0.07f);
    glPopMatrix();

    // Se o objeto estiver pegado, desenha-lo aqui, para que acompanhe a garra
    if (objetoPegado) {
      GLfloat difObj[] = {0.8f, 0.1f, 0.1f, 1.0f};
      GLfloat ambObj[] = {0.2f, 0.05f, 0.05f, 1.0f};
      glMaterialfv(GL_FRONT, GL_DIFFUSE, difObj);
      glMaterialfv(GL_FRONT, GL_AMBIENT, ambObj);
      glMaterialf(GL_FRONT, GL_SHININESS, 5.0f);

      glPushMatrix();
        // Desenha o bloco no referencial da garra (na palma da mão)
        glTranslatef(0.0f, 0.0f, 0.0f); 
        glutSolidCube(0.1f);
      glPopMatrix();
    }

  glPopMatrix();   
  gluDeleteQuadric(quadratic);
}

void desenhaMesa() {
  // Configurando texturas
  glEnable(GL_TEXTURE_2D);
  carregarTextura("textures/mesa_textura.jpg", 0);
  // Ajustar modo para a textura não escurecer
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
  
  GLfloat difusoMarrom[]    = {0.4f, 0.2f, 0.0f, 1.0f};
  GLfloat ambienteMarrom[]  = {0.2f, 0.1f, 0.0f, 1.0f};
  GLfloat especularMarrom[] = {0.0f, 0.0f, 0.0f, 1.0f}; 
  glMaterialfv(GL_FRONT, GL_DIFFUSE, difusoMarrom);
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambienteMarrom);
  glMaterialfv(GL_FRONT, GL_SPECULAR, especularMarrom);
  glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);

  glPushMatrix();
    // adicionando textura
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);

    // Tampo da mesa
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glScalef(4.0f, 0.2f, 2.0f);
    solidCube(1.0f);
    glPopMatrix();

    // Pés da mesa
    float tamPé = 0.2f; 
    float alturaPé = 1.65f;

    for (float x : std::initializer_list<float>{-1.7f, 1.7f}) {
      for (float z : std::initializer_list<float>{-0.8f, 0.8f}) {
        glPushMatrix();
          glTranslatef(x, 0.23, z);
          glScalef(tamPé, alturaPé, tamPé);
          solidCube(1.0f);
        glPopMatrix();
      }
    }
    
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);

  // Desenhar o cubo sobre a mesa se não estiver pegado
  if (!objetoPegado) {
    GLfloat difObj[] = {0.8f, 0.1f, 0.1f, 1.0f};
    GLfloat ambObj[] = {0.2f, 0.05f, 0.05f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difObj);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambObj);
    glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);

    glPushMatrix();
      glTranslatef(objetoX, objetoY, objetoZ); 
      glutSolidCube(0.1f);
    glPopMatrix();
  }

  // Desenhar a garra acima da mesa
  glPushMatrix();
    glTranslatef(0.0f, 1.243f, 0.0f); 
    desenhaGarra(1.0f); 
  glPopMatrix();
}

void desenhaQuarto() {
  if (loadHouseTex) {
    // Configurando texturas
    glEnable(GL_TEXTURE_2D);
    carregarTextura("textures/parede.jpg", 1);
    carregarTextura("textures/chao.jpg", 2);
    carregarTextura("textures/teto.jpg", 3);
    // Ajustar modo para a textura não escurecer
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  }
  else glDisable(GL_TEXTURE_2D);
  
  float size = 15.0;
  glPushMatrix();
    // face frontal
    if (loadHouseTex) glBindTexture(GL_TEXTURE_2D, idsTextura[1]);
    glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0); glVertex3f(-size, -0.6, size);
      glTexCoord2f(1.0, 0.0); glVertex3f(size, -0.6, size);
      glTexCoord2f(1.0, 1.0); glVertex3f(size, 10, size);
      glTexCoord2f(0.0, 1.0); glVertex3f(-size, 10, size);
    glEnd();    

    // face traseira
    if (loadHouseTex) glBindTexture(GL_TEXTURE_2D, idsTextura[1]);
    glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0); glVertex3f(-size, -0.6, -size);
      glTexCoord2f(1.0, 0.0); glVertex3f(size, -0.6, -size);
      glTexCoord2f(1.0, 1.0); glVertex3f(size, 10, -size);
      glTexCoord2f(0.0, 1.0); glVertex3f(-size, 10, -size);
    glEnd();    

    // face superior
    if (loadHouseTex) glBindTexture(GL_TEXTURE_2D, idsTextura[3]);
    glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0); glVertex3f(-size, 10, -size);
      glTexCoord2f(1.0, 0.0); glVertex3f(size, 10, -size);
      glTexCoord2f(1.0, 1.0); glVertex3f(size, 10, size);
      glTexCoord2f(0.0, 1.0); glVertex3f(-size, 10, size);
    glEnd();    

    // face inferior
    if (loadHouseTex) glBindTexture(GL_TEXTURE_2D, idsTextura[2]);
    glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0); glVertex3f(-size, -0.6, -size);
      glTexCoord2f(1.0, 0.0); glVertex3f(size, -0.6, -size);
      glTexCoord2f(1.0, 1.0); glVertex3f(size, -0.6, size);
      glTexCoord2f(0.0, 1.0); glVertex3f(-size, -0.6, size);
    glEnd();    

    // face direita
    if (loadHouseTex) glBindTexture(GL_TEXTURE_2D, idsTextura[1]);
    glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0); glVertex3f(size, -0.6, -size);
      glTexCoord2f(1.0, 0.0); glVertex3f(size, 10, -size);
      glTexCoord2f(1.0, 1.0); glVertex3f(size, 10, size);
      glTexCoord2f(0.0, 1.0); glVertex3f(size, -0.6, size);
    glEnd();    

    // face esquerda
    if (loadHouseTex) glBindTexture(GL_TEXTURE_2D, idsTextura[1]);
    glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0); glVertex3f(-size, -0.6, -size);
      glTexCoord2f(1.0, 0.0); glVertex3f(-size, 10, -size);
      glTexCoord2f(1.0, 1.0); glVertex3f(-size, 10, size);
      glTexCoord2f(0.0, 1.0); glVertex3f(-size, -0.6, size);
    glEnd();    
    
  glPopMatrix();
  
  if (loadHouseTex) glDisable(GL_TEXTURE_2D);
}

void inicializa() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Ativa iluminação
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat luzAmbiente[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat luzDifusa[]   = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat posicaoLuz[]  = {10.0f, 10.0f, 10.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

    glShadeModel(GL_SMOOTH);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  float camX = cameraDistance * sinf(cameraAngleHorizontal * M_PI / 180.0f) * cosf(cameraAngleVertical * M_PI / 180.0f);
  float camZ = cameraDistance * cosf(cameraAngleHorizontal * M_PI / 180.0f) * cosf(cameraAngleVertical * M_PI / 180.0f);
  float camY = cameraDistance * sinf(cameraAngleVertical * M_PI / 180.0f);

  // camera
  gluLookAt(camX, camY, camZ, // posicao da camera
            0.0, 0.0, 0.0,    // ponto alvo da camera
            0.0, 1.0, 0.0);   // rotacao da camera

  if (sombraTipo == 0)
    glShadeModel(GL_FLAT);  // sombreamento plano (flat)

  if (sombraTipo == 1)
    glShadeModel(GL_SMOOTH);  // sombreamento suave (Gouraud)

  glPushMatrix();
    desenhaQuarto();
    desenhaMesa();
  glPopMatrix();

  glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void teclado(unsigned char tecla, int x, int y) {
    const float incremento = 0.1f; 

    switch (tecla) {
        case 'w': cameraAngleVertical  += incremento * 20; break;   //
        case 's': cameraAngleVertical  -= incremento * 20; break;   //
        case 'a': cameraAngleHorizontal  += incremento * 20; break; //
        case 'd': cameraAngleHorizontal  -= incremento * 20; break; //
        case 'q': cameraDistance += incremento; break;         //
        case 'e': cameraDistance -= incremento; break;         //
    }

    if (tecla == '1')
      sombraTipo = 0;
    if (tecla == '2')
      sombraTipo = 1;

    // Tecla espaço alterna entre abrir e fechar a garra
    if (tecla == 32) { 
        pegar = !pegar;
    }

    if (tecla == 't') anguloArticulacao1Y += 1.0f; 
    if (tecla == 'g') anguloArticulacao1Y -= 1.0f; 
    if (tecla == 'h') anguloArticulacao1X -= 1.0f; 
    if (tecla == 'f') anguloArticulacao1X += 1.0f; 
    if (tecla == 'i') anguloArticulacao2Y += 1.0f;
    if (tecla == 'k') anguloArticulacao2Y -= 1.0f;
    if (tecla == 'l') anguloArticulacao2X -= 1.0f;
    if (tecla == 'j') anguloArticulacao2X += 1.0f;

    glutPostRedisplay();
}

void tecladoEspecial(int tecla, int x, int y) {
    switch(tecla) {
      case GLUT_KEY_UP:
        anguloArticulacao3Y += 1.0f;
        break;
      case GLUT_KEY_DOWN:
        anguloArticulacao3Y -= 1.0f;
        break;
      case GLUT_KEY_RIGHT:
        anguloArticulacao3X += 1.0f;
        break;
      case GLUT_KEY_LEFT:
        anguloArticulacao3X -= 1.0f;
        break;  
    }

    glutPostRedisplay(); 
}

void mousePressionado(int botao, int estado, int x, int y) {
  if (estado == GLUT_DOWN) {
    if (botao == GLUT_LEFT_BUTTON)
      loadHouseTex = true;
    else if (botao == GLUT_RIGHT_BUTTON)
      loadHouseTex = false;
    }

 glutPostRedisplay();
}

void menu(int opcao) {
    switch(opcao) {
        case 1: // Reiniciar articulações
            anguloArticulacao1X = 0.0;
            anguloArticulacao1Y = 0.0;
            anguloArticulacao2X = 0.0;
            anguloArticulacao2Y = 0.0;
            anguloArticulacao3X = 0.0;
            anguloArticulacao3Y = 0.0;
            anguloPegada = 0.0;
            objetoPegado = false;
            pegar = false;
            break;
    }
    glutPostRedisplay();
}

void criarMenu() {
    // Cria o menu e associa a função callback
    glutCreateMenu(menu);
    glutAddMenuEntry("Reiniciar articulacoes", 1);

    // Associa o menu ao botão direito do mouse
    glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

void atualiza(int valor) {
  // Controla a abertura/fechamento da garra
  if (pegar) {
    if (anguloPegada < ANGOLO_PEGADA_MAX) {
      anguloPegada += 1.0f;
      if (anguloPegada > ANGOLO_PEGADA_MAX) anguloPegada = ANGOLO_PEGADA_MAX;
    }

    // Se a garra está quase fechada, verificar se o objeto está próximo da garra
    if (!objetoPegado && anguloPegada > 70.0f) {
      // Checamos se o objeto está perto do centro da mão da garra (no momento posição fixa)
      // A mão da garra está aproximadamente em X=1.9 se a garra estiver reta, mas aqui simplificamos.
      // Já que o cubo está em (1.0, 1.2, 0.0) e a garra está no centro (0,1.243,0),
      // Podemos verificar a distância em X e Z:
      float dx = objetoX - 0.0f; // garra no centro x=0
      float dy = objetoY - 1.243f; 
      float dz = objetoZ - 0.0f;

      float dist = sqrt(dx*dx + dy*dy + dz*dz);
      // Se a distância for pequena o suficiente, consideramos o objeto capturado
      // Ajuste conforme necessidade (aqui dist < 0.3 por exemplo)
      if (dist < 0.3f) {
        objetoPegado = true;
      }
    }

  } else {
    if (anguloPegada > ANGOLO_PEGADA_MIN) {
      anguloPegada -= 1.0f;
      if (anguloPegada < ANGOLO_PEGADA_MIN) anguloPegada = ANGOLO_PEGADA_MIN;
    }

    // Se a garra abriu totalmente, solta o objeto
    if (anguloPegada <= ANGOLO_PEGADA_MIN && objetoPegado) {
      objetoPegado = false;
      // Quando solta, o objeto permanece na última posição da garra
      // Para isso, vamos armazenar a posição da garra e atribuir ao objeto
      // A garra está no centro, se a garra não se move em X/Z so mantemos?
      // Nesse caso simplificado, a garra não se translada, então deixamos o objeto parado no local da garra
      objetoX = 0.0f;
      objetoY = 1.243f;
      objetoZ = 0.0f;
    }
  }
  
  glutPostRedisplay();
  glutTimerFunc(16, atualiza, 0);   
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Mesa 3D com Garra (Abrir/Fechar) e Objeto Capturável");

    inicializa();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(tecladoEspecial);
    glutMouseFunc(mousePressionado);
    glutTimerFunc(1000, atualiza, 0);

    criarMenu();

    glutMainLoop();
    return 0;
}

