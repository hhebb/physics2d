#version 330 core

// fragment shader

in vec4 fragColor;    // input: interpolated color as rgba-value
out vec4 finalColor;  // output: final color value as rgba-value

void main() {
  finalColor = fragColor;
  //finalColor = vec4(0.8, 0.2, 0.1, 1);
}