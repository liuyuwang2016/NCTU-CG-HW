#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> /* offsetof */
#include "../GL/glew.h"
#include "../GL/glut.h""
#include "../shader_lib/shader.h"

#include "glm/glm.h"
extern "C"
{
#include "glm_helper.h"
}

void init(void);
void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void idle(void);

GLMmodel *Apple;
GLfloat light_pos[] = { 10.0, 10.0, 0.0 };
GLuint program_stem;
GLuint program_skin;

GLuint Norm_vbo, Position_vbo, Texcoord_vbo, Flatnor_vbo, 
       Norm2_vbo, Position2_vbo, Texcoord2_vbo, Flatnor2_vbo;

float eyex = 0.0;
float eyey = 0.0;

int mode = 0;
int degree = 0;
int flag = 1;
//int countID = 0;
//int Model_num = 3;

GLfloat ambientL[] = { 1.0,1.0,1.0,1.0 };
GLfloat specularL[] = { 1.0,1.0,1.0,1.0 };
/*------------------------------------Added by Yu-Wang Liu------------------------------------*/
//char* filename[] =
//{
//	"Models/apple.obj",
//	"Models/ball.obj",
//	"Models/Mew.obj",
//};
/*------------------------------------Added by Yu-Wang Liu------------------------------------*/
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("OpenGL HW3 - A051724");
	glutReshapeWindow(512, 512);

	glewInit();

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	glmDelete(Apple);
	
	return 0;
}

//void loadOBJModel()
//{
//	 read an obj model here
//	if (OBJ != NULL) {
//		free(OBJ);
//	}
//	OBJ = glmReadOBJ(filename[countID]);
//	printf("\nID: %d\n", countID);
//	printf("filename: %s\n", filename[countID]);
//}
//因為加載蘋果的OBJ模型的時候是把stem和skin分開加載的，因此想要加入其它模型的時候不太好加，因此將試圖加入其它模型的函數保留而並不加載。
/*------------------------------------Added by Yu-Wang Liu------------------------------------*/
void init(void)
{
	glEnable(GL_DEPTH_TEST);

	Apple = glmReadOBJ("Models/apple.obj");
	glmUnitize(Apple);
	glmFacetNormals(Apple);
	glmVertexNormals(Apple, 90.0, GL_FALSE);
	print_model_info(Apple);

	/*Shader*/
	GLuint skin_vert = createShader("Shaders/apple_skin.vert", "vertex");
	GLuint skin_frag = createShader("Shaders/apple_skin.frag", "fragment");
	GLuint stem_vert = createShader("Shaders/apple_stem.vert", "vertex");
	GLuint stem_frag = createShader("Shaders/apple_stem.frag", "fragment");
	
	program_skin = createProgram(skin_vert, skin_frag);
	program_stem = createProgram(stem_vert, stem_frag);

	GLMgroup *obj = Apple->groups;
	GLfloat  *verst, *norst, *tcost, *flatnt,
		     *verti, *norm, *texco, *flatn;
	/*------------------------------------Added by Yu-Wang Liu------------------------------------*/
	//Put the stem data under this line 
	verst = (GLfloat*)malloc(sizeof(GLfloat) * obj->numtriangles * 9);
	norst = (GLfloat*)malloc(sizeof(GLfloat) * obj->numtriangles * 9);
	tcost = (GLfloat*)malloc(sizeof(GLfloat) * obj->numtriangles * 6);
	flatnt = (GLfloat*)malloc(sizeof(GLfloat) * obj->numtriangles * 9);

	for (int i = 0; i < (obj->numtriangles); i++)
	{ 
		int findex = Apple->triangles[obj->triangles[i]].findex;
		for (int j = 0; j < 3; j++)
		{ 
			int vertindex1 = Apple->triangles[obj->triangles[i]].vindices[j];
			int normalindex1 = Apple->triangles[obj->triangles[i]].nindices[j];
			int triangleindex1 = Apple->triangles[obj->triangles[i]].tindices[j];

			verst[i * 9 + j * 3 + 0] = Apple->vertices[vertindex1 * 3 + 0];
			verst[i * 9 + j * 3 + 1] = Apple->vertices[vertindex1 * 3 + 1];
			verst[i * 9 + j * 3 + 2] = Apple->vertices[vertindex1 * 3 + 2];

			norst[i * 9 + j * 3 + 0] = Apple->normals[normalindex1 * 3 + 0];
			norst[i * 9 + j * 3 + 1] = Apple->normals[normalindex1 * 3 + 1];
			norst[i * 9 + j * 3 + 2] = Apple->normals[normalindex1 * 3 + 2];

			tcost[i * 6 + j * 2 + 0] = Apple->texcoords[triangleindex1 * 2 + 0];
			tcost[i * 6 + j * 2 + 1] = Apple->texcoords[triangleindex1 * 2 + 1];

			flatnt[i * 9 + j * 3 + 0] = Apple->facetnorms[findex * 3 + 0];
			flatnt[i * 9 + j * 3 + 1] = Apple->facetnorms[findex * 3 + 1];
			flatnt[i * 9 + j * 3 + 2] = Apple->facetnorms[findex * 3 + 2];
		}
	}
	/*------------------------------------Added by Yu-Wang Liu------------------------------------*/
	glGenBuffers(1, &Position2_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Position2_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * obj->numtriangles * 9, verst, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &Norm2_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Norm2_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * obj->numtriangles * 9, norst, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &Texcoord2_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Texcoord2_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * obj->numtriangles * 6, tcost, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &Flatnor2_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Flatnor2_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * obj->numtriangles * 9, flatnt, GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	/*------------------------------------Added by Yu-Wang Liu------------------------------------*/
	obj = obj->next;
	/*------------------------------------Added by Yu-Wang Liu------------------------------------*/
	//Put the skin data under this line 
	verti = (GLfloat*)malloc(sizeof(GLfloat) * obj->numtriangles * 9);
	norm = (GLfloat*)malloc(sizeof(GLfloat) * obj->numtriangles * 9);
	texco = (GLfloat*)malloc(sizeof(GLfloat) * obj->numtriangles * 6);
	flatn = (GLfloat*)malloc(sizeof(GLfloat) * obj->numtriangles * 9);

	for (int i = 0; i < (obj->numtriangles); i++)
	{
		int findex = Apple->triangles[obj->triangles[i]].findex;
		for (int j = 0; j < 3; j++)
		{
			int vertindex2 = Apple->triangles[obj->triangles[i]].vindices[j];
			int normalindex2 = Apple->triangles[obj->triangles[i]].nindices[j];
			int triangleindex2 = Apple->triangles[obj->triangles[i]].tindices[j];

			verti[i * 9 + j * 3 + 0] = Apple->vertices[vertindex2 * 3 + 0];
			verti[i * 9 + j * 3 + 1] = Apple->vertices[vertindex2 * 3 + 1];
			verti[i * 9 + j * 3 + 2] = Apple->vertices[vertindex2 * 3 + 2];

			norm[i * 9 + j * 3 + 0] = Apple->normals[normalindex2 * 3 + 0];
			norm[i * 9 + j * 3 + 1] = Apple->normals[normalindex2 * 3 + 1];
			norm[i * 9 + j * 3 + 2] = Apple->normals[normalindex2 * 3 + 2];

			texco[i * 6 + j * 2 + 0] = Apple->texcoords[triangleindex2 * 2 + 0];
			texco[i * 6 + j * 2 + 1] = Apple->texcoords[triangleindex2 * 2 + 1];

			flatn[i * 9 + j * 3 + 0] = Apple->facetnorms[findex * 3 + 0];
			flatn[i * 9 + j * 3 + 1] = Apple->facetnorms[findex * 3 + 1];
			flatn[i * 9 + j * 3 + 2] = Apple->facetnorms[findex * 3 + 2];
		}
	}
	/*------------------------------------Added by Yu-Wang Liu------------------------------------*/
	glGenBuffers(1, &Position_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Position_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * obj->numtriangles * 9, verti, GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &Norm_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Norm_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * obj->numtriangles * 9, norm, GL_STATIC_DRAW);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &Texcoord_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Texcoord_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * obj->numtriangles * 6, texco, GL_STATIC_DRAW);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &Flatnor_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Flatnor_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * obj->numtriangles * 9, flatn, GL_STATIC_DRAW);
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 0.01, 100);
	/*------------------------------------Added by Yu-Wang Liu------------------------------------*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyex, eyey, 3.0,
		      0.0, 0.0, 0.0,
		      0.0, 1.0, 0.0);

	GLfloat P[16];
	glGetFloatv(GL_PROJECTION_MATRIX, P);

	GLfloat MV[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, MV);
	glRotated(degree / 20, 0, 1, 0);

	GLfloat Ro[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, Ro);

	if (flag == 1) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (flag == 2) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	/*------------------------------------Added by Yu-Wang Liu------------------------------------*/
	/*Program_stem: apple stem*/

	//Material:光照材質
	GLuint spec_loc = glGetUniformLocation(program_stem, "Specular");
	GLuint shi_loc = glGetUniformLocation(program_stem, "Shiness");
	GLuint amb_loc = glGetUniformLocation(program_stem, "Ambient");
	GLuint ambLight_loc = glGetUniformLocation(program_stem, "Ambient_Light");
	GLuint specLight_loc = glGetUniformLocation(program_stem, "Specular_Light");

	//Texture:紋理紋路
	GLuint Tex0_loc = glGetUniformLocation(program_stem, "Tex0");
	GLuint Tex1_loc = glGetUniformLocation(program_stem, "Tex1");
	glUseProgram(program_stem);

	//Vectors & Matrices:向量描述
	GLuint eye_loc = glGetUniformLocation(program_stem, "Eye");
	GLuint light_loc = glGetUniformLocation(program_stem, "Light");
	GLuint P_loc = glGetUniformLocation(program_stem, "P");
	GLuint MV_loc = glGetUniformLocation(program_stem, "MV");
	GLuint Ro_loc = glGetUniformLocation(program_stem, "Rotate");
	GLuint Mode_loc = glGetUniformLocation(program_stem, "Mode");

	//Uniform
	glUniform3f(eye_loc, eyex, eyey, 3.0);
	glUniform3f(light_loc, light_pos[0], light_pos[1], light_pos[2]);
	glUniformMatrix4fv(P_loc, 1, GL_FALSE, P);
	glUniformMatrix4fv(MV_loc, 1, GL_FALSE, MV);
	glUniformMatrix4fv(Ro_loc, 1, GL_FALSE, Ro);
	glUniform1i(Mode_loc, mode);
	//Material
	glUniform4f(spec_loc, Apple->materials[3].specular[0], Apple->materials[3].specular[1], Apple->materials[3].specular[2], Apple->materials[3].specular[3]);
	glUniform1f(shi_loc, Apple->materials[3].shininess);
	glUniform4f(amb_loc, Apple->materials[3].ambient[0], Apple->materials[3].ambient[1], Apple->materials[3].ambient[2], Apple->materials[3].ambient[3]);
	glUniform4f(ambLight_loc, ambientL[0], ambientL[1], ambientL[2], ambientL[3]);
	glUniform4f(specLight_loc, specularL[0], specularL[1], specularL[2], specularL[3]);
	//Texture
	glActiveTexture(GL_TEXTURE0 + 1); 
	glBindTexture(GL_TEXTURE_2D, Apple->textures[1].id);
	glUniform1i(Tex1_loc, 1);
	//Draw Array
	glDrawArrays(GL_TRIANGLES, 0, Apple->groups->numtriangles * 3);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glUseProgram(NULL);
	/*------------------------------------Added by Yu-Wang Liu------------------------------------*/
	//Program_skin: apple skin
	/*------------------------------------Added by Yu-Wang Liu------------------------------------*/
	//Material:光照材質
	spec_loc = glGetUniformLocation(program_skin, "Specular");
	shi_loc = glGetUniformLocation(program_skin, "Shiness");
	amb_loc = glGetUniformLocation(program_skin, "Ambient");
	ambLight_loc = glGetUniformLocation(program_skin, "Ambient_Light");
	specLight_loc = glGetUniformLocation(program_skin, "Specular_Light");
	//Texture:紋理紋路
	Tex0_loc = glGetUniformLocation(program_skin, "Tex0");
	Tex1_loc = glGetUniformLocation(program_skin, "Tex1");
	glUseProgram(program_skin);
	//Vectors and Matrices:向量和矩陣
	eye_loc = glGetUniformLocation(program_skin, "Eye");
	light_loc = glGetUniformLocation(program_skin, "Light");
	P_loc = glGetUniformLocation(program_skin, "P");
	MV_loc = glGetUniformLocation(program_skin, "MV");
	Ro_loc = glGetUniformLocation(program_skin, "Rotate");
	Mode_loc = glGetUniformLocation(program_skin, "Mode");
	
	//Uniform
	glUniform3f(eye_loc, eyex, eyey, 3.0);
	glUniform3f(light_loc, light_pos[0], light_pos[1], light_pos[2]);
	glUniformMatrix4fv(P_loc, 1, GL_FALSE, P);
	glUniformMatrix4fv(MV_loc, 1, GL_FALSE, MV);
	glUniformMatrix4fv(Ro_loc, 1, GL_FALSE, Ro);
	glUniform1i(Mode_loc, mode);
	//Material
	glUniform4f(spec_loc, Apple->materials[2].specular[0], Apple->materials[2].specular[1], Apple->materials[2].specular[2], Apple->materials[2].specular[3]);
	glUniform1f(shi_loc, Apple->materials[2].shininess);
	glUniform4f(amb_loc, Apple->materials[2].ambient[0], Apple->materials[2].ambient[1], Apple->materials[2].ambient[2], Apple->materials[2].ambient[3]);
	glUniform4f(ambLight_loc, ambientL[0], ambientL[1], ambientL[2], ambientL[3]);
	glUniform4f(specLight_loc, specularL[0], specularL[1], specularL[2], specularL[3]);
	//Texture
	glActiveTexture(GL_TEXTURE0 + 0); 
	glBindTexture(GL_TEXTURE_2D, Apple->textures[0].id);
	glUniform1i(Tex0_loc, 0);
	//Draw Array
	glDrawArrays(GL_TRIANGLES, 0, Apple->groups->next->numtriangles * 3);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glUseProgram(NULL);
	/*------------------------------------Added by Yu-Wang Liu------------------------------------*/
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	printf("%18s(): %dx%d\n", __FUNCTION__, width, height);
}
/*------------------------------------Added by Yu-Wang Liu------------------------------------*/
void keyboard(unsigned char key, int x, int y) 
{	
    printf("%18s(): (%d, %d) key: %c(0x%02X) ", __FUNCTION__, x, y, key, key);
	switch (key) {
	case 'h':case'H': {
		printf("\n========幫助=======\n");
		printf("按ESC:結束程式\n");
		printf("按 WSAD 控制物體轉動\n");
		printf("按12345678切換光照\n");
		printf("按P/p切換shading模式\n");
		printf("按L/l:切換蘋果solid/wire模式\n");
		printf("按H/h:幫助表單\n");
		printf("========謝謝==A051724\n");
		break;
	}
	case 27:{ exit(0); break; }
	case 'd':case 'D':{ eyex += 0.5; break;}
	case 'a':case 'A': { eyex -= 0.5; break; }
	case 'w':case 'W': { eyey += 0.5; break; }
	case 's':case 'S':{ eyey -= 0.5; break; }
	case 'r': { eyex = eyey = 0.0; break; }
	case '1': { specularL[0] = 1.0; specularL[1] = 1.0; specularL[2] = 1.0; break; }
	case '2': { ambientL[0] = 1.0; ambientL[1] = 1.0; ambientL[2] = 1.0; break; }
	case '3': { specularL[0] = 1.0; specularL[1] = 0.0; specularL[2] = 0.0; break; }
	case '4': { ambientL[0] = 1.0; ambientL[1] = 0.0; ambientL[2] = 0.0; break; }
	case '5': { specularL[0] = 0.0; specularL[1] = 1.0; specularL[2] = 0.0; break; }
	case '6': { ambientL[0] = 0.0; ambientL[1] = 1.0; ambientL[2] = 0.0; break; }
	case '7': { specularL[0] = 0.0; specularL[1] = 0.0; specularL[2] = 1.0; break; }
	case '8': { ambientL[0] = 0.0; ambientL[1] = 0.0; ambientL[2] = 1.0; break; }
	case 'p': case'P':{ mode = mode < 1 ? mode + 1 : 0; printf("%d", mode); break; }
	case 'l':case'L': {if (flag == 1) {flag = 2; } else if (flag == 2){ flag = 1;} break;}
	default: { break; }
	}
}
/*------------------------------------Added by Yu-Wang Liu------------------------------------*/
void idle(void)
{
	degree = degree < 72000 ? degree + 1 : 0;
	glutPostRedisplay();
}
