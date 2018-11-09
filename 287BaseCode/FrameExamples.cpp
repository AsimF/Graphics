#include <iostream>
#include <vector>
#include "Utilities.h"
#include "Defs.h"

void test(const glm::vec3 &origin,
			const glm::vec3 &w,
			const glm::vec3 &up) {
	Frame frame = Frame::createOrthoNormalBasis(origin, w, up);

	std::vector<glm::vec3> coords = { glm::vec3(0,0,0), origin, glm::vec3(1, 0, 0), glm::vec3(0, 0, 1) };
	std::cout << frame << std::endl;
	for (int i = 0; i < coords.size(); i++) {
		glm::vec3 frameCoords = frame.toFrameCoords(coords[i]);
		glm::vec3 backToWorldCoords = frame.toWorldCoords(frameCoords);
		std::cout << "World coords: " << coords[i] <<
			" To frame coords: " << frameCoords <<
			" Back to world coords: " << backToWorldCoords << std::endl;

		glm::vec3 worldCoords = frame.toWorldCoords(coords[i]);
		glm::vec3 backToFrameCoords = frame.toFrameCoords(worldCoords);
		std::cout << "Frame: " << coords[i] <<
			" To world: " << worldCoords <<
			" Back to frame: " << backToFrameCoords << std::endl << std::endl;
	}
	std::vector<glm::vec3> vectors = { glm::vec3(1,0,0), glm::vec3(1,1,1) };
	for (int i = 0; i < vectors.size(); i++) {
		glm::vec3 frameVector = frame.toFrameVector(vectors[i]);
		glm::vec3 backToWorldVector = frame.toWorldVector(frameVector);
		std::cout << "World vector: " << vectors[i] <<
			" To frame vector: " << frameVector <<
			" Back to world vector: " << backToWorldVector << std::endl;

		glm::vec3 worldVector = frame.toWorldVector(vectors[i]);
		glm::vec3 backToFrameVector = frame.toFrameVector(worldVector);
		std::cout << "Frame: " << vectors[i] <<
			" To world: " << worldVector <<
			" Back to frame: " << backToFrameVector << std::endl << std::endl;
	}
}
int main(int argc, char *argv[]) {
	test(glm::vec3(10, 10, 10), glm::vec3(1, 1, 1), glm::vec3(0, 1, 0));
	return 0;
}
/*
Pos: [ 10 10 10 ]
U: [ 0.707107 0 -0.707107 ]
V: [ -0.408248 0.816497 -0.408248 ]
W: [ 0.57735 0.57735 0.57735 ]

World coords: [ 0 0 0 ] To frame coords: [ 0 0 -17.3205 ] Back to world coords: [ 0 0 0 ]
Frame: [ 0 0 0 ] To world: [ 10 10 10 ] Back to frame: [ 0 0 9.53674e-07 ]

World coords: [ 10 10 10 ] To frame coords: [ 0 0 9.53674e-07 ] Back to world coords: [ 10 10 10 ]
Frame: [ 10 10 10 ] To world: [ 18.7621 23.9385 4.61995 ] Back to frame: [ 10 10 10 ]

World coords: [ 1 0 0 ] To frame coords: [ 0.707107 -0.408248 -16.7432 ] Back to world coords: [ 1 0 0 ]
Frame: [ 1 0 0 ] To world: [ 10.7071 10 9.29289 ] Back to frame: [ 1 2.38419e-07 9.53674e-07 ]

World coords: [ 0 0 1 ] To frame coords: [ -0.707107 -0.408248 -16.7432 ] Back to world coords: [ 0 0 1 ]
Frame: [ 0 0 1 ] To world: [ 10.5774 10.5774 10.5774 ] Back to frame: [ 0 0 1 ]

World vector: [ 1 0 0 ] To frame vector: [ 0.707107 -0.408248 0.577351 ] Back to world vector: [ 1 9.53674e-07 9.53674e-07 ]
Frame: [ 1 0 0 ] To world: [ 0.707107 0 -0.707107 ] Back to frame: [ 1 0 0 ]

World vector: [ 1 1 1 ] To frame vector: [ 0 0 1.73205 ] Back to world vector: [ 1 1 1 ]
Frame: [ 1 1 1 ] To world: [ 0.876209 1.39385 -0.538004 ] Back to frame: [ 1 1 1 ]
*/