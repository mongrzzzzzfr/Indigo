PROJECT(Reaction)

file (GLOB Reaction_src src/*.c*)
file (GLOB Reaction_headers *.h)

include_directories(${Common_SOURCE_DIR} ${Common_SOURCE_DIR}/..)

add_library(reaction OBJECT ${Reaction_src} ${Reaction_headers})

target_link_libraries(reaction molecule)

set_target_properties(reaction PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS}")
