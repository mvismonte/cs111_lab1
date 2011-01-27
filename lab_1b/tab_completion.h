/*
 *  tab_completion.h
 *  lab_1
 *
 *  Created by Mark Vismonte & Timothy Wang on 1/26/11.
 *  Copyright 2011 University of California, Los Angeles. All rights reserved.
 *
 */
#ifndef TAB_COMPLETION_H
#define TAB_COMPLETION_H

char **
command_completion(char *str, int start, int end);

char*
command_matcher(char *str, int state);

#endif