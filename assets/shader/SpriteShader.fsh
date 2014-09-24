// UV setting
varying mediump vec2 fTexCoord;
// texture
uniform sampler2D tex;
// color
uniform lowp vec4    blendColor;
uniform bool	colorOnly;

void main() {
    if(colorOnly) {
    	gl_FragColor = blendColor;
    }else{
        gl_FragColor = texture2D(tex, fTexCoord) * blendColor;    
    }
}