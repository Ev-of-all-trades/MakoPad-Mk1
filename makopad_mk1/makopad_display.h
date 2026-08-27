#pragma once

#include "quantum.h"

// ============================================================================
// MakoPad Display API
// ============================================================================

void makopad_display_init(void);

void makopad_display_update(void);

void makopad_display_mark_dirty(void);

bool makopad_display_is_ready(void);