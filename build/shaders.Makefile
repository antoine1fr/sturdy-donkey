SHADERS = \
	shaders/albedo-pass.frag.glsl \
	shaders/ambient-pass.frag.glsl \
	shaders/gbuffer-pass.frag.glsl \
	shaders/gbuffer-pass.vert.glsl \
	shaders/light-pass.frag.glsl \
	shaders/simple.vert.glsl

shaders: $(SHADERS)

.PHONY: shaders

%.frag.glsl: %.frag.glsl.m4
	m4 --prefix-builtins -Ishaders $< > $@

%.vert.glsl: %.vert.glsl.m4
	m4 --prefix-builtins -Ishaders  $< > $@
