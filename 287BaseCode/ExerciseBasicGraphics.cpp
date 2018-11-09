#include <ctime>
#include <vector>
#include "defs.h"
#include "Utilities.h"
#include "FrameBuffer.h"
#include "ColorAndMaterials.h"
#include "Rasterization.h"

FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT); //will not change, corresponds to color

void closed5x5Square(int x, int y, color C) { //draws filled square by using lines on pixel points
	drawLine(frameBuffer, x - 2, y-2, x + 2, y-2, C);
	drawLine(frameBuffer, x - 2, y-1, x + 2, y-1, C);
	drawLine(frameBuffer, x - 2,   y, x + 2,   y, C); //format it like this to easily find errors.
	drawLine(frameBuffer, x - 2, y+1, x + 2, y+1, C);
	drawLine(frameBuffer, x - 2, y+2, x + 2, y+2, C);
}

void closed5x5Square(const glm::vec2 &centerPt, color C) {
	closed5x5Square(centerPt.x, centerPt.y, C);
}

void open5x5Square(const glm::vec2 &centerPt, color C) {
	drawLine(frameBuffer, centerPt.x - 2, centerPt.y - 2, centerPt.x + 2, centerPt.y - 2,C);
	drawLine(frameBuffer, centerPt.x - 2, centerPt.y + 2, centerPt.x + 2, centerPt.y + 2,C);
	drawLine(frameBuffer, centerPt.x - 2, centerPt.y + 2, centerPt.x - 2, centerPt.y - 2,C);
	drawLine(frameBuffer, centerPt.x + 2, centerPt.y - 2, centerPt.x + 2, centerPt.y + 2,C);
}
//for an arc, the length of an arc is (percentage * 2pi) because 1-percentage of pi multiplied by 360*.
//must know the length of the arcs and the angles and starting/stopping points (4 lines, 2 arcs)
//NOTE: atan2(y,x)*** function easiest to find an angle theta under an arc
void pieChart(const glm::vec2 &centerPt, float rad, float perc, const color &C1, const color &C2) {
	drawArc(frameBuffer, centerPt, rad, M_PI_4, (M_PI*.75), C1);
	glm::vec2 pt1 = pointOnCircle(centerPt, rad, M_PI_4);
	glm::vec2 pt2 = pointOnCircle(centerPt, rad, (M_PI + M_PI*.75));

	drawLine(frameBuffer, centerPt, pt1, C1);
	drawLine(frameBuffer, centerPt, pt1, C1);

	glm::vec2 centerPt2 = glm::vec2(centerPt.x + 20, centerPt.y + 20);

	drawLine(frameBuffer, centerPt2, pt1, C2);
	drawLine(frameBuffer, centerPt2, pt1, C2);

	drawArc(frameBuffer, centerPt2, rad, (M_PI + M_PI * .75), (M_PI_4), C2);

}

void render() {
	frameBuffer.clearColorAndDepthBuffers();
	closed5x5Square(50, 50, red);
	closed5x5Square(glm::vec2(100, 50), green);
	open5x5Square(glm::vec2(150, 50), blue);
	pieChart(glm::vec2(250, 100), 50, 0.25, red, green);
	frameBuffer.showColorBuffer();
}

void resize(int width, int height) {
	frameBuffer.setFrameBufferSize(width, height);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case ESCAPE:	glutLeaveMainLoop();
					break;
	}
}

int main(int argc, char *argv[]) {
	//glm::vec2 v1(3, 4);
	//std::cout << v1.x << ' ' << v1.y << std::endl;
	//v1.x += 10;
	//std::cout << v1.x << ' ' << v1.y << std::endl;

	//glm::vec2 v2 = v1;
	//v2.x += 10;
	//std::cout << v2 << std::endl;

	//std::cout << normalizeRadians(-33*M_PI) << std::endl;

	//std::cout << directionInDegrees(3, 11, 2, 10) << std::endl;

	//return -1;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	GLuint world_Window = glutCreateWindow(__FILE__);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseUtility);

	frameBuffer.setClearColor(black);

	glutMainLoop();

	return 0;
}