#include "sb_buffer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h> /* Keep for fprintf in the macro */

/* --- Initialization --- */

/**
 * @brief Initializes the Buffer structure.
 * @param b Pointer to the Buffer structure.
 * @return int 1 on success, 0 if the input pointer is NULL.
 */
int sb_buffer_init(SB_Buffer *b) {
    /* Always check if the input pointer is NULL first */
    if (b == NULL) return 0; 
    
    /* Set the signature: Must be set before any other function is called */
    b->magic = SB_BUFFER_MAGIC;
    
    /* Initialization: Pointer points to the internal stack buffer (SSO) */
    b->len = 0;
    b->cap = SB_BUFFER_INITIAL_CAP;
    b->str = b->init;
    
    /* Ensure the string starts empty (null terminated) */
    b->str[0] = '\0';
    
    return 1;
}

/* --- Core Logic --- */

/**
 * @brief Adds a string of known length to the buffer.
 */
int sb_buffer_add_lstring(SB_Buffer *b, const char *s, size_t len){
    
    /* Safety check using macro defined in buffer.h */
    CHECK_SB_BUFFER_POINTER_RET(b, 0);
    
    size_t sobrando = b->cap - b->len;
    
    /* Check if capacity needs to be increased */
    if( len >= sobrando){ 
        
        size_t nova_cap = b->cap;
        
        /* Growth strategy: doubles until sufficient. */
        /* +1 ensures space for the null terminator. */
        while (nova_cap <= b->len + len + 1) { 
            nova_cap *= 2; 
        }
        
        if (b->str != b->init){ 
            /* Already malloc'd: now realloc it */
            char *novo_ptr = (char*)realloc(b->str, nova_cap);
            if (!novo_ptr) { return 0; } 
            b->str = novo_ptr;
            
        } else { 
            /* Currently using b->init: Migrate to malloc */
            char *novo_ptr = (char*)malloc(nova_cap);
            if (!novo_ptr) { return 0; }
            
            /* Copy the old content to the new heap area */
            memcpy(novo_ptr, b->init, b->len);
            
            b->str = novo_ptr;
        }
        
        b->cap = nova_cap;
    }
    
    /* Appends the new string at the end */
    memcpy(b->str + b->len, s, len);
    
    /* Updates the length and ensures null termination */
    b->len += len;
    b->str[b->len] = '\0'; 
    return 1;
}

/* --- Utility and Cleanup --- */

/**
 * @brief Returns the raw C string pointer.
 */
const char *sb_buffer_get_str(SB_Buffer *b){
    /* Returns NULL if pointer is invalid */
    CHECK_SB_BUFFER_POINTER_RET(b, NULL); 
    return b->str;	
}

/*get the length of the string*/
size_t sb_buffer_get_len(SB_Buffer *b){
    /* Returns 0 (zero) if pointer is invalid */
    CHECK_SB_BUFFER_POINTER_RET(b, 0); 
    return b->len;	
}

/**
 * @brief Releases memory allocated on the heap (Final Cleanup).
 */
int sb_buffer_finalize(SB_Buffer *b){
    /* Returns without action if pointer is invalid */
    CHECK_SB_BUFFER_POINTER_RET(b, 0); 
    
    if ( b->str != b->init){
        free(b->str);
    }
    
    /* For security: reset the structure state */
    b->str = b->init;
    b->str[0] = '\0';
    b->len = 0;
    b->cap = SB_BUFFER_INITIAL_CAP;
    
    return 1;
}

/**
 * @brief Resets the buffer to the initial stack state, freeing any heap memory (Hard Reset).
 */
int sb_buffer_reset(SB_Buffer *b) {
    /* Returns without action if pointer is invalid */
    CHECK_SB_BUFFER_POINTER_RET(b, 0);
    
    /* Free memory ONLY if b->str is NOT pointing to b->init */
    if (b->str != b->init) { 
        free(b->str);
    }
    
    /* Reset to initial stack state */
    b->len = 0;
    b->cap = SB_BUFFER_INITIAL_CAP;
    b->str = b->init;
    b->str[0] = '\0';
    
    return 1;
}

/**
 * @brief Clears the buffer content without freeing allocated heap memory (Soft Reset).
 */
int sb_buffer_clear(SB_Buffer *b) {
    /* Returns without action if pointer is invalid */
    CHECK_SB_BUFFER_POINTER_RET(b, 0);
    
    /* Only reset length and null terminator */
    b->len = 0;
    b->str[0] = '\0';
    
    return 1;
}

/**
 * @brief Checks if the given pointer is a valid, initialized Buffer structure.
 */
int sb_buffer_isbuffer(SB_Buffer *b){
    /* Uses explicit check without the macro to be clean */
    if ( (b) == NULL || (b)->magic != SB_BUFFER_MAGIC) { 
        return 0;
    }
    return 1;
}


/* copy src to dest */
int sb_buffer_copy(SB_Buffer *src, SB_Buffer *dest) {
	
    CHECK_SB_BUFFER_POINTER_RET(src, 0); 
    
    if (!sb_buffer_isbuffer(dest)) return 0;

    sb_buffer_reset(dest); 
    
    return sb_buffer_add_lstring(dest, src->str, src->len);
}
