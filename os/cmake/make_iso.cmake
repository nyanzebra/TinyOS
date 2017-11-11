# Setup a directory for the ISO
file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/isofiles ${CMAKE_BINARY_DIR}/isofiles/boot ${CMAKE_BINARY_DIR}/isofiles/boot/grub)

file(COPY ${CMAKE_SOURCE_DIR}/kern/${ARCHITECTURE}/${PLATFORM}/grub.cfg DESTINATION ${CMAKE_BINARY_DIR}/isofiles/boot/grub)

find_program(MAKE_RESCUE /users/robertbaldwin/opt/bin/grub-mkrescue REQUIRED)

add_custom_target(
	iso # Make target
	COMMAND cp ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin ${CMAKE_BINARY_DIR}/isofiles/boot/${PROJECT_NAME}.bin
	DEPENDS ${PROJECT_NAME}.bin
)

add_custom_command(
	COMMAND ${MAKE_RESCUE}
	ARGS -o ${PROJECT_NAME}.iso ${CMAKE_BINARY_DIR}/isofiles
	TARGET iso
)
