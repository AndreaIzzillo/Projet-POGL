#version 430

uniform mat4 uView;
uniform mat4 uProjection;
uniform vec2 uResolution;

uniform vec3 uBlackHolePosition;
uniform float uEventHorizonRadius;
uniform float uInfluenceRadius;

uniform float uDistortionStrength;
uniform float uDistortionFalloff;
uniform float uMaxOffset;

uniform vec3 uRingColor;
uniform float uRingIntensity;

uniform sampler2D uScene;

out vec4 FragColor;

float screenRadius(float R, float dist) {
    return R * uProjection[1][1] / (2.0 * dist);
}

void main() {
    float aspect = uResolution.x / uResolution.y;
    vec2 uv = gl_FragCoord.xy / uResolution;

    vec4 centerView = uView * vec4(uBlackHolePosition, 1.0);
    float centerDepth = -centerView.z;
    vec4 centerClip = uProjection * centerView;

    // Skip fragments behind the camera to avoid invalid projections.
    if (centerDepth <= 0.0 || centerClip.w <= 0.0) {
        FragColor = vec4(0.0);
        return;
    }

    vec2 centerUV = (centerClip.xy / centerClip.w) * 0.5 + 0.5;

    vec2 toCenter = vec2((centerUV.x - uv.x) * aspect, centerUV.y - uv.y);
    float r = length(toCenter);

    float eventR = screenRadius(uEventHorizonRadius, centerDepth);
    float influenceR = screenRadius(uInfluenceRadius, centerDepth);

    float t = clamp((influenceR - r) / max(influenceR - eventR, 1e-4), 0.0, 1.0);
    float lens = pow(t, uDistortionFalloff);

    vec2 dir = (r > 1e-5) ? toCenter / r : vec2(0.0);
    vec2 offset = dir * lens * uDistortionStrength * eventR;

    float offLen = length(offset);
    if (offLen > uMaxOffset)
        offset *= uMaxOffset / offLen;

    vec2 sampleUV = uv + vec2(offset.x / aspect, offset.y);

    vec2 outAmt = max(-sampleUV, sampleUV - 1.0);
    float outside = max(max(outAmt.x, outAmt.y), 0.0);
    vec3 bg = texture(uScene, clamp(sampleUV, 0.0, 1.0)).rgb;
    bg = mix(bg, vec3(0.0), smoothstep(0.0, 0.03, outside));

    float hole = 1.0 - smoothstep(eventR * 0.97, eventR, r);
    vec3 color = mix(bg, vec3(0.0), hole);

    float ring = 1.0 - smoothstep(0.0, eventR * 0.10, abs(r - eventR * 1.04));
    color += uRingColor * ring * uRingIntensity;

    float alpha = 1.0 - smoothstep(influenceR * 0.97, influenceR, r);

    FragColor = vec4(color, alpha);
}
