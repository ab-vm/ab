add_custom_target(manpages ALL)
INSTALL(
	FILES
		ab-asm.1
		ab-disasm.1
		ab-run.1
		ab.1
	DESTINATION
		share/man/man1
	PERMISSIONS
		WORLD_READ
		OWNER_WRITE
	COMPONENT
		documentation
)
