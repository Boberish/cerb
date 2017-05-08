/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prox_eyes.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjones <sjones@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/07 18:05:53 by sjones            #+#    #+#             */
/*   Updated: 2017/05/07 21:47:40 by sjones           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROX_EYES_H
# define PROX_EYES_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>
# include <errno.h>

typedef struct				s_prox
{
	struct sockaddr_in		addr_in;
	struct sockaddr_in		cli_addr;
	struct sockaddr_in		serv_addr;
	struct sockaddr_in		host_addr;
	struct hostent			*host;
	char					buffer[510];
	char					t1[300];
	char					t2[300];
	char					t3[10];
	char					*temp;
	int						sockfd;
	int						sockfd1;
	int						newsockfd;
	int						newsockfd1;
	int						clilen;
	int						flag;
	int						port;
	int						n;
	int						i;
	pid_t					pid;
}							t_prox;

void						error(char *msg);
void						init_prox(t_prox *pr, char **av);
void						accept_new(t_prox *pr);
void						child_fun(t_prox *pr);
void						exec_request(t_prox *pr);
void						grab_host(t_prox *pr);
void						http_work(t_prox *pr);
void						init_child(t_prox *pr);
void						put_host(t_prox *pr);
void						welcome(void);

#endif
