/* Copyright (c) 2008, 2009, 2010, 2016, 2017 Nicira, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UUID_H
#define UUID_H 1

#include "util.h"
#include "openvswitch/uuid.h"

#ifdef __cplusplus
extern "C" {
#endif

/* An initializer or expression for an all-zero UUID. */
#define UUID_ZERO ((struct uuid) { .parts = { 0, 0, 0, 0 } })

/* Formats a UUID as a string, in the conventional format.
 *
 * Example:
 *   struct uuid uuid = ...;
 *   printf("This UUID is "UUID_FMT"\n", UUID_ARGS(&uuid));
 *
 */
#define UUID_LEN 36
#define UUID_FMT "%08x-%04x-%04x-%04x-%04x%08x"
#define UUID_ARGS(UUID)                             \
    ((unsigned int) ((UUID)->parts[0])),            \
    ((unsigned int) ((UUID)->parts[1] >> 16)),      \
    ((unsigned int) ((UUID)->parts[1] & 0xffff)),   \
    ((unsigned int) ((UUID)->parts[2] >> 16)),      \
    ((unsigned int) ((UUID)->parts[2] & 0xffff)),   \
    ((unsigned int) ((UUID)->parts[3]))

/* Returns a hash value for 'uuid'.  This hash value is the same regardless of
 * whether we are running on a 32-bit or 64-bit or big-endian or little-endian
 * architecture. */
static inline size_t
uuid_hash(const struct uuid *uuid)
{
    return uuid->parts[0];
}

/* Returns true if 'a == b', false otherwise. */
static inline bool
uuid_equals(const struct uuid *a, const struct uuid *b)
{
    return (a->parts[0] == b->parts[0]
            && a->parts[1] == b->parts[1]
            && a->parts[2] == b->parts[2]
            && a->parts[3] == b->parts[3]);
}

/* Returns the first 'n' hex digits of 'uuid', for 0 < 'n' <= 8.
 *
 * This is useful for displaying a few leading digits of the uuid, e.g. to
 * display 4 digits:
 *     printf("%04x", uuid_prefix(uuid, 4));
 */
static inline unsigned int
uuid_prefix(const struct uuid *uuid, int digits)
{
    return (uuid->parts[0] >> (32 - 4 * digits));
}

/* Returns a string representation of a UUID.
 *
 * The string is allocated on the heap. Ownership of the string is
 * transferred to the caller.
 */
static inline char *
uuid_to_string(const struct uuid *uuid)
{
    char *data = xmalloc(UUID_LEN + 1);

    snprintf(data, UUID_LEN + 1, UUID_FMT, UUID_ARGS(uuid));
    return data;
}

void uuid_init(void);
void uuid_generate(struct uuid *);
struct uuid uuid_random(void);
void uuid_zero(struct uuid *);
bool uuid_is_zero(const struct uuid *);
int uuid_compare_3way(const struct uuid *, const struct uuid *);
bool uuid_from_string(struct uuid *, const char *);
bool uuid_from_string_prefix(struct uuid *, const char *);
int uuid_is_partial_string(const char *);
int uuid_is_partial_match(const struct uuid *, const char *match);
void uuid_set_bits_v4(struct uuid *);

#ifdef __cplusplus
}
#endif

#endif /* uuid.h */
