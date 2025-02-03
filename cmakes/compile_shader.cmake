include(utilities.cmake)

find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)
find_program(SPIRV_OPT spirv-opt HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)

function(compile_glsl_shaders PROJECT_TARGET TARGET_ENV TARGET_ENV_OPT SHADER_DIR SHADER_C_DIR COMPILED_SHADER_DIR)
  if(NOT GLSL_VALIDATOR)
    message(STATUS "Can't find the glslValidator")
    return()
  else()
    message(STATUS "glslValidator is found")
  endif()

  if(NOT SPIRV_OPT)
    message(STATUS "Cant't find the spirv-opt")
  else()
    message(STATUS "spirv-opt is found")
  endif()

  file(GLOB_RECURSE GLSL_FILES
        "${SHADER_DIR}/*.frag"
        "${SHADER_DIR}/*.vert"
        "${SHADER_DIR}/*.comp"
        "${SHADER_DIR}/*.mesh"
        "${SHADER_DIR}/*.task"
        "${SHADER_DIR}/*.tesc"
        "${SHADER_DIR}/*.tese"
        "${SHADER_DIR}/*.geom"
        "${SHADER_DIR}/*.rgen"
        "${SHADER_DIR}/*.rint"
        "${SHADER_DIR}/*.rahit"
        "${SHADER_DIR}/*.rchit"
        "${SHADER_DIR}/*.rmiss"
        "${SHADER_DIR}/*.rcall"
    )

  set(IS_VULKAN_ENV string_starts_with(${TARGET_ENV} vulkan))

  if(IS_VULKAN_ENV EQUAL true)
    set(GEN_BIN_FLAG -V)
  else()
    set(GEN_BIN_FLAG -G)
  endif()

  foreach(GLSL_FILE ${GLSL_FILES})
    get_filename_component(GLSL_FILENAME ${GLSL_FILE} NAME_WE)
    get_filename_component(GLSL_FILE_EXT ${GLSL_FILE} LAST_EXT)
    string(REPLACE "." "" GLSL_FILE_EXT ${GLSL_FILE_EXT})

    set(SPIRV_FILENAME ${GLSL_FILENAME}_${GLSL_FILE_EXT})

    set(SPIRV_FILE "${COMPILED_SHADER_DIR}/${SPIRV_FILENAME}.spv")

    set(SPIRV_C_NAME "${SPIRV_FILENAME}_spv")
    set(SPIRV_C_HEADER_FILE "${SHADER_C_DIR}/${SPIRV_C_NAME}.h")

    add_custom_target(
            ${SPIRV_FILENAME}_SPIRV_TARGET
            COMMAND ${GLSL_VALIDATOR} ${GEN_BIN_FLAG} ${GLSL_FILE} -o ${SPIRV_FILE} -e main -t --enhanced-msgs --target-env ${TARGET_ENV} -I${SHADER_DIR} --spirv-val
        )

    add_custom_target(
            ${SPIRV_FILENAME}_SPIRV_C_TARGET
            COMMAND ${GLSL_VALIDATOR} ${GEN_BIN_FLAG} ${GLSL_FILE} -o ${SPIRV_C_HEADER_FILE} -e main -t --enhanced-msgs --target-env ${TARGET_ENV} -I${SHADER_DIR} --vn ${SPIRV_C_NAME} --spirv-val
        )

    add_dependencies(${PROJECT_TARGET} ${SPIRV_FILENAME}_SPIRV_TARGET ${SPIRV_FILENAME}_SPIRV_C_TARGET)

    if(NOT SPIRV_OPT)
      continue()
    endif()

    set(SPIRV_OPT_NAME "${SPIRV_FILENAME}_opt")
    set(SPIRV_OPT_FILE "${COMPILED_SHADER_DIR}/${SPIRV_OPT_NAME}.spv")

    add_custom_target(
            ${SPIRV_FILENAME}_SPIRV_OPT_TARGET
            COMMAND ${SPIRV_OPT} ${SPIRV_FILE} -O -o ${SPIRV_OPT_FILE} --strip-debug --target-env=${TARGET_ENV_OPT} --upgrade-memory-model --remove-duplicates --amd-ext-to-khr
        )

    add_dependencies(${PROJECT_TARGET} ${SPIRV_FILENAME}_SPIRV_OPT_TARGET)
  endforeach()
endfunction()
