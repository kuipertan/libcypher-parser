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


struct node_index_lookup
{
    cypher_astnode_t _astnode;
    const cypher_astnode_t *identifier;
    const cypher_astnode_t *index;
    const cypher_astnode_t *prop_name;
    const cypher_astnode_t *lookup;
};


static ssize_t detailstr(const cypher_astnode_t *self, char *str, size_t size);


const struct cypher_astnode_vt cypher_node_index_lookup_astnode_vt =
    { .name = "node index lookup",
      .detailstr = detailstr,
      .free = cypher_astnode_free };


cypher_astnode_t *cypher_ast_node_index_lookup(
        const cypher_astnode_t *identifier, const cypher_astnode_t *index,
        const cypher_astnode_t *prop_name, const cypher_astnode_t *lookup,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range)
{
    REQUIRE(cypher_astnode_instanceof(identifier, CYPHER_AST_IDENTIFIER), NULL);
    REQUIRE(cypher_astnode_instanceof(index, CYPHER_AST_INDEX_NAME), NULL);
    REQUIRE(cypher_astnode_instanceof(prop_name, CYPHER_AST_PROP_NAME), NULL);
    REQUIRE(lookup != NULL, NULL);

    struct node_index_lookup *node = calloc(1, sizeof(struct node_index_lookup));
    if (node == NULL)
    {
        return NULL;
    }
    if (cypher_astnode_init(&(node->_astnode), CYPHER_AST_NODE_INDEX_LOOKUP,
            children, nchildren, range))
    {
        free(node);
        return NULL;
    }
    node->identifier = identifier;
    node->index = index;
    node->prop_name = prop_name;
    node->lookup = lookup;
    return &(node->_astnode);
}


ssize_t detailstr(const cypher_astnode_t *self, char *str, size_t size)
{
    REQUIRE(cypher_astnode_instanceof(self, CYPHER_AST_NODE_INDEX_LOOKUP), -1);
    struct node_index_lookup *node =
            container_of(self, struct node_index_lookup, _astnode);
    return snprintf(str, size, "@%u = node:@%u(@%u = @%u)",
                node->identifier->ordinal, node->index->ordinal,
                node->prop_name->ordinal, node->lookup->ordinal);
}