/* getopts.c - Command line argument parser
 *
 * Whom: Steve Mertz <steve@dragon-ware.com>
 * Date: 20010111
 * Why:  Because I couldn't find one that I liked. So I wrote this one.
 *
*/
/*
 * Copyright (c) 2001, 2002, Steve Mertz <steve@dragon-ware.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of Dragon Ware Computer Services nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getopts.h"

int option_index = 1;

/* int getopts()
 *
 * Returns: 0  - No arguements to parse
 *          #  - The number in the struct for the matched arg.
 *
*/
int getopts(int argc, char **argv, struct options opts[], char *args)
{
  char *cmd;
  int count1, count2, opts_s;

  if (argc == 1 || option_index == argc)
    return 0;
  opts_s = sizeof(opts) / sizeof(opts[0]);

/* Search for '-h' or '--help' first.  Then we can just exit */
  for (count1 = 1; count1 < argc; count1++)
    {
      if (!strcmp(argv[count1], "-h") || !strcmp(argv[count1], "--help"))
        {
useage:
          cmd = (char *)calloc(1, 128);
          printf("Usage: %s [options]\n\n", argv[0]);
          printf("  --help,\t-h\t\t\tDisplays this information\n");
          for (count2 = 0; opts[count2].description; count2++)
            {
              if (opts[count2].name && opts[count2].shortName)
    {
                  if (opts[count2].args)
                    sprintf(cmd, "--%s,\t-%s <args>\t\t", opts[count2].name, opts[count2].shortName);
                  else
                    sprintf(cmd, "--%s,\t-%s\t\t\t", opts[count2].name, opts[count2].shortName);
    }
              else if (opts[count2].name)
    {
                  if (opts[count2].args)
                    sprintf(cmd, "--%s <args>\t\t\t", opts[count2].name);
                  else
                    sprintf(cmd, "--%s\t\t\t", opts[count2].name);
    }
              else if (opts[count2].shortName)
    {
                  if (opts[count2].args)
                    sprintf(cmd, "\t\t-%s <args>\t\t", opts[count2].shortName);
      else
                    sprintf(cmd, "\t\t-%s\t\t\t", opts[count2].shortName);
    }
              printf("  %s%s\n", cmd, opts[count2].description);
            }
          exit(0);
        }
    }
/* End of -h --help section */

  if (option_index <= argc)
    {
      for (count1 = 0; opts[count1].description; count1++)
        {
          if ((opts[count1].name && !strcmp(opts[count1].name,
              (argv[option_index]+2))) ||
              (opts[count1].shortName && !strcmp(opts[count1].shortName,
              (argv[option_index]+1))))
            {
              if (opts[count1].args)
                {
                  int arg_s;
                  option_index++;
                  if (option_index >= argc)
                    goto useage;
/* This grossness that follows is to supporte having a '-' in the
   argument.  It's all squished together like this because I use
   an 80 char wide screen for my coding.  If you don't like it, help
   yourself to fixing it so you do. */
                  if (*argv[option_index] == '-')
                    {
                      int optionSeeker;
                      for (optionSeeker = 0; opts[optionSeeker].description;
                           optionSeeker++)
                        {
                          if ((opts[optionSeeker].name &&
                               !strcmp(opts[optionSeeker].name,
                                       (argv[option_index]+2))) ||
                               (opts[optionSeeker].shortName &&
                               !strcmp(opts[optionSeeker].shortName,
                                       (argv[option_index]+1))))
                            {
                              goto useage;
                            }
                        }
/* End of gross hack for supporting '-' in arguments. */
                    }
                  arg_s = strlen(argv[option_index]);
/*                  strncpy(args, argv[option_index], arg_s);*/
                  strcpy ( args, argv[option_index] );
                }
              option_index++;
              return opts[count1].number;
            }
        }
    }
  return 0;
}







