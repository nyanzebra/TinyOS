# Setup a directory for the ISO
file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/isofiles ${CMAKE_BINARY_DIR}/isofiles/boot ${CMAKE_BINARY_DIR}/isofiles/boot/grub)

set(QEMU_SYSTEM "qemu-system-${ARCHITECTURE}")
#-S for debug
set(QEMU_ARGS "-cdrom ${CMAKE_BINARY_DIR}/isofiles/${PROJECT_NAME}.iso -s -m 128M -D ${CMAKE_BINARY_DIR}/qemu")
find_program(QEMU ${QEMU_SYSTEM} REQUIRED)

add_custom_target(
	run # Make target
	# COMMAND cp ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.iso ${CMAKE_BINARY_DIR}/isofiles/${PROJECT_NAME}.iso
	# DEPENDS ${PROJECT_NAME}.iso
)

add_custom_command(
	COMMAND ${QEMU}
	ARGS -cdrom ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.iso -s -m 128M
	TARGET run
)
