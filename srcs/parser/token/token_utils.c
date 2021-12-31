/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/14 15:52:18 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/14 15:52:18 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

/* subset of tokens that fit into one char */
static t_token_map	g_map[] = {
	(t_token_map){.type = TOK_DOLLAR, "$"},
	(t_token_map){.type = TOK_QUOTE, "\'"},
	(t_token_map){.type = TOK_DQUOTE, "\""},
	(t_token_map){.type = TOK_SPACE, " "},
	(t_token_map){.type = TOK_PIPE, "|"},
	(t_token_map){.type = TOK_LESS, "<"},
	(t_token_map){.type = TOK_GREAT, ">"},
	(t_token_map){-1, NULL}
};

bool	token_is_word_end(int c)
{
	int	idx;

	if (!c)
		return (true);
	idx = 0;
	while (g_map[idx].c)
	{
		if (c == *g_map[idx].c)
			return (true);
		idx++;
	}
	return (false);
}

t_token	*get_next_token(t_lexer *lexer)
{
	t_token	*token;
	int		idx;

	idx = 0;
	while (g_map[idx].c)
	{
		if (lexer->input_string[lexer->idx] == *g_map[idx].c)
		{
			token = token_new_val(g_map[idx].type, g_map[idx].c);
			lexer->idx++;
			return (token);
		}
		idx++;
	}
	return (token_new_word(lexer));
}

/*
** quote_is_type() is used to prevent
** remove_spaces() from removing spaces
** in between quotes of the same type.
*/

bool	quote_is_type(bool match_type, t_quote *quote, t_token_type type)
{
	bool	type_matches;
	bool	is_quote_type;

	type_matches = (quote->type == type);
	is_quote_type = (type == TOK_QUOTE || type == TOK_DQUOTE);
	if (!match_type && is_quote_type)
	{
		quote->type = type;
		return (true);
	}
	return (match_type && type_matches && is_quote_type);
}

void	token_display_stdout(t_list *tokens) // TODO not norm
{
	printf("[list size: %d]\n[type  :  token]\n", ft_lstsize(tokens));
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		t_token	*token = node->content;
		if (token->type == TOK_WORD)
			printf("\"word\"");
		if (token->type == TOK_MERGE)
			printf("\"merge\"");
		for (int idx = 0; idx < (int)(sizeof(g_map) / sizeof(t_token_map)); idx++)
		{
			if (g_map[idx].type == token->type)
				printf("\"%4s\"", g_map[idx].c);
		}
		if (token->type == TOK_DGREAT)
			printf("\"%4s\"", ">>");
		if (token->type == TOK_DLESS)
			printf("\"%4s\"", "<<");
		printf(" | \"%s\"\n", token->token);
	}
}
