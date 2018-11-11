//This fragment shader implements the Phong Reflection Model and Examples 8.4, 8.12, and 8.20 in the OpenGL 
//Programming Guide (8th edition) to draw a brick pattern on a polygon using Procedural Texturing.   
//ACKNOWLEDGEMENT: I got some help from: http://www.gamedev.net/topic/658486-glsl-send-array-receive-struct/ on how 
//to pass the array of structs with the light info to the shader.

#version 420 core

struct LightProperties
{
	vec4 color;			//color of light
	vec4 position;		//location of light if w = 1,
						//otherwise the direction towards the light
	
	//spotlight attributes (can only use if light is local, i.e. position.w = 1)
	vec4 spotLightValues;	//if spotLightValues.x > 0 then it is a spotLight
							//spotLightValues.y is spotCosCuttoff
							//spotLightValues.z is spotExponent
	vec4 spotConeDirection;  //direction that the spotlight is shinining

	//local light attenuation coefficients (position.w must be 1 to use these)
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;

	uint isEnabled;
};

layout (binding = 0, r32ui) uniform uimage2D output_buffer;


const int MAXLIGHTS = 3;  //Number of lights the shader should be able to support
layout(binding = 0,std140) uniform Lights
{
	LightProperties lights[MAXLIGHTS];
};
uniform int totalLights;  //Number of lights in this application
uniform vec3 globalAmbientLight;

uniform mat4 viewMatrix; //needed to transform the light positions into "eye" coordinates

in vec4 pos; //position of the fragment in "eye" coordinates
in vec4 fColor;
in vec3 normal; //orientation of the normal in "eye" coordinates
in vec2 fragmentUV;

uniform sampler2D texSampler;

out vec4 FragColor;

void main()
{
	//vec3 color = vec3(0, 0, 1);
	vec3 color;
	double alpha = 1;
	if(fColor.w < 0) {
		vec4 textureColor = texture(texSampler, fragmentUV);
		color = vec3(textureColor.r, textureColor.g, textureColor.b);
		alpha = textureColor.a;
		alpha = 0.1;
	} else {
		color = vec3(fColor.r, fColor.g, fColor.b);
		alpha = fColor.a;
	}
	vec3 surfaceDiffuseColor = color; //use the color computed by the procedural texture code as the diffuse color 
	vec3 surfaceSpecularColor = vec3(0.0, 0.0, 0.0); //Make it black so that there will be no specular highlights
	vec3 scatteredLight = globalAmbientLight*surfaceDiffuseColor; //initialize scatteredLight to the global ambience*fragment color
	vec3 reflectedLight = vec3(0.0, 0.0, 0.0);
	
	//first, loop over all of the lights
	for (int lightNum = 0; lightNum < totalLights; lightNum++)
	{
		if (lights[lightNum].isEnabled == 1)  //Make sure the light is on
		{
			vec3 I = lights[lightNum].color.rgb;  //Intensity (color) of the light
			vec3 L; //this will be the L vector in the Phong reflectance model (lightDirection in the code on pages 377 and 378)
			vec4 lightPos_eyeCoords = viewMatrix*lights[lightNum].position; //put light position in "eye" coordinates
			vec3 n = normal;

			float f; //attenuation in the phong reflectance model
				
			if (lights[lightNum].position.w > 0.1)  //a point light
			{
				vec4 LTemp = lightPos_eyeCoords - pos;  //Calculate the L vector
				L = vec3(LTemp.x, LTemp.y, LTemp.z); //remove the w value from the L vector
				float d = length(L);  //d in the phong reflectance model (lightDistance in the code on pages 377 and 378)
				L = L / d; //normalize L

				//Calculate the attenuation factor, f in the phong reflectance model
				float attenuationDenominator =	lights[lightNum].constantAttenuation +
												lights[lightNum].linearAttenuation * d +
												lights[lightNum].quadraticAttenuation * d * d;
				if (attenuationDenominator == 0) attenuationDenominator = 1;  //make sure we don't divide by 0
				f = 1.0 / attenuationDenominator;

				if (lights[lightNum].spotLightValues.x > 0)  //This is a spotLight
				{
					vec4 spotConeDir_eyeCoords = viewMatrix*lights[lightNum].spotConeDirection;  
					vec3 spotVector = -normalize(vec3(spotConeDir_eyeCoords.x, spotConeDir_eyeCoords.y, spotConeDir_eyeCoords.z));
					float spotCos = dot (L, spotVector);
					if (spotCos < lights[lightNum].spotLightValues.y)  //Spotlight should not be visible
					{
						f = 0; //negate this light's effects
					}
					else //spotlight is shining on this fragment
					{
						f *= pow (spotCos, lights[lightNum].spotLightValues.z);  //Remember, spotLightValues.z is the spotExponent
					}
				}
			}
			else //directional light
			{
				L = normalize(vec3(lightPos_eyeCoords.x, lightPos_eyeCoords.y, lightPos_eyeCoords.z)); //remove w value to form the L vector and normalize
				f = 1.0;  //no attenuation for directional lights
			}

			float diffuseModifier = max ( 0.0, dot(n, L) );
			float specularModifier = 0.0;
			vec4 viewerPos = vec4(0.0, 0.0, 0.0, 1.0); //The viewer is at (0,0,0) in eye space
			vec4 vTemp = viewerPos - pos;
			vec3 v = normalize(vec3(vTemp.x, vTemp.y, vTemp.z)); //calculate v, the vector towards the viewer
			if (diffuseModifier > 0.001) //if little or no diffuse lighting, then there should not be a specular 
			{							 //highlight since the light is not shining directly on the object	 
				vec3 r = normalize( reflect (-L, n) );  //find r by "reflecting" lightDirection (L) around n
				specularModifier = pow (max ( 0.0, dot(r, v)), 1 ); //this 1 is the shininess
			}
			//Accumulate all the light's effects
			reflectedLight +=	f *													//attenuation
								( (I * surfaceDiffuseColor * diffuseModifier) +		//diffuse term
								 (I * surfaceSpecularColor * specularModifier) );	//specular term
		}
	}
	vec3 sumOfLights = scatteredLight + reflectedLight;
	vec3 rgb = min ( sumOfLights, vec3(1.0, 1.0, 1.0) );  //clamp lighting at all white
	FragColor = vec4(rgb.r, rgb.g, rgb.b, alpha);  //use the fragment's original alpha
	//FragColor = vec4(textureColor.x * lightColor.x, textureColor.y * lightColor.y, textureColor.z * lightColor.z, textureColor.w);
	imageAtomicAdd(output_buffer, ivec2(gl_FragCoord.xy), 1); //See page 581 in the OpenGL Programming Guide (8th edition)  	
}