#include "Light.h"

/**
 * @fn	color ambientColor(const color &mat, const color &light)
 * @brief	Computes the ambient color produced by a single light at a single point.
 * @param	mat  	Ambient material property.
 * @param	light	Light's ambient color.
 * @return	Ambient color.
  */

color ambientColor(const color &mat, const color &light) {
	//return mat*light; //the star operator is pt. wise operation (NOT DOT PRODUCT) --> returns a triplet of three pieces
	return glm::clamp(mat*light, 0.0f, 1.0f); //glm clamp it closes the value if greater than one > becomes 1, < 0 becomes 0 (etc.)
}

/**
 * @fn	color diffuseColor(const color &mat, const color &light, const glm::vec3 &l, const glm::vec3 &n)
 * @brief	Computes diffuse color produce by a single light at a single point.
 * @param	mat		 	Material.
 * @param	light	 	The light.
 * @param	l		 	Light vector.
 * @param	n		 	Normal vector.
 * @return	Diffuse color.
 */

color diffuseColor(const color &mat, const color &light,
					const glm::vec3 &l, const glm::vec3 &n) {
	return glm::clamp(mat*light*fmax(0.0f, glm::dot(l, n)),0.0f, 1.0f);
}

/**
 * @fn	color specularColor(const color &mat, const color &light, float shininess, const glm::vec3 &r, const glm::vec3 &v)
 * @brief	Computes specular color produce by a single light at a single point.
 * @param	mat		 	Material.
 * @param	light	 	The light's color.
 * @param	shininess	Material shininess.
 * @param	r		 	Reflection vector.
 * @param	v		 	Viewing vector.
 * @return	Specular color.
 */

color specularColor(const color &mat, const color &light,
					float shininess,
					const glm::vec3 &r, const glm::vec3 &v) {
	return glm::clamp(mat*light*powf(fmax(0.0f, glm::dot(v, r)), shininess), 0.0f, 1.0f);
}

/**
 * @fn	color totalColor(const Material &mat, const LightColor &lightColor, const glm::vec3 &viewingDir, const glm::vec3 &normal, const glm::vec3 &lightPos, const glm::vec3 &intersectionPt, bool attenuationOn, const LightAttenuationParameters &ATparams)
 * @brief	Color produced by a single light at a single point.
 * @param	mat			  	Material.
 * @param	lightColor	  	The light's color.
 * @param	v	  			The v vector.
 * @param	normal		  	Normal vector.
 * @param	lightPos	  	Light position.
 * @param	intersectionPt	(x,y,z) of intersection point.
 * @param	attenuationOn 	true if attenuation is on.
 * @param	ATparams	  	Attenuation parameters.
 * @return	Color produced by a single light at a single point.
 */
 
//make all of the calls ^ to above equations, then sum them up. (IF attenuation is on, you do it's computation)
color totalColor(const Material &mat, const LightColor &lightColor,
				const glm::vec3 &v, const glm::vec3 &n,
				const glm::vec3 &lightPos, const glm::vec3 &intersectionPt,
				bool attenuationOn, 
				const LightAttenuationParameters &ATparams) {

	float attenuationFactor = 1;

	if (DEBUG_PIXEL) {
		std::cout << std::endl;
	}

	if (attenuationOn) {
		float distance = glm::distance(lightPos, intersectionPt);
		attenuationFactor = 1 / (ATparams.constant + ATparams.linear*distance + ATparams.quadratic*pow(distance, 2));
	}
	
	glm::vec3 lightVect = glm::normalize(lightPos - intersectionPt);
	glm::vec3 reflect = 2.0f * glm::dot(lightVect, n) * n - lightVect;

	return ambientColor(mat.ambient,lightColor.ambient) + attenuationFactor * diffuseColor(mat.diffuse,lightColor.diffuse,lightVect,n) + 
		attenuationFactor * specularColor(mat.specular,lightColor.specular,mat.shininess,reflect,v);

}

/**
 * @fn	color PositionalLight::illuminate(const HitRecord &hit, const glm::vec3 &viewingDir, const Frame &eyeFrame, bool inShadow) const
 * @brief	Computes the color this light produces in raytracing applications.
 * @param	hit					The surface properties of the intercept point.
 * @param	v					The v vector
 * @param	eyeFrame			The coordinate frame of the camera.
 * @param	inShadow			true if the point is in a shadow.
 * @return	The color produced at the intercept point, given this light.
 */

color PositionalLight::illuminate(const glm::vec3 &interceptWorldCoords,
									const glm::vec3 &normal,
									const Material &material,
									const Frame &eyeFrame, bool inShadow) const {
	if (!isOn) return black;
	//if (inShadow) return ambient(****); //returns ambient if object is in the shadow
	if (inShadow)
		return ambientColor(material.ambient, this->lightColorComponents.ambient);
	else
		return totalColor(material, this->lightColorComponents, glm::normalize(eyeFrame.origin-interceptWorldCoords), normal,
		this->lightPosition, interceptWorldCoords, this->attenuationIsTurnedOn, this->attenuationParams);
		//return totalColor(material.am)
}

/**
 * @fn	color SpotLight::illuminate(const HitRecord &hit, const glm::vec3 &viewingDir, const Frame &eyeFrame, bool inShadow) const
 * @brief	Computes the color this light produces in raytracing applications.
 * @param	hit					The surface properties of the intercept point.
 * @param	v					The v vector
 * @param	eyeFrame			The coordinate frame of the camera.
 * @param	inShadow			true if the point is in a shadow.
 * @return	The color produced at the intercept point, given this light.
 */

color SpotLight::illuminate(const glm::vec3 &interceptWorldCoords,
							const glm::vec3 &normal,
							const Material &material,
							const Frame &eyeFrame, bool inShadow) const {

	bool isOutsideCone = false;
	if(glm::dot(glm::normalize(interceptWorldCoords - this->origin), this->spotDirection)
		> glm::cos(this->fov/2.0f))
		isOutsideCone = true;

	if (!isOn)
		return black;

	if(isOutsideCone)
		return black;

	if(inShadow)
		return ambientColor(material.ambient, this->lightColorComponents.ambient);

	return totalColor(material, this->lightColorComponents, glm::normalize(eyeFrame.origin - interceptWorldCoords), normal,
					  this->lightPosition, interceptWorldCoords, this->attenuationIsTurnedOn, this->attenuationParams);
}

/**
* @fn	ostream &operator << (std::ostream &os, const LightAttenuationParameters &at)
* @brief	Output stream for light attenuation parameters.
* @param	os		Output stream.
* @param	at		Attenuation parameters.
* @return	The output stream.
*/

std::ostream &operator << (std::ostream &os, const LightAttenuationParameters &at) {
	os << glm::vec3(at.constant, at.linear, at.quadratic) << std::endl;
	return os;
}

/**
* @fn	ostream &operator << (std::ostream &os, const PositionalLight &pl)
* @brief	Output stream for light attenuation parameters.
* @param	os		Output stream.
* @param	pl		Positional light.
* @return	The output stream.
*/

std::ostream &operator << (std::ostream &os, const PositionalLight &pl) {
	os << (pl.isOn ? "ON" : "OFF") << std::endl;
	os << (pl.isTiedToWorld? "WORLD" : "CAMERA") << std::endl;
	os << " position " << pl.lightPosition << std::endl;
	os << " ambient " << pl.lightColorComponents.ambient << std::endl;
	os << " diffuse " << pl.lightColorComponents.diffuse << std::endl;
	os << " specular " << pl.lightColorComponents.specular << std::endl;
	os << "Attenuation: " << (pl.attenuationIsTurnedOn ? "ON" : "OFF")
		<< " " << pl.attenuationParams << std::endl;
	return os;
}

/**
* @fn	ostream &operator << (std::ostream &os, const SpotLight &sl)
* @brief	Output stream for light attenuation parameters.
* @param	os		Output stream.
* @param	sl		Spotlight.
* @return	The output stream.
*/

std::ostream &operator << (std::ostream &os, const SpotLight &sl) {
	PositionalLight pl = (sl);
	os << pl;
	os << " FOV " << sl.fov << std::endl;
	return os;
}