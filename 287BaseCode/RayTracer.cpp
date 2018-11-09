#include "RayTracer.h"
#include "IShape.h"

/**
 * @fn	RayTracer::RayTracer(const color &defa)
 * @brief	Constructs a raytracers.
 * @param	defa	The clear color.
 */

RayTracer::RayTracer(const color &defa)
	: defaultColor(defa) {
}

/**
 * @fn	void RayTracer::raytraceScene(FrameBuffer &frameBuffer, int depth, const IScene &theScene) const
 * @brief	Raytrace scene
 * @param [in,out]	frameBuffer	Framebuffer.
 * @param 		  	depth	   	The current depth of recursion.
 * @param 		  	theScene   	The scene.
 */

void RayTracer::raytraceScene(FrameBuffer &frameBuffer, int depth,
								const IScene &theScene) const {
	const RaytracingCamera &camera = *theScene.camera;
	const std::vector<VisibleIShapePtr> &objs = theScene.visibleObjects;
	const std::vector<PositionalLightPtr> &lights = theScene.lights;
	
	for (int y = 0; y < frameBuffer.getWindowHeight(); ++y)
	{
		for (int x = 0; x < frameBuffer.getWindowWidth(); ++x) {
			color colorForPixel = black;
			for(int subX = 0; subX < antiAliasing; subX++) { //divide each pixel into *antiAlising* rows
				for(int subY = 0; subY < antiAliasing; subY++){ //divide each pixel into *antiAliasing* col
					Ray ray = camera.getRay((float)x + ((float)subX / antiAliasing), (float)y + ((float)subY / antiAliasing));
					colorForPixel += traceIndividualRay(ray, theScene, depth)/((float)antiAliasing*antiAliasing);
				}
			}
			frameBuffer.setColor(x, y, colorForPixel);
		}
	}

	frameBuffer.showColorBuffer();
}

/**
 * @fn	color RayTracer::traceIndividualRay(const Ray &ray, const IScene &theScene, int recursionLevel) const
 * @brief	Trace an individual ray.
 * @param	ray			  	The ray.
 * @param	theScene	  	The scene.
 * @param	recursionLevel	The recursion level.
 * @return	The color to be displayed as a result of this ray.
 */

color RayTracer::traceIndividualRay(const Ray &ray, const IScene &theScene, int recursionLevel) const {
	HitRecord theHit = VisibleIShape::findIntersection(ray, theScene.visibleObjects);
	color result = defaultColor;
	bool inShadow = false;

	if (theHit.t < FLT_MAX) {
		if (theHit.texture != nullptr) { //if object has a texture, use it
			float u = glm::clamp(theHit.u, 0.0f, 1.0f);
			float v = glm::clamp(theHit.v, 0.0f, 1.0f);
			result = theHit.texture->getPixel(u, v);
		}
		else { //compute color normally (use illuminate / total color)
			for(int i = 0; i < theScene.lights.size(); i++) {
				result += theScene.lights.get(i).illuminate(theHit.interceptPoint, theHit.surfaceNormal,
				theHit.material, theScene.cameraFrame, inShadow);
			}
		}
	}
	return result;
}