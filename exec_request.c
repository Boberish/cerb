/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_request.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaylor <sjones@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/07 21:21:04 by jaylor            #+#    #+#             */
/*   Updated: 2017/05/07 21:33:23 by jaylor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prox_eyes.h"

void	exec_request(t_prox *pr)
{
	bzero((char*)&pr->host_addr, sizeof(pr->host_addr));
	pr->host_addr.sin_port = htons(pr->port);
	pr->host_addr.sin_family = AF_INET;
	bcopy((char*)pr->host->h_addr, (char*)&pr->host_addr.sin_addr.s_addr,
			pr->host->h_length);
	pr->sockfd1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	pr->newsockfd1 = connect(pr->sockfd1, (struct sockaddr*)&pr->host_addr,
			sizeof(struct sockaddr));
	sprintf(pr->buffer, "\nConnected to %s  IP - %s\n", pr->t2,
			inet_ntoa(pr->host_addr.sin_addr));
	if (pr->newsockfd1 < 0)
		error("Error in connecting to remote server");
	printf("\n%s\n", pr->buffer);
	bzero((char*)pr->buffer, sizeof(pr->buffer));
	if (pr->temp != NULL)
		sprintf(pr->buffer, "GET /%s %s\r\nHost: "
				"%s\r\nConnection: close\r\n\r\n", pr->temp, pr->t3, pr->t2);
	else
		sprintf(pr->buffer, "GET / %s\r\nHost: "
				"%s\r\nConnection: close\r\n\r\n", pr->t3, pr->t2);
}

void	put_host(t_prox *pr)
{
	if (pr->flag == 1)
	{
		pr->temp = strtok(NULL, "/");
		pr->port = atoi(pr->temp);
	}
	strcat(pr->t1, "^]");
	pr->temp = strtok(pr->t1, "//");
	pr->temp = strtok(NULL, "/");
	if (pr->temp != NULL)
		pr->temp = strtok(NULL, "^]");
	printf("\npath = %s\nPort = %d\n", pr->temp, pr->port);
}

void	child_fun(t_prox *pr)
{
	init_child(pr);
	if (((strncmp(pr->t1, "GET", 3) == 0)) &&
			(strncmp(pr->t2, "http://", 7) == 0) &&
			((strncmp(pr->t3, "HTTP/1.1", 8) == 0) ||
			(strncmp(pr->t3, "HTTP/1.0", 8))))
		http_work(pr);
	else
		send(pr->newsockfd, "400 : BAD REQUEST\nONLY HTTP REQUESTS ALLOWED",
				18, 0);
	close(pr->sockfd1);
	close(pr->newsockfd);
	close(pr->sockfd);
	exit(0);
}

void	http_work(t_prox *pr)
{
	grab_host(pr);
	put_host(pr);
	exec_request(pr);
	pr->n = send(pr->sockfd1, pr->buffer, strlen(pr->buffer), 0);
	printf("\n%s\n", pr->buffer);
	if (pr->n < 0)
		error("Error writing to socket");
	else
	{
		while (pr->n > 0)
		{
			bzero((char*)pr->buffer, 500);
			pr->n = recv(pr->sockfd1, pr->buffer, 500, 0);
			if (!(pr->n <= 0))
				send(pr->newsockfd, pr->buffer, pr->n, 0);
		}
	}
}

void	init_prox(t_prox *pr, char **av)
{
	bzero((char*)&pr->serv_addr, sizeof(pr->serv_addr));
	bzero((char*)&pr->cli_addr, sizeof(pr->cli_addr));
	pr->serv_addr.sin_family = AF_INET;
	pr->serv_addr.sin_port = htons(atoi(av[1]));
	pr->serv_addr.sin_addr.s_addr = INADDR_ANY;
	pr->sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (pr->sockfd < 0)
		error("Problem initializing socket");
	if (bind(pr->sockfd, (struct sockaddr*)&pr->serv_addr,
				sizeof(pr->serv_addr)) < 0)
		error("Error on binding");
	listen(pr->sockfd, 50);
	pr->clilen = sizeof(pr->cli_addr);
}
