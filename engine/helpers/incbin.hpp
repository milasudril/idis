#ifndef IDIS_HELPERS_INCBIN_HPP
#define IDIS_HELPERS_INCBIN_HPP

#include <cstddef>

#define IDIS_INCBIN(identifier, filename)                                                          \
	asm(".pushsection .rodata\n"                                                                   \
	    "\t.local " #identifier "_begin\n"                                                         \
	    "\t.type " #identifier "_begin, @object\n"                                                 \
	    "\t.align 16\n" #identifier "_begin:\n"                                                    \
	    "\t.incbin \"" filename "\"\n\n"                                                           \
                                                                                                   \
	    "\t.local " #identifier "_end\n"                                                           \
	    "\t.type " #identifier "_end, @object\n"                                                   \
	    "\t.align 1\n" #identifier "_end:\n"                                                       \
	    "\t.byte 0\n"                                                                              \
	    "\t.popsection\n");                                                                        \
                                                                                                   \
	extern std::byte const identifier##_begin[];                                                   \
	extern std::byte const identifier##_end[]

#endif