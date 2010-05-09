//varying vec3 lightVec;
//varying vec3 eyeVec;
varying vec3 halfVec;
varying vec3 lightDir;

attribute vec3 tangent;

void main()
{
	gl_TexCoord[0] =  gl_MultiTexCoord0;

	// go from eye space to tangent space
	vec3 n = normalize (gl_NormalMatrix * gl_Normal);
	vec3 t = normalize(gl_NormalMatrix * tangent);
	vec3 b = cross (n, t);
	
	vec3 vertexPosition = vec3(gl_ModelViewMatrix *  gl_Vertex);
	lightDir = normalize(gl_LightSource[0].position.xyz - vertexPosition);
	
	vec3 v;
	//v.x = dot (lightDir, t);
	//v.y = dot (lightDir, b);
	//v.z = dot (lightDir, n);
	//lightVec = normalize (v);
	
	  
	//v.x = dot (vertexPosition, t);
	//v.y = dot (vertexPosition, b);
	//v.z = dot (vertexPosition, n);
	//eyeVec = normalize (v);
	
	vertexPosition = normalize(vertexPosition);
	
	/* Normalize the halfVector to pass it to the fragment shader */
	vec3 halfVector = normalize((vertexPosition + lightDir) / 2.0);
	v.x = dot (halfVector, t);
	v.y = dot (halfVector, b);
	v.z = dot (halfVector, n);
	halfVec = v;
	
	gl_Position = ftransform();
}