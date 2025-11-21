/*!
 * @file buffer.h
 * @brief Defines the Buffer structure and associated functions for dynamic string handling.
 *
 * This implementation uses Small String Optimization (SSO) to avoid heap allocation
 * for strings up to SB_BUFFER_INITIAL_CAP size.
 */
#ifndef SB_BUFFER_H
#define SB_BUFFER_H

/* Includes */
#include <stddef.h> /* For size_t */
#include <string.h> /* For strlen */
#include <stdint.h> /* For uint32_t */
#include <stdio.h> /* For fprintf (used in debug macro) */


/* C++ compatibility wrapper */
#ifdef __cplusplus
extern "C" {
#endif

/* --- Constants and Types --- */

/*! @brief Initial capacity for the internal stack buffer. */
#ifndef SB_BUFFER_INITIAL_CAP
#define SB_BUFFER_INITIAL_CAP 256
#endif

/*! @brief Unique value used to identify a valid Buffer instance. */
#define SB_BUFFER_MAGIC 0xBABECAFE

/*!
 * @struct Buffer
 * @brief Structure representing a dynamic string buffer with Small String Optimization (SSO).
 */
typedef struct {
    /*! @var uint32_t magic */
    uint32_t magic;      /* Signature for validity check */
    char init[SB_BUFFER_INITIAL_CAP];
    char *str;
    size_t len;
    size_t cap;
} SB_Buffer;


/* --- Function Prototypes --- */

/**
 * @brief Initializes the Buffer structure.
 *
 * Sets the buffer to the empty string state, utilizing the internal stack array
 * (Small String Optimization). Also sets the magic number for validity checks.
 *
 * @param b Pointer to the Buffer structure.
 * @return int 1 on success, 0 if the input pointer is NULL.
 */
int sb_buffer_init(SB_Buffer *b);

/*!
 * @brief Checks if the given pointer is a valid, initialized Buffer structure.
 * Verifies that the pointer is not NULL and that the structure contains the correct magic number.
 * @param b Pointer to the Buffer structure.
 * @return int 1 if valid, 0 otherwise.
 */
int sb_buffer_isbuffer(SB_Buffer *b);

/*!
 * @brief Releases memory allocated on the heap, if the buffer expanded beyond the initial capacity.
 * @param b Pointer to the Buffer structure.
 * @return int 1 on success, 0 if the pointer is invalid.
 */
int sb_buffer_finalize(SB_Buffer *b);

/*!
 * @brief Returns the raw C string pointer.
 * @param b Pointer to the Buffer structure.
 * @return const char* Pointer to the null-terminated string, or NULL if the pointer is invalid.
 */
const char *sb_buffer_get_str(SB_Buffer *b);

/*!
 * @brief Adds a string of known length to the buffer.
 *
 * Resizes the buffer if required using a doubling strategy.
 * @param b Pointer to the Buffer structure.
 * @param s Pointer to the source string.
 * @param len Length of the source string to append.
 * @return int 1 on success, 0 on failure (memory allocation failed or invalid input).
 */
int sb_buffer_add_lstring(SB_Buffer *b, const char *s, size_t len);

/*!
 * @brief Resets the buffer to the initial stack state, freeing any heap memory.
 *
 * This function performs a "hard reset" (frees heap memory and resets capacity).
 * @param b Pointer to the Buffer structure.
 * @return int 1 on success, 0 if the pointer is invalid.
 */
int sb_buffer_reset(SB_Buffer *b);

/*!
 * @brief Clears the buffer content without freeing allocated heap memory.
 *
 * This function performs a "soft reset" (keeps memory allocated for reuse).
 * @param b Pointer to the Buffer structure.
 * @return int 1 on success, 0 if the pointer is invalid.
 */
int sb_buffer_clear(SB_Buffer *b);


/**
 * @brief Copies the content from the source buffer to the destination buffer.
 * @note This function guarantees that the destination buffer will be an independent copy.
 * @param src Pointer to the source SB_Buffer (must be initialized).
 * @param dest Pointer to the destination SB_Buffer (must be initialized).
 * @return int 1 on success, 0 on memory allocation failure or invalid input.
 */
int sb_buffer_copy(SB_Buffer *src, SB_Buffer *dest);


/*!
 * @brief Returns the current length of the string content in the buffer.
 * @param b Pointer to the SB_Buffer structure.
 * @return size_t The current length of the content (excluding the null terminator), or 0 if the pointer is invalid.
 */
size_t sb_buffer_get_len(SB_Buffer *b);


/* --- Convenience Macros --- */

/**
 * @brief Safety macro that verifies if the pointer is valid (not NULL) and if the struct has been initialized
 * by checking its magic number.
 * * Examples:
 * (void) -->  CHECK_SB_BUFFER_POINTER_RET(b, );
 * (int)  -->  CHECK_SB_BUFFER_POINTER_RET(b, 0);
 * (Ptr)  -->  CHECK_SB_BUFFER_POINTER_RET(b, NULL); 
 */
#define CHECK_SB_BUFFER_POINTER_RET(b, ret) \
    do { \
        if ( (b) == NULL || (b)->magic != SB_BUFFER_MAGIC) { \
            /* In production, consider removing fprintf(stderr, ...) for extreme speed */ \
            fprintf(stderr, "SB_Buffer Error: Invalid or uninitialized buffer pointer. File: %s, Line: %d\n", __FILE__, __LINE__); \
            return ret; \
        } \
    } while(0)
    

/*!
 * @brief Adds a null-terminated string (calculates length via strlen).
 * @param b The Buffer pointer.
 * @param s The null-terminated string pointer.
 */
#define sb_buffer_add_string(b, s) sb_buffer_add_lstring(b, s, strlen(s))

/*!
 * @brief Adds a string literal (calculates length via sizeof at compile time).
 * @note Only works with string literals ("...").
 * @param b The Buffer pointer.
 * @param literal The string literal.
 */
#define sb_buffer_add_literal(b, literal) sb_buffer_add_lstring(b, literal, (sizeof(literal)-1))

/* Help to create and initialize the SB_Buffer
 * eg. SB_Buffer SB_INIT(b);   
 * is the same as: SBBuffer b; 
 *                 sb_buffer_init(&b);
 * 
 * Use it only inside functions!
 */
#define SB_INIT(b) b;\
   sb_buffer_init((&b))


/* Close C++ compatibility wrapper */
#ifdef __cplusplus
}
#endif

#endif /* SB_BUFFER_H */
