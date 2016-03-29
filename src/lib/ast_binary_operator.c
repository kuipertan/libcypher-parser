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
#include "operators.h"
#include "util.h"
#include <assert.h>


struct binary_operator
{
    cypher_astnode_t _astnode;
    const cypher_operator_t *op;
    const cypher_astnode_t *arg1;
    const cypher_astnode_t *arg2;
};


static ssize_t detailstr(const cypher_astnode_t *self, char *str, size_t size);


const struct cypher_astnode_vt cypher_binary_operator_astnode_vt =
    { .name = "binary operator",
      .detailstr = detailstr,
      .free = cypher_astnode_free };


cypher_astnode_t *cypher_ast_binary_operator(const cypher_operator_t *op,
        const cypher_astnode_t *arg1, const cypher_astnode_t *arg2,
        cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range)
{
    REQUIRE(op != NULL, NULL);
    REQUIRE(arg1 != NULL, NULL);
    REQUIRE(arg2 != NULL, NULL);

    struct binary_operator *node = calloc(1, sizeof(struct binary_operator));
    if (node == NULL)
    {
        return NULL;
    }
    if (cypher_astnode_init(&(node->_astnode), CYPHER_AST_BINARY_OPERATOR,
            children, nchildren, range))
    {
        free(node);
        return NULL;
    }
    node->op = op;
    node->arg1 = arg1;
    node->arg2 = arg2;
    return &(node->_astnode);
}


ssize_t detailstr(const cypher_astnode_t *self, char *str, size_t size)
{
    REQUIRE(cypher_astnode_instanceof(self, CYPHER_AST_BINARY_OPERATOR), -1);
    struct binary_operator *node =
        container_of(self, struct binary_operator, _astnode);
    return snprintf(str, size, "@%u %s @%u", node->arg1->ordinal,
                node->op->str, node->arg2->ordinal);
}
