/* vi:set ts=4 sw=4 expandtab:
 *
 * Copyright 2016, Chris Leishman (http://github.com/cleishm)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "../../config.h"
#include "astnode.h"
#include "util.h"
#include <assert.h>


struct set
{
    cypher_astnode_t _astnode;
    const cypher_astnode_t **items;
    unsigned int nitems;
};


static ssize_t detailstr(const cypher_astnode_t *self, char *str, size_t size);
static void set_free(cypher_astnode_t *self);


const struct cypher_astnode_vt cypher_set_astnode_vt =
    { .name = "SET",
      .detailstr = detailstr,
      .free = set_free };


cypher_astnode_t *cypher_ast_set(cypher_astnode_t * const *items,
        unsigned int nitems, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range)
{
    REQUIRE(nitems > 0, NULL);
    REQUIRE(items != NULL, NULL);

    struct set *node = calloc(1, sizeof(struct set));
    if (node == NULL)
    {
        return NULL;
    }
    if (cypher_astnode_init(&(node->_astnode), CYPHER_AST_SET,
            children, nchildren, range))
    {
        goto cleanup;
    }
    node->items = mdup(items, nitems * sizeof(cypher_astnode_t *));
    if (node->items == NULL)
    {
        goto cleanup;
    }
    node->nitems = nitems;
    return &(node->_astnode);

    int errsv;
cleanup:
    errsv = errno;
    free(node);
    errno = errsv;
    return NULL;
}


void set_free(cypher_astnode_t *self)
{
    struct set *node = container_of(self, struct set, _astnode);
    free(node->items);
    cypher_astnode_free(self);
}


ssize_t detailstr(const cypher_astnode_t *self, char *str, size_t size)
{
    REQUIRE(cypher_astnode_instanceof(self, CYPHER_AST_SET), -1);
    struct set *node = container_of(self, struct set, _astnode);

    strncpy(str, "items=", size);
    if (size > 0)
    {
        str[size-1] = '\0';
    }
    size_t n = 6;

    ssize_t r = snprint_sequence(str+n, (n < size)? size-n : 0,
            node->items, node->nitems);
    if (r < 0)
    {
        return -1;
    }
    n += r;
    return n;
}