/*
___________________________________________________________________

This file is a part of GPoSTTL: An enhanced version of Brill's
rule-based Parts-of-Speech Tagger with built-in Tokenizer and
Lemmatizer by Golam Mortuza Hossain <gmhossain at gmail.com>.
This file can be redistributed and/or modified under the terms of
either the license of GPoSTTL or optionally under GPL as below.
___________________________________________________________________
 
Copyright (C) 2007-2008, Golam Mortuza Hossain <gmhossain@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU General Public License for more details.
 
You should have received a copy of the GNU General Public
License along with this program; if not, write to the Free
Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.
__________________________________________________________________

*/

#include <stdio.h>
#include <stdlib.h>

#include "tagger_controller.h"
#include "lex.h"
#include "darray.h"
#include "useful.h"
#include "memory.h"
#include "tagger.h"
#include "tokenizer.h"
#include "enhance_penntag.h"

#define  INPUTLEN  4*10240


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

int initialize_tagger( char *envp )
{
	char basepath[250] = "../data" ;
	int i ;
	
	if ( envp != NULL ) 
	{
		sprintf( basepath,"%s", envp); 
		i = _initialize( basepath );
		if ( i == 0 ) return 0;
	}
	
	#ifdef HAVE_CONFIG_H
	sprintf( basepath,"%s/%s", DATADIR, PACKAGE); 
	i = _initialize( basepath );
	if ( i == 0 ) return 0;
	#endif

	
	return -1 ; 
}




/* check for sgml tag and then tag */
int check_and_tag ( char *ptr0, int  enhance_penntag)
{
	char  ch, *ptr1, *ptr2, *ptr3, *ptr4, *output ;
	char sgml1[2]= "<", sgml2[2]= ">", space[2]= " " ;

	ptr4 = ptr0 ;	
	while ( (ptr1 = strstr( ptr4, sgml1))!= NULL )
	{
	
	ptr2 = strstr( ptr1, sgml2) ;
	ptr3 = strstr( ptr1, space) ;
		
	/* Check for sgml tag <XXXX> */
	if (ptr2 && (!ptr3 || ptr3 > ptr2 ) )  
		{
		ch = *ptr1 ; *ptr1 = '\0' ;

		output = tag(Tokenizer(ptr0), enhance_penntag);
		fprintf( stdout, "%s", output) ;
		free(output) ; 
		
		*ptr1 = ch ; ptr2++ ; ch = *ptr2 ; *ptr2 = '\0' ;
		fprintf ( stdout, "%s\n", ptr1 ); 
		*ptr2 = ch ; ptr0 = ptr2 ; ptr4 = ptr0;
		}
	else	
		{
		ptr4 = ptr1; ptr4++ ;	
		}
	}

	output = tag(Tokenizer(ptr0), enhance_penntag);
	fprintf( stdout, "%s", output) ;
	free(output) ; 
		
	return 0 ;		
}


/* Main */	
int main (int argc, char *argv[], char *envp[])
{
	int  enhance_penntag = 1 ;
	int  ch, ch2, i=0, n=1, j=1, silent=0, version=0 ;
	char *buf, *inptr, *ptr0, *ptr1, *ptr2, *ptr3 ;
	FILE *fp;

	fp = stdin ;

/* Read command line options */
	if ( argc > 4 )
	{
	fprintf( stderr, "error!!usage: %s [OPTION] [FILE or STDIN]\n",argv[0]);
	exit(1) ;	
	}

	for (i=1;  i< argc; i++)
	{
		if (argv[i][0] == '-') 
		{
		if ( strcmp( argv[i],"--brill-mode")==0 ) enhance_penntag = 0;
		else if ( strcmp( argv[i],"--tree-tagger-mode")==0 ) 
				enhance_penntag = 1;
		else if ( strcmp( argv[i],"--silent")==0 ) silent = 1;
		else if ( strcmp( argv[i],"--version")==0 ) version = 1;
		}
		else 
		{
		fp = fopen(argv[i],"r") ;
		if ( fp == NULL )
			{
			fprintf( stderr, "error!! File %s not found\n",argv[i]);
			exit(1) ;	
			}
		else	break ;	
		}
	}
/* Print tagger info and quit */
	if ( version )
	{
	fprintf( stdout, "GPoSTTL %s beta", VERSION); 
	if ( fp ) fclose (fp) ;
	exit(0) ;	
	}

/*	initialize */
	if ( initialize_tagger( getenv("GPOSTTL_DATA_DIR")) !=0 )
	{
	fprintf( stderr, "Initialization failed! Datafiles are missing. ") ; 
	fprintf( stderr, "If you have\nthe datafiles in a ");
	fprintf( stderr, "non-standard location then please set\n");
	fprintf( stderr, "the envioronment variable GPOSTTL_DATA_DIR.\n");
	exit(1) ;	
	}
	
/*	Malloc */
	inptr = (char *)malloc( sizeof(char)*INPUTLEN + 1) ; 
	if ( inptr == NULL )
		{
		fprintf( stderr, "Insufficient memory!\n");
		exit(1) ;	
		}


/* start processing */
	if ( !silent )
	fprintf( stderr, "GPoSTTL (Ver. %s): Tagging...\n", VERSION);
	
	ptr0 = inptr ;   
	ch = fgetc( fp )  ;

	while ( ch != EOF )
	{
	if ( ch == '\n' )	ch = ' ' ; /*Remove new line*/
	*inptr = ch ; inptr++ ;	n++ ; 	
	
	if ( n == INPUTLEN - 4 ) 
		{
		fprintf( stderr, "\r processing %d bytes\n",j*INPUTLEN) ; 
		j++;
		n=0; ptr3 = inptr; ptr3--; 
		*inptr = ' ';  inptr++;  *inptr = '\0';
		
		while (*ptr3 != ' ') ptr3--; 
		ptr3++; ch = *ptr3; *ptr3 = '\0' ;
	
		check_and_tag( ptr0, enhance_penntag) ;

	/* Now push remaining part in the begining */ 
		inptr = ptr0; *ptr3 = ch; strcpy(inptr, ptr3); 
		while (*inptr != '\0') { inptr++ ; n++; }
		inptr--; n--;
		}

	ch = fgetc(fp) ; 
	}
	*inptr = ' ' ; inptr++ ;  /* one extra space for tokenizer */
	*inptr = '\0' ;	

	check_and_tag( ptr0, enhance_penntag) ;
		
/* Destroy tagger */		
	destroy_tagger (); free(ptr0) ;

	if ( !silent )
	fprintf( stderr, "GPoSTTL: Done\n");
	return 0;	
}

