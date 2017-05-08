/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accept_new.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjones <sjones@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/07 21:15:16 by sjones            #+#    #+#             */
/*   Updated: 2017/05/07 21:17:24 by sjones           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prox_eyes.h"

void	accept_new(t_prox *pr)
{
	pr->newsockfd = accept(pr->sockfd, (struct sockaddr*)&pr->cli_addr,
			(socklen_t*)&pr->clilen);
	if (pr->newsockfd < 0)
		error("Problem accepting connection");
}

void	error(char *msg)
{
	printf("ERRORIZING THE COUNTRYSIDE");
	perror(msg);
	exit(0);
}

void	grab_host(t_prox *pr)
{
	strcpy(pr->t1, pr->t2);
	pr->flag = 0;
	pr->i = 7;
	while (pr->i < strlen(pr->t2))
	{
		if (pr->t2[pr->i] == ':')
		{
			pr->flag = 1;
			break ;
		}
		pr->i++;
	}
	pr->temp = strtok(pr->t2, "//");
	if (pr->flag == 0)
	{
		pr->port = 80;
		pr->temp = strtok(NULL, "/");
	}
	else
		pr->temp = strtok(NULL, ":");
	sprintf(pr->t2, "%s", pr->temp);
	printf("host = %s", pr->t2);
	pr->host = gethostbyname(pr->t2);
}

void	init_child(t_prox *pr)
{
	pr->flag = 1;
	pr->port = 0;
	pr->temp = NULL;
	bzero((char*)pr->buffer, 500);
	recv(pr->newsockfd, pr->buffer, 500, 0);
	sscanf(pr->buffer, "%s %s %s", pr->t1, pr->t2, pr->t3);
}

int		main(int ac, char **av)
{
	t_prox pr;

	if (ac != 2)
		error("./prox_eyes <port>");
	init_prox(&pr, av);
	while (1)
	{
		accept_new(&pr);
		pr.pid = fork();
		if (pr.pid == 0)
			child_fun(&pr);
		else
			close(pr.newsockfd);
	}
	return (0);
}
