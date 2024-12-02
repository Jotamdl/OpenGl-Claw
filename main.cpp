#include <GL/glut.h>
#include <math.h>

float anguloArticulacao1X = 0.0;
float anguloArticulacao1Y = 0.0;
float anguloArticulacao2X = 0.0;
float anguloArticulacao2Y = 0.0;
float anguloArticulacao3X = 0.0;
float anguloArticulacao3Y = 0.0;

float anguloPegada = 0.0;
float pegar = false;

float cameraAngleHorizontal = 0.0;
float cameraAngleVertical = 0.0;
float cameraDistance = 5.0;
float cameraHeight = 0.0 ;

int sombraTipo = 0;


void inicializa() {
 glClearColor(0.0, 0.0, 0.0, 1.0);
 glEnable(GL_DEPTH_TEST);

 // ativar a iluminacao
 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);
}

void configuraIluminacao()
{
 GLfloat luzAmbiente[] = {0.1, 0.1, 0.1, 1.0};   // luz suave
 GLfloat luzDifusa[]   = {1.0, 1.0, 1.0, 1.0};   // luz branca
 GLfloat luzPosicao[]  = {0.0, 0.0, 5.0, 1.0};   // luz pontual

 glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
 glLightfv(GL_LIGHT0, GL_POSITION, luzPosicao);
}

void configuraMaterial()
{
 GLfloat materialDifuso[]    = {1.0, 0.0, 0.0, 1.0}; // vermelho
 GLfloat materialAmbiente[]  = {0.1, 0.0, 0.0, 1.0}; // vermelho escuro
 GLfloat materialEspecular[] = {0.5, 0.5, 0.5, 1.0}; // um pouco de brilho

 glMaterialfv(GL_FRONT, GL_DIFFUSE , materialDifuso   );
 glMaterialfv(GL_FRONT, GL_AMBIENT , materialAmbiente );
 glMaterialfv(GL_FRONT, GL_SPECULAR, materialEspecular);

 glMaterialf(GL_FRONT, GL_SHININESS, 20); // brilho medio
}

void solidPyramid(float height, float baseSize) {

  // piramide
  glPushMatrix();
    
    glBegin(GL_TRIANGLES);
      // face frontal
      glVertex3f(-baseSize*0.5,        0.0,  baseSize*0.5);
      glVertex3f( baseSize*0.5,        0.0,  baseSize*0.5);
      glVertex3f(          0.0, height*1.0,           0.0);

      // face direita
      glVertex3f( baseSize*0.5,        0.0,  baseSize*0.5);
      glVertex3f( baseSize*0.5,        0.0, -baseSize*0.5);
      glVertex3f(          0.0, height*1.0,           0.0);

      // face traseira
      glVertex3f( baseSize*0.5,        0.0, -baseSize*0.5);
      glVertex3f(-baseSize*0.5,        0.0, -baseSize*0.5);
      glVertex3f(          0.0, height*1.0,           0.0);

      // face esquerda
      glVertex3f(-baseSize*0.5,        0.0, -baseSize*0.5);
      glVertex3f(-baseSize*0.5,        0.0,  baseSize*0.5);
      glVertex3f(          0.0, height*1.0,           0.0);

    glEnd();

      // base
    glBegin(GL_QUADS);
      glVertex3f(-baseSize*0.5, 0.0,  baseSize*0.5);
      glVertex3f( baseSize*0.5, 0.0,  baseSize*0.5);
      glVertex3f( baseSize*0.5, 0.0, -baseSize*0.5);
      glVertex3f(-baseSize*0.5, 0.0, -baseSize*0.5);
    glEnd();

  glPopMatrix();
}

void desenhaGarra(GLfloat raio) {
  GLUquadricObj *quadratic;
  quadratic = gluNewQuadric();

  glPushMatrix();
    // desenhando base
    glPushMatrix();
      GLfloat baseDifusa[] = {1.0, 1.0, 0.0, 1.0};
      GLfloat baseAmbiente[] = {0.2, 0.2, 0.0, 1.0};
      glMaterialfv(GL_FRONT, GL_DIFFUSE, baseDifusa);
      glMaterialfv(GL_FRONT, GL_AMBIENT, baseAmbiente);
      glRotatef(90.0, 0.0, 1.0, 0.0);
      glRotatef(90.0, 1.0, 0.0, 0.0);
      gluCylinder(quadratic, 0.5, 0.5, 0.13, 20, 20);
    glPopMatrix();
    
    // rotaciona articulacao maior
    glRotatef(anguloArticulacao1X, 0.0, 1.0, 0.0);
    glRotatef(anguloArticulacao1Y, 0.0, 0.0, 1.0);
    // desenhando primeiro braço
    glPushMatrix();
      GLfloat bracoDifusa[] = {0.0, 1.0, 1.0, 1.0};
      GLfloat bracoAmbiente[] = {0.0, 0.2, 0.2, 1.0};
      glMaterialfv(GL_FRONT, GL_DIFFUSE, bracoDifusa);
      glMaterialfv(GL_FRONT, GL_AMBIENT, bracoAmbiente);
      glutSolidSphere(0.13, 20, 20);
      glRotatef(90.0, 0.0, 1.0, 0.0);
      glRotatef(-90.0, 1.0, 0.0, 0.0);
      gluCylinder(quadratic, 0.1, 0.1, 1.0, 20, 20);
    glPopMatrix();
    glTranslatef(0.0, 1.0, 0.0);

    // rotaciona articulacao media
    glRotatef(anguloArticulacao2X, 0.0, 1.0, 0.0);
    glRotatef(anguloArticulacao2Y, 0.0, 0.0, 1.0);
    // desenhando segundo braço
    glPushMatrix();
      glutSolidSphere(0.13, 20, 20);
      glRotatef(90.0, 0.0, 1.0, 0.0);
      gluCylinder(quadratic, 0.1, 0.1, 0.7, 20, 20);
    glPopMatrix();
    glTranslatef(0.7, 0.0, 0.0);
    
    // rotaciona articulacao menor
    glRotatef(anguloArticulacao3X, 0.0, 1.0, 0.0);
    glRotatef(anguloArticulacao3Y, 0.0, 0.0, 1.0);
    // desenhando articulação da mão
    glPushMatrix();
      glutSolidSphere(0.13, 20, 20);
    glPopMatrix();
    
    // desenha dedo superior
    glPushMatrix();
      glRotatef(-anguloPegada, 0.0, 0.0, 1.0);
      glTranslatef(0.0, 0.1, 0.0);
      glRotatef(-20.0, 0.0, 0.0, 1.0);
      solidPyramid(0.2, 0.07);
    glPopMatrix();

    // desenha dedo inferior
    glPushMatrix();
      glRotatef( anguloPegada, 0.0, 0.0, 1.0);
      glTranslatef(0.0, -0.1, 0.0);
      glRotatef(-160.0, 0.0, 0.0, 1.0);
      solidPyramid(0.2, 0.07);
    glPopMatrix();
    
    // desenha dedo esquerdo
    glPushMatrix();
      glRotatef(-anguloPegada, 0.0, 1.0, 0.0);
      glTranslatef(0.0, 0.0, -0.1);
      glRotatef(-90.0, 1.0, 0.0, 0.0);
      glRotatef(-20.0 , 0.0, 0.0, 1.0);
      solidPyramid(0.2, 0.07);
    glPopMatrix();
    
    // desenha dedo direito
    glPushMatrix();
      glRotatef( anguloPegada, 0.0, 1.0, 0.0);
      glTranslatef(0.0, 0.0, 0.1);
      glRotatef( 90.0, 1.0, 0.0, 0.0);
      glRotatef(-20.0, 0.0, 0.0, 1.0);
      solidPyramid(0.2, 0.07);
    glPopMatrix();
    
  glPopMatrix();   
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  float posX = cameraDistance * cos(cameraAngleHorizontal);
  float posY = cameraDistance * sin(cameraAngleVertical);
  float posZ = cameraDistance * sin(cameraAngleHorizontal);

 // camera
 gluLookAt(posX, posY, posZ, // posicao da camera
           0.0, 0.0, 0.0,    // ponto alvo da camera
           0.0, 1.0, 0.0);   // rotacao da camera

 configuraIluminacao();
 configuraMaterial();

//  if (sombraTipo == 0)
//    glShadeModel(GL_FLAT);  // sombreamento plano (flat)

//  if (sombraTipo == 1)
//    glShadeModel(GL_SMOOTH);  // sombreamento suave (Gouraud)

  desenhaGarra(1.0);

  glutSwapBuffers();
}

void reshape(int largura, int altura) {
 glViewport(0, 0, largura, altura);

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();

 gluPerspective(45.0, (float)largura / (float)altura, 1.0, 100.0);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
}

void atualiza(int valor) {
  if (pegar) {
    if (anguloPegada >= 75.0) anguloPegada = 75.0; 
    anguloPegada += 1.0;
  }
  else {
    if (anguloPegada <= 0.0) anguloPegada = 0.0; 
    anguloPegada -= 1.0;
  }
  
  glutPostRedisplay();
  glutTimerFunc(16, atualiza, 0);   
}

void teclado(unsigned char tecla, int x, int y) {
  // controle de sombra
  if (tecla == '1')
    sombraTipo = 0;
  if (tecla == '2')
    sombraTipo = 1;

  // controle da camera
    // controle de distancia
  if (tecla == 'q')   
    cameraDistance -= 0.1; // move a camera para perto
  if (tecla == 'e')   
    cameraDistance += 0.1; // move a camera para longe
    //controle de posaicao
      // controle de posicao horizontal
  if (tecla == 'a')   
    cameraAngleHorizontal -= 0.1; // move a camera para esquerda
  if (tecla == 'd')   
    cameraAngleHorizontal += 0.1; // move a camera para direita
      // controle de posicao vertical
  if (tecla == 'w')   
    cameraAngleVertical += 0.1; // move a camera para cima
  if (tecla == 's')   
    cameraAngleVertical -= 0.1; // move a camera para baixo
  
  // flag pra decidir se o braco deve ou nao fazer o movimento de pegar
  if (tecla == 32) {
      pegar = !pegar;
  }
  
  // CONTROLES DO BRACO
    // articulacao MAIOR
  if (tecla == 't') {
    anguloArticulacao1Y += 1.0; // move articulacao maior para cima
  }
  if (tecla == 'g') {
    anguloArticulacao1Y -= 1.0; // move articulacao maior para baixo
  }
  if (tecla == 'h') {
    anguloArticulacao1X -= 1.0; // move articulacao maior para direita
  }
  if (tecla == 'f') {
    anguloArticulacao1X += 1.0; // move articulacao maior para esquerda
  }
    // articulacao MEDIA
  if (tecla == 'i') {
    anguloArticulacao2Y += 1.0; // move articulacao maior para cima
  }
  if (tecla == 'k') {
    anguloArticulacao2Y -= 1.0; // move articulacao maior para baixo
  }
  if (tecla == 'l') {
    anguloArticulacao2X -= 1.0; // move articulacao maior para direita
  }
  if (tecla == 'j') {
    anguloArticulacao2X += 1.0; // move articulacao maior para esquerda
  }

  glutPostRedisplay();
}

void tecladoEspecial(int tecla, int x, int y) {
  switch(tecla) {
    // controle da articulação MENOR
    case GLUT_KEY_UP:
    anguloArticulacao3Y += 1.0;
      break;
    case GLUT_KEY_DOWN:
    anguloArticulacao3Y -= 1.0;
      break;
    case GLUT_KEY_RIGHT:
    anguloArticulacao3X += 1.0;
      break;
    case GLUT_KEY_LEFT:
    anguloArticulacao3X -= 1.0;
      break;
    

  }

  glutPostRedisplay(); 
}

int main(int argc, char** argv) {
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 glutInitWindowSize(800, 600);
 glutCreateWindow("Garra");

 inicializa();

 glutDisplayFunc(display);
 glutReshapeFunc(reshape);
 glutKeyboardFunc(teclado);
 glutSpecialFunc(tecladoEspecial);

 glutTimerFunc(1000, atualiza, 0);   
  
 glutMainLoop();

 return 0;
}