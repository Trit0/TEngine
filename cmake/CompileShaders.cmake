function(compile_shader target)
    cmake_parse_arguments(PARSE_ARGV 1 arg "" "FORMAT" "SOURCES")

    foreach(source ${arg_SOURCES})
        get_filename_component(filename ${source} NAME)
        get_filename_component(filepath ${source} DIRECTORY)
        set(output_file ${filepath}/${filename}.${arg_FORMAT})

        string(TOUPPER ${arg_FORMAT} FORMAT_UPPER)

        add_custom_command(
                OUTPUT ${output_file}
                COMMAND ${glslc_executable} ${source} -o ${output_file}
                DEPENDS ${source}
                COMMENT "Compiling ${FORMAT_UPPER} shader ${filename}.${arg_FORMAT}"
        )

        target_sources(${target} PRIVATE ${output_file})
    endforeach()
endfunction()
