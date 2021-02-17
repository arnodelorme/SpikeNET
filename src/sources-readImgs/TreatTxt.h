// Copyright (C) 1997-2002 Arnaud Delorme, arno@salk.edu
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

// $Log: TreatTxt.h,v $

#include <stdlib.h>
#include <string.h>

#ifndef _TREATTXT
#define _TREATTXT

/// This class help to create fields from a text line
/// It can not work by itself

   class TreatTxt
   {
   	protected:
   		
   		int		oper[10];		// operators and fields
   		char		*value[10];	// values
   		int		nbOper;		// number of operator
		
	public :
		 
		/// create an empty object 		
   		TreatTxt() { nbOper = 0; }
   		
   		/// add an operator to the list
		void addOper( int codeOper, char *valueOper)
		{ oper[ nbOper ] = codeOper; if (valueOper != NULL) { value[ nbOper ] = new char[strlen(valueOper)+1]; strcpy( value[ nbOper ], valueOper); } nbOper++; }
		
		/// get the index  for a definite operator
		int getIndex( int oper0) {
			for (int i=0; i< nbOper; i++)
				if ( oper[i] == -1) return i;
			return -1;
		}

		/// get the value for a definite operator
		char *getValue( int oper0) {
			int res = getIndex( oper0 );
			if (res != -1) return value[res]; else return NULL;
		}
};
#endif