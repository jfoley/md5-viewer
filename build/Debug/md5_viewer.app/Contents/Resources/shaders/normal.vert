#version 110

varying vec3 lightDir;
varying vec3 viewDir;
varying vec3 normal;
void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;

    vec3 vertexPos = vec3(gl_ModelViewMatrix * gl_Vertex);
    
    vec3 n = normalize(gl_NormalMatrix * gl_Normal);
    vec3 t = normalize(gl_NormalMatrix * gl_MultiTexCoord1.xyz);
    vec3 b = cross(n, t) * gl_MultiTexCoord1.w;
    
	normal = n;
    mat3 tbnMatrix = mat3(t.x, b.x, n.x,
                          t.y, b.y, n.y,
                          t.z, b.z, n.z);

    lightDir = (gl_LightSource[0].position.xyz - vertexPos);
    //lightDir = tbnMatrix * lightDir;
	lightDir = normalize(lightDir);

    viewDir = -vertexPos;
    viewDir = tbnMatrix * viewDir;
}

