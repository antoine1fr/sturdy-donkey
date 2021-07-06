set(SHADER_DIR "${CMAKE_CURRENT_LIST_DIR}/shaders")
set(SHADER_FILES
  "${SHADER_DIR}/albedo-pass.frag.glsl"
	"${SHADER_DIR}/ambient-pass.frag.glsl"
	"${SHADER_DIR}/gbuffer-pass.frag.glsl"
	"${SHADER_DIR}/gbuffer-pass.vert.glsl"
	"${SHADER_DIR}/light-pass.frag.glsl"
	"${SHADER_DIR}/simple.vert.glsl"
)

#foreach(FILE ${SHADER_FILES})
#  add_custom_command(
#    OUTPUT ${FILE}
#    COMMAND m4 --prefix-builtins -I${SHADER_DIR} "${FILE}.m4" > ${FILE}
#    DEPENDS "${FILE}.m4"
#    WORKING_DIRECTORY "${SHADER_DIR}"
#  )
#endforeach()

#add_custom_target(shaders ALL
#  DEPENDS ${SHADER_FILES})

message("[shaders] CMAKE_C_COMPILE_OBJECT: ${CMAKE_C_COMPILE_OBJECT}")
