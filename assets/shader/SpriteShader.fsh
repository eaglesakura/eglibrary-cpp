#version 100 /* GLES 2.0 */

// UV setting
varying mediump vec2 fTexCoord;
// texture
uniform sampler2D tex;
// color
uniform lowp vec4    blendColor;
uniform lowp float	colorOnly;

void main() {
    if(colorOnly != 0.0) {
    	gl_FragColor = blendColor;
    }else{
        gl_FragColor = texture2D(tex, fTexCoord) * blendColor;    
    }
}