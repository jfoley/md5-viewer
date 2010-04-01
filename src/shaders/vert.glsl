//  Identity vertex shader

uniform float time;

void main()
{
   //  Use color unchanged
   gl_FrontColor = gl_Color;
   //  Set vertex coordinates
   gl_Position = ftransform();
}
