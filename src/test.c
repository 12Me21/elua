#define lj_parse_c
#define LUA_CORE
#include "lj_obj.h"
#include "lj_gc.h"
#include "lj_err.h"
#include "lj_debug.h"
#include "lj_buf.h"
#include "lj_str.h"
#include "lj_tab.h"
#include "lj_func.h"
#include "lj_state.h"
#include "lj_bc.h"
#if LJ_HASFFI
#include "lj_ctype.h"
#endif
#include "lj_strfmt.h"
#include "lj_lex.h"
#include "lj_parse.h"
#include "lj_vm.h"
#include "lj_vmevent.h"
typedef enum {
	/* Constant expressions must be first and in this order: */
	VKNIL,
	VKFALSE,
	VKTRUE,
	VKSTR,	/* sval = string value */
	VKNUM,	/* nval = number value */
	VKLAST = VKNUM,
	VKCDATA,	/* nval = cdata value, not treated as a constant expression */
	VLOCAL,	/* info = local register, aux = vstack index */
	VUPVAL,	/* info = upvalue index, aux = vstack index */
	VGLOBAL,	/* sval = string value */
	VINDEXED,	/* info = table register, aux = index reg/byte/string const */
	VJMP,		/* info = instruction PC */
	VRELOCABLE,	/* info = instruction PC */
	VNONRELOC,	/* info = result register */
	VCALL,	/* info = instruction PC, aux = base */
	VVOID
} ExpKind;
/* Expression descriptor. */
typedef struct ExpDesc {
	union {
		struct {
			uint32_t info;	/* Primary info. */
			uint32_t aux;	/* Secondary info. */
    } s;
