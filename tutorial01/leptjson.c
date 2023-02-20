#include "leptjson.h"
#include <assert.h> /* assert() */
#include <stdlib.h> /* NULL */

#define EXPECT(c, ch)               \
    do                              \
    {                               \
        assert(*(c->json) == (ch)); \
        (c->json)++;                \
    } while (0)
/* json数组内容与ch是否相等，若一致则后移 */
typedef struct
{
    const char *json;
} lept_context;

static void lept_parse_whitespace(lept_context *c)
{
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

static int lept_parse_null(lept_context *c, lept_value *v)
{
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_NULL;
    return LEPT_PARSE_OK;
}

static int lept_parse_value(lept_context *c, lept_value *v)
{
    const char *temp=c->json; /* 记录c->json  在switch语句之前恢复，不然会出bug*/
    const char *p = c->json;
    if (*p != '\0')
    {/*第一个空白后有字符*/

       
        while (*p != '\0' && *p != ' ' && *p != '\t' && *p != '\n' && *p != '\r')
        {   
            p++;
        }
        c->json = p;/*跳过字符*/
        lept_parse_whitespace(c);/*跳过字符后的空白*/
        p = c->json;
        if (*p != '\0')
        return LEPT_PARSE_ROOT_NOT_SINGULAR;
        c->json=temp;
    }  /*空白-字符-空白 之后还有其他字符返回 LEPT_PARSE_ROOT_NOT_SINGULAR*/

    
    switch (*c->json)
    {
    case 'n':
        return lept_parse_null(c, v);
    case '\0':
        return LEPT_PARSE_EXPECT_VALUE;
    default:
        return LEPT_PARSE_INVALID_VALUE;
    }
}

int lept_parse(lept_value *v, const char *json)
{
    lept_context c;
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;
    lept_parse_whitespace(&c);
    return lept_parse_value(&c, v);
}

lept_type lept_get_type(const lept_value *v)
{
    assert(v != NULL);
    return v->type;
}
