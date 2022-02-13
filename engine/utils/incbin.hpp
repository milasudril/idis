#ifndef IDIS_UTILS_INCBIN_HPP
#define IDIS_UTILS_INCBIN_HPP

#include <cstddef>
#include <cstdint>

#define IDIS_INCBIN(identifier, filename)                                                          \
	asm(".pushsection .rodata\n"                                                                   \
	    "\t.local " #identifier "_begin\n"                                                         \
	    "\t.type " #identifier "_begin, @object\n"                                                 \
	    "\t.balign 16\n" #identifier "_begin:\n"                                                   \
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

#define IDIS_INCBIN_U32(identifier, filename)                                                      \
	asm(".pushsection .rodata\n"                                                                   \
	    "\t.local " #identifier "_begin\n"                                                         \
	    "\t.type " #identifier "_begin, @object\n"                                                 \
	    "\t.balign 4\n" #identifier "_begin:\n"                                                    \
	    "\t.incbin \"" filename "\"\n\n"                                                           \
                                                                                                   \
	    "\t.local " #identifier "_end\n"                                                           \
	    "\t.type " #identifier "_end, @object\n"                                                   \
	    "\t.balign 1\n" #identifier "_end:\n"                                                      \
	    "\t.ifgt (" #identifier "_end-" #identifier "_begin)&3\n"                                  \
	    "\t  .error \"Size of " filename " is not a multiple of 4\"\n"                             \
	    "\t.endif\n"                                                                               \
	    "\t.byte 0\n"                                                                              \
	    "\t.popsection\n");                                                                        \
                                                                                                   \
	extern uint32_t const identifier##_begin[];                                                    \
	extern uint32_t const identifier##_end[]

#endif