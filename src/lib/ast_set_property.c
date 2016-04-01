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


struct set_property
{
    cypher_astnode_t _astnode;
    const cypher_astnode_t *property;
    const cypher_astnode_t *expression;
};


static ssize_t detailstr(const cypher_astnode_t *self, char *str, size_t size);


const struct cypher_astnode_vt cypher_set_property_astnode_vt =
    { .name = "set property",
      .detailstr = detailstr,
      .free = cypher_astnode_free };


cypher_astnode_t *cypher_ast_set_property(const cypher_astnode_t *property,
        const cypher_astnode_t *expression, cypher_astnode_t **children,
        unsigned int nchildren, struct cypher_input_range range)
{
    REQUIRE(cypher_astnode_instanceof(property,
                CYPHER_AST_PROPERTY_OPERATOR), NULL);
    REQUIRE(expression != NULL, NULL);

    struct set_property *node = calloc(1, sizeof(struct set_property));
    if (node == NULL)
    {
        return NULL;
    }
    if (cypher_astnode_init(&(node->_astnode), CYPHER_AST_SET_PROPERTY,
            children, nchildren, range))
    {
        free(node);
        return NULL;
    }
    node->property = property;
    node->expression = expression;
    return &(node->_astnode);
}


ssize_t detailstr(const cypher_astnode_t *self, char *str, size_t size)
{
    REQUIRE(cypher_astnode_instanceof(self, CYPHER_AST_SET_PROPERTY), -1);
    struct set_property *node =
            container_of(self, struct set_property, _astnode);

    return snprintf(str, size, "@%u = @%u", node->property->ordinal,
            node->expression->ordinal);
}