add_subdirectory(squirrel)
add_subdirectory(sqrat)
add_subdirectory(Union)
add_subdirectory(Hacks)

target_link_libraries(G2O_ExtendedTools PUBLIC Squirrel)
target_link_libraries(G2O_ExtendedTools PUBLIC SqRat)
target_link_libraries(G2O_ExtendedTools PUBLIC Union)
target_link_libraries(G2O_ExtendedTools PUBLIC Hacks)