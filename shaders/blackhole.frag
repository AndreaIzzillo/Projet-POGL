#version 430

in vec3 vViewNormal;
in vec3 vViewPosition;

uniform mat4 uView;
uniform mat4 uProjection;
uniform vec2 uResolution;

uniform vec3 uBlackHolePosition;   // World-space center
uniform float uEventHorizonRadius; // World units (the solid black disk)
uniform float uInfluenceRadius;    // World units (where distortion fades to zero)

uniform float uDistortionStrength; // How hard UVs are pushed toward the center
uniform float uDistortionFalloff;  // Shapes the falloff from center to edge

uniform vec3 uGlowColor;
uniform float uGlowIntensity;
uniform float uGlowFalloff;

uniform sampler2D uScene; // The whole scene, captured before this pass

out vec4 FragColor;

// uProjection[1][1] == 1 / tan(fovY / 2): the screen-space (vertical-UV) radius of a world
// radius R seen at view-space depth `dist`. Using the projection here also makes the effect
// scale with the black hole's apparent on-screen size as you move closer / further away.
float screenRadius(float R, float dist) {
    return R * uProjection[1][1] / (2.0 * dist);
}

void main() {
    float aspect = uResolution.x / uResolution.y;
    vec2 uv = gl_FragCoord.xy / uResolution;

    // Project the black hole center into screen space.
    vec4 centerView = uView * vec4(uBlackHolePosition, 1.0);
    float centerDepth = -centerView.z;
    vec4 centerClip = uProjection * centerView;

    // Center is behind the camera: nothing to lens here, stay invisible.
    if (centerDepth <= 0.0 || centerClip.w <= 0.0) {
        FragColor = vec4(0.0);
        return;
    }

    vec2 centerUV = (centerClip.xy / centerClip.w) * 0.5 + 0.5;

    // Aspect-corrected vector / distance from this pixel to the center (so it stays circular).
    vec2 toCenter = vec2((centerUV.x - uv.x) * aspect, centerUV.y - uv.y);
    float r = length(toCenter);

    float eventR = screenRadius(uEventHorizonRadius, centerDepth);
    float influenceR = screenRadius(uInfluenceRadius, centerDepth);

    // Lensing mask: 1 at the event horizon, fading to 0 at the influence edge.
    float t = clamp((influenceR - r) / max(influenceR - eventR, 1e-4), 0.0, 1.0);
    float lens = pow(t, uDistortionFalloff);

    // Push the sampled UV toward the center, scaled by the on-screen black hole size.
    vec2 dir = (r > 1e-5) ? toCenter / r : vec2(0.0);
    vec2 offset = dir * lens * uDistortionStrength * eventR;
    vec2 sampleUV = uv + vec2(offset.x / aspect, offset.y);

    vec3 bg = texture(uScene, clamp(sampleUV, 0.0, 1.0)).rgb;

    // Event horizon: a solid black disk with a soft edge.
    float hole = 1.0 - smoothstep(eventR * 0.97, eventR, r);
    vec3 color = mix(bg, vec3(0.0), hole);

    // Faint fresnel rim glow on the sphere silhouette.
    vec3 N = normalize(vViewNormal);
    vec3 V = normalize(-vViewPosition);
    float fresnel = pow(1.0 - abs(dot(N, V)), uGlowFalloff);
    color += uGlowColor * fresnel * uGlowIntensity;

    // Feather only the very outer ring so the polygonal silhouette blends in seamlessly.
    float alpha = 1.0 - smoothstep(influenceR * 0.97, influenceR, r);

    FragColor = vec4(color, alpha);
}
