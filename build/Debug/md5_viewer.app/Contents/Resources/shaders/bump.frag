uniform sampler2D textureMap;
uniform sampler2D normalMap;
	
//varying vec3 lightVec;
//varying vec3 eyeVec;
varying vec3 halfVec;
varying vec3 lightDir;


void main()
{

	// lookup normal from normal map, change from [0,1] to  [-1, 1] range
	vec3 normal = 2.0 * texture2D (normalMap, gl_TexCoord[0].st).rgb - 1.0;
	normal = normalize (normal);

	// ambient light
	//vec4 ambientLight = gl_LightSource[0].ambient;
	vec4 ambientLight = vec4(0.0); //looks like crap with ambient...
	
	// compute diffuse lighting
	float lamberFactor = max (dot (lightDir, normal), 0.0);
	vec4 diffuseMaterial = texture2D (textureMap, gl_TexCoord[0].st);
	vec4 diffuseLight  = gl_LightSource[0].diffuse;
  
	// compute specular lighting
	//TODO: change this to a texture lookup
	vec4 specularMaterial =  vec4(1.0);
	vec4 specularLight = gl_LightSource[0].specular;
	float shininess = pow (max (dot (halfVec, normal), 0.0), 2.0);

	gl_FragColor =	ambientLight;
	gl_FragColor += diffuseMaterial * diffuseLight * lamberFactor;
	gl_FragColor +=	specularMaterial * specularLight * shininess;
	
	//gl_FragColor = vec4(lightDir, 0.0);				
	//gl_FragColor = vec4(normal, 0.0);
	
}