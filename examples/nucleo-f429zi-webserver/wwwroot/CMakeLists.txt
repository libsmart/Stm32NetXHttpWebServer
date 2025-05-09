cmake_minimum_required(VERSION 3.27)
project(wwwroot)

# Sammle alle Dateien im public-Verzeichnis
file(GLOB_RECURSE PUBLIC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/public/*.*")

# Generiere .h Dateien für jede Datei
foreach(SOURCE_FILE ${PUBLIC_FILES})
    # Extrahiere den relativen Pfad ab public/
    file(RELATIVE_PATH REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/public" ${SOURCE_FILE})
    string(REPLACE "/" "_" VARIABLE_NAME "${REL_PATH}")

    # Erstelle den Output-Dateinamen
    set(OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${VARIABLE_NAME}.h")

    # Füge custom command für jede Datei hinzu
    add_custom_command(
            OUTPUT ${OUTPUT_FILE}
            COMMAND "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_SOURCE_DIR}/public" xxd -i "${REL_PATH}" "${OUTPUT_FILE}"
            DEPENDS ${SOURCE_FILE}
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/public"
    )

    list(APPEND GENERATED_HEADERS ${OUTPUT_FILE})
endforeach()

# Erstelle eine zusammenfassende Header-Datei
set(COMBINED_HEADER "${CMAKE_CURRENT_BINARY_DIR}/wwwroot.h")
file(WRITE ${COMBINED_HEADER} "")
file(APPEND ${COMBINED_HEADER} "#ifndef WWWROOT_H\n")
file(APPEND ${COMBINED_HEADER} "#define WWWROOT_H\n")
#file(APPEND ${COMBINED_HEADER} "namespace wwwroot {\n\n")
foreach(HEADER ${GENERATED_HEADERS})
    get_filename_component(HEADER_NAME ${HEADER} NAME)
    file(APPEND ${COMBINED_HEADER} "#include \"${HEADER_NAME}\"\n")
endforeach()
file(APPEND ${COMBINED_HEADER} "#endif\n\n")
#file(APPEND ${COMBINED_HEADER} "\n}\n\n")

# Erstelle die Bibliothek mit allen generierten Header-Dateien
add_library(wwwroot STATIC ${COMBINED_HEADER} wwwroot.c ${GENERATED_HEADERS})

# Füge Include-Verzeichnis hinzu
target_include_directories(wwwroot PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)
